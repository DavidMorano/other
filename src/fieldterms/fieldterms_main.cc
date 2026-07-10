/* fieldterms_main SUPPORT (fieldterms) */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* show (display) the terminator block characters */
/* version %I% last-modified %G% */

#define	CF_CHARTEST	0		/* test |char(3uc)| */
#define	CF_SIF		1		/* test |sif(3uc)| */

/* revision history:

	= 1998-04-13, David A-D- Morano
	Originally written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */
/* Use is subject to license terms. */

#include	<envstandards.h>	/* ordered first to configure */
#include	<climits>		/* |UCHAR_MAX| */
#include	<cstddef>		/* CSTD */
#include	<cstdlib>		/* CSTD */
#include	<cstdio>		/* |fprintf(3c)| */
#include	<cstring>		/* |lenstr(3c)| */
#include	<new>			/* C++STD */
#include	<exception>		/* C++STD */
#include	<stdexcept>		/* C++STD */
#include	<iostream>		/* C++STD */
#include	<sstream>		/* C++STD */
#include	<clanguage.h>		/* LIBU */
#include	<usysbase.h>		/* LIBU */
#include	<baops.h>		/* LIBUC */
#include	<strn.h>		/* LIBUC */
#include	<sfx.h>			/* LIBUC */
#include	<cfdec.h>		/* LIBUC */
#include	<char.h>		/* LIBUC */
#include	<hasx.h>		/* LIBUC */
#include	<ascii.h>		/* LIBU */
#include	<ccfile.hh>		/* LIBU */
#include	<strnul.hh>		/* LIBU */
#include	<charnames.h>		/* LIBU */
#include	<localmisc.h>		/* LIBU |MAXLINELEN| */

#pragma		GCC dependency		"mod/libutil.ccm"
#pragma		GCC dependency		"mod/sif.ccm"
#pragma		GCC dependency		"mod/digtab.ccm"

import libutil ;			/* |lenstr(3u)| */
import sif ;
import digtab ;

/* local defines */

#define	MAXLINE		MAXLINELEN


/* imported namespaces */

using std::string ;			/* type */
using std::cerr ;			/* variable */
using std::cout ;			/* variable */
using std::nothrow ;			/* constant */
using std::bad_alloc ;			/* constant */


/* local typedefs */


/* external subroutines */


/* external variables */


/* local structures */

namespace {
    constexpr int	termsize = ((UCHAR_MAX+1)/CHAR_BIT) ;
    struct termer {
	int	idx = 0 ;
	char	terms[termsize] = {} ;
	int accum(cchar *,int) noex ;
	operator int () noex { return 0 ; } ;
    } ; /* end struct (termer) */
} /* end namespace */


/* forward references */

local void	showterms(cchar *) noexcept ;
local int	chartest() noex ;

local int	procfile(cchar *) noex ;
local int	procfiler(termer *,cchar *) noex ;
local int	procline(termer *,cchar *,int) noex ;
local int	cfnum(cchar *,int,int *) noex ;


/* local variables */

constexpr int		tablen = (UCHAR_MAX+1) ;
constexpr bool		f_chartest = CF_CHARTEST ;
constexpr bool		f_sif = CF_SIF ;


/* exported variables */


/* exported subroutines */

int main(int argc,mainv argv,mainv) {
	int		ex = EXIT_SUCCESS ;
	int		rs = SR_OK ;
	if_constexpr (f_chartest) {
	     chartest() ;
	}
	if (argc > 1) {
	    for (int ai = 1 ; (rs >= 0) && (ai < argc) ; ai += 1) {
	        cchar	*fn = argv[ai] ;
		if (fn[0] != '\0') {
	            rs = procfile(fn) ;
		}
	    } /* end for */
	} /* end if (argument) */
	if ((ex == EXIT_SUCCESS) && (rs < 0)) {
	    ex = EXIT_FAILURE ;
	}
	return ex ;
} /* end subroutine (main) */


/* local subroutines */

