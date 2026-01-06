/* ucids HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* LIBUC identification management operations */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-11-01, David A­D­ Morano
	This subroutine was written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	UCIDS_INCLUDE
#define	UCIDS_INCLUDE


#include	<envstandards.h>	/* ordered first to configure */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>


EXTERNC_begin

extern int	uc_inproj(cchar *,cchar *,char *,int) noex ;

EXTERNC_end


#endif /* UCIDS_INCLUDE */


