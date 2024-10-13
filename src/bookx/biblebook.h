/* biblebook HEADER */
/* lang=C++20 */

/* BIBLEBOOK object-load management */
/* version %I% last-modified %G% */


/* revision history:

	= 2008-10-01, David A­D­ Morano
	Originally written for Rightcore Network Services.

*/

/* Copyright © 2008 David A­D­ Morano.  All rights reserved. */

#ifndef	BIBLEBOOK_INCLUDE
#define	BIBLEBOOK_INCLUDE


#include	<envstandards.h>	/* ordered first to configure */
#include	<sys/types.h>
#include	<modload.h>
#include	<localmisc.h>


#define	BIBLEBOOK_MAGIC		0x99447242
#define	BIBLEBOOK		struct biblebook_head
#define	BIBLEBOOK_FL		struct biblebook_flags
#define	BIBLEBOOK_CA		struct biblebook_calls
#define	BIBLEBOOK_LEN		80 /* bible-book-name length */
#define	BIBLEBOOK_NBOOKS	66


struct biblebook_calls {
	int	(*open)(void *,cchar *,cchar *) ;
	int	(*count)(void *) ;
	int	(*max)(void *) ;
	int	(*lookup)(void *,char *,int,int) ;
	int	(*get)(void *,int,char *,int) ;
	int	(*match)(void *,cchar *,int) ;
	int	(*size)(void *) ;
	int	(*audit)(void *) ;
	int	(*close)(void *) ;
} ;

struct biblebook_flags {
	int		localdb:1 ;	/* using local DB */
} ;

struct biblebook_head {
	modload		loader ;
	void		*obj ;		/* object pointer */
	cchar		**names ;
	cchar		*namestrs ;
	BIBLEBOOK_CA	call ;
	BIBLEBOOK_FL	f ;
	uint		magic ;
	int		objsize ;	/* object size */
	int		namesize ;	/* names-size */
} ;

typedef	BIBLEBOOK		biblebook ;
typedef	BIBLEBOOK_FL		biblebook_fl ;
typedef	BIBLEBOOK_CA		biblebook_ca ;

EXTERNC_begin

extern int	biblebook_open(biblebook *,cchar *,cchar *) noex ;
extern int	biblebook_count(biblebook *) noex ;
extern int	biblebook_max(biblebook *) noex ;
extern int	biblebook_read(biblebook *,char *,int,int) noex ;
extern int	biblebook_lookup(biblebook *,char *,int,int) noex ;
extern int	biblebook_get(biblebook *,int,char *,int) noex ;
extern int	biblebook_match(biblebook *,cchar *,int) noex ;
extern int	biblebook_size(biblebook *) noex ;
extern int	biblebook_audit(biblebook *) noex ;
extern int	biblebook_close(biblebook *) noex ;

EXTERNC_end


#endif /* BIBLEBOOK_INCLUDE */


