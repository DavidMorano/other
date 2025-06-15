/* files_main SUPPORT (files) */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* enumerate filenames */
/* version %I% last-modified %G% */


/* revision history:

	= 2017-03-21, David A-D- Morano
	This subroutine was originally written.

*/

/* Copyright © 2017 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Names:
	files_main

	Description:
	This program enumerates directories and files.

	Synopsis:
	main(int,argv,argv)

	Arguments:

	Returns:
	==0		for normal operation success (EX_OK)
	!=0		some error (! EX_OK)

*******************************************************************************/

#include	<envstandards.h>	/* ordered first to configure */
#include	<unistd.h>
#include	<fcntl.h>
#include	<climits>
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<cstdio>
#include	<new>			/* |nothrow(3c++)| */
#include	<string>
#include	<string_view>
#include	<filesystem>
#include	<iostream>
#include	<usystem.h>
#include	<getourenv.h>
#include	<varnames.hh>
#include	<strn.h>
#include	<sfx.h>
#include	<rmx.h>
#include	<strwcpy.h>
#include	<strnul.hh>
#include	<ccfile.hh>
#include	<readln.hh>
#include	<filetype.h>
#include	<matstr.h>
#include	<isnot.h>
#include	<mapex.h>
#include	<exitcodes.h>
#include	<localmisc.h>

import libutil ;			/* |xstrlen(3u)| */
import fonce ;
import ulibvals ;

/* local defines */

#define	NENTS		1000


/* imported namespaces */

namespace fs = std::filesystem ;

using std::nullptr_t ;			/* type */
using fs::recursive_directory_iterator ;	/* type */
using std::string ;			/* type */
using std::string_view ;		/* type */
using std::istream ;			/* type */
using std::cin;				/* variable */
using std::cout ;			/* variable */
using std::cerr ;			/* variable */
using std::nothrow ;			/* constant */


/* local typedefs */

typedef string_view			strview ;
typedef recursive_directory_iterator	rdi ;


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
    struct proginfo_fl {
	uint		uniqfile:1 ;		/* 'u' arg-opt */
	uint		uniqdir:1 ;		/* 'ud' arg-opt */
	uint		followarg:1 ;		/* 'H' arg-opt */
	uint		followall:1 ;		/* 'L' arg-opt */
    } ;
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
	proginfo_fl	fl{} ;
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
	int fileuniq(custat *) noex ;
	int process() noex ;
	int process_pmbegin() noex ;
	int process_pmend() noex ;
	int readin() noex ;
	int procname(cchar *,int = -1) noex ;
	int procfile_list(custat *,cchar *,int = -1) noex ;
	int procfile_lc(custat *,cchar *,int = -1) noex ;
	int procdir(custat *,cchar *,int = -1) noex ;
	int procent(custat *,cchar *,int = -1) noex ;
	int procfile(custat *,cchar *,int = -1) noex ;
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
	progmode_files,
	progmode_filelines,
	progmode_overlast
} ;

constexpr cpcchar	prognames[] = {
	[progmode_files]	= "files",
	[progmode_filelines]	= "filelines",
	[progmode_overlast]	= nullptr
} ;

enum argopts {
    	argopt_version,
    	argopt_debug,
    	argopt_help,
	argopt_overlast
} ;

constexpr cpcchar	argopts[] = {
    	[argopt_version]	= "VERSION",
    	[argopt_debug]		= "DEBUG",
    	[argopt_help]		= "HELP",
	[argopt_overlast]	= nullptr
} ;

enum arglongs {
    	arglong_version,
	arglong_overlast
} ;

