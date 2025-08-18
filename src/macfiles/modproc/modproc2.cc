/* modproc2 MODULE (module-implementation-unit) */
/* charset=ISO8859-1 */
/* lang=C++20 (conformance reviewed) */

/* load module names from a C++ source code file */
/* version %I% last-modified %G% */


/* revision history:

	= 2017-03-21, David A-D- Morano
	This subroutine was originally written.

*/

/* Copyright © 2017 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Object:
	modproc

*******************************************************************************/

module ;

#include	<envstandards.h>	/* ordered first to configure */
#include	<sys/stat.h>
#include	<unistd.h>
#include	<fcntl.h>		/* |O_{xx}| */
#include	<climits>		/* |INT_MAX| */
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<cstdio>
#include	<new>			/* placement-new + |nothrow(3c++)| */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>
#include	<usyscalls.h>
#include	<ulogerror.h>
#include	<ascii.h>
#include	<hasx.h>		/* |hasmodname(3uc)| */
#include	<strop.h>
#include	<strwcmp.h>
#include	<mkchar.h>
#include	<ischarx.h>		/* |isalnumlatin(3cu)| */
#include	<localmisc.h>

#pragma		GCC dependency	"mod/modproc.ccm"
#pragma		GCC dependency	"mod/ureserve.ccm"

module modproc ;

import ureserve ;			/* |vecstr(3u)| */

/* local defines */


/* imported namespaces */

using std::nothrow ;			/* constant */


/* local typedefs */

typedef vecstr *	vecstrp ;


/* external subroutines */


/* external variables */


/* local structures */


/* forward references */


/* local variables */


/* exported variables */


/* exported subroutines */

int modproc::istart() noex {
    	cnullptr	np{} ;
    	int		rs = SR_NOMEM ;
    	if (vecstr *vsp ; (vsp = new(nothrow) vecstr) != np) {
	    vop = vsp ;
	    rs = SR_OK ;
	} /* end if (new-vecstr) */
	return rs ;
} /* end method (modproc::istart) */

int modproc::ifinish() noex {
    	int		rs = SR_NOTOPEN ;
	if (vop)  {
	    vecstr	*vsp = vecstrp(vop) ;
	    rs = SR_OK ;
	    {
		delete vsp ;
	    }
	    vop = nullptr ;
	} /* end if (non-null) */
	return rs ;
} /* end method (modproc::ifinish) */

int modproc::procfile(cchar *fn) noex {
    	int		rs = SR_FAULT ;
	int		c = 0 ;
	if (fn) ylikely {
	    if (fn[0]) ylikely {
		vecstr *vsp = vecstrp(vop) ;
		rs = modprocload(vsp,fn) ;
		c = rs ;
	    } /* end if (valid) */
	} /* end if (non-null) */
	return (rs >= 0) ? c : rs ;
} /* end method (modproc::procfile) */

int modproc::procout(FILE *osp,uint ot) noex {
        cint		rsn = SR_NOTFOUND ;
    	int		rs = SR_FAULT ;
	int		rs1 ;
	int		c = 0 ;
	if (osp) {
	    vecstr *vsp = vecstrp(vop) ;
	    vecstr_cur cur ;
	    cchar	*cp ;
	    rs = SR_OK ;
	    (void) ot ;
	    for (int i = 0 ; (rs1 = vsp->get(i,&cp)) >= 0 ; i += 1) {
		if (cp) {
	            rs = fprintf(osp,"%s\n",cp) ;
		    c += 1 ;
		}
		if (rs < 0) break ;
	    } /* end for */
	    if ((rs >= 0) && (rs1 != rsn)) rs = rs1 ;
	} /* end if (non-null) */
	return (rs >= 0) ? c : rs ;
} /* end method (modproc::procout) */


/* local subroutines */

void modproc::dtor() noex {
	if (cint rs = finish ; rs < 0) {
	    ulogerror("modproc",rs,"fini-finish") ;
	}
} /* end method (modproc::dtor) */

modproc_co::operator int () noex {
	int		rs = SR_BUGCHECK ;
	if (op) ylikely {
	    switch (w) {
	    case modprocmem_start:
	        rs = op->istart() ;
	        break ;
	    case modprocmem_finish:
	        rs = op->ifinish() ;
	        break ;
	    } /* end switch */
	} /* end if (non-null) */
	return rs ;
} /* end method (modproc_co::operator) */


