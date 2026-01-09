/* toxc HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* convert a single character to a given case */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-04-10, David A­D­ Morano
	This subroutine was written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	TOXC_INCLUDE
#define	TOXC_INCLUDE


#include	<envstandards.h>	/* MUST be first to configure */
#include	<limits.h>		/* |UCHAR_MAX| */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<char.h>		/* |CHAR_{xx}(3uc)| */


EXTERNC_begin

local inline int tobc(int ch) noex {
	return (ch & UCHAR_MAX) ;
}
local inline int tolc(int ch) noex {
	return CHAR_TOLC(ch) ;
}
local inline int touc(int ch) noex {
	return CHAR_TOUC(ch) ;
}
local inline int tofc(int ch) noex {
	return CHAR_TOFC(ch) ;
}

local inline char chtobc(int ch) noex {
	return char(ch) ;
}
local inline char chtolc(int ch) noex {
	return char(CHAR_TOLC(ch)) ;
}
local inline char chtouc(int ch) noex {
	return char(CHAR_TOUC(ch)) ;
}
local inline char chtofc(int ch) noex {
	return char(CHAR_TOFC(ch)) ;
}

EXTERNC_end


#endif /* TOXC_INCLUDE */


