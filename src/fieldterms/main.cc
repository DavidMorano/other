/* main SUPPORT (fieldterms) */
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

/* Copyright © 2017 David A­D­ Morano.  All rights reserved. */
/* Use is subject to license terms. */

#include	<envstandards.h>	/* ordered first to configure */
#include	<climits>		/* <- for |UCHAR_MAX| */
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>		/* |atoi(3c)| + |strtol(3c)| */
#include	<cstdio>		/* |fprintf(3c)| */
#include	<cstring>		/* |strlen(3c)| */
#include	<new>
#include	<exception>
#include	<stdexcept>
#include	<iostream>
#include	<sstream>
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysrets.h>
#include	<baops.h>
#include	<strn.h>
#include	<sfx.h>
#include	<sif.hh>
#include	<ccfile.hh>
#include	<strnul.hh>
#include	<cfdec.h>
#include	<char.h>
#include	<hasx.h>
#include	<localmisc.h>		/* |MAXLINELEN| */


/* local defines */

#define	MAXLINE		MAXLINELEN


/* imported namespaces */

using std::nullptr_t ;			/* type */
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
}

/* forward references */

static void	showterms(cchar *) noexcept ;
static int	chartest() noex ;

static int	procfile(cchar *) noex ;
static int	procfiler(termer *,cchar *) noex ;
static int	procline(termer *,cchar *,int) noex ;
static int	cfnum(cchar *,int,int *) noex ;


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
	    }
	} /* end if (argument) */
	if ((ex == EXIT_SUCCESS) && (rs < 0)) ex = EXIT_FAILURE ;
	return ex ;
}
/* end subroutine (main) */


/* local subroutines */

static int chartest() noex {
	int		rs = SR_OK ;
	constexpr cchar		tstr[] = "ab \r\n" ;
	for (int i = 0 ; tstr[i] ; i += 1) {
	    bool	f = CHAR_ISWHITE(tstr[i]) ;
	    cerr << "chartest ch=>" << tstr[i] << "<" << " f=" << f << eol ;
	}
	return rs ;
}

int termer::accum(cchar *sp,int sl) noex {
	int		rs ;
	if (int v ; (rs = cfnum(sp,sl,&v)) >= 0) {
	    terms[idx++] = uchar(v) ;
	}
	return rs ;
} /* end method (termer::accum) */

static int procfile(cchar *fn) noex {
	termer		to ;
	int		rs ;
	if ((rs = procfiler(&to,fn)) >= 0) {
	    showterms(to.terms) ;
	} /* end if (procfiler) */
	return rs ;
}
/* end subroutine (procfile) */

static int procfiler(termer *top,cchar *fn) noex {
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
/* end subroutine (procfiler) */

static int procline(termer *top,cchar *lp,int ll) noex {
	int		rs ;
	cchar		*cp{} ;
	if_constexpr (f_sif) {
	    sif		fo(lp,ll,',') ;
	    while ((rs = fo(&cp)) > 0) {
	        rs = top->accum(cp,rs) ;
	        if (rs < 0) break ;
	    } /* end while */
	} else {
	    int		cl ;
	    rs = SR_OK ;
	    (void) top ;
	    while ((cl = sfnextchr(lp,ll,',',&cp)) > 0) {
		ll -= ((cp + cl + 1) - lp) ;
		lp = (cp + cl + 1) ;
	    }
	} /* end if_constexpr (f_sif) */
	return rs ;
}
/* end subroutine (procline) */

static int cfnum(cchar *sp,int sl,int *rp) noex {
	int		rs = SR_FAULT ;
	if (sp && rp) {
	    rs = SR_OK ;
	    if (sl < 0) sl = strlen(sp) ;
	    try {
	        string	sbuf(sp,sl) ;
		{
		    cchar	*raw = sbuf.c_str() ;
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
}
/* end subroutine (cfnum) */

void showterms(cchar *terms) noexcept {
	cint		cols = 8 ;
	int		cnt = 0 ;
	for (int ch = 0 ; ch < tablen ; ch += 1) {
	    if (BATSTB(terms,ch)) {
		if (cnt >= cols) {
		    printf("\n") ;
		    cnt = 0 ;
		}
		if (ch == 0) {
		    printf(" NUL") ;
		} else if (strchr("\t\n\r\f\v\b",ch)) {
		    cchar	*ostr = nullptr ;
		    switch (ch) {
		    case '\t':
			ostr = "HT" ;
			break ;
		    case '\n':
			ostr = "NL" ;
			break ;
		    case '\r':
			ostr = "CR" ;
			break ;
		    case '\f':
			ostr = "FF" ;
			break ;
		    case '\v':
			ostr = "VT" ;
			break ;
		    case '\b':
			ostr = "BS" ;
			break ;
		    } /* end switch */
		    if (ostr) {
		        printf(" %s",ostr) ;
		    }
		} else {
		    if (ch < 0x20) {
		       printf(" \\x%02X",ch) ;
		    } else if (ch == 0x20) {
		       printf(" SP") ;
		    } else if (ch == 0xA0) {
		       printf(" NBSP") ;
		    } else {
		       printf(" %c",ch) ;
		    }
		} /* end if (special) */
		cnt += 1 ;
	    } /* end if (hit) */
	} /* end for */
	if (cnt > 0) printf("\n") ;
}
/* end subroutine (showterms) */