local int chartest() noex {
	constexpr cchar		tstr[] = "ab \r\n" ;
	int		rs = SR_OK ;
	for (int i = 0 ; tstr[i] ; i += 1) {
	    bool	f = CHAR_ISWHITE(tstr[i]) ;
	    cerr << "chartest ch=>" << tstr[i] << "<" << " f=" << f << eol ;
	} /* end for */
	return rs ;
} /* end subroutine */

int termer::accum(cchar *sp,int sl) noex {
	int		rs ;
	if (int v ; (rs = cfnum(sp,sl,&v)) >= 0) {
	    terms[idx++] = uchar(v) ;
	} /* end if */
	return rs ;
} /* end method (termer::accum) */

local int procfile(cchar *fn) noex {
	int		rs ;
	if (termer to ; (rs = procfiler(&to,fn)) >= 0) {
	    showterms(to.terms) ;
	} /* end if (procfiler) */
	return rs ;
} /* end subroutine (procfile) */

local int procfiler(termer *top,cchar *fn) noex {
	cnullptr	np{} ;
        cint            llen = MAXLINE ;
        int             rs = SR_NOMEM ;
        int             rs1 ;
        if (char *lbuf ; (lbuf = new(nothrow) char[llen+1]) != np) {
            try {
                if (ccfile fis ; (rs = fis.open(fn)) >= 0) {
                    while ((rs = fis.readln(lbuf,llen)) > 0) {
			cchar	*cp{} ;
			if (int cl ; (cl = sfcontent(lbuf,rs,&cp)) > 0) {
			    rs = procline(top,cp,cl) ;
                        } /* end if (hasnotempty) */
                    } /* end while */
                    rs1 = fis.close ;
                    if (rs >= 0) rs = rs1 ;
                } /* end if (opened) */
	    } catch (std::bad_alloc &e) {
		(void) e ;
		rs = SR_NOMEM ;
            } catch (...) {
                rs = SR_IO ;
            }
            delete [] lbuf ;            
        } /* end if (m-a-f) */          
        return rs ;
} /* end subroutine (procfiler) */

local int procline(termer *top,cchar *lp,int ll) noex {
	int		rs ;
	cchar		*cp{} ;
	if_constexpr (f_sif) {
	    for (sif fo(lp,ll,',') ; (rs = fo(&cp)) > 0 ; ) {
	        rs = top->accum(cp,rs) ;
	        if (rs < 0) break ;
	    } /* end for */
	} else {
	    rs = SR_OK ;
	    (void) top ;
	    for (int cl ; (cl = sfnextchr(lp,ll,',',&cp)) > 0 ; ) {
		ll -= intconv((cp + cl + 1) - lp) ;
		lp = (cp + cl + 1) ;
	    } /* end for */
	} /* end if_constexpr (f_sif) */
	return rs ;
} /* end subroutine (procline) */

local int cfnum(cchar *sp,int sl,int *rp) noex {
	int		rs = SR_FAULT ;
	if (sp && rp) {
	    rs = SR_OK ;
	    if (sl < 0) sl = lenstr(sp) ;
	    try {
	        string	ss(sp,sl) ;
		{
		    cchar	*raw = ss.c_str() ;
		    errno = 0 ;
		    {
			long	val = strtol(raw,nullptr,0) ;
			if (errno) {
			    rs = (- errno) ;
			} else {
			    *rp = int(val) ;
			}
		    } /* end block */
		} /* end block */
	    } catch (std::bad_alloc &) {
		rs = SR_NOMEM ;
	    } /* end block */
	} /* end if (non-null) */
	return rs ;
} /* end subroutine (cfnum) */

void showterms(cchar *terms) noexcept {
	for (int ch = 0 ; ch < tablen ; ch += 1) {
	    if (BATSTB(terms,ch)) {
		cchar *name = charname[ch] ;
		printf("0x%02X %s\n",ch,name) ;
	    } /* end if (hit) */
	} /* end for */
} /* end subroutine (showterms) */


