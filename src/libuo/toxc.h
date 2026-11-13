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
#include	<limits.h>		/* CSTD |UCHAR_MAX| */
#include	<clanguage.h>		/* LIBU */
#include	<utypedefs.h>		/* LIBU */
#include	<utypealiases.h>	/* LIBU */
#include	<usysdefs.h>		/* LIBU */
#include	<char.h>		/* LIBUC |CHAR_{xx}(3uc)| */


EXTERNC_beg
local inline int tobc(int ch) noex {
	return (ch & UCHAR_MAX) ;
} /* end */
local inline int tolc(int ch) noex {
	return CHAR_TOLC(ch) ;
} /* end */
local inline int touc(int ch) noex {
	return CHAR_TOUC(ch) ;
} /* end */
local inline int tofc(int ch) noex {
	return CHAR_TOFC(ch) ;
} /* end */
EXTERNC_end

EXTERNC_beg
local inline char chtobc(int ch) noex {
	return char(ch) ;
} /* end */
local inline char chtolc(int ch) noex {
	return char(CHAR_TOLC(ch)) ;
} /* end */
local inline char chtouc(int ch) noex {
	return char(CHAR_TOUC(ch)) ;
} /* end */
local inline char chtofc(int ch) noex {
	return char(CHAR_TOFC(ch)) ;
} /* end */
EXTERNC_end


#endif /* TOXC_INCLUDE */


