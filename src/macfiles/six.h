/* six HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* string-index operations */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-11-01, David A­D­ Morano
	This subroutine was written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	SIX_INCLUDE
#define	SIX_INCLUDE


#include	<envstandards.h>	/* ordered first to configure */
#include	<clanguage.h>
#include	<usysbase.h>

#include	<sicasechr.h>
#include	<sichar.h>
#include	<siext.h>
#include	<sifext.h>
#include	<sileadchr.h>
#include	<sileader.h>
#include	<sileadzero.h>
#include	<sinext.h>
#include	<sinon.h>
#include	<sispan.h>
#include	<sixbrk.h>
#include	<sixchr.h>
#include	<sixterm.h>
#include	<sixsub.h>
#include	<siwht.h>


EXTERNC_begin

extern int sidquote(cchar *,int) noex ;
extern int sibasename(cchar *,int) noex ;
extern int silbrace(cchar *,int) noex ;
extern int sihyphen(cchar *,int) noex ;
extern int sicite(cchar *,int,cchar *,int) noex ;

EXTERNC_end


#endif /* SIX_INCLUDE */


