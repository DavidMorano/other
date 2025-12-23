/* six HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* string-index operations */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-11-01, David A­D­ Morano
	This subroutine was written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	SIX_INCLUDE
#define	SIX_INCLUDE


#include	<envstandards.h>	/* ordered first to configure */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>

#include	<siext.h>
#include	<sifext.h>
#include	<sixchr.h>
#include	<sixbrk.h>
#include	<sixterm.h>
#include	<sixsub.h>
#include	<siwht.h>
#include	<sicasechr.h>
#include	<sinon.h>
#include	<sichar.h>
#include	<sispan.h>
#include	<sileadchr.h>
#include	<sileader.h>
#include	<sileadzero.h>


EXTERNC_begin

extern int sidquote(cchar *,int) noex ;
extern int sibasename(cchar *,int) noex ;
extern int sinext(cchar *,int) noex ;
extern int silbrace(cchar *,int) noex ;
extern int sihyphen(cchar *,int) noex ;
extern int sicite(cchar *,int,cchar *,int) noex ;

static inline int sichr(cchar *sp,int sl,int sch) noex {
	return siochr(sp,sl,sch) ;
}
static inline int sibrk(cchar *sp,int sl,cchar *ss) noex {
    	return siobrk(sp,sl,ss) ;
}
static inline int sibreak(cchar *sp,int sl,cchar *ss) noex {
	return sibrk(sp,sl,ss) ;
}
static inline int sisub(cchar *sp,int sl,cchar *ss) noex {
	return sibasesub(sp,sl,ss) ;
}
static inline int siterm(cchar *sp,int sl,cchar *ss) noex {
    	return sioterm(sp,sl,ss) ;
}

EXTERNC_end


#endif /* SIX_INCLUDE */


