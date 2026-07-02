/* envs HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* environment-variable list container */
/* version %I% last-modified %G% */


/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	ENVS_INCLUDE
#define	ENVS_INCLUDE


#include	<envstandards.h>	/* MUST be first to configure */
#include	<sys/types.h>		/* POSIX */
#include	<clanguage.h>		/* LIBU */
#include	<usysbase.h>		/* LIBU */
#include	<hdb.h>			/* LIBUC */
#include	<expcook.h>		/* LIBUC*/
#include	<vecstr.h>		/* LIBUC*/
#include	<localmisc.h>		/* LIBU */


#define	ENVS		struct envs_head
#define	ENVS_CUR	struct envs_cursor
#define	ENVS_MAGIC	0x44376247


struct envs_cursor {
	hdb_cur		*curp ;
	int		i ;
} ; /* end struct */

struct envs_head {
	hdb		*varp ;
	uint		magic ;
} ; /* end struct */

typedef ENVS		envs ;
typedef ENVS_CUR	envs_cur ;

EXTERNC_begin

extern int envs_start		(envs *,int) noex ;
extern int envs_store		(envs *,cchar *,int,cchar *,int) noex ;
extern int envs_present		(envs *,cchar *,int) noex ;
extern int envs_substr		(envs *,cchar *,int,cchar *,int) noex ;
extern int envs_curbegin	(envs *,envs_cur *) noex ;
extern int envs_curend		(envs *,envs_cur *) noex ;
extern int envs_curenumkey	(envs *,envs_cur *,cchar **) noex ;
extern int envs_curenum		(envs *,envs_cur *,cchar **,cchar **) noex ;
extern int envs_fetch		(envs *,cchar *,int,envs_cur *,cchar **) noex ;
extern int envs_delname		(envs *,cchar *,int) noex ;
extern int envs_count		(envs *) noex ;
extern int envs_finish		(envs *) noex ;

extern int envs_procxe		(envs *,expcook *,mainv,vecstr *,cchar *) noex ;
extern int envs_subs		(envs *,expcook *,vecstr *,vecstr *) noex ;

EXTERNC_end


#endif /* ENVS_INCLUDE */


