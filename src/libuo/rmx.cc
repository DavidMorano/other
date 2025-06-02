/* rmx SUPPORT */
/* encoding=ISO8859-1 */
/* lang=C++20 */

/* remove characters from the end of the given c-string */
/* version %I% last-modified %G% */


/* revision history:

	= 2000-05-14, David A-D- Morano
	Originally written for Rightcore Network Services.

*/

/* Copyright © 2000 David A-D- Morano.  All rights reserved. */

/*******************************************************************************

	Names:
	rmeol
	rmochr
	rmrchr
	rmtrailchr
	rmext
	rmcomment

	Description:
	Return the length of the given string without any EOL
	(End-Of-Line) characters included. Any EOL characters are
	only considered starting from the end of the counted string.

	Synopsis:
	int rmeol(cchar *sp,int sl) noex
	int rmochr(cchar *sp,int sl,int sch) noex
	int rmrchr(cchar *sp,int sl,int sch) noex
	int rmtrailchr(cchar *sp,int sl,int sch) noex
	int rmext(cchar *sp,int sl) noex
	int rmcomment(cchar *sp,int sl) noex

	Arguments:
	sp		pinter to constant string to consider
	sl		length of supplied string
	sch		character to search for

	Returns:
	-		length of string without removed ending

*******************************************************************************/

#include	<envstandards.h>	/* MUST be first to configure */
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<strn.h>
#include	<matstr.h>
#include	<ischarx.h>
#include	<localmisc.h>

#include	"rmx.h"

import libutil ;

/* local defines */


/* imported namespaces */


/* local typedefs */


/* external subroutines */


/* external variables */


/* local structures */


/* forward references */


/* local variables */

constexpr cpcchar	exts[] = {
	"x",
	"s5",
	"s5u",
	"us5",
	"s4",
	"aout",
	"elf",
	"ksh",
	"sh",
	"ksh93",
	"csh",
	"osf",
	nullptr
} ; /* end array (exts) */


/* exported variables */


/* exported subroutines */

int rmeol(cchar *sp,int sl) noex {
	if (sl < 0) sl = xstrlen(sp) ;
	while (sl && iseol(sp[sl - 1])) {
	    sl -= 1 ;
	}
	return sl ;
}
/* end subroutine (rmeol) */

int rmochr(cchar *sp,int sl,int ch) noex {
	if (sl < 0) sl = xstrlen(sp) ;
	if (cchar *tp ; (tp = strnochr(sp,sl,ch)) != nullptr) {
	    sl = intconv(tp - sp) ;
	} /* end if */
	return sl ;
}
/* end subroutine (rmochr) */

int rmrchr(cchar *sp,int sl,int ch) noex {
	if (sl < 0) sl = xstrlen(sp) ;
	if (cchar *tp ; (tp = strnrchr(sp,sl,ch)) != nullptr) {
	    sl = intconv(tp - sp) ;
	} /* end if */
	return sl ;
}
/* end subroutine (rmrchr) */

int rmext(cchar *sp,int sl) noex {
	if (sl < 0) sl = xstrlen(sp) ;
	if (cchar *tp ; (tp = strnrchr(sp,sl,'.')) != nullptr) {
	    cint	el = intconv((sp + sl) - (tp + 1)) ;
	    cchar	*ep = (tp + 1) ;
	    if (matstr(exts,ep,el) >= 0) {
	        sl = intconv(tp - sp) ;
	    }
	} /* end if */
	return sl ;
}
/* end subroutine (rmext) */

int rmtrailchr(cchar *sp,int sl,int sch) noex {
	if (sl < 0) sl = xstrlen(sp) ;
	while ((sl > 1) && (sp[sl - 1] == char(sch))) {
	   sl -= 1 ;
	}
	return sl ;
}
/* end subroutine (rmtrailchr) */

int rmcomment(cchar *lp,int ll) noex {
	int		rl ;
	if (ll < 0) ll = xstrlen(lp) ;
	if ((rl = rmochr(lp,ll,'#')) == ll) {
	   rl = rmeol(lp,ll) ;
	}
	return rl ;
}
/* end subroutine (rmcomment) */


