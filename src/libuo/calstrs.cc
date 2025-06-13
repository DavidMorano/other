/* calstrs SUPPORT */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* calendar strings */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-02-01, David A­D­ Morano
	This code was originally written.

	= 2018-03-13, David A­D­ Morano
	I prefixed the exported symbols w/ 'calstrs_'.

*/

/* Copyright © 1998,2018 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

  	Data:
	calstrs

	Description:
	We provide a database of various common calendar strings.

*******************************************************************************/

#include	<envstandards.h>	/* MUST be first to configure */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>

#include	"calstrs.h"


/* local defines */


/* external subroutines */


/* external variables */


/* local structures */


/* forward references */


/* local variables */


/* exported variables */

cpcchar	calstrs_months[] = {
	"January", 
	"February", 
	"March", 
	"April", 
	"May", 
	"June", 
	"July", 
	"August", 
	"September", 
	"October", 
	"November", 
	"December", 
	nullptr
} ;

cpcchar	calstrs_days[] = {
	"Sunday", 
	"Monday", 
	"Tuesday", 
	"Wednesday", 
	"Thursday", 
	"Friday", 
	"Saturday", 
	nullptr
} ;

cpcchar	calstrs_daytypes[] = {
	"First", 
	"Second", 
	"Third", 
	"Fourth", 
	"Fifth", 
	"Last", 
	nullptr
} ;


/* exported subroutines */


/* local subroutines */


