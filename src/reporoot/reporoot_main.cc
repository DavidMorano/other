/* reporoot_main SUPPORT (files) */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* filter filenames */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-03-01, David A-D- Morano
	This subroutine was originally written.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Names:
	reporoot
	reponame

	Description:
	This program finds and prints out the "repository root" 
	given the current working directory.

	Synopsis:
	$ reporoot 
	$ reponame
	
	Arguments:
	-

	Returns:
	==0		for normal operation success
	!=0		some error

	Notes:
	1. The use of |u_getvwd(3u)| always returns a "resolved" 
	file-path.  So there is no need to call something like
	|u_resolvepath(3u)| to resolve the returned path.

*******************************************************************************/

#include	<envstandards.h>	/* ordered first to configure */
#include	<sys/param.h>		/* |MAXPATHLEN| */
#include	<unistd.h>
#include	<fcntl.h>
#include	<climits>
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<cstdio>
#include	<cstring>
#include	<new>			/* |nothrow(3c++)| */
#include	<string_view>
#include	<vector>
#include	<iostream>
#include	<usystem.h>
#include	<strnul.hh>
#include	<sfx.h>
#include	<rmx.h>
#include	<matstr.h>
#include	<isnot.h>
#include	<mapex.h>
#include	<exitcodes.h>
#include	<localmisc.h>

import ulibvals ;
import umisc ;
import fonce ;

/* local defines */

#define	NENTS		1000


/* imported namespaces */

using std::nullptr_t ;			/* type */
using std::string_view ;		/* type */
using std::istream ;			/* type */
using libu::umemalloc ;			/* subroutine (LIBU) */
using libu::umemfree ;			/* subroutine (LIBU) */
using libu::snwcpy ;			/* subroutine (LIBU) */
using std::cout ;			/* variable */
using std::nothrow ;			/* constant */


/* local typedefs */

typedef string_view	strview ;


/* external subroutines */


/* external variables */


/* local structures */

namespace {
    enum proginfomems {
	proginfomem_start,
	proginfomem_finish,
	proginfomem_flistbegin,
	proginfomem_flistend,
	proginfomem_overlast
    } ;
    struct proginfo ;
    struct proginfo_co {
	proginfo	*op = nullptr ;
	int		w = -1 ;
	void operator () (proginfo *p,int m) noex {
	    op = p ;
	    w = m ;
	} ;
	int operator () (int = 0) noex ;
	operator int () noex {
	    return operator () (0) ;
	} ;
    } ; /* end struct (proginfo_co) */
    typedef int (proginfo::*proginfo_m)(CUSTAT *,cchar *,int) noex ;
    struct proginfo {
	friend		proginfo_co ;
	proginfo_co	start ;
	proginfo_co	finish ;
	proginfo_co	flistbegin ;
	proginfo_co	flistend ;
	fonce		seen ;
	mainv		argv ;
	mainv		envv ;
	cchar		*pn = nullptr ;
	char		*lbuf = nullptr ;
	int		argc ;
	int		pm = 0 ;
	int		llen = 0 ;
	proginfo(int c,mainv a,mainv e) noex : argc(c), argv(a), envv(e) { 
	    start	(this,proginfomem_start) ;
	    finish	(this,proginfomem_finish) ;
	    flistbegin	(this,proginfomem_flistbegin) ;
	    flistend	(this,proginfomem_flistend) ;
	} ;
	proginfo() noex : proginfo(0,nullptr,nullptr) { } ;
	void operator () (int c,mainv a,mainv e) noex {
	    argc = c ;
	    argv = a ;
	    envv = e ;
	} ;
	int process() noex ;
	int process_loop(char *,int,int) noex ;
	int process_check(char *,int,int) noex ;
	int process_print(cchar *,int) noex ;
	int process_pmbegin() noex ;
	int process_pmend() noex ;
    private:
	int istart() noex ;
	int ifinish() noex ;
	int getpn(mainv) noex ;
	int iflistbegin() noex ;
	int iflistend() noex ;
    } ; /* end struct (proginfo) */
} /* end namespace */


/* forward references */


/* local variables */

enum progmodes {
	progmode_reporoot,
	progmode_reponame,
	progmode_overlast
} ;

