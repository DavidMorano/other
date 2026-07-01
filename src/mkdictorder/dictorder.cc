/* dictorder_main (dictorder) */
/* charset=ISO8859-1 */
/* lang=C++20 (conformance reviewed) */


/* revision history:

	= 2000-05-14, David A­D­ Morano
	Originally written for Rightcore Network Services.

*/

/* Copyright © 2000 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

  	Description:
	Just print out 256 characters!

*******************************************************************************/

#include	<envstandards.h>	/* ordered first to configure */
#include	<sys/types.h>
#include	<sys/param.h>
#include	<cstddef>
#include	<cstdlib>
#include	<cstdio>
#include	<clanguage.h>
#include	<usysbase.h>
#include	<ascii.h>
#include	<localmisc.h>

#pragma		GCC dependency		"mod/libutil.ccm"
#pragma		GCC dependency		"mod/ureserve.ccm"

import libutil ;			/* |lenstr(3u)| */
import ureserve ;

/* local defines */


/* external subroutines */


/* external variables */


/* forward references */


/* local variables */


/* exported variables */


/* exported subroutines */

int main(int,con mainv,con mainv) {
	FILE		*fp = stdout ;
	int		n ;
	int		c ;

	for (int c = 0 ; c < 64 ; c += 1) {
	   if (isalnumlatin(c)) {
	       fprintf(fp,"%c\n",c) ;
	   }
	} /* end for */

	n = 32 ;
	for (int c = 64 ; c < (128-n) ; c += 1) {
	   if (isalnumlatin(c)) {
	       fprintf(fp,"%c\n",c) ;
	   }
	   if (isalnumlatin(c+n)) {
	       fprintf(fp,"%c\n",(c+n)) ;
	   }
	} /* end for */

	n = 32 ;
	c = (128+64) ;
	for (i = 0 ; i < n ; (i += 1, c += 1)) {
	   if (isalnumlatin(c)) {
	       fprintf(fp,"%c\n",c) ;
	   }
	   if (isalnumlatin(c+n)) {
	       fprintf(fp,"%c\n",(c+n)) ;
	   }
	} /* end for */
}
/* end subroutine (main) */


/* local subroutines */


