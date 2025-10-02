/* modproc1 MODULE (module-implementation-unit) */
/* charset=ISO8859-1 */
/* lang=C++20 (conformance reviewed) */

/* load module names from a C++ source code file */
/* version %I% last-modified %G% */


/* revision history:

	= 2017-03-21, David A-D- Morano
	This subroutine was originally written.

*/

/* Copyright © 2017 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Names:
	modprocload

	Description:
	I parse a C++ source code file and find all of the module
	names that I can that are being imported to that source file.

	Synopsis:
	modprocload(vecstr *op,cchar *fname) noex

	Arguments:
	op		ureserve::vecstr
	fname		file-name to parse

	Returns:
	>=0		number of imported modules found
	<0		error (system-return)

	Notes:
	1. The handling of line continuations is quite poor (if I have
	to say it myself).  I should have used a C++ |string(3c++|
	object rather than the fixed (although quite large) buffer
	that I am using.  In the end laziness about not changing
	existing code rules the day (which practice I usually quite 
	despised back in the 1980s).

*******************************************************************************/

module ;

#include	<envstandards.h>	/* ordered first to configure */
#include	<sys/stat.h>
#include	<unistd.h>
#include	<fcntl.h>		/* |O_{xx}| */
#include	<climits>		/* |INT_MAX| */
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<string>
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>
#include	<usyscalls.h>
#include	<strnul.hh>
#include	<langproc.h>
#include	<ascii.h>
#include	<six.h>			/* |sispanwht(3uc)| */
#include	<hasx.h>		/* |hasmodname(3uc)| */
#include	<strmgr.h>
#include	<strop.h>
#include	<strwcmp.h>
#include	<mkchar.h>
#include	<ischarx.h>		/* |isalnumlatin(3cu)| */
#include	<localmisc.h>

#pragma		GCC dependency		"mod/modproc.ccm"
#pragma		GCC dependency		"mod/libutil.ccm"
#pragma		GCC dependency		"mod/ulibvals.ccm"

module modproc ;

import libutil ;			/* |getlenstr(3u)| */
import ulibvals ;			/* |ulibval(3u)| + |pagesz| */

/* local defines */

#define	ALTLNMULT	5		/* multplier to pagesz for alt-line */


/* imported namespaces */

using std::string ;			/* type */
using libu::umem ;			/* variable */


/* local typedefs */


/* external subroutines */


/* external variables */


/* local structures */

namespace {
    struct modmgr {
	vecstr		*vop ;
	char		*lbuf{} ;
	langproc	proc ;
	strmgr		lnmgr ;
	int		llen = 0 ;
	int		li = 0 ;
	bool		lnalt = false ;
	modmgr(vecstr *p) noex : vop(p) { } ;
	int operator () (cchar *) noex ;
	int start() noex ;
	int finish() noex ;
	int fileload(cchar *) noex ;
	int fileproc() noex ;
	int liner(int,size_t) noex ;
	int checkln(int,cchar *,int) noex ;
	int procln(int,cchar *,int) noex ;
    private:
	int lnbegin() noex ;
	int lnend() noex ;
	int lnready() noex ;
	int lnadd(cchar *,int) noex ;
	int lncheck(int) noex ;
    } ; /* end struct (modmgr) */
} /* end namespace */


/* forward references */

static bool haspound(cchar *sp,int µsl) noex {
    	bool		f = true ;
    	if (int sl ; (sl = getlenstr(sp,µsl)) > 0) {
    	    cint	si = sispanwht(sp,sl) ;
	    f = (si >= 0) && (si < sl) && (sp[si] == '#') ;
	} /* end if (getlenstr) */
    	return f ;
} /* end subroutine (haspound) */


/* local variables */

cchar			istr[] = "import" ;


/* exported variables */


/* exported subroutines */

int modprocload(vecstr *op,cchar *fname) noex {
    	int		rs = SR_FAULT ;
	if_constexpr (f_debug) debprintf(__func__,"ent fn=%s\n",fname) ;
	if (op && fname) ylikely {
    	    modmgr	mgr(op) ;
	    rs = mgr(fname) ;
	} /* end if (non-null) */
	if_constexpr (f_debug) debprintf(__func__,"ret rs=%d\n",rs) ;
	return rs ;
}
/* end subroutine (modprocload) */


/* local subroutines */

int modmgr::operator () (cchar *fname) noex {
    	int		rs ;
	int		rs1 ;
	int		rv = 0 ; /* return-value */
	if ((rs = start()) >= 0) {
	    if ((rs = proc.start) >= 0) {
	        if ((rs = fileload(fname)) >= 0) {
		    rs = fileproc() ;
		    rv = rs ;
	        } /* end if (fileload) */
	        rs1 = proc.finish ;
	        if (rs >= 0) rs = rs1 ;
	    } /* end if (langproc) */
	    rs1 = finish() ;
	    if (rs >= 0) rs = rs1 ;
	} /* end if (start-finish) */
    	return (rs >= 0) ? rv : rs ;
} /* end method (modmgr::operator) */

