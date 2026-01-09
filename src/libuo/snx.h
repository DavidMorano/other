/* snx HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* counted c-string operations */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-11-01, David A­D­ Morano
	This subroutine was written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	SNX_INCLUDE
#define	SNX_INCLUDE


#include	<envstandards.h>	/* ordered first to configure */
#include	<sys/types.h>		/* system types (see below) */
#include        <pthread.h>		/* |pthread_t| */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>

#include	<snabbr.h>
#include	<snadd.h>
#include	<sncpy.h>
#include	<sncpyx.h>		/* repeat? */
#include	<sncpyxc.h>		/* repeat? */
#include	<sncpyxw.h>		/* repeat? */
#include	<snflags.h>
#include	<snflagsx.h>
#include	<snxxx.h>
#include	<sntmtime.h>
#include	<snuuid.h>
#include	<snclean.h>
#include	<snwx.h>


EXTERNC_begin

extern int snxtilook(char *,int,int) noex ;
extern int sninetaddr(char *,int,int,cchar *) noex ;
extern int snshellunder(char *,int,pid_t,cchar *) noex ;
extern int snfilemode(char *,int,mode_t) noex ;
extern int sntid(char *,int,pthread_t) noex ;
extern int snrealname(char *,int,mainv,int) noex ;
extern int snloadavg(char *,int,uint,int,int,int) noex ;
extern int snkeyval(char *,int,cchar *,int,cchar *,int) noex ;

EXTERNC_end


#endif /* SNX_INCLUDE */


