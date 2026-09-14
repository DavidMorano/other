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
	This program removes (deletes) files within the given
	directories that prefix-match on the given pattern.

	Synopsis:
	$ rmsubpat [-d <dir(s)>] <pattern(s)> 

	Arguments:
	<dir(s)>	directories to search
	<pattern(s)>	pattern(s) to match on

	Returns:
	EXIT_SUCCESS	OK
	EXIT_FAILURE	some problem

*******************************************************************************/

#include	<envstandards.h>	/* MUST be ordered first to configure */
#include	<cstddef>		/* CSTD */
#include	<cstdlib>		/* CSTD |EXIT_FAILURE| */
#include	<cstring>		/* CSTD |strncmp(3c)| */
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
	int findpats	() noex ;
	int patsload	(vecstr *) noex ;
	int checkargs	() noex ;
	int checkdirs	(cpcchar *,cchar *) noex ;
	int check	(cchar *,cchar *) noex ;
    } ; /* end struct (dirmgr) */
} /* end namespace */


/* forward references */


/* local constants */

constexpr cpcchar	dirs[] = {
    "gcm.cache",
    nullptr
} ; /* end array */

cbool			f_debug		= CF_DEBUG ;


/* exported variables */


/* exported subroutines */

int main(int argc,con mainv argv,con mainv) {
	int		ex = EX_OK ;
	int		rs = SR_OK ;
	DPRINTF("ent\n") ;
	if (argc > 1) {
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
	    rs = checkargs() ;
	    c += rs ;
	} else {
	    rs = checkfinds() ;
	    c += rs ;
	} /* end if (arguments or not) */
	DPRINTF("ret rs=%d c=%d\n",rs,c) ;
	return rs ;
} /* end method (dirmgr::operator) */

int dirmsg::checkargs() noex {
    	int		rs = SR_OK ;
	int		c = 0 ;
	DPRINTF("ent\n") ;
	    for (int ai = 1 ; (ai < argc) && argv[ai] ; ai += 1) {
	        if (cchar *ap = argv[ai] ; ap[0]) {
		    DPRINTF("ap=%s\n",ap) ;
		    rs = checkdirs(dirs,ap) ;
		    c += rs ;
	        } /* end if (non-empty) */
	        if (rs < 0) break ;
	    } /* end for */
	DPRINTF("ret rs=%d c=%d\n",rs,c) ;
	return (rs >= 0) ? c : rs ;
} /* end method (dirmgr::checkargs) */

int dirmsg::checkfinds() noex {
    	int		rs = SR_OK ;
	int		rs1 ;
	int		c = 0 ;
	DPRINTF("ent\n") ;
	if (vecstr pats ; (rs = pats.start) >= 0) {
	    if ((rs = patsload(&pats)) > 0) {
		rs = patscheck(&pats) ;
		c += rs ;
	    } /* end if (patsload) */
	    rs1 = pats.finish ;
	    if (rs >= 0) rs = rs1 ;
	} /* end if (vecstr) */
	DPRINTF("ret rs=%d c=%d\n",rs,c) ;
	return (rs >= 0) ? c : rs ;
} /* end method (dirmgr::checkfinds) */

int dirmgr::patsload(vecstr *plp) noex {
	int		rs = SR_OK ;
	int		c = 0 ;

	return (rs >= 0) ? c : rs ;
} /* end method (dirmgr::patsload) */

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
    	cint		patl = lenstr(patp) ;
	int		rs = SR_OK ;
	int		c = 0 ;
	DPRINTF("ent\n") ;
	for (dirit dir(dn) ; cauto &e : dir) {
            if (con path &p = e.path() ; e.is_regular_file()) {
                con path bn = p.filename() ;
                if (cchar *bns = bn.c_str() ; strncmp(bns,patp,patl) == 0) {
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


