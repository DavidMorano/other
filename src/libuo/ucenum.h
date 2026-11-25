/* ucenum HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* UNIX® C-language system database access (UCENUM) */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-03-23, David A­D­ Morano
	This code was originally written.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

  	Group:

	Description:
	These subroutines (below) are part of the collection of
	subroutine groups that try to form a common thread-safe
	interface to the system databases.  The managed system
	databaes are:

	+ passwd
	+ shadow
	+ userattr
	+ group
	+ project
	+ protocols
	+ networks
	+ hosts
	+ services

*******************************************************************************/

#ifndef	UCENUM_INCLUDE
#define	UCENUM_INCLUDE


#include	<envstandards.h>	/* ordered first to configure */
#include	<clanguage.h>		/* LIBU */
#include	<usysbase.h>		/* LIBU */

#include	<ucenumpw.h>
#include	<ucenumsp.h>
#include	<ucenumua.h>
#include	<ucenumgr.h>
#include	<ucenumpj.h>

#include	<ucenumpr.h>
#include	<ucenumnw.h>
#include	<ucenumho.h>
#include	<ucenumsv.h>


#endif /* UCENUM_INCLUDE */


