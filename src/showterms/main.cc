/* main SUPPORT (showterms) */
/* encoding=ISO8859-1 */
/* lang=C++20 */

/* show (display) the terminator block characters */
/* version %I% last-modified %G% */

#define	CF_FIELD	0		/* used |field(3uc)| */

/* revision history:

	= 1998-04-13, David A-D- Morano
	Originally written for Rightcore Network Services.

*/

/* Copyright © 2017 David A­D­ Morano.  All rights reserved. */
/* Use is subject to license terms. */

/*******************************************************************************

	Name:
	showterms(1)

	Description:
	This program displays (to STDOUT) the characters within an
	old-style terminal block (in source C-language form).

	Synopsis:
	$ showterms [<file>]

	Arguments:
	<file>		optional file, of STDIN otherwise

*******************************************************************************/

#include	<envstandards.h>	/* ordered first to configure */
#include	<climits>		/* |UCHAR_MAX| + |CHAR_BIT| */
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>		/* |atoi(3c)| + |strtol(3c)| */
#include	<cstdio>
#include	<cstring>
#include	<new>
#include	<exception>
#include	<iostream>
#include	<iomanip>
#include	<usysrets.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<clanguage.h>
#include	<baops.h>
#include	<strn.h>
#include	<sfx.h>
#include	<sif.hh>
#include	<field.h>
#include	<fieldterms.h>
#include	<fieldterminit.hh>
#include	<ccfile.hh>
#include	<strnul.hh>
#include	<char.h>
#include	<mkchar.h>
#include	<hasx.h>
#include	<localmisc.h>		/* |MAXLINELEN| */


/* local defines */

#define	MAXLINE		MAXLINELEN


/* imported namespaces */

using std::nullptr_t ;			/* type */
using std::cout ;			/* (global) variable */
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
	    strnul	s(sp,sl) ;
	    int		rs = SR_OK ;
	    long	v ; 
	    if ((v = strtol(s,nullptr,0)) == 0) {
		rs = (- errno) ;
	    }
	    if (rs >= 0) {
		terms[idx++] = uchar(v) ;
	    }
	    return rs ;
	} ; /* end method (accum) */
    } ; /* end struct (termer) */
}


/* forward references */

static int	procfile(cchar *) noex ;
static int	procline(termer *,cchar *,int) noex ;
static int	readterms(termer *,cchar *) noex ;

static void	showterms(cchar *) noex ;


/* local variables */

static fieldterminit	fterms(",") ;

constexpr bool		f_field = CF_FIELD ;


/* exported variables */


/* exported subroutines */

int main(int argc,mainv argv,mainv) {
	int		ex = EXIT_SUCCESS ;
	int		rs = SR_OK ;
	if (argc > 1) {
	    cchar	*fn = argv[1] ;
	    rs = procfile(fn) ;
	} /* end if (argument) */
	if ((ex >= 0) && (rs < 0)) {
	    ex = EXIT_FAILURE ;
	}
	return ex ;
}
/* end subroutine (main) */


/* local subroutines */

static void showterms(cchar *terms) noexcept {
	for (int ch = 0 ; ch < tablen ; ch += 1) {
	    if (BATSTB(terms,ch)) {
		if (strchr("\n\r\f\v\b",ch)) {
		    printf("¯\\x%02X¯",ch) ;
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
	int		rs ;
	if (termer to ; (rs = readterms(&to,fn)) >= 0) {
	    showterms(to.terms) ;
	}
	return rs ;
}
/* end subroutine (procfile) */

static int procline(termer *top,cchar *lp,int ll) noex {
	int		rs = SR_OK ;
	int		rs1 ;
	int		c = 0 ;
	if_constexpr (f_field) {
	    if (field fb ; (rs = fb.start(lp,ll)) >= 0) {
	        int	fl ;
	        cchar	*fp ;
	        cchar	*t = fterms.terms ;
	        while ((fl = fb.get(t,&fp)) > 0) {
		    c += 1 ;
	            rs = top->accum(fp,fl) ;
	            if (rs < 0) break ;
	        } /* end while */
	        rs1 = fb.finish ;
	        if (rs >= 0) rs = rs1 ;
	    } /* end if (field) */
	} else {
	    sif		fb(lp,ll,',') ;
	    int		fl ;
	    cchar	*fp ;
	    while ((fl = fb(&fp)) > 0) {
		c += 1 ;
	        rs = top->accum(fp,fl) ;
	        if (rs < 0) break ;
	    } /* end while */
	} /* end if_constexpr (f_field) */
	return (rs >= 0) ? c : rs ;
}
/* end subroutine (procline) */

static int readterms(termer *top,cchar *fn) noex {
	cnullptr	np{} ;
        cint            llen = MAXLINE ;
        int             rs = SR_NOMEM ;
        int             rs1 ;
	int		c = 0 ;
        if (char *lbuf ; (lbuf = new(nothrow) char[llen+1]) != np) {
            try {
                if (ccfile fis ; (rs = fis.open(fn)) >= 0) {
                    while ((rs = fis.readln(lbuf,llen)) > 0) {
			cchar	*cp{} ;
			if (int cl ; (cl = sfcontent(lbuf,rs,&cp)) > 0) {
			    rs = procline(top,cp,cl) ;
			    c += rs ;
                        } /* end if (hasnotempty) */
			if (rs < 0) break ;
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
        return (rs >= 0) ? c : rs ;
}
/* end subroutine (readterms) */


