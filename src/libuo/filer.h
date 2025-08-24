/* filer HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* support some buffered file operations */
/* version %I% last-modified %G% */


/* revision history:

	= 2002-04-01, David A­D­ Morano
	This code was originally written for Rightcore Network
	Services (RNS).

*/

/* Copyright © 2002 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

  	Object:
	filer

	Description:
        This little object supports some buffered file operations for
        low-overhead buffered I-O requirements.

*******************************************************************************/

#ifndef	FILER_INCLUDE
#define	FILER_INCLUDE


#include	<envstandards.h>	/* MUST be first to configure */
#include	<sys/types.h>		/* system types */
#include	<unistd.h>		/* system types */
#include	<stdarg.h>		/* |va_list(3c)| */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysrets.h>
#include	<usysdefs.h>


#define	FILER		struct filer_head
#define	FILER_FL	struct filer_flags
#define	FILER_MAGIC	0x21387462
#define	FILER_ONET	O_NETWORK	/* specify network FD */
#define	FILER_RCNET	3		/* read-count for network */


struct filer_flags {
	uint		net:1 ;		/* network FD */
	uint		write:1 ;	/* we are writing (otherwise reading) */
} ; /* end struct (filer_flags) */

struct filer_head {
	char		*dbuf ;		/* data-bufer pointer (constant) */
	char		*bp ;		/* goes up with use */
	off_t		off ;		/* virtual file pointer */
	FILER_FL	fl ;
	uint		magic ;
	int		fd ;		/* file-descriptor */
	int		of ;		/* open-flags */
	int		dt ;		/* file-type ("DT_XXX" values)*/
	int		dlen ;		/* data-buffer length (constant) */
	int		len ;		/* length of valid-data in buffer */
} ; /* end struct (filer_head) */

#ifdef	__cplusplus
enum filermems {
	filermem_reserve,
	filermem_invalidate,
	filermem_flush,
	filermem_adv,
	filermem_poll,
	filermem_lockend,
	filermem_finish,
	filermem_writeblanks,
	filermem_writealign,
	filermem_writezero,
	filermem_overlast
} ; /* end enum (filermems) */
struct filer ;
struct filer_co {
	filer		*op = nullptr ;
	int		w = -1 ;
	void operator () (filer *p,int m) noex {
	    op = p ;
	    w = m ;
	} ;
	int operator () (int) noex ;
	operator int () noex {
	    return operator () (0) ;
	} ;
} ; /* end struct (filer_co) */
struct filer : filer_head {
	filer_co	reserve ;
	filer_co	invalidate ;
	filer_co	flush ;
	filer_co	adv ;
	filer_co	poll ;
	filer_co	lockend ;
	filer_co	finish ;
	filer_co	writeblanks ;
	filer_co	writealign ;
	filer_co	writezero ;
	filer() noex {
	    reserve	(this,filermem_reserve) ;
	    invalidate	(this,filermem_invalidate) ;
	    flush	(this,filermem_flush) ;
	    adv		(this,filermem_adv) ;
	    poll	(this,filermem_poll) ;
	    lockend	(this,filermem_lockend) ;
	    finish	(this,filermem_finish) ;
	    writeblanks	(this,filermem_writeblanks) ;
	    writealign	(this,filermem_writealign) ;
	    writezero	(this,filermem_writezero) ;
	    magic = 0 ;
	} ; /* end ctor */
	filer(const filer &) = delete ;
	filer &operator = (const filer &) = delete ;
	int start(int = 0,off_t = 0z,int = 0,int = 0) noex ;
	int read(void *,int,int = -1) noex ;
	int readp(void *,int,off_t,int = -1) noex ;
	int readln(char *,int,int = -1) noex ;
	int readlns(char *,int,int = -1,int * = nullptr) noex ;
	int write(cvoid *,int) noex ;
	int writeto(cvoid *,int,int) noex ;
	int println(cchar *,int) noex ;
	int printf(cchar *,...) noex ;
	int vprintf(cchar *,va_list) noex ;
	int seek(off_t,int) noex ;
	int tell(off_t *) noex ;
	int writefill(cchar *,int) noex ;
	int writefd(char *,int,int,int) noex ;
	int stat(ustat *) noex ;
	int lockbegin(int,int) noex ;
	void dtor() noex ;
	destruct filer() {
	    if (magic) dtor() ;
	} ;
} ; /* end struct (filer) */
#else	/* __cplusplus */
typedef FILER		filer ;
#endif /* __cplusplus */

typedef FILER_FL	filer_fl ;

EXTERNC_begin

extern int	filer_start(filer *,int,off_t,int,int) noex ;
extern int	filer_read(filer *,void *,int,int) noex ;
extern int	filer_readp(filer *,void *,int,off_t,int) noex ;
extern int	filer_readln(filer *,char *,int,int) noex ;
extern int	filer_readlns(filer *,char *,int,int,int *) noex ;
extern int	filer_write(filer *,cvoid *,int) noex ;
extern int	filer_writeto(filer *,cvoid *,int,int) noex ;
extern int	filer_println(filer *,cchar *,int) noex ;
extern int	filer_printf(filer *,cchar *,...) noex ;
extern int	filer_vprintf(filer *,cchar *,va_list) noex ;
extern int	filer_reserve(filer *,int) noex ;
extern int	filer_update(filer *,off_t,cchar *,int) noex ;
extern int	filer_invalidate(filer *) noex ;
extern int	filer_flush(filer *) noex ;
extern int	filer_adv(filer *,int) noex ;
extern int	filer_seek(filer *,off_t,int) noex ;
extern int	filer_tell(filer *,off_t *) noex ;
extern int	filer_poll(filer *,int) noex ;
extern int	filer_stat(filer *,ustat *) noex ;
extern int	filer_lockbegin(filer *,int,off_t,int) noex ;
extern int	filer_lockend(filer *) noex ;
extern int	filer_finish(filer *) noex ;

extern int	filer_writeblanks(filer *,int) noex ;
extern int	filer_writefill(filer *,cchar *,int) noex ;
extern int	filer_writealign(filer *,int) noex ;
extern int	filer_writezero(filer *,int) noex ;
extern int	filer_writefd(filer *,char *,int,int,int) noex ;

EXTERNC_end

#if	__cplusplus

template<typename ... Args>
static inline int filer_magic(filer *op,Args ... args) noex {
	int		rs = SR_FAULT ;
	if (op && (args && ...)) {
	    rs = (op->magic == FILER_MAGIC) ? SR_OK : SR_NOTOPEN ;
	}
	return rs ;
}

#endif /* __cplusplus */


#endif /* FILER_INCLUDE */


