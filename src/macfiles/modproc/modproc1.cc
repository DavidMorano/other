/* modproc1 MODULE (module-implementation-unit) */
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

	Names:
	modprocload

	Description:
	I parse a C++ source code file and find all of the module
	names that I can that are being imported to that source file.

	Synopsis:
	modprocload(vecstr *op,cchar *fname) noex

	Arguments:
	op		ureserve::vecstr
	fname		file-name to parse

	Returns:
	>=0		number of imported modules found
	<0		error (system-return)

*******************************************************************************/

module ;

#include	<envstandards.h>	/* ordered first to configure */
#include	<sys/stat.h>
#include	<unistd.h>
#include	<fcntl.h>		/* |O_{xx}| */
#include	<climits>		/* |INT_MAX| */
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>
#include	<usyscalls.h>
#include	<ascii.h>
#include	<hasx.h>		/* |hasmodname(3uc)| */
#include	<strop.h>
#include	<strwcmp.h>
#include	<mkchar.h>
#include	<ischarx.h>		/* |isalnumlatin(3cu)| */
#include	<localmisc.h>

#pragma		GCC dependency	"mod/modproc.ccm"
#pragma		GCC dependency	"mod/ureserve.ccm"
#pragma		GCC dependency	"mod/debug.ccm"

module modproc ;

/* local defines */


/* imported namespaces */


/* local typedefs */


/* external subroutines */


/* external variables */


/* local structures */

namespace {
    struct vecmgr {
	vecstr		*vop ;
	vecmgr(vecstr *p) noex : vop(p) { } ;
	int operator () (cchar *) noex ;
	int filenmods(cchar *) noex ;
	int liner(int,size_t) noex ;
	int procln(cchar *,int) noex ;
    } ; /* end struct (vecmgr) */
} /* end namespace */


/* forward references */


/* local variables */

cchar			istr[] = "import" ;


/* exported variables */


/* exported subroutines */

int modprocload(vecstr *op,cchar *fname) noex {
    	int		rs = SR_FAULT ;
	if_constexpr (f_debug) debprintf(__func__,"ent fn=%s\n",fname) ;
	if (op && fname) ylikely {
    	    vecmgr	mgr(op) ;
	    rs = mgr(fname) ;
	} /* end if (non-null) */
	if_constexpr (f_debug) debprintf(__func__,"ret rs=%d\n",rs) ;
	return rs ;
}
/* end subroutine (modprocload) */


/* local subroutines */

int vecmgr::operator () (cchar *fname) noex {
    	return filenmods(fname) ;
} /* end method (vecmgr::operator) */

int vecmgr::filenmods(cchar *fname) noex {
	int		rs = SR_FAULT ;
	int		rs1 ;
	int		nmods = 0 ;
	if (fname) ylikely {
	    rs = SR_INVALID ;
	    if (fname[0]) ylikely {
		cint	of = O_RDONLY ;
		if ((rs = u_open(fname,of,0)) >= 0) ylikely {
		    cint	fd = rs ;
		    if (ustat sb ; (rs = u_fstat(fd,&sb)) >= 0) ylikely {
			csize	fsize = size_t(sb.st_size) ;
			rs = SR_NOTSUP ;
		        if (S_ISREG(sb.st_mode)) ylikely {
			    rs = SR_OK ;
			    if (fsize > 0) {
			        rs = liner(fd,fsize) ;
			        nmods = rs ;
			    }
			} /* end if (regular file) */
		    } /* end if (stat) */
		    rs1 = u_close(fd) ;
		    if (rs >= 0) rs = rs1 ;
		} /* end if (open) */
	    } /* end if (valid) */
	} /* end if (non-null) */
	return (rs >= 0) ? nmods : rs ;
} /* end method (vecmgr::filenmods) */

int vecmgr::liner(int fd,csize ms) noex {
	cnullptr	np{} ;
	cint		mp = PROT_READ ;
	cint		mf = MAP_SHARED ;
	int		rs ;
	int		rs1 ;
	int		nmods = 0 ; /* return-value */
	if (void *md ; (rs = u_mmapbegin(np,ms,mp,mf,fd,0z,&md)) >= 0) {
	    cint	cmd = MADV_SEQUENTIAL ;
	    if ((rs = u_madvise(md,ms,cmd)) >= 0) ylikely {
		size_t	ll = ms ;
		cchar	*lp = charp(md) ;
		for (cchar *tp ; (tp = charp(memchr(lp,'\n',ll))) != np ; ) {
		    csize	si = ((tp + 1) - lp) ;
		    if (cint sl = intconv(si - 1) ; sl > 0) {
		        rs = procln(lp,sl) ;
		        nmods += rs ;
		    }
		    ll -= si ;
		    lp += si ;
		} /* end for */
		if ((rs >= 0) && (ll > 0)) {
		    cint	sl = intconv(ll) ;
		    rs = procln(lp,sl) ;
		    nmods += rs ;
		} /* end if (trailing line */
	    } /* end if (memory-advise) */
	    rs1 = u_mmapend(md,ms) ;
	    if (rs >= 0) rs = rs1 ;
	} /* end if (map-file) */
	if_constexpr (f_debug) {
	    debprintf(__func__,"ret rs=%d c=%d\n",rs,nmods) ;
	}
	return (rs >= 0) ? nmods : rs ;
} /* end method (vecmgr::liner) */

int vecmgr::procln(cchar *lp,int ll) noex {
    	int		rs ;
	int		rs1 ;
	int		c = 0 ; /* return-value -- number of names found */
	if_constexpr (f_debug) {
	    strnul st(lp,ll) ;
	    debprintf(__func__,"ent ln=>%s<\n",ccp(st)) ;
	}
    	if (strop s ; (rs = s.start(lp,ll)) >= 0) ylikely {
	    cchar	*ip ;
	    if (int il ; (il = s.fieldwht(&ip)) > 0) {
		if_constexpr (f_debug) {
	    	    strnul st(ip,il) ;
	    	    debprintf(__func__,"piece=>%s<\n",ccp(st)) ;
		}
		if (strwcmp(istr,ip,il) == 0) {
		    cint	ch_s = CH_SEMI ;
		    cchar	*mp ;
		    if_constexpr (f_debug) debprintf(__func__,"cmp\n") ;
		    if (int ml ; (ml = s.fieldchr(ch_s,&mp)) > 0) {
			if_constexpr (f_debug) {
	    	    	    strnul st(mp,ml) ;
	    	    	    debprintf(__func__,"m=>%s<\n",ccp(st)) ;
			}
			if (hasmodname(mp,ml)) {
			    rs = vop->adduniq(mp,ml) ;
			    c = (rs < INT_MAX) ;
			}
		    } /* end if (strop_fieldchr) */
	        } /* end if (got "import") */
	    } /* end if (got field) */
	    rs1 = s.finish ;
	    if (rs >= 0) rs = rs1 ;
	} /* end if (strop) */
	if_constexpr (f_debug) debprintf(__func__,"ret rs=%d c=%d\n",rs,c) ;
	return (rs >= 0) ? c : rs ;
} /* end method (vecmgr::procln) */


