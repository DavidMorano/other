/* mapex SUPPORT */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* map status-return (SR) values to program exit-codes (EX) */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-11-01, David A­D­ Morano
	This subroutine was written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#include	<envstandards.h>	/* MUST be ordered first to configure */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>
#include	<exitcodes.h>
#include	<localmisc.h>

#include	"mapex.h"


/* local defines */


/* local namespaces */


/* local typedefs */


/* external subroutines */


/* external variables */


/* local structures */


/* forward references */


/* local variables */


/* exported variables */


/* exported subroutines */

int mapex(const mapex_map *mapexs,int rs) noex {
	int		ex = EX_UNKNOWN ;
	if (mapexs) {
	    ex = EX_OK ;
	    if (rs < 0) {
	        for (int i = 0 ; mapexs[i].rs ; i += 1) {
	            if (mapexs[i].rs == rs) {
			ex = mapexs[i].ex ;
			break ;
		    }
	        } /* end for */
	    } /* end if */
	} /* end if */
	return ex ;
}
/* end subroutine (mapex) */


