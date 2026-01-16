/* hasx HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* does a counted c-string some characteristic? */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-11-01, David A­D­ Morano
	This subroutine was written for Rightcore Network Services.

*/

/* Copyright (c) 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	HASX_INCLUDE
#define	HASX_INCLUDE


#include	<envstandards.h>	/* MUST be first to configure */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<matstr.h>		/* |matstr(3uc)| used below */
#include	<stdfnames.h>		/* |stdfnames| used below */

#include	<hasall.h>
#include	<hasclass.h>
#include	<haslead.h>
#include	<hasmodname.h>
#include	<hasnon.h>
#include	<hasnot.h>
#include	<hasonly.h>
#include	<hasprint.h>
#include	<hasprime.h>
#include	<hasuniq.h>
#include	<hasvarprefix.h>


EXTERNC_begin

static inline bool hascontent(cchar *fp,int fl) noex {
	return hasnotempty(fp,fl) ;
}
static inline bool hasfnamespecial(cchar *fp,int fl) noex {
	return (matstr(stdfnames,fp,fl) >= 0) ;
}
static inline bool hasValidMagic(cchar *sp,int sl,cchar *ms) noex  {
	return hasvalidmagic(sp,sl,ms) ;
}
static inline bool hasEOH(cchar *sp,int sl) noex {
	return haseoh(sp,sl) ;
}
static inline bool hasMeAlone(cchar *sp,int sl) noex {
	return hasmealone(sp,sl) ;
}
static inline bool hasDotDir(cchar *sp,int sl) noex {
	return hasdots(sp,sl) ;
}
static inline bool hasINET4AddrStr(cchar *sp,int sl) noex {
	return hasinet4addrstr(sp,sl) ;
}

EXTERNC_end


#endif /* HASX_INCLUDE */


