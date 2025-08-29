/* uckvamatch SUPPORT */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* interface component for UNIX® library-3c */
/* system data-abse key-value handling */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-04-13, David A­D­ Morano
	Originally written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Name:
	uc_kvamatch

	Description:
	This subroutine finds a keyname (if it exists) in a KVA-type
	of object.

	Synopsis:
	int uc_kvamatch(kva_t *kva,cchar *keyname,cchar **rpp) noex

	Arguments:
	kva		key-value handle
	keyname		key-name to lookup
	rpp		pointer to pointer to results

	Returns:
	>=0		key-name found and this is the length (including zero)
	SR_NOTFOUND	key-name not found
	SR_NOSYS	function not implemented on this system
	<0		error (system-return)

*******************************************************************************/

#include	<envstandards.h>	/* MUST be first to configure */
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<new>			/* |nothrow(3c++)| */
#include	<secdb.h>		/* <- the money shot */
#include	<usystem.h>
#include	<localmisc.h>

#include	"uckvamatch.h"

import libutil ;			/* |lenstr(3u)| */

/* local defines */


/* imported namespaces */

using std::nullptr_t ;			/* type */
using std::nothrow ;			/* constant */


/* external subroutines */


/* external variables */


/* local structures */


/* forward references */


/* local variables */


/* exported variables */


/* exported subroutines */

int uc_kvamatch(kva_t *kva,cchar *keyname,cchar **rpp) noex {
	cnullptr	np{} ;
	int		rs = SR_FAULT ;
	cchar		*rp = nullptr ;
	if (kva && keyname) {
	    rs = SR_INVALID ;
	    if (keyname[0]) {
		rs = SR_OK ;
	        if (syshas.userattr) {
		    char	*kp = charp(keyname) ;
	            if ((rp = kva_match(kva,kp)) != np) {
	                rs = lenstr(rp) ;
	            } else {
	                rs = SR_NOTFOUND ;
	            }
	        } else {
	            rs = SR_NOSYS ;
	        }
	    } /* end if (valid) */
	} /* end if (non-null) */
	if (rpp) *rpp = (rs >= 0) ? rp : nullptr ;
	return rs ;
}
/* end subroutine (uc_kvamatch) */


