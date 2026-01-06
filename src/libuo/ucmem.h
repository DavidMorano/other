/* ucmem HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* memory allocation facility */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-03-05, David A­D­ Morano
	This subroutine was written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	UCMEM_INCLUDE
#define	UCMEM_INCLUDE


#include	<envstandards.h>	/* ordered first to configure */
#include	<clanguage.h>
#include	<usysbase.h>


#define	UCMEM_STATS	struct ucmem_status
#define	UCMEM_CUR	struct ucmem_cursor
#define	UCMEM_ENT	struct ucmem_entry


struct ucmem_cursor {
	void		*mcp ;		/* Mall-Cursor-Pointer */
} ; /* end struct (ucmem_cursor) */

struct ucmem_entry {
	caddr_t		addr ;
	size_t		asize ;
} ; /* end struct (ucmem_entry) */

enum ucmallsets {
	ucmallset_off,
	ucmallset_on,
	ucmallset_overlast
} ; /* end enum (ucmallsets) */

struct ucmem_status {
	ulong		err_underflow ;
	ulong		err_overflow ;
	ulong		err_noalloc ;
	ulong		err_nofree ;
	ulong		num_allocs ;
	ulong		num_malloc ;
	ulong		num_valloc ;
	ulong		num_calloc ;
	ulong		num_realloc ;
	ulong		num_frees ;
	ulong		num_outmax ;
	ulong		out_size ;
	ulong		out_sizemax ;
	int		err_rs ;
} ; /* end structure (ucmem_stats) */

typedef	UCMEM_STATS	ucmem_stats ;
typedef	UCMEM_CUR	ucmem_cur ;
typedef	UCMEM_ENT	ucmem_ent ;

EXTERNC_begin

extern int uc_malloc		(int,void *) noex ;
extern int uc_valloc		(int,void *) noex ;
extern int uc_calloc		(int,int,void *) noex ;
extern int uc_realloc		(void *,int,void *) noex ;
extern int uc_free		(void *vp) noex ;
extern int uc_mallset		(int) noex ;
extern int uc_mallcount		(ulong *) noex ;
extern int uc_mallout		(ulong *) noex ;
extern int uc_mallstats		(ucmem_stats *) noex ;
extern int uc_mallpresent	(cvoid *) noex ;
extern int uc_mincoreset	(void *,size_t,char *) noex ;
extern int ucmem_curbegin	(ucmem_cur *) noex ;
extern int ucmem_curend		(ucmem_cur *) noex ;
extern int ucmem_curenum	(ucmem_cur *,ucmem_ent *) noex ;

EXTERNC_end

#ifdef	__cplusplus
namespace libuc {
    struct mems {
	int malloc	(int,void *) noex ;
	int valloc	(int,void *) noex ;
	int calloc	(int,int,void *) noex ;
	int ralloc	(void *,int,void *) noex ;
	int strw	(cchar *,int,void *) noex ;
	int mall	(int,void *) noex ;
	int vall	(int,void *) noex ;
	int call	(int,int,void *) noex ;
	int rall	(void *,int,void *) noex ;
	int free	(void *vp) noex ;
	int mallset	(int) noex ;
	int mallcount	(ulong *) noex ;
	int mallout	(ulong *) noex ;
	int mallstats	(ucmem_stats *) noex ;
	int mallpresent	(cvoid *) noex ;
        int curbegin	(ucmem_cur *curp) noex ;
        int curend	(ucmem_cur *curp) noex ;
        int curenum	(ucmem_cur *curp,ucmem_ent *rp) noex ;
    } ; /* end struct (mems) */
    extern mems		mem ;
} /* end namespace (libuc) */
#endif /* __cplusplus */


#endif /* UCMEM_INCLUDE */


