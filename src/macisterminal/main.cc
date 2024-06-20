/* main SUPPORT (isterminal) */
/* lang=C++20 */

/* whole program for ISTERMINAL */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-11-01, David Morano
	Originally written for RightCore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#include	<envstandards.h>	/* ordered first to configure */
#include	<unistd.h>
#include	<cstdlib>
#include	<iostream>
#include	<utypedefs.h>
#include	<clanguage.h>
#include	<exitcodes.h>


/* local defines */


/* imported namespaces */


/* external subroutines */


/* exported subroutines */

int main(int,mainv,mainv) {
	int		ex = 0 ;
	cbool		f = (isatty(0) > 0) ;
	ex = (f) ? EXIT_SUCCESS : EXIT_FAILURE ;
	return ex ;
}
/* end subroutine (main) */


