/* argmgr1 MODULE */
/* encoding=ISO8859-1 */
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

module argmgr ;

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

argmgr_co::operator int () noex {
	int		rs = SR_BUGCHECK ;
	if (op) {
	    switch (w) {
	    case argmgrmem_start:
	        rs = SR_OK ;
	        break ;
	    case argmgrmem_finish:
	        rs = SR_OK ;
	        break ;
	    case argmgrmem_arg:
	        rs = SR_OK ;
	        break ;
	    case argmgrmem_count:
	        rs = op->cpos ;
	        break ;
	    default:
		rs = SR_INVALID ;
		break ;
	    } /* end switch */
	} /* end if (non-null) */
	return rs ;
} /* end method (argmgr_co) */


