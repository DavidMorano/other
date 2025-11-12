/* ucstrftime SUPPORT */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* interface component for UNIX® library-3c */
/* Internet-Conversion functions */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-04-13, David A­D­ Morano
	Originally written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Name:
	uc_strftime

	Description:
	This subroutine creates a c-string in a caller-supplied
	buffer as directed by a format specification and using the
	values in a broken-out time structure (|struct tm|).

	Synopsis:
	int uc_strftime(char *dbuf,int dlen,cc *fmt,CTM *tmp) noex

	Arguments:
	dbuf		result buffer pointer
	dlen		result buffer length
	tmp		pointer to a constant |struct tm|

	Returns:
	>=0		successful - number of bytes created in result
	<0		error (system-return)

*******************************************************************************/

#include	<envstandards.h>	/* MUST be first to configure */
#include	<cerrno>
#include	<ctime>			/* |strftime(3v)| */
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>
#include	<intsat.h>
#include	<localmisc.h>

#include	"ucstrftime.h"

/* local defines */


/* local namespaces */


/* local typedefs */


/* external subroutines */


/* external variables */


/* local structures */


/* forward references */


/* local variables */


/* exported variables */


/* exported subroutines */

int uc_strftime(char *dbuf,int dlen,cc *fmt,CTM *tmp) noex {
	int		rs = SR_FAULT ;
	int		rl = 0 ; /* return-value */
	if (dbuf && fmt && tmp) ylikely {
	    rs = SR_INVALID ;
	    if ((dlen > 0) && fmt[0]) {
		csize	dsize = size_t(dlen) ;
		errno = 0 ;
		if (size_t res ; (res = strftime(dbuf,dsize,fmt,tmp)) > 0) {
		    rs = SR_OK ;
		    rl = intsat(res) ;
		} else if (res == 0) {
		    rs = (errno) ? (- errno) : SR_OK ;
		} else {
		    rs = SR_BADFMT ;
	        } /* end if (strftime) */
	    } /* end if (valid) */
	} /* end if (non-null) */
	return (rs >= 0) ? rl : rs ;
} /* end subroutine (uc_strftime) */


