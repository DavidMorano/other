/* nulstr SUPPORT */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* assert a NUL-terminated string */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-02-01, David A­D­ Morano
	This code was originally written.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

  	Object:
	nulstr

	Descrition:
	This object module (nulstr) provides support for creating
	NUL-terminated strings when only a counted string is
	available.

*******************************************************************************/

#include	<envstandards.h>	/* MUST be first to configure */
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>
#include	<usyscalls.h>
#include	<uclibmem.h>
#include	<strwcpy.h>
#include	<localmisc.h>

#include	"nulstr.h"

#pragma		GCC dependency		"mod/libutil.ccm"

import libutil ;			/* |lenstr(3u)| */

/* local defines */


/* imported namespaces */

using libuc::libmem ;			/* variable */


/* local typedefs */


/* external subroutines */


/* external variables */


/* local structures */


/* forward references */


/* local variables */


/* exported variables */


/* exported subroutines */

int nulstr_start(nulstr *ssp,cchar *sp,int sl,cchar **rpp) noex {
	int		rs = SR_FAULT ;
	int		cl = 0 ;
	if (ssp && sp && rpp) ylikely {
	    rs = SR_OK ;
	    ssp->as = nullptr ;
	    *rpp = sp ;
	    if (sl >= 0) ylikely {
	        if (sp[sl] != '\0') {
	            if (sl > NULSTR_SHORTLEN) {
	                if (cc *cp ; (rs = libmem.mallocstrw(sp,sl,&cp)) >= 0) {
	                    cl = rs ;
	                    *rpp = cp ;
	                    ssp->as = cp ;
	                }
	            } else {
	                *rpp = ssp->buf ;
	                cl = intconv(strwcpy(ssp->buf,sp,sl) - ssp->buf) ;
	            } /* end if */
	        } else {
	            cl = sl ;
	        }
	    } else {
	        cl = lenstr(sp) ;
	    }
	} /* end if (non-null) */
	return (rs >= 0) ? cl : rs ;
}
/* end subroutine (nulstr_start) */

int nulstr_finish(nulstr *ssp) noex {
	int		rs = SR_FAULT ;
	int		rs1 ;
	if (ssp) ylikely {
	    rs = SR_OK ;
	    if (ssp->as) ylikely {
		char *bp = cast_const<charp>(ssp->as) ;
	        rs1 = libmem.free(bp) ;
	        if (rs >= 0) rs = rs1 ;
	        ssp->as = nullptr ;
	    }
	    ssp->buf[0] = '\0' ;
	} /* end if (non-null) */
	return rs ;
}
/* end subroutine (nulstr_finish) */


/* local subroutines */

int nulstr::start(cchar *sp,int sl,cchar **rpp) noex {
	return nulstr_start(this,sp,sl,rpp) ;
}

nulstr_co::operator int () noex {
	int		rs = SR_BUGCHECK ;
	if (op) ylikely {
	    switch (w) {
	    case nulstrmem_finish:
	        rs = nulstr_finish(op) ;
	        break ;
	    } /* end switch */
	} /* end if (non-null) */
	return rs ;
}
/* end method (nulstr_co::operator) */

void nulstr::dtor() noex {
	if (cint rs = finish ; rs < 0) {
	    ulogerror("dtor",rs,"fini-finish") ;
	}
} /* end method (nulstr::dtor) */


