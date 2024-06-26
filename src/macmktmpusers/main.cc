/* main (macmktmpusers) */
/* lang=C++20 */

/* generic front-end for SHELL built-ins */
/* version %I% last-modified %G% */

#define	CF_STRTCPY	0

/* revision history:

	= 1989-03-01, David A.D. Morano
	This subroutine was originally written.

	= 2007-08-22, David A.D. Morano
	I refactored and enhanced this for use on MacOS. 

*/

/* Copyright © 1989,2007 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Names:
	mktmpusers

	Description:
	Make the directory:
		/tmp/users/

	Synopsis:
	$ mktmpusers

	Returns:
	EXIT_SUCCESS	user was found and processed successfully
	EXIT_FAILURE	some kind of error (there could be many)

*******************************************************************************/

#include	<envstandards.h>
#include	<sys/types.h>
#include	<sys/param.h>
#include	<unistd.h>		/* |getusershell(3c)| */
#include	<climits>
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<cstdio>
#include	<cstring>		/* for |strlen(3c)| + |strnlen(3c)| */
#include	<string>
#include	<fstream>
#include	<iostream>
#include	<algorithm>
#include	<unordered_set>
#include	<utility>
#include	<usystem.h>
#include	<sfx.h>
#include	<matstr.h>
#include	<rmx.h>
#include	<strwcpy.h>
#include	<utmpx.h>
#include	<pwd.h>
#include	<grp.h>
#include	<ccfile.hh>
#include	<snx.h>
#include	<hasx.h>
#include	<isoneof.h>
#include	<isnot.h>
#include	<mapex.h>
#include	<exitcodes.h>
#include	<localmisc.h>


/* local defines */


/* imported namespaces */

using std::string ;			/* type */
using std::clog ;			/* variable */
using std::cerr ;			/* variable */
using std::cout ;			/* variable */
using std::nothrow ;			/* constant */


/* local typedefs */

using setiter = std::unordered_set<string>::iterator ;


/* external subroutines */


/* external variables */


/* local structures */

