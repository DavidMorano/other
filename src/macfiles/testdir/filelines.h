/* fileines HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* calculate the number of lines in a file */
/* version %I% last-modified %G% */


/* revision history:

	= 2023-12-17, David A­D­ Morano
	Originally written for Rightcore Network Services.

*/

/* Copyright © 2023 David A­D­ Morano.  All rights reserved. */

#ifndef	FILELINES_INCLUDE
#define	FILELINES_INCLUDE


#include	<envstandards.h>	/* MUST be first to configure */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>


EXTERNC_begin

extern int	filelines(cchar *) noex ;

EXTERNC_end


#endif /* FILELINES_INCLUDE */


