/* mapblock SUPPORT */
/* lang=C++20 */

/* implement a map container of blocks (of a given structure) */
/* version %I% last-modified %G% */


/* revision history:

	= 2011-04-12, David A­D­ Morano
	This subroutine was originally written.  Only the introduction
	of C++11 has allowed this (finally), due to the addition
	(in C++11) of |unordered_map(3c++)|.

*/

/* Copyright © 2011 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Description:
	mapblock

	Symopsis:
	int mapblock_start<typename K,typename Block>(int n = 0) noex

	Arguments:
	n		suggested starting size

	Returns:
	>=0		ok
	<		error (system-return)

*******************************************************************************/

#include	<envstandards.h>	/* ordered first to configure */
#include	<sys/types.h>
#include	<unistd.h>
#include	<cstring>
#include	<usystem.h>

#include	"mapblock.hh"


/* code comments */


/* local defines */


/* local typedefs */


/* external subroutines */


/* local structures */


/* forward references */


/* lcoal variables */


/* exported subroutines */


