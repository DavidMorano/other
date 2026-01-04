/* tmpx HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* object to facilitate UTMPX management */
/* version %I% last-modified %G% */


/* revision history:

	= 2000-05-14, David A­D­ Morano
	Originally written for Rightcore Network Services.

*/

/* Copyright © 2000 David A­D­ Morano.  All rights reserved. */

#ifndef	TMPX_INCLUDE
#define	TMPX_INCLUDE


#include	<envstandards.h>	/* ordered first to configure */
#include	<sys/types.h>		/* |pid_t| */
#include	<utmpx.h>
#include	<time.h>		/* |time_t| */
#include	<clanguage.h>
#include	<usysbase.h>
#include	<vecstr.h>
#include	<vecint.h>


/* UTMPX file */
#if	(defined(OSNAME_SunOS) && (OSNAME_SunOS > 0))
#define	UTMPX_DEFUTMP	"/var/adm/utmpx"
#elif	defined(OSNAME_Darwin) && (OSNAME_Darwin > 0)
#define	UTMPX_DEFUTMP	"/var/run/utmpx"
#elif	defined(OSNAME_Linux) && (OSNAME_Linux > 0)
#define	UTMPX_DEFUTMP	"/var/run/utmpx"
#else
#define	UTMPX_DEFUTMP	"/var/run/utmpx"
#endif

/* UTMPX stuff */
#ifndef	UTMPX_TEMPTY
#define	UTMPX_TEMPTY		0	/* entry is unused */
#define	UTMPX_TRUNLEVEL		1
#define	UTMPX_TBOOTTIME		2
#define	UTMPX_TOLDTIME		3
#define	UTMPX_TNEWTIME		4
#define	UTMPX_TPROCINIT		5	/* process spawned by "init" */
#define	UTMPX_TPROCLOGIN	6	/* a "getty" waiting for login */
#define	UTMPX_TPROCUSER		7	/* a regular user process */
#define	UTMPX_TPROCDEAD		8	/* used in WTMPX only? */
#define	UTMPX_TACCOUNT		9	/* used in WTMPX only? */
#define	UTMPX_TSIGNATURE	10	/* used in WTMPX only? */
#endif /* UTMPX_TEMPTY */

#ifndef	UTMPX_LID
#define	UTMPX_LID		4
#define	UTMPX_LUSER		32
#define	UTMPX_LLINE		32
#define	UTMPX_LHOST		256
#endif

/* object defines */
#define	TMPX		struct tmpx_head
#define	TMPX_FL		struct tmpx_flags
#define	TMPX_CUR	struct tmpx_cursor
#define	TMPX_ENT	struct utmpx
#define	TMPX_MAGIC	1092387456
#define	TMPX_ENTSIZE	szof(TMPX_ENT)
#define	TMPX_DEFUTMP	UTMPX_DEFUTMP

/* entry-type values */
#define	TMPX_TEMPTY	UTMPX_TEMPTY
#define	TMPX_TRUNLEVEL	UTMPX_TRUNLEVEL
#define	TMPX_TBOOTTIME	UTMPX_TBOOTTIME
#define	TMPX_TOLDTIME	UTMPX_TOLDTIME
#define	TMPX_TNEWTIME	UTMPX_TNEWTIME
#define	TMPX_TPROCINIT	UTMPX_TPROCINIT
#define	TMPX_TPROCLOGIN	UTMPX_TPROCLOGIN
#define	TMPX_TPROCUSER	UTMPX_TPROCUSER
#define	TMPX_TPROCDEAD	UTMPX_TPROCDEAD
#define	TMPX_TACCOUNT	UTMPX_TACCOUNT
#define	TMPX_TSIGNATURE	UTMPX_TSIGNATURE

/* entry-legnth values */
#define	TMPX_LID	UTMPX_LID
#define	TMPX_LUSER	UTMPX_LUSER
#define	TMPX_LLINE	UTMPX_LLINE
#define	TMPX_LHOST	UTMPX_LHOST


struct tmpx_cursor {
	int		i ;
} ;

struct tmpx_flags {
	uint		writable:1 ;
} ;

struct tmpx_head {
	cchar		*fname ;	/* stored file name */
	caddr_t		mapdata ;	/* file mapping buffer */
	time_t		ti_open ;	/* open time (for FD caching) */
	time_t		ti_mod ;	/* last modification time */
	time_t		ti_check ;	/* last check time */
	size_t		mapsize ;
	size_t		fsize ;		/* file total size */
	TMPX_FL		fl ;
	uint		mapoff ;	/* file mapping starting offset */
	uint		magic ;
	int		pagesz ;
	int		oflags ;	/* open flags */
	int		operms ;	/* open permissions */
	int		fd ;		/* file descriptor */
	int		ncursors ;
	int		mapei ;		/* index of top mapped entry */
	int		mapen ;		/* number of mapped entries */
} ;

