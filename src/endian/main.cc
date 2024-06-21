/* main SUPPORT (endian) */
/* lang=C++23 */

/* print out the machine endianness */
/* version %I% last-modified %G% */


/* revision history:

	= 2001-04-11, David A­D­ Morano
	Originally written for Rightcore Network Services.

*/

/* Copyright © 2001 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	This program prints out a table of the ASCII control characters.

*******************************************************************************/

#include	<envstandards.h>
#include	<unistd.h>
#include	<cstddef>		/* |nullptr_t| */
#include	<bit>			/* |endian(3c++)| */
#include	<iostream>
#include	<cstdio>


/* local defines */

#ifndef	eol
#define	eol		'\n'
#endif


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

int main(int,mainv,mainv) noexcept {
	int	ex = 0 ;
	if constexpr (endian::native == endian::little) {
	    cout << "little" << eol ;
	} else if constexpr (endian::native == endian::big) {
	    cout << "big" << eol ;
	} else {
	    cout << "mixed" << eol ;
	}
	return ex ;
}
/* end subroutine (main) */


