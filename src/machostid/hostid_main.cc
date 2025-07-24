/* main SUPPORT */
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

  	Description:
	Get and print out the host-ID from the system.

***************************************************************************/

#include	<envstandards.h>	/* ordered first to configure */
#include	<unistd.h>		/* |gethostid(3c)| */
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<cstdio>		/* |printf(3c++)| */
#include	<iostream>		/* |cout(3c++)| */
#include	<iomanip>		/* |hex(3c++)| + |setw(3c++)| */
#include	<usystem.h>
#include	<localmisc.h>		/* |eol(3dam)| */


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


