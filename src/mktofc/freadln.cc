/* freadln SUPPORT */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* read a coded line from the STDIO stream */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-08-17, David A­D­ Morano
	This code was originally written.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Name:
	freadln
	fgetline

	Description:
	This routine will only read at most 'len' number of bytes
	from the file.  Note that the semantics of this call are not
	the same as |fgets(3c)|.  This call will write a NUL character
	into the user buffer after the supplied length of the buffer
	is used up.  With |fgets(3c)|, it will never write more
	than the supplied length of bytes.

	Synopsis:
	int freadln(FILE *fp,char *rbuf,int rlen) noex
	int fgetline(FILE *fp,char *rbuf,int rlen) noex

	Arguments:
	fp		pointer to FILE object
	rbuf		result buffer pointer
	rlen		resuly buffer length

	Returns:
	>=0		length of data (in bytes) transferred
	<0		error (system-return)

*******************************************************************************/

#include	<envstandards.h>	/* must be ordered first to configure */
#include	<cerrno>
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<cstdio>
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>
#include	<localmisc.h>

#include	"freadln.h"

#pragma		GCC dependency		"mod/libutil.ccm"

import libutil ;			/* |lenstr(3u)| */

/* local defines */


/* external subroutines */

extern "C" {
    int		freadln(FILE *,char *,int) noex ;
    int		fgetline(FILE *,char *,int) noex ;
}


/* external variables */


/* local structures */


/* forward references */


/* local variables */


/* exported variables */


/* exported subroutines */

int freadln(FILE *fp,char *rbuf,int rlen) noex {
	int		rs = SR_FAULT ;
	int		len = 0 ;
	if (fp && rbuf) {
	    rs = SR_INVALID ;
	    if (rlen > 0) {
	        if (char *bp = fgets(rbuf,(rlen + 1),fp) ; bp) {
		    rs = SR_OK ;
		    len = lenstr(bp) ;
		} else {
		    if (feof(fp)) {
			rs = SR_OK ;
		    } else if (ferror(fp)) {
		        rs = (errno) ? (- errno) : SR_IO ;
	                clearerr(fp) ;
	            }
		} /* end if (result) */
	    } /* end if (valid) */
	} /* end if (non-null) */
	return (rs >= 0) ? len : rs ;
}
/* end subroutine (freadln) */

int fgetline(FILE *fp,char *rbuf,int rlen) noex {
	return freadln(fp,rbuf,rlen) ;
}
/* end subroutine (fgetline) */


