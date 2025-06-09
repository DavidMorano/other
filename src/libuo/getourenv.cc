/* getourenv SUPPORT */
/* encoding=ISO8859-1 */
/* lang=C++20 */

/* get (retrieve) a local environment variable */
/* version %I% last-modified %G% */


/* revision history:

	= 2003-11-04, David A­D­ Morano
	This code is generalized from one of my old programs.  Why
	the need for this?  Because with the introduction of loadable
	commands in Korn Shell (KSH), we also needed a local source
	for an environment since the global one is indeterminate
	(missing or incorrect).

*/

/* Copyright © 2003 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Name:
	getourenv

	Description:
	This subroutine is similar to the standard library |getenv(3c)|
	subroutine but uses a local environment array instead.

	Synopsis:
	cchar *getourenv(cchar *const *envv,cchar *key) noex

	Arguments:
	envv		environment array
	key		key-string to lookup

	Returns:
	-		pointer to value for given key
	nullptr		key not found in environment database

*******************************************************************************/

#include	<envstandards.h>	/* MUST be first to configure */
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<cstring>		/* <- |strchr(3c)| */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<matkeystr.h>

#include	"getourenv.h"


/* local defines */


/* imported namespaces */


/* local typedefs */


/* external subroutines */


/* external variables */


/* local structures */


/* forward references */


/* local varialbes */


/* exported variables */


/* exported subroutines */

cchar *getourenv(mainv envv,cchar *key) noex {
	cchar		*vp = nullptr ;
	if (envv && key && (key[0] != '\0')) {
	    if (int ei ; (ei = matkeystr(envv,key,-1)) >= 0) {
		if ((vp = strchr(envv[ei],'=')) != nullptr) {
		    vp += 1 ;
		}
	    }
	}
	return vp ;
} 
/* end subroutine (getourenv) */


