/* termstr SUPPORT */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* object to manage terminal database strings */
/* version %I% last-modified %G% */


/* revision history:

	= 2004-06-24, David A­D­ Morano
	I rewrote this from scratch. The previous version of this
	program was a hack.

*/

/* Copyright © 2004 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Name:
	termstr

	Description:
	This object manages terminal database strings.

	Synopsis:
	int termstr_start(termstr *op,cchar *termtype) noex

	Arguments:
	op		object pointer
	termtype	terminal type as a descriptive string

	Returns:
	>=0		OK
	<0		error (system-return)

*******************************************************************************/

#include	<envstandards.h>	/* MUST be first to configure */
#include	<climits>
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<cstdarg>
#include	<clanguage.h>
#include	<usysbase.h>
#include	<ascii.h>
#include	<ansigr.h>
#include	<buffer.h>
#include	<termconseq.h>
#include	<nleadstr.h>
#include	<localmisc.h>

#include	"termstr.h"

#pragma		GCC dependency		"mod/libutil.ccm"

import libutil ;			/* |lenstr(3u)| + |getlenstr(3u)| */

/* local defines */

#define	TERMSTR_MAGIC	0x88726325
#define	TERMSTR_START	60
#define	TERMSTR_COLS	132
/* mask for graphic renditions */
#define	TERMSTR_GRMASK		\
	(TERMSTR_GRBOLD| TERMSTR_GRUNDER| TERMSTR_GRBLINK| TERMSTR_GRREV)

#define	TCF_MDEFAULT	0x0000	/* default */
#define	TCF_MEC		0x0001	/* erase character */
#define	TCF_MVCV	0x0002	/* cursor visibility (VT) */
#define	TCF_MACV	0x0004	/* cursor visibility (ANSI) */
#define	TCF_MSCV	0x1000	/* cursor visibility (SCREEN) */
#define	TCF_MPSF	0x0008	/* has a preferred supplimental font */
#define	TCF_MSCS94	0x0010	/* supplemental character set 94 */
#define	TCF_MSCS96	0x0020	/* supplemental character set 96 */
#define	TCF_MSD		0x0040	/* has a status display (line) */
#define	TCF_MSCS94A	0x0080	/* supplemental character set 94a */
#define	TCF_MSR		0x0100	/* has setable line-scrolling regions */
#define	TCF_MSL		0x0400	/* setable number of lines */
#define	TCF_MVCSR	0x0200	/* cursor save-restore (VT) */
#define	TCF_MACSR	0x0800	/* cursor save-restore (ANSI) */
#define	TCF_MACSRS	0x2000	/* cursor save-restore (ANSI) is screwed */

#define	TCF_MBASIC	(TCF_MSR)
#define	TCF_MVT		(TCF_MSR | TCF_MVCSR)
#define	TCF_MVTE	(TCF_MSR | TCF_MVCSR | TCF_MEC)
#define	TCF_MVTADV	\
	(TCF_MVTE) | \
	(TCF_MPSF | TCF_MSCS94 | TCF_MSCS96 | TCF_MVCV | TCF_MSD) | \
	(TCF_MSL)

#define	TCF_MSCREEN	\
	(TCF_MVTE | TCF_MVCV | TCF_MACV | TCF_MSCV | TCF_MACSR)


#define	TLEN		30		/* stage buffer length */


/* imported namespaces */

using std::nothrow ;			/* constant */


/* local typedefs */


/* external subroutines */

extern "C" {
    extern int	buffer_blanks(buffer *,int) noex ;
    extern int	buffer_backs(buffer *,int) noex ;
}


/* external variables */


/* local structures */

struct termtype {
	cchar		*name ;
	uint		flags ;
} ;

enum curtypes {
	curtype_u,
	curtype_d,
	curtype_r,
	curtype_l,
	curtype_overlast
} ;


/* forward references */

template<typename ... Args>
local int termstr_ctor(termstr *op,Args ... args) noex {
    	TERMSTR		*hop = op ;
	cnullptr	np{} ;
	int		rs = SR_FAULT ;
	if (op && (args && ...)) ylikely {
	    memclear(hop) ;
	    rs = SR_NOMEM ;
	    op->magic = 0 ;
	    op->ti = -1 ;
	    if ((op->sbp = new(nothrow) buffer) != np) ylikely {
		rs = SR_OK ;
	    }
	} /* end if (non-null) */
	return rs ;
} /* end subroutine (termstr_ctor) */

local int termstr_dtor(termstr *op) noex {
	int		rs = SR_FAULT ;
	if (op) ylikely {
	    rs = SR_OK ;
	    if (op->sbp) ylikely {
		delete op->sbp ;
		op->sbp = nullptr ;
	    }
	} /* end if (non-null) */
	return rs ;
} /* end subroutine (termstr_dtor) */

