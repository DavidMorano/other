/* strdcpy HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* string-copy variant */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-11-01, David A­D­ Morano
	This subroutine was written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	STRDCPY_INCLUDE
#define	STRDCPY_INCLUDE


#include	<envstandards.h>	/* ordered first to configure */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>

#include	<strdcpyx.h>
#include	<strdcpyxw.h>


EXTERNC_begin

extern char	*strdcpycompact(char *,int,cc *,int) noex ;
extern char	*strdcpyopaque(char *,int,cc *,int) noex ;
extern char	*strdcpyclean(char *,int,int,cchar *,int) noex ;

EXTERNC_end

#ifdef	__cplusplus

static inline char *strdcpy(char *dp,int dl,cc *s1,int sl) noex {
	return strdcpyxw(dp,dl,1,s1,sl) ;
}
static inline char *strdcpy(char *dp,int dl,cc *s1,cc *s2,int sl) noex {
	return strdcpyxw(dp,dl,2,s1,s2,sl) ;
}
static inline char *strdcpy(char *dp,int dl,cc *s1,cc *s2,cc *s3,int sl) noex {
	return strdcpyxw(dp,dl,3,s1,s2,s3,sl) ;
}
static inline char *strdcpy(char *dp,int dl,cc *s1,cc *s2,cc *s3,
		cc *s4,int sl) noex {
	return strdcpyxw(dp,dl,4,s1,s2,s3,s4,sl) ;
}
static inline char *strdcpy(char *dp,int dl,cc *s1,cc *s2,cc *s3,cc *s4,
		cc *s5,int sl) noex {
	return strdcpyxw(dp,dl,5,s1,s2,s3,s4,s5,sl) ;
}
static inline char *strdcpy(char *dp,int dl,cc *s1,cc *s2,cc *s3,cc *s4,
		cc *s5,cc *s6,int sl) noex {
	return strdcpyxw(dp,dl,6,s1,s2,s3,s4,s5,s6,sl) ;
}

#endif /* __cplusplus */


#endif /* STRDCPY_INCLUDE */


