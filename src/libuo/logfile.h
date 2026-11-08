/* logfile HEADER */
/* lang=C20 */

/* perform logging operations on a file */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-02-22, David A­D­ Morano
	Originally written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	LOGFILE_INCLUDE
#define	LOGFILE_INCLUDE


#include	<envstandards.h>	/* MUST be first to configure */
#include	<stdarg.h>		/* CSTD |va_list| */
#include	<clanguage.h>		/* LIBU */
#include	<usysbase.h>		/* LIBU */
#include	<userinfo.h>		/* LIBUC */


/* object defines */
#define	LOGFILE			struct logfile_head
#define	LOGFILE_MAGIC		0x13f3c200
#define	LOGFILE_BUFSIZE		(2 * 1024)
#define	LOGFILE_LOGSIZE		80000	/* default log-file length */
#define	LOGFILE_PERCENT		20	/* percent allowed over limit */
#define	LOGFILE_LOGIDLEN	15
#define	LOGFILE_LINELEN		80
#define	LOGFILE_FMTLEN		(LOGFILE_LINELEN - (LOGFILE_LOGIDLEN + 1))
#define	LOGFILE_USERLEN		(LOGFILE_LINELEN - (LOGFILE_LOGIDLEN + 1))
/* control codes */
#define	LOGFILE_CNOP		0
#define	LOGFILE_CSIZE		1


struct logfile_head {
	cchar		*fname ;
	char		*buf ;
	time_t		ti_open ;
	time_t		ti_data ;
	time_t		ti_write ;
	uint		magval ;
	int		oflags ;
	int		lfd ;
	int		logidlen ;
	int		bufsz ;
	int		len ;		/* length of buffer filled so far */
	int		percent ;
	mode_t		operm ;
	char		logid[LOGFILE_LOGIDLEN + 1] ;
} ; /* end struct */

#ifdef	__cplusplus
enum logfilemems {
	logfilemem_flush,
	logfilemem_cksize,
	logfilemem_close,
	logfilemem_overlast
} ; /* end enum (logfilemems) */
struct logfile ;
struct logfile_co {
	logfile		*op = nullptr ;
	int		w = -1 ;
	void operator () (logfile *p,int m) noex {
	    op = p ;
	    w = m ;
	} ; /* end */
	int operator () (int = -1) noex ;
	operator int () noex {
	    return operator () () ;
	} ; /* end */
} ; /* end struct (logfile_co) */
struct logfile : logfile_head {
	logfile_co	flush ;
	logfile_co	cksize ;
	logfile_co	close ;
	logfile() noex {
	    flush	(this,logfilemem_flush) ;
	    cksize	(this,logfilemem_cksize) ;
	    close	(this,logfilemem_close) ;
	    buf		= nullptr ;
	} ; /* end ctor */
	logfile(const logfile &) = delete ;
	logfile &operator = (const logfile &) = delete ;
	int open	(cchar *,int,mode_t,cchar *) noex ;
	int setid	(cchar *) noex ;
	int write	(cchar *,int) noex ;
	int print	(cchar *,int) noex ;
	int printf	(cchar *,...) noex ;
	int vprintf	(cchar *,va_list) noex ;
	int check	(time_t) noex ;
	int chmod	(mode_t) noex ;
	int control	(int,void *) noex ;
	int printfold	(cchar *,cchar *,int) noex ;
	int userinfo	(userinfo *,time_t,cc *,cc *) noex ;
	void dtor	() noex ;
	destruct logfile() {
	    if (buf) dtor() ;
	} ; /* end */
} ; /* end struct (logfile) */
#else	/* __cplusplus */
typedef LOGFILE		logfile ;
#endif /* __cplusplus */

EXTERNC_begin

extern int logfile_open		(logfile *,cchar *,int,mode_t,cchar *) noex ;
extern int logfile_setid	(logfile *,cchar *) noex ;
extern int logfile_write	(logfile *,cchar *,int) noex ;
extern int logfile_print	(logfile *,cchar *,int) noex ;
extern int logfile_printf	(logfile *,cchar *,...) noex ;
extern int logfile_vprintf	(logfile *,cchar *,va_list) noex ;
extern int logfile_cksize	(logfile *,int) noex ;
extern int logfile_check	(logfile *,time_t) noex ;
extern int logfile_flush	(logfile *) noex ;
extern int logfile_chmod	(logfile *,mode_t) noex ;
extern int logfile_control	(logfile *,int,void *) noex ;
extern int logfile_printfold	(logfile *,cchar *,cchar *,int) noex ;
extern int logfile_userinfo	(logfile *,userinfo *,time_t,cc *,cc *) noex ;
extern int logfile_close	(logfile *) noex ;

EXTERNC_end


#endif /* LOGFILE_INCLUDE */