int modmgr::start() noex {
    	return SR_OK ;
}

int modmgr::finish() noex {
    	return lnend() ;
}

int modmgr::fileload(cchar *fname) noex {
	int		rs = SR_FAULT ;
	int		rs1 ;
	int		nmods = 0 ;
	if (fname) ylikely {
	    rs = SR_INVALID ;
	    if (fname[0]) ylikely {
		cint	of = O_RDONLY ;
		if ((rs = u_open(fname,of,0)) >= 0) ylikely {
		    cint	fd = rs ;
		    if (ustat sb ; (rs = u_fstat(fd,&sb)) >= 0) ylikely {
			csize	fsize = size_t(sb.st_size) ;
			rs = SR_NOTSUP ;
		        if (S_ISREG(sb.st_mode)) ylikely {
			    rs = SR_OK ;
			    if (fsize > 0) {
			        rs = liner(fd,fsize) ;
			        nmods = rs ;
			    }
			} /* end if (regular file) */
		    } /* end if (stat) */
		    rs1 = u_close(fd) ;
		    if (rs >= 0) rs = rs1 ;
		} /* end if (open) */
	    } /* end if (valid) */
	} /* end if (non-null) */
	return (rs >= 0) ? nmods : rs ;
} /* end method (modmgr::fileload) */

int modmgr::liner(int fd,csize ms) noex {
	cnullptr	np{} ;
	cint		mp = PROT_READ ;
	cint		mf = MAP_SHARED ;
	int		rs ;
	int		rs1 ;
	int		nmods = 0 ; /* return-value */
	if_constexpr (f_debug) {
	    debprintf(__func__,"ent\n") ;
	}
	if (void *md ; (rs = u_mmapbegin(np,ms,mp,mf,fd,0z,&md)) >= 0) {
	    cint	cmd = MADV_SEQUENTIAL ;
	    if ((rs = u_madvise(md,ms,cmd)) >= 0) ylikely {
		size_t	ll = ms ;
		int	ln = 1 ;
		cchar	*lp = charp(md) ;
		for (cchar *tp ; (tp = charp(memchr(lp,'\n',ll))) != np ; ) {
		    csize	si = ((tp + 1) - lp) ;
		    if (int sl = intconv(si - 1) ; sl > 0) {
			cchar *sp = lp ;
	    		if (sp[sl - 1] == '\r') {
			    sl -= 1 ;
	    		}
	    		if (sl > 0) {
			    if (sp[sl - 1] == CH_BSLASH) {
			        rs = lnadd(sp,(sl - 1)) ;
			    } else {
			        if (lnalt) {
			            if ((rs = lnadd(sp,sl)) >= 0) {
				        rs = lncheck(ln++) ;
		                        nmods += rs ;
				    }
			        } else {
		                    rs = checkln(ln++,sp,sl) ;
		                    nmods += rs ;
			        }
			    } /* end if (continuation or regular) */
			} /* end if (still not-empty) */
		    } /* end if (non-empty) */
		    ll -= si ;
		    lp += si ;
		    if (rs < 0) break ;
		} /* end for */
		if ((rs >= 0) && (ll > 0)) {
		    cint	sl = intconv(ll) ;
		    rs = checkln(ln++,lp,sl) ;
		    nmods += rs ;
		} /* end if (trailing line */
	    } /* end if (memory-advise) */
	    rs1 = u_mmapend(md,ms) ;
	    if (rs >= 0) rs = rs1 ;
	} /* end if (map-file) */
	if_constexpr (f_debug) {
	    debprintf(__func__,"ret rs=%d c=%d\n",rs,nmods) ;
	}
	return (rs >= 0) ? nmods : rs ;
} /* end method (modmgr::liner) */

int modmgr::checkln(int ln,cchar *lp,int ll) noex {
    	int		rs = SR_OK ;
	if_constexpr (f_debug) {
	    strnul s(lp,ll) ;
	    debprintf(__func__,"ent ln=%d l=>%s<\n",ln,ccp(s)) ;
	}
	if ((ll > 0) && (! haspound(lp,ll))) {
	    if ((ll > 0) && (lp[ll - 1] == '\r')) {
		ll -= 1 ;
	    }
	    if (ll > 0) {
	        rs = proc.proc(ln,lp,ll) ;
	        debprintf(__func__,"proc.proc() ret rs=%d\n",rs) ;
	    }
	} /* end if (not preprocessor directive) */
	debprintf(__func__,"ret rs=%d\n",rs) ;
    	return rs ; 
} /* end method (modmgr::checkln) */

