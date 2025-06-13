/* charset_main SUPPORT (charset) */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* Character-Set (setting character-set on files) */
/* version %I% last-modified %G% */


/* revision history:

	= 2000-03-01, David A-D- Morano
	This subroutine was originally written.

*/

/* Copyright © 2000 David A-D- Morano.  All rights reserved. */

/*******************************************************************************

	Names:
	charset

	Description:
	This program sets the character set on the given files to
	the first argument of the program.

	Synopsis:
	$ charset <charset> <file(s)>

	Arguments:
	<charset>	character-set string to set on the given files
	<file(s)>	files to get a new character set

	Returns:
	==0		for normal operation success
	!=0		some error

*******************************************************************************/

#include	<envstandards.h>	/* ordered first to configure */
#include	<sys/stat.h>
#include	<sys/xattr.h>
#include	<unistd.h>
#include	<fcntl.h>
#include	<climits>
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<cstring>		/* |strlen(3c)| */
#include	<iostream>
#include	<usystem.h>
#include	<getourenv.h>
#include	<varnames.hh>
#include	<strn.h>
#include	<sfx.h>
#include	<matstr.h>
#include	<strwcpy.h>
#include	<strnul.hh>
#include	<readln.hh>
#include	<strnul.hh>
#include	<rmx.h>
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
using std::istream ;			/* type */
using std::cin;				/* variable */
using std::cout ;			/* variable */
using std::nothrow ;			/* constant */


/* local typedefs */


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
	int operator () (int = 1) noex ;
	operator int () noex {
	    return operator () (1) ;
	} ;
    } ; /* end struct (proginfo_co) */
    struct proginfo {
	friend proginfo_co ;
	fonce		seen ;
	proginfo_co	start ;
	proginfo_co	finish ;
	proginfo_co	flistbegin ;
	proginfo_co	flistend ;
	mainv		argv ;
	mainv		envv ;
	cchar		*pn = nullptr ;
	cchar		*charsetval = nullptr ;
	int		argc ;
	int		pm = 0 ;
	bool		ffound = false ;
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
	int charset() noex ;
	int charprocess(char *,int) noex ;
	int readin(char *,int) noex ;
	int fileproc(char *,int,cchar *,int = -1) noex ;
	int filecheck(CUSTAT *) noex ;
    private:
	int getpn(mainv) noex ;
	int istart() noex ;
	int ifinish() noex ;
	int iflistbegin(int) noex ;
	int iflistend() noex ;
    } ; /* end struct (proginfo) */
}


/* forward references */


/* local variables */

enum progmodes {
	progmode_charset,
	progmode_overlast
} ;

static constexpr cpcchar	prognames[] = {
	[progmode_charset]	= "charset",
	[progmode_overlast]	= nullptr
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
} ; /* end array (mapexs) */

constexpr cchar		xaname[] = "charset" ;

static cint		maxpathlen	= ulibval.maxpathlen ;
static cint		pagesize	= ulibval.pagesz ;

constexpr int		nents = NENTS ;



/* exported variables */


/* exported subroutines */

int main(int argc,mainv argv,mainv envv) noex {
	int		ex = EX_OK ;
	int		rs ;
	int		rs1 ;
	if (proginfo pi(argc,argv,envv) ; (rs = pi.start) >= 0) {
            switch (pi.pm) {
            case progmode_charset:
                rs = pi.charset() ;
                break ;
	    default:
		rs = SR_NOSYS ;
		break ;
            } /* end switch */
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

int proginfo::charset() noex {
	cnullptr	np{} ;
	int		rs = SR_INVALID ;
	int		rs1 ;
	int		c = 0 ; /* return-value */
	if ((argc >= 2) && (charsetval = argv[1]) != np) {
	    if ((rs = flistbegin(nents)) >= 0) {
		if ((rs = pagesize) >= 0) {
		    cnothrow	nt{} ;
		    cint	vlen = rs ;
		    if (char *vbuf ; (vbuf = new(nt) char[vlen+1]) != np) {
		        {
	                    rs = charprocess(vbuf,vlen) ;
		            c = rs ;
		        }
		        delete [] vbuf ;
	            } /* end if (m-a-f) */
		} /* end if (pagesize) */
	        rs1 = flistend ;
	        if (rs >= 0) rs = rs1 ;
	    } /* end if (flist) */
	} /* end if (had a character-set specification) */
	return (rs >= 0) ? c : rs ;
}
/* end method (proginfo::charset) */

int proginfo::charprocess(char *vbuf,int vlen) noex {
	int		rs = SR_OK ;
	int		c = 0 ;
	for (int ai = 2 ; (rs >= SR_OK) && (ai < argc) && argv[ai] ; ai += 1) {
            cchar   *fn = argv[ai] ;
            if (fn[0]) {
                if ((fn[0] == '-') && (fn[1] == '\0')) {
                    rs = readin(vbuf,vlen) ;
                    c += rs ;
                } else {
                    rs = fileproc(vbuf,vlen,fn) ;
                    c += rs ;
                }
            }
            if (rs < 0) break ;
	} /* end for */
	return (rs >= 0) ? c : rs ;
}
/* end subroutine (proginfo::charprocess) */

int proginfo::readin(char *vbuf,int vlen) noex {
	istream		*isp = &cin ;
	int		rs ;
	int		c = 0 ;
	if ((rs = maxpathlen) >= 0) {
	    cnullptr	np{} ;
	    cint	llen = rs ;
	    rs = SR_NOMEM ;
	    if (char *lbuf ; (lbuf = new(nothrow) char[llen+1]) != np) {
	        while ((rs = readln(isp,lbuf,llen)) > 0) {
		    int		ll = rs ;
		    if ((ll > 0) && (lbuf[ll - 1] == eol)) ll -= 1 ;
		    if (ll > 0) {
		        rs = fileproc(vbuf,vlen,lbuf,ll) ;
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

int proginfo::fileproc(char *vbuf,int vlen,cc *sp,int sl) noex {
	static csize	xasz = strlen(charsetval) ;
	strnul		fn(sp,sl) ;
	cint		rse = SR_EXISTS ;
	cint		xo = XATTR_CREATE ;
	int		rs ;
	int		c = 0 ;
	cchar		*xaval = charsetval ;
	(void) vbuf ;
	(void) vlen ;
	if ((rs = u_xattrset(fn,xaname,xaval,xasz,0u,xo)) == rse) {
	    rs = SR_OK ;
	    c += 1 ;
	}
	return (rs >= 0) ? c : rs ;
}
/* end subroutine (proginfo::fileproc) */

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

int proginfo::iflistbegin(int n) noex {
        return seen.start(n) ;
}
/* end method (proginfo::iflistbegin) */

int proginfo::iflistend() noex {
	return seen.finish ;
}
/* end method (proginfo::iflistend) */

int proginfo_co::operator () (int a) noex {
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
	        rs = op->iflistbegin(a) ;
	        break ;
	    case proginfomem_flistend:
	        rs = op->iflistend() ;
	        break ;
	    } /* end switch */
	} /* end if (non-null) */
	return rs ;
}
/* end method (proginfo_co::operator) */


