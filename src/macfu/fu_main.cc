/* fu_main SUPPORT (fu) */
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
#include	<unistd.h>
#include	<fcntl.h>
#include	<climits>
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<cstdio>
#include	<string>
#include	<string_view>
#include	<vector>
#include	<iostream>
#include	<usystem.h>
#include	<varnames.hh>
#include	<getourenv.h>
#include	<strn.h>
#include	<sfx.h>
#include	<rmx.h>
#include	<sncpyx.h>
#include	<strwcpy.h>
#include	<strnul.hh>
#include	<ccfile.hh>
#include	<readln.hh>
#include	<matstr.h>
#include	<isnot.h>
#include	<mapex.h>
#include	<exitcodes.h>
#include	<localmisc.h>

import fonce ;
import ulibvals ;

/* local defines */

#define	NENTS		1000


/* imported namespaces */

using std::nullptr_t ;			/* type */
using std::string ;			/* type */
using std::string_view ;		/* type */
using std::istream ;			/* type */
using std::cin;				/* variable */
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
    typedef int (proginfo::*proginfo_m)(custat *,cchar *,int) noex ;
    struct proginfo {
	friend		proginfo_co ;
	proginfo_co	start ;
	proginfo_co	finish ;
	proginfo_co	flistbegin ;
	proginfo_co	flistend ;
	proginfo_m	m ;
	fonce		seen ;
	mainv		argv ;
	mainv		envv ;
	cchar		*pn = nullptr ;
	char		*lbuf = nullptr ;
	int		argc ;
	int		pm = 0 ;
	int		llen = 0 ;
	int		lines = 0 ;
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
	int filecheck(custat *) noex ;
	int process() noex ;
	int process_pmbegin() noex ;
	int process_pmend() noex ;
	int process_stdin() noex ;
	int readin() noex ;
	int fileproc(cchar *,int = -1) noex ;
	int fileproc_fu(custat *,cchar *,int = -1) noex ;
	int fileproc_lc(custat *,cchar *,int = -1) noex ;
	int fileproc_lines() noex ;
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
	progmode_fileuniq,
	progmode_fu,
	progmode_lc,
	progmode_charset,
	progmode_overlast
} ;

constexpr cpcchar	prognames[] = {
	[progmode_fileuniq]	= "fileuniq",
	[progmode_fu]		= "fu",
	[progmode_lc]		= "lc",
	[progmode_charset]	= "charset",
	[progmode_overlast]	= nullptr
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
} ;

static cint		maxpathlen = ulibval.maxpathlen ;
static cint		maxlinelen = ulibval.maxline ;

constexpr int		nents = NENTS ;


/* exported variables */


/* exported subroutines */

