/* varsub HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* module to handle variable substitution in strings */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-02-01, David A­D­ Morano
	Originally written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	VARSUB_INCLUDE
#define	VARSUB_INCLUDE


#include	<envstandards.h>	/* MUST be first to configure */
#include	<clanguage.h>		/* LIBU */
#include	<usysbase.h>		/* LIBU */
#include	<vechand.h>		/* LIBUC */
#include	<buffer.h>		/* LIBUC */
#include	<vecstr.h>		/* LIBUC */


/* object defines */
#define	VARSUB			struct varsub_head
#define	VARSUB_FL		struct varsub_flags
#define	VARSUB_CUR		struct varsub_cursor
#define	VARSUB_MAGIC		0x91827364

enum varsubos {
    	varsubo_blank,		/* default, substitute blank */
    	varsubo_noblank,	/* substitute the key on failure */
    	varsubo_badnokey,	/* fail out on failure */
	varsubo_brace,		/* substitute on braces */
    	varsubo_paren,		/* substitute on parentheses */
    	varsubo_overlast	
} ; /* end enum (varsubos) */

#ifdef	__cplusplus
struct varsubms {
    	static inline cint	blank 		= (1 << varsubo_blank) ;
    	static inline cint	noblank 	= (1 << varsubo_noblank) ;
    	static inline cint	badnokey 	= (1 << varsubo_badnokey) ;
    	static inline cint	brace 		= (1 << varsubo_brace) ;
    	static inline cint	paren 		= (1 << varsubo_paren) ;
} ; /* end struct (varsubms) */
#endif /* __cplusplus */

/* initialization options */
#define	VARSUB_OBLANK		0	/* default, substitute blank */
#define	VARSUB_ONOBLANK		(1 << varsubo_noblank)
#define	VARSUB_OBADNOKEY	(1 << varsubo_badnokey)
#define	VARSUB_OBRACE		(1 << varsubº_brace)
#define	VARSUB_OPAREN		(1 << varsubo_paren)

struct varsub_flags {
	uint		badnokey:1 ;
	uint		noblank:1 ;
	uint		brace:1 ;
	uint		paren:1 ;
	uint		sorted:1 ;
} ; /* end struct */

struct varsub_cursor {
	int		i ;
} ; /* end struct */

struct varsub_head {
	vechand		*slp ;
	VARSUB_FL	fl ;
	uint		magval ;
	int		n ;		/* current allocated length */
	int		i ;		/* maximum length used */
	int		badline ;
} ; /* end struct */

typedef VARSUB		varsub ;
typedef	VARSUB_FL	varsub_fl ;
typedef VARSUB_CUR	varsub_cur ;

EXTERNC_begin

extern int	varsub_start(varsub *,int) noex ;
extern int	varsub_add(varsub *,cchar *,int,cchar *,int) noex ;
extern int	varsub_addva(varsub *,mainv) noex ;
extern int	varsub_addquick(varsub *,cchar *,int,cchar *,int) noex ;
extern int	varsub_addvaquick(varsub *,cchar **) noex ;
extern int	varsub_curbegin(varsub *,varsub_cur *) noex ;
extern int	varsub_curenum(varsub *,varsub_cur *,cchar **,cchar **) noex ;
extern int	varsub_curend(varsub *,varsub_cur *) noex ;
extern int	varsub_del(varsub *,cchar *,int) noex ;
extern int	varsub_fetch(varsub *,cchar *,int,cchar **) noex ;
extern int	varsub_exp(varsub *,char *,int,cchar *,int) noex ;
extern int	varsub_expbuf(varsub *,buffer *,cchar *,int) noex ;
extern int	varsub_loadfile(varsub *,cchar *) noex ;
extern int	varsub_addvec(varsub *,vecstr *) noex ;
extern int	varsub_finish(varsub *) noex ;

EXTERNC_end

#ifdef	__cplusplus
extern const varsubms	varsubm ;
#endif /* __cplusplus */


#endif /* VARSUB_INCLUDE */


