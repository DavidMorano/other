/* strcpyxc SUPPORT */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* copy a c-string to destination buffer w/ case conversions */
/* version %I% last-modified %G% */

#define	CF_STPCPY	1		/* use |stpcpy(3c)| */

/* revision history:

	= 1998-11-01, David A­D­ Morano
	This code was originally written.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Names:
	strcpybc
	strcpylc
	strcpyuc
	strcpyfc

	Description:
	These subroutines copy a source c-string to a (uncounted)
	destination while converting the case of the characters
	(according to which subroutine is called).

	Synopsis:
	rp = *strcpy{x}c(char *dst,cchar *src) noex

	Arguments:
	dst	destination result buffer (pointer)
	src	source c-string pointer
	rp	result pointer (pointing to resulting NUL character in 'dst'

	Returns:
	-	a pointer to the NUL character in the destination buffer

*******************************************************************************/

#include	<envstandards.h>	/* MUST be first to configure */
#include	<cstring>
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<toxc.h>
#include	<localmisc.h>

#include	"strcpyxc.h"


/* local defines */

#ifndef	CF_STPCPY
#define	CF_STPCPY	1		/* use |stpcpy(3c)| */
#endif


/* imported namespaces */


/* local typedefs */

extern "C" {
    typedef int (*toxc_f)(int) noex ;
}


/* external subroutines */


/* external variables */


/* local structures */


/* forward references */

static char *strcpyxc(toxc_f toxc,char *dp,cchar *sp) noex {
    	if (dp && sp) ylikely {
	    while (*sp) {
	        *dp++ = char(toxc(*sp++)) ;
	    } /* end while */
	    *dp = '\0' ;
	} /* end if (non-null) */
	return dp ;
}
/* end subroutine (strcpyxc) */


/* local variables */

constexpr bool		f_stpcpy = CF_STPCPY ;


/* exported variables */


/* exported subroutines */

char *strcpybc(char *dp,cchar *sp) noex {
	char		*rp = nullptr ;
	if (dp && sp) {
	    if_constexpr (f_stpcpy) {
	        rp = stpcpy(dp,sp) ;
	    } else {
	        rp = strcpyxc(tobc,dp,sp) ;
	    }
	} /* end if (non-null) */
	return rp ;
}

char *strcpylc(char *dp,cchar *sp) noex {
	return strcpyxc(tolc,dp,sp) ;
}

char *strcpyuc(char *dp,cchar *sp) noex {
	return strcpyxc(touc,dp,sp) ;
}

char *strcpyfc(char *dp,cchar *sp) noex {
	return strcpyxc(tofc,dp,sp) ;
}


