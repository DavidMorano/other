/* rmsubpat_main SUPPORT (rmsubpat) */
/* charset=ISO8859-1 */
/* lang=C++23 */

/* print out matching filenames */
/* version %I% last-modified %G% */

#define	CF_DEBUG	0		/* debugging */

/* revision history:

	= 2001-04-11, David A­D­ Morano
	Originally written for Rightcore Network Services.

*/

/* Copyright © 2001 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Name:
	rmsubpat

	Description:
	This program is used to remove files -- that contain a given
	pattern -- from the specified directories.  If no patterns
	are given, patterns are determined (if present at all) from
	the current directory from files that are C++ source modules.

	Synopsis:
	$ rmsubpat [-d <dir(s)>] [<pattern(s)>]

	Arguments:
	<dir(s)>	optional directories to search
	<pattern(s)>	optional pattern(s) to match on

	Returns:
	EXIT_SUCCESS	OK
	EXIT_FAILURE	some problem

*******************************************************************************/

#include	<envstandards.h>	/* MUST be ordered first to configure */
#include	<cstddef>		/* CSTD */
#include	<cstdlib>		/* CSTD |EXIT_FAILURE| */
#include	<cstring>		/* CSTD |strncmp(3c)| + |strstr(3c)| */
#include	<exception>		/* C++STD */
#include	<filesystem>		/* C++STD */
#include	<clanguage.h>		/* LIBU */
#include	<usysbase.h>		/* LIBU */
#include	<usyscalls.h>		/* LIBU */
#include	<mapex.h>		/* LIBU */
#include	<localmisc.h>		/* LIBU |eol| */
#include	<dprint.hh>		/* LIBU |DPRINTF(3u)| */

#pragma		GCC dependency		"mod/libutil.ccm"

import libutil ;			/* |lenstr(3u)| */
import ureserve ;			/* |vecstr(3u)| */

/* local defines */

#ifndef	CF_DEBUG
#define	CF_DEBUG	0		/* debugging */
#endif


/* imported namespaces */

using namespace	std::filesystem ;	/* namespace */

using std::error_code ;			/* type */
using libu::siext ;			/* subroutine */
using libu::hasext ;			/* subroutine */


/* local typedefs */

typedef directory_iterator	dirit ;


/* external subroutines */


/* external variables */


/* local structures */

namespace {
    struct dirmgr {
	mainv		argv ;
	int		argc ;
	dirmgr(int c,con mainv v) noex : argc(c), argv(v) { } ;
	operator int () noex ;
	int checkargs	(cpccharp) noex ;
	int checkdefs	(cpccharp) noex ;
	int checkdirs	(cpccharp,cchar *) noex ;
	int check	(cchar *,cchar *) noex ;
	int defload	(vecstr *) noex ;
	int defcheck	(cpccharp,vecstr *) noex ;
    } ; /* end struct (dirmgr) */
} /* end namespace */


/* forward references */


/* local constants */

constexpr cpcchar	dirs[] = {
    "gcm.cache",
    nullptr
} ; /* end array */

cchar			defext[]	= "ccm" ;
cbool			f_debug		= CF_DEBUG ;


/* exported variables */


/* exported subroutines */

int main(int argc,con mainv argv,con mainv) {
	int		ex = EX_OK ;
	int		rs = SR_OK ;
	DPRINTF("ent\n") ;
	if (argc >= 0) {
	    dirmgr dm(argc,argv) ;
	    rs = dm ;
	} /* end if (arguments) */
	if ((ex == EX_OK) && (rs < 0)) {
	    ex = mapex(nullptr,rs) ;
	} /* end if */
	DPRINTF("ret ex=%d rs=%d\n",ex,rs) ;
	return ex ;
} /* end subroutine (main) */


/* local subroutines */

dirmgr::operator int () noex {
    	int		rs = SR_OK ;
	int		c = 0 ;
	DPRINTF("ent\n") ;
	if (argc > 1) {
	    rs = checkargs(dirs) ;
	    c += rs ;
	} else {
	    rs = checkdefs(dirs) ;
	    c += rs ;
	} /* end if (arguments or not) */
	DPRINTF("ret rs=%d c=%d\n",rs,c) ;
	return rs ;
} /* end method (dirmgr::operator) */

int dirmgr::checkargs(cpccharp dirpp) noex {
    	int		rs = SR_OK ;
	int		c = 0 ;
	DPRINTF("ent\n") ;
	    for (int ai = 1 ; (ai < argc) && argv[ai] ; ai += 1) {
	        if (cchar *ap = argv[ai] ; ap[0]) {
		    DPRINTF("ap=%s\n",ap) ;
		    rs = checkdirs(dirpp,ap) ;
		    c += rs ;
	        } /* end if (non-empty) */
	        if (rs < 0) break ;
	    } /* end for */
	DPRINTF("ret rs=%d c=%d\n",rs,c) ;
	return (rs >= 0) ? c : rs ;
} /* end method (dirmgr::checkargs) */

