/* argmgr2 MODULE */
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
#include	<mkchar.h>
#include	<ischarx.h>
#include	<localmisc.h>

module argmgr ;

import libutil ;

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
	if (ai < o.ai) {
    	    int		rs ;
	    if (cchar *ap ; (rs = op->get(ai,&ap)) > 0) {
	        ai = rs ;
	        f = true ;
	    } else if (rs < 0) {
	        ulogerror("argmgr_iter",rs,"operator-!=") ;
	    } /* end if */
	}
	return f ;
} /* end method (argmgr_iter::operator) */

bool argmgr_iter::operator < (const argmgr_iter &o) noex {
	bool		f = false ;
	if (ai < o.ai) {
    	    int		rs ;
	    if (cchar *ap ; (rs = op->get(ai,&ap)) > 0) {
	        ai = rs ;
	        f = true ;
	    } else if (rs < 0) {
	        ulogerror("argmgr_iter",rs,"operator-<") ;
	    } /* end if */
	}
	return f ;
} /* end method (argmgr_iter::operator) */

argmgr_iter::operator ccharp () noex {
    	int		rs ;
	cchar		*rp = nullptr ;
	if (cchar *ap ; (rs = op->get(ai,&ap)) > 0) {
	    ai = rs ;
	    rp = ap ;
	} else if (rs < 0) {
	    ulogerror("argmgr_iter",rs,"operator-ccharp") ;
	} /* end if */
	return rp ;
} /* end method (argmgr_iter::operator) */


