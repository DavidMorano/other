/* main SUPPORT (termop) */
/* lang=C++20 */

/* print specified terminal sequences */
/* version %I% last-modified %G% */


/* revision history:

	= 1989-03-01, David A-D- Morano
	This subroutine was originally written.

*/

/* Copyright © 1989 David A-D- Morano.  All rights reserved. */

/*******************************************************************************

	Names:
	termopt

	Description:
	Print to STDOUT whatever sequences are specified by arguments.
	The current possible arguments are:
		home
		clearforward

	Synopsis:
	$ termopt <key>

	Arguments:
	home		print sequence to put cursor to HOME position
	clearforward	print sequence to clear-forward

	Returns:
	0		for normal operation success
	126		haveprogram: some program found but not executable
	127		haveprogram: program not found

*******************************************************************************/

#include	<envstandards.h>	/* ordered first to configure */
#include	<sys/param.h>
#include	<unistd.h>
#include	<fcntl.h>
#include	<climits>
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<cstring>		/* <- for |strlen(3c)| */
#include	<utility>		/* |pair(3c++)| */
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


/* local defines */

#ifndef	MAXPATH
#define	MAXPATH		4096
#endif


/* imported namespaces */

using std::nullptr_t ;			/* type */
using std::string ;			/* type */
using std::string_view ;		/* type */
using std::unordered_map ;		/* type */
using std::vector ;			/* type */
using std::istream ;			/* type */
using std::pair ;			/* type */
using std::cin;				/* variable */
using std::cout ;			/* variable */
using std::cerr ;			/* variable */
using std::nothrow ;			/* constant */


/* local typedefs */

typedef string_view	strview ;

typedef const strview	cstrview ;
typedef const string	cstring ;


/* external subroutines */


/* external variables */


/* local structures */

namespace {
    enum proginfomems {
	proginfomem_start,
	proginfomem_finish,
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
	typedef mapblock<devino,filenode>		nodedb ;
	typedef mapblock<devino,filenode>::iterator	flit_t ;
	friend proginfo_co ;
	proginfo_co	start ;
	proginfo_co	finish ;
	mainv		argv ;
	mainv		envv ;
	cchar		*pn = nullptr ;
	int		argc ;
	int		pm = 0 ;
	proginfo(int c,mainv a,mainv e) noex : argc(c), argv(a), envv(e) { 
	    start(this,proginfomem_start) ;
	    finish(this,proginfomem_finish) ;
	} ;
	proginfo() noex : proginfo(0,nullptr,nullptr) { } ;
	void operator () (int c,mainv a,mainv e) noex {
	    argc = c ;
	    argv = a ;
	    envv = e ;
	} ;
	int output() noex ;
    private:
	int istart() noex ;
	int ifinish() noex ;
	int getpn(mainv) noex ;
	int termop() noex ;
    } ; /* end struct (proginfo) */
}


/* forward references */


/* local variables */

enum progmodes {
	progmode_termop,
	progmode_overlast
} ;

constexpr cpcchar	prognames[] = {
	"termop",
	nullptr
} ;

enum keys {
    	key_home,
	key_clearforward,
	key_overlast
} ;

constexpr cpcchar	keynames[] = {
	"home",
	"clearforward",
	nullptr
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

constexpr int		maxpathlen = MAXPATH ;


/* exported variables */


/* exported subroutines */

int main(int argc,mainv argv,mainv envv) noex {
	proginfo	pi(argc,argv,envv) ;
	int		ex = EX_OK ;
	int		rs ;
	int		rs1 ;
	if ((rs = pi.start) >= 0) {
                switch (pi.pm) {
                case progmode_termop:
                    rs = pi.termop() ;
                    break ;
		default:
		    rs = SR_BUGCHECK ;
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
	        if (cchar *bp{} ; (bl = sfbasename(arg0,-1,&bp)) > 0) {
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

int proginfo::termop() noex {
	int		rs = SR_OK ;
	for (int ai ; ai = 1 ; ai < argc ; ai += 1) {
	    cchar	*kn = argv[ai] ;

	    if (rs < 0) break ;
	} /* end for */
	return rs ;
}
/* end method (proginfo::termop) */

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
	    } /* end switch */
	} /* end if (non-null) */
	return rs ;
}
/* end method (proginfo_co::operator) */


