/* filelines SUPPORT */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* calculate the number of lines in a file */
/* get the size of an open file */


/* revision history:

	= 2023-12-17, David A­D­ Morano
	Originally written for Rightcore Network Services.

*/

/* Copyright © 2023 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

  	Name:
	filelines

	Description:
	This is the (now) famous 'filelines' subroutine.  It is the
	fastest version of this kind of funtion (counting lines in
	a text file).  Note that unlike the |fileliner(3dam)|
	subroutine, no comment character can be specified.

	Synopsis:
	int filelines(cchar *fn) noex

	Arguments:
	fn		file-name

	Returns:
	>=0		number of lines in file
	<0		error (system-return)

*******************************************************************************/

#include	<envstandards.h>	/* ordered first to configure */
#include	<sys/stat.h>
#include	<sys/mman.h>
#include	<unistd.h>
#include	<fcntl.h>
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<cstring>		/* <- for |memchr(3c)| */
#include	<usystem.h>
#include	<localmisc.h>

#include	"filelines.h"

import libutil ;

/* local defines */


/* imported namespaces */

using std::nullptr_t ;


/* local typedefs */


/* external subroutines */


/* external variables */


/* local structures */


/* forward references */

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
		if ((rs = uc_open(fn,of,0)) >= 0) ylikely {
		    cint	fd = rs ;
		    if (ustat sb ; (rs = uc_fstat(fd,&sb)) >= 0) ylikely {
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
		    rs1 = uc_close(fd) ;
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


