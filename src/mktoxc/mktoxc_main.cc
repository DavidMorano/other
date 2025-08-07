/* mktoxc_main (mktoxc) */
/* charset=ISO8859-1 */
/* lang=C++20 (conformance reviewed) */

/* create translation arrays for character case conversion */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-06-29, David A­D­ Morano
	This subroutine was written for Rightcore Network Services
	(RNS).

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

  	Description:
	This program makes two arrays of characters, each 256
	characters long.  These arrays serve as translation tables
	for use in converting characters either to lower-case or
	to upper-case (each array serves one of these two functions).

*******************************************************************************/

#include	<envstandards.h>	/* must be ordered first to configure */
#include	<sys/param.h>
#include	<climits>		/* |UCHAR_MAX| + |CHAR_BIT| */
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<cstdio>
#include	<cstring>		/* |strcmp(3c)| */
#include	<usystem.h>
#include	<ascii.h>
#include	<baops.h>
#include	<char.h>
#include	<exitcodes.h>
#include	<localmisc.h>


/* local defines */

#ifndef	NTAB
#define	NTAB	8
#endif


/* external subroutines */


/* external variables */


/* local structures */


/* forward references */

static int	getpm(int,mainv) noex ;
static int	mklc() noex ;
static int	mkuc() noex ;
static int	printcharterms(ccharp) noex ;
static int	mktoxc(int) noex ;
static int	tolc(int) noex ;
static int	touc(int) noex ;


/* local variables */

enum progmodes {
    	progmode_mkcaseterms,
	progmode_mktoxc,
	progmode_overlast
} ; /* end enum (progmodes) */

constexpr cpcchar	prognames[] = {
    	[progmode_mkcaseterms]	= "mkcaseterms",
	[progmode_mktoxc] 	= "mktoxc",
	[progmode_overlast]	= nullptr
} ; /* end array (prognames) */

enum cases {
	case_tolower,
	case_toupper,
	case_overlast
} ; /* end enum (cases) */

cint		nchars = (UCHAR_MAX + 1) ;

static char	case_lower[nchars] ;
static char	case_upper[nchars] ;


/* exported variables */


/* exported subroutines */

int main(int argc,mainv argv,mainv) {
    	int		rs = SR_OK ;
	int		ex = EXIT_FAILURE ;
	if ((rs = getpm(argc,argv)) >= 0) {
	    cint	pm = rs ;
	    ex = EXIT_SUCCESS ;
    	    if (rs >= 0) rs = mklc() ;
    	    if (rs >= 0) rs = mkuc() ;
	    if (pm == progmode_mkcaseterms) {
		if (rs >= 0) rs = printcharterms(case_lower) ;
		if (rs >= 0) rs = printcharterms(case_upper) ;
	    } else if (pm == progmode_mktoxc) {
	        if (rs >= 0) rs = mktoxc(case_tolower) ;
	        if (rs >= 0) rs = mktoxc(case_toupper) ;
	    } else {
		ex = EXIT_FAILURE ;
	    } /* end if (progmode_mktoxc) */
	} /* end if (getpm) */
	if ((ex == EXIT_SUCCESS) && (rs < 0)) ex = EXIT_FAILURE ;
	return ex ;
} /* end subroutine (main) */


/* local subroutines */

static int getpm(int argc,mainv argv) noex {
    	int		pm = -1 ; /* used afterwards */
	if (argc > 0) {
	    if (cc *argz = argv[0] ; argz) {
    	        for (pm = 0 ; prognames[pm] ; pm += 1) {
		    cc *pn = prognames[pm] ;
		    if (strcmp(argz,pn) == 0) break ;
	        }
	    }
	}
    	return pm ;
} /* end subroutine (getpm) */

extern "C" {
    typedef bool (*mkxc_f)(int) noex ;
}

static int mklc() noex {
	int		rs = SR_OK ;
	for (int i = 0 ; i < nchars ; i += 1) {
	    if (char_islc(i)) {
	        baset(case_lower,i) ;
	    }
	}
	return rs ;
} /* end subroutine (mklc) */

static int mkuc() noex {
	int		rs = SR_OK ;
	for (int i = 0 ; i < nchars ; i += 1) {
	    if (char_isuc(i)) {
	        baset(case_upper,i) ;
	    }
	}
	return rs ;
} /* end subroutine (mklc) */

static int printcharterms(ccharp terms) noex {
    	cint		n = (nchars / CHAR_BIT) ;
    	int		rs = SR_OK ;
	for (int i = 0 ; i < n ; i += 1) {
	    if ((i % NTAB) == 0) {
		printf("\t") ;
	    } else {
		printf(" ") ;
	    }
	    printf("0x%02X,",terms[i]) ;
	    if ((i % NTAB) == (NTAB -1)) printf("\n") ;
	} /* end for */
	return rs ;
} /* end subroutine (printcharterms) */

static int mktoxc(int w) noex {
    	int		rs = SR_INVALID ;
	if ((w >= 0) && (w < case_overlast)) {
	cchar		*name ;
	rs = SR_OK ;
	switch (w) {
	case case_tolower:
	    name = "tolc" ;
	    break ;
	case case_toupper:
	    name = "touc" ;
	    break ;
	} /* end switch */

	printf("const unsigned char char_%s[] = %c\n",
	    name,CH_LBRACE) ;

	for (int i = 0 ; i < nchars ; i += 1) {
	    int		rc = 0 ;
	    switch (w) {
	    case case_tolower:
	        rc = tolc(i) ;
	        break ;
	    case case_toupper:
	        rc = touc(i) ;
	        break ;
	    } /* end switch */
	    if ((i & 7) == 0) printf("\t") ;
	    if ((i & 7) != 0) printf(" ") ;
	    if (i != UCHAR_MAX) {
		printf("0x%02x,",rc) ;
	    } else {
		printf("0x%02x",rc) ;
	    }
	    if ((i & 7) == 7) printf("\n") ;
	} /* end for */

	printf("%c ;\n", CH_RBRACE) ;

	} /* end if (valid) */
	return rs ;
}
/* end subroutine (mktoxc) */

static int tolc(int ch) noex {
    	if ((ch >= 'A') && (ch <= 'Z')) {
	    ch += 0x20 ;
	} else if ((ch >= 'À') && (ch <= 'Ö')) {
	    ch += 0x20 ;
	} else if ((ch >= 'Ø') && (ch <= 'Þ')) {
	    ch += 0x20 ;
	}
    	return ch ;
} /* end subroutine (mklc) */

static int touc(int ch) noex {
    	if ((ch >= 'a') && (ch <= 'z')) {
	    ch -= 0x20 ;
	} else if ((ch >= 'à') && (ch <= 'ö')) {
	    ch -= 0x20 ;
	} else if ((ch >= 'ø') && (ch <= 'þ')) {
	    ch -= 0x20 ;
	}
    	return ch ;
} /* end subroutine (mkuc) */


