/* pathxx HEADER */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* add a c-string to the end of an existing path c-string */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-11-01, David A­D­ Morano
	This subroutine was written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	PATHXX_INCLUDE
#define	PATHXX_INCLUDE
#ifdef	__cplusplus


#include	<envstandards.h>	/* MUST be first to configure */
#include	<clanguage.h>		/* LIBU */
#include	<usysbase.h>		/* LIBU */


namespace pathxx {
    extern int pathnaddw(char *,int,int,cchar *,int = -1) noex ;
    inline int pathnadd(char *pbuf,int plen,int pl,cchar *sp,int sl = -1) noex {
	return pathnaddw(pbuf,plen,pl,sp,sl) ;
    } /* end subroutine */
    template<typename ... Args>
    inline int pathnadd(char *dbuf,int dl,Args ... args) noex {
	cint		na = npack(Args) ;
	return pathnaddx(dbuf,dl,na,args ...) ;
    } /* end subroutine-template */
} /* end namespace (pathxx) */

namespace pathxx {
    extern int pathaddw(char *,int,cchar *,int = -1) noex ;
    inline int pathadd(char *pbuf,int pl,cchar *sp,int sl = -1) noex {
	return pathaddw(pbuf,pl,sp,sl) ;
    } /* end subroutine */
    template<typename ... Args>
    inline int pathadd(char *dbuf,int dl,Args ... args) noex {
	cint		na = npack(Args) ;
	return pathaddx(dbuf,dl,na,args ...) ;
    } /* end subroutine-template */
} /* end namespace (pathxx) */


#endif /* __cplusplus */
#endif /* PATHXX_INCLUDE */


