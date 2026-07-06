/* mkui HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* string-index operations */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-11-01, David A­D­ Morano
	This subroutine was written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	MKUI_INCLUDE
#define	MKUI_INCLUDE


#include	<envstandards.h>	/* ordered first to configure */
#include	<clanguage.h>		/* LINU */
#include	<usysbase.h>		/* LINU */


EXTERNC_begin

extern int	mkuibang(char *,int,userinfo *) noex ;
extern int	mkuiname(char *,int,userinfo *) noex ;

EXTERNC_end


#endif /* MKUI_INCLUDE */


