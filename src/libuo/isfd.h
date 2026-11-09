/* isfiledesc HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* is a file-descriptor associated with a someting? */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-04-10, David A­D­ Morano
	This subroutine was written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	ISFILEDESC_INCLUDE
#define	ISFILEDESC_INCLUDE


#include	<envstandards.h>	/* MUST be first to configure */
#include	<clanguage.h>		/* LIBU */
#include	<utypedefs.h>		/* LIBU */
#include	<utypealiases.h>	/* LIBU */
#include	<usysdefs.h>		/* LIBU */
#include	<usysrets.h>		/* LIBU */


EXTERNC_begin

extern int	isfdterminal	(int) noex ;
extern int	isfdsocket	(int) noex ;
extern int	isfdfsremote	(int) noex ;

EXTERNC_end


#endif /* ISFILEDESC_INCLUDE */


