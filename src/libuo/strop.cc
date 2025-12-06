/* strop SUPPORT */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* string-read-operations */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-02-01, David A­D­ Morano
	This code was originally written.  

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

  	Object:
	strop

	Description:
	This object allows for some specialized manipulations on a
	counted-string object.  The given string is supplied by the
	caller and is read-only.

*******************************************************************************/

#include	<envstandards.h>	/* MUST be first to configure */
#include	<climits>		/* |UCHAR_MAX| */
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<cstring>		/* |strchr(3c)| */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysrets.h>
#include	<ulogerror.h>
#include	<six.h>			/* |siwhtbrk(3uc)| */
#include	<baops.h>
#include	<mkchar.h>
#include	<char.h>		/* |char_iswhite(3uc)| */
#include	<localmisc.h>

#include	"strop.h"

#pragma		GCC dependency		"mod/libutil.ccm"

import libutil ;			/* |getlenstr(3u)| */

/* local defines */


/* external subroutines */


/* external variables */


/* local structures */


/* forward references */

local inline bool iswht(int ch) noex {
    	return CHAR_ISWHITE(ch) ;
}

local bool iswhiteand(strop *sop,int tch) noex {
	cint	ch = mkchar(sop->sp[0]) ;
	return CHAR_ISWHITE(ch) && (ch != tch) ;
}

local bool isnotchr(strop *sop,int tch) noex {
	cint	ch = mkchar(sop->sp[0]) ;
	return (ch != tch) ;
}

local bool isnotterm(strop *sop,cchar *terms) noex {
	cint	ch = mkchar(sop->sp[0]) ;
	return (! batst(terms,ch)) ;
}

local bool isterm(strop *sop,cchar *terms) noex {
	cint	ch = mkchar(sop->sp[0]) ;
	return  batst(terms,ch) ;
}


/* local variables */


/* exported variables */


/* exported subroutines */

int strop_start(strop *sop,cchar *sp,int µsl) noex {
	int		rs = SR_FAULT ;
	if (sop && sp) ylikely {
	    rs = SR_INVALID ;
	    if (int sl ; (sl = getlenstr(sp,µsl)) >= 0) {
	        sop->sp = sp ;
	        sop->sl = sl ;
	        rs = sop->sl ;
	    }
	} /* end if (non-null) */
	return rs ;
}
/* end subroutine (strop_start) */

int strop_finish(strop *sop) noex {
	int		rs = SR_FAULT ;
	if (sop) ylikely {
	    rs = sop->sl ;
	    sop->sp = nullptr ;
	    sop->sl = 0 ;
	}
	return rs ;
}
/* end subroutine (strop_finish) */

int strop_shrink(strop *sop) noex {
    	int		rl ; /* return-value */
    	if ((rl = strop_white(sop)) >= 0) {
	    if (int si ; (si = siwht(sop->sp,sop->sl)) > 0) {
	        sop->sl -= si ;
	    }
	    rl = sop->sl ;
	} /* end if */
	return rl ;
} /* end subroutine (strop_shrink) */

int strop_white(strop *sop) noex {
    	int		rl = 0 ; /* return-value */
	if (int si ; (si = siskipwhite(sop->sp,sop->sl)) > 0) {
	    sop->sp += si ;
	    sop->sl -= si ;
	}
	rl = sop->sl ;
	return rl ;
}
/* end subroutine (strop_white) */

int strop_whitechr(strop *sop,int tch) noex {
    	int		rl = 0 ; /* return-value */
	while ((sop->sl > 0) && iswhiteand(sop,tch)) {
	    sop->sp += 1 ;
	    sop->sl -= 1 ;
	}
	rl = sop->sl ;
	return rl ;
}
/* end subroutine (strop_whitechr) */

int strop_fieldwht(strop *sop,cchar **rpp) noex {
    	cchar	wht[] = " \t\r\f\v" ;
    	return strop_fieldbrk(sop,wht,rpp) ;
}
/* end subroutine (strop_fieldwht) */

int strop_fieldchr(strop *sop,int sch,cchar **rpp) noex {
    	char	bstr[2] = { char(sch) } ;
	return strop_fieldbrk(sop,bstr,rpp) ;
}
/* end subroutine (strop_fieldchr) */

