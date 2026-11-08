/* strbcpy HEADER (String-Buffer-Copy) */
/* charset=ISO8859-1 */
/* lang=C20 */

/* string-copy variant */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-11-01, David A­D­ Morano
	This subroutine was written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	STRBCPY_INCLUDE
#define	STRBCPY_INCLUDE


#include	<envstandards.h>	/* ordered first to configure */
#include	<clanguage.h>		/* LIBU */
#include	<utypedefs.h>		/* LIBU */
#include	<utypealiases.h>	/* LIBU */
#include	<usysdefs.h>		/* LIBU */

#include	<strbcpyx.h>
#include	<strbcpyxw.h>
#include	<strbcpyxc.h>


#ifdef	__cplusplus

inline char *strbcpy(char *dp,int dl,
		cc *s1,int len) noex {
	return strbcpyxw(dp,dl,1,s1,len) ;
} /* end */
inline char *strbcpy(char *dp,int dl,
		cc *s1,cc *s2,int len) noex {
	return strbcpyxw(dp,dl,2,s1,s2,len) ;
} /* end */
inline char *strbcpy(char *dp,int dl,
		cc *s1,cc *s2,cc *s3,int len) noex {
	return strbcpyxw(dp,dl,3,s1,s2,s3,len) ;
} /* end */
inline char *strbcpy(char *dp,int dl,
		cc *s1,cc *s2,cc *s3,cc *s4,int len) noex {
	return strbcpyxw(dp,dl,4,s1,s2,s3,s4,len) ;
} /* end */
inline char *strbcpy(char *dp,int dl,
		cc *s1,cc *s2,cc *s3,cc *s4,cc *s5,int len) noex {
	return strbcpyxw(dp,dl,5,s1,s2,s3,s4,s5,len) ;
} /* end */
inline char *strbcpy(char *dp,int dl,
		cc *s1,cc *s2,cc *s3,cc *s4,cc *s5,cc *s6,int len) noex {
	return strbcpyxw(dp,dl,6,s1,s2,s3,s4,s5,s6,len) ;
} /* end */

#endif /* __cplusplus */


#endif /* STRBCPY_INCLUDE */


