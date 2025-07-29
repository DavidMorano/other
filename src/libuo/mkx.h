/* mkx HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* provided are various general c-string creation subroutines */
/* version %I% last-modified %G% */


/* revision history:

	= 2000-05-14, David A-D- Morano
	This was written for Rightcore Network Services (RNS).
 
	= 2018-11-27, David A-D- Morano
	I created an inline version for the C language. We will
	likely do something for C++ at some other time.

	= 2023-10-23, David A-D- Morano
	At some point I added an inline (here) for the C++ language.

*/

/* Copyright © 2000,2018,23023 David A-D- Morano.  All rights reserved. */

#ifndef	MKX_INCLUDE
#define	MKX_INCLUDE


#include	<envstandards.h>	/* MUST be first to configure */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>

#include	<mkcexsync.h>
#include	<mkbasename.h>
#include	<mkgecosname.h>
#include	<mkquoted.h>
#include	<mkprogenv.h>
#include	<mkpath.h>
#include	<mknpath.h>
#include	<mkmagic.h>
#include	<mkfname.h>
#include	<mkfdfname.h>
#include	<mkdisplayable.h>
#include	<mknpath.h>
#include	<mkpath.h>
#include	<mkaddr.h>
#include	<mkpr.h>
#include	<mkxdisp.h>
#include	<mklineclean.h>
#include	<mkunique.h>
#include	<mkutmpid.h>
#include	<mkuuid.h>


EXTERNC_begin

extern int	mkrealname(char *,int,cchar *,int) noex ;
extern int	mknoise(uint *a,int n) noex ;
extern int	mkcdpath(char *,cchar *,int) noex ;
extern int	mkintfname(char *,cchar *,cchar *,cchar *) noex ;
extern int	mktagfname(char *,cchar *,cchar *,int) noex ;
extern int	mkmid(char *,int,cchar *,cchar *,pid_t,int) noex ;
extern int	mkonefrom(char *,int,cchar *,int) noex ;
extern int	mkmailname(char *,int,cchar *,int) noex ;
extern int	mkshlibname(char *,cchar *,int) noex ;
extern int	mkshmname(char *,cchar *,int,cchar *,int) noex ;
extern int	mksofname(char *,cchar *,cchar *,cchar *) noex ;
extern int	mkvarpath(char *,cchar *,int) noex ;
extern int	mkexpandpath(char *,cchar *,int) noex ;
extern int	mkuserpath(char *,cchar *,cchar *,int) noex ;
extern int	mkaltext(char *,cchar *,cchar *) noex ;
extern int	mkufname(char *,cchar *,cchar *) noex ;
extern int	mkmaildirtest(char *,cchar *,int) noex ;
extern int	mkfmtphone(char *,int,cchar *,int) noex ;
extern int	mkfingerquery(char *,int,int,cchar *,mainv) noex ;

EXTERNC_end

#ifdef	__cplusplus

inline int mkshmname(char *rbuf,cchar *ip,int il,cchar *sp) noex {
	return mkshmname(rbuf,ip,il,sp,-1) ;
}

#endif /* __cplusplus */


#endif /* MKX_INCLUDE */


