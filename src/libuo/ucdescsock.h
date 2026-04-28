/* ucdescsock HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* string-index operations */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-11-01, David A­D­ Morano
	This subroutine was written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	UCDESCSOCK_INCLUDE
#define	UCDESCSOCK_INCLUDE


#include	<envstandards.h>	/* ordered first to configure */
#include	<clanguage.h>
#include	<usysbase.h>

#include	<ucsockprime.h>

EXTERNC_begin

extern int uc_sockjoin(int,SOCKADDR *,int,int,mode_t) noex ;
extern int uc_sockreuse(int) noex ;
local inline int uc_reuseaddr(int fd) noex {
    	return uc_sockreuse(fd) ;
}

EXTERNC_end


#endif /* UCDESCSOCK_INCLUDE */