int dirmgr::checkdefs(cpccharp dirpp) noex {
    	int		rs = SR_OK ;
	int		rs1 ;
	int		c = 0 ;
	DPRINTF("ent\n") ;
	if (vecstr pats ; (rs = pats.start) >= 0) ylikely {
	    if ((rs = defload(&pats)) > 0) ylikely {
		rs = defcheck(dirpp,&pats) ;
		c += rs ;
	    } /* end if (defload) */
	    rs1 = pats.finish ;
	    if (rs >= 0) rs = rs1 ;
	} /* end if (vecstr) */
	DPRINTF("ret rs=%d c=%d\n",rs,c) ;
	return (rs >= 0) ? c : rs ;
} /* end method (dirmgr::checkdefs) */

int dirmgr::defload(vecstr *plp) noex {
	int		rs = SR_OK ;
	int		c = 0 ;
	DPRINTF("ent\n") ;
	error_code ec ;
	if (path cur = current_path(ec) ; (! ec)) ylikely {
	    for (dirit dir(cur) ; cauto &e : dir) {
                if (con path &p = e.path() ; e.is_regular_file()) ylikely {
                    con path bn = p.filename() ;
                    if (cpcchar bns = bn.c_str() ; bns[0]) ylikely {
			DPRINTF("bns=%s\n",bns) ;
		        if (cint si = siext(bns,-1) ; si >= 0) {
			    DPRINTF("si=%d ext=%s\n",si,(bns+si)) ;
			    if (strcmp((bns+si),defext) == 0) {
			        rs = plp->add(bns,(si - 1)) ;
			        c += 1 ;
			    } /* end if (strcmp) */
		        } /* end if (siext) */
		    } /* end if (get c-string) */
                } /* end if (regular-file) */
	        if (rs < 0) break ;
            } /* end for */
	} else {
	    rs = (neg ec.value()) ;
	} /* end if (path) */
	DPRINTF("ret rs=%d c=%d\n",rs,c) ;
	return (rs >= 0) ? c : rs ;
} /* end method (dirmgr::defload) */

int dirmgr::defcheck(cpccharp dirpp,vecstr *plp) noex {
	int		rs = SR_OK ;
	int		c = 0 ;
	DPRINTF("ent\n") ;
	cchar *pp ;
	for (int i = 0 ; plp->get(i,&pp) >= 0 ; i += 1) {
	    if (pp) {
		if (pp[0]) {
		    rs = checkdirs(dirpp,pp) ;
		    c += rs ;
		} /* end if (non-empty) */
	    } /* end */
	    if (rs < 0) break ;
	} /* end for */
	DPRINTF("ret rs=%d c=%d\n",rs,c) ;
	return (rs >= 0) ? c : rs ;
} /* end method (dirmgr::defcheck) */

int dirmgr::checkdirs(cpcchar *dirpp,cchar *patp) noex {
	int		rs = SR_OK ;
	int		c = 0 ;
	DPRINTF("ent\n") ;
	for (int i = 0 ; dirpp[i] ; i += 1) {
	    DPRINTF("dn=%s\n",dirpp[i]) ;
	    if (ccharp dn = dirpp[i] ; dn[0]) ylikely {
	        if (ustat sb ; (rs = u_stat(dn,&sb)) >= 0) ylikely {
		    if (S_ISDIR(sb.st_mode)) {
	    		DPRINTF("yes-is-dir\n") ;
	                rs = check(dn,patp) ;
	                c += rs ;
		    } /* end if (was a directory) */
		} /* end if (u_stat) */
	    } /* end if (non-empty) */
	    if (rs < 0) break ;
	} /* end for */
	DPRINTF("ret rs=%d c=%d\n",rs,c) ;
	return (rs >= 0) ? c : rs ;
} /* end method (dirmgr::checkdirs) */

int dirmgr::check(cchar *dn,cchar *patp) noex {
	int		rs = SR_OK ;
	int		c = 0 ;
	DPRINTF("ent\n") ;
	for (dirit dir(dn) ; cauto &e : dir) {
            if (con path &p = e.path() ; e.is_regular_file()) {
                con path bn = p.filename() ;
                if (cchar *bns = bn.c_str() ; strstr(bns,patp)) {
		    DPRINTF("prefix-match ps=%s\n",bns) ;
		    if (cchar *fn = p.c_str() ; fn[0]) {
		        DPRINTF("prefix-match file-path=%s\n",fn) ;
		        rs = u_unlink(fn) ;
		        DPRINTF("u_unlink() rs=%d\n",rs) ;
		        c += 1 ;
		    } /* end if (non-empty) */
		} /* end if (prefix match) */
            } /* end if (regular-file) */
	    if (rs < 0) break ;
        } /* end for */
	DPRINTF("ret rs=%d c=%d\n",rs,c) ;
	return (rs >= 0) ? c : rs ;
} /* end method (dirmgr::check) */


