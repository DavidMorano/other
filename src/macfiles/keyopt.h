/* keyopt HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* paramater option manipulations */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-09-01, David A­D­ Morano
	This code module was originally written.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	KEYOPT_INCLUDE
#define	KEYOPT_INCLUDE


#include	<envstandards.h>	/* ordered first to configure */
#include	<clanguage.h>
#include	<usysbase.h>


#define	KEYOPT		struct keyopt_head
#define	KEYOPT_NA	struct keyopt_name
#define	KEYOPT_VAL	struct keyopt_value
#define	KEYOPT_CUR	struct keyopt_cursor
#define	KEYOPT_MAGIC	0x84634270


struct keyopt_value {
	KEYOPT_VAL	*next ;
	cchar		*value ;
} ; /* end struct */

struct keyopt_name {
	cchar		*name ;		/* the key */
	KEYOPT_NA	*next ;
	KEYOPT_VAL	*head ;		/* first */
	KEYOPT_VAL	*tail ;		/* last */
	KEYOPT_VAL	*current ;	/* used for interation only */
	int		cnt ;		/* count of values */
} ; /* end struct */

struct keyopt_cursor {
	KEYOPT_NA	*keyp ;
	KEYOPT_VAL	*valuep ;
} ; /* end struct */

struct keyopt_head {
	KEYOPT_NA	*head ;
	KEYOPT_NA	*tail ;
	uint		magval ;
	int		cnt ;		/* count of keys */
} ; /* end struct */

typedef	KEYOPT_NA	keyopt_na ;
typedef KEYOPT_CUR	keyopt_cur ;
typedef	KEYOPT_VAL	keyopt_val ;

#ifdef	__cplusplus
enum keyoptmems {
    	keyoptmem_start,
    	keyoptmem_incr,
	keyoptmem_count,
	keyoptmem_finish,
	keyoptmem_overlast
} ; /* end enum (keyoptmems) */
struct keyopt ;
struct keyopt_co {
	keyopt		*op = nullptr ;
	int		w = -1 ;
	void operator () (keyopt *p,int m) noex {
	    op = p ;
	    w = m ;
	} ;
	operator int () noex ;
	int operator () () noex { 
	    return operator int () ;
	} ;
} ; /* end struct (keyopt_co) */
struct keyopt : keyopt_head {
	keyopt_co	start ;
	keyopt_co	incr ;
	keyopt_co	count ;
	keyopt_co	finish ;
	keyopt() noex {
	    start	(this,keyoptmem_start) ;
	    incr	(this,keyoptmem_incr) ;
	    count	(this,keyoptmem_count) ;
	    finish	(this,keyoptmem_finish) ;
	    magval = 0 ;
	} ; /* end ctor */
	keyopt(const keyopt &) = delete ;
	keyopt &operator = (const keyopt &) = delete ;
	int loads	(cchar *,int) noex ;
	int load	(cchar *,int) noex ;
	int loadvalue	(cchar *,cchar *,int) noex ;
	int curbegin	(keyopt_cur *) noex ;
	int curend	(keyopt_cur *) noex ;
	int enumkeys	(keyopt_cur *,cchar **) noex ;
	int fetch	(cchar *,keyopt_cur *,cchar **) noex ;
	int enumvalues	(cchar *,keyopt_cur *,cchar **) noex ;
	int findvalue	(cc *,cc *,int,keyopt_val **) noex ;
	int lastvalue	(cchar *,cchar **) noex ;
	operator int () noex ;
	void dtor() noex ;
	destruct keyopt() {
	    if (magval) dtor() ;
	} ;
} ; /* end struct (keyopt) */
#else	/* __cplusplus */
typedef KEYOPT		keyopt ;
#endif /* __cplusplus */

EXTERNC_begin

extern int keyopt_start		(keyopt *) noex ;
extern int keyopt_loads		(keyopt *,cchar *,int) noex ;
extern int keyopt_load		(keyopt *,cchar *,int) noex ;
extern int keyopt_loadvalue	(keyopt *,cchar *,cchar *,int) noex ;
extern int keyopt_incr		(keyopt *op) noex ;
extern int keyopt_count		(keyopt *) noex ;
extern int keyopt_curbegin	(keyopt *,keyopt_cur *) noex ;
extern int keyopt_curend	(keyopt *,keyopt_cur *) noex ;
extern int keyopt_enumkeys	(keyopt *,keyopt_cur *,cchar **) noex ;
extern int keyopt_fetch		(keyopt *,cchar *,keyopt_cur *,cchar **) noex ;
extern int keyopt_enumvalues	(keyopt *,cchar *,keyopt_cur *,cchar **) noex ;
extern int keyopt_finish	(keyopt *) noex ;
extern int keyopt_findvalue	(keyopt *,cc *,cc *,int,keyopt_val **) noex ;
extern int keyopt_lastvalue	(keyopt *,cchar *,cchar **) noex ;

EXTERNC_end

#ifdef	__cplusplus

template<typename ... Args>
local inline int keyopt_magic(keyopt *op,Args ... args) noex {
	int		rs = SR_FAULT ;
	if (op && (args && ...)) {
	    rs = (op->magval == KEYOPT_MAGIC) ? SR_OK : SR_NOTOPEN ;
	}
	return rs ;
} /* end subroutine (keyopt_magic) */

namespace keyopt_keyname {
	extern int keyname_incri(keyopt_na *) noex ;
	extern int keyname_findv(keyopt_na *,cchar *,int,keyopt_val **) noex ;
} /* end namespace (keyopt_keyname) */

#endif /* __cplusplus */


#endif /* KEYOPT_INCLUDE */


