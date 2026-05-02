/* ucdesclock HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* sister subroutine to |uc_lockf(3uc)| */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-02-01, David A­D­ Morano
	This code was originally written.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	UCESCLOCK_INCLUDE
#define	UCESCLOCK_INCLUDE


#include	<envstandards.h>	/* MUST be first to configure */
#include	<sys/types.h>		/* |off_t| */
#include	<clanguage.h>
#include	<usysbase.h>


EXTERNC_begin

extern int	uc_lockf(int,int,off_t) noex ;
extern int	uc_lockfile(int,int,off_t,off_t,int) noex ;
extern int	uc_locktail(int,int,int,int) noex ;

static inline int uc_lockend(int fd,int f_lock,int f_read,int to) noex {
    	return uc_locktail(fd,f_lock,f_read,to) ;
}

EXTERNC_end


#endif /* UCESCLOCK_INCLUDE */


