/* ucopen HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* additional UNIX® support */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-03-21, David A­D­ Morano
	This module was originally written.

	= 2017-08-01, David A­D­ Morano
	Updated for lack of interfaces in Apple-Darwin

*/

/* Copyright © 1998,2017 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

  	Name:
	ucopen

	Description:
	Additional (or supplemental) support for UNIX® limits.

*******************************************************************************/

#ifndef	UCOPEN_INCLUDE
#define	UCOPEN_INCLUDE


#include	<envstandards.h>	/* ordered first to configure */
#include	<sys/types.h>		/* POSIX |mode_t| ? */
#include	<sys/socket.h>		/* POSIX */
#include	<unistd.h>		/* POSIX */
#include	<fcntl.h>		/* POSIX */
#include	<clanguage.h>		/* LIBU */
#include	<usysbase.h>		/* LIBU */

#include	<ucopeninfo.h>
#include	<ucopenprime.h>
#include	<ucopensys.h>
#include	<ucopensysdb.h>
#include	<ucopenxsvc.h>
#include	<ucpipe.h>
#include	<ucclose.h>


EXTERNC_begin

extern int uc_accepte		(int,SOCKADDR *,int *,int) noex ;

extern int uc_create		(cchar *,mode_t) noex ;
extern int uc_createfile	(cchar *,mode_t) noex ;
extern int uc_openex		(cchar *,int,mode_t,int,int) noex ;
extern int uc_opensocket	(cchar *,int,int) noex ;
extern int uc_openproto		(cchar *,int,int,int) noex ;
extern int uc_openpass		(cchar *,int,int,int) noex ;
extern int uc_openuser		(cchar *,cchar *,int,mode_t,int) noex ;
extern int uc_openprog		(cchar *,int,con mainv,con mainv) noex ;
extern int uc_openprogerr	(cchar *,int,con mainv,con mainv,int *) noex ;
extern int uc_opendialer	(cchar *,cchar *,int,mode_t,
			con mainv,con mainv,int) noex ;
extern int uc_opendev		(cchar *,int,mode_t,con mainv,int,int) noex ;
extern int uc_openpt		(int) noex ;
extern int uc_openshm		(cchar *,int,mode_t) noex ;
extern int uc_openshmto		(cchar *,int,mode_t,int) noex ;

EXTERNC_end


#endif /* UCOPEN_INCLUDE */


