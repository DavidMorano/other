/* argmgr3 MODULE */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* manage program arguments */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-09-10, David A­D­ Morano
	This subroutine was written for Rightcore Network Services.

	= 2020-08-13, David A­D­ Morano
	This code has been modularized (w/ C++20 modules).

*/

/* Copyright © 1998,2020 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

  	Object:
	argmgr

	Description:
	This object manages program arguments.

*******************************************************************************/

module ;

#include	<envstandards.h>	/* MUST be first to configure */
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<new>			/* |nothrow(3c++)| */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>
#include	<ulogerror.h>
#include	<localmisc.h>

#pragma		GCC dependency	"mod/debug.ccm"

module argmgr ;

import debug ;				/* |deb{xx}(3u)| */

/* local defines */


/* imported namespaces */


/* local typedefs */


/* external subroutines */


/* local structures */


/* forward references */


/* forward refernces */


/* local variables */


/* exported variables */


/* exported subroutines */


/* local subroutines */

bool argmgr_iter::operator != (const argmgr_iter &o) noex {
	bool		f = false ;
	if_constexpr (f_debug) {
	    debprintf(__func__,"ent ai=%d\n",ai) ;
	}
	if (op && (ai < o.ai)) {
    	    int		rs ;
	    if (cchar *ap ; (rs = op->get(ai,&ap)) > 0) {
	        ai = rs ;
	        f = bool(ap) ;
	    } else if (rs < 0) {
	        ulogerror(__func__,rs,"operator-!=") ;
	    } /* end if */
	}
	if_constexpr (f_debug) {
	    debprintf(__func__,"ret ai=%d f=%u\n",ai,f) ;
	}
	return f ;
} /* end method (argmgr_iter::operator) */

bool argmgr_iter::operator < (const argmgr_iter &o) noex {
	bool		f = false ;
	if (op && (ai < o.ai)) {
    	    int		rs ;
	    if (cchar *ap ; (rs = op->get(ai,&ap)) > 0) {
	        ai = rs ;
	        f = bool(ap) ;
	    } else if (rs < 0) {
	        ulogerror(__func__,rs,"operator-<") ;
	    } /* end if */
	}
	return f ;
} /* end method (argmgr_iter::operator) */

ccharp argmgr_iter::operator * () noex {
	cchar		*rp = nullptr ;
	if (op) {
	    if_constexpr (f_debug) {
	        debprintf(__func__,"ent ai=%d\n",ai) ;
	    }
	    if (int rs ; (rs = op->present(ai)) > 0) {
	        if_constexpr (f_debug) {
	            debprintf(__func__,"present=YES\n") ;
		}
	        if (cchar *ap ; (rs = op->get(ai,&ap)) > 0) {
	            rp = ap ;
	        } else if (rs < 0) {
	            ulogerror(__func__,rs,"operator-deref") ;
	        } /* end if */
	    } else {
	        ulogerror(__func__,rs,"operator-deref") ;
	    } /* end if (present) */
	}
	if_constexpr (f_debug) {
	    cchar *fmt = "ret rp=%s\n" ;
	    debprintf(__func__,fmt,((rp) ? "ok" : "null")) ;
	}
	return rp ;
} /* end method (argmgr_iter::operator) */

argmgr_iter::operator ccharp () noex {
    	return *(*this) ;
} /* end method (argmgr_iter::operator) */

argmgr_iter argmgr_iter::operator + (int inc) const noex {
    	argmgr_iter res = *this ;
	res.ai += inc ;
	if (res.ai < 0) res.ai = 0 ;
	return res ;
}

argmgr_iter &argmgr_iter::operator += (int n) noex {
    	if (cint rs = increment(n) ; rs < 0) {
	    ulogerror("argmgr_iter",rs,"operator-assign-inc") ;
	}
	return *this ;
}

argmgr_iter argmgr_iter::operator ++ () noex { /* pre */
    	increment() ;
    	return *this ;
}

argmgr_iter argmgr_iter::operator ++ (int) noex { /* post */
    	argmgr_iter	prev(*this) ;
	increment() ;
    	return prev ;
}

int argmgr_iter::increment(int n) noex {
    	int		rs = SR_INVALID ;
	if (op && (n >= 0)) {
	    bool	f = false ;
	    rs = SR_OK ;
	    for (int i = 0 ; (rs >= 0) && (i < n) && (! f) ; i += 1) {
	        if (cc *ap ; (rs = op->get((ai + 1),&ap)) > 0) {
		    ai = rs ;
		    f = !ap ;
		} else if (rs == 0) {
		    f = true ;
	        }
	    } /* end for */
	} /* end if (valid) */
	return rs ;
} /* end method (argmgr_iter::increment) */


