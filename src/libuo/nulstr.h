/* nulstr HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* assert a NUL-terminated string */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-02-01, David A­D­ Morano
	This code was originally written.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	NULSTR_INCLUDE
#define	NULSTR_INCLUDE


#include	<envstandards.h>	/* MUST be first to configure */
#include	<clanguage.h>
#include	<usysbase.h>


#define	NULSTR		struct nulstr_head
#define	NULSTR_SHORTLEN	128		/* "string length optimization" */


struct nulstr_head {
	cchar		*as ;	/* allocated string */
	char		buf[NULSTR_SHORTLEN+1] ;
} ;

#ifdef	__cplusplus
enum nulstrmems {
	nulstrmem_finish,
	nulstrmem_overlast
} ;
struct nulstr ;
struct nulstr_co {
	nulstr		*op = nullptr ;
	int		w = -1 ;
	void operator () (nulstr *p,int m) noex {
	    op = p ;
	    w = m ;
	} ;
	operator int () noex ;
	int operator () () noex { 
	    return operator int () ;
	} ;
} ; /* end struct (nulstr_co) */
struct nulstr : nulstr_head {
	nulstr_co	finish ;
	nulstr() noex {
	    finish(this,nulstrmem_finish) ;
	    as = nullptr ;
	} ;
	nulstr(const nulstr &) = delete ;
	nulstr &operator = (const nulstr &) = delete ;
	int start(cchar *,int,cchar **) noex ;
	void dtor() noex ;
	operator int () noex ;
	destruct nulstr() {
	    if (as) dtor() ;
	} ;
} ; /* end struct (nulstr) */
#else /* __cplusplus */
typedef NULSTR		nulstr ;
#endif /* __cplusplus */

EXTERNC_begin

extern int	nulstr_start(nulstr *,cchar *,int,cchar **) noex ;
extern int	nulstr_finish(nulstr *) noex ;

EXTERNC_end


#endif /* NULSTR_INCLUDE */


