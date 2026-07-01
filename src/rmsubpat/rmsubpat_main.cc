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
	$ rmsubpat <pattern> <dir(s)>

	Arguments:
	<pattern>	pattern to match on
	<dir(s)>	directories to search

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
	mainv	argv ;
	cchar	*patp ;
	int	argc ;
	int	patl ;
	dirmgr(int c,con mainv v) noex : argc(c), argv(v) { 
	    patp = nullptr ;
	    patl = 0 ;
	} ;
	operator int () noex ;
	int check(cchar *) noex ;
    } ; /* end struct (dirmgr) */
} /* end namespace */


/* forward references */


/* local constants */

cbool		f_debug = CF_DEBUG ;


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
}
/* end subroutine (main) */


/* local subroutines */

dirmgr::operator int () noex {
    	int		rs = SR_OK ;
	int		c = 0 ;
	DPRINTF("ent\n") ;
	for (int i = 1 ; (i < argc) && argv[i] ; i += 1) {
	    if (cchar *ap = argv[i] ; ap[0]) {
		DPRINTF("ap=%s\n",ap) ;
		if (patp) {
		    if (ustat sb ; (rs = u_stat(ap,&sb)) >= 0) {
			DPRINTF("u_stat() rs=%d\n",rs) ;
			if (S_ISDIR(sb.st_mode)) {
			    DPRINTF("-> check\n") ;
		            rs = check(ap) ;
			    c += 1 ;
			} else {
			    rs = SR_NOTDIR ;
			}
		    } /* end if (u_stat) */
		} else {
		    DPRINTF("pattern=»%s«\n",ap) ;
		    patp = ap ;
		    patl = lenstr(ap) ;
		} /* end if */
	    } /* end if (non-empty) */
	    if (rs < 0) break ;
	} /* end for */
	DPRINTF("ret rs=%d c=%d\n",rs,c) ;
	return rs ;
} /* end method (dirmgr::operator) */

int dirmgr::check(cchar *dn) noex {
	int		rs = SR_OK ;
	int		c = 0 ;
	DPRINTF("ent\n") ;
	for (dirit dir(dn) ; cauto &e : dir) {
            if (con path &p = e.path() ; e.is_regular_file()) {
                con path bn = p.filename() ;
                if (cchar *bns = bn.c_str() ; strncmp(bns,patp,patl) == 0) {
		    cchar *fn = p.c_str() ;
		    DPRINTF("prefix-match ps=%s\n",bns) ;
		    DPRINTF("prefix-match file-path=%s\n",fn) ;
		    rs = u_unlink(fn) ;
		    DPRINTF("u_unlink() rs=%d\n",rs) ;
		    c += 1 ;
		} /* end if (prefix match) */
            } /* end if (regular-file) */
	    if (rs < 0) break ;
        } /* end for */
	DPRINTF("ret rs=%d c=%d\n",rs,c) ;
	return (rs >= 0) ? c : rs ;
} /* end method (dirmgr::check) */


