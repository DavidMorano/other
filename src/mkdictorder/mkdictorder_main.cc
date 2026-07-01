/* mkditorder_main (mkdictorder) */
/* charset=ISO8859-1 */
/* lang=C++20 (conformance reviewed) */

/* make a translation table to provide dictionary-order for characters */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-06-29, David A­D­ Morano
	This subroutine was written for Rightcore Network Services
	(RNS).

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

  	Name:
	main

	Description:
	This program makes an array 256 short integers.  This array
	serves as a sort of translation table for use in finding
	the dictionary-collating-ordinal number of a latin character.
	The array is indexed by the latin character (an 8-bit clean
	character) and returns the short integer representing its
	dictionary-collating-ordinal number.

*******************************************************************************/

#include	<envstandards.h>	/* ordered first to configure */
#include	<climits>		/* |UCHAR_MAX| */
#include	<cstddef>
#include	<cstdlib>
#include	<cstdio>
#include	<clanguage.h>
#include	<usysbase.h>
#include	<ascii.h>
#include	<localmisc.h>
#include	<libf.h>		/* LIBF |freadln(3f)| */

#pragma		GCC dependency		"mod/libutil.ccm"
#pragma		GCC dependency		"mod/ureserve.ccm"

import libutil ;			/* |lenstr(3u)| */
import ureserve ;			/* |lenstr(3u)| */

/* local defines */


/* external subroutines */


/* forward references */

local int	printout(FILE *,con short *) noex ;


/* local variables */

cint		mktablen = (UCHAR_MAX + 1) ;


/* exported variables */


/* exported subroutines */

int main(int,con mainv,con mainv) {
	FILE		*ifp = stdin ;
	FILE		*ofp = stdout ;
	cint		llen = LINEBUFLEN ;
	int		ch ;
	int		sz ;
	int		len ;
	short		order = 100 ;
	short		a[256] ;
	char		lbuf[LINEBUFLEN + 1] ;

	sz = 256 * szof(short) ;
	memclear(a,sz) ;

/* load some special characters (special handling) */

	{
	    cchar	*sp = "­-'`´  " ;
	    for (int i = 0 ; sp[i] ; i += 1) {
		ch = MKCHAR(sp[i]) ;
		a[ch] = order++ ;
	    }
	} /* end block */

/* load all other characters according to the standard-input file */

	order = 1000 ;
	while ((len = freadln(ifp,lbuf,llen)) > 0) {
	    if (lbuf[len-1] == '\n') len -= 1 ;
	    if ((len == 0) || (lbuf[0] == '#')) continue ;
	    for (int i = 0 ; i < len ; i += 1) {
		ch = MKCHAR(lbuf[i]) ;
		if (isalnumlatin(ch)) {
		    a[ch] = order++ ;
		}
	    } /* end for */
	} /* end while (readling lines) */

/* print out the results */

	printout(ofp,a) ;

	return 0 ;
}
/* end subroutine (main) */


/* local subroutines */

local int printout(FILE *ofp,cshort *a) noex {
	fprintf(ofp,"const short	char_dictorder[] = %c\n",CH_LBRACE) ;
	for (int i = 0 ; i < 32 ; i += 1) {
	    int order ;
	    fprintf(ofp,"\t") ;
	    for (int j = 0 ; j < 8 ; j += 1) {
	        if (j > 0) {
	            fprintf(ofp," ") ;
		}
		order = ((i * 8) + j) ;
	        fprintf(ofp,"0x%04x%s",
			a[order],((order != UCHAR_MAX) ? "," : "")) ;

	    } /* end for */
	    fprintf(ofp,"\n") ;
	} /* end for */
	fprintf(ofp,"%c ;\n", CH_RBRACE) ;
	return 0 ;
}
/* end subroutine (printout) */


