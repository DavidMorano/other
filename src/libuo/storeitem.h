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
#include	<clanguage.h>		/* LIBU */
#include	<usysbase.h>		/* LIBU */
#include	<uconcepts.hh>		/* LIBU */


#define	STOREITEM	struct storeitem_head


struct storeitem_head {
	charp		dbuf ;
	int		dlen ;
	int		index ;
	int		f_overflow ;
} ; /* end struct */

#ifdef	__cplusplus
enum storeitemmems {
	storeitemmem_getlen,
	storeitemmem_finish,
	storeitemmem_overlast
} ; /* end enum */
struct storeitem ;
struct storeitem_co {
	storeitem	*op = nullptr ;
	int		w = -1 ;
	void operator () (storeitem *p,int m) noex {
	    op = p ;
	    w = m ;
	} ; /* end */
	operator int	() noex ;
	int operator ()	() noex { 
	    return operator int () ;
	} ; /* end */
} ; /* end struct (storeitem_co) */
struct storeitem : storeitem_head {
	storeitem_co	getlen ;
	storeitem_co	finish ;
	storeitem() noex {
	    getlen	(this,storeitemmem_getlen) ;
	    finish	(this,storeitemmem_finish) ;
	    dbuf	= nullptr ;
	} ; /* end ctor */
	storeitem(const storeitem &) = delete ;
	storeitem &operator = (const storeitem &) = delete ;
	int start	(char *,int) noex ;
	int strw	(cchar *,int,cchar **) noex ;
	int strw	(cchar **,ccharp,int = -1) noex ;
	int buf		(cvoid *,int,cchar **) noex ;
	int dec		(int,cchar **) noex ;
	int chr		(int,cchar **) noex ;
	int nul		(cchar **) noex ;
	template<std::integral T>
	int word	(con T **,con T) noex ;
	int ptab	(int,void ***) noex ;
	int block	(int,int,void ***) noex ;
	int strtab	(ccharpp *,con mainv) noex ;
	int str		(cchar *sp,cchar **rpp) noex {
	    return strw(sp,-1,rpp) ;
	} ; /* end */
	int aligner	(int) noex ;
	void dtor	() noex ;
	operator int	() noex ;
	destruct storeitem() {
	    if (dbuf) dtor() ;
	} ; /* end destruct */
    private:
	int getinc	(int) noex ;
	int wordx	(cvoid **,int,...) noex ;
} ; /* end struct (storeitem) */
#else	/* __cplusplus */
typedef STOREITEM	storeitem ;
#endif /* __cplusplus */

EXTERNC_begin
    extern int storeitem_start	(storeitem *,char *,int) noex ;
    extern int storeitem_strw	(storeitem *,cchar *,int,cchar **) noex ;
    extern int storeitem_buf	(storeitem *,cvoid *,int,cchar **) noex ;
    extern int storeitem_dec	(storeitem *,int,cchar **) noex ;
    extern int storeitem_chr	(storeitem *,int,cchar **) noex ;
    extern int storeitem_nul	(storeitem *,cchar **) noex ;
    extern int storeitem_ptab	(storeitem *,int,void ***) noex ;
    extern int storeitem_block	(storeitem *,int,int,void ***) noex ;
    extern int storeitem_getlen	(storeitem *) noex ;
    extern int storeitem_finish	(storeitem *) noex ;
EXTERNC_end

#ifdef	__cplusplus
    template<std::integral T> int storeitem::word(con T **rpp,con T w) noex {
	cint sz = szof(T) ;
	cvoidpp vpp = cvoidpp(rpp) ;
	return wordx(vpp,sz,w) ;
    } /* end subroutine */
#endif /* __cplusplus */


#endif /* STOREITEM_INCLUDE */


