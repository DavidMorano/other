/* utmp_support SUPPORT (macutmp) */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* support */
/* version %I% last-modified %G% */


/* revision history:

	= 2023-12-15, David A-D- Morano
	This is originally written.

*/

/* Copyright © 2023 David A-D- Morano.  All rights reserved. */
/* Use is subject to license terms. */

/*******************************************************************************

	Names:
	snadd
	snaddw

	Description:
	Support subroutines.

*******************************************************************************/

#include	<envstandards.h>	/* ordered first to configure */
#include	<sys/types.h>		/* POSSIX® |passwd| */
#include	<sys/stat.h>		/* POSIX® |USTAT| */
#include	<sys/mman.h>		/* POSIX® */
#include	<unistd.h>		/* POSIX® |ttyname_r(3c)| */
#include	<cstddef>		/* CSTD */
#include	<cstdlib>		/* CSTD |EXIT_SUCCESS| */
#include	<cstdio>		/* CSTD |printf(3c)| */
#include	<clanguage.h>		/* LIBU */
#include	<usysbase.h>		/* LIBU */
#include	<localmisc.h>		/* |TIMEBUFLEN| */

#pragma		GCC dependency		"mod/umisc.ccm"

import umisc ;				/* |snaddw(3u)| */

/* local defines */


/* imported namespaces */


/* local typedefs */


/* external subroutines */


/* external variables */


/* local structures */


/* forward references */


/* local variables */


/* exported variables */


/* exported subroutines */

    extern int mac_snaddw(char *dp,int dl,int i,cchar *sp,int sl) noex {
	return snaddw(dp,dl,i,sp,sl) ;
    }


