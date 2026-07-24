/* deb HEADER */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* version %I% last-modified %G% */
/* C-language defines */


/* revision history:

	= 1998-03-21, David A­D­ Morano
	This module was originally written.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

  	Name:
	deb

	Description:
	This header file is associated with the DEBUG module.
	Access of the DEBUG module is gained with the importation
	of the module; like:
	{
		import deb ;
	}
	Enjoy.

*******************************************************************************/

#ifndef	DEB_INCLUDE
#define	DEB_INCLUDE
#ifdef	__cplusplus


#include	<envstandards.h>	/* MUST be first to configure */
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<clanguage.h>
#include	<usysbase.h>


#define DEBPRINTF(FMT, ...) \
    debprintf(__func__, FMT __VA_OPT__(,) __VA_ARGS__)


#endif /* __cplusplus */
#endif /* DEB_INCLUDE */