namespace {
    enum proginfomems {
	proginfomem_start,
	proginfomem_finish,
	proginfomem_pmbegin,
	proginfomem_pmend,
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
    typedef int (proginfo::*proginfo_m)() noex ;
    struct proginfo {
	friend		proginfo_co ;
	proginfo_co	start ;
	proginfo_co	finish ;
	proginfo_co	pmbegin ;
	proginfo_co	pmend ;
	USTAT		sb ;
	proginfo_m	m ;
	mainv		argv ;
	mainv		envv ;
	cchar		*pn = nullptr ;
	char		*pbuf = nullptr ;
	int		argc ;
	int		pm = 0 ;
	int		plen = 0 ;
	int		pl = 0 ;
	proginfo(int c,mainv a,mainv e) noex : argc(c), argv(a), envv(e) { 
	    start(this,proginfomem_start) ;
	    finish(this,proginfomem_finish) ;
	    pmbegin(this,proginfomem_pmbegin) ;
	    pmend(this,proginfomem_pmend) ;
	} ;
	proginfo() noex : proginfo(0,nullptr,nullptr) { } ;
	void operator () (int c,mainv a,mainv e) noex {
	    argc = c ;
	    argv = a ;
	    envv = e ;
	} ;
	int mktmp() noex ;
	int mktmp_present() noex ;
	int mktmp_wait() noex ;
	int mktmp_make() noex ;
	int mktmp_mode() noex ;
    private:
	int istart() noex ;
	int ifinish() noex ;
	int ipmbegin() noex ;
	int ipmend() noex ;
	int getpn(mainv) noex ;
    } ; /* end struct (proginfo) */
}


/* forward references */

#if	CF_STRTCPY
static char *strtcpy(char *,cchar *,int) noex ;
#endif


/* local variables */

enum progmodes {
	progmode_mktmpusers,
	progmode_overlast
} ;

constexpr cpcchar	prognames[] = {
	"mktmpusers",
	nullptr
} ;

constexpr proginfo_m	mems[] = {
	&proginfo::mktmp_wait,
	&proginfo::mktmp_make,
	&proginfo::mktmp_mode
} ;

constexpr mode_t		dm = (0777|S_ISVTX) ;


/* forward references */


/* local variables */

static constexpr MAPEX	mapexs[] = {
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
} ;

constexpr int		maxpathlen = MAXPATHLEN ;
constexpr int		maxlinelen = MAXLINELEN ;


/* exported variables */


/* exported subroutines */

int main(int argc,mainv argv,mainv envv) noex {
	proginfo	pi(argc,argv,envv) ;
	int		ex = EX_OK ;
	int		rs ;
	int		rs1 ;
	if ((rs = pi.start) >= 0) {
	    if ((rs = pi.pmbegin) >= 0) {
                switch (pi.pm) {
                case progmode_mktmpusers:
                    rs = pi.mktmp() ;
                    break ;
		default:
		    rs = SR_BUGCHECK ;
		    break ;
                } /* end switch */
		rs1 = pi.pmend ;
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
	    if ((rs = maxpathlen) >= 0) {
		cint	pathlen = rs ;
	        rs = SR_NOMEM ;
	        if ((pbuf = new(nothrow) char[pathlen + 1]) != nullptr) {
		    plen = pathlen ;
		    rs = SR_OK ;
	        } /* end if (new-char) */
	    } /* end if (maxpathlen) */
	} /* end if (proginfo::getpn) */
	return rs ;
}
/* end method (proginfo::istart) */

int proginfo::ifinish() noex {
	int		rs = SR_OK ;
	if (pbuf) {
	    delete [] pbuf ;
	    pbuf = nullptr ;
	}
	return rs ;
}
/* end method (proginfo::ifinish) */

int proginfo::ipmbegin() noex {
	int		rs = SR_OK ;
	return rs ;
}
/* end subroutine (proginfo::ipmbegin) */

int proginfo::ipmend() noex {
	int		rs = SR_OK ;
	return rs ;
}
/* end subroutine (proginfo::ipmend) */

int proginfo::getpn(mainv names) noex {
	int		rs = SR_FAULT ;
	if (argv) {
	    rs = SR_NOMSG ;
	    if ((argc > 0) && argv[0]) {
	        int	bl ;
		cchar	*arg0 = argv[0] ;
	        cchar	*bp{} ;
	        if ((bl = sfbasename(arg0,-1,&bp)) > 0) {
		    int		pl = rmchr(bp,bl,'.') ;
		    cchar	*pp = bp ;
		    if (pl > 0) {
	                if ((pm = matstr(names,pp,pl)) >= 0) {
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

int proginfo::mktmp() noex {
	int		rs = SR_OK ;
	for (cauto &m : mems) {
	    rs = (this->*m)() ;
	    if (rs < 0) break ;
	} /* end for */
	return rs ;
}

int proginfo::mktmp_present() noex {
	int		rs = SR_BUGCHECK ;
	int		fpresent = false ;
	if (pbuf) {
	    if (pbuf[0]) {
	        if ((rs = u_stat(pbuf,&sb)) >= 0) {
		    fpresent = true ;
	        } else if (isNotPresent(rs)) {
	            rs = SR_OK ;
	        } /* end if (u_stat) */
	    } /* end if (valid) */
	} /* end if (non-null) */
	return (rs >= 0) ? fpresent : rs ;
}

int proginfo::mktmp_wait() noex {
	int		rs ;
	if ((rs = snadd(pbuf,plen,pl,"/tmp")) >= 0) {
	    pl += rs ;
	    while ((rs = mktmp_present()) == 0) {
		sleep(1) ;
	    } /* end while */
	} /* end if (snadd) */
	return rs ;
}
/* end subroutine (proginfo::mktmp_wait) */

int proginfo::mktmp_make() noex {
	int		rs ;
	int		c = 0 ;
	if ((rs = snadd(pbuf,plen,pl,"/users")) >= 0) {
	    pl += rs ;
	    if ((rs = u_stat(pbuf,&sb)) >= 0) {
		c = 0 ;
	    } else if (isNotPresent(rs)) {
		if ((rs = u_mkdir(pbuf,dm)) >= 0) {
		    if ((rs = u_stat(pbuf,&sb)) >= 0) {
		        c = 1 ;
		    }
		}
	    }
	} /* end if (snadd) */
	return (rs >= 0) ? c : rs ;
}
/* end subroutine (proginfo::mktmp_make) */

int proginfo::mktmp_mode() noex {
	int		rs = SR_OK ;
	int		c = 0 ;
	mode_t		efm = (sb.st_mode & (~S_IFMT)) ;
	if ((efm & dm) != dm) {
	    rs = u_minmod(pbuf,(efm | dm)) ;
	    c = 1 ;
	} /* end if (fix mode) */
	return (rs >= 0) ? c : rs ;
}
/* end method (proginfo::mktmp_mode) */

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
	    case proginfomem_pmbegin:
	        rs = op->ipmbegin() ;
	        break ;
	    case proginfomem_pmend:
	        rs = op->ipmend() ;
	        break ;
	    } /* end switch */
	} /* end if (non-null) */
	return rs ;
}
/* end method (proginfo_co::operator) */

#if	CF_STRTCPY
/* this is similar to |strwcpy(3uc)| */
static char *strtcpy(char *dp,cchar *sp,int dl) noex {
	if (dl >= 0) {
	    dp = strncpy(dp,sp,dl) + dl ;
	    *dp = '\0' ;
	} else {
	    dp = nullptr ;
	}
	return dp ;
}
/* end subroutine (strtcpy) */
#endif /* CF_STRTCPY */


