/* ucttyname HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* LIBUC Teermina-Control functions */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-04-10, David A­D­ Morano
	This subroutine was written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	UCTTYNAME_INCLUDE
#define	UCTTYNAME_INCLUDE


#include	<envstandards.h>	/* MUST be first to configure */
#include	<clanguage.h>		/* LIBU */
#include	<usysbase.h>		/* LIBU */


EXTERNC_begin

extern int uc_ttyname(int,char *,int) noex ;

EXTERNC_end


#endif /* UCTTYNAME_INCLUDE */


