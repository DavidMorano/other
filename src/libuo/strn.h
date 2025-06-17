/* strn HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* perform variable counted c-string operations */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-11-01, David A­D­ Morano
	This subroutine was written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	STRN_INCLUDE
#define	STRN_INCLUDE


#include	<envstandards.h>	/* MUST be first to configure */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>

#include	<strnxcmp.h>		/* compare */
#include	<strnxchr.h>		/* find */
#include	<strnxpbrk.h>		/* find */
#include	<strnxsub.h>		/* find */
#include	<strnwht.h>		/* find */
#include	<strncpyxc.h>		/* copy */
#include	<strnwcpyxc.h>		/* copy (case changing) */


EXTERNC_begin

/* calculate string length */
extern int strnnlen(cchar *,int,int) noex ;
/* set */
extern char *strnset(char *dp,int ch,int n) noex ;
/* copy */
static inline char *strnwcpy(char *dp,int dl,cchar *sp,int sl) noex {
    	return strnwcpybc(dp,dl,sp,sl) ;
}
static inline char *strnblanks(char *dp,int n) noex {
	return strnset(dp,' ',n) ;
}
static inline char *strnncpy(char *dp,cchar *sp,int sl,int sz) noex {
    	return strnwcpy(dp,sz,sp,sl) ;
}

EXTERNC_end


#endif /* STRN_INCLUDE */