constexpr cpcchar	prognames[] = {
	[progmode_reporoot]	= "reporoot",
	[progmode_reponame]	= "reponame",
	[progmode_overlast]	= nullptr
} ;

constexpr cpcchar	repomarks[] = {
    	".git",
	".repo",
	".reporoot"
} ;

constexpr MAPEX		mapexs[] = {
	{ SR_NOENT,	EX_NOUSER },
	{ SR_AGAIN,	EX_TEMPFAIL },
	{ SR_DEADLK,	EX_TEMPFAIL },
	{ SR_NOLCK,	EX_TEMPFAIL },
	{ SR_TXTBSY,	EX_TEMPFAIL },
	{ SR_ACCESS,	EX_NOPERM },
	{ SR_REMOTE,	EX_PROTOCOL },
	{ SR_NOSPC,	EX_TEMPFAIL },
	{ SR_INTR,	EX_INTR },
	{ SR_EXIT,	EX_TERM },
	{ SR_NOMSG,	EX_OSERR },
	{ SR_NOSYS,	EX_OSFILE },
	{ 0, 0 }
} ; /* end array (mapex) */

static cint	maxpathlen = ulibval.maxpathlen ;
static cint	maxlinelen = ulibval.maxline ;

constexpr int	nents = NENTS ;


/* exported variables */


/* exported subroutines */

int main(int argc,mainv argv,mainv envv) noex {
	proginfo	pi(argc,argv,envv) ;
	int		ex = EX_OK ;
	int		rs ;
	int		rs1 ;
	if ((rs = pi.start) >= 0) {
	    if ((rs = pi.flistbegin) >= 0) {
                switch (pi.pm) {
                case progmode_reporoot:
                case progmode_reponame:
                    rs = pi.process() ;
                    break ;
		default:
		    rs = SR_BUGCHECK ;
		    break ;
                } /* end switch */
		rs1 = pi.flistend ;
		if (rs >= 0) rs = rs1 ;
	    } /* end if (flist-) */
	    rs1 = pi.finish ;
	    if (rs >= 0) rs = rs1 ;
	} /* end if (proginfo) */
	if ((rs < 0) && (ex == EX_OK)) {
	    ex = mapex(mapexs,rs) ;
	}
	return ex ;
}
/* end subroutine (main) */


/* local subroutines */

int proginfo::istart() noex {
	int		rs ;
	if ((rs = getpn(prognames)) >= 0) {
	    rs = 0 ;
	} /* end if (proginfo::getpn) */
	return rs ;
}
/* end method (proginfo::istart) */

int proginfo::ifinish() noex {
	return SR_OK ;
}
/* end method (proginfo::ifinish) */

int proginfo::getpn(mainv names) noex {
	int		rs = SR_FAULT ;
	if (argv) {
	    rs = SR_NOMSG ;
	    if ((argc > 0) && argv[0]) {
		cchar	*arg0 = argv[0] ;
	        cchar	*bp{} ;
	        if (int bl ; (bl = sfbasename(arg0,-1,&bp)) > 0) {
		    if (int pl = rmchr(bp,bl,'.') ; pl > 0) {
	                if ((pm = matstr(names,bp,pl)) >= 0) {
			    pn = names[pm] ;
		            rs = pm ;
	                }
		    } /* end if (non-zero positive progname) */
		} /* end if (have base-name) */
	    } /* end if (have first argument) */
	} /* end if (non-null) */
	return rs ;
}
/* end method (proginfo::getpn) */

int proginfo::iflistbegin() noex {
	return seen.start(nents) ;
}
/* end method (proginfo::iflistbegin) */

int proginfo::iflistend() noex {
	return seen.finish ;
}
/* end method (proginfo::iflistend) */

int proginfo::process() noex {
	int		rs ;
	int		rs1 ;
	int		c = 0 ;
	if ((rs = process_pmbegin()) >= 0) {
	    if ((rs = maxpathlen) >= 0) {
	        cint plen = rs ;
	        if (char *pbuf ; (rs = umemalloc((plen + 1),&pbuf)) >= 0) {
	            if ((rs = u_getcwd(pbuf,plen)) >= 0) {
			rs = process_loop(pbuf,plen,rs) ;
			c = rs ;
		    } /* end if (u_getcwd) */
	            rs1 = umemfree(pbuf) ;
		    if (rs >= 0) rs = rs1 ;
	        } /* end if (m-a-f) */
	    } /* end if (maxpathlen) */
	    rs1 = process_pmend() ;
	    if (rs >= 0) rs = rs1 ;
	} /* end if (process_pm) */
	return (rs >= 0) ? c : rs ;
}
/* end subroutine (proginfo::process) */

