/* ucstrftime HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* UNIX® UCSTRFTIME support */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-03-21, David A­D­ Morano
	This module was originally written.

	= 2017-08-01, David A­D­ Morano
	Updated for lack of interfaces in Apple-Darwin

*/

/* Copyright © 1998,2017 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

  	Name:
	uc_strftime

	Description:
	This file contains some support for the UNIX® UCSTRFTIME
	related operations.

*******************************************************************************/

#ifndef	UCSTRFTIME_INCLUDE
#define	UCSTRFTIME_INCLUDE


#include	<envstandards.h>	/* ordered first to configure */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>


EXTERNC_begin

extern int uc_strftime(char *,int,cchar *,CTM *) noex ;

EXTERNC_end


#endif /* UCSTRFTIME_INCLUDE */


