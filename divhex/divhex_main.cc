/* divhex_main SUPPORT */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* test integer operations */
/* version %I% last-modified %G% */

#define	CF_DEBUG	0		/* debugging */

/* revision history:

	= 1998-04-13, David A-D- Morano
	Originally written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */
/* Use is subject to license terms. */

/*******************************************************************************

  	Name:
	dighex

  	Description:
  	Test the integer promotion.

*******************************************************************************/

#include	<envstandards.h>	/* ordered first to configure */
#include	<cerrno>
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>		/* |getenv(3c)| */
#include	<cstdio>
#include	<bit>			/* |popcount(3c++)| */
#include	<iostream>
#include	<clanguage.h>
#include	<usysbase.h>
#include	<localmisc.h>
#include	<dprint.hh>		/* debugging */


#define	BASE	16

#ifndef	CF_DEBUG
#define	CF_DEBUG	0		/* debugging */
#endif

constexpr bool		f_debug = CF_DEBUG ;


int main(int argc,mainv argv,mainv) {
    	cint		b = BASE ;
    	int		rs = SR_OK ;
	int		ex = EXIT_SUCCESS ;
	DPRINTF("ent\n") ;
	if ((argc > 1) && argv[1]) {
	    cchar *divstr = argv[1] ;
	    char *end{} ;
	    errno = 0 ;
	    DPRINTF("divstr=%s\n",divstr) ;
	    cuint divisor = strtoui(divstr,&end,b) ;
	    DPRINTF("errno=%d\n",errno) ;
	    if (! errno) {
	        for (int ai = 2 ; ai < argc ; ai += 1) {
	            if (cchar *arg = argv[ai] ; arg[0]) {
    	                uint	q ;
    	                uint	r ;
		        errno = 0 ;
		        cuint	a = strtoui(arg,&end,b) ;
		        if (! errno) {
			    q = a / divisor ;
			    r = a % divisor ;
	                    printf("%08X - %08X %08X\n",a,q,r) ;
		        } else {
	                    fprintf(stderr,"error\n") ;
			    rs = SR_INVALID ;
		        }
	            } /* end if (valid) */
		    if (rs < 0) break ;
	        } /* end for */
	    } else {
		fprintf(stderr,"error\n") ;
		rs = SR_INVALID ;
	    }
	} /* end if (divisor) */
	if ((ex == EXIT_SUCCESS) && (rs < 0)) {
	    ex = EXIT_FAILURE ;
	}
	DPRINTF("ent ex=%u rs=%d\n",ex,rs) ;
	return ex ;
} /* end subroutine (main) */


