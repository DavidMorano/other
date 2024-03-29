/* main SUPPORT */
/* lang=C++23 */

/* determine if two files are actually the same (but dev-ino comparison) */
/* version %I% last-modified %G% */


/* revision history:

	= 2001-04-11, David A�D� Morano
	Originally written for Rightcore Network Services.

*/

/* Copyright � 2001 David A�D� Morano.  All rights reserved. */

/*******************************************************************************

	This program determines if the two named files (given as
	two arguments) are the same file or not (based on equality
	of 'device' and 'inode' numbers.

*******************************************************************************/

#include	<sys/types.h>
#include	<sys/stat.h>
#include	<unistd.h>
#include	<cstdlib>		/* |EXIT_SUCCESS| */
#include	<libgen.h>
#include	<iostream>


/* namespaces */

using namespace		std ;

/* typedefs */

typedef const char *const *	mainv ;
typedef const char		cchar ;
typedef struct stat		statblock ;

/* exported subroutines */

int main(int argc,mainv argv,mainv) noexcept {
	int		ex = EXIT_FAILURE ;
	if (argc > 0) {
	    cchar	*pn = basename(const_cast<char *>(argv[0])) ;
	    if ((argc == 3) && (argv != nullptr)) {
		statblock	sb1, sb2 ;
	        cchar		*fn1 = argv[1] ;
	        cchar		*fn2 = argv[2] ;
		if (stat(fn1,&sb1) >= 0) {
		    if (stat(fn2,&sb2) >= 0) {
			bool	f = (sb1.st_dev == sb2.st_dev) ;
			f = f && (sb1.st_ino == sb2.st_ino) ;
			if (f) ex = EXIT_SUCCESS ;
		    } else {
	                cerr << pn << ": cannot stat file (2)\n" ;
		    }
		} else {
	            cerr << pn << ": cannot stat file (1)\n" ;
		}
	    } else {
	        cerr << pn << ": not enough arguments\n" ;
	    }
	} /* end if */
	return ex ;
}
/* end subroutine (main) */


