/* snaddw SUPPORT */
/* lang=C++20 */

/* append a counted c-string to a given counted c-string */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-11-01, David A­D­ Morano
	This subroutine was originally written.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Name:
	snaddw

	Description:
	Add some c-string to the end of an existing string.  Return the
	incremental amount added.

	Synopsis:
	int snaddw(char *rbuf,int rlen,int rl,cchar *sp,int sl) noex

	Arguments:
	rbuf		c-string buffer pointer
	rlen		c-string buffer length
	rl		length of buffer already filled
	sp		new c-string pointer to be added
	sl		new c-string length to be added

	Returns:
	>=0		the amount of additional string added to
			the existing c-string buffer
	<0		error (system-return)

*******************************************************************************/

#include	<envstandards.h>	/* MUST be first to configure */
#include	<usystem.h>

#include	"snaddw.h"


/* local defines */


/* external subroutines */


/* external variables */


/* local structures */


/* foward references */


/* local variables */


/* exported variables */


/* exported subroutines */

int snaddw(char *rbuf,int rlen,int i,cchar *sp,int sl) noex {
	int		rs = SR_FAULT ;
	int		len = 0 ;
	if (rbuf && sp) {
 	    if (i >= 0) {	
	        char	*bp = (rbuf + i) ;
	        rs = SR_OK ;
	        if (rlen < 0) {
	            if (sl < 0) {
	                while (*sp) {
	                    *bp++ = *sp++ ;
		        }
	            } else {
	                while (sl && *sp) {
	                    *bp++ = *sp++ ;
	                    sl -= 1 ;
	                }
	            } /* end if */
	        } else {
	            if (sl < 0) {
	                while ((bp < (rbuf + rlen)) && *sp) {
	                    *bp++ = *sp++ ;
		        }
		        if ((bp == (rbuf + rlen)) && (*sp != '\0')) {
		            rs = SR_OVERFLOW ;
		        }
	            } else {
	                while ((bp < (rbuf + rlen)) && sl && *sp) {
	                    *bp++ = *sp++ ;
	                    sl -= 1 ;
	                }
		        if ((bp == (rbuf + rlen)) && sl) {
		            rs = SR_OVERFLOW ;
		        }
	            } /* end if */
	        } /* end if */
	        *bp = '\0' ;
		len = (bp - (rbuf + i)) ;
	    } /* end if (valid) */
	} /* end if (non-null) */
	return (rs >= 0) ? len : rs ;
}
/* end subroutine (snaddw) */


