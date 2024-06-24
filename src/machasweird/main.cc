/* main SUPPORT (hasweird) */
/* lang=C++20 */

/* weird character identification */
/* version %I% last-modified %G% */


/* revision history:

	= 1989-03-01, David A.D. Morano
	This subroutine was originally written.

*/

/* Copyright © 1989 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Names:
	hasweird

	Description:
	This program prints out the lines (read from STDIN) that have
	weirdo characters in them.

	Synopsis:
	$ hasweird

	Arguments:
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
#include	<six.h>
#include	<matstr.h>
#include	<strwcpy.h>
#include	<strnul.hh>
#include	<sncpyx.h>
#include	<getourenv.h>
#include	<mapblock.hh>
#include	<readln.hh>
#include	<ccfile.hh>
#include	<rmx.h>
#include	<hasx.h>
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
    typedef int (proginfo::*proginfo_m)(CUSTAT *,cchar *,int) noex ;
    struct proginfo {
	friend		proginfo_co ;
	proginfo_co	start ;
	proginfo_co	finish ;
	proginfo_co	flistbegin ;
	proginfo_co	flistend ;
	fonce		seen ;
	proginfo_m	m ;
	mainv		argv ;
	mainv		envv ;
	cchar		*pn = nullptr ;
	char		*lbuf = nullptr ;
	int		argc ;
	int		pm = 0 ;
	int		llen = 0 ;
	int		lines = 0 ;
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
	int filecheck(CUSTAT *) noex ;
	int process() noex ;
	int process_pmbegin() noex ;
	int process_pmend() noex ;
	int process_stdin() noex ;
	int readin() noex ;
	int fileproc(cchar *,int = -1) noex ;
	int fileproc_fu(CUSTAT *,cchar *,int = -1) noex ;
	int fileproc_hasweird(CUSTAT *,cchar *,int = -1) noex ;
	int fileproc_lines() noex ;
    private:
	int istart() noex ;
	int ifinish() noex ;
	int getpn(mainv) noex ;
	int iflistbegin() noex ;
	int iflistend() noex ;
    } ; /* end struct (proginfo) */
}


/* forward references */

static int	weirdcheck(cchar *,int) noex ;


/* local variables */

enum progmodes {
	progmode_hasweird,
	progmode_overlast
} ;

static constexpr cpcchar	prognames[] = {
	"hasweird",
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
constexpr int		maxlinelen = MAXLINELEN ;


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
                case progmode_hasweird:
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
	return SR_OK ;
}
/* end method (proginfo::iflistbegin) */

int proginfo::iflistend() noex {
	return SR_OK ;
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
	case progmode_hasweird:
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
        case progmode_hasweird:
	    m = &proginfo::fileproc_hasweird ;
	    if ((rs = maxlinelen) >= 0) {
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
        case progmode_hasweird:
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

int proginfo::readin() noex {
	istream		*isp = &cin ;
	int		rs ;
	int		c = 0 ;
	if ((rs = maxpathlen) >= 0) {
	    cint	plen = rs ;
	    char	*pbuf ;
	    rs = SR_NOMEM ;
	    if ((pbuf = new(nothrow) char[plen+1]) != nullptr) {
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
	USTAT		sb ;
	strnul		s(sp,sl) ;
	int		rs ;
	int		c = 0 ;
	if ((rs = u_stat(s,&sb)) >= 0) {
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

int proginfo::fileproc_hasweird(CUSTAT *sbp,cchar *sp,int sl) noex {
	strview		fn(sp,sl) ;
	int		rs = SR_OK ;
	int		rs1 ;
	int		c = 0 ;
	if (sp && sl) {
	    strview	fn(sp,sl) ;
	    if (S_ISREG(sbp->st_mode)) {
	        ccfile	rf ;
		c += 1 ;
	 	if ((rs = rf.open(fn,"r",0)) >= 0) {
		    int		nl = 0 ;
		    while ((rs = rf.readln(lbuf,llen)) > 0) {
	    	        int	ll = rs ;
	    	        if ((ll > 0) && (lbuf[ll - 1] == eol)) ll -= 1 ;
	    	        rs = weirdcheck(lbuf,ll) ;
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
/* end method (proginfo::fileproc_hasweird) */

int proginfo::fileproc_lines() noex {
	istream		*isp = &cin ;
	int		rs ;
	int		c = 1 ;
	int		nl = 0 ;
	while ((rs = readln(isp,lbuf,llen)) > 0) {
	    int		ll = rs ;
	    if ((ll > 0) && (lbuf[ll - 1] == eol)) ll -= 1 ;
	    rs = weirdcheck(lbuf,ll) ;
	    nl += 1 ;
	} /* end if (reading lines) */
	lines += nl ;
	return (rs >= 0) ? c : rs ;
}
/* end method (proginfo::fileproc_lines) */

int proginfo::filecheck(CUSTAT *)  noex {
	return 1 ;
}
/* end method (proginfo::filecheck) */

static int weirdcheck(cchar *lbuf,int llen) noex {
	constexpr cchar	wc[] = R"xx( `\'":)xx" ;
	int		rs = SR_OK ;
	if (sibrk(lbuf,llen,wc) >= 0) {
	    strview	ln(lbuf,llen) ;
	    cout << "»" << ln << "«\n" ;
	} else if (hasprintbad(lbuf,llen)) {
	    strview	ln(lbuf,llen) ;
	    cout << "»" << ln << "«\n" ;
	}
	return rs ;
}
/* end method (weirdcheck) */

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


