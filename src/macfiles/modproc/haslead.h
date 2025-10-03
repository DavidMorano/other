/* haslead HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* does the c-string contain a leading pound character (after skipping WS) */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-11-01, David A­D­ Morano
	This subroutine was written for Rightcore Network Services.

*/

/* Copyright (c) 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	HASLEAD_INCLUDE
#define	HASLEAD_INCLUDE


#include	<envstandards.h>	/* MUST be first to configure */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>

EXTERNC_begin

extern bool	haslead(cchar *,int,int) noex ;

static inline bool hasleadpound(cchar *sp,int sl) noex {
	return haslead(sp,sl,'#') ;
}

EXTERNC_end


#endif /* HASLEAD_INCLUDE */


