/* char HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* fast character text and conversion facility */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-04-05, David A­D­ Morano
	This module was adapted from assembly lanauge.

	= 2011-08-19, David A­D­ Morano
	I changed this to use the C++ |bitset| object instead of
	an array of bytes for the single-bit truth-value observers.

*/

/* Copyright © 1998,2011 David A­D­ Morano.  All rights reserved. */

/******************************************************************************

	Group:
	char

	Names:
	CHAR_ISALPHA
	CHAR_ISALNUM
	CHAR_ISDIGIT
	CHAR_ISDIGEX
	CHAR_ISWHITE
	CHAR_ISOCTAL
	CHAR_ISLC
	CHAR_ISUC
	CHAR_ISFC
	CHAR_ISDIG
	CHAR_ISOCT
	CHAR_ISDEC
	CHAR_ISHEX
	CHAR_ISWHT
	CHAR_ISSPACETAB
	CHAR_DICTORDER
	CHAR_TOBC
	CHAR_TOLC
	CHAR_TOUC
	CHAR_TOFC
	CHAR_TOVAL

	Description:
	This file (the header file) essentially supplies macros
	that provide the calling interface for this facility.  These
	macros provide super fast character test and conversion
	functions.  This level of speed is actually rarely needed
	since the normal corresponding UNIX®-supplied character
	subroutes are already very fast, but they are here for
	speacial needs that may arise.  Only 8-bit characters are
	supported (ISO-Latin-1 character set).  For other character
	sets, use the system-supplied facilities.

	Extra-note: 
	Note that non-breaking-white-space (NBSP) characters are
	*not* considered to be white-space!

******************************************************************************/

#ifndef	CHAR_INCLUDE
#define	CHAR_INCLUDE


#include	<envstandards.h>	/* ordered first to configure */
#include	<limits.h>		/* |UCHAR_MAX| */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>


/* support tables; not accessed directly by callers  */
extern const short		chardata_dictorder[] ;
extern const unsigned char	chardata_tolc[] ;
extern const unsigned char	chardata_touc[] ;
extern const unsigned char	chardata_tofc[] ;

/* test character attribute routines */
#define	CHAR_ISALPHA(c)		char_isalpha(c)
#define	CHAR_ISALNUM(c)		char_isalnum(c)
#define	CHAR_ISDIGIT(c)		char_isdigit(c)
#define	CHAR_ISDIGEX(c)		char_isdigex(c)
#define	CHAR_ISOCTAL(c)		char_isoctal(c)
#define	CHAR_ISWHITE(c)		char_iswhite(c)
#define	CHAR_ISBLANK(c)		char_isblank(c)
#define	CHAR_ISLC(c)		char_islc(c)
#define	CHAR_ISUC(c)		char_isuc(c)
#define	CHAR_ISFC(c)		char_isfc(c)
#define	CHAR_ISDIG(c)		char_isdig(c)
#define	CHAR_ISOCT(c)		char_isoct(c)
#define	CHAR_ISDEC(c)		char_isdec(c)
#define	CHAR_ISHEX(c)		char_ishex(c)
#define	CHAR_ISWHT(c)		char_iswht(c)
#define	CHAR_ISSPACETAB(c)	(((c) == ' ') || ((c) == '\t'))

/* dictionary-collating-ordinal */
#define	CHAR_DICTORDER(c)	(chardata_dictorder[(c) & UCHAR_MAX])

/* my super-fast conversions */
#define	CHAR_TOBC(c)		((c) & UCHAR_MAX)
#define	CHAR_TOLC(c)		(chardata_tolc[(c) & UCHAR_MAX])
#define	CHAR_TOUC(c)		(chardata_touc[(c) & UCHAR_MAX])
#define	CHAR_TOFC(c)		(chardata_tofc[(c) & UCHAR_MAX])
#define	CHAR_TOVAL(c)		char_toval(c) ;

EXTERNC_begin

extern bool char_isalpha(int) noex ;
extern bool char_isalnum(int) noex ;
local inline bool char_isdigit(int ch) noex {
	return ((ch >= '0') && (ch <= '9')) ;
}
local inline bool char_isdigex(int ch) noex {
    	bool f = false ;
	f = f || ((ch >= '0') && (ch <= '9')) ;
	f = f || ((ch >= 'A') && (ch <= 'Z')) ;
	f = f || ((ch >= 'a') && (ch <= 'z')) ;
	return f ;
}
local inline bool char_isoctal(int ch) noex {
	return ((ch >= '0') && (ch <= '7')) ;
}
local inline bool char_isblank(int ch) noex {
	return ((ch == ' ') && (ch == '\t')) ;
}
extern bool char_iswhite(int) noex ;
extern bool char_islc(int) noex ;
extern bool char_isuc(int) noex ;
extern bool char_isfc(int) noex ;

local inline bool char_isdig(int ch) noex {
	return ((ch >= '0') && (ch <= '9')) ;
}
local inline bool char_isoct(int ch) noex {
	return ((ch >= '0') && (ch <= '7')) ;
}
local inline bool char_isdec(int ch) noex {
	return ((ch >= '0') && (ch <= '9')) ;
}
local inline bool char_ishex(int ch) noex {
    	bool f = false ;
	f = f || ((ch >= '0') && (ch <= '9')) ;
	f = f || ((ch >= 'A') && (ch <= 'Z')) ;
	f = f || ((ch >= 'a') && (ch <= 'z')) ;
	return f ;
}
local inline bool char_iswht(int ch) noex {
    	return char_iswhite(ch) ;
}
local inline bool char_isspacetab(int ch) noex {
	return CHAR_ISSPACETAB(ch) ;
}
local inline short char_dictorder(int ch) noex {
	return chardata_dictorder[ch & UCHAR_MAX] ;
}
local inline uchar char_tobc(int ch) noex {
	return uchar(ch & UCHAR_MAX) ;
}
local inline uchar char_tolc(int ch) noex {
	return chardata_tolc[ch & UCHAR_MAX] ;
}
local inline uchar char_touc(int ch) noex {
	return chardata_touc[ch & UCHAR_MAX] ;
}
local inline uchar char_tofc(int ch) noex {
	return chardata_tofc[ch & UCHAR_MAX] ;
}
extern int char_toval(int) noex ;

EXTERNC_end


#endif /* CHAR_INCLUDE */


