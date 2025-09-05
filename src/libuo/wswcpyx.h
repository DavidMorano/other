/* wswcpyx HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* counted wide-c-string operations */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-11-01, David A­D­ Morano
	This subroutine was written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	WSWCPYX_INCLUDE
#define	WSWCPYX_INCLUDE


#include	<envstandards.h>	/* ordered first to configure */
#include	<stddef.h>		/* |wchar_t| */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>


EXTERNC_begin

extern int wswcpynarrow(wchar_t *,cchar *,int) noex ;

EXTERNC_end


#endif /* WSWCPYX_INCLUDE */


