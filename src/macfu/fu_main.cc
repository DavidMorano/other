/* fu_main SUPPORT (fu) */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* filter filenames */
/* version %I% last-modified %G% */

#define	CF_DEBUG	0		/* debugging */

/* revision history:

	= 1998-03-01, David A-D- Morano
	This subroutine was originally written.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Names:
	fileuniq
	fu
	lc

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
#include	<unistd.h>		/* POSIX */
#include	<fcntl.h>		/* POSIX */
#include	<climits>		/* CSTD */
#include	<cstddef>		/* CSTD */
#include	<cstdlib>		/* CSTD */
#include	<string>		/* C++STD */
#include	<string_view>		/* C++STD */
#include	<vector>		/* C++STD */
#include	<iostream>		/* C++STD */
#include	<clanguage.h>		/* LIBU */
#include	<usysbase.h>		/* LIBU */
#include	<usyscalls.h>		/* LIBU */
#include	<usupport.h>		/* LIBU */
#include	<strnul.hh>		/* LIBU */
#include	<ccfile.hh>		/* LIBU */
#include	<readln.hh>		/* LIBU */
#include	<path.h>		/* LIBUC |pathdangle(3uc)| */
#include	<mapex.h>		/* LIBU */
#include	<localmisc.h>		/* LIBU */
#include	<dprint.hh>		/* LIBU |DPRINTF(3u)| */
#include	<deb.hh>		/* LIBU |DEBPRINTF(3u)| */

#pragma		GCC dependency		"mod/libutil.ccm"
#pragma		GCC dependency		"mod/ulibvals.ccm"
#pragma		GCC dependency		"mod/ureserve.ccm"
#pragma		GCC dependency		"mod/fonce.ccm"

import libutil ;			/* |lenstr(3u)| */
import ulibvals ;
import ureserve ;
import fonce ;
import deb ;

/* local defines */

#ifndef	CF_DEBUG
#define	CF_DEBUG	0		/* debugging */
#endif

#define	VARPATHGROUP	"PATHGROUP_DFN"
#define	NENTS		1000


/* imported namespaces */

using std::string ;			/* type */
using std::string_view ;		/* type */
using std::istream ;			/* type */
using libu::snvprintf ;			/* subroutine */
using libu::rmchr ;			/* subroutine */
using libu::rmeol ;			/* subroutine */
using libu::rmtrailchr ;		/* subroutine */
using libu::matstr;			/* subroutine */
using libu::strnchr ;			/* subroutine */
using std::cin;				/* variable */
using std::cout ;			/* variable */


/* local typedefs */

typedef string_view	strview ;


/* external subroutines */


/* external variables */


/* local structures */

namespace {
    struct vars {
	int		maxpath ;
	int		maxline ;
	int		pagesz ;
	operator int () noex ;
    } ; /* end struct (vars) */
} /* end namespace */

namespace {
    enum proginfomems {
	proginfomem_start,
	proginfomem_finish,
	proginfomem_flistbegin,
	proginfomem_flistend,
	proginfomem_overlast
    } ; /* end enum */
    struct proginfo ;
    struct proginfo_co {
	proginfo	*op = nullptr ;
	int		w = -1 ;
	void operator () (proginfo *p,int m) noex {
	    op = p ;
	    w = m ;
	} ; /* end */
	int operator () (int = 0) noex ;
	operator int () noex {
	    return operator () (0) ;
	} ; /* end */
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
	ccharp		pn = nullptr ;
	charp		lbuf = nullptr ;
	int		argc ;
	int		pm = 0 ;
	int		llen = 0 ;
	int		lines = 0 ;
	proginfo(int c,mainv a,mainv e) noex : argc(c), argv(a), envv(e) { 
	    start	(this,proginfomem_start) ;
	    finish	(this,proginfomem_finish) ;
	    flistbegin	(this,proginfomem_flistbegin) ;
	    flistend	(this,proginfomem_flistend) ;
	    m		= nullptr ;
	} ; /* end ctor */
	proginfo() noex : proginfo(0,nullptr,nullptr) { } ;
	void operator () (int c,mainv a,mainv e) noex {
	    argc = c ;
	    argv = a ;
	    envv = e ;
	} ; /* end method */
	int filecheck		(custat *) noex ;
	int process		() noex ;
	int process_pmbegin	() noex ;
	int process_pmend	() noex ;
	int process_stdin	() noex ;
	int readin		() noex ;
	int fileproc		(cchar *,int = -1) noex ;
	int fileproc_fu		(custat *,cchar *,int = -1) noex ;
	int fileproc_lc		(custat *,cchar *,int = -1) noex ;
	int fileproc_dangling	(custat *,cchar *,int = -1) noex ;
	int fileproc_lines	() noex ;
    private:
	int istart		() noex ;
	int ifinish		() noex ;
	int getpn		(con mainv) noex ;
	int iflistbegin		() noex ;
	int iflistend		() noex ;
    } ; /* end struct (proginfo) */
} /* end namespace */


