/* snaddw HEADER */
/* lang=C20 */

/* concatenate c-string to existing counted c-string */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-11-01, David A­D­ Morano
	This subroutine was originally written.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

/******************************************************************************

	Ths object is used to concatenate a new c-string to
	an existing counted c-string.

******************************************************************************/

#ifndef	SNADDW_INCLUDE
#define	SNADDW_INCLUDE


#include	<envstandards.h>	/* MUST be first to configure */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>


EXTERNC_begin

extern int snaddw(char *,int,int,cchar *,int) noex ;

EXTERNC_end

#ifdef	__cplusplus

static inline int snaddw(char *dbuf,int dlen,int dl,cchar *sp) noex {
	return snaddw(dbuf,dlen,dl,sp,-1) ;
}

#endif /* __cplusplus */


#endif /* SNADDW_INCLUDE */


