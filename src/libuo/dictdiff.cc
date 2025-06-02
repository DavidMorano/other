/* dictdiff SUPPORT */
/* encoding=ISO8859-1 */
/* lang=C++20 */

/* compare two dictionary characters (like for a dictionary) */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-11-01, David A­D­ Morano
	This code was originally written.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

/******************************************************************************

	Name:
	dictdiff

	Description:
	This subroutine essentially compares two dictionary characters.

	Synopsis:
	int dictdiff(int ch1,int ch2) noex

	Arguments:
	ch1	one character
	ch2	another character

	Returns:
	>0	the first dictionary character is bigger than the second
	0	both dictionary characters are equal
	<0	the first dictionary character is less than the second

******************************************************************************/

#include	<envstandards.h>	/* MUST be first to configure */
#include	<climits>		/* |UCHAR_MAX| */
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<toxc.h>
#include	<localmisc.h>

#include	"dictdiff.h"


/* local defines */


/* external subroutines */


/* external variables */


/* local structures */


/* forward references */


/* local variables */


/* exported variables */


/* exported subroutines */

int dictdiff(int ch1,int ch2) noex {
	int		rc ;
	ch1 &= UCHAR_MAX ;
	ch2 &= UCHAR_MAX ;
	if ((rc = (tofc(ch1) - tofc(ch2))) == 0) {
	    rc = (ch1 - ch2) ;
	}
	return rc ;
}
/* end subroutine (dictdiff) */


