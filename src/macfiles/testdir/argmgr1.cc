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
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>
#include	<ulogerror.h>
#include	<mkchar.h>
#include	<ischarx.h>
#include	<localmisc.h>

module argmgr ;

import libutil ;

/* local defines */

#define	ARGMGR_MAGIC		0x97873787


/* imported namespaces */


/* local typedefs */


/* external subroutines */


/* local structures */


/* forward references */


/* forward refernces */

template<typename ... Args>
static inline int argmgr_magic(argmgr *op,Args ... args) noex {
	int		rs = SR_FAULT ;
	if (op && (args && ...)) {
	    rs = (op->magic == ARGMGR_MAGIC) ? SR_OK : SR_NOTOPEN ;
	}
	return rs ;
}
/* end subroutine (argmgr_magic) */


/* local variables */


/* exported variables */


/* exported subroutines */


/* local subroutines */

int argmgr::istart() noex {
    	int		rs = SR_FAULT ;
	if (argv) {
	    rs = SR_INVALID ;
	    if (argc >= 0) {
	        if ((rs = amap.start) >= 0) {
	            magic = ARGMGR_MAGIC ;
	        }
	    } /* end if (valid) */
	} /* end if (non-null) */
	return rs ;
} /* end method (argmgr::istart) */

int argmgr::ifinish() noex {
    	int		rs ;
	int		rs1 ;
	if ((rs = argmgr_magic(this)) >= 0) {
	    {
	        rs1 = amap.finish ;
	        if (rs >= 0) rs = rs1 ;
	    }
	    {
		argv = nullptr ;
		argc = 0 ;
	    }
	    magic = 0 ;
	} /* end if (magic) */
	return rs ;
} /* end method (argmgr::ifinish) */

int argmgr::iarg() noex {
    	int		rs ;
	if ((rs = argmgr_magic(this)) >= 0) {
	    rs = int(++ai < argc) ;
	} /* end if (magic) */
	return rs ;
} /* end method (argmgr::iarg) */

int argmgr::argopt(cchar **rpp) noex {
    	int		rs = SR_OK ;
	if (ai < argc) {
	    cchar	*ap = argv[ai] ;
	    if (isplusminus(ap[0])) {
		fl.plus = (ap[0] == '+') ;
		if (cint ch = mkchar(ap[1]) ; ch) {
		    if (isalphalatin(ch)) {
			if ((rs = amap.set[ai]) >= 0) {
		            if (rpp) *rpp = (ap + 1) ;
		            rs = xstrlen(ap + 1) ;
			}
		    }
		}
	    }
	}
	return rs ;
} /* end method (argmgr::argopt) */

int argmgr::argoptlong(cchar **rpp) noex {
    	int		rs = SR_OK ;
	if (ai < argc) {
	    cchar	*ap = argv[ai] ;
	    if ((ap[0] == '-') && (ap[1] == '-')) {
		if (cint ch = mkchar(ap[2]) ; ch) {
		    if (isalphalatin(ch)) {
			if ((rs = amap.set[ai]) >= 0) {
		            if (rpp) *rpp = (ap + 1) ;
		            rs = xstrlen(ap + 1) ;
			}
		    } else {
		        argoptdone = true ;
		        aie = ai ;
		    }
		} else {
		        argoptdone = true ;
		        aie = ai ;
		}
	    }
	}
	return rs ;
} /* end method (argmgr::argoptlong) */

int argmgr::iargchar() noex {
    	int		rs = SR_OK ;
	return rs ;
}

int argmgr::ipositional() noex {
    	int		rs = SR_OK ;
	return rs ;
}

int argmgr::icount() noex {
    	int		rs = SR_OK ;
	return rs ;
} /* end method (argmgr::icount) */

argmgr::operator int () noex {
    	int		rs ;
	if ((rs = argmgr_magic(this)) >= 0) {
	    rs = argc ;
	}
	return rs ;
} /* end method (argmgr::operator) */

void argmgr::dtor() noex {
	if (cint rs = finish ; rs < 0) {
	    ulogerror("argmgr",rs,"fini-finish") ;
	}
}

argmgr_co::operator int () noex {
	int		rs = SR_BUGCHECK ;
	if (op) {
	    switch (w) {
	    case argmgrmem_start:
	        rs = op->istart() ;
	        break ;
	    case argmgrmem_finish:
	        rs = op->ifinish() ;
	        break ;
	    case argmgrmem_arg:
	        rs = op->iarg() ;
	        break ;
	    case argmgrmem_argchar:
		rs = op->iargchar() ;
		break ;
	    case argmgrmem_posarg:
	    case argmgrmem_positional:
		rs = op->ipositional() ;
		break ;
	    case argmgrmem_count:
	        rs = op->icount() ;
	        break ;
	    default:
		rs = SR_INVALID ;
		break ;
	    } /* end switch */
	} /* end if (non-null) */
	return rs ;
} /* end method (argmgr_co) */


