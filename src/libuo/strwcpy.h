/* strwcpy HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* string-copy variant */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-11-01, David A­D­ Morano
	This subroutine was written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	STRWCPY_INCLUDE
#define	STRWCPY_INCLUDE


#include	<envstandards.h>	/* MUST be first to configure */
#include	<string.h>		/* |stpcpy(3c)| */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>

#include	<strwcpyx.h>


#ifdef	__cplusplus

constexpr inline char *strwcpy(char *dp,cchar *sp,int sl = -1) noex {
	if (sl >= 0) {
	    while (sl-- && *sp) *dp++ = *sp++ ;
	    *dp = '\0' ;
	} else {
	    dp = stpcpy(dp,sp) ;
	} /* end if */
	return dp ;
} /* end subroutine (strwcpy) */

#else /* __cplusplus */

static inline char *strwcpy(char *dp,cchar *sp,int sl) noex {
	return strwcpybc(dp,sp,sl) ;
}

#endif /* __cplusplus */


#endif /* STRWCPY_INCLUDE */


