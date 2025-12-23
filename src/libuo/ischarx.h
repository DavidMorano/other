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
#include	<limits.h>		/* |UCHAR_MAX| */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>


EXTERNC_begin

extern bool	isalphalatin(int) noex ;
extern bool	isalnumlatin(int) noex ;
extern bool	isdigitlatin(int) noex ;
extern bool	isdigexlatin(int) noex ;
extern bool	iswhitelatin(int) noex ;
extern bool	islowerlatin(int) noex ;
extern bool	isupperlatin(int) noex ;
extern bool	isprintbad(int) noex ;
extern bool	isprintlatin(int) noex ;
extern bool	isprintterm(int) noex ;
extern bool	isdict(int) noex ;
extern bool	iscmdstart(int) noex ;
extern bool	ishdrkey(int) noex ;
extern bool	ismmclass_7bit(int) noex ;
extern bool	ismmclass_8bit(int) noex ;
extern bool	ismmclass_binary(int) noex ;

EXTERNC_end

#ifdef	__cplusplus

constexpr inline bool	isdiglatin(int ch) noex {
	return (ch >= '0') && (ch <= '9') ;
}
constexpr inline bool	isoctlatin(int ch) noex {
	return (ch >= '0') && (ch <= '7') ;
}
constexpr inline bool	isdeclatin(int ch) noex {
	return (ch >= '0') && (ch <= '9') ;
}
constexpr inline bool	ishexlatin(int ch) noex {
    	return isdigexlatin(ch) ;
}
constexpr inline bool	iswhtlatin(int ch) noex {
    	return iswhitelatin(ch) ;
}
constexpr inline bool	iswhite(int ch) noex {
    	return iswhitelatin(ch) ;
}
constexpr inline bool	isnumlatin(int ch) noex {
    	return isdigexlatin(ch) || (ch == '\\') || (ch == 'x') ;
}
constexpr inline bool	islclatin(int ch) noex {
	return islowerlatin(ch) ;
}
constexpr inline bool	isuclatin(int ch) noex {
	return isupperlatin(ch) ;
}
constexpr inline bool	isnumsign(int ch) noex {
	return ((ch == '+') || (ch == '-')) ;
}
constexpr inline bool	iseol(int ch) noex {
	return (ch == '\n') || (ch == '\r') ;
}
constexpr inline bool	iszero(int ch) noex {
	return (ch == '0') ;
}
constexpr inline bool	isplusminus(int ch) noex {
	return (ch == '+') || (ch == '-') ;
}
constexpr inline bool	isabbr(int ch) noex {
	ch &= UCHAR_MAX ;
	return (ch == '.') || (ch == ('­' & UCHAR_MAX)) || (ch == '-') ;
}
constexpr inline bool	isblanklatin(int ch) noex {
	return (ch == ' ') || (ch == '\t') ;
}
constexpr inline bool	isspacetab(int ch) noex {
	return isblanklatin(ch) ;
}
constexpr inline bool	iswht(int ch) noex {
	return iswhite(ch) ;
}

#else /* __cplusplus */

static inline bool	isdiglatin(int ch) noex {
	return (ch >= '0') && (ch <= '9') ;
}
static inline bool	isoctlatin(int ch) noex {
	return (ch >= '0') && (ch <= '7') ;
}
static inline bool	isdeclatin(int ch) noex {
	return (ch >= '0') && (ch <= '9') ;
}
static inline bool	ishexlatin(int ch) noex {
    	return isdigexlatin(ch) ;
}
static inline bool	iswhtlatin(int ch) noex {
    	return iswhitelatin(ch) ;
}
static inline bool	iswhite(int ch) noex {
    	return iswhitelatin(ch) ;
}
static inline bool	isnumlatin(int ch) noex {
    	return isdigexlatin(ch) || (ch == '\\') || (ch == 'x') ;
}
constexpr inline bool   islclatin(int ch) noex {
        return islowerlatin(ch) ;
}
constexpr inline bool   isuclatin(int ch) noex {
        return isupperlatin(ch) ;
}
static inline bool	isnumsign(int ch) noex {
	return ((ch == '+') || (ch == '-')) ;
}
static inline bool	iseol(int ch) noex {
	return (ch == '\n') || (ch == '\r') ;
}
static inline bool	iszero(int ch) noex {
	return (ch == '0') ;
}
static inline bool	isplusminus(int ch) noex {
	return (ch == '+') || (ch == '-') ;
}
static inline bool	isabbr(int ch) noex {
	ch &= UCHAR_MAX ;
	return (ch == '.') || (ch == ('­' & UCHAR_MAX)) || (ch == '-') ;
}
static inline bool	isblanklatin(int ch) noex {
	return (ch == ' ') || (ch == '\t') ;
}
static inline bool	isspacetab(int ch) noex {
	return isblanklatin(ch) ;
}
static inline bool	iswht(int ch) noex {
	return iswhite(ch) ;
}

#endif /* __cplusplus */

EXTERNC_begin

static inline bool	isAbbr(int ch) noex {
    	return isabbr(ch) ;
}

EXTERNC_end


#endif /* ISCHARX_INCLUDE */


