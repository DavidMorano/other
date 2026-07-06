/* mklogid HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* make a log-ID c-string, suitable for logging purposes */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-09-01, David A­D­ Morano
	This was written for Rightcore Network Services (RNS).

*/

/* Copyright (c) 1998 David A-D- Morano.  All rights reserved. */

#ifndef	MKLOGID_INCLUDE
#define	MKLOGID_INCLUDE


#include	<envstandards.h>	/* MUST be first to configure */
#include	<clanguage.h>
#include	<usysbase.h>


EXTERNC_begin

extern int	mklogid(char *,int,cchar *,int,int) noex ;
extern int	mklogidpre(char *,int,cchar *,int) noex ;
extern int	mklogidsub(char *,int,cchar *,int) noex ;

EXTERNC_end

#ifdef	__cplusplus

namespace libuc {
    struct logdigmaxer {
	int mkdigmax() noex ;
	operator int () noex {
	    return mkdigmax() ;
	} ;
	int operator () () noex {
	    return mkdigmax() ;
	} ;
    } ; /* end struct (logdigmaxer) */
}

extern libuc::logdigmaxer	logdigmax ;

#endif /* __cplusplus */


#endif /* MKLOGID_INCLUDE */


