/* storebufxx SUPPORT */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* storage buffer manipulation subroutines */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-03-24, David A­D­ Morano
	This object module was originally written.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

  	Name:
	storebuf

	Description:
	This subroutine can be used to construct strings or messages
	in a buffer WITHOUT using the |sprintf(3c)| subroutine.
	This module is useful when the user supplies a buffer of a
	specified length and does not want to track the creation
	and destruction of an associated object.  There is NO object
	(besides the user supplied buffer -- which can be considered
	THE object) to create and then destroy when using this
	module.  The user must carefully track the buffer usage so
	that subsequent calls can be supplied with the correct index
	value of the next available (unused) byte in the buffer.

	Example usage:

	rs = 0 ;
	i = 0 ;
	if (rs >= 0) {
	    rs = storebuf_strw(rbuf,rlen,i,sp,sl) ;
	    i += rs ;
	}

	if (rs >= 0) {
	    rs = storebuf_buf(rbuf,rlen,i,bp,bl) ;
	    i += rs ;
	}

	if (rs >= 0) {
	    rs = storebuf_deci(rbuf,rlen,i,value) ;
	    i += rs ;
	}

	if (rs >= 0) {
	    rs = storebuf_strw(rbuf,rlen,i,sp,sl) ;
	    i += rs ;
	}

*******************************************************************************/

#include	<envstandards.h>	/* MUST be first to configure */
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<clanguage.h>
#include	<usysbase.h>
#include	<localmisc.h>		/* |DIGBUFLEN| */

#include	"storebuf.h"

#pragma		GCC dependency		"mod/libutil.ccm"

import libutil ;			/* |memcopy(3u)| */

/* local defines */


/* local typedefs */

template<typename T> using ctxxx_f = int (*)(char *,int,T) noex ;


/* external subroutines */


/* external variables */


/* local structures */


/* forward references */


/* local variables */


/* exported variables */


/* exported subroutines */

int storebuf_chrs(char *rbuf,int rlen,int idx,int ch,int n) noex {
	int		rs = SR_FAULT ;
	if (rbuf) ylikely {
	    rs = SR_INVALID ;
	    if ((idx >= 0) && (n >= 0)) ylikely {
	        char	*bp = (rbuf + idx) ;
		rs = SR_OK ;
	        if ((rlen < 0) || ((rlen - idx) >= n)) ylikely {
		    for (int i = 0 ; i < n ; i += 1) {
	                *bp++ = char(ch) ;
		    }
	        } else {
	            rs = SR_OVERFLOW ;
	        }
	        *bp = '\0' ;
	    } /* end if (valid) */
	} /* end if (non-null) */
	return (rs >= 0) ? 1 : rs ;
}
/* end subroutine (storebuf_chrs) */

int storebuf_chr(char *rbuf,int rlen,int i,int ch) noex {
	int		rs = SR_FAULT ;
	if (rbuf) ylikely {
	    rs = SR_INVALID ;
	    if (i >= 0) ylikely {
	        char	*bp = (rbuf + i) ;
		rs = SR_OK ;
	        if ((rlen < 0) || ((rlen - i) >= 1)) ylikely {
	            *bp++ = char(ch) ;
	        } else {
	            rs = SR_OVERFLOW ;
	        }
	        *bp = '\0' ;
	    } /* end if (valid) */
	} /* end if (non-null) */
	return (rs >= 0) ? 1 : rs ;
}
/* end subroutine (storebuf_chr) */

int storebuf_strw(char *rbuf,int rlen,int i,cchar *sp,int sl) noex {
	int		rs = SR_FAULT ;
	int		len = 0 ;
	if (rbuf && sp) ylikely {
 	    if (i >= 0) ylikely {	
	        char	*bp = (rbuf + i) ;
	        rs = SR_OK ;
	        if (rlen < 0) ylikely {
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
		len = intconv(bp - (rbuf + i)) ;
	    } /* end if (valid) */
	} /* end if (non-null) */
	return (rs >= 0) ? len : rs ;
}
/* end subroutine (storebuf_strw) */


