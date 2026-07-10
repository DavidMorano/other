/* files_show HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* calculate the number of lines in a file */
/* version %I% last-modified %G% */


/* revision history:

	= 2023-12-17, David A­D­ Morano
	Originally written for Rightcore Network Services.

*/

/* Copyright © 2023 David A­D­ Morano.  All rights reserved. */

#ifndef	FILESSHOW_INCLUDE
#define	FILESSHOW_INCLUDE


#include	<envstandards.h>	/* MUST be first to configure */
#include	<clanguage.h>
#include	<usysbase.h>


EXTERNC_begin

extern int	files_show(cchar *,int) noex ;

EXTERNC_end


#endif /* FILESSHOW_INCLUDE */


