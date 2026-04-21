/* strxcmp HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* c-string comparisons */
/* version %I% last-modified %G% */


/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	STRXCMP_INCLUDE
#define	STRXCMP_INCLUDE


#include	<envstandards.h>	/* ordered first to configure */
#include	<strings.h>		/* |strcasecmp(3c)| + |strcmp(3c)| */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<strnxcmp.h>		/* |strnndictcmp(3uc)| */

#include	<stremacmp.h>
#include	<strkeycmp.h>
#include	<strvalcmp.h>
#include	<strxxxxcmp.h>
#include	<strleadcmp.h>
#include	<strpcmp.h>


EXTERNC_begin

local inline int strdictcmp(cchar *s1,cchar *s2) noex {
	return strnndictcmp(s1,-1,s2,-1) ;
}

EXTERNC_end


#endif /* STRXCMP_INCLUDE */


