/* storeitem HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* storage object */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-02-17, David A­D­ Morano
	This object was originally written for use in some old
	Personal Communication Services (PCS) code (from some time
	ago, 1993?).

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	STOREITEM_INCLUDE
#define	STOREITEM_INCLUDE


#include	<envstandards.h>	/* ordered first to configure */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>


#define	STOREITEM	struct storeitem_head


struct storeitem_head {
	char		*dbuf ;
	int		dlen ;
	int		index ;
	int		f_overflow ;
} ;

#ifdef	__cplusplus
enum storeitemmems {
	storeitemmem_getlen,
	storeitemmem_finish,
	storeitemmem_overlast
} ;
struct storeitem ;
struct storeitem_co {
	storeitem	*op = nullptr ;
	int		w = -1 ;
	void operator () (storeitem *p,int m) noex {
	    op = p ;
	    w = m ;
	} ;
	operator int () noex ;
	int operator () () noex { 
	    return operator int () ;
	} ;
} ; /* end struct (storeitem_co) */
struct storeitem : storeitem_head {
	storeitem_co	getlen ;
	storeitem_co	finish ;
	storeitem() noex {
	    getlen(this,storeitemmem_getlen) ;
	    finish(this,storeitemmem_finish) ;
	} ;
	storeitem(const storeitem &) = delete ;
	storeitem &operator = (const storeitem &) = delete ;
	int start(char *,int) noex ;
	int strw(cchar *,int,cchar **) noex ;
	int buf(cvoid *,int,cchar **) noex ;
	int dec(int,cchar **) noex ;
	int chr(int,cchar **) noex ;
	int nul(cchar **) noex ;
	int ptab(int,void ***) noex ;
	int block(int,int,void **) noex ;
	void dtor() noex ;
	~storeitem() {
	    dtor() ;
	} ;
	int str(cchar *sp,cchar **rpp) noex {
	    return strw(sp,-1,rpp) ;
	} ;
} ; /* end struct (storeitem) */
#else	/* __cplusplus */
typedef STOREITEM	storeitem ;
#endif /* __cplusplus */

EXTERNC_begin

extern int storeitem_start(storeitem *,char *,int) noex ;
extern int storeitem_strw(storeitem *,cchar *,int,cchar **) noex ;
extern int storeitem_buf(storeitem *,cvoid *,int,cchar **) noex ;
extern int storeitem_dec(storeitem *,int,cchar **) noex ;
extern int storeitem_chr(storeitem *,int,cchar **) noex ;
extern int storeitem_nul(storeitem *,cchar **) noex ;
extern int storeitem_ptab(storeitem *,int,void ***) noex ;
extern int storeitem_block(storeitem *,int,int,void **) noex ;
extern int storeitem_getlen(storeitem *) noex ;
extern int storeitem_finish(storeitem *) noex ;

EXTERNC_end


#endif /* STOREITEM_INCLUDE */


