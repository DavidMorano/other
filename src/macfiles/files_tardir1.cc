/* files_tardir1 MODULE (module-interface-unit) */
/* charset=ISO8859-1 */
/* lang=C++20 (conformance reviewed) */

/* enumerate filenames */
/* version %I% last-modified %G% */


/* revision history:

	= 2017-03-21, David A-D- Morano
	This subroutine was originally written.

*/

/* Copyright © 2017 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Names:
	files_tardir

*******************************************************************************/

module ;

#include	<envstandards.h>	/* ordered first to configure */
#include	<unistd.h>
#include	<fcntl.h>
#include	<climits>		/* |INT_MAX| */
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<new>			/* |nothrow(3c++)| */
#include	<usyscalls.h>		/* |u_stat(3u)| */
#include	<strnul.hh>
#include	<strn.h>
#include	<sfx.h>
#include	<six.h>
#include	<strwcpy.h>
#include	<localmisc.h>

module files_tardir ;

import sif ;
import debug ;

/* local defines */


/* imported namespaces */

using std::nullptr_t ;			/* type */
using std::nothrow ;			/* constant */


/* local typedefs */


/* external subroutines */


/* external variables */


/* local structures */


/* forward references */


/* local variables */


/* exported variables */


/* exported subroutines */


/* local subroutines */

int tardir::istart() noex {
    	int		rs ;
	if ((rs = dirs.start) >= 0) {
	    if ((rs = seen.start) >= 0) {
		fl.open = true ;
	    }
	    if (rs < 0) {
		dirs.finish() ;
	    }
	} /* end if (dirs.start) */
	return rs ;
}

int tardir::ifinish() noex {
    	int		rs = SR_NOTOPEN ;
	int		rs1 ;
	if (fl.open) {
	    rs = SR_OK ;
	    {
		rs1 = seen.finish ;
		if (rs >= 0) rs = rs1 ;
	    }
	    {
		rs1 = dirs.finish ;
		if (rs >= 0) rs = rs1 ;
	    }
	    fl.open = false ;
	} /* end if (open) */
	return rs ;
}

int tardir::icount() noex {
    	int		rs = SR_NOTOPEN ;
	int		c = 0 ;
	if (fl.open) {
	    rs = dirs.count ;
	    c = rs ;
	} /* end if (non-null) */
	return (rs >= 0) ? c : rs ;
}

int tardir::add(cchar *sp,int sl) noex {
    	int		rs = SR_FAULT ;
	int		c = 0 ;
	if (sp) {
	    rs = SR_NOTOPEN ;
	    if (fl.open) {
	        sif so(sp,sl,':') ;
	        cchar *cp ;
	        for (int cl ; (cl = so(&cp)) > 0 ; ) {
	            strnul dn(cp,cl) ; 
	            if (ustat sb ; (rs = u_stat(dn,&sb)) >= 0) {
		        rs = SR_NOTDIR ;
		        if (S_ISDIR(sb.st_mode)) {
		            rs = iaddone(sp,sl,&sb) ;
			    c += rs ;
		        }
	            } /* end if (u_stat) */
		    if (rs < 0) break ;
	        } /* end for */
	    } /* end if (open) */
	} /* end if (non-null) */
	return (rs >= 0) ? c : rs ;
}

int tardir::get(int idx,ccharpp rpp) noex {
    	int		rs = SR_NOTOPEN ;
	if (fl.open) {
	    rs = dirs.get(idx,rpp) ;
	}
	return (rs >= 0) ? idx : rs ;
}

int tardir::curbegin(tardir_cur *curp) noex {
    	int		rs = SR_FAULT ;
	if (curp) {
	    rs = SR_NOTOPEN ;
	    if (fl.open) {
	        curp->i = -1 ;
	        rs = SR_OK ;
	    }
	} /* end if (non-null) */
	return rs ;
} /* end method (tardir::curbegin) */

int tardir::curend(tardir_cur *curp) noex {
    	int		rs = SR_FAULT ;
	if (curp) {
	    rs = SR_NOTOPEN ;
	    if (fl.open) {
	        curp->i = -1 ;
	        rs = SR_OK ;
	    }
	} /* end if (non-null) */
	return rs ;
} /* end method (tardir::curend) */

int tardir::curenum(tardir_cur *curp,ccharpp rpp) noex {
    	int		rs = SR_FAULT ;
	if (curp && rpp) {
	    rs = SR_NOTOPEN ;
	    if (fl.open) {
		cint idx = (curp->i >= 0) ? (curp->i + 1) : 0 ;
	        if ((rs = get(idx,rpp)) >= 0) {
		    curp->i = idx ;
		}
	    } /* end if (open) */
	} /* end if (non-null) */
	return rs ;
} /* end method (tardir::curenum) */

int tardir::iaddone(cchar *sp,int sl,ustat *sbp) noex {
    	int		rs ;
	int		c = 0 ;
	if ((rs = seen.checkin(sbp)) > 0) {
	    rs = dirs.add(sp,sl) ;
	    if (rs < INT_MAX) c += rs ;
	} /* end if (ssen.checkin) */
	return (rs >= 0) ? c : rs ;
}

void tardir::dtor() noex {
	if (cint rs = finish ; rs < 0) {
	    ulogerror("tardir",rs,"fini-finish") ;
	}
}

tardir::operator int () noex {
	return icount() ;
}

int tardir_co::operator () (int) noex {
	int		rs = SR_BUGCHECK ;
	if (op) {
	    switch (w) {
	    case tardirmem_start:
	        rs = op->istart() ;
	        break ;
	    case tardirmem_count:
	        rs = op->icount() ;
	        break ;
	    case tardirmem_finish:
	        rs = op->ifinish() ;
	        break ;
	    } /* end switch */
	} /* end if (non-null) */
	return rs ;
} /* end method (tardir_co::operator) */


