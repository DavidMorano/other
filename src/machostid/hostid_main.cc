/* hostid_main SUPPORT (machostid) */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* get the various IDs from the system */
/* version %I% last-modified %G% */


/* revision history:

	= 1988-01-10, David A­D­ Morano
	This subroutine was written (originally) as a test of the
	Sun Solaris® UNIX® 'kstat' facility.  But now it just prints
	the machine ID.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

/************************************************************************

  	Name:
	hostid_main

  	Description:
	Get and print out the host-ID from the system.

***************************************************************************/

#include	<envstandards.h>	/* ordered first to configure */
#include	<unistd.h>		/* POSIX |gethostid(3c)| */
#include	<cstddef>		/* CSTD */
#include	<cstdlib>		/* CSTD */
#include	<cstdio>		/* CSTD |printf(3c++)| */
#include	<iostream>		/* C++STD |cout(3c++)| */
#include	<iomanip>		/* C++STD |hex(3c++)| + |setw(3c++)| */
#include	<clanguage.h>		/* LIBU */
#include	<usysbase.h>		/* LIBU */
#include	<localmisc.h>		/* LIBU |eol(3u)| */


/* local defines */


/* imported namespaces */

using std::setw ;			/* subroutine */
using std::hex ;			/* subroutine? */
using std::printf ;			/* subroutine */
using std::cout ;			/* variable */


/* local typedefs */


/* external subroutines */


/* external variables */


/* local structures */


/* forward references */


/* local variables */


/* exported variables */


/* exported subroutines */

int main(int,mainv,mainv) {
	ulong	id = gethostid() ;
	printf("%08lX\n",id) ;
}
/* end subroutine (main) */


