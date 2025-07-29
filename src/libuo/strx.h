/* strx HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* c-string comparisons */
/* version %I% last-modified %G% */


/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	STRX_INCLUDE
#define	STRX_INCLUDE


#include	<envstandards.h>	/* ordered first to configure */
#include	<stddef.h>
#include	<stdlib.h>
#include	<string.h>		/* |strchr(3c)| + |strpbrk(3c)| */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>

#include	<strsub.h>
#include	<strxbrk.h>
#include	<strxcmp.h>
#include	<strerrabbr.h>
#include	<strsigabbr.h>
#include	<straltwchar.h>
#include	<strcpyxc.h>
#include	<strlinelen.h>
#include	<strlocktype.h>


EXTERNC_begin

extern int	strwildsub(cchar *,cchar *) noex ;

static inline char *strochr(cchar *sp,int sch) noex {
    	return strchr(sp,sch) ;
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


#ifndef	DECLARATION_STRBRK
#define	DECLARATION_STRBRK
EXTERNC_begin
static inline char *strbrk(cchar *sp,cchar *ss) noex {
    	return strpbrk(sp,ss) ;
}
EXTERNC_end
#endif /* DECLARATION_STRBRK */


#endif /* STRX_INCLUDE */


