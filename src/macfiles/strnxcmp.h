/* strnxcmp HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* find a character in a counted c-string */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-09-10, David A­D­ Morano
	This subroutine was written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	STRNXCMP_INCLUDE
#define	STRNXCMP_INCLUDE


#include	<envstandards.h>	/* MUST be first to configure */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>

#include	<strnxxxxcmp.h>
#include	<strnkeycmp.h>


EXTERNC_begin

extern int strnvalcmp	(cchar *,cchar *,int) noex ;
extern int strnleadcmp	(cchar *,cchar *,int) noex ;
extern int strnncmp	(cchar *,int,cchar *,int) noex ;
extern int strnndictcmp	(cchar *,int,cchar *,int) noex ;

EXTERNC_end


#endif /* STRNXCMP_INCLUDE */


