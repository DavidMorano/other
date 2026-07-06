/* hdbstr HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* Key-Value Hash DataBase for Strings */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-11-01, David A­D­ Morano
	This subroutine was written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	HDBSTR_INCLUDE
#define	HDBSTR_INCLUDE


#include	<envstandards.h>	/* MUST be first to configure */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>
#include	<hdb.h>


#define	HDBSTR		hdb
#define	HDBSTR_CUR	hdb_cur
#define	HDBSTR_DAT	hdb_dat


typedef	HDBSTR_CUR	hdbstr_cur ;
typedef	HDBSTR_DAT	hdbstr_dat ;

#ifdef	__cplusplus
enum hdbstrmems {
    	hdbstrmem_start,
	hdbstrmem_delall,
	hdbstrmem_count,
	hdbstrmem_finish,
	hdbstrmem_overlast
} ;
struct hdbstr ;
struct hdbstr_iter {
    	hdbstr		*op = nullptr ;
    	hdbstr_cur	*curp = nullptr ;
	hdbstr_iter() = default ;
	hdbstr_iter(const hdbstr_iter &oit) noex ;
	hdbstr_iter &operator = (const hdbstr_iter &oit) noex ;
	bool operator != (const hdbstr_iter &) noex ;
	bool operator == (const hdbstr_iter &) noex ;
	cchar *operator * () noex ;
	hdbstr_iter operator + (int) const noex ;
	hdbstr_iter &operator += (int) noex ;
	hdbstr_iter &operator ++ () noex ; /* pre */
	hdbstr_iter operator ++ (int) noex ; /* post */
	destruct hdbstr_iter() {
	    if (op) dtor() ;
	} ;
    private:
	int increment(int = 1) noex ;
	void dtor() noex ;
} ; /* end struct (hdbstr_iter) */
struct hdbstr_co {
	hdbstr		*op = nullptr ;
	int		w = -1 ;
	void operator () (hdbstr *p,int m) noex {
	    op = p ;
	    w = m ;
	} ;
	int operator () (int = 0) noex ;
	operator int () noex {
	    return operator () (0) ;
	} ;
} ; /* end struct (hdbstr_co) */
struct hdbstr : hdb {
	hdbstr_co	start ;
	hdbstr_co	delall ;
	hdbstr_co	count ;
	hdbstr_co	finish ;
	bool		fopen{} ;
	hdbstr() noex {
	    start(this,hdbstrmem_start) ;
	    delall(this,hdbstrmem_delall) ;
	    count(this,hdbstrmem_count) ;
	    finish(this,hdbstrmem_finish) ;
	} ;
	hdbstr(const hdbstr &) = delete ;
	hdbstr &operator = (const hdbstr &) = delete ;
	int add(cchar *,int,cchar *,int) noex ;
	int curbegin(hdbstr_cur *) noex ;
	int curend(hdbstr_cur *) noex ;
	int curget(hdbstr_cur *,cc **,cc **,int *) noex ;
	int curenum(hdbstr_cur *,cc **,cc **,int *) noex ;
	int curnext(hdbstr_cur *) noex ;
	int curdel(hdbstr_cur *,int) noex ;
	int curdone(hdbstr_cur *) noex ;
	int fetch(cchar *,int,hdbstr_cur *,cchar **) noex ;
	int delkey(cchar *,int) noex ;
	int loadkeys(cchar *) noex ;
	int loadpairs(cchar *) noex ;
	hdbstr_iter begin() noex ;
	hdbstr_iter end() noex ;
	void dtor() noex ;
	destruct hdbstr() {
	    if (fopen) dtor() ;
	} ;
} ; /* end struct (hdbstr) */
#else	/* __cplusplus */
typedef HDBSTR		hdbstr ;
#endif /* __cplusplus */

EXTERNC_begin

extern int hdbstr_start(hdbstr *,int) noex ;
extern int hdbstr_add(hdbstr *,cchar *,int,cchar *,int) noex ;
extern int hdbstr_curbegin(hdbstr *,hdbstr_cur *) noex ;
extern int hdbstr_curget(hdbstr *,hdbstr_cur *,cchar **,cchar **,int *) noex ;
extern int hdbstr_curenum(hdbstr *,hdbstr_cur *,cchar **,cchar **,int *) noex ;
extern int hdbstr_curnext(hdbstr *,hdbstr_cur *) noex ;
extern int hdbstr_curdel(hdbstr *,hdbstr_cur *,int) noex ;
extern int hdbstr_fetch(hdbstr *,cchar *,int,hdbstr_cur *,cchar **) noex ;
extern int hdbstr_fetchrec(hdbstr *,cchar *,int,hdbstr_cur *,
				cchar **,cchar **,int *) noex ;
extern int hdbstr_nextkey(hdbstr *,cchar *,int,hdbstr_cur *) noex ;
extern int hdbstr_curcopy(hdbstr *,hdbstr_cur *,hdbstr_cur *) noex ;
extern int hdbstr_curdone(hdbstr *,hdbstr_cur *) noex ;
extern int hdbstr_curend(hdbstr *,hdbstr_cur *) noex ;
extern int hdbstr_delkey(hdbstr *,cchar *,int) noex ;
extern int hdbstr_delall(hdbstr *) noex ;
extern int hdbstr_count(hdbstr *) noex ;
extern int hdbstr_finish(hdbstr *) noex ;

extern int hdbstr_loadkeys(hdbstr *,cchar *) noex ;
extern int hdbstr_loadpairs(hdbstr *,cchar *) noex ;

EXTERNC_end


#endif /* HDBSTR_INCLUDE */


