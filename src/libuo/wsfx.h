/* wsfx HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* counted wide-c-string operations */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-11-01, David A­D­ Morano
	This subroutine was written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	WSX_INCLUDE
#define	WSX_INCLUDE


#include	<envstandards.h>	/* ordered first to configure */
#include	<stddef.h>		/* |wchar_t| */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>


EXTERNC_begin

extern int wsfnext(const wchar_t *,int,const wchar_t **) noex ;
extern int wswcpynarrow(wchar_t *,cchar *,int) noex ;

EXTERNC_end


#endif /* WSX_INCLUDE */


