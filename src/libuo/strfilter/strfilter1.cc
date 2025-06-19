/* strfilter MODULE (module-implementation-unit) */
/* charset=ISO8859-1 */
/* lang=C++20 (conformance reviewed) */

/* manage a variable length bit-array */
/* version %I% last-modified %G% */

#define	CF_BINSEARCH	1		/* binary-search */

/* revision history:

	= 1998-09-10, David A­D­ Morano
	This subroutine was written for Rightcore Network Services.

	= 2020-08-13, David A­D­ Morano
	This code has been modularized (w/ C++20 modules).
	There was really no good reason for this.  In many cases,
	w/ my limited experience w/ modules so far, sometimes
	modules are more of a hassle than they are worth.

*/

/* Copyright © 1998,2020 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Module:
	strfilter

	Description:
	This object implements a (fast) filter for strings.

	Synopsis:

	Arguments:

	Returns:
	>=0		number of matches before target count reached
	<0		error (system-return)

*******************************************************************************/

module ;

#include	<envstandards.h>	/* MUST be first to configure */
#include	<climits>		/* |UCHAR_MAX| + |CHAR_BIT| */
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<new>			/* |nothrow(3c++)| */
#include	<bitset>
#include	<algorithm>		/* |min(3c++)| + |max(3c++)| */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>
#include	<ulogerror.h>
#include	<umemalloc.hh>
#include	<strn.h>		/* |strnbrk(3uc)| */
#include	<strnul.hh>
#include	<mkchar.h>
#include	<localmisc.h>

module strfilter ;

import libutil ;
import ureserve ;			/* |vecstr(3u)| */
import sif ;

/* local defines */

#define	STRFILTER_DEFENTS	15	/* default entries */
#define	STRFILTER_MAGIC		0x98865854


/* imported namespaces */

using std::nullptr_t ;			/* type */
using std::bitset ;			/* type */
using std::min ;			/* subroutine */
using std::max ;			/* subroutine */
using std::sort ;			/* subroutine (niebloid) */
using std::binary_search ;		/* subroutine */
using libu::umemallocstrw ;		/* subroutine */
using libu::umemalloc ;			/* subroutine */
using libu::umemvalloc ;		/* subroutine */
using libu::umemcalloc ;		/* subroutine */
using libu::umemrealloc ;		/* subroutine */
using libu::umemfree ;			/* subroutine */
using libu::umemrsfree ;		/* subroutine */
using std::nothrow ;			/* constant */


/* local typedefs */


/* external subroutines */


/* external variables */


/* local structures */


/* forwards references */

static ulong mksw(cchar *sp,int sl) noex {
    	cint	n = min(sl,szof(ulong)) ;
    	ulong	res = 0 ; /* return-value */
	for (int i = 0 ; i < n ; i += 1) {
	    ulong v = mkchar(*sp++) ;
	    v <<= (CHAR_BIT * i) ;
	    res |= v ;
	} /* end for */
	return res ;
} /* end method (mksw) */


/* local variables */

constexpr int		defents = STRFILTER_DEFENTS ;	/* defualt entries */

constexpr bool		f_binsearch = CF_BINSEARCH ;


/* exported variables */


/* exported subroutines */

int strfilter::istart() noex {
    	int		rs ;
	nents = defents ;
	if ((rs = vecstr::start) >= 0) {
	    fl.open = true ;
	    magic = STRFILTER_MAGIC ;
	}
	return rs ;
} /* end method (strfilter::istartadd) */

int strfilter::ifinish() noex {
    	int		rs = SR_NOTOPEN ;
	int		rs1 ;
	if (magic == STRFILTER_MAGIC) {
	    rs = SR_OK ;
	    if (filtarr) {
		rs1 = umemfree(filtarr) ;
		if (rs >= 0) rs = rs1 ;
		filtarr = nullptr ;
	    }
	    {
	        rs1 = vecstr::finish() ;
		if (rs >= 0) rs = rs1 ;
	    }
	    fl.open = false ;
	    magic = 0 ;
	} /* end if (magic) */
	return rs ;
} /* end method (strfilter::ifinsh) */

int strfilter::add(cchar *sp,int sl) noex {
    	int		rs = SR_FAULT ;
	int		c = 0 ;
	if (sp) {
	    if (sl < 0) sl = xstrlen(sp) ;
	    rs = SR_INVALID ;
	    if (sl > 0) {
    	        rs = SR_NOTOPEN ;
	        if (magic == STRFILTER_MAGIC) {
		    rs = SR_OK ;
		    if (strnbrk(sp,sl," \t,")) {
		        sif sa(sp,sl,',') ;
		        cc *cp ;
		        for (int cl ; (cl = sa(&cp)) > 0 ; ) {
			    c += 1 ;
		            rs = iaddone(cp,cl) ;
			    if (rs < 0) break ;
		        } /* end for */
		    } else {
			c += 1 ;
		        rs = iaddone(sp,sl) ;
		    }
	        } /* end if (magic) */
	    } /* end if (valid) */
	} /* end if (non-null) */
	return (rs >= 0) ? c : rs ;
} /* end method (strfilter::add) */

