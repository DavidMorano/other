/* repomanip_main SUPPORT (repos) */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* do something with a software repository */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-04-13, David A-D- Morano
	Originally written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */
/* Use is subject to license terms. */

/*******************************************************************************

	Description:
	This program reads a file and copies it to STDOUT, but it
	also changes the value of a key if it is first determined
	if a repository is being perused.

*******************************************************************************/

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
#include	<usupport.h>		/* LIBU |strn{x}(3u)| */
#include	<localmisc.h>		/* LIBU |LINEBUFLEN| */
#include	<libf.h>		/* LIBF */

#pragma		GCC dependency		"mod/libutil.ccm"

import libutil ;			/* |lenstr(3u)| */
import ulibvals ;			/* |ulibval(3u)| */

/* local defines */


/* imported namespaces */

using libu::strnchr ;			/* subroutine */
using libu::strwcmp ;			/* subroutine */


/* local typedefs */


/* external subroutines */


/* external variables */


/* local structures */


/* forward references */

local bool hmat(cchar *,int) noex ;
local bool rmat(cchar *,int,cchar *) noex ;


/* local variables */

cint		linebuflen = ulibval.maxline ;


/* exported variables */


/* exported subroutines */

int main(int argc,con mainv argv,con mainv) {
	FILE		*ifp = stdin ;
	FILE		*ofp = stdout ;
	cint		llen = linebuflen ;
	cint		ex = EXIT_SUCCESS ;
	int		rs ;
	bool		f = false ;
	char		lbuf[linebuflen +1] ;
	cchar		*name = "REPOS" ;
	if (argc > 1) {
	    name = argv[1] ;
	} /* end */
	while ((rs = freadln(ifp,lbuf,llen)) > 0) {
	    int	ll = lenstr(lbuf) ;
	    if (lbuf[ll-1] == '\n') {
	        ll -= 1 ;
	    } /* end */
	    if (hmat(lbuf,ll)) {
	        f = rmat(lbuf,ll,name) ;
	    } /* end */
 	    if (f) {
		if (strstr(lbuf,"enabled=1") != nullptr) {
		    fprintf(ofp,"enabled=0\n") ;
		} else {
		    fprintf(ofp,"%s",lbuf) ;
		}
	    } else {
		fprintf(ofp,"%s",lbuf) ;
	    } /* end */
	} /* end while (freadln) */
	if ((ex == EXIT_SUCCESS) && (rs < 0)) {
	    ex = EXIT_FAILURE ;
	} /* end if (error) */
	return ex ;
} /* end subroutine (main) */


/* local subrouines */

local bool hmat(cchar *sp,int sl) noex {
	bool		f = 0 ;
	if (cchar *tp = strnchr(sp,sl,'{')) ylikely {
	    sl -= intconv((tp + 1) - sp) ;
	    sp = (tp+1) ;
	    if ((tp = strnchr(sp,sl,'}')) != nullptr) {
		f = true ;
	    }
	}
	return f ;
} /* end subroutine (hmat) */

local bool rmat(cchar *sp,int sl,cchar *name) noex {
	bool		f = 0 ;
	if (cchar *tp = strnchr(sp,sl,'{')) ylikely {
	    cchar *cp = (tp + 1) ;
	    sl -= intconv((tp + 1) - sp) ;
	    sp = (tp+1) ;
	    if ((tp = strnchr(sp,sl,'}')) != nullptr) {
		cint cl = intconv(tp - sp) ;
		f = (strwcmp(name,cp,cl) == 0) ;
	    }
	}
	return f ;
} /* end subroutine (rmat) */


