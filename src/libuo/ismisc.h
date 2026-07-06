/* ismisc HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* is something something? */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-04-10, David A­D­ Morano
	This subroutine was written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	ISMISC_INCLUDE
#define	ISMISC_INCLUDE


#include	<envstandards.h>	/* MUST be first to configure */
#include	<clanguage.h>		/* LIBU */
#include	<utypedefs.h>		/* LIBU */
#include	<utypealiases.h>	/* LIBU */
#include	<usysdefs.h>		/* LIBU */

#include	<isaccmode.h>		/* LIBUC */


EXTERNC_begin

extern bool	isprime		(uint) noex ;
extern bool	isleapyear	(int) noex ;
extern bool	isdotdir	(cchar *) noex ;
extern bool	isobjspecial	(void *) noex ;
extern bool	issamehostname	(cchar *,cchar *,cchar *) noex ;
extern bool	isfnamespecial	(cchar *,int) noex ;

local inline bool isDotDir(cchar *dp) noex {
	return isdotdir(dp) ;
}
local inline bool isSpecialObject(void *op) noex {
	return isobjspecial(op) ;
}

EXTERNC_end


#endif /* ISMISC_INCLUDE */