int proginfo::process_loop(char *pbuf,int plen,int pl) noex {
    	int		rs = SR_OK ;
	int		f = false ;
	while ((rs >= 0) && (! f) && (pl > 1)) {
	    if ((rs = process_check(pbuf,plen,pl)) > 0) {
		f = true ;
		rs = process_print(pbuf,pl) ;
	    } else if (rs == 0) {
		cchar *sp ;
		if (int sl ; (sl = sfdirname(pbuf,pl,&sp)) > 0) {
		    pbuf[sl] = '\0' ;
		    pl = sl ;
		}
	    } /* end if */
	} /* end while */
	return (rs >= 0) ? f : rs ;
}
/* end subroutine (proginfo::process_loop) */

int proginfo::process_check(char *pbuf,int plen,int pl) noex {
    	int		rs = SR_OK ;
	int		f = false ;
	for (cauto &n : repomarks) {
	    if ((rs = pathnadd(pbuf,plen,pl,n)) >= 0) {
	        if (USTAT sb ; (rs = u_stat(pbuf,&sb)) >= 0) {
		    f = S_ISDIR(sb.st_mode) || S_ISREG(sb.st_mode) ;
	        } else if (isNotAccess(rs)) {
		    rs = SR_OK ;
	        }
	    } /* end if (pathadd) */
	    if ((rs < 0) || f) break ;
	} /* end for */
	pbuf[pl] = '\0' ; /* restore original */
	return (rs >= 0) ? f : rs ;
}
/* end subroutine (proginfo::process_check) */

int proginfo::process_print(cchar *sp,int sl) noex {
    	int		rs = SR_OK ;
	switch (pm) {
	case progmode_reporoot:
	    {
		strview n(sp,sl) ;
		cout << n << eol ;
	    }
	    break ;
	case progmode_reponame:
	    {
		cchar *cp ;
	        if (int cl ; (cl = sfbasename(sp,sl,&cp)) > 0) {
		    strview n(cp,cl) ;
		    cout << n << eol ;
		}
	    }
	    break ;
	} /* end switch */
	return rs ;
}
/* end subroutine (proginfo::process_print) */

int proginfo::process_pmbegin() noex {
	int		rs = SR_OK ;
	switch (pm) {
        case progmode_reporoot:
        case progmode_reponame:
	    if ((rs = maxlinelen) >= 0) {
	        llen = rs ;
		rs = SR_NOMEM ;
	        if ((lbuf = new(nothrow) char[llen + 1]) != nullptr) {
		    rs = SR_OK ;
	        } else {
		    llen = 0 ;
		}
	    } /* end block */
	    break ;
	} /* end switch */
	return rs ;
}
/* end subroutine (proginfo::process_pmbegin) */

int proginfo::process_pmend() noex {
	int		rs = SR_OK ;
	switch (pm) {
        case progmode_reporoot:
        case progmode_reponame:
	    if (lbuf) {
		delete [] lbuf ;
		lbuf = nullptr ;
	        llen = 0 ;
	    } /* end block */
	    break ;
	} /* end switch */
	return rs ;
}
/* end subroutine (proginfo::process_pmend) */

int proginfo_co::operator () (int) noex {
	int		rs = SR_BUGCHECK ;
	if (op) {
	    switch (w) {
	    case proginfomem_start:
	        rs = op->istart() ;
	        break ;
	    case proginfomem_finish:
	        rs = op->ifinish() ;
	        break ;
	    case proginfomem_flistbegin:
	        rs = op->iflistbegin() ;
	        break ;
	    case proginfomem_flistend:
	        rs = op->iflistend() ;
	        break ;
	    } /* end switch */
	} /* end if (non-null) */
	return rs ;
}
/* end method (proginfo_co::operator) */


