/* samefile_main SUPPORT (samefile) */
/* encoding=ISO8859-1 */
/* lang=C++23 */

/* determine if two files are actually the same (but dev-ino comparison) */
/* version %I% last-modified %G% */


/* revision history:

	= 2001-04-11, David A­D­ Morano
	Originally written for Rightcore Network Services.

*/

/* Copyright © 2001 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

  	Names:
	samefile
	newerfile

	Description:
	This program determines if the two named files (given as
	two arguments) are the same file or not (based on equality
	of 'device' and 'inode' numbers.

*******************************************************************************/

#include	<envstandards.h>	/* must be ordered fist to configure */
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<unistd.h>
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>		/* |EXIT_SUCCESS| */
#include	<libgen.h>
#include	<iostream>
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>
#include	<matstr.h>
#include	<localmisc.h>


/* namespaces */

using std::cerr ;			/* variable */


/* typedefs */


/* external subroutines */


/* external variables */


/* local structures */


/* forward references */

static bool newer(ustat *sb1p,ustat *sb2p) noex {
    	bool f = false ;
	f = f || (sb1p->st_mtimespec.tv_sec > sb2p->st_mtimespec.tv_sec) ;
	f = f || (sb1p->st_mtimespec.tv_nsec > sb2p->st_mtimespec.tv_nsec) ;
	return f ;
}


/* local variables */

enum progmodes {
    	progmode_samefile,
    	progmode_newerfile,
	progmode_overlast
} ;

static constexpr cpcchar	prognames[] = {
    	[progmode_samefile]	= "samefile",
	[progmode_newerfile]	= "newerfile",
	[progmode_overlast]	= nullptr
} ;


/* exported variables */


/* exported subroutines */

int main(int argc,mainv argv,mainv) noexcept {
	int		ex = EXIT_FAILURE ;
	if (argc > 0) {
	    cchar	*pn = basename(const_cast<char *>(argv[0])) ;
	    if (cint pm = matstr(prognames,pn,-1) ; pm >= 0) {
	        if (argc == 3) {
	            cchar	*fn1 = argv[1] ;
	            cchar	*fn2 = argv[2] ;
		    if (fn1[0] && fn2[0]) {
		        ustat	sb1, sb2 ;
		        if (stat(fn1,&sb1) >= 0) {
		            if (stat(fn2,&sb2) >= 0) {
			        bool f = false ;
			        switch (pm) {
			        case progmode_samefile:
				    {
				        f = true ;
			                f = f && (sb1.st_dev == sb2.st_dev) ;
			                f = f && (sb1.st_ino == sb2.st_ino) ;
				    }
			            break ;
			        case progmode_newerfile:
				    f = newer(&sb1,&sb2) ;
			            break ;
			        } /* end switch */
			        if (f) ex = EXIT_SUCCESS ;
		            } else {
			        ex = EXIT_SUCCESS ;
		            }
		        } else {
	                    cerr << pn << ": cannot stat file (1)\n" ;
		        }
		    } else {
	                cerr << pn << ": one or more empty file-argument\n" ;
		    }
	        } else {
	            cerr << pn << ": not enough arguments\n" ;
	        }
	    } /* end if (matstr) */
	} /* end if */
	return ex ;
}
/* end subroutine (main) */


