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

	Object:
	strmgr

	Description:
	This object performs some simple string composition management.
	The caller supplies a destination (result) string-buffer
	(with a length), and then using "builder" methods composes
	what the caller wants in the result (a composed string).

*******************************************************************************/

#include	<envstandards.h>	/* MUST be first to configure */
#include	<clanguage.h>
#include	<usysbase.h>
#include	<ulogerror.h>
#include	<localmisc.h>

#include	"strmgr.h"


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
	    rs = SR_INVALID ;
	    if (dlen >= 0) {
	        op->dbuf = dbuf ;
	        op->dlen = dlen ;
	        op->dl = 0 ;
		rs = SR_OK ;
	    } /* end if (valid) */
	} /* end if (non-null) */
	return rs ;
}
/* end subroutine (strmgr_start) */

int strmgr_finish(strmgr *op) noex {
	int		rs = SR_FAULT ;
	if (op) ylikely {
	    rs = op->dl ;
	    op->dbuf[0] = '\0' ;
	    op->dbuf = nullptr ;
	    op->dlen = 0 ;
	    op->dl = 0 ;
	} /* end if (non-null) */
	return rs ;
}
/* end subroutine (strmgr_finish) */

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

int strmgr_lenrem(strmgr *op) noex {
	return strmgr_avail(op) ;
}
/* end subroutine (strmgr_lenrem) */

int strmgr_str(strmgr *op,cchar *sp,int sl) noex {
	int		rs = SR_FAULT ;
	int		tl = 0 ;
	if (op && sp) ylikely {
	    cint	rlen = (op->dlen - op->dl) ;
	    rs = SR_OK ;
	    while ((rs >= 0) && sl-- && *sp) {
	        if (tl < rlen) ylikely {
	            op->dbuf[op->dl + tl++] = *sp++ ;
	        } else {
	            rs = SR_OVERFLOW ;
	        }
	    } /* end for */
	    if (rs >= 0) ylikely {
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
	        op->dbuf[op->dl++] = char(ch) ;
	    } else {
	        rs = SR_OVERFLOW ;
	    }
	} /* end if (non-null) */
	return (rs >= 0) ? 1 : rs ;
}
/* end subroutine (strmgr_chr) */

int strmgr_get(strmgr *op,ccharpp rpp) noex {
	int		rs = SR_FAULT ;
	if (op) ylikely {
	    rs = op->dl ;
	    if (rpp) *rpp = op->dbuf ;
	} /* end if (non-null) */
	return rs ;
}
/* end subroutine (strmgr_len) */

int strmgr_len(strmgr *op) noex {
	int		rs = SR_FAULT ;
	if (op) ylikely {
	    rs = op->dl ;
	} /* end if (non-null) */
	return rs ;
}
/* end subroutine (strmgr_len) */

int strmgr_reset(strmgr *op) noex {
	int		rs = SR_FAULT ;
	if (op) ylikely {
	    rs = SR_OK ;
	    op->dl = 0 ;
	} /* end if (non-null) */
	return rs ;
}
/* end subroutine (strmgr_reset) */


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

int strmgr::get(ccharpp rpp) noex {
    	return strmgr_get(this,rpp) ;
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
	    case strmgrmem_len:
	        rs = strmgr_len(op) ;
	        break ;
	    case strmgrmem_reset:
	        rs = strmgr_reset(op) ;
	        break ;
	    case strmgrmem_finish:
	        rs = strmgr_finish(op) ;
	        break ;
	    } /* end switch */
	} /* end if (non-null) */
	return rs ;
} /* end method (strmgr_co::operator) */


