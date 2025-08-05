/* files_utils1 MODULE (module-implementation-unit) */
/* charset=ISO8859-1 */
/* lang=C++20 (conformance reviewed) */

/* Files-Utilities (for FILES program) */
/* version %I% last-modified %G% */


/* revision history:

	= 2017-03-21, David A-D- Morano
	This subroutine was originally written.

*/

/* Copyright © 2017 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Object:
	files_util 

*******************************************************************************/

module ;

#include	<envstandards.h>	/* ordered first to configure */
#include	<climits>		/* |INT_MAX| */
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>
#include	<usyscalls.h>
#include	<usupport.h>		/* |cfdec(3u)| */
#include	<matxstr.h>		/* |matocasestr| */
#include	<mkchar.h>
#include	<ischarx.h>		/* |isnumlatin(3uc)| */
#include	<localmisc.h>

#pragma		GCC dependency	"mod/libutil.ccm"

module files_utils ;

import libutil ;			/* |lenstr(3u)| */


/* local defines */


/* imported namespaces */

using libu::cfdec ;			/* |cfdec(3u)| */


/* local typedefs */


/* external subroutines */


/* external variables */


/* local structures */


/* forward references */


/* local variables */


/* exported variables */


/* exported subroutines */

constexpr cpcchar	hits[] = {
	"0",
	"1",
	"no",
	"yes",
	"false",
	"true",
	"-",
	"+",
	nullptr
} ; /* end array (hits) */


/* exported variables */


/* exported subroutines */

int optval(cchar *sp,int sl) noex {
	int		rs = SR_FAULT ;
	int		v = 0 ;
	if (sp) {
	    rs = SR_OK ;
	    if (sl < 0) sl = lenstr(sp) ;
	    if (sl > 0) {
		if (int hi ; (hi = matocasestr(hits,1,sp,sl)) >= 0) {
	            v = (hi & 1) ;
	        } else {
	            cint	ch = mkchar(sp[0]) ;
		    rs = SR_INVALID ;
	            if (isnumlatin(ch)) {
	                rs = cfdec(sp,sl,&v) ; /* <- swapped out */
		    }
	        } /* end if */
	    } /* end if */
	} /* end if (non-null) */
	return (rs >= 0) ? v : rs ;
}
/* end subroutine (optval) */


