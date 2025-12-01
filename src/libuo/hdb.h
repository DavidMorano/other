/* hdb HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* general-purpose in-core hashing, DBM-like interface */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-05-01, David A­D­ Morano
	Orginally written due to frustration with other types of these things.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	HDB_INCLUDE
#define	HDB_INCLUDE


#include	<envstandards.h>	/* MUST be first to configure */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>
#include	<lookaside.h>


#define HDB_MAGIC	314159261
#define	HDB_DEFNUM	10
#define	HDB		struct hdb_head
#define HDB_DATUM	struct hdb_datum
#define HDB_DAT		struct hdb_datum
#define	HDB_CUR		struct hdb_cursor
#define HDB_ENT		struct hdb_entry


EXTERNC_begin

typedef uint		(*hdbhash_f)(cvoid *,int) noex ;
typedef int		(*hdbcmp_f)(cvoid *,cvoid *,int) noex ;

EXTERNC_end

struct hdb_datum {
	cvoid		*buf ;
	int		len ;
} ;

struct hdb_entry {
	HDB_ENT		*next ;		/* next in hash chain */
	HDB_ENT		*same ;		/* next w/ same key */
	HDB_DATUM	key ;
	HDB_DATUM	val ;
	uint		hv ;		/* hash-value of key */
} ;

struct hdb_cursor {
	int		i, j, k ;
} ;

struct hdb_head {
	HDB_ENT		**htaddr ;	/* array HDB_ENT pointers */
	hdbhash_f	hashfunc ;
	hdbcmp_f	cmpfunc ;
	lookaside	*esp ;		/* key-entries */
	uint		magic ;
	int		htlen ;
	int		count ;
	int		at ;		/* allocation-type */
} ; /* end struct (hdb_head) */

typedef HDB_DAT		hdb_dat ;
typedef HDB_ENT		hdb_ent ;
typedef HDB_CUR		hdb_cur ;

#ifdef	__cplusplus
enum hdbmems {
    	hdbmem_delall,
	hdbmem_audit,
	hdbmem_finish,
	hdbmem_overlast
} ;
struct hdb ;
struct hdb_co {
	hdb		*op = nullptr ;
	int		w = -1 ;
	void operator () (hdb *p,int m) noex {
	    op = p ;
	    w = m ;
	} ;
	operator int () noex ;
	int operator () () noex {
	    return operator int () ;
	} ;
} ; /* end struct (hdb_co) */
struct hdb : hdb_head {
	hdb_co		delall ;
	hdb_co		audit ;
	hdb_co		finish ;
	hdb() noex {
	    delall	(this,hdbmem_delall) ;
	    audit	(this,hdbmem_audit) ;
	    finish	(this,hdbmem_finish) ;
	    htaddr = nullptr ;
	} ;
	hdb(const hdb &) = delete ;
	hdb &operator = (const hdb &) = delete ;
	int start	(int,int,hdbhash_f,hdbcmp_f) noex ;
	int store	(hdb_dat,hdb_dat) noex ;
	int have	(hdb_dat) noex ;
	int fetch	(hdb_dat,hdb_cur *,hdb_dat *) noex ;
	int fetchrec	(hdb_dat,hdb_cur *,hdb_dat *,hdb_dat *) noex ;
	int getkeyrec	(hdb_dat,hdb_cur *,hdb_dat *,hdb_dat *) noex ;
	int nextrec	(hdb_dat,hdb_cur *) noex ;
	int curbegin	(hdb_cur *) noex ;
	int curget	(hdb_cur *,hdb_dat *,hdb_dat *) noex ;
	int curenum	(hdb_cur *,hdb_dat *,hdb_dat *) noex ;
	int curnext	(hdb_cur *) noex ;
	int curdel	(hdb_cur *,int) noex ;
	int curend	(hdb_cur *) noex ;
	int curdone	(hdb_cur *) noex ;
	int delkey	(hdb_dat) noex ;
	void dtor() noex ;
	destruct hdb() {
	    if (htaddr) dtor() ;
	} ;
} ; /* end struct (hdb) */
#else	/* __cplusplus */
typedef HDB		hdb ;
#endif /* __cplusplus */

EXTERNC_begin

extern int hdb_start	(hdb *,int,int,hdbhash_f,hdbcmp_f) noex ;
extern int hdb_have	(hdb *,hdb_dat) noex ;
extern int hdb_store	(hdb *,hdb_dat,hdb_dat) noex ;
extern int hdb_curbegin	(hdb *,hdb_cur *) noex ;
extern int hdb_curenum	(hdb *,hdb_cur *,hdb_dat *,hdb_dat *) noex ;
extern int hdb_curget	(hdb *,hdb_cur *,hdb_dat *,hdb_dat *) noex ;
extern int hdb_curnext	(hdb *,hdb_cur *) noex ;
extern int hdb_curdel	(hdb *,hdb_cur *,int) noex ;
extern int hdb_curend	(hdb *,hdb_cur *) noex ;
extern int hdb_curdone	(hdb *,hdb_cur *) noex ;
extern int hdb_curcopy	(hdb *,hdb_cur *,hdb_cur *) noex ;
extern int hdb_fetch	(hdb *,hdb_dat,hdb_cur *,hdb_dat *) noex ;
extern int hdb_fetchrec	(hdb *,hdb_dat,hdb_cur *,
			hdb_dat *,hdb_dat *) noex ;
extern int hdb_getkeyrec(hdb *,hdb_dat,hdb_cur *,
			hdb_dat *,hdb_dat *) noex ;
extern int hdb_nextrec	(hdb *,hdb_dat,hdb_cur *) noex ;
extern int hdb_delkey	(hdb *,hdb_dat) noex ;
extern int hdb_delall	(hdb *) noex ;
extern int hdb_count	(hdb *) noex ;
extern int hdb_hashtablen(hdb *,uint *) noex ;
extern int hdb_hashtabcounts(hdb *,int *,int) noex ;
extern int hdb_audit	(hdb *) noex ;
extern int hdb_finish	(hdb *) noex ;

EXTERNC_end


#endif /* HDB_INCLUDE */