template<typename ... Args>
local int termstr_magic(termstr *op,Args ... args) noex {
	int		rs = SR_FAULT ;
	if (op && (args && ...)) ylikely {
	    rs = (op->magic == TERMSTR_MAGIC) ? SR_OK : SR_NOTOPEN ;
	}
	return rs ;
} /* end subroutine (termstr_magic) */

local int	termstr_curm	(termstr *,int,int) noex ;
local int	termstr_findterm(termstr *,cchar *) noex ;
local int	termstr_conseq	(termstr *,int,int,...) noex ;


/* local variables */

constexpr termtype	terms[] = {
	{ "sun", 0 },
	{ "vt100", (TCF_MVT) },
	{ "ansi", (TCF_MSR | TCF_MEC | TCF_MACV | TCF_MACSR | TCF_MACSRS) },
	{ "vt101", (TCF_MVTE) },
	{ "vt102", (TCF_MVTE) },
	{ "vt220", (TCF_MVTE | TCF_MSCS94) },
	{ "xterm", (TCF_MVTE) },
	{ "xterm-color", (TCF_MVTE) },
	{ "screen", (TCF_MSCREEN | TCF_MSCS94A) },
	{ "screen94a", (TCF_MSCREEN | TCF_MSCS94 | TCF_MSCS94A) },
	{ "screen96", (TCF_MSCREEN | TCF_MSCS94 | TCF_MSCS96) },
	{ "vt320", (TCF_MVTADV) },
	{ "vt330", (TCF_MVTADV) },
	{ "vt340", (TCF_MVTADV) },
	{ "vt420", (TCF_MVTADV) },
	{ "vt430", (TCF_MVTADV) },
	{ "vt440", (TCF_MVTADV) },
	{ "vt520", (TCF_MVTADV) },
	{ "vt530", (TCF_MVTADV) },
	{ "vt540", (TCF_MVTADV) },
	{ nullptr, 0 }
} ; /* end array (terms) */

constexpr char		curtypes[] = "ABCD" ;


/* exported variables */


/* exported subroutines */

int termstr_start(termstr *op,cchar *termtype) noex {
	int		rs ;
	if ((rs = termstr_ctor(op,termtype)) >= 0) ylikely {
	    rs = SR_INVALID ;
	    if (termtype[0]) ylikely {
	        if ((rs = termstr_findterm(op,termtype)) >= 0) ylikely {
		    cint	bsz = TERMSTR_START ;
	            if ((rs = buffer_start(op->sbp,bsz)) >= 0) {
	                op->magic = TERMSTR_MAGIC ;
	            }
	        } /* end if (termstr_findterm) */
	    } /* end if (valid) */
	    if (rs < 0) {
		termstr_dtor(op) ;
	    }
	} /* end if (termstr_ctor) */
	return rs ;
}
/* end subroutine (termstr_start) */

int termstr_finish(termstr *op) noex {
	int		rs ;
	int		rs1 ;
	if ((rs = termstr_magic(op)) >= 0) ylikely {
	    if (op->sbp) ylikely {
		rs1 = buffer_finish(op->sbp) ;
		if (rs >= 0) rs = rs1 ;
	    }
	    {
		rs1 = termstr_dtor(op) ;
		if (rs >= 0) rs = rs1 ;
	    }
	    op->ti = -1 ;
	    op->magic = 0 ;
	} /* end if (magic) */
	return rs ;
}
/* end subroutines (termstr_finish) */

int termstr_clean(termstr *op) noex {
	int		rs ;
	if ((rs = termstr_magic(op)) >= 0) ylikely {
	    rs = buffer_reset(op->sbp) ;
	} /* end if (magic) */
	return rs ;
}
/* end subroutines (termstr_clean) */

int termstr_write(termstr *op,cchar *bp,int bl) noex {
	int		rs ;
	int		len = 0 ;
	if ((rs = termstr_magic(op,bp)) >= 0) ylikely {
	    rs = buffer_buf(op->sbp,bp,bl) ;
	    len += rs ;
	} /* end if (magic) */
	return (rs >= 0) ? len : rs ;
}
/* end subroutines (termstr_write) */

