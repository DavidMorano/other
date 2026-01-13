/* uckvamatch HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* interface component for UNIX® library-3c */
/* virtual per-process timer management */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-04-13, David A-D- Morano
	Originally written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Name:
	uc_kvamatch

	Description:
	This subroutine finds a keyname (if it exists) in a KVA-type
	of object.

	Synopsis:
	int uc_kvamatch(kva_t *kva,cchar *keyname,cchar **rpp) noex

	Arguments:
	kva		key-value handle
	keyname		key-name to lookup
	rpp		pointer to pointer to results

	Returns:
	>=0		key-name found and this is the length (including zero)
	SR_NOTFOUND	key-name not found
	SR_NOSYS	function not implemented on this system
	<0		error (system-return)

*******************************************************************************/

#ifndef	UCKVAMATCH_INCLUDE
#define	UCKVAMATCH_INCLUDE


#include	<envstandards.h>	/* ordered first to configure */
#include	<secdb.h>		/* <- the money shot |kva_t| */
#include	<clanguage.h>
#include	<usysbase.h>


EXTERNC_begin

extern int	uc_kvamatch(kva_t *,cchar *,cchar **) noex ;

EXTERNC_end


#endif /* UCKVAMATCH_INCLUDE */


