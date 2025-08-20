/* strop HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* string-read-operations */
/* version %I% last-modified %G% */


/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	STROP_INCLUDE
#define	STROP_INCLUDE


#include	<envstandards.h>	/* MUST be first to configure */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>


#define	STROP		struct strop_head


struct strop_head {
	cchar		*sp ;
	int		sl ;
} ; /* end struct (strop_head) */

#ifdef	__cplusplus
enum stropmems {
	stropmem_remlen,
	stropmem_inc,
	stropmem_shrink,
	stropmem_white,
	stropmem_whitedash,
	stropmem_whitecolon,
	stropmem_finish,
	stropmem_overlast
} ;
struct strop ;
struct strop_co {
	strop		*op = nullptr ;
	int		w = -1 ;
	void operator () (strop *p,int m) noex {
	    op = p ;
	    w = m ;
	} ;
	operator int () noex ;
	int operator () () noex { 
	    return operator int () ;
	} ;
} ; /* end struct (strop_co) */
struct strop : strop_head {
	strop_co	remlen ;
	strop_co	inc ;
	strop_co	shrink ;
	strop_co	white ;
	strop_co	whitedash ;
	strop_co	whitecolon ;
	strop_co	finish ;
	strop() noex {
	    remlen	(this,stropmem_remlen) ;
	    inc		(this,stropmem_inc) ;
	    shrink	(this,stropmem_shrink) ;
	    white	(this,stropmem_white) ;
	    whitedash	(this,stropmem_whitedash) ;
	    whitecolon	(this,stropmem_whitecolon) ;
	    finish	(this,stropmem_finish) ;
	    sp = nullptr ;
	} ; /* end ctor */
	strop(const strop &) = delete ;
	strop &operator = (const strop &) = delete ;
	int start(cchar *,int = -1) noex ;
	int shrinkchr(int = -1) noex ;
	int whitechr(int) noex ;		/* skip wht until */
	int fieldwht(cchar **) noex ;
	int fieldchr(int,cchar **) noex ;
	int fieldbrk(cchar *,cchar **) noex ;
	int findchr(int) noex ;
	int findterm(cchar *) noex ;
	int spanterm(cchar *) noex ;
	int span(cchar *) noex ;
	void dtor() noex ;
	destruct strop() {
	    if (sp) dtor() ;
	} ;
} ; /* end struct (strop) */
#else	/* __cplusplus */
typedef STROP		strop ;
#endif /* __cplusplus */

EXTERNC_begin

extern int	strop_start(strop *,cchar *,int) noex ;
extern int	strop_shrink(strop *) noex ;
extern int	strop_white(strop *) noex ;
extern int	strop_whitechr(strop *,int) noex ;
extern int	strop_fieldwht(strop *,cchar **) noex ;
extern int	strop_fieldchr(strop *,int,cchar **) noex ;
extern int	strop_fieldbrk(strop *,cchar *,cchar **) noex ;
extern int	strop_findchr(strop *,int) noex ;
extern int	strop_findterm(strop *,cchar *) noex ;
extern int	strop_spanterm(strop *,cchar *) noex ;
extern int	strop_span(strop *,cchar *) noex ;
extern int	strop_finish(strop *) noex ;

inline int strop_whitedash(strop *op) noex {
	return strop_whitechr(op,'-') ;
}
inline int strop_whitecolon(strop *op) noex {
	return strop_whitechr(op,':') ;
}

EXTERNC_end


#endif /* STROP_UNCLUDE */


