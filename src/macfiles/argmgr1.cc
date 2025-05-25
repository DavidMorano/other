/* argmgr1 MODULE */
/* encoding=ISO8859-1 */
/* lang=C++20 */

/* manage program arguments */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-09-10, David A­D­ Morano
	This subroutine was written for Rightcore Network Services.

	= 2020-08-13, David A­D­ Morano
	This code has been modularized (w/ C++20 modules).

*/

/* Copyright © 1998,2020 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

  	Object:
	argmgr

	Description:
	This object manages program arguments.

*******************************************************************************/

module ;

#include	<envstandards.h>	/* MUST be first to configure */
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<new>			/* |nothrow(3c++)| */
#include	<vector>
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>
#include	<ulogerror.h>
#include	<localmisc.h>

module argmgr ;

/* local defines */


/* imported namespaces */


/* local typedefs */


/* external subroutines */


/* local structures */


/* forward references */


/* forward refernces */


/* local variables */


/* exported variables */


/* exported subroutines */


/* local subroutines */

int argmgr::resize(int n) noex {
	    int		rs = SR_INVALID ;
	    if (n >= 0) {
		try {
	            csize nsize = size_t(n) ;
	            vector<bool>::resize(nsize,false) ;
	            rs = SR_OK ;
		} catch (...) {
		    rs = SR_NOMEM ;
		}
	    }
	    return rs ;
} /* end method (argmgr::resize) */

int argmgr::adj(int idx) noex {
	int		rs = SR_OK ;
	try {
	    csize	nsize = size_t(idx + 1) ;
	    vector<bool>::resize(nsize,false) ;
	} catch (...) {
	    rs = SR_NOMEM ;
	}
	return rs ;
} /* end method (argmgr::adj) */

argmgr_co::operator int () noex {
	int		rs = SR_BUGCHECK ;
	if (op) {
	    switch (w) {
	    case argmgrmem_start:
	        rs = SR_OK ;
	        break ;
	    case argmgrmem_finish:
	        rs = SR_OK ;
	        break ;
	    case argmgrmem_extent:
	        rs = int(op->capacity()) ;
	        break ;
	    case argmgrmem_count:
	        rs = int(op->size()) ;
	        break ;
	    default:
		rs = SR_INVALID ;
		break ;
	    } /* end switch */
	} /* end if (non-null) */
	return rs ;
} /* end method (argmgr_co) */

int argmgr_co::operator [] (int idx) noex {
    	int		rs = SR_BUGCHECK ;
	bool		f = false ;
	if (op) {
	    rs = SR_INVALID ;
	    if (idx >= 0) {
	        if ((rs = op->adj(idx)) >= 0) {
                    switch (w) {
                    case argmgrmem_set:
                        (*op)[idx] = true ;
                        break ;
                    case argmgrmem_clr:
                        (*op)[idx] = false ;
                        break ;
                    case argmgrmem_tst:
                        f = (*op)[idx] ;
                        break ;
	            default:
		        rs = SR_INVALID ;
		        break ;
                    } /* end switch */
		    if (rs >= 0) rs = int(f) ;
	        } /* end if (adj) */
	    } /* end if (valid) */
	} /* end if (non-null) */
	return rs ;
} /* end method (argmgr_co) */


