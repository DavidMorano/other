/* fonce SUPPORT */
/* lang=C++20 */

/* implement a map container of blocks (of a given structure) */
/* version %I% last-modified %G% */


/* revision history:

	= 2011-04-12, David A­D­ Morano
	This subroutine was originally written.  Only the introduction
	of C++11 has allowed this (finally), due to the addition
	(in C++11) of |unordered_map(3c++)|.

*/

/* Copyright © 2011 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Description:
	fonce

	Symopsis:
	int fonce_start<typename K,typename Block>(int n = 0) noex

	Arguments:
	n		suggested starting size

	Returns:
	>=0		ok
	<0		error (system-return)

*******************************************************************************/

#include	<envstandards.h>	/* ordered first to configure */
#include	<sys/types.h>
#include	<unistd.h>
#include	<fcntl.h>
#include	<new>
#include	<utility>		/* |pair(3c++)| */
#include	<unordered_set>
#include	<cstring>
#include	<usystem.h>

#include	"fonce.hh"


/* local defines */

#define	FONCE_DEFTABLEN		100


/* imported namespaces */

using std::nullptr_t ;			/* type */
using std::unordered_set ;              /* type */
using std::pair ;                       /* type */
using std::nothrow ;			/* constant */


/* local typedefs */

typedef decltype(std::nothrow)	nothrow_t ;

typedef fonce::stype::iterator		setiter ;


/* external subroutines */


/* external variables */


/* local structures */


/* lcoal variables */


/* exported variables */


/* exported subroutines */


/* local subroutines */

int fonce::istart(int n) noex {
	int		rs = SR_INVALID ;
	if (n >= 0) {
	    if (n == 0) n = FONCE_DEFTABLEN ;
	    try {
		cnullptr	np{} ;
	        rs = SR_NOMEM ;
	        if ((setp = new(nothrow) stype(n)) != np) {
	            rs = SR_OK ;
	        } /* end if (new-stype) */
	    } catch (...) {
		rs = SR_NOMEM ;
	    }
	}
	return rs ;
}
/* end method (fonce::istart) */

int fonce::ifinish() noex {
	int		rs = SR_BUGCHECK ;
	if (setp) {
	    delete setp ;
	    setp = nullptr ;
	    rs = SR_OK ;
	}
	return rs ;
}

int fonce::checkin(CUSTAT *sbp) noex {
	int		rs = SR_FAULT ;
	int		f = false ;
	if (sbp) {
	    fonce_devino	k(sbp->st_dev,sbp->st_ino) ;
	    rs = SR_BUGCHECK ;
	    if (setp) {
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
}

int fonce::icount() noex {
	int		rs = SR_BUGCHECK ;
	if (setp) {
	    rs = setp->size() ;
	} /* end if (non-null) */
	return rs ;
}
/* end method (fonce::icount) */

int fonce_co::operator () (int a) noex {
	int		rs = SR_BUGCHECK ;
	if (op) {
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
}
/* end method (fonce_co::operator) */


