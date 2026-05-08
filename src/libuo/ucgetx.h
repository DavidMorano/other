/* ucgetx HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* subroutines to retrieve various system information items */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-03-23, David A­D­ Morano
	This code was originally written.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	UCGETX_INCLUDE
#define	UCGETX_INCLUDE


#include	<envstandards.h>	/* ordered first to configure */
#include	<sys/types.h>		/* system types */
#include	<netdb.h>
#include	<clanguage.h>
#include	<usysbase.h>

#include	<ucgethost.h>
#include	<ucgetipnodeby.h>
#include	<ucgetloadavg.h>
#include	<ucgetnetname.h>
#include	<ucgetpid.h>
#include	<ucgetrandom.h>


EXTERNC_begin

extern int	uc_getarch	(char *,int)			noex ;
extern int	uc_getauid	()				noex ;
extern int	uc_getcwd	(char *,int)			noex ;
extern int	uc_gethostid	(ulong *)			noex ;
extern int	uc_gethostname	(char *,int)			noex ;
extern int	uc_getloadavg	(double *,int)			noex ;
extern int	uc_getnisdomain	(char *,int)			noex ;
extern int	uc_getpriority	(int,id_t,int *)		noex ;
extern int	uc_getprojdef	(PROJECT *,char *,int,cchar *)	noex ;
extern int	uc_getpuid	(pid_t)				noex ;
extern int	uc_getrusage	(int,RUSAGE *)			noex ;
extern int	uc_gettimeofday	(TIMEVAL *,void *)		noex ;

/* NETWORK SOCKADDR combined database */
extern int	uc_getnameinfo(CSOCKADDR *,int,char *,int,char *,int,int) noex ;

/* why was this missing? */
extern int	uc_getlogin(char *,int) noex ;


EXTERNC_end


#endif /* UCGETX_INCLUDE */


