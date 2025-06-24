/* strmgr HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* string manager */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-02-01, David A­D­ Morano
	This code was originally written.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	STRMGR_INCLUDE
#define	STRMGR_INCLUDE


#include	<envstandards.h>	/* MUST be first to configure */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>


#define	STRMGR		struct strmgr_head


struct strmgr_head {
	char		*dp ;
	int		dl ;
	int		dlen ;
} ;

#ifdef	__cplusplus
enum strmgrmems {
    	strmgrmem_avail,
	strmgrmem_rem,
	strmgrmem_finish,
	strmgrmem_overlast
} ;
struct strmgr ;
struct strmgr_co {
	strmgr		*op = nullptr ;
	int		w = -1 ;
	void operator () (strmgr *p,int m) noex {
	    op = p ;
	    w = m ;
	} ;
	operator int () noex ;
	int operator () () noex { 
	    return operator int () ;
	} ;
} ; /* end struct (strmgr_co) */
struct strmgr : strmgr_head {
	strmgr_co	avail ;
	strmgr_co	rem ;
	strmgr_co	finish ;
	strmgr() noex {
	    avail(this,strmgrmem_avail) ;
	    rem(this,strmgrmem_rem) ;
	    finish(this,strmgrmem_finish) ;
	} ;
	strmgr(const strmgr &) = delete ;
	strmgr &operator = (const strmgr &) = delete ;
	int start(char *,int = -1) noex ;
	int str(cchar *,int = -1) noex ;
	int chr(int) noex ;
	void dtor() noex ;
	destruct strmgr() {
	    if (dp) dtor() ;
	} ;
} ; /* end struct (strmgr) */
#else	/* __cplusplus */
typedef STRMGR		strmgr ;
#endif /* __cplusplus */


EXTERNC_begin

extern int strmgr_start(strmgr *,char *,int) noex ;
extern int strmgr_avail(strmgr *) noex ;
extern int strmgr_rem(strmgr *) noex ;
extern int strmgr_str(strmgr *,cchar *,int) noex ;
extern int strmgr_chr(strmgr *,int) noex ;
extern int strmgr_finish(strmgr *) noex ;

EXTERNC_end


#endif /* STRMGR_INCLUDE */


