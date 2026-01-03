/* permx HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* stat-perm (Status-Permissions) */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-04-05, David A­D­ Morano
	This module was adapted from assembly lanauge.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

/******************************************************************************

  	Names:

	Description:
	We check for specified permissions on a file given an IDS
	object (for the user to check permissions for) and the
	|stat(2)| of the file in question.

******************************************************************************/

#ifndef	PERMX_INCLUDE
#define	PERMX_INCLUDE


#include	<envstandards.h>	/* MUST be first to configure */
#include	<sys/types.h>		/* UNIX® system types */
#include	<sys/stat.h>
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>
#include	<ids.h>
#include	<vecstr.h>


EXTERNC_begin

extern int permid(ids *,ustat *,int) noex ;
extern int perm(cchar *,uid_t,gid_t,const gid_t *,int) noex ;
extern int permf(int,uid_t,gid_t,const gid_t *,int) noex ;
extern int permsched(mainv,vecstr *,char *,int,cc *,mode_t) noex ;

EXTERNC_end


#endif /* PERMX_INCLUDE */