/* forward references */


/* local variables */

enum progmodes {
	progmode_fileuniq,
	progmode_fu,
	progmode_lc,
	progmode_dangling,
	progmode_overlast
} ; /* end enum (progmodes) */

constexpr cpcchar	prognames[] = {
	"fileuniq",
	"fu",
	"lc",
	"dangling",
	nullptr
} ; /* end array */

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
} ; /* end array */

static vars		var ;
cint			nents		= NENTS ;
cbool			f_debug		= CF_DEBUG ;


/* exported variables */


/* exported subroutines */

int main(int argc,con mainv argv,con mainv envv) {
	int		ex = EX_OK ;
	int		rs ;
	int		rs1 ;
	if (ccharp dcp = getenv(VARPATHGROUP)) {
	    if (cint rsd = DEBOPEN(dcp) ; rsd >= 0) {
		DEBPRINTF("debinit fd=%d\n",rsd) ;
	    } /* end if (DEBOPEN) */
	} /* end if (getenv) */
	if (static cint rsv = var ; (rs = rsv) >= 0) ylikely {
	    if (proginfo pi(argc,argv,envv) ; (rs = pi.start) >= 0) ylikely {
	        if ((rs = pi.flistbegin) >= 0) ylikely {
                    switch (pi.pm) {
                    case progmode_fileuniq:
                    case progmode_fu:
                    case progmode_lc:
                    case progmode_dangling:
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
	} /* end if (vars) */
	if ((ex == EX_OK) && (rs < 0)) {
	    ex = mapex(mapexs,rs) ;
	} /* end if (error) */
	return ex ;
} /* end subroutine (main) */


/* local subroutines */

int proginfo::istart() noex {
	int		rs ;
	if ((rs = getpn(prognames)) >= 0) ylikely {
	    rs = 0 ;
	} /* end if (proginfo::getpn) */
	return rs ;
} /* end method (proginfo::istart) */

int proginfo::ifinish() noex {
	return SR_OK ;
} /* end method (proginfo::ifinish) */

int proginfo::getpn(con mainv names) noex {
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
} /* end method (proginfo::getpn) */

int proginfo::iflistbegin() noex {
	return seen.start(nents) ;
} /* end method (proginfo::iflistbegin) */

int proginfo::iflistend() noex {
	return seen.finish ;
} /* end method (proginfo::iflistend) */

int proginfo::process() noex {
	int		rs ;
	int		rs1 ;
	int		c = 0 ;
	if ((rs = process_pmbegin()) >= 0) ylikely {
	    if (argc > 1) {
	        for (int ai = 1 ; (ai < argc) && argv[ai] ; ai += 1) {
	            if (ccharp fn = argv[ai] ; fn[0]) ylikely {
		        if ((fn[0] == '-') && (fn[1] == '\0')) {
		            rs = readin() ;
		            c += rs ;
		        } else {
	                    rs = fileproc(fn) ;
		            c += rs ;
		        } /* end */
	            } /* end */
	            if (rs < 0) break ;
	        } /* end for */
	    } else {
		rs = process_stdin() ;
	        c += rs ;
	    } /* end */
	    rs1 = process_pmend() ;
	    if (rs >= 0) rs = rs1 ;
	} /* end if (process_pm) */
	return (rs >= 0) ? c : rs ;
} /* end subroutine (proginfo::process) */

int proginfo::process_stdin() noex {
	int		rs = SR_OK ;
	int		c = 0 ;
	switch (pm) {
	case progmode_fileuniq:
	case progmode_fu:
	case progmode_dangling:
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
} /* end subroutine (proginfo::process_stdin) */

int proginfo::process_pmbegin() noex {
	int		rs = SR_OK ;
	cnullptr	np{} ;
	cnothrow	nt{} ;
	switch (pm) {
        case progmode_fileuniq:
        case progmode_fu:
	    m = &proginfo::fileproc_fu ;
	    break ;
        case progmode_dangling:
	    m = &proginfo::fileproc_dangling ;
	    break ;
        case progmode_lc:
	    m = &proginfo::fileproc_lc ;
	    if ((rs = var.maxline) >= 0) ylikely {
	        llen = rs ;
		rs = SR_NOMEM ;
	        if ((lbuf = new(nt) char[llen+1]) != np) {
		    rs = SR_OK ;
	        } else {
		    llen = 0 ;
		} /* end if */
	    } /* end if (var.maxline) */
	    break ;
	} /* end switch */
	return rs ;
} /* end subroutine (proginfo::process_pmbegin) */

int proginfo::process_pmend() noex {
	int		rs = SR_OK ;
	switch (pm) {
        case progmode_lc:
	    if (lbuf) {
		cout << lines << eol ;
		delete [] lbuf ;
		lbuf = nullptr ;
	        llen = 0 ;
	    } /* end if (delete-char) */
	    break ;
	} /* end switch */
	return rs ;
} /* end subroutine (proginfo::process_pmend) */

int proginfo::readin() noex {
	istream		*isp = &cin ;
	cnothrow	nt{} ;
	int		rs ;
	int		c = 0 ;
	if ((rs = var.maxpath) >= 0) ylikely {
	    cint	plen = rs ;
	    rs = SR_NOMEM ;
	    if (char *pbuf = new(nt) char[plen+1]) {
	        while ((rs = readln(isp,pbuf,plen)) > 0) {
		    if (cint pl = rmeol(pbuf,rs) ; pl > 0) {
		        rs = fileproc(pbuf,pl) ;
			c += rs ;
		    } /* end */
		    if (rs < 0) break ;
	        } /* end if (reading lines) */
	        delete [] pbuf ;
	    } /* end if (m-a-f) */
	} /* end if (var.maxpath) */
	return (rs >= 0) ? c : rs ;
} /* end method (proginfo::readin) */

int proginfo::fileproc(cchar *sp,int sl) noex {
	int		rs = SR_BUGCHECK ;
	int		c = 0 ;
	DPRINTF("ent pm=%d\n",pm) ;
	if (m) {
	    rs = SR_NOMEM ;
	    if (strnul fn(sp,sl) ; fn.fok) ylikely {
	        if (ustat sb ; (rs = u_lstat(fn,&sb)) >= 0) ylikely {
		    switch (pm) {
        	    case progmode_fileuniq:
        	    case progmode_fu:
        	    case progmode_lc:
	                if ((rs = filecheck(&sb)) > 0) {
		            rs = (this->*m)(&sb,sp,sl) ;
		            c += rs ;
	                } /* end if (filecheck) */
		        break ;
        	    case progmode_dangling:
			{
		            rs = (this->*m)(&sb,sp,sl) ;
		            c += rs ;
			}
		        break ;
		    } /* end switch */
	        } else if (isNotAccess(rs)) {
	            rs = SR_OK ;
	        } /* end if (ok) */
	    } /* end if (strnul) */
	} /* end if (non-null) */
	return (rs >= 0) ? c : rs ;
} /* end method (proginfo::fileproc) */

int proginfo::fileproc_fu(custat *,cchar *sp,int sl) noex {
	int		rs = SR_OK ;
	int		c = 0 ;
	if (sp) ylikely {
	    rs = SR_NOMEM ;
	    if (strnul fn(sp,sl) ; fn.fok) {
		rs = SR_OK ;
	        cout << ccp(fn) << eol ;
	        c += 1 ;
	    } /* end if (strnul) */
	} /* end if (non-null) */
	return (rs >= 0) ? c : rs ;
} /* end method (proginfo::fileproc_fu) */

int proginfo::fileproc_lc(custat *sbp,cchar *sp,int µsl) noex {
	int		rs = SR_OK ;
	int		rs1 ;
	int		c = 0 ;
	if (int sl ; sbp && (sl = getlenstr(sp,µsl)) >= 0) ylikely {
	    if (S_ISREG(sbp->st_mode)) ylikely {
	        strview fn(sp,sl) ;
		c += 1 ;
	        if (ccfile rf ; (rs = rf.open(fn,"r",0)) >= 0) ylikely {
		    int	nl = 0 ;
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
} /* end method (proginfo::fileproc_lc) */

int proginfo::fileproc_dangling(custat *sbp,cchar *sp,int sl) noex {
    	int		rs = SR_OK ;
	int		c = 0 ;
	DPRINTF("ent\n") ;
	if (sbp && sp && sl) ylikely {
	    if (S_ISLNK(sbp->st_mode)) {
	        if ((rs = pathdangle(sp,sl)) > 0) {
		    rs = SR_NOMEM ;
	            if (strnul fn(sp,sl) ; fn.fok) {
			rs = SR_OK ;
	                cout << ccp(fn) << eol ;
	                c += 1 ;
		    } /* end if (strnul) */
	        } /* end if (pathdangle) */
	    } /* end if (is_link) */
	} /* end if (non-null) */
	DPRINTF("ret rs=%d\n",rs) ;
	return (rs >= 0) ? c : rs ;
} /* end method (proginfo::fileproc_dangling) */

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
} /* end method (proginfo::fileproc_lines) */

int proginfo::filecheck(custat *sbp)  noex {
	return seen.checkin(sbp) ;
} /* end method (proginfo::filecheck) */

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
} /* end method (proginfo_co::operator) */

vars::operator int () noex {
    	int		rs ;
	if ((rs = ulibval.maxpathlen) >= 0) ylikely {
	    maxpath = rs ;
	    if ((rs = ulibval.maxline) >= 0) ylikely {
		maxline = rs ;
	        if ((rs = ulibval.pagesz) >= 0) ylikely {
		    pagesz = rs ;
	        } /* end */
	    } /* end */
	} /* end */
    	return rs ;
} /* end method (vars::operator) */


