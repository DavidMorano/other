/* files_utils2 MODULE (module-implementation-unit) */
/* charset=ISO8859-1 */
/* lang=C++20 (conformance reviewed) */

/* Files-Utilities (for FILES program) */
/* version %I% last-modified %G% */


/* revision history:

	= 2017-03-21, David A-D- Morano
	This subroutine was originally written.

*/

/* Copyright © 2017 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Object:
	files_util 

*******************************************************************************/

module ;

#include	<envstandards.h>	/* ordered first to configure */
#include	<sys/stat.h>
#include	<sys/mman.h>
#include	<unistd.h>
#include	<fcntl.h>		/* |O_{xx}| */
#include	<climits>		/* |INT_MAX| */
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<cstring>		/* |memchr(3c)| */
#include	<usystem.h>
#include	<mkchar.h>
#include	<localmisc.h>

#pragma		GCC dependency	"mod/libutil.ccm"
#pragma		GCC dependency	"mod/debug.ccm"

module files_utils ;

import libutil ;			/* |lenstr(3u)| */
import debug ;				/* |debprintf(3u)| */


/* local defines */


/* imported namespaces */


/* local typedefs */


/* external subroutines */


/* external variables */


/* local structures */


/* forward references */


/* local variables */


/* exported variables */


/* exported subroutines */


/* exported variables */


/* exported subroutines */

static int	liner(int,size_t) noex ;


/* local variables */


/* exported variables */


/* exported subroutines */

int filelines(cchar *fn) noex {
	int		rs = SR_FAULT ;
	int		rs1 ;
	int		lines = 0 ;
	if (fn) ylikely {
	    rs = SR_INVALID ;
	    if (fn[0]) ylikely {
		cint	of = O_RDONLY ;
		if ((rs = u_open(fn,of,0)) >= 0) ylikely {
		    cint	fd = rs ;
		    if (ustat sb ; (rs = u_fstat(fd,&sb)) >= 0) ylikely {
			csize	fsize = size_t(sb.st_size) ;
			rs = SR_NOTSUP ;
		        if (S_ISREG(sb.st_mode)) ylikely {
			    rs = SR_OK ;
			    if (fsize > 0) {
			        rs = liner(fd,fsize) ;
			        lines = rs ;
			    }
			} /* end if (regular file) */
		    } /* end if (stat) */
		    rs1 = u_close(fd) ;
		    if (rs >= 0) rs = rs1 ;
		} /* end if (open) */
	    } /* end if (valid) */
	} /* end if (non-null) */
	return (rs >= 0) ? lines : rs ;
}
/* end subroutine (filelines) */


/* local subroutines */

static int liner(int fd,csize ms) noex {
	cint		mp = PROT_READ ;
	cint		mf = MAP_SHARED ;
	int		rs ;
	int		rs1 ;
	int		lines = 0 ; /* return-value */
	cnullptr	np{} ;
	if (void *md ; (rs = u_mmapbegin(np,ms,mp,mf,fd,0z,&md)) >= 0) ylikely {
	    cint	cmd = MADV_SEQUENTIAL ;
	    if ((rs = u_madvise(md,ms,cmd)) >= 0) ylikely {
		size_t	ll = ms ;
		cchar	*lp = charp(md) ;
		for (cchar *tp ; (tp = charp(memchr(lp,'\n',ll))) != np ; ) {
		    csize	si = ((tp + 1) - lp) ;
		    lines += 1 ;
		    ll -= si ;
		    lp += si ;
		} /* end for */
		if (ll > 0) lines += 1 ;
	    } /* end if (memory-advise) */
	    rs1 = u_mmapend(md,ms) ;
	    if (rs >= 0) rs = rs1 ;
	} /* end if (map-file) */
	return (rs >= 0) ? lines : rs ;
}
/* end subroutine (liner) */


