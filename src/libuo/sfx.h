/* sfx HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* various string-find operations */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-11-01, David A­D­ Morano
	This subroutine was written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	SFX_INCLUDE
#define	SFX_INCLUDE


#include	<envstandards.h>	/* ordered first to configure */
#include	<clanguage.h>
#include	<usysbase.h>

#include	<sfcasesub.h>
#include	<sfext.h>
#include	<sfnext.h>
#include	<sfxchr.h>
#include	<sfxbrk.h>
#include	<sfxname.h>
#include	<sfxsub.h>
#include	<sfweirdo.h>

EXTERNC_begin

extern int sfbaselib(cchar *,int,cchar **) noex ;
extern int sfbracketval(cchar *,int,cchar **) noex ;
extern int sfcontent(cchar *,int,cchar **) noex ;
extern int sfcookkey(cchar *,int,cchar **) noex ;
extern int sfdequote(cchar *,int,cchar **) noex ;
extern int sfkey(cchar *,int,cchar **) noex ;
extern int sfkeyval(cchar *,int,cchar *,cchar **) noex ;
extern int sflast(cchar *,int,int,cchar **) noex ;
extern int sfnamecomp(cchar *,int,cchar **) noex ;

extern int sfprogroot(cchar *,int,cchar **) noex ;
extern int sfshrink(cchar *,int,cchar **) noex ;
extern int sfskipwhite(cchar *,int,cchar **) noex ;
extern int sfsubstance(cchar *,int,cchar **) noex ;
extern int sfthing(cchar *,int,cchar *,cchar **) noex ;
extern int sfwhitedot(cchar *,int,cchar **) noex ;
extern int sfword(cchar *,int,cchar **) noex ;
extern int sfsign(cchar *,int,cchar **,bool *) noex ;
extern int sfcenter(cchar *,int,cchar *,cchar **) noex ;

static inline int sfchr(cchar *sp,int sl,int sch,cchar **rpp) noex {
	return sfochr(sp,sl,sch,rpp) ;
}
static inline int sfbrk(cchar *sp,int sl,cchar *ss,cchar **rpp) noex {
	return sfobrk(sp,sl,ss,rpp) ;
}
static inline int sfsub(cchar *sp,int sl,cchar *ss,cchar **rpp) noex {
    	return sfosub(sp,sl,ss,rpp) ;
}
static inline int nextfield(cchar *sp,int sl,cchar **rpp) noex {
	return sfnext(sp,sl,rpp) ;
}

EXTERNC_end


#endif /* SFX_INCLUDE */


