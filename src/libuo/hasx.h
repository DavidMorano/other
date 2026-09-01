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
#include	<clanguage.h>		/* LIBU */
#include	<utypedefs.h>		/* LIBU */
#include	<utypealiases.h>	/* LIBU */
#include	<usysdefs.h>		/* LIBU */

#include	<hasall.h>
#include	<hasbasename.h>
#include	<hasclass.h>
#include	<hasend.h>
#include	<haseol.h>
#include	<hasinet.h>
#include	<haslead.h>
#include	<hasmodname.h>
#include	<hasneed.h>
#include	<hasnon.h>
#include	<hasnot.h>
#include	<hasonly.h>
#include	<hasourmjd.h>
#include	<haspath.h>
#include	<hasprint.h>
#include	<hasmisc.h>
#include	<hasuniq.h>

#include	<hasallof.h>		/* special (different return) */


EXTERNC_begin

local inline bool hascontent(cchar *fp,int fl) noex {
	return hasnotempty(fp,fl) ;
}
local inline bool hasValidMagic(cchar *sp,int sl,cchar *ms) noex  {
	return hasvalidmagic(sp,sl,ms) ;
}
local inline bool hasEOH(cchar *sp,int sl) noex {
	return haseoh(sp,sl) ;
}
local inline bool hasDotDir(cchar *sp,int sl) noex {
	return hasdots(sp,sl) ;
}
local inline bool hasINET4AddrStr(cchar *sp,int sl) noex {
	return hasinet4addrstr(sp,sl) ;
}

EXTERNC_end


#endif /* HASX_INCLUDE */


