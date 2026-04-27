/* getourenv HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* get an environment variable value from the passed environment array */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-11-01, David A­D­ Morano
	This subroutine was written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	GETOURENV_INCLUDE
#define	GETOURENV_INCLUDE


#include	<envstandards.h>	/* MUST be first to configure */
#include	<clanguage.h>
#include	<usysbase.h>


EXTERNC_begin

extern cchar	*getourenv(con mainv,cchar *) noex ;

EXTERNC_end


#endif /* GETOURENV_INCLUDE */


