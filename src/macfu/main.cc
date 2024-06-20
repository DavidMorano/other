/* main SUPPORT (fu) */
/* lang=C++20 */

/* filter filenames */
/* version %I% last-modified %G% */


/* revision history:

	= 1989-03-01, David A.D. Morano
	This subroutine was originally written.

*/

/* Copyright © 1989 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Names:
	fu

	Description:
	This program filters filenames keeping only unique files.

	Synopsis:
	$ fu [<file(s)>] [-]

	Arguments:
	<file(s)>	file(s) to process
	-		read standard-input for file(s) to process

	Returns:
	==0		for normal operation success
	!=0		some error

*******************************************************************************/

#include	<envstandards.h>	/* ordered first to configure */
#include	<sys/param.h>		/* |MAXPATHLEN| */
#include	<unistd.h>
#include	<fcntl.h>
#include	<climits>
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<cstdio>
#include	<cstring>		/* <- for |strlen(3c)| */
#include	<string>
#include	<string_view>
#include	<vector>
#include	<iostream>
#include	<usystem.h>
#include	<varnames.hh>
#include	<strn.h>
#include	<sfx.h>
#include	<matstr.h>
#include	<strwcpy.h>
#include	<strnul.hh>
#include	<sncpyx.h>
#include	<getourenv.h>
#include	<mapblock.hh>
#include	<readln.hh>
#include	<rmx.h>
#include	<isnot.h>
#include	<mapex.h>
#include	<exitcodes.h>
#include	<localmisc.h>

#include	"fonce.hh"


/* local defines */

#ifndef	MAXPATHLEN
#define	MAXPATHLEN	4096
#endif


/* imported namespaces */

using std::nullptr_t ;			/* type */
using std::string ;			/* type */
using std::string_view ;		/* type */
using std::unordered_set ;		/* type */
using std::unordered_map ;		/* type */
using std::vector ;			/* type */
using std::istream ;			/* type */
using std::cin;				/* variable */
using std::cout ;			/* variable */
using std::cerr ;			/* variable */
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
	int		argc ;
	int		pm = 0 ;
	bool		ffound = false ;
	proginfo(int c,mainv a,mainv e) noex : argc(c), argv(a), envv(e) { 
	    start(this,proginfomem_start) ;
	    finish(this,proginfomem_finish) ;
	    flistbegin(this,proginfomem_flistbegin) ;
	    flistend(this,proginfomem_flistend) ;
	} ;
	proginfo() noex : proginfo(0,nullptr,nullptr) { } ;
	void operator () (int c,mainv a,mainv e) noex {
	    argc = c ;
	    argv = a ;
	    envv = e ;
	} ;
	int fileproc(cchar *,int = -1) noex ;
	int filecheck(const USTAT *) noex ;
	int output() noex ;
    private:
	int istart() noex ;
	int ifinish() noex ;
	int getpn(mainv) noex ;
	int iflistbegin() noex ;
	int iflistend() noex ;
	int readin() noex ;
    } ; /* end struct (proginfo) */
}


/* forward references */


/* local variables */

enum progmodes {
	progmode_fileuniq,
	progmode_fu,
	progmode_overlast
} ;

static constexpr cpcchar	prognames[] = {
	"fileuniq",
	"fu",
	nullptr
} ;

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
                case progmode_fileuniq:
                case progmode_fu:
                    rs = pi.output() ;
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

int proginfo::iflistbegin() noex {
	return seen.start(1000) ;
}
/* end method (proginfo::iflistbegin) */

int proginfo::iflistend() noex {
	return seen.finish ;
}
/* end method (proginfo::iflistend) */

int proginfo::readin() noex {
	istream		*isp = &cin ;
	int		rs ;
	int		c = 0 ;
	if ((rs = maxpathlen) >= 0) {
	    cint	llen = rs ;
	    char	*lbuf ;
	    rs = SR_NOMEM ;
	    if ((lbuf = new(nothrow) char[llen+1]) != nullptr) {
	        while ((rs = readln(isp,lbuf,llen)) > 0) {
		    int		ll = rs ;
		    if ((ll > 0) && (lbuf[ll - 1] == eol)) ll -= 1 ;
		    if (ll > 0) {
		        rs = fileproc(lbuf,ll) ;
			c += rs ;
		    }
		    if (rs < 0) break ;
	        } /* end if (reading lines) */
	        delete [] lbuf ;
	    } /* end if (m-a-f) */
	} /* end if (maxpathlen) */
	return (rs >= 0) ? c : rs ;
}
/* end method (proginfo::readin) */

int proginfo::fileproc(cchar *sp,int sl) noex {
	USTAT		sb ;
	strnul		s(sp,sl) ;
	int		rs ;
	int		c = 0 ;
	if ((rs = u_stat(s,&sb)) >= 0) {
	    if (S_ISREG(sb.st_mode)) {
	        if ((rs = filecheck(&sb)) > 0) {
		    c = 1 ;
		    cout << s << eol ;
	        } /* end if (filecheck) */
	    } /* end if (is-dir) */
	} else if (isNotAccess(rs)) {
	    rs = SR_OK ;
	}
	return (rs >= 0) ? c : rs ;
}
/* end method (proginfo::fileproc) */

int proginfo::filecheck(const USTAT *sbp)  noex {
	int		rs = seen.checkin(sbp) ;
	return rs ;
}
/* end method (proginfo::filecheck) */

int proginfo::output() noex {
	int		rs = SR_OK ;
	int		c = 0 ;
	if (argc > 1) {
	    for (int ai = 1 ; (ai < argc) && argv[ai] ; ai += 1) {
	        cchar	*fn = argv[ai] ;
	        if (fn[0]) {
		    if ((fn[0] == '-') && (fn[1] == '\0')) {
		        rs = readin() ;
		        c += rs ;
		    } else {
	                rs = fileproc(fn) ;
		        c += rs ;
		    }
	        }
	        if (rs < 0) break ;
	    } /* end for */
	} else {
	    rs = readin() ;
	    c += rs ;
	}
	return (rs >= 0) ? c : rs ;
}
/* end subroutine (proginfo::output) */

int proginfo_co::operator () (int) noex {
	int	rs = SR_BUGCHECK ;
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


