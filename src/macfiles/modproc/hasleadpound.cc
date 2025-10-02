/* hasleadpund SUPPORT */
/* charset=ISO8859-1 */
/* lang=C++20 (conformance reviewed) */

/* does the c-string contain a leading pound character (after skipping WS) */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-11-01, David A­D­ Morano
	This subroutine was originally written.

	= 2017-03-21, David A-D- Morano
	I made some sort of update.

*/

/* Copyright © 1998,2017 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Names:
	hasleadpound

	Description:
	Determine if the given c-string contains a leading pound
	character.

	Synopsis:
	bool hasleadpound(cchar *sp,int sl) noex

	Arguments:
	sp		c-string pointer
	sl		c-string length

	Returns:
	true		the given c-string has a leading pound character
	false		the given c-string does not have

*******************************************************************************/

#include	<envstandards.h>	/* ordered first to configure */
#include	<climits>		/* |INT_MAX| */
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>
#include	<six.h>			/* |sispanwht(3uc)| */
#include	<localmisc.h>

#include	"hasleadpound.h"

#pragma		GCC dependency		"mod/libutil.ccm"

import libutil ;			/* |getlenstr(3u)| */

/* local defines */


/* imported namespaces */


/* local typedefs */


/* external subroutines */


/* external variables */


/* local structures */


/* forward references */


/* local variables */


/* exported variables */


/* exported subroutines */

bool haspound(cchar *sp,int µsl) noex {
    	bool		f = true ;
    	if (int sl ; (sl = getlenstr(sp,µsl)) > 0) {
    	    cint	si = sispanwht(sp,sl) ;
	    f = (si >= 0) && (si < sl) && (sp[si] == '#') ;
	} /* end if (getlenstr) */
    	return f ;
} /* end subroutine (haspound) */