typedef TMPX_FL		tmpx_fl ;
typedef TMPX_CUR	tmpx_cur ;
typedef TMPX_ENT	tmpx_ent ;

#ifdef	__cplusplus
enum tmpxmems {
    	tmpxmem_open,
    	tmpxmem_getrunlevel,
	tmpxmem_nusers,
	tmpxmem_close,
	tmpxmem_overlast
} ;
struct tmpx ;
struct tmpx_op {
	tmpx		*op = nullptr ;
	int		w = -1 ;
	void init(tmpx *p,int m) noex {
	    op = p ;
	    w = m ;
	} ;
	int operator () (cchar * = nullptr,int = 0) noex ;
	operator int () noex {
	    return operator () () ;
	} ;
} ; /* end struct (tmpx_op) */
struct tmpx_co {
	tmpx		*op = nullptr ;
	int		w = -1 ;
	void operator () (tmpx *p,int m) noex {
	    op = p ;
	    w = m ;
	} ;
	operator int () noex ;
	int operator () () noex { 
	    return operator int () ;
	} ;
} ; /* end struct (tmpx_co) */
struct tmpx : tmpx_head {
    	tmpx_op		open ;
	tmpx_co		getrunlevel ;
	tmpx_co		nusers ;
	tmpx_co		close ;
	tmpx() noex {
	    open.init	(this,tmpxmem_open) ;
	    getrunlevel	(this,tmpxmem_getrunlevel) ;
	    nusers	(this,tmpxmem_nusers) ;
	    close	(this,tmpxmem_close) ;
	    magic = 0 ;
	} ; /* end ctor */
	tmpx(const tmpx &) = delete ;
	tmpx &operator = (const tmpx &) = delete ;
	int read(int,tmpx_ent *) noex ;
	int write(int,tmpx_ent *) noex ;
	int check(time_t) noex ;
	int curbegin(tmpx_cur *) noex ;
	int curend(tmpx_cur *) noex ;
	int curenum(tmpx_cur *,tmpx_ent *) noex ;
	int fetchuser(tmpx_cur *,tmpx_ent *,cchar *) noex ;
	int fetchpid(tmpx_ent *,pid_t) noex ;
	int getboottim(time_t *) noex ;
	int getuserlines(vecstr *,cchar *) noex ;
	int getuserterms(vecstr *,cchar *) noex ;
	void dtor() noex ;
	destruct tmpx() {
	    if (magic) dtor() ;
	} ;
} ; /* end struct (tmpx) */
#else	/* __cplusplus */
typedef TMPX		tmpx ;
#endif /* __cplusplus */

EXTERNC_begin

extern int tmpx_open		(tmpx *,cchar *,int) noex ;
extern int tmpx_read		(tmpx *,int,tmpx_ent *) noex ;
extern int tmpx_write		(tmpx *,int,tmpx_ent *) noex ;
extern int tmpx_check		(tmpx *,time_t) noex ;
extern int tmpx_curbegin	(tmpx *,tmpx_cur *) noex ;
extern int tmpx_curend		(tmpx *,tmpx_cur *) noex ;
extern int tmpx_curenum		(tmpx *,tmpx_cur *,tmpx_ent *) noex ;
extern int tmpx_fetchuser	(tmpx *,tmpx_cur *,tmpx_ent *,cchar *) noex ;
extern int tmpx_fetchpid	(tmpx *,tmpx_ent *,pid_t) noex ;
extern int tmpx_nusers		(tmpx *) noex ;
extern int tmpx_close		(tmpx *) noex ;

extern int tmpx_getboottime	(tmpx *,time_t *) noex ;
extern int tmpx_getrunlevel	(tmpx *) noex ;
extern int tmpx_getuserlines	(tmpx *,vecstr *,cchar *) noex ;
extern int tmpx_getuserterms	(tmpx *,vecstr *,cchar *) noex ;
extern int tmpx_getsessions	(tmpx *,vecint *,cchar *) noex ;

EXTERNC_end

#ifdef	__cplusplus
template<typename ... Args>
static inline int tmpx_magic(tmpx *op,Args ... args) noex {
	int		rs = SR_FAULT ;
	if (op && (args && ...)) {
	    rs = (op->magic == TMPX_MAGIC) ? SR_OK : SR_NOTOPEN ;
	}
	return rs ;
}
/* end subroutine (tmpx_magic) */
#endif /* __cplusplus */


#endif /* TMPX_INCLUDE */


