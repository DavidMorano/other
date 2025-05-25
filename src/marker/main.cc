/* main SUPPORT (marker) */
/* encoding=ISO8859-1 */
/* lang=C++23 */

/* undetermined */
/* version %I% last-modified %G% */


/* revision history:

	= 2001-04-11, David A­D­ Morano
	Originally written for Rightcore Network Services.

*/

/* Copyright © 2001 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Name:
	main (marker)

	Description:
	the purpose of this program is undermined.

	Synopsis:
	$ marker

	Arguments:
	-		none

	Returns:
	EXIT_SUCCESS	OK
	EXIT_FAILURE	some problem

*******************************************************************************/

#include	<sys/types.h>
#include	<sys/stat.h>
#include	<termios.h>
#include	<unistd.h>
#include	<fcntl.h>
#include	<cerrno>
#include	<ctime>
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>		/* |EXIT_{xx}| */
#include	<iostream>
#include	<iomanip>


/* local defines */

#ifndef	noex
#define	noex		noexcept
#endif

#ifndef	repeat
#define repeat		do
#define	until(cond)	while(!(cond))
#endif

#ifndef	SR_NOMEM
#define	SR_NOMEM	(-ENOMEM)
#endif

#ifndef	SR_INTR
#define	SR_INTR		(-EINTR)
#endif


/* imported namespaces */

using std::cout ;			/* variable */


/* local typedefs */

typedef const char *const *mainv ;


/* local structures */


/* forward references */


/* local constants */


/* exported variables */


/* exported subroutines */

int main(int,mainv,mainv) {
    	return EXIT_SUCCESS ;
}
/* end subroutine (main) */


