/* strmgr SUPPORT */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* string compostion management */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-08-10 David A-D- Morano
	This was written from scratch.

*/

/* Copyright © 1998 David A-D- Morano.  All rights reserved. */

/*******************************************************************************

	Name:
	strmgr

	Description:
	We perform some simple string composition management.  The
	caller supplies a destination (result) string-buffer (with
	a length), and then using "builder" methods composes what
	she wants in the result.

*******************************************************************************/

#include	<envstandards.h>	/* MUST be first to configure */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>
#include	<ulogerror.h>
#include	<localmisc.h>

#include	"strmgr.h"

import libutil ;

/* local defines */


/* external subroutines */


/* external variables */


/* local structures */


/* forward references */


/* local variables */


/* exported variables */


/* exported subroutines */

int strmgr_start(strmgr *op,char *dbuf,int dlen) noex {
	int		rs = SR_FAULT ;
	if (op && dbuf) ylikely {
	    rs = SR_OK ;
	    if (dlen < 0) dlen = lenstr(dbuf) ;
	    op->dp = dbuf ;
	    op->dlen = dlen ;
	    op->dl = 0 ;
	} /* end if (non-null) */
	return rs ;
}
/* end subroutine (strmgr_start) */

int strmgr_avail(strmgr *op) noex {
	int		rs = SR_FAULT ;
	if (op) ylikely {
	    rs = (op->dlen - op->dl) ;
	}
	return rs ;
}
/* end subroutine (strmgr_avail) */

int strmgr_rem(strmgr *op) noex {
	return strmgr_avail(op) ;
}
/* end subroutine (strmgr_rem) */

int strmgr_str(strmgr *op,cchar *sp,int sl) noex {
	int		rs = SR_FAULT ;
	int		tl = 0 ;
	if (op && sp) ylikely {
	    cint	rlen = (op->dlen - op->dl) ;
	    rs = SR_OK ;
	    while ((rs >= 0) && sl-- && *sp) {
	        if (tl < rlen) ylikely {
	            op->dp[tl++] = *sp++ ;
	        } else {
	            rs = SR_OVERFLOW ;
	        }
	    } /* end for */
	    if (rs >= 0) ylikely {
	        op->dp += tl ;
	        op->dl += tl ;
	    }
	} /* end if (non-null) */
	return (rs >= 0) ? tl : rs ;
}
/* end subroutine (strmgr_str) */

int strmgr_chr(strmgr *op,int ch) noex {
	int		rs = SR_FAULT ;
	if (op) ylikely {
	    rs = SR_OK ;
	    if (op->dl < op->dlen) ylikely {
	        *op->dp++ = char(ch) ;
	        op->dl += 1 ;
	    } else {
	        rs = SR_OVERFLOW ;
	    }
	} /* end if (non-null) */
	return (rs >= 0) ? 1 : rs ;
}
/* end subroutine (strmgr_chr) */

int strmgr_finish(strmgr *op) noex {
	int		rs = SR_FAULT ;
	if (op) ylikely {
	    rs = op->dl ;
	    op->dp[0] = '\0' ;
	    op->dp = nullptr ;
	    op->dl = 0 ;
	    op->dlen = 0 ;
	} /* end if (non-null) */
	return rs ;
}
/* end subroutine (strmgr_finish) */


/* local subroutines */

int strmgr::start(char *sp,int sl) noex {
    	return strmgr_start(this,sp,sl) ;
}

int strmgr::str(cchar *sp,int sl) noex {
    	return strmgr_str(this,sp,sl) ;
}

int strmgr::chr(int ch) noex {
    	return strmgr_chr(this,ch) ;
}

void strmgr::dtor() noex {
    	if (cint rs = finish ; rs < 0) {
	    ulogerror("strmgr",rs,"dtor-finish") ;
	}
} /* end method (strmgr::dtor) */

strmgr_co::operator int () noex {
	int		rs = SR_BUGCHECK ;
	if (op) ylikely {
	    switch (w) {
	    case strmgrmem_avail:
	        rs = strmgr_avail(op) ;
	        break ;
	    case strmgrmem_rem:
	        rs = strmgr_rem(op) ;
	        break ;
	    case strmgrmem_finish:
	        rs = strmgr_finish(op) ;
	        break ;
	    } /* end switch */
	} /* end if (non-null) */
	return rs ;
} /* end method (strmgr_co::operator) */


