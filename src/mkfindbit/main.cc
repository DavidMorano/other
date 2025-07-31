/* main SUPPORT (mkfindbit) */
/* lang=C++20 */

/* the the data arrays for finding bits (first and last) in integers */
/* version %I% last-modified %G% */



/* revision history:

	= 1998-06-29, David A­D­ Morano
	This subroutine was written for Rightcore Network Services
	(RNS).

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	This little program creates the lookup tables for the
	Find-First-Bit-Set (ffbs) and the Find-Last-Bit-Set (flbs)
	subroutines. Other subroutines in this family have to
	calculate the result the "hard way."

*******************************************************************************/

#include	<envstandards.h>	/* MUST be ordered first to configure */
#include	<sys/types.h>
#include	<sys/param.h>
#include	<climits>
#include	<cstdio>
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<ascii.h>


/* local defines */

#define	MOD	8


/* forward references */

static int	mktabfbs() noex ;
static int	mktablbs() noex ;

static int	ffbsb(int) noex ;
static int	flbsb(int) noex ;


/* exported variables */


/* exported subroutines */

int main(int,mainv,mainv) {
	mktabfbs() ;
	mktablbs() ;
}
/* end subroutine (main) */


/* local subroutines */

static int mktabfbs() noex {
	int		i, j ;
	int		n ;
	int		v ;
	const char	*fmt ;

	fprintf(stdout,"\n") ;

	fprintf(stdout,
	    "const unsigned char	fbstab[] = {\n") ;

	j = 0 ;
	for (i = 0 ; i < 256 ; i += 2) {

	    v = 0 ;

	    n = ffbsb(i) ;
	    if (n < 0) n = 15 ;
	    v = v | (n<<0) ;

	    n = ffbsb(i+1) ;
	    if (n < 0) n = 15 ;
	    v = v | (n<<4) ;

		if ((j % MOD) == 0) {
			fmt = "\t0x%02x," ;
		} else if ((j % MOD) == (MOD - 1)) {
			fmt = " 0x%02x,\n" ;
		} else {
			fmt = " 0x%02x," ;
		}

	        fprintf(stdout,fmt,v) ;

		j += 1 ;
	} /* end for */

	fprintf(stdout, "} ;\n") ;

	fprintf(stdout,"\n") ;

	return 0 ;
}
/* end subroutine (mktabfbs) */

static int mktablbs() noex {
	int		i, j ;
	int		n ;
	int		v ;
	const char	*fmt ;

	fprintf(stdout,"\n") ;

	fprintf(stdout,
	    "const unsigned char	lbstab[] = {\n") ;

	j = 0 ;
	for (i = 0 ; i < 256 ; i += 2) {

	    v = 0 ;

	    n = flbsb(i) ;
	    if (n < 0) n = 15 ;
	    v = v | (n<<0) ;

	    n = flbsb(i+1) ;
	    if (n < 0) n = 15 ;
	    v = v | (n<<4) ;

		if ((j % MOD) == 0) {
			fmt = "\t0x%02x," ;
		} else if ((j % MOD) == (MOD - 1)) {
			fmt = " 0x%02x,\n" ;
		} else {
			fmt = " 0x%02x," ;
		}

	        fprintf(stdout,fmt,v) ;

		j += 1 ;
	} /* end for */

	fprintf(stdout, "} ;\n") ;

	fprintf(stdout,"\n") ;

	return 0 ;
}
/* end subroutine (mktablbs) */

static int ffbsb(int v) noex {
	int		i = -1 ;
	if (v) {
	    for (i = 0 ; i < 8 ; i += 1) {
	        if (v&1) break ;
	        v >>= 1 ;
	    }
	}
	return i ;
}
/* end subroutine (ffbsb) */

static int flbsb(int v) noex {
	int		i ;
	for (i = 7 ; i >= 0 ; i -= 1) {
	    if ((v>>i) & 1) break ;
	}
	return i ;
}
/* end subroutine (flbsb) */


