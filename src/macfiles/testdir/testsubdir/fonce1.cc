/* fonce1 SUPPORT */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* (File-Once) implement a map container to track file device-inode values */
/* version %I% last-modified %G% */


/* revision history:

	= 2011-04-12, David A­D­ Morano
	This code was originally written.  Only the introduction
	of C++11 has allowed this (finally), due to the addition
	(in C++11) of |unordered_map(3c++)|.

*/

/* Copyright © 2011 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Object:
	fonce

	Description:
	This object implements a map container of blocks (of a given
	structure).

	Symopsis:
	int fonce_start(int n = 0) noex

	Arguments:
	n		suggested starting length

	Returns:
	>=0		ok
	<0		error (system-return)

*******************************************************************************/

module ;

#include	<envstandards.h>	/* ordered first to configure */
#include	<sys/stat.h>		/* |dev_t| + |ino_t| */
#include	<unistd.h>
#include	<fcntl.h>
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<new>			/* placement-new + |nothrow(3c++)| */
#include	<utility>		/* |pair(3c++)| */
#include	<unordered_set>
#include	<clanguage.h>
#include	<usysbase.h>
#include	<ulogerror.h>
#include	<localmisc.h>

#pragma		GCC dependency	"mod/fonce.ccm"

module fonce ;

/* local defines */


/* imported namespaces */

using std::unordered_set ;              /* type */
using std::pair ;                       /* type */
using std::nothrow ;			/* constant */


/* local typedefs */

typedef fonce::stype::iterator	setiter ;


/* external subroutines */


/* external variables */


/* local structures */


/* forward references */


/* local variables */


/* exported variables */


/* exported subroutines */


/* local subroutines (methods) */

int fonce::istart(int n) noex {
	cnullptr	np{} ;
	int		rs = SR_INVALID ;
	if (n >= 0) ylikely {
	    if (n == 0) n = deftablen ;
	    try {
	        rs = SR_NOMEM ;
	        if ((setp = new(nothrow) stype(n)) != np) ylikely {
	            rs = SR_OK ;
	        } /* end if (new-stype) */
	    } catch (...) {
		rs = SR_NOMEM ;
	    }
	} /* end if (valid) */
	return rs ;
} /* end method (fonce::istart) */

int fonce::ifinish() noex {
	int		rs = SR_NOTOPEN ;
	if (setp) ylikely {
	    delete setp ;
	    setp = nullptr ;
	    rs = SR_OK ;
	}
	return rs ;
} /* end method (fonce::ifinish) */

int fonce::checkin(custat *sbp) noex {
	int		rs = SR_FAULT ;
	int		f = false ; /* return-value: 1=unique, 0=not_unique */
	if (sbp) ylikely {
	    rs = SR_BUGCHECK ;
	    if (setp) ylikely {
	        fonce_devino	k(sbp->st_dev,sbp->st_ino) ;
		try {
	            pair<setiter,bool>	ret = setp->insert(k) ;
		    rs = SR_OK ;
		    f = ret.second ;
		} catch (...) {
		    rs = SR_NOMEM ;
		}
	    } /* end if (initialize) */
	} /* end if (non-null) */
	return (rs >= 0) ? f : rs ;
} /* end method (fonce::checkin) */

int fonce::icount() noex {
	int		rs = SR_BUGCHECK ;
	if (setp) ylikely {
	    csize cnt = setp->size() ;
	    rs = intconv(cnt) ;
	} /* end if (non-null) */
	return rs ;
} /* end method (fonce::icount) */

void fonce::dtor() noex {
	if (cint rs = finish ; rs < 0) {
	    ulogerror("fonce",rs,"fini-finish") ;
	}
} /* end method (fonce::dtor) */

int fonce_co::operator () (int a) noex {
	int		rs = SR_BUGCHECK ;
	if (op) ylikely {
	    switch (w) {
	    case foncemem_start:
	        rs = op->istart(a) ;
	        break ;
	    case foncemem_finish:
	        rs = op->ifinish() ;
	        break ;
	    case foncemem_count:
	        rs = op->icount() ;
	        break ;
	    } /* end switch */
	} /* end if (non-null) */
	return rs ;
} /* end method (fonce_co::operator) */


