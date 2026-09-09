/* endian_main SUPPORT (endian) */
/* charset=ISO8859-1 */
/* lang=C++23 */

/* print out the machine endianness */
/* version %I% last-modified %G% */


/* revision history:

	= 2001-04-11, David A­D­ Morano
	Originally written for Rightcore Network Services.

*/

/* Copyright © 2001 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

  	Description:
	This program prints out the endian configuation of the
	current machine architecture.

*******************************************************************************/

#include	<envstandards.h>	/* ordered first to configure */
#include	<unistd.h>		/* POSIX® */
#include	<cstddef>		/* CSTD */
#include	<cstdlib>		/* CSTD */
#include	<bit>			/* C++STD |endian(3c++)| */
#include	<iostream>		/* C++STD */
#include	<cstdio>		/* C++STD */
#include	<clanguage.h>		/* LIBU */
#include	<usysbase.h>		/* LIBU */
#include	<localmisc.h>		/* LIBU */


/* local defines */


/* namespaces */

using std::endian ;			/* enumeration */
using std::cout ;			/* variable */


/* typedefs */

typedef const char		cchar ;
typedef const char *const *	mainv ;


/* exported subroutines */


/* exported variables */


/* local structures */


/* local variables */


/* exported variables */


/* exported subroutines */

int main(int,mainv,mainv) {
	int	ex = 0 ;
	if constexpr (endian::native == endian::little) {
	    cout << "little" << eol ;
	} else if constexpr (endian::native == endian::big) {
	    cout << "big" << eol ;
	} else {
	    cout << "mixed" << eol ;
	}
	return ex ;
} /* end subroutine (main) */


