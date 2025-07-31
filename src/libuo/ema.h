/* ema HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* E-Mail Address */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-02-03, David A­D­ Morano
	Originally written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	EMA_INCLUDE
#define	EMA_INCLUDE


#include	<envstandards.h>	/* MUST be first to configure */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>
#include	<vechand.h>


#define	EMA		struct ema_head
#define	EMA_FL		struct ema_flags
#define	EMA_ENT		struct ema_entry
#define	EMA_MAGIC	0x73169284


enum ematypes {
	ematype_reg,			/* regular */
	ematype_pcs,			/* PCS list */
	ematype_lalias,			/* local alias */
	ematype_salias,			/* system alias */
	ematype_group,			/* RFC-822 mail-address "group" */
	ematype_overlast
} ; /* end enum (ematypes) */

enum emaparts {
	emapart_address,
	emapart_route,
	emapart_comment,
	emapart_overlast
} ;

struct ema_head {
	vechand		*elp ;
	uint		magic ;
	int		n ;
} ; /* end struct (ema_head) */

struct ema_flags {
	uint		error:1 ;	/* address parse error */
	uint		expanded:1 ;	/* list has been expanded */
} ;

struct ema_entry {
	cchar		*op ;		/* original address */
	cchar		*ap ;		/* regular address part */
	cchar		*rp ;		/* route-address part (if any) */
	cchar		*cp ;		/* comment */
	EMA		*listp ;
	EMA_FL		fl ;
	int		type ;		/* mailing list type */
	int		n ;		/* number in list */
	int		ol, al, rl, cl ;
} ;

typedef	EMA_FL		ema_fl ;
typedef EMA_ENT		ema_ent ;

#ifdef	__cplusplus
enum emamems {
    	emamem_start,
	emamem_count,
	emamem_finish,
	emamem_overlast
} ;
struct ema ;
struct ema_st {
	ema		*op = nullptr ;
	int		w = -1 ;
	void operator () (ema *p,int m) noex {
	    op = p ;
	    w = m ;
	} ;
	int operator () (cchar * = nullptr,int = -1) noex ;
	operator int () noex {
	    cchar	*sp = nullptr ;
	    return operator () (sp,0) ;
	} ;
} ; /* end struct (ema_st) */
struct ema_co {
	ema		*op = nullptr ;
	int		w = -1 ;
	void operator () (ema *p,int m) noex {
	    op = p ;
	    w = m ;
	} ;
	operator int () noex ;
	int operator () () noex { 
	    return operator int () ;
	} ;
} ; /* end struct (ema_co) */
struct ema : ema_head {
    	ema_st		start ;
	ema_co		count ;
	ema_co		finish ;
	ema() noex {
	    start(this,emamem_start) ;
	    count(this,emamem_count) ;
	    finish(this,emamem_finish) ;
	    magic = 0 ;
	} ;
	ema(const ema &) = delete ;
	ema &operator = (const ema &) = delete ;
	int parse(cchar *,int = -1) noex ;
	int addent(ema_ent *) noex ;
	int addents(ema *) noex ;
	int get(int,ema_ent **) noex ;
	int getbestaddr(int,cchar **) noex ;
	int haveaddr(cchar *,int = -1) noex ;
	int first(cchar **) noex ;
	void dtor() noex ;
	destruct ema() {
	    if (magic) dtor() ;
	} ;
} ; /* end struct (ema) */
#else	/* __cplusplus */
typedef EMA		ema ;
#endif /* __cplusplus */

EXTERNC_begin

extern int ema_start(ema *) noex ;
extern int ema_parse(ema *,cchar *,int) noex ;
extern int ema_addent(ema *,ema_ent *) noex ;
extern int ema_addents(ema *,ema *) noex ;
extern int ema_get(ema *,int,ema_ent **) noex ;
extern int ema_getbestaddr(ema *,int,cchar **) noex ;
extern int ema_count(ema *) noex ;
extern int ema_haveaddr(ema *,cchar *,int) noex ;
extern int ema_first(ema *,cchar **) noex ;
extern int ema_finish(ema *) noex ;

EXTERNC_end

#ifdef	__cplusplus

template<typename ... Args>
static int ema_magic(ema *op,Args ... args) noex {
	int		rs = SR_FAULT ;
	if (op && (args && ...)) {
	    rs = (op->magic == EMA_MAGIC) ? SR_OK : SR_NOTOPEN ;
	}
	return rs ;
}
/* end subroutine (ema_magic) */

namespace ema_ns {
    extern int	ema_starter(ema *,cchar *,int) noex ;
}

#endif /* __cplusplus */


#endif /* EMA_INCLUDE */


