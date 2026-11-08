/* termop_main SUPPORT (termop) */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* print specified terminal sequences */
/* version %I% last-modified %G% */

#define	CF_DEBUG	0		/* debugging */

/* revision history:

	= 1989-03-01, David A-D- Morano
	This subroutine was originally written.

*/

/* Copyright © 1989 David A-D- Morano.  All rights reserved. */

/*******************************************************************************

	Names:
	termop

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
#include	<sys/param.h>		/* POSIX® */
#include	<unistd.h>		/* POSIX® */
#include	<fcntl.h>		/* POSIX® */
#include	<climits>		/* CSTD */
#include	<cstddef>		/* CSTD */
#include	<cstdlib>		/* CSTD */
#include	<cstring>		/* CSTD */
#include	<utility>		/* C++STD */
#include	<string>		/* C++STD */
#include	<iostream>		/* C++STD */
#include	<clanguage.h>		/* LIBU */
#include	<usysbase.h>		/* LIBU */
#include	<usyscalls.h>		/* LIBU */
#include	<usupport.h>		/* LIBU */
#include	<readln.hh>		/* LIBU */
#include	<termstr.h>		/* LIBU */
#include	<mapex.h>		/* LIBU */
#include	<localmisc.h>		/* LIBU */
#include	<dprint.hh>		/* LIBU |DPRINTF(3u)| */

#pragma		GCC dependency		"mod/libutil.ccm"

import libutil ;			/* |lenstr(3u)| */
import ureserve ;			/* |sf{x}(3u)| */

/* local defines */

#ifndef	MAXPATH
#define	MAXPATH		4096
#endif

#ifndef	CF_DEBUG
#define	CF_DEBUG	0		/* debugging */
#endif


/* imported namespaces */

using std::string ;			/* type */
using std::istream ;			/* type */
using libu::rmchr ;			/* subroutine */
using libu::matstr ;			/* subroutine */
using std::cout ;			/* variable */


/* local typedefs */

typedef const string	cstring ;


/* external subroutines */


/* external variables */


/* local structures */

namespace {
    enum proginfomems {
	proginfomem_start,
	proginfomem_finish,
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
    struct proginfo {
	friend		proginfo_co ;
	proginfo_co	start ;
	proginfo_co	finish ;
	mainv		argv ;
	mainv		envv ;
	cchar		*pn = nullptr ;
	int		argc ;
	int		pm = 0 ;
	proginfo(int c,mainv a,mainv e) noex : argc(c), argv(a), envv(e) { 
	    start	(this,proginfomem_start) ;
	    finish	(this,proginfomem_finish) ;
	} ; /* end ctor */
	proginfo() noex : proginfo(0,nullptr,nullptr) { } ;
	void operator () (int c,con mainv a,con mainv e) noex {
	    argc = c ;
	    argv = a ;
	    envv = e ;
	} ; /* end method */
	int termop	() noex ;
    private:
	int istart	() noex ;
	int ifinish	() noex ;
	int getpn	(con mainv) noex ;
    } ; /* end struct (proginfo) */
} /* end namespace */


/* forward references */


/* local variables */

enum progmodes {
	progmode_termop,
	progmode_overlast
} ; /* end enum */

constexpr cpcchar	prognames[] = {
	"termop",
	nullptr
} ; /* end array */

enum keys {
    	key_home,
	key_clearforward,
	key_overlast
} ; /* end enum */

constexpr cpcchar	keynames[] = {
	"home",
	"clearforward",
	"clearbackward",
	nullptr
} ; /* end array */

constexpr cpcchar	keystrs[] = {
	TERMSTR_HOME,	/* home */
	TERMSTR_ED,	/* clear-foward */
	"",		/* clear-backward */
	nullptr
} ; /* end array */

constexpr mapex_map	mapexs[] = {
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

cbool			f_debug		= CF_DEBUG ;


/* exported variables */


/* exported subroutines */

int main(int argc,con mainv argv,con mainv envv) {
	int		ex = EX_OK ;
	int		rs ;
	int		rs1 ;
	DPRINTF("ent\n") ;
	if (proginfo pi(argc,argv,envv) ; (rs = pi.start) >= 0) {
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
	if ((ex == EX_OK) && (rs < 0)) {
	    ex = mapex(mapexs,rs) ;
	} /* end if (error) */
	DPRINTF("ret ex=%d rs=%d\n",ex,rs) ;
	return ex ;
} /* end subroutine (main) */


/* local subroutines */

int proginfo::istart() noex {
	int		rs ;
	if ((rs = getpn(prognames)) >= 0) {
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
} /* end method (proginfo::getpn) */

int proginfo::termop() noex {
	int		rs = SR_OK ;
	for (int ai = 1 ; ai < argc ; ai += 1) {
	    if (ccharp kn = argv[ai] ; kn[0]) {
		DPRINTF("arg-kn=%s\n",kn) ;
		if (int si = matstr(keynames,kn) ; si >= 0) {
		    cout << keystrs[si] ;
		} else {
		    rs = SR_NOENT ;
		} /* end if */
	    } /* end if (non-empty) */
	    if (rs < 0) break ;
	} /* end for */
	return rs ;
} /* end method (proginfo::termop) */

int proginfo_co::operator () (int) noex {
	int	rs = SR_BUGCHECK ;
	if (op) ylikely {
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
} /* end method (proginfo_co::operator) */


