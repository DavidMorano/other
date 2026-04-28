/* ucdescread HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* auxillary operating system support */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-03-21, David A­D­ Morano
	This module was originally written.

	= 2017-08-01, David A­D­ Morano
	Updated for lack of interfaces in MacOS Darwin

*/

/* Copyright © 1998,2017 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

  	Name:
	ucdescread

*******************************************************************************/

#ifndef	UCDESCREAD_INCLUDE
#define	UCDESCREAD_INCLUDE


#include	<envstandards.h>	/* ordered first to configure */
#include	<clanguage.h>
#include	<usysbase.h>
#include	<funcodes.h>


EXTERNC_begin

extern int	uc_read		(int,void *,int) noex ;
extern int	uc_reade	(int,void *,int,int,int) noex ;
extern int	uc_readlnto	(int fd,char *lbuf,int llen,int to) noex ;
extern int	uc_readln	(int fd,char *lbuf,int llen) noex ;
extern int	uc_readn	(int fd,void *abuf,int alen) noex ;

EXTERNC_end


#endif /* UCDESCREAD_INCLUDE */


