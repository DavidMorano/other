/* vecbool1 MODULE */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* manage a variable length bit-array */
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
	vecbool

	Description:
	This object implements an array of bits (bools).

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
#include	<intsat.h>
#include	<localmisc.h>

module vecbool ;

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

int vecbool::resize(int n) noex {
	int		rs = SR_INVALID ;
	if (n >= 0) ylikely {
	    try {
	        csize nsize = size_t(n) ;
	        vector<bool>::resize(nsize,false) ;
	        rs = SR_OK ;
	    } catch (...) {
		rs = SR_NOMEM ;
	    }
	} /* end if (valid) */
	return rs ;
} /* end method (vecbool::resize) */

int vecbool::icount() noex {
    	csize	cnt = size() ;
    	return intsat(cnt) ;
}

int vecbool::adj(int idx) noex {
	int		rs = SR_OK ;
	try {
	    csize	nsize = size_t(idx + 1) ;
	    vector<bool>::resize(nsize,false) ;
	} catch (...) {
	    rs = SR_NOMEM ;
	}
	return rs ;
} /* end method (vecbool::adj) */

void vecbool::dtor() noex {
	if (cint rs = finish ; rs < 0) {
	    ulogerror("vecbool",rs,"fini-finish") ;
	}
}

vecbool_co::operator int () noex {
	int		rs = SR_BUGCHECK ;
	if (op) ylikely {
	    switch (w) {
	    case vecboolmem_start:
		op->fl.open = true ;
	        rs = SR_OK ;
	        break ;
	    case vecboolmem_finish:
		op->fl.open = false ;
	        rs = SR_OK ;
	        break ;
	    case vecboolmem_extent:
	        rs = int(op->capacity()) ;
	        break ;
	    case vecboolmem_count:
	        rs = int(op->size()) ;
	        break ;
	    } /* end switch */
	} /* end if (non-null) */
	return rs ;
} /* end method (vecbool_co) */

int vecbool_co::operator [] (int idx) noex {
    	int		rs = SR_BUGCHECK ;
	if (op) ylikely {
	    rs = SR_INVALID ;
	    if (idx >= 0) ylikely {
		bool	f = false ;
                switch (w) {
                case vecboolmem_set:
	            if ((rs = op->adj(idx)) >= 0) {
                        (*op)[idx] = true ;
		    }
                    break ;
                case vecboolmem_clr:
		    rs = SR_OK ;
		    if (idx < op->icount()) {
                        (*op)[idx] = false ;
		    }
                    break ;
                case vecboolmem_tst:
		    rs = SR_OK ;
		    if (idx < op->icount()) {
                        f = (*op)[idx] ;
		    }
                    break ;
                } /* end switch */
		if (rs >= 0) rs = int(f) ;
	    } /* end if (valid) */
	} /* end if (non-null) */
	return rs ;
} /* end method (vecbool_co) */


