/* fmtstr HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* header file for the FMTSTR subroutine */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-07-01, David A­D­ Morano
	This file was originally written.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	FMTSTR_INCLUDE
#define	FMTSTR_INCLUDE


#include	<envstandards.h>	/* MUST be first to configure */
#include	<stdarg.h>		/* |va_list(3c)| */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>

#include	<fmtopt.h>


#ifndef	FMTSTR_NULLSTR
#define	FMTSTR_NULLSTR		"(null)"
#endif

EXTERNC_begin

extern int	fmtstr(char *,int,int,cchar *,va_list) noex ;

EXTERNC_end


#endif /* FMTSTR_INCLUDE */


