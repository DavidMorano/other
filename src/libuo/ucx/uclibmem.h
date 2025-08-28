/* uclibmem HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* memory allocation facility (for library use) */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-11-01, David A­D­ Morano
	This subroutine was written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	UCLIBMEM_INCLUDE
#define	UCLIBMEM_INCLUDE


#include	<envstandards.h>	/* ordered first to configure */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>


EXTERNC_begin

extern int uc_libmemallocstrw(cchar *,int,cchar **) noex ;
extern int uc_libmemallocitem(cvoid *,int,voidpp) noex ;
extern int uc_libmemalloc(int,void *) noex ;
extern int uc_libmemvalloc(int,void *) noex ;
extern int uc_libmemcalloc(int,int,void *) noex ;
extern int uc_libmemrealloc(void *,int,void *) noex ;
extern int uc_libmemfree(void *) noex ;
extern int uc_libmemrsfree(int,void *) noex ;

EXTERNC_end


#endif /* UCLIBMEM_INCLUDE */