int main(int argc,mainv argv,mainv envv) {
	int		ex = EX_OK ;
	int		rs ;
	int		rs1 ;
	if (proginfo pi(argc,argv,envv) ; (rs = pi.start) >= 0) ylikely {
	    if ((rs = pi.flistbegin) >= 0) ylikely {
                switch (pi.pm) {
                case progmode_fileuniq:
                case progmode_fu:
                case progmode_lc:
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
	if ((rs = getpn(prognames)) >= 0) ylikely {
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
	    if ((argc > 0) && argv[0]) ylikely {
		cchar	*arg0 = argv[0] ;
	        cchar	*bp{} ;
	        if (int bl ; (bl = sfbasename(arg0,-1,&bp)) > 0) ylikely {
		    if (cint pl = rmchr(bp,bl,'.') ; pl > 0) ylikely {
	                if ((pm = matstr(names,bp,pl)) >= 0) ylikely {
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
	if ((rs = process_pmbegin()) >= 0) ylikely {
	    if (argc > 1) {
	        for (int ai = 1 ; (ai < argc) && argv[ai] ; ai += 1) {
	            cchar	*fn = argv[ai] ;
	            if (fn[0]) ylikely {
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
		rs = process_stdin() ;
	        c += rs ;
	    }
	    rs1 = process_pmend() ;
	    if (rs >= 0) rs = rs1 ;
	} /* end if (process_pm) */
	return (rs >= 0) ? c : rs ;
}
/* end subroutine (proginfo::process) */

int proginfo::process_stdin() noex {
	int		rs = SR_OK ;
	int		c = 0 ;
	switch (pm) {
	case progmode_fu:
	case progmode_fileuniq:
	    {
	        rs = readin() ;
	        c += rs ;
	    }
	    break ;
	case progmode_lc:
	    {
		rs = fileproc_lines() ;
		c += rs ;
	    }
	    break ;
	} /* end switch */
	return (rs >= 0) ? c : rs ;
}
/* end subroutine (proginfo::process_stdin) */

int proginfo::process_pmbegin() noex {
	int		rs = SR_OK ;
	switch (pm) {
        case progmode_fileuniq:
        case progmode_fu:
	    m = &proginfo::fileproc_fu ;
	    break ;
        case progmode_lc:
	    m = &proginfo::fileproc_lc ;
	    if ((rs = maxlinelen) >= 0) ylikely {
	        llen = rs ;
		rs = SR_NOMEM ;
	        if ((lbuf = new(nothrow) char[llen+1]) != nullptr) {
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
        case progmode_lc:
	    if (lbuf) {
		cout << lines << eol ;
		delete [] lbuf ;
		lbuf = nullptr ;
	        llen = 0 ;
	    } /* end block */
	    break ;
	} /* end switch */
	return rs ;
}
/* end subroutine (proginfo::process_pmend) */

int proginfo::readin() noex {
	istream		*isp = &cin ;
	int		rs ;
	int		c = 0 ;
	if ((rs = maxpathlen) >= 0) ylikely {
	    cint	plen = rs ;
	    char	*pbuf ;
	    rs = SR_NOMEM ;
	    if ((pbuf = new(nothrow) char[plen+1]) != nullptr) ylikely {
	        while ((rs = readln(isp,pbuf,plen)) > 0) {
		    int		pl = rs ;
		    if ((pl > 0) && (pbuf[pl - 1] == eol)) pl -= 1 ;
		    if (pl > 0) {
		        rs = fileproc(pbuf,pl) ;
			c += rs ;
		    }
		    if (rs < 0) break ;
	        } /* end if (reading lines) */
	        delete [] pbuf ;
	    } /* end if (m-a-f) */
	} /* end if (maxpathlen) */
	return (rs >= 0) ? c : rs ;
}
/* end method (proginfo::readin) */

int proginfo::fileproc(cchar *sp,int sl) noex {
	strnul		s(sp,sl) ;
	int		rs ;
	int		c = 0 ;
	if (ustat sb ; (rs = u_stat(s,&sb)) >= 0) ylikely {
	    if ((rs = filecheck(&sb)) > 0) {
		rs = (this->*m)(&sb,sp,sl) ;
		c += rs ;
	    } /* end if (filecheck) */
	} else if (isNotAccess(rs)) {
	    rs = SR_OK ;
	}
	return (rs >= 0) ? c : rs ;
}
/* end method (proginfo::fileproc) */

int proginfo::fileproc_fu(custat *,cchar *sp,int sl) noex {
	int		rs = SR_OK ;
	int		c = 0 ;
	if (sp) ylikely {
	    strview	fn(sp,sl) ;
	    cout << fn << eol ;
	    c += 1 ;
	}
	return (rs >= 0) ? c : rs ;
}
/* end method (proginfo::fileproc_fu) */

int proginfo::fileproc_lc(custat *sbp,cchar *sp,int sl) noex {
	int		rs = SR_OK ;
	int		rs1 ;
	int		c = 0 ;
	if (sbp && sp && sl) ylikely {
	    strview	fn(sp,sl) ;
	    if (S_ISREG(sbp->st_mode)) ylikely {
		c += 1 ;
	        if (ccfile rf ; (rs = rf.open(fn,"r",0)) >= 0) ylikely {
		    int		nl = 0 ;
		    while ((rs = rf.readln(lbuf,llen)) > 0) {
			nl += 1 ;
		    } /* end while */
		    lines += nl ;
		    rs1 = rf.close ;
		    if (rs >= 0) rs = rs1 ;
		 } /* end if (ccfile) */
	    } /* end if (is-reg) */
	} /* end if (non-null) */
	return (rs >= 0) ? c : rs ;
}
/* end method (proginfo::fileproc_lc) */

int proginfo::fileproc_lines() noex {
	istream		*isp = &cin ;
	int		rs ;
	int		c = 1 ;
	int		nl = 0 ;
	while ((rs = readln(isp,lbuf,llen)) > 0) {
	    nl += 1 ;
	} /* end if (reading lines) */
	lines += nl ;
	return (rs >= 0) ? c : rs ;
}
/* end method (proginfo::fileproc_lines) */

int proginfo::filecheck(custat *sbp)  noex {
	return seen.checkin(sbp) ;
}
/* end method (proginfo::filecheck) */

int proginfo_co::operator () (int) noex {
	int		rs = SR_BUGCHECK ;
	if (op) ylikely {
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


