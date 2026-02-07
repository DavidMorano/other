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

/*******************************************************************************

	Object:
	strmgr

	Description:
	This object performs some simple string composition management.
	The caller supplies a destination (result) string-buffer
	(with a length), and then using "builder" methods composes
	what the caller wants in the result (a composed string).

*******************************************************************************/

#ifndef	STRMGR_INCLUDE
#define	STRMGR_INCLUDE


#include	<envstandards.h>	/* MUST be first to configure */
#include	<clanguage.h>
#include	<usysbase.h>


#define	STRMGR		struct strmgr_head


struct strmgr_head {
	char		*dbuf ;
	int		dlen ;
	int		dl ;
} ; /* end struct (strmgr_head) */

#ifdef	__cplusplus
enum strmgrmems {
    	strmgrmem_avail,
	strmgrmem_rem,
	strmgrmem_len,
	strmgrmem_reset,
	strmgrmem_finish,
	strmgrmem_overlast
} ; /* end enum (strmgrmems) */
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
	strmgr_co	len ;
	strmgr_co	reset ;
	strmgr_co	finish ;
	strmgr() noex {
	    avail	(this,strmgrmem_avail) ;
	    rem		(this,strmgrmem_rem) ;
	    len		(this,strmgrmem_len) ;
	    reset	(this,strmgrmem_reset) ;
	    finish	(this,strmgrmem_finish) ;
	    dbuf = nullptr ;
	} ; /* end ctor */
	strmgr(const strmgr &) = delete ;
	strmgr &operator = (const strmgr &) = delete ;
	int start(char *,int = -1) noex ;
	int str(cchar *,int = -1) noex ;
	int chr(int) noex ;
	int get(ccharpp) noex ;
	void dtor() noex ;
	operator int () noex ;
	destruct strmgr() {
	    if (dbuf) dtor() ;
	} ;
} ; /* end struct (strmgr) */
#else	/* __cplusplus */
typedef STRMGR		strmgr ;
#endif /* __cplusplus */

EXTERNC_begin

extern int strmgr_start(strmgr *,char *,int) noex ;
extern int strmgr_avail(strmgr *) noex ;
extern int strmgr_rem(strmgr *) noex ;
extern int strmgr_lenrem(strmgr *) noex ;
extern int strmgr_str(strmgr *,cchar *,int) noex ;
extern int strmgr_chr(strmgr *,int) noex ;
extern int strmgr_get(strmgr *,ccharpp) noex ;
extern int strmgr_len(strmgr *) noex ;
extern int strmgr_reset(strmgr *) noex ;
extern int strmgr_finish(strmgr *) noex ;

EXTERNC_end


#endif /* STRMGR_INCLUDE */