/* write to the buffer w/ graphic-rendition */
int termstr_writegr(termstr *op,int gr,cchar *bp,int bl) noex {
	int		rs ;
	int		len = 0 ;
	if ((rs = termstr_magic(op,bp)) >= 0) ylikely {
	    cint	grmask = TERMSTR_GRMASK ;
	    bool	f_have = false ;
	    if (gr & grmask) {
	        cint	a1 = (gr & TERMSTR_GRBOLD) ? ANSIGR_BOLD : -1 ;
	        cint	a2 = (gr & TERMSTR_GRUNDER) ? ANSIGR_UNDER : -1 ;
	        cint	a3 = (gr & TERMSTR_GRBLINK) ? ANSIGR_BLINK : -1 ;
	        cint	a4 = (gr & TERMSTR_GRREV) ? ANSIGR_REV : -1 ;
	        f_have = true ;
	        rs = termstr_conseq(op,'m',4,a1,a2,a3,a4) ;
	        len += rs ;
	    }
	    if (rs >= 0) ylikely {
	        rs = buffer_buf(op->sbp,bp,bl) ;
	        len += rs ;
	    }
	    if ((rs >= 0) && gr && f_have) {
	        cint	code = ANSIGR_OFFALL ;
	        rs = termstr_conseq(op,'m',1,code,-1,-1,-1) ;
	        len += rs ;
	    }
	} /* end if (magic) */
	return (rs >= 0) ? len : rs ;
}
/* end subroutines (termstr_writegr) */

int termstr_char(termstr *op,int ch) noex {
	int		rs ;
	if ((rs = termstr_magic(op)) >= 0) ylikely {
	    rs = buffer_chr(op->sbp,ch) ;
	} /* end if (magic) */
	return rs ;
}
/* end subroutines (termstr_char) */

int termstr_get(termstr *op,cchar **rpp) noex {
	int		rs ;
	if ((rs = termstr_magic(op,rpp)) >= 0) ylikely {
	    rs = buffer_get(op->sbp,rpp) ;
	} /* end if (magic) */
	return rs ;
}
/* end subroutines (termstr_get) */

/* erase-display (ED) */
int termstr_ed(termstr *op,int type) noex {
	int		rs ;
	if ((rs = termstr_magic(op)) >= 0) ylikely {
	    /* type: 0=forward, 1=back, 2=whole */
	    if (type >= 2) type = 2 ;
	    rs = termstr_conseq(op,'J',1,type,-1,-1,-1) ;
	} /* end if (magic) */
	return rs ;
}
/* end subroutine (termstr_ed) */

/* erase-line (EL) */
int termstr_el(termstr *op,int type) noex {
	int		rs ;
	if ((rs = termstr_magic(op)) >= 0) ylikely {
	    /* 0=forward, 1=back, 2=whole */
	    if (type >= 2) type = 2 ;
	    rs = termstr_conseq(op,'K',1,type,-1,-1,-1) ;
	} /* end if (magic) */
	return rs ;
}
/* end subroutine (termstr_el) */

/* erase-character (EC) */
int termstr_ec(termstr *op,int n) noex {
	int		rs ;
	if ((rs = termstr_magic(op)) >= 0) ylikely {
	    if (n > 0) ylikely {
	        uint	tf ;
	        char	tbuf[TLEN + 1] ;
	        if (n >= TERMSTR_COLS) n = TERMSTR_COLS ;
	        if (n <= 1) {
	            char	*bp = tbuf ;
	            *bp++ = ' ' ;
	            *bp++ = CH_BS ;
	            *bp = '\0' ;
		    cint tl = intconv(bp - tbuf) ;
	            rs = buffer_strw(op->sbp,tbuf,tl) ;
	        } else {
	            cint	ti = op->ti ;
	            tf = terms[ti].flags ;
	            if (tf & TCF_MEC) {
	                rs = termstr_conseq(op,'X',1,n,-1,-1,-1) ;
	            } else {
	                if ((rs = buffer_blanks(op->sbp,n)) >= 0) {
	                    rs = buffer_backs(op->sbp,n) ;
			}
	            } /* end if */
	        } /* end if */
	    } else if (n < 0) {
	        rs = SR_INVALID ;
	    }
	} /* end if (magic) */
	return rs ;
}
/* end subroutine (termstr_ec) */

/* cursor-up (CUP) */
int termstr_curu(termstr *op,int n) noex {
	return termstr_curm(op,curtype_u,n) ;
}
/* end subroutine (termstr_curu) */

/* cursor-down (CUD) */
int termstr_curd(termstr *op,int n) noex {
	return termstr_curm(op,curtype_d,n) ;
}
/* end subroutine (termstr_curd) */

/* cursor-right (CUR) */
int termstr_curr(termstr *op,int n) noex {
	return termstr_curm(op,curtype_r,n) ;
}
/* end subroutine (termstr_curr) */

/* cursor-left (CUL) */
int termstr_curl(termstr *op,int n) noex {
	return termstr_curm(op,curtype_l,n) ;
}
/* end subroutine (termstr_curl) */

/* cursor-home (CUH) */
int termstr_curh(termstr *op,int r,int c) noex {
	int		rs ;
	if ((rs = termstr_magic(op)) >= 0) ylikely {
	    if ((r <= 0) && (c <= 0)) {
	        cchar	*sp = "\033[H" ;
	        cint	sl = 3 ;
	        rs = buffer_strw(op->sbp,sp,sl) ;
	    } else if (c <= 0) {
	        rs = termstr_conseq(op,'H',1,(r+1),-1,-1,-1) ;
	    } else {
	        if (r < 0) r = 0 ;
	        rs = termstr_conseq(op,'H',1,(r+1),(c+1),-1,-1) ;
	    }
	} /* end if (magic) */
	return rs ;
}
/* end subroutine (termstr_curh) */

