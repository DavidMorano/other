/* servents_main (servents) */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* Server-Entries */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-04-13, David A-D- Morano
	Originally written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */
/* Use is subject to license terms. */

/******************************************************************************

	Name:
	servents

	Description:
	This program prints out all of the service entries in the
	system 'services' database.

	Synopsis:
	$ servents

	Returns:
	EXIT_SUCCESS		OK
	EXIT_FAILURE		error (of some kind)

******************************************************************************/

#include	<envstandards.h>	/* ordered first to configure */
#include	<sys/types.h>		/* POSIX® */
#include	<netdb.h>		/* POSIX® |ntohs(3c)| */
#include	<cstddef>		/* CSTD */
#include	<cstdlib>		/* CSTD */
#include	<cstdio>		/* CSTD */
#include	<clanguage.h>		/* LIBU */
#include	<usysbase.h>		/* LIBU */
#include	<localmisc.h>		/* LIBU */


/* local defines */


/* local namespaces */


/* local typedefs */


/* external subroutines */


/* external variables */


/* local structures */


/* forward references */

local int convhostport(int netport) noex ;


/* local variables */


/* exported variables */


/* exported subroutines */

int main(int,mainv,mainv) {
    	cnullptr	np{} ;
	int		ex = EXIT_SUCCESS ;
	int		rs = SR_OK ;
	setservent(1) ;
	for (servent *sep ; (sep = getservent()) != np ; ) {
	    cint	port = convhostport(sep->s_port) ;
	    cchar	*fmt = "%-24s %-8s %5u" ;
	    printf(fmt,sep->s_name,sep->s_proto,port) ;
	    if (sep->s_aliases) {
	        for (int i = 0 ; sep->s_aliases[i] ; i += 1) {
	            printf(" %s",sep->s_aliases[i]) ;
	        } /* end for */
	    } /* end if (aliases) */
	    printf("\n") ;
	} /* end for */
	endservent() ;
	if ((ex == EXIT_SUCCESS) && (rs < 0)) {
	    ex = EXIT_FAILURE ;
	}
	return ex ;
} /* end subroutine (main) */


/* local subroutines */

local int convhostport(int netport) noex {
    	ushort uport = conv<ushort>(netport) ;
	int		port{} ;
	{
	    port = conv<int>(ntohs(uport)) ;
	}
	return port ;
} /* end subroutine */


