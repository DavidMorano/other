/* main SUPPORT (showterms) */
/* lang=C++20 */

/* show (display) the terminator block characters */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-04-13, David A-D- Morano
	Originally written for Rightcore Network Services.

*/

/* Copyright © 2017 David A­D­ Morano.  All rights reserved. */
/* Use is subject to license terms. */

#include	<envstandards.h>	/* ordered first to configure */
#include	<climits>		/* |UCHAR_MAX| + |CHAR_BIT| */
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>		/* |atoi(3c)| */
#include	<cstdio>
#include	<cstring>
#include	<new>
#include	<exception>
#include	<iostream>
#include	<usysrets.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<clanguage.h>
#include	<baops.h>
#include	<strn.h>
#include	<sfx.h>
#include	<ccfile.hh>
#include	<strnul.hh>
#include	<cfhex.h>
#include	<hasx.h>
#include	<localmisc.h>		/* |MAXLINELEN| */


/* local defines */

#define	MAXLINE		MAXLINELEN


/* imported namespaces */

using std::nullptr_t ;			/* type */
using std::cout ;			/* variable */
using std::nothrow ;			/* constant */


/* local typedefs */


/* external subroutines */


/* external variables */


/* local structures */

constexpr int		tablen = (UCHAR_MAX+1) ;

constexpr int		termsize = ((UCHAR_MAX+1)/CHAR_BIT) ;

namespace {
    struct termer {
	int	idx = 0 ;
	char	terms[termsize] = {} ;
	int accum(cchar *sp,int sl) noex {
	    int		rs ;
	    if (int v ; (rs = cfhex(sp,sl,&v)) >= 0) {
		terms[idx++] = uchar(v) ;
	    }
	    return rs ;
	} ;
    } ; /* end struct (termer) */
}


/* forward references */

static int	procfile(cchar *) noex ;
static int	procline(termer *,cchar *,int) noex ;
static int	readterms(termer *,cchar *) noex ;

static void	showterms(cchar *) noex ;


/* local variables */


/* exported variables */


/* exported subroutines */

int main(int argc,mainv argv,mainv) {
	int		ex = EXIT_SUCCESS ;
	int		rs = SR_OK ;
	if (argc > 1) {
	    cchar	*fn = argv[1] ;
	    rs = procfile(fn) ;
	} /* end if (argument) */
	if ((ex >= 0) && (rs < 0)) ex = EXIT_FAILURE ;
	return ex ;
}
/* end subroutine (main) */


/* local subroutines */

static void showterms(cchar *terms) noexcept {
	for (int ch = 0 ; ch < tablen ; ch += 1) {
	    if (BATSTB(terms,ch)) {
		if (strchr("\n\r\f\v\b",ch)) {
		    printf("¯\\x%02X",ch) ;
		} else {
		    if (ch < 0x20) {
		       printf("¯»%02X«",ch) ;
		    } else if (ch == 0xA0) {
		       printf("¯NBSP") ;
		    } else {
		       printf("¯%c¯",ch) ;
		    }
		} /* end if (special) */
	    } /* end if (hit) */
	} /* end for */
	printf("\n") ;
}
/* end subroutine (showterms) */

static int procfile(cchar *fn) noex {
	termer		to ;
	int		rs ;
	if ((rs = readterms(&to,fn)) >= 0) {
	    showterms(to.terms) ;
	}
	return rs ;
}
/* end subroutine (procfile) */

static int procline(termer *top,cchar *lp,int ll) noex {
	sof		fo(lp,ll) ;
	int		rs = SR_OK ;
	int		cl ;
	cchar		*cp{} ;
	while ((cl = fo.next(&cp)) > 0) {
	    rs = top->accum(cp,cl) ;
	    if (rs < 0) break ;
	} /* end while */
	return rs ;
}
/* end subroutine (procline) */

static int readterms(termer *top,cchar *fn) noex {
	cnullptr	np{} ;
        cint            llen = MAXLINE ;
        int             rs = SR_NOMEM ;
        int             rs1 ;
        if (char *lbuf ; (lbuf = new(nothrow) char[llen+1]) != np) {
            try {
                ccfile          fis ;
                if ((rs = fis.open(fn)) >= 0) {
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
}
/* end subroutine (readterms) */


