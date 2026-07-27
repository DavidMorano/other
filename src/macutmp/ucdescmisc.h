/* ucdescmisc HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* additional UNIX® support */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-03-21, David A­D­ Morano
	This module was originally written.

	= 2017-08-01, David A­D­ Morano
	Updated for lack of interfaces in Apple-Darwin.

*/

/* Copyright © 1998,2017 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

  	Names:
	uc_seeko
	uc_fcntl

  	Description:
	Additional (or supplemental) support for UNIX® limits.

*******************************************************************************/

#ifndef	UCDESCMISC_INCLUDE
#define	UCDESCMISC_INCLUDE


#include	<envstandards.h>	/* ordered first to configure */
#include	<unistd.h>		/* types? */
#include	<fcntl.h>		/* types? */
#include	<stddef.h>		/* CSTD */
#include	<stdlib.h>		/* CSTD */
#include	<clanguage.h>		/* LIBU */
#include	<usysbase.h>		/* LIBU */

#include	<ucdescmanip.h>
#include	<ucdescmiscers.h>
#include	<uccloseonexec.h>
#include	<ucfminmod.h>
#include	<ucfpassfd.h>
#include	<ucfstat.h>
#include	<ucfsync.h>
#include	<ucftruncate.h>
#include	<ucndelay.h>
#include	<ucnonblock.h>
#include	<ucpeek.h>
#include	<ucsetappend.h>
#include	<ucttyname.h>
#include	<ucclose.h>


#endif /* UCDESCMISC_INCLUDE */


