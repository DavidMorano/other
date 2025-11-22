/* gethostid SUPPORT */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* get the host-ID value (a 32-bit binary number) */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-04-13, David A-D- Morano
	Originally written for Rightcore Network Services.

*/

/* Copyright © 2017 David A­D­ Morano.  All rights reserved. */
/* Use is subject to license terms. */

/*******************************************************************************

  	Name:
	getostid

	Description:
	Retrieve the host ID value (a 32-bit unsigned binary integer
	value).

*******************************************************************************/

#include	<envstandards.h>	/* ordered first to configure */
#include	<sys/types.h>		/* required by |sysctl(3)| */
#include	<sys/sysctl.h>		/* required by |sysctl(3)| */
#include	<cerrno>
#include	<cstddef>		/* <- |nullptr_t| ?? */
#include	<cstdint>		/* <- |int32_t| */
#include	<cstdio>
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>
#include	<usyscalls.h>
#include	<localmisc.h>


/* local defines */


/* imported namespaces */


/* exported variables */


/* exported subroutines */

long gethostid() noex {
	cnullptr	np{} ;
	size_t		idlen = sizeof(int32_t) ;
	int32_t		id{} ;
	if (sysctlbyname("kern.hostid",&id,&idlen,np,0uz) < 0) {
	    fprintf(stderr,"errno=%d id=%08x\n",errno,id) ;
	    id = 0 ;
	}
	return long(id) ;
}
/* end subroutine (gethostid) */


