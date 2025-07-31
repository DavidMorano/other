/* mktofc_main SUPPORT (mktofc) */
/* charset=ISO8859-1 */
/* lang=C++20 (conformance reviewed) */

/* Make To Folded Case */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-06-29, David A­D­ Morano
	This subroutine was written for Rightcore Network Services
	(RNS).

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

  	Description:
	This program makes a C-language fragment that is an array
	of unsigned characters that constitutes a translation table
	used to fold characters down to their base case (or character).
	This is especially necessary for folding 'ISO-Ltin-1'
	characters down to the representative regular ASCII capital
	character.

	Program-usage:
	$ mktofc < foldcase.txt > tofc.chars

*******************************************************************************/

#include	<envstandards.h>	/* must be ordered first to configure */
#include	<sys/param.h>
#include	<climits>		/* |UCHAR_MAX| */
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<cstdio>
#include	<cstring>
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>
#include	<ascii.h>
#include	<sfx.h>			/* |sfnext(3uc)| */
#include	<ischarx.h>		/* |isalnumlatin(3uc)| */
#include	<localmisc.h>


/* local defines */

#ifndef	LINEBUFLEN
#define	LINEBUFLEN	2048
#endif

#ifndef	NTAB
#define	NTAB		8		/* number of columns in a TAB */
#endif


/* external subroutines */


/* external variables */

extern "C" {
    extern int	freadln(FILE *,char *,int) noex ;
}


/* local structures */


/* forward references */

static int	printout(FILE *,const uchar *) noex ;


/* local variables */

cint		nchars = (UCHAR_MAX + 1) ;


/* exported variables */


/* exported subroutines */

int main(int,mainv,mainv) {
	FILE		*ifp = stdin ;
	FILE		*ofp = stdout ;
	cint		llen = LINEBUFLEN ;
	uchar		a[nchars] ;
	char		lbuf[LINEBUFLEN + 1] ;

	for (int i = 0 ; i < nchars; i += 1) {
	    a[i] = char(i) ;
	}

	for (int len ; (len = freadln(ifp,lbuf,llen)) > 0 ; ) {
	    if (lbuf[len - 1] == '\n') len -= 1 ;
	    if ((len > 0) && (lbuf[0] != '#')) {
	        cchar	*lp = lbuf ;
	        int	ll = len ;
		int	ch_base = UCHAR_MAX ;
		cchar	*cp ;
	        for (int cl,i = 0 ; (cl = sfnext(lp,ll,&cp)) > 0 ; ) {
		    cint	ch = (cp[0] & UCHAR_MAX) ;
		    if (isalnumlatin(ch)) {
		        if (i++ == 0) {
			    ch_base = ch ;
		        } else {
		            a[ch] = char(ch_base) ;
		        }
		    } /* end if (alnum) */
		    ll -= intconv((cp + cl) - lp) ;
		    lp = (cp + cl) ;
	        } /* end for */
	    } /* end if (non-empty line) */
	} /* end for (readling lines) */

	printout(ofp,a) ;
}
/* end subroutine (main) */


/* local subroutines */

static int printout(FILE *ofp,const uchar *a) noex {
	cint		ntab = NTAB ;	/* number of numeric columns */
	cchar		fmt[] = "0x%02x%s" ;
	fprintf(ofp,"const unsigned char char_tofc[] = %c\n",CH_LBRACE) ;
	for (int i = 0 ; i < (nchars / ntab) ; i += 1) {
	    int		order ;
	    fprintf(ofp,"\t") ;
	    for (int j = 0 ; j < ntab ; j += 1) {
	        if (j > 0) {
	            fprintf(ofp," ") ;
		}
		order = ((i * ntab) + j) ;
	        fprintf(ofp,fmt,a[order],((order != UCHAR_MAX) ? "," : "")) ;
	    } /* end for */
	    fprintf(ofp,"\n") ;
	} /* end for */
	fprintf(ofp,"%c ;\n", CH_RBRACE) ;
	fflush(ofp) ;
	return 0 ;
}
/* end subroutine (printout) */


