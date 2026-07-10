/* ucinetconv HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* UNIX® UCINETCONV support */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-04-13, David A­D­ Morano
	This module was originally written.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

  	Group:
	ucinetconv

	Description:
	This file contains some support for the UNIX® UCINETCONV
	related operations.

*******************************************************************************/

#ifndef	UCINETCONV_INCLUDE
#define	UCINETCONV_INCLUDE


#include	<envstandards.h>	/* ordered first to configure */
#include	<clanguage.h>		/* LIBU */
#include	<usysbase.h>		/* LIBU */


EXTERNC_begin

extern int uc_inetnetpton	(void *,int,int,cchar *) noex ;
extern int uc_inetpton		(void *,int,cchar *) noex ;
extern int uc_inetntop		(char *,int,int,cvoid *) noex ;

EXTERNC_end


#endif /* UCINETCONV_INCLUDE */


