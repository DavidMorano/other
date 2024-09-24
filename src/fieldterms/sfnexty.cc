/* sfnext SUPPORT */
/* lang=C++20 */

/* find a sub-string within a larger string (given some criteria) */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-03-01, David A­D­ Morano
	This code was originally written.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Names:
	sfnext
	sfnextchr
	sfnextbrk
	sfnextterm

	Philosophy:
	An explicit terminator (given) is *optional* in order for
	a non-empty result string to be returned.  A new-line (NL)
	character and white-space are always implied terminators
	(this is historic).

	Description:
	This subroutine will extract the next white-space-separated
	field from the input buffer.  If an NL is encountered, it
	is treated like a delimiter and all characters before it
	(including zero characters) are returned as the delimited
	field.  The caller should test for zero characters being
	returned.  This condition could indicate either that the
	line has no additional characters, or that a NL was
	encountered.  If the caller determines that there are
	characters left in the line but that zero was returned, a
	check by the caller for a NL character is indicated.  If
	the caller determines that a NL was present, the caller
	should arrange to "step over" the NL before making new calls
	in order to get the fields beyond the NL.
	The returned string is white-space shrunken.

	Synopsis:
	int sfnext(cchar *sp,int sl,cchar **spp) noex
	int sfnextchr(cchar *sp,int sl,int sch,cchar **spp) noex
	int sfnextbrk(cchar *sp,int sl,cchar *str,cchar **spp) noex
	int sfnextterm(cchar *sp,int sl,cchar *terms,cchar **spp) noex

	Arguments:
	sp		pointer to start of user supplied buffer
	sl		length of user supplied buffer
	sch		search character
	str		search string
	terms		terminator characters
	spp		pointer to pointer of the found field

	Returns:
	>0		length of found field
	==0		no field found or a NL character was encountered


	Name:
	sfnextbrk

	Description:
	This subroutine will extract the next white-space-separated
	or a combination of white-space and a trailing termination
	character c-string from a counted source c-string.
	The returned string is white-space shrunken.

	Synopsis:
	int sfnextbrk(cchar *sp,int sl,cchar *terms,cchar **spp) noex

	Arguments:
	sp		pointer to start of user supplied buffer
	sl		length of user supplied buffer
	terms		terminator characters (in a c-string)
	spp		pointer to pointer of the found field

	Returns:
	>0		length of found field
	==0		no field found or a NL character was encountered

*******************************************************************************/

#include	<envstandards.h>	/* MUST be first to configure */
#include	<climits>		/* |UCHAR_MAX| */
#include	<cstddef>		/* |nullptr_t| */
#include	<cstring>		/* <- for |strchr(3c)| */
#include	<iostream>
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<char.h>
#include	<localmisc.h>		/* |eol| */

#include	"sfx.h"


/* local defines */


/* imported namespaces */

using std::cerr ;			/* variable */


/* local typedefs */


/* external subroutines */

extern "C" {
    extern int	sfnexty(cchar *,int,cchar **) noex ;
    extern int	sfnextchry(cchar *,int,int,cchar **) noex ;
    extern int	sfnextbrky(cchar *,int,cchar *,cchar **) noex ;
}

/* external variables */


/* local structures */

namespace {
   struct sfnextx {
	cchar		**rpp ;
	cchar		*sp ;
	int		sl ;
	sfnextx(cchar *p,int l,cchar **r) noex : rpp(r), sp(p), sl(l) { } ;
	operator int () noex ;
	virtual bool termx(int ch) noex {
	    return (ch == 0) ;
	} ;
	bool isterm(int ch) noex {
	    bool	f = false ;
	    ch &= UCHAR_MAX ;
cerr << "isterm ch=" << char(ch) << eol ;
	    f = f || (ch == '\n') ;
	    f = f || CHAR_ISWHITE(ch) ;
	    f = f || termx(ch) ;
cerr << "isterm ret f=" << f << eol ;
	    return f ;
	} ;
   } ; /* end struct (sfnextx) */
}

/* forward references */


/* local variables */


/* exported variables */


/* exported subroutines */

int sfnexty(cchar *sp,int sl,cchar **rpp) noex {
	sfnextx		sf(sp,sl,rpp) ;
	return sf ;
}

int sfnextchry(cchar *sp,int sl,int sch,cchar **rpp) noex {
	struct esfx : sfnextx {
	    int		sch ;
	    esfx(cchar *p,int l,cchar **r) noex : sfnextx(p,l,r) { } ;
	    bool termx(int ch) noex override {
		return (ch == sch) ;
	    } ;
	} ; /* end struct */
	esfx		sf(sp,sl,rpp) ;
	sf.sch = sch ;
	return sf ;
}

int sfnextbrky(cchar *sp,int sl,cchar *bstr,cchar **rpp) noex {
	int		rl = -1 ;
	struct esfx : sfnextx {
	    cchar	*bstr ;
	    esfx(cchar *p,int l,cchar **r) noex : sfnextx(p,l,r) { } ;
	    bool termx (int ch) noex override {
		return (strchr(bstr,ch) != nullptr) ;
	    } ;
	} ; /* end struct */
	if (bstr) {
	    esfx	sf(sp,sl,rpp) ;
	    sf.bstr = bstr ;
	    rl = sf ;
	} /* end if (non-null) */
	return rl ;
}


/* local subroutines */

sfnextx::operator int () noex {
	int		rl = -1 ;
	cchar		*rp = nullptr ;
	if (sp && rpp) {
	    while (sl && CHAR_ISWHITE(*sp)) {
	        sp += 1 ;
	        sl -= 1 ;
	    } /* end while */
	    rp = sp ;
	    while (sl && *sp && (! isterm(*sp))) {
cerr << "op ch=" << *sp << eol ;
	        sp += 1 ;
	        sl -= 1 ;
	    } /* end while */
	    rl = (sp - rp) ;
	    *rpp = rp ;
	} /* end if (non-null) */
	return rl ;
}
/* end subroutine (sfnextx:operator) */