int strfilter::have(cchar *sp,int sl) noex {
    	int		rs = SR_FAULT ;
	if (sp) {
	    if (sl < 0) sl = xstrlen(sp) ;
	    rs = SR_INVALID ;
	    if (sl > 0) {
	        rs = ihave(sp,sl) ;
	    }
	}
	return rs ;
} /* end method (strfilter::have) */

int strfilter::ents() noex {
	int		rs = SR_NOTOPEN ;
	if (magic == STRFILTER_MAGIC) {
    	    rs = nents ;
	} /* end if (non-null) */
    	return rs ;
} /* end method (strfilter::ents) */

int strfilter::icount() const noex {
	int		rs = SR_NOTOPEN ;
	if (magic == STRFILTER_MAGIC) {
	    rs = vecstr::icount() ;
	} /* end if (magic) */
	return rs ;
} /* end method (strfilter::icount) */

int strfilter::iready() noex {
	int		rs = SR_NOTOPEN ;
	if (magic == STRFILTER_MAGIC) {
	    if ((rs = vecstr::sort) >= 0) {
	        std::sort(filtarr,(filtarr+idx)) ;
	    }
	} /* end if (magic) */
	return rs ;
} /* end method (strfilter::iready) */

int strfilter::iaddone(cchar *cp,int cl) noex {
    	int		rs ;
	if (cl < 0) cl = xstrlen(cp) ;
	if ((rs = ihave(cp,cl)) == 0) {
	    if ((rs = inschrs(cp,cl)) >= 0) {
		rs = vecstr::add(cp,cl) ;
	    }
	}
	return rs ;
} /* end method (strfilter::iaddone) */

int strfilter::ihave(cchar *sp,int sl) noex {
	constexpr cint	sz = szof(ulong) ;
    	cint		ch = mkchar(*sp) ;
    	int		rs = SR_OK ;
	int		f = false ;
	if (bool(filtchr[ch])) {
	    ulong	sw = mksw(sp,sl) ;
	    cint	rl = (sl - sz) ;
	    if (fl.ready) {
		f = binary_search(filtarr,(filtarr + idx),sw) ;
	    } else {
	        for (int i = 0 ; i < idx ; i += 1) {
		    if ((f = (filtarr[i] == sw))) break ;
	        }
	    } /* end if (fl.ready) */
	    if (f && (rl > 0)) {
		rs = vecstr::search(sp,sl) ;
		f = rs ;
	    }
	} /* end if (had in character bit-array) */
	return (rs >= 0) ? f : rs ;
} /* end method (strfilter::ihave) */

int strfilter::inschrs(cchar *sp,int sl) noex {
    	cint		ch = mkchar(*sp) ;
    	int		rs ;
	filtchr[ch] = true ;
	if ((rs = extend()) >= 0) {
	    ulong	sw = mksw(sp,sl) ;
	    bool	f = false ;
	    for (int i = 0 ; i < idx ; i += 1) {
		if ((f = (filtarr[i] == sw))) break ;
	    }
	    if (! f) {
		filtarr[idx++] = sw ;
		filtarr[idx] = 0UL ;
	    }
	} /* end if (extend) */
	return rs ;
} /* end method (strfilter::inschrs) */

int strfilter::extend(int n) noex {
	int		rs = SR_OK ;
	if ((idx + n) > ext) {
	    int		ne ;
	    int		sz ;
	    voidp	na{} ;
	    if (filtarr == nullptr) {
	        ne = max((n + 1),defents) ;
	        sz = (ne + 1) * szof(ulong) ;
	        rs = umemalloc(sz,&na) ;
	    } else {
	        ne = (ext * 2) ;
	        sz = (ne + 1) * szof(ulong) ;
	        rs = umemrealloc(filtarr,sz,&na) ;
	    }
	    if (rs >= 0) {
	        filtarr = ulongp(na) ;
	        ext = ne ;
	    }
	} /* end if (extension needed) */
	return rs ;
} /* end method (strfilter::extend) */

void strfilter::dtor() noex {
	if (cint rs = finish ; rs < 0) {
	    ulogerror("strfilter",rs,"fini-finish") ;
	}
}

strfilter::operator int () const noex {
	int		rs = SR_NOTOPEN ;
	if (magic == STRFILTER_MAGIC) {
	    rs = icount() ;
	} /* end if (magic) */
	return rs ;
} /* end method (strfilter::operator) */

strfilter_co::operator int () noex {
	int		rs = SR_BUGCHECK ;
	if (op) {
	    switch (w) {
	    case strfiltermem_start:
	        rs = op->istart() ;
	        break ;
	    case strfiltermem_finish:
	        rs = op->ifinish() ;
	        break ;
	    case strfiltermem_extent:
	        rs = op->ext ;
	        break ;
	    case strfiltermem_count:
	        rs = op->icount() ;
	        break ;
	    case strfiltermem_ready:
	        rs = op->iready() ;
	        break ;
	    } /* end switch */
	} /* end if (non-null) */
	return rs ;
} /* end method (strfilter_co::operator) */


