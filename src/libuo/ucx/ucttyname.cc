/* ucttyname SUPPORT */
/* charset=ISO8859-1 */
/* lang=C++20 (conformance reviewed) */

/* interface component for UNIX® library-3c */
/* get the filename (path) of a terminal device */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-03-01, David A­D­ Morano
        This is written to get a portable (reentrant) version of TTYNAME.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

  	Name:
	uc_ttyname

	Description:
	This subroutine gets the filepath of a terminal device under
	'/dev/'.

	Notes:
	The infernal subroutine |ttyname_rp(3u)| returns '0' if
	successful and |errno| if failure (mostly overflow).

*******************************************************************************/

#include	<envstandards.h>	/* MUST be ordered first to configure */
#include	<unistd.h>		/* |ttyname_r(3c)| */
#include	<fcntl.h>
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<usystem.h>
#include	<localmisc.h>

import libutil ;

/* local defines */


/* imported namespaces */


/* local typedefs */


/* external subroutines */


/* external variables */


/* forward references */


/* local variables */


/* exported variables */


/* exported subroutines */

int uc_ttyname(int fd,char *dbuf,int dlen) noex {
	int		rs = SR_FAULT ;
	int		len = 0 ;
	if (dbuf) {
	    rs = SR_BADF ;
	    if (fd >= 0) {
		rs = SR_INVALID ;
		if (dlen >= 0) {
		    if ((rs = ttyname_rp(fd,dbuf,dlen)) == 0) {
	    	        len = xstrnlen(dbuf,dlen) ;
		    } else {
			rs = (- rs) ; /* returned an ERRNO code */
		    }
		} /* end if (valid) */
	    } /* end if (valid) */
	} /* end if (non-null) */
	return (rs >= 0) ? len : rs ;
}
/* end subroutine (uc_ttyname) */


