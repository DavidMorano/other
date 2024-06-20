/* main SUPPORT */
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

#include	<sys/types.h>
#include	<unistd.h>
#include	<fcntl.h>
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>


/* external subroutines */


/* external variables */


/* forward references */


/* local variables */


/* exported variables */


/* exported subroutines */

int main(int argc,const char *const *argv,const char *const *envv) {
	int		ex = 1 ;
	if (argc >= 3) {
	    if (unsigned long sz ; (sz = atol(argv[2])) >= 0) {
		const char	*fn = argv[1] ;
		truncate(fn,(off_t) sz) ;
	    } /* end if (atol) */
	} /* end if (valid) */
	return ex ;
}
/* end subroutine (main) */


