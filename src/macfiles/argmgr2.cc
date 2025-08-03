/* argmgr2 MODULE */
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
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>
#include	<localmisc.h>

#pragma		GCC dependency	"mod/debug.ccm"

module argmgr ;

import debug ;

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

argmgr_iter argmgr::begin() noex {
    	argmgr_iter	res(this,0) ;
	cchar		*ap = nullptr ;
	if (cint rs = get(1,&ap) ; rs > 0) {
	    if_constexpr (f_debug) {
		cchar *fmt = "rs=%d ap=%s\n" ;
	        debprintf(__func__,fmt,rs,((ap) ? "ok" : "null")) ;
	    }
	    if (ap) res.ai = rs ;
	} /* end if (get) */
	return res ;
} /* end method (argmgr::begin) */

argmgr_iter argmgr::end() noex {
    	argmgr_iter	res(this,argc) ;
	return res ;
} /* end method (argmgr::end) */