int modmgr::fileproc() noex {
    	cint		rsn = SR_NOTFOUND ;
    	int		rs ;
	int		rs1{} ;
	int		rv = 0 ; /* return-value */
	debprintf(__func__,"ent\n") ;
	if (langproc_cur cur ; (rs = proc.curbegin(&cur)) >= 0) {
	    int		ln = 1 ;
	    debprintf(__func__,"while-cur\n") ;
	    for (cchar *lp ; (rs1 = proc.curenum(&cur,&lp)) >= 0 ; ) {
		cint ll = rs1 ;
		if (lp) {
		    {
			strnul s(lp,ll) ;
		        debprintf(__func__,"enum s=>%s<\n",ccp(s)) ;
		    }
		    rs = procln(ln++,lp,ll) ;
		    rv += rs ;
		}
		if (rs < 0) break ;
	    } /* end for (langproc_curenum) */
	    debprintf(__func__,"while-cur rs=%d rs1=%d\n",rs,rs1) ;
	    if ((rs >= 0) && (rs1 != rsn)) rs = rs1 ;
	    rs1 = proc.curend(&cur) ;
	    if (rs >= 0) rs = rs1 ;
	} /* end if (langproc_cur) */
	debprintf(__func__,"ret rs=%d rv=%d\n",rs,rv) ;
    	return (rs >= 0) ? rv : rs ;
} /* end method (modmgr::fileproc) */

int modmgr::procln(int ln,cchar *lp,int ll) noex {
    	int		rs ;
	int		rs1 ;
	int		c = 0 ; /* return-value -- number of names found */
	if_constexpr (f_debug) {
	    strnul st(lp,ll) ;
	    debprintf(__func__,"ent ln=>%s<\n",ccp(st)) ;
	}
	(void) ln ;
    	if (strop s ; (rs = s.start(lp,ll)) >= 0) ylikely {
	    cchar	*ip ;
	    if (int il ; (il = s.fieldwht(&ip)) > 0) {
		if_constexpr (f_debug) {
	    	    strnul st(ip,il) ;
	    	    debprintf(__func__,"piece=>%s<\n",ccp(st)) ;
		}
		if (strwcmp(istr,ip,il) == 0) {
		    cint	ch_s = CH_SEMI ;
		    cchar	*mp ;
		    if_constexpr (f_debug) debprintf(__func__,"cmp\n") ;
		    if (int ml ; (ml = s.fieldchr(ch_s,&mp)) > 0) {
			if_constexpr (f_debug) {
	    	    	    strnul st(mp,ml) ;
	    	    	    debprintf(__func__,"m=>%s<\n",ccp(st)) ;
			}
			if (hasmodname(mp,ml)) {
			    rs = vop->adduniq(mp,ml) ;
			    c = (rs < INT_MAX) ;
			}
		    } /* end if (strop_fieldchr) */
	        } /* end if (got "import") */
	    } /* end if (got field) */
	    rs1 = s.finish ;
	    if (rs >= 0) rs = rs1 ;
	} /* end if (strop) */
	if_constexpr (f_debug) debprintf(__func__,"ret rs=%d c=%d\n",rs,c) ;
	return (rs >= 0) ? c : rs ;
} /* end method (modmgr::procln) */

int modmgr::lnbegin() noex {
    	int		rs = SR_OK ;
	if (lbuf == nullptr) {
	    if ((rs = ulibval.pagesz) >= 0) {
		cint	sz = (ALTLNMULT * rs) ;
		if (char *bp ; (rs = umem.mall(sz,&bp)) >= 0) {
		    lbuf = bp ;
		    llen = sz ;
		    rs = lnmgr.start(lbuf,llen) ;
		    if (rs < 0) {
			umem.free(lbuf) ;
			lbuf = nullptr ;
			llen = 0 ;
		    } /* end if (error) */
		} /* end if (memory-allocation) */
	    } /* end if (ulibvals) */
	} /* end if (required) */
	return rs ;
} /* end method (modmgr::lnbegin) */

int modmgr::lnend() noex {
    	int		rs = SR_OK ;
	int		rs1 ;
	if (lbuf) {
	    {
		rs1 = lnmgr.finish ;
		if (rs >= 0) rs = rs1 ;
	    }
	    {
	        rs1 = umem.free(lbuf) ;
	        if (rs >= 0) rs = rs1 ;
	        lbuf = nullptr ;
	        llen = 0 ;
	    }
	    li = 0 ;
	} /* end if (was initialized) */
	return rs ;
} /* end method (modmgr::lnend) */

int modmgr::lnready() noex {
    	int		rs = SR_OK ;
	if (lbuf == nullptr) {
	    rs = lnbegin() ;
	}
	return rs ;
} /* end method (modmgr::lnready) */

int modmgr::lnadd(cchar *sp,int sl) noex {
    	int		rs ;
	if ((rs = lnready()) >= 0) {
	    if ((rs = lnmgr.str(sp,sl)) >= 0) {
	        lnalt = true ;
	    } /* end if (lnadd) */
	} /* end if (lnready) */
	return rs ;
} /* end method (modmgr::lnadd) */

int modmgr::lncheck(int ln) noex {
    	int		rs ;
	if (cchar *cp ; (rs = lnmgr.get(&cp)) >= 0) {
	    cint cl = rs ;
	    if ((rs = checkln(ln,cp,cl)) >= 0) {
	        if ((rs = lnmgr.reset) >= 0) {
	            lnalt = false ;
	        }
	    } /* endif (checkln) */
	} /* end if (lnmgr.get) */
	return rs ;
} /* end method (modmgr::lncheck) */


