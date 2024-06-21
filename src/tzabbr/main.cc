/* main SUPPORT (tzabbr) */
/* lang=C++23 */

/* print out the abbreviation of the curreent time-zone */
/* version %I% last-modified %G% */


/* revision history:

	= 2001-04-11, David A­D­ Morano
	Originally written for Rightcore Network Services.

*/

/* Copyright © 2001 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Name:
	main (tzabbr)

	Description:
	This program prints out the abbreviation of the curreent
	time-zone.

	Synopsis:
	$ tzabbr

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
#include	<cstdlib>
#include	<iostream>
#include	<iostream>


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
	struct tm	tms ;
	struct tm	*tmp ;
	const time_t	dt = time(nullptr) ;
	int	ex = EXIT_FAILURE ;
	if ((tmp = localtime_r(&dt,&tms)) != nullptr) {
	    cout << tms.tm_zone << '\n' ;
	    ex = EXIT_SUCCESS ;
	} /* end if (localtime_t) */
	return ex ;
}
/* end subroutine (main) */