/* set-scroll-region */
int termstr_ssr(termstr *op,int r,int n) noex {
	int		rs ;
	if ((rs = termstr_magic(op)) >= 0) ylikely {
	    if (r < 0) {
	        cchar	*sp = "\033[r" ;
	        cint	sl = 3 ;
	        rs = buffer_strw(op->sbp,sp,sl) ;
	    } else if (n > 0) {
	        if (r < 0) r = 0 ;
	        rs = termstr_conseq(op,'r',2,(r+1),(r+n),-1,-1) ;
	    }
	} /* end if (magic) */
	return rs ;
}
/* end subroutine (termstr_ssr) */

/* crusor-save-restore */
int termstr_csr(termstr *op,int f) noex {
	int		rs ;
	if ((rs = termstr_magic(op)) >= 0) ylikely {
	    uint	tf ;
	    int		sl = -1 ;
	    int		ti = op->ti ;
	    cchar	*sp = nullptr ;
	    tf = terms[ti].flags ;
	    if (tf & TCF_MVCSR) {
	        sp = (f) ? TERMSTR_VCURS : TERMSTR_VCURR ;
	    } else if (tf & TCF_MACSR) {
	        sp = (f) ? TERMSTR_ACURS : TERMSTR_ACURR ;
	    } else {
	        rs = SR_NOTSUP ;
	    }
	    if ((rs >= 0) && sp) {
	        rs = buffer_strw(op->sbp,sp,sl) ;
	    }
	} /* end if (magic) */
	return rs ;
}
/* end subroutine (termstr_csr) */

#ifdef	COMMENT

/* insert */
int termstr_il(op,n) ;
int termstr_ic(op,n) ;

/* delete */
int termstr_dl(op,n) ;
int termstr_dc(op,n) ;

/* insert-replacement-mode */
int termstr_irm(op,f) ;

/* cursor-visibility */
int termstr_cvis(op,f) ;

#endif /* COMMENT */


/* private subroutines */

local int termstr_curm(termstr *op,int curtype,int n) noex {
	int		rs = SR_OK ;
	if (n != 0) ylikely {
	    rs = SR_INVALID ;
	    if ((curtype >= 0) && (curtype < curtype_overlast)) {
	        char	tbuf[TLEN + 1] ;
	        cint	name = curtypes[curtype] ;
	        if (n <= 1) {
	            char	*bp = tbuf ;
	            *bp++ = '\033' ;
	            *bp++ = CH_LBRACK ;
	            *bp++ = char(name) ;
	            *bp = '\0' ;
		    cint tl = intconv(bp - tbuf) ;
	            rs = buffer_strw(op->sbp,tbuf,tl) ;
	        } else {
	            rs = termstr_conseq(op,name,1,n,-1,-1,-1) ;
	        }
	    } /* end if (non-zero) */
	} /* end if (NE) */
	return rs ;
}
/* end subroutine (termstr_curm) */

local int termstr_findterm(termstr *op,cchar *termtype) noex {
	cint		tlen = lenstr(termtype) ;
	int		rs = SR_OK ;
	int		n = 2 ;
	int		m ;
	int		m_max = -1 ;
	int		si = -1 ;
	for (int i = 0 ; terms[i].name ; i += 1) {
	    cchar	*sp = terms[i].name ;
	    m = nleadstr(sp,termtype,-1) ;
	    if (((m >= n) && (m == tlen)) || (sp[m] == '\0')) {
	        if (m > m_max) {
	            m_max = m ;
	            si = i ;
	        }
	    } /* end if */
	} /* end for */
	rs = (si >= 0) ? si : SR_NOTFOUND ;
	if (rs >= 0) op->ti = si ;
	return rs ;
}
/* end subroutine (termstr_findterm) */

local int termstr_conseq(termstr *op,int name,int na,...) noex {
	va_list		ap ;
	cnullptr	np{} ;
	int		rs = SR_INVALID ;
	int		len = 0 ; /* return-value */
	if (name != 0) ylikely {
	    cint	tlen = TLEN ;
	    char	tbuf[TLEN + 1] ;
	    va_begin(ap,na) ;
	    if ((rs = termconseqva(tbuf,tlen,name,np,na,ap)) >= 0) {
	        cint	tl = rs ;
	        rs = buffer_strw(op->sbp,tbuf,tl) ;
	        len += rs ;
	    }
	    va_end(ap) ;
	} /* end if */
	return (rs >= 0) ? len : rs ;
}
/* end subroutine (termstr_conseq) */


