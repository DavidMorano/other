/* pathxx SUPPORT */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* add a component (c-string) to an existing path (c-string) */
/* version %I% last-modified %G% */


/* revision history:

	= 2002-07-13, David A­D­ Morano
	This was originally written.

*/

/* Copyright © 2002 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Name:
	pathaddw
	pathaddx
	pathadd

	Description:
	This subroutine adds a new component to an existing file-path.
	These subroutines return the total length of the final path.
	The length of the supplied buffer for the added-to path is
	assumed to be MAXPATHLEN (determined dynamically).

	Synopses:
	int pathnaddw(char *pbuf,int plen,int pl,cchar *sp) noex
	int pathnaddx(char *pbuf,int plen,int pl,cchar *sp) noex
	int pathaddw(char *pbuf,int pl,cchar *sp,int sl) noex
	int pathaddx(char *pbuf,int pl,int n,cchar *sp ...) noex

	Arguments:
	pbuf		result buffer pointer
	pl		length of result buffer that is already filled
	n		number of arguments
	sp		new-componment c-string pointer
	sl		new-componment c-string length

	Returns:
	>=0		new length of new path; total length from beginning
	<0		error (system-return)

	Note:
	These subroutines return the total length of the final path
	that resides in the given path-result-buffer.  If you want
	a subroutine that returns just the added part, then check
	out the |storebuf(3uc)| facility or the |snadd(3uc)| facility,
	or simply create your own using the various |snx(3uc)|
	subroutines (and other methods also).  In the subroutines
	presented here, the length of the supplied buffer for the
	added-to path is assumed to be MAXPATHLEN (determined
	dynamically), and so is not needed as a call argument.

	Extra:
	The |pathaddw| subroutine is almost equivalent to:
	    int pathaddw(char *rbuf,int rl,cc *sp,int sl) noex {
	        int	rs ;
	        if ((rs = getbufsize(bufsize_mp)) >= 0) {
		    cint	rlen = rs ;
	            rs = storebuf_strw(rbuf,rlen,rl,sp,sl) ;
		    rl += rs ;
	        }
	        return (rs >= 0) ? rl : rs ;
	    }

*******************************************************************************/

#include	<envstandards.h>	/* MUST be first to configure */
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<cstdarg>
#include	<clanguage.h>
#include	<usysbase.h>
#include	<usyscalls.h>
#include	<storebuf.h>
#include	<localmisc.h>

#include	"pathadd.h"

#pragma		GCC dependency		"mod/libutil.ccm"
#pragma		GCC dependency		"mod/ulibvals.ccm"

import libutil ;			/* ?? */
import ulibvals ;			/* |ulibval(3u)| */

/* local defines */


/* imported namespaces */


/* local typedefs */


/* external subroutines */


/* external variables */


/* local structures */


/* forward references */

local int		local_pathadd(char *,int,int,cchar *,int) noex ;


/* local variables */

cint			maxpathlen = ulibval.maxpathlen ;


/* exported variables */


/* exported subroutines */

int pathnaddw(char *pbuf,int plen,int pl,cchar *sp,int sl) noex {
    	int		rs = SR_FAULT ;
	if (pbuf && sp) {
	    rs = SR_INVALID ;
	    if ((plen >= 0) && (pl >= 0)) {
		rs = local_pathadd(pbuf,plen,pl,sp,sl) ;
		pl = rs ;
	    } /* end if (valid) */
	} /* end if (non-null) */
	return (rs >= 0) ? pl : rs ;
}
/* end subroutine (pathnaddw) */

int pathnaddx(char *pbuf,int plen,int pl,int n,...) noex {
	va_list		ap ;
	int		rs = SR_FAULT ;
	if (pbuf) {
	    rs = SR_INVALID ;
	    if ((plen >= 0) && (pl >= 0)) {
	        va_begin(ap,n) ;
	        for (int i = 0 ; (rs >= SR_OK) && (i < n) ; i += 1) {
		    cchar	*sp = (char *) va_arg(ap,char *) ;
		    rs = local_pathadd(pbuf,plen,pl,sp,-1) ;
		    pl = rs ;
	        } /* end for */
	        va_end(ap) ;
	    } /* end if (valid) */
	} /* end if (non-null) */
	return (rs >= 0) ? pl : rs ;
}
/* end subroutine (pathnaddx) */

int pathaddw(char *pbuf,int pl,cchar *sp,int sl) noex {
	int		rs = SR_FAULT ;
	if (pbuf && sp) {
	    rs = SR_INVALID ;
	    if (pl >= 0) {
	        if ((rs = maxpathlen) >= 0) {
		    cint	plen = rs ;
		    rs = local_pathadd(pbuf,plen,pl,sp,sl) ;
		    pl = rs ;
	        } /* end if (maxpathlen) */
	    } /* end if (valid) */
	} /* end if (non-null) */
	return (rs >= 0) ? pl : rs ;
}
/* end subroutine (pathaddw) */

int pathaddx(char *pbuf,int pl,int n,...) noex {
	va_list		ap ;
	int		rs = SR_FAULT ;
	if (pbuf) {
	    rs = SR_INVALID ;
	    if (pl >= 0) {
	        if ((rs = maxpathlen) >= 0) {
		    cint	plen = rs ;
	            va_begin(ap,n) ;
	            for (int i = 0 ; (rs >= SR_OK) && (i < n) ; i += 1) {
		        cchar	*sp = (char *) va_arg(ap,char *) ;
		        rs = local_pathadd(pbuf,plen,pl,sp,-1) ;
		        pl = rs ;
	            } /* end for */
	            va_end(ap) ;
	        } /* end if (maxpathlen) */
	    } /* end if (valid) */
	} /* end if (non-null) */
	return (rs >= 0) ? pl : rs ;
}
/* end subroutine (pathaddx) */


/* local subroutines */

local int local_pathadd(char *pbuf,int plen,int pl,cchar *sp,int sl) noex {
	int		rs = SR_OK ;
	if ((pl > 0) && (pbuf[pl - 1] != '/')) {
	    rs = storebuf_chr(pbuf,plen,pl,'/') ;
	    pl += rs ;
	}
	if (rs >= 0) {
	    rs = storebuf_strw(pbuf,plen,pl,sp,sl) ;
	    pl += rs ;
	}
	return (rs >= 0) ? pl : rs ;
}
/* end subroutine (local_pathadd) */


