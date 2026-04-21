/* strpack HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* string-pack */
/* version %I% last-modified %G% */


/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	STRPACK_INCLUDE
#define	STRPACK_INCLUDE


#include	<envstandards.h>	/* MUST be first to configure */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>
#include	<vechand.h>


#define	STRPACK_MAGIC		0x42114683
#define	STRPACK			struct strpack_head
#define	STRPACK_CH		struct strpack_chunk


struct strpack_chunk {
	char		*cdata ;
	int		csz ;		/* allocated buffer length */
	int		i ;		/* index length */
	int		c ;		/* item count within chunk */
} ; /* end struct */

struct strpack_head {
	STRPACK_CH	*chp ;		/* current chunk pointer */
	vechand		*clp ;		/* chunk-list-pointer */
	uint		magic ;
	int		chsize ;
	int		totalsize ;
	int		c ;		/* total count */
} ; /* end struct (strpack_head) */

#ifdef	__cplusplus
enum strpackmems {
    	strpackmem_start,
	strpackmem_count,
	strpackmem_size,
	strpackmem_finish,
	strpackmem_overlast
} ; /* end enum (strpackmems) */
struct strpack ;
struct strpack_co {
	strpack		*op = nullptr ;
	int		w = -1 ;
	void operator () (strpack *p,int m) noex {
	    op = p ;
	    w = m ;
	} ;
	int operator () (int = 0) noex ;
	operator int () noex {
	    return operator () () ;
	} ;
} ; /* end struct (strpack_co) */
struct strpack : strpack_head {
	strpack_co	start ;
	strpack_co	count ;
	strpack_co	size ;
	strpack_co	finish ;
	strpack() noex {
	    start(this,strpackmem_start) ;
	    count(this,strpackmem_count) ;
	    size(this,strpackmem_size) ;
	    finish(this,strpackmem_finish) ;
	} ;
	strpack(const strpack &) = delete ;
	strpack &operator = (const strpack &) = delete ;
	int store(cchar *,int = -1,cchar ** = nullptr) noex ;
	void dtor() noex ;
	destruct strpack() {
	    if (magic) dtor() ;
	} ;
} ; /* end struct (strpack) */
#else	/* __cplusplus */
typedef STRPACK		strpack ;
#endif /* __cplusplus */

typedef	STRPACK_CH	strpack_ch ;

EXTERNC_begin

extern int	strpack_start(strpack *,int) noex ;
extern int	strpack_store(strpack *,cchar *,int,cchar **) noex ;
extern int	strpack_count(strpack *) noex ;
extern int	strpack_size(strpack *) noex ;
extern int	strpack_finish(strpack *) noex ;

extern int strpack_envstore(strpack *,cc *,cc *,int,cc **) noex ;
extern int strpack_envstorer(strpack *,cc *,int,cc *,int,cc **) noex ;

EXTERNC_end

#ifdef	__cplusplus

template<typename ... Args>
local inline int strpack_magic(strpack *op,Args ... args) noex {
	int		rs = SR_FAULT ;
	if (op && (args && ...)) {
	    rs = (op->magic == STRPACK_MAGIC) ? SR_OK : SR_NOTOPEN ;
	}
	return rs ;
} /* end subroutine (strpack_magic) */

#endif /* __cplusplus */


#endif /* STRPACK_INCLUDE */