constexpr cpcchar	arglongs[] = {
    	[arglong_version]	= "version",
	[arglong_overlast]	= nullptr
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

static cint		maxpathlen = ulibval.maxpathlen ;
static cint		maxlinelen = ulibval.maxline ;
constexpr int		nents = NENTS ;


/* exported variables */


/* exported subroutines */

int main(int argc,mainv argv,mainv envv) noex {
	int		ex = EX_OK ;
	int		rs ;
	int		rs1 ;
	if (proginfo pi(argc,argv,envv) ; (rs = pi.start) >= 0) {
	    if ((rs = pi.flistbegin) >= 0) {
                switch (pi.pm) {
                case progmode_files:
                case progmode_filelines:
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
		    if (cint pl = rmchr(bp,bl,'.') ; pl > 0) {
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
	    if (argc > 1) {
	        for (int ai = 1 ; (ai < argc) && argv[ai] ; ai += 1) {
	            cchar	*fn = argv[ai] ;
	            if (fn[0]) {
		        if (fn[0] == '-') {
			    if (fn[1] == '\0') {
		                rs = readin() ;
		                c += rs ;
			    }
		        } else {
	                    rs = procname(fn) ;
		            c += rs ;
		        }
	            }
	            if (rs < 0) break ;
	        } /* end for */
	    } /* end if (have argument) */
	    rs1 = process_pmend() ;
	    if (rs >= 0) rs = rs1 ;
	} /* end if (process_pm) */
	return (rs >= 0) ? c : rs ;
}
/* end subroutine (proginfo::process) */

int proginfo::process_pmbegin() noex {
	int		rs = SR_OK ;
	switch (pm) {
        case progmode_files:
	    break ;
        case progmode_filelines:
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
        case progmode_filelines:
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
	cnullptr	np{} ;
	int		rs ;
	int		c = 0 ;
	if ((rs = maxpathlen) >= 0) {
	    cint	plen = rs ;
	    rs = SR_NOMEM ;
	    if (char *pbuf ; (pbuf = new(nothrow) char[plen + 1]) != np) {
	        while ((rs = readln(&cin,pbuf,plen)) > 0) {
		    if (cint pl = rmeol(pbuf,rs) ; pl > 0) {
		        rs = procname(pbuf,pl) ;
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

int proginfo::procname(cchar *sp,int sl) noex {
	strnul		s(sp,sl) ;
	int		rs ;
	int		c = 0 ;
	if (USTAT sb ; (rs = u_lstat(s,&sb)) >= 0) {
	    cint ft = filetype(sb.st_mode) ;
	    switch (ft) {
	    case filetype_dir:
		rs = procdir(&sb,sp,sl) ;
		c = rs ;
		break ;
	    case filetype_reg:
		rs = procfile(&sb,sp,sl) ;
		c = rs ;
		break ;
	    } /* end switch */
	} else if (isNotAccess(rs)) {
	    rs = SR_OK ;
	}
	return (rs >= 0) ? c : rs ;
} /* end method (proginfo::procname) */

int proginfo::procdir(custat *sbp,cchar *sp,int sl) noex {
    	using enum	fs::path::format ;
    	using enum	fs::directory_options ;
    	int		rs = SR_OK ;
	int		c = 0 ;
	(void) sbp ;
	if (sl && sp[0]) {
	    if (sl < 0) sl = xstrlen(sp) ;
	    try {
	        fs::path p(sp,(sp + sl),native_format) ;
    	        fs::directory_options dopt = skip_permission_denied ;
		std::error_code ec ;
		{
		    rdi it(p,dopt,ec) ;
		    for (cauto &de : it) {
			const fs::path depath = de ;
			{
			    cchar *fn = depath.c_str() ;
			    if (ustat sb ; (rs = u_lstat(fn,&sb)) >= 0) {
				rs = procent(&sb,fn,-1) ;
			        c = rs ;
			    }
			}
		    } /* end for */
		}
	    } catch (...) {
		rs = SR_NOMEM ;
	    }
	} /* end if (non-empty) */
	return (rs >= 0) ? c : rs ;
} /* end method (proginfo::procdir) */

int proginfo::procent(custat *sbp,cchar *sp,int sl) noex {
	cint		ft = filetype(sbp->st_mode) ;
    	int		rs = SR_OK ;
	int		c = 0 ;
	switch (ft) {
	case filetype_reg:
	    rs = procfile(sbp,sp,sl) ;
	    c = rs ;
	} /* end switch */
	return (rs >= 0) ? c : rs ;
} /* end method (proginfo::procent) */

int proginfo::procfile(custat *sbp,cchar *sp,int sl) noex {
    	int		rs ;
	int		c = 0 ;
	if ((rs = fileuniq(sbp)) > 0) {
	    switch (pm) {
	    case progmode_files:
		rs = procfile_list(sbp,sp,sl) ;
		c = rs ;
	        break ;
	    case progmode_filelines:
		rs = procfile_lc(sbp,sp,sl) ;
		c = rs ;
	        break ;
	    } /* end switch */
	} /* end if (fileuniq) */
	return (rs >= 0) ? c : rs ;
} /* end method (proginfo::procfile) */

int proginfo::procfile_list(custat *,cchar *sp,int sl) noex {
	int		rs = SR_OK ;
	int		c = 0 ;
	if (sp) {
	    strnul	fn(sp,sl) ;
	    cout << fn << eol ;
	    c += 1 ;
	}
	return (rs >= 0) ? c : rs ;
}
/* end method (proginfo::procfile_list) */

int proginfo::procfile_lc(custat *sbp,cchar *sp,int sl) noex {
	int		rs = SR_OK ;
	int		rs1 ;
	int		c = 0 ;
	if (sbp && sp && sl) {
	    if (S_ISREG(sbp->st_mode)) {
	        strnul fn(sp,sl) ;
		c += 1 ;
	        if (ccfile rf ; (rs = rf.open(fn,"r",0)) >= 0) {
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
/* end method (proginfo::procfile_lc) */

int proginfo::fileuniq(custat *sbp)  noex {
    	int		rs = 1 ; /* default indicates "uniq" */
	if (fl.uniqfile) {
	    rs = seen.checkin(sbp) ;
	}
	return rs ;
}
/* end method (proginfo::fileuniq) */

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


