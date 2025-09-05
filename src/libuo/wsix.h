/* wsix HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* wide-c-string find-index operations */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-11-01, David A­D­ Morano
	This subroutine was written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	WSIX_INCLUDE
#define	WSIX_INCLUDE


#include	<envstandards.h>	/* ordered first to configure */
#include	<stddef.h>		/* |wchar_t| */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>


EXTERNC_begin

extern int wsiochr(const wchar_t *,int,int) noex ;
extern int wsirchr(const wchar_t *,int,int) noex ;
extern int wsinul(const wchar_t *) noex ;

static inline int wsichr(const wchar_t *wsp,int wsl,int sch) noex {
	return wsiochr(wsp,wsl,sch) ;
}

EXTERNC_end


#endif /* WSIX_INCLUDE */


