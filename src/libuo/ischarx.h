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

extern bool	iswhite(int) noex ;
extern bool	isalnumlatin(int) noex ;
extern bool	isalphalatin(int) noex ;
extern bool	isdict(int) noex ;
extern bool	ishexlatin(int) noex ;
extern bool	islowerlatin(int) noex ;
extern bool	isupperlatin(int) noex ;
extern bool	isprintbad(int) noex ;
extern bool	isprintlatin(int) noex ;
extern bool	isprintterm(int) noex ;
extern bool	iscmdstart(int) noex ;
extern bool	ishdrkey(int) noex ;
extern bool	ismmclass_7bit(int) noex ;
extern bool	ismmclass_8bit(int) noex ;
extern bool	ismmclass_binary(int) noex ;

EXTERNC_end

#ifdef	__cplusplus

constexpr inline bool	isdigitlatin(int ch) noex {
	return (ch >= '0') && (ch <= '9') ;
}
constexpr inline bool	isxdigitlatin(int ch) noex {
	return ishexlatin(ch) ;
}
constexpr inline bool	isdeclatin(int ch) noex {
    	return isdigitlatin(ch) ;
}
constexpr inline bool	isoctlatin(int ch) noex {
	return (ch >= '0') && (ch <= '7') ;
}
constexpr inline bool	isnumlatin(int ch) noex {
    	return ishexlatin(ch) || (ch == '\\') || (ch == 'x') ;
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

#else /* __cplusplus */

static inline bool	isdigitlatin(int ch) noex {
	return (ch >= '0') && (ch <= '9') ;
}
static inline bool	isxdigitlatin(int ch) noex {
	return ishexlatin(ch) ;
}
static inline bool	isdeclatin(int ch) noex {
    	return isdigitlatin(ch) ;
}
static inline bool	isoctlatin(int ch) noex {
	return (ch >= '0') && (ch <= '7') ;
}
static inline bool	isnumlatin(int ch) noex {
    	return ishexlatin(ch) || (ch == '\\') || (ch == 'x') ;
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
constexpr inline bool	isblanklatin(int ch) noex {
	return (ch == ' ') || (ch == '\t') ;
}
constexpr inline bool	isspacetab(int ch) noex {
	return isblanklatin(ch) ;
}

#endif /* __cplusplus */

EXTERNC_begin

static inline bool	isAbbr(int ch) noex {
    	return isabbr(ch) ;
}

EXTERNC_end


#endif /* ISCHARX_INCLUDE */


