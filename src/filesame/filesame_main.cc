/* filesame_main SUPPORT (filesame) */
/* charset=ISO8859-1 */
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
	filesame
	filenewer

	Description:
	This program determines if the two named files (given as
	two arguments) are the same file or not (based on equality
	of 'device' and 'inode' numbers.

*******************************************************************************/

#include	<envstandards.h>	/* must be ordered first to configure */
#include	<sys/types.h>		/* POSIX® */
#include	<sys/stat.h>		/* POSIX® */
#include	<unistd.h>		/* POSIX® */
#include	<cstddef>		/* CSTD */
#include	<cstdlib>		/* CSTD */
#include	<libgen.h>
#include	<iostream>		/* C++STD */
#include	<clanguage.h>		/* LIBU */
#include	<usysbase.h>		/* LIBU */
#include	<usupport.h>		/* LIBU |matstr(3u)| */
#include	<prognamevar.hh>	/* LIBUC but **local** */
#include	<localmisc.h>		/* LIBU */


/* local defines */


/* namespaces */

using std::cerr ;			/* variable */
using libu::matstr ;			/* subroutine */


/* typedefs */


/* external subroutines */


/* external variables */


/* local structures */


/* forward references */

local bool newer(ustat *sb1p,ustat *sb2p) noex {
    	bool f = false ;
	f = f || (sb1p->st_mtimespec.tv_sec > sb2p->st_mtimespec.tv_sec) ;
	f = f || (sb1p->st_mtimespec.tv_nsec > sb2p->st_mtimespec.tv_nsec) ;
	return f ;
} /* end subroutine (newer) */


/* local variables */

enum progmodes {
    	progmode_filesame,
    	progmode_filesametime,
    	progmode_filenewer,
	progmode_overlast
} ; /* end enum */

constexpr cpcchar	prognames[] = {
    	[progmode_filesame]	= "filesame",
    	[progmode_filesametime]	= "filesametime",
	[progmode_filenewer]	= "filenewer",
	[progmode_overlast]	= nullptr
} ; /* end array (prognames) */


/* exported variables */


/* exported subroutines */

int main(int argc,con mainv argv,con mainv envv) {
    	prognamevar	progname(argc,argv,envv) ;
	int		ex = EXIT_FAILURE ;
	if (argc > 0) {
	    cchar	*pn = progname ;
	    if (cint pm = matstr(prognames,pn,-1) ; pm >= 0) {
	        if (argc == 3) {
	            cchar	*fn1 = argv[1] ;
	            cchar	*fn2 = argv[2] ;
		    if (fn1[0] && fn2[0]) {
		        if (ustat sb1 ; statfile(fn1,&sb1) >= 0) {
		            if (ustat sb2 ; statfile(fn2,&sb2) >= 0) {
			        bool f = false ;
			        switch (pm) {
			        case progmode_filesame:
				    {
				        f = true ;
			                f = f && (sb1.st_dev == sb2.st_dev) ;
			                f = f && (sb1.st_ino == sb2.st_ino) ;
				    }
			            break ;
			        case progmode_filesametime:
			            f = (sb1.st_mtime == sb2.st_mtime) ;
				    break ;
			        case progmode_filenewer:
				    f = newer(&sb1,&sb2) ;
			            break ;
			        } /* end switch */
			        if (f) ex = EXIT_SUCCESS ;
		            } else {
			        ex = EXIT_SUCCESS ;
		            } /* end */
		        } else {
	                    cerr << pn << ": cannot stat file (1)\n" ;
		        } /* end */
		    } else {
	                cerr << pn << ": one or more empty file-argument\n" ;
		    } /* end */
	        } else {
	            cerr << pn << ": not enough arguments\n" ;
	        } /* end */
	    } /* end if (matstr) */
	} /* end if */
	return ex ;
} /* end subroutine (main) */


