/* mactruncate_main SUPPORT */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* truncate a file */
/* version %I% last-modified %G% */


/* Copyright © 2017 David A­D­ Morano.  All rights reserved. */
/* Use is subject to license terms. */

/*******************************************************************************

	Name:
	mactruncate

	Description:
	This little program will truncate files to a fixed length.

	Synopsis:
	$ mactruncate <file> <length>

	Arguments:
	<file>		file to truncate
	<length>	decimal length to truncate

*******************************************************************************/

#include	<envstandards.h>	/* ordered first to configure */
#include	<sys/types.h>		/* POSIX® */
#include	<unistd.h>		/* POSIX® */
#include	<fcntl.h>		/* POSIX® */
#include	<cstddef>		/* CSTD */
#include	<cstdlib>		/* CSTD */
#include	<clanguage.h>		/* LIBU */
#include	<usysbase.h>		/* LIBU */
#include	<localmisc.h>		/* LIBU */


/* external subroutines */


/* external variables */


/* forward references */


/* local variables */


/* exported variables */


/* exported subroutines */

int main(int argc,const char *const *argv,const char *const *envv) {
	int		ex = EXIT_SUCCESS ;
	if (argc >= 3) {
	    if (unsigned long sz ; (sz = atol(argv[2])) >= 0) {
		cchar	*fn = argv[1] ;
		truncate(fn,(off_t) sz) ;
	    } /* end if (atol) */
	} /* end if (valid) */
	if ((ex == EXIT_SUCCESS) && (rs < 0)) {
	    ex = EXIT_FAILURE ;
	} /* end if (error) */
	return ex ;
} /* end subroutine (main) */


