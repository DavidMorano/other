/* shortq HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* short-word queue */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-02-01, David A­D­ Morano
	This obejct module was originally written.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	SHORTQ_INCLUDE
#define	SHORTQ_INCLUDE


#include	<envstandards.h>	/* MUST be ordered first to configure */
#include	<stddef.h>
#include	<stdlib.h>
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>


#define	SHORTQ		struct shortq_head
#define	SHORTQ_MAGIC	0x87625374


struct shortq_head {
    	void		*qvp ;		/* queue-value-pointer */
	uint		magic ;
} ; /* end struct (shortq_head) */

#ifdef	__cplusplus
enum shortqmems {
    	shortqmem_start,
	shortqmem_remall,
	shortqmem_size,
	shortqmem_count,
	shortqmem_len,
	shortqmem_finish,
	shortqmem_overlast
} ; /* end enum (shortqmem) */
struct shortq ;
struct shortq_co {
	shortq		*op = nullptr ;
	int		w = -1 ;
	void operator () (shortq *p,int m) noex {
	    op = p ;
	    w = m ;
	} ;
	int operator () (int = 0) noex ;
	operator int () noex {
	    return operator () (0) ;
	} ;
} ; /* end struct (shortq_co) */
struct shortq : shortq_head {
	shortq_co	start ;
	shortq_co	remall ;
	shortq_co	size ;
	shortq_co	count ;
	shortq_co	len ;
	shortq_co	finish ;
	shortq() noex {
	    start	(this,shortqmem_start) ;
	    remall	(this,shortqmem_remall) ;
	    size	(this,shortqmem_size) ;
	    count	(this,shortqmem_count) ;
	    len		(this,shortqmem_len) ;
	    finish	(this,shortqmem_finish) ;
	    magic = 0 ;
	} ; /* end ctor */
	shortq(const shortq &) = delete ;
	shortq &operator = (const shortq &) = delete ;
	int load(short *,int) noex ;
	int ins(short) noex ;
	int rem(short *) noex ;
	int remread(short *,int) noex ;
	int get(int) noex ;
	int readat(int,short *,int) noex ;
	int read(short *,int) noex ;
	void dtor() noex ;
	int operator [] (int) noex ;
	operator int () noex ;
	destruct shortq() {
	    if (magic) dtor() ;
	} ;
} ; /* end struct (shortq) */
#else	/* __cplusplus */
typedef SHORTQ		shortq ;
#endif /* __cplusplus */

EXTERNC_begin

extern int	shortq_start	(shortq *,int) noex ;
extern int	shortq_load	(shortq *,short *,int) noex ;
extern int	shortq_ins	(shortq *,short) noex ;
extern int	shortq_rem	(shortq *,short *) noex ;
extern int	shortq_remread	(shortq *,short *,int) noex ;
extern int	shortq_remall	(shortq *) noex ;
extern int	shortq_get	(shortq *,int) noex ;
extern int	shortq_readat	(shortq *,int,short *,int) noex ;
extern int	shortq_read	(shortq *,short *,int) noex ;
extern int	shortq_size	(shortq *) noex ;
extern int	shortq_count	(shortq *) noex ;
extern int	shortq_finish	(shortq *) noex ;

local inline int shortq_len(shortq *op) noex {
    	return shortq_count(op) ;
}

EXTERNC_end


#endif /* SHORTQ_INCLUDE */