int strop_fieldbrk(strop *sop,cchar *ss,cchar **rpp) noex {
    	cnullptr	np{} ;
    	int		rs = SR_FAULT ;
	int		rl = -1 ; /* return-value */
	if (sop && ss && rpp) ylikely {
	    rs = SR_OK ;
	    *rpp = nullptr ;
	    if (sop->sl > 0) {
	        if (int si ; (si = siskipwhite(sop->sp,sop->sl)) > 0) {
	            sop->sp += si ;
	            sop->sl -= si ;
	        }
	        if (sop->sl > 0) {
	            *rpp = sop->sp ;
	            if (int si ; (si = siwhtbrk(sop->sp,sop->sl,ss)) >= 0) {
			cint tch = mkchar(sop->sp[si]) ; 
			rl = si ;
			if (strchr(ss,tch) != np) {
			    while ((rl > 0) && iswht(sop->sp[rl - 1])) {
				rl -= 1 ;
			    }
			}
	                sop->sp += (si + 1) ; /* step over found character */
	                sop->sl -= (si + 1) ; /* step over found character */
	            } else {
	                rl = sop->sl ;
	                sop->sl = 0 ;
	            }
	        } /* end if (non-zero) */
	    } /* end if (non-zero) */
	} /* end if (non-null) */
	return (rs >= 0) ? rl : rs ;
}
/* end subroutine (strop_fieldbrk) */

int strop_findchr(strop *sop,int tch) noex {
    	int		rl = 0 ; /* return-value */
	while ((sop->sl > 0) && isnotchr(sop,tch)) {
	    sop->sp += 1 ;
	    sop->sl -= 1 ;
	}
	rl = sop->sl ;
	return rl ;
}
/* end subroutine (strop_findchr) */

int strop_findterm(strop *sop,cchar *terms) noex {
    	int		rl = 0 ; /* return-value */
	while ((sop->sl > 0) && isnotterm(sop,terms)) {
	    sop->sp += 1 ;
	    sop->sl -= 1 ;
	}
	rl = sop->sl ;
	return rl ;
}
/* end subroutine (strop_findterm) */

int strop_spanterm(strop *sop,cchar *terms) noex {
    	int		rl = 0 ; /* return-value */
	while ((sop->sl > 0) && isterm(sop,terms)) {
	    sop->sp += 1 ;
	    sop->sl -= 1 ;
	}
	rl = sop->sl ;
	return rl ;
}
/* end subroutine (strop_spanterm) */

int strop_span(strop *sop,cchar *ss) noex {
    	int		rl = 0 ; /* return-value */
	if (int si ; (si = sispan(sop->sp,sop->sl,ss)) > 0) {
	    sop->sp += si ;
	    sop->sl -= si ;
	}
	rl = sop->sl ;
	return rl ;
}
/* end subroutine (strop_span) */


/* local subroutines */

int strop::start(cchar *ap,int al) noex {
	return strop_start(this,ap,al) ;
}
int strop::fieldwht(cchar **rpp) noex {
	return strop_fieldwht(this,rpp) ;
}
int strop::fieldchr(int sch,cchar **rpp) noex {
	return strop_fieldchr(this,sch,rpp) ;
}
int strop::fieldbrk(cchar *ss,cchar **rpp) noex {
	return strop_fieldbrk(this,ss,rpp) ;
}
int strop::whitechr(int tch) noex {
	return strop_whitechr(this,tch) ;
}
int strop::findchr(int tch) noex {
	return strop_findchr(this,tch) ;
}
int strop::findterm(cchar *terms) noex {
	return strop_findterm(this,terms) ;
}
int strop::spanterm(cchar *terms) noex {
	return strop_spanterm(this,terms) ;
}
int strop::span(cchar *ss) noex {
	return strop_span(this,ss) ;
}

void strop::dtor() noex {
	if (cint rs = finish ; rs < 0) {
	    ulogerror("strop",rs,"fini-finish") ;
	}
} /* end method (strop::dtor) */

strop_co::operator int () noex {
	int		rs = SR_BUGCHECK ;
	if (op) ylikely {
	   switch (w) {
	   case stropmem_remlen:
	       rs = op->sl ;
	       break ;
	   case stropmem_inc:
	       rs = 0 ;
	       if (op->sl > 0) {
		   rs = 1 ;
	           op->sp += 1 ;
	           op->sl -= 1 ;
	       }
	       break ;
	   case stropmem_shrink:
	       rs = strop_shrink(op) ;
	       break ;
	   case stropmem_white:
	       rs = strop_white(op) ;
	       break ;
	   case stropmem_whitedash:
	       rs = strop_whitechr(op,'-') ;
	       break ;
	   case stropmem_whitecolon:
	       rs = strop_whitechr(op,':') ;
	       break ;
	   case stropmem_finish:
	       rs = strop_finish(op) ;
	       break ;
	   } /* end switch */
	} /* end if (non-null) */
	return rs ;
} /* end method (strop_co::operator) */


