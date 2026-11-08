/* rmdirs_main SUPPORT */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* test the |u_rmdirs(3u)| subroutine */
/* version %I% last-modified %G% */


/* revision history:

	= 2000-05-14, David A­D­ Morano
	Originally written for Rightcore Network Services.

*/

/* Copyright © 2000 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

  	Name:

	Description:
	This program removes whole directory trees.

*******************************************************************************/

#include	<envstandards.h>	/* ordered first to configure */
#include	<cstddef>		/* CSTD */
#include	<cstdlib>		/* CSTD */
#include	<cstdio>		/* CSTD */
#include	<clanguage.h>		/* LIBU */
#include	<usysbase.h>		/* LIBU */
#include	<usyscalls.h>		/* LIBU |u_rmdirs(3u)| */
#include	<mapex.h>		/* LIBU */
#include	<localmisc.h>		/* LIBU */
#include	<prognamevar.hh>	/* locally supplied */
#include	<strabbrerr.h>		/* locally supplied */

#pragma		GCC dependency		"mod/libutil.ccm"

import libutil ;			/* |lenstr(3u)| */

int main(int argc,con mainv argv,con mainv envv) {
    	prognamevar	pn(argc,argv,envv) ;
	int		rs = SR_OK ;
	int		ex = EXIT_SUCCESS ;
	if (argc > 1) {
	    for (int ai = 1 ; (ai < argc) && argv[ai] ; ai += 1) {
		if (cchar *fn = argv[ai] ; fn[0]) {
		    rs = u_rmdirs(fn) ;
		}
		if (rs < 0) break ;
	    } /* end for */
	} /* end arguments */
	if ((ex == EXIT_SUCCESS) && (rs < 0)) {
	    cchar fmt[] = "%s: error ex=%d %s (%d)\n" ;
	    ex = EXIT_FAILURE ;
	    fprintf(stderr,fmt,ccp(pn),ex,strabbrerr(rs),rs) ;
	} /* end if (error) */
	return ex ;
} /* end subroutine (main) */


