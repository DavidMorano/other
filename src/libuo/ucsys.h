/* ucsys HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* UCSYS object management */
/* version %I% last-modified %G% */


/* revision history:

	= 2001-04-11, David A-D- Morano
	This subroutine was written for Rightcore Network Services.

*/

/* Copyright © 2001 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

  	Super-Group:
	ucsys

	Description:
	These subroutine groups provide access to the UNIX® system
	databases for UNIX® accounts and network entities.  The
	following groups are provied:
		ucsyspw		UNIX® 'passwd'		
		ucsyssp		UNIX® 'shadow'		
		ucsysgr		UNIX® 'group'		
		ucsyspj		UNIX® 'project'		
		ucsyspr		INET 'protocols'	
		ucsysnw		INET 'networks'
		ucsysho		INET 'hosts'		
		ucsyssv		INET 'services'

*******************************************************************************/

#ifndef	UCSYS_INCLUDE
#define	UCSYS_INCLUDE


#include	<envstandards.h>	/* ordered first to configure */
#include	<clanguage.h>
#include	<usysbase.h>

#include	<ucsys_sup.h>

#include	<spwd.h>		/* <- special */
#include	<userattr.h>		/* <- special */

#include	<ucsyspw.h>
#include	<ucsyssp.h>
#include	<ucsysgr.h>
#include	<ucsyspj.h>

#include	<ucsyspr.h>
#include	<ucsysnw.h>
#include	<ucsysho.h>
#include	<ucsyssv.h>


#endif /* UCSYS_INCLUDE */


