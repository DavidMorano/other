/* repomanip_main SUPPORT (repos) */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* test something */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-04-13, David A-D- Morano
	Originally written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */
/* Use is subject to license terms. */

#include	<envstandards.h>	/* ordered first to configure */
#include	<cstddef>		/* CSTD */
#include	<cstdlib>		/* CSTD */
#include	<cstring>		/* CSTD */
#include	<cstdio>		/* CSTD */
#include	<clanguage.h>		/* LIBU */
#include	<utypedefs.h>		/* LIBU */
#include	<utypealiases.h>	/* LIBU */
#include	<usysdefs.h>		/* LIBU */
#include	<usysrets.h>		/* LIBU */
#include	<strn.h>		/* POSIX® */
#include	<strwcmp.h>		/* POSIX® */
#include	<localmisc.h>		/* LIBU |LINEBUFLEN| */


/* local defines */

#ifndef	LINEBUFLEN
#define	LINEBUFLEN	2048
#endif

/* external subroutines */


/* forward references */

local bool hmat(cchar *,int) noex ;
local bool rmat(cchar *,int,cchar *) noex ;

/* exported subroutines */

int main(int argc,mainv argv,mainv) {
	FILE		*ifp = stdin ;
	FILE		*ofp = stdout ;
	cint		llen = LINEBUFLEN ;
	int		f = 0 ;
	char		lbuf[LINEBUFLEN+1] ;
	cchar		*name = "REPOS" ;
	if (argc > 1) {
	    name = argv[1] ;
	}
	while (fgets(lbuf,llen,ifp) != nullptr) {
	   int	ll = strlen(lbuf) ;
	   if (lbuf[ll-1] == '\n') ll -= 1 ;
	   if (hmat(lbuf,ll)) {
		f = rmat(lbuf,ll,name) ;
	   }
 	   if (f) {
		if (strstr(lbuf,"enabled=1") != nullptr) {
		    fprintf(ofp,"enabled=0\n") ;
		} else {
		    fprintf(ofp,"%s",lbuf) ;
		}
	   } else {
		fprintf(ofp,"%s",lbuf) ;
	    }
	} /* end while */
	return 0 ;
} /* end subroutine (main) */


/* local subrouines */

local bool hmat(cchar *sp,int sl) noex {
	bool		f = 0 ;
	if (cchar *tp ; (tp = strnchr(sp,sl,'{')) != nullptr) {
	    sl -= ((tp+1)-sp) ;
	    sp = (tp+1) ;
	    if ((tp = strnchr(sp,sl,'}')) != nullptr) {
		f = 1 ;
	    }
	}
	return f ;
} /* end */

local bool rmat(cchar *sp,int sl,cchar *name) noex {
	int		cl ;
	bool		f = 0 ;
	cchar		*cp ;
	if (cchar *tp ; (tp = strnchr(sp,sl,'{')) != nullptr) {
	    cp = (tp+1) ;
	    sl -= ((tp+1)-sp) ;
	    sp = (tp+1) ;
	    if ((tp = strnchr(sp,sl,'}')) != nullptr) {
		cl = (tp-sp) ;
		f = (strwcmp(name,cp,cl) == 0) ;
	    }
	}
	return f ;
} /* end subroutine (rmat) */


