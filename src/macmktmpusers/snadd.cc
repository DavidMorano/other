/* snadd SUPPORT */
/* lang=C++20 */

/* append a counted c-string to a given counted c-string */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-11-01, David A­D­ Morano
	This subroutine was originally written.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Name:
	snaddx

	Description:
	Add some c-string to the end of an existing string.  Return the
	incremental amount added.

	Synopsis:
	int snaddx(char *rbuf,int rlen,int rl,int n,...) noex

	Arguments:
	rbuf		c-string buffer pointer
	rlen		c-string buffer length
	rl		length of buffer already filled
	sp		new c-string pointer to be added
	sl		new c-string length to be added

	Returns:
	>=0		the amount of additional string added to
			the existing c-string buffer
	<0		error (system-return)

*******************************************************************************/

#include	<envstandards.h>	/* MUST be first to configure */
#include	<stdarg.h>
#include	<usystem.h>

#include	"snadd.h"


/* local defines */


/* external subroutines */


/* external variables */


/* local structures */


/* foward references */


/* local variables */


/* exported variables */


/* exported subroutines */

int snaddx(char *rbuf,int rlen,int rl,int n,...) noex {
	va_list		ap ;
	int		rs = SR_FAULT ;
	int		len = 0 ;
	if (rbuf) {
	    va_begin(ap,n) ;
	    rs = SR_OK ;
	    for (int i = 0 ; (rs >= SR_OK) && (i < n) ; i += 1) {
		cchar	*sp = (char *) va_arg(ap,char *) ;
		rs = snadd(rbuf,rlen,rl,sp) ;
		len += rs ;
		rl += rs ;
	    } /* end for */
	    va_end(ap) ;
	} /* end if (non-null) */
	return (rs >= 0) ? len : rs ;
}
/* end subroutine (snadd) */


