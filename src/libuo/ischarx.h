/* ischarx HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* test a character for a property */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-11-01, David A­D­ Morano
	This subroutine was written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	ISCHARX_INCLUDE
#define	ISCHARX_INCLUDE


#include	<envstandards.h>	/* MUST be first to configure */
#include	<limits.h>		/* CSTD |UCHAR_MAX| */
#include	<clanguage.h>		/* LIBU */
#include	<utypedefs.h>		/* LIBU */
#include	<utypealiases.h>	/* LIBU */
#include	<usysdefs.h>		/* LIBU */


EXTERNC_begin

extern bool	isalphalatin	(int) noex attrpure ;
extern bool	isalnumlatin	(int) noex attrpure ;
extern bool	isdigitlatin	(int) noex attrpure ;
extern bool	isdigexlatin	(int) noex attrpure ;
extern bool	iswhitelatin	(int) noex attrpure ;
extern bool	islowerlatin	(int) noex attrpure ;
extern bool	isupperlatin	(int) noex attrpure ;
extern bool	isprintlatin	(int) noex attrpure ;
extern bool	istermlatin	(int) noex attrpure ;
extern bool	isprintterm	(int) noex attrpure ;
extern bool	isprintbad	(int) noex attrpure ;
extern bool	isdict		(int) noex attrpure ;
extern bool	iscmdstart	(int) noex attrpure ;
extern bool	ishdrkey	(int) noex attrpure ;

local inline bool	ispl	(int ch) noex attrconst {
	return (ch == '+') ;
} /* end */
local inline bool	ismi	(int ch) noex attrconst {
	return (ch == '-') ;
} /* end */
local inline bool	ispm	(int ch) noex attrconst {
	return (ch == '+') || (ch == '-') ;
} /* end */
local inline bool	isme	(int ch) noex attrconst {
	return (ch == '+') || (ch == '-') || (ch == '!') ;
} /* end */

EXTERNC_end

#ifdef	__cplusplus

constexpr inline bool	isbinarlatin	(int ch) noex {
	return (ch >= '0') && (ch <= '1') ;
} /* end */
constexpr inline bool	isoctallatin	(int ch) noex {
	return (ch >= '0') && (ch <= '7') ;
} /* end */
constexpr inline bool	isblanklatin	(int ch) noex {
	return (ch == ' ') || (ch == '\t') ;
} /* end */
constexpr inline bool	isdiglatin	(int ch) noex {
	return (ch >= '0') && (ch <= '9') ;
} /* end */
constexpr inline bool	isbinlatin	(int ch) noex {
	return (ch >= '0') && (ch <= '1') ;
} /* end */
constexpr inline bool	isoctlatin	(int ch) noex {
	return (ch >= '0') && (ch <= '7') ;
} /* end */
constexpr inline bool	isdeclatin	(int ch) noex {
	return (ch >= '0') && (ch <= '9') ;
} /* end */
constexpr inline bool	ishexlatin	(int ch) noex {
    	return isdigexlatin(ch) ;
} /* end */
constexpr inline bool	iswhtlatin	(int ch) noex {
    	return iswhitelatin(ch) ;
} /* end */
constexpr inline bool	isnumlatin	(int ch) noex {
    	return isdigexlatin(ch) || (ch == '\\') || (ch == 'x') ;
} /* end */
constexpr inline bool	islclatin	(int ch) noex {
	return islowerlatin(ch) ;
} /* end */
constexpr inline bool	isuclatin	(int ch) noex {
	return isupperlatin(ch) ;
} /* end */
constexpr inline bool	iseol		(int ch) noex {
	return (ch == '\n') || (ch == '\r') ;
} /* end */
constexpr inline bool	iszero		(int ch) noex {
	return (ch == '0') ;
} /* end */
constexpr inline bool	isabbr		(int ch) noex {
	ch &= UCHAR_MAX ;
	return (ch == '.') || (ch == ('­' & UCHAR_MAX)) || (ch == '-') ;
} /* end */
constexpr inline bool	iswht		(int ch) noex {
	return iswhitelatin(ch) ;
} /* end */
constexpr inline bool	isblk		(int ch) noex {
	return isblanklatin(ch) ;
} /* end */
constexpr inline bool	isspacetab	(int ch) noex {
	return isblanklatin(ch) ;
} /* end */

#else /* __cplusplus */

local inline bool	isbinarlatin(int ch) noex attrconst {
	return (ch >= '0') && (ch <= '1') ;
} /* end */
local inline bool	isoctallatin(int ch) noex attrconst {
	return (ch >= '0') && (ch <= '7') ;
} /* end */
local inline bool	isblanklatin(int ch) noex attrconst {
	return (ch == ' ') || (ch == '\t') ;
} /* end */
local inline bool	isdiglatin(int ch) noex attrconst {
	return (ch >= '0') && (ch <= '9') ;
} /* end */
local inline bool	isbinlatin(int ch) noex attrconst {
	return (ch >= '0') && (ch <= '1') ;
} /* end */
local inline bool	isoctlatin(int ch) noex attrconst {
	return (ch >= '0') && (ch <= '7') ;
} /* end */
local inline bool	isdeclatin(int ch) noex attrconst {
	return (ch >= '0') && (ch <= '9') ;
} /* end */
local inline bool	ishexlatin(int ch) noex attrpure {
    	return isdigexlatin(ch) ;
} /* end */
local inline bool	iswhtlatin(int ch) noex attrpure {
    	return iswhitelatin(ch) ;
} /* end */
local inline bool	iswhite(int ch) noex attrpure {
    	return iswhitelatin(ch) ;
} /* end */
local inline bool	isnumlatin(int ch) noex attrpure {
    	return isdigexlatin(ch) || (ch == '\\') || (ch == 'x') ;
} /* end */
local inline bool   islclatin(int ch) noex attrpure {
        return islowerlatin(ch) ;
} /* end */
local inline bool   isuclatin(int ch) noex attrpure {
        return isupperlatin(ch) ;
} /* end */
local inline bool	iseol(int ch) noex attrconst {
	return (ch == '\n') || (ch == '\r') ;
} /* end */
local inline bool	iszero(int ch) noex attrconst {
	return (ch == '0') ;
} /* end */
local inline bool	isabbr(int ch) noex attrconst {
	ch &= UCHAR_MAX ;
	return (ch == '.') || (ch == ('­' & UCHAR_MAX)) || (ch == '-') ;
} /* end */
local inline bool	iswht(int ch) noex attrpure {
	return iswhitelatin(ch) ;
} /* end */
local inline bool	isblk(int ch) noex attrpure {
	return isblanklatin(ch) ;
} /* end */
local inline bool	isspacetab(int ch) noex attrpure {
	return isblanklatin(ch) ;
} /* end */

#endif /* __cplusplus */

EXTERNC_begin

local inline bool	isAbbr(int ch) noex attrpure {
    	return isabbr(ch) ;
} /* end */

EXTERNC_end


#endif /* ISCHARX_INCLUDE */


