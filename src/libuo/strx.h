/* strx HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* c-string comparisons */
/* version %I% last-modified %G% */


/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	STRX_INCLUDE
#define	STRX_INCLUDE


#include	<envstandards.h>	/* ordered first to configure */
#include	<string.h>		/* |strchr(3c)| + |strpbrk(3c)| */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>

#include	<strsub.h>
#include	<strxcmp.h>
#include	<strerrabbr.h>
#include	<strsigabbr.h>
#include	<straltwchar.h>


EXTERNC_begin

extern int	strwildsub(cchar *,cchar *) noex ;

extern char	*strobrk(cchar *,cchar *) noex ;
extern char	*strrbrk(cchar *,cchar *) noex ;

static inline char *strochr(cchar *sp,int sch) noex {
    	return strchr(sp,sch) ;
}

static inline char *strbrk(cchar *sp,cchar *ss) noex {
    	return strpbrk(sp,ss) ;
}

extern char	*strwhite(cchar *) noex ;

/* deprecated (can give erroneous result) */
[[deprecated("can give erroneous result")]] 
extern char *strbasename(cchar *) noex ;

#ifdef	COMMENT /* retired 2024-12-26 */
/* deprecated (writes to source string) */
[[deprecated("writes to source string")]] 
extern char *strdirname(char *) noex ; 
#endif /* COMMENT */

EXTERNC_end


#endif /* STRX_INCLUDE */


