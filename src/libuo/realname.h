/* realname HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* manipulate real names */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-02-01, David A­D­ Morano
	Originally written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	REALNAME_INCLUDE
#define	REALNAME_INCLUDE


#include	<envstandards.h>	/* MUST be first to configure */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>
#include	<dstr.h>


#define	REALNAME_MAGIC		0x06253425
#define	REALNAME		struct realname_head
#define	REALNAME_LEN		struct realname_len
#define	REALNAME_ABV		struct realname_abv
#define	REALNAME_NNAMES		5
#define	REALNAME_NPARTS		REALNAME_NNAMES


struct realname_len {
	uchar		first ;
	uchar		m1 ;
	uchar		m2 ;
	uchar		m3 ;
	uchar		last ;
	uchar		store ;
} ;

struct realname_abv {
	uint		first:1 ;
	uint		m1:1 ;
	uint		m2:1 ;
	uint		m3:1 ;
	uint		last:1 ;
} ;

struct realname_head {
    	char		*stbuf ;	/* allocated */
	cchar		*first ;
	cchar		*m1 ;
	cchar		*m2 ;
	cchar		*m3 ;
	cchar		*last ;
	REALNAME_LEN	len ;
	REALNAME_ABV	abv ;
	int		stlen ;
	uint		magic ;
} ;

#ifdef	__cplusplus
enum realnamemems {
	realnamemem_finish,
	realnamemem_overlast
} ;
struct realname ;
struct realname_esther {
	realname	*op = nullptr ;
	int		w = -1 ;
	void init(realname *p,int m) noex {
	    op = p ;
	    w = m ;
	} ;
	int operator () () noex ;
	int operator () (cchar *,int = -1) noex ;
	int operator () (cchar **,int) noex ;
	int operator () (dstr *) noex ;
	operator int () noex {
	    return operator () () ;
	} ;
} ; /* end struct (realname_esther) */
struct realname_co {
	realname	*op = nullptr ;
	int		w = -1 ;
	void operator () (realname *p,int m) noex {
	    op = p ;
	    w = m ;
	} ;
	operator int () noex ;
	int operator () () noex { 
	    return operator int () ;
	} ;
} ; /* end struct (realname_co) */
struct realname : realname_head {
    	realname_esther	start ;
	realname_co	finish ;
	realname() noex {
	    start.init	(this,0) ;
	    finish	(this,realnamemem_finish) ;
	    stbuf = nullptr ;
	} ;
	realname(const realname &) = delete ;
	realname &operator = (const realname &) = delete ;
	int loadparts(dstr *) noex ;
	int loadpieces(char **,int) noex ;
	int getlast(cchar **) noex ;
	int getfirst(cchar **) noex ;
	int getm1(cchar **) noex ;
	int getm2(cchar **) noex ;
	int getm3(cchar **) noex ;
	int getpieces(cchar **) noex ;
	int fullname(char *,int) noex ;
	int name(char *,int) noex ;
	int mailname(char *,int) noex ;
	int mat(realname *) noex ;
	void dtor() noex ;
	destruct realname() {
	    if (stbuf) dtor() ;
	} ;
} ; /* end struct (realname) */
#else	/* __cplusplus */
typedef REALNAME	realname ;
#endif /* __cplusplus */

EXTERNC_begin

extern int	realname_start(realname *,cchar *,int) noex ;
extern int	realname_loadparts(realname *,dstr *) noex ;
extern int	realname_loadpieces(realname *,cchar **,int) noex ;
extern int	realname_getlast(realname *,cchar **) noex ;
extern int	realname_getfirst(realname *,cchar **) noex ;
extern int	realname_getm1(realname *,cchar **) noex ;
extern int	realname_getm2(realname *,cchar **) noex ;
extern int	realname_getm3(realname *,cchar **) noex ;
extern int	realname_getpieces(realname *,cchar **) noex ;
extern int	realname_fullname(realname *,char *,int) noex ;
extern int	realname_name(realname *,char *,int) noex ;
extern int	realname_mailname(realname *,char *,int) noex ;
extern int	realname_finish(realname *) noex ;

EXTERNC_end

#ifdef	COMMENT
EXTERNC_begin

extern int	realname_mat(realname *,realname *) noex ;
extern int	realname_matlast(realname *,char *,int) noex ;
extern int	realname_matfirst(realname *,char *,int) noex ;
extern int	realname_matm1(realname *,char *,int) noex ;
extern int	realname_matm2(realname *,char *,int) noex ;

EXTERNC_end
#endif /* COMMENT */


#endif /* REALNAME_INCLUDE */


