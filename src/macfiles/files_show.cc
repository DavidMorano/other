/* files_show SUPPORT */
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
	files_show

	Description:
	This is the (now) famous 'filelines' subroutine.  It is the
	fastest version of this kind of funtion (counting lines in
	a text file).  Note that unlike the |fileliner(3dam)|
	subroutine, no comment character can be specified.

	Synopsis:
	int files_show(cchar *fn,int mlen) noex

	Arguments:
	fn		file-name
	mlen		maximum line-length

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
#include	<cstring>		/* |memchr(3c)| */
#include	<algorithm>		/* |min(3c++)| + |max(3c++)| */
#include	<string_view>
#include	<iostream>
#include	<clanguage.h>
#include	<usysbase.h>
#include	<usyscalls.h>
#include	<localmisc.h>		/* |eol| */

#include	"files_show.h"


/* local defines */

#define		MLEN	76


/* imported namespaces */

using std::string_view ;		/* type */
using std::min ;			/* subroutine-template */
using std::max ;			/* subroutine-template */
using std::cout ;			/* variable */


/* local typedefs */

typedef std::string_view	strview ;


/* external subroutines */


/* external variables */


/* local structures */


/* forward references */

local int	liner(cchar *,int,size_t,int) noex ;
local int	check(cchar *,size_t,cchar *,size_t,int,int) noex ;


/* local variables */

csize		msize = MLEN ;


/* exported variables */


/* exported subroutines */

int files_show(cchar *fn,int mlen) noex {
	int		rs = SR_FAULT ;
	int		rs1 ;
	int		lines = 0 ; /* return-value */
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
			        rs = liner(fn,fd,fsize,mlen) ;
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

local int liner(cchar *fn,int fd,csize ms,int mlen) noex {
	cnullptr	np{} ;
    	csize		mll = size_t(mlen + 1) ;
	cint		mp = PROT_READ ;
	cint		mf = MAP_SHARED ;
	int		rs ;
	int		rs1 ;
	int		c = 0 ; /* return-value */
	if (void *md ; (rs = u_mmapbegin(np,ms,mp,mf,fd,0z,&md)) >= 0) ylikely {
	    cint	cmd = MADV_SEQUENTIAL ;
	    if ((rs = u_madvise(md,ms,cmd)) >= 0) ylikely {
		size_t	ll = ms ;
		int	lno = 0 ;
		cchar	*lp = charp(md) ;
		for (cchar *tp ; (tp = charp(memchr(lp,'\n',ll))) != np ; ) {
		    csize	si = size_t((tp + 1) - lp) ;
		    lno += 1 ;
		    c = check(fn,mll,lp,si,lno,c) ;
		    ll -= si ;
		    lp += si ;
		} /* end for */
		if (ll > 0) {
		    lno += 1 ;
		    c = check(fn,mll,lp,ll,lno,c) ;
		}
	    } /* end if (memory-advise) */
	    rs1 = u_mmapend(md,ms) ;
	    if (rs >= 0) rs = rs1 ;
	} /* end if (map-file) */
	return (rs >= 0) ? c : rs ;
} /* end subroutine (liner) */

local int check(cchar *fn,size_t mll,cchar *lp,size_t ll,int lno,int c) noex {
	if (ll > mll) {
	    strview sv(lp,min((ll-1),msize)) ;
	    c += 1 ;
	    cout << fn << " " << lno << eol ;
	    cout << "»" << sv << "«" << eol ;
	} /* end if (over-sized) */
	return c ;
} /* end subroutine (check) */


