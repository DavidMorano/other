/* debprintf HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* version %I% last-modified %G% */
/* C-language defines */


/* revision history:

	= 1998-03-21, David A­D­ Morano
	This module was originally written.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

  	Name:
	debprintf

	Description:
	This header file is associated with the DEBUG module.
	Access of the DEBUG module is gained with the importation
	of the module; like:
	{
		import debug ;
	}
	Enjoy.

*******************************************************************************/

#ifndef	DEBPRINTF_INCLUDE
#define	DEBPRINTF_INCLUDE


#include	<envstandards.h>	/* MUST be first to configure */


#define DEBPRINTF(FMT, ...) debprintf(__func__, FMT __VA_OPT__(,) __VA_ARGS__)


#endif /* DEBPRINTF_INCLUDE */


