/* fsdirtree HEADER */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* traverse directory tree */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-11-01, David A­D­ Morano
	This subroutine was written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	FSDIRTREE_INCLUDE
#define	FSDIRTREE_INCLUDE


#include	<envstandards.h>	/* MUST be first to configure */
#include	<clanguage.h>
#include	<usysbase.h>
#include	<fsdir.h>
#include	<fifostr.h>
#include	<hdb.h>


#define	FSDIRTREE		struct fsdirtree_head
#define	FSDIRTREE_FL		struct fsdirtree_flags
#define	FSDIRTREE_MAGIC		0x98653217

/* options */
enum fsdirtreeos {
    fsdirtreeo_pipe,
    fsdirtreeo_chr,
    fsdirtreeo_dir,
    fsdirtreeo_name,
    fsdirtreeo_blk,
    fsdirtreeo_reg,
    fsdirtreeo_lnk,
    fsdirtreeo_sock,
    fsdirtreeo_door,
    fsdirtreeo_wht,
    fsdirtreeo_follow,
    fsdirtreeo_noent,
    fsdirtreeo_uniqfile,
    fsdirtreeo_uniqdir,
    fsdirtreeo_igndotfile,
    fsdirtreeo_igndotdir,
    fsdirtreeo_overlast
} ; /* end enum (fsdirtreeos) */

/* masks */
#ifdef	__cplusplus
struct fsdirtreems {
    constexpr static cint	pipe		= (1 << fsdirtreeo_pipe) ;
    constexpr static cint	chr		= (1 << fsdirtreeo_chr) ;
    constexpr static cint	dir		= (1 << fsdirtreeo_dir) ;
    constexpr static cint	name		= (1 << fsdirtreeo_name) ;
    constexpr static cint	blk		= (1 << fsdirtreeo_blk) ;
    constexpr static cint	reg		= (1 << fsdirtreeo_reg) ;
    constexpr static cint	lnk		= (1 << fsdirtreeo_lnk) ;
    constexpr static cint	sock		= (1 << fsdirtreeo_sock) ;
    constexpr static cint	door		= (1 << fsdirtreeo_door) ;
    constexpr static cint	wht		= (1 << fsdirtreeo_wht) ;
    constexpr static cint	follow		= (1 << fsdirtreeo_follow) ;
    constexpr static cint	noent		= (1 << fsdirtreeo_noent) ;
    constexpr static cint	uniqfile	= (1 << fsdirtreeo_uniqfile) ;
    constexpr static cint	uniqdir		= (1 << fsdirtreeo_uniqdir) ;
    constexpr static cint	igndotfile	= (1 << fsdirtreeo_igndotfile) ;
    constexpr static cint	igndotdir	= (1 << fsdirtreeo_igndotdir) ;
} ; /* end struct (fsdirtreems) */
#endif /* __cplusplus */

/* options */
#define	FSDIRTREE_MPIPE		(1<<	    fsdirtreeo_pipe)
#define	FSDIRTREE_MCHR		(1<<	    fsdirtreeo_chr)
#define	FSDIRTREE_MDIR		(1<<	    fsdirtreeo_dir)
#define	FSDIRTREE_MNAME		(1<<	    fsdirtreeo_name)
#define	FSDIRTREE_MBLK		(1<<	    fsdirtreeo_blk)
#define	FSDIRTREE_MREG		(1<<	    fsdirtreeo_reg)
#define	FSDIRTREE_MLNK		(1<<	    fsdirtreeo_lnk)
#define	FSDIRTREE_MSOCK		(1<<	    fsdirtreeo_sock)
#define	FSDIRTREE_MDOOR		(1<<	    fsdirtreeo_door)
#define	FSDIRTREE_MWHT		(1<<	    fsdirtreeo_wht)
#define	FSDIRTREE_MFOLLOW	(1<<	    fsdirtreeo_follow)
#define	FSDIRTREE_MNOENT	(1<<	    fsdirtreeo_noent)
#define	FSDIRTREE_MUNIQFILE	(1<<	    fsdirtreeo_uniqfile)
#define	FSDIRTREE_MUNIQDIR	(1<<	    fsdirtreeo_uniqdir)
#define	FSDIRTREE_MIGNDOTFILE	(1<<	    fsdirtreeo_igndotfile)
#define	FSDIRTREE_MIGNDOTDIR	(1<<	    fsdirtreeo_igndotdir)

#define	FSDIRTREE_MFIFO		FSDIRTREE_MPIPE
#define	FSDIRTREE_MCHAR		FSDIRTREE_MCHR
#define	FSDIRTREE_MBLOCK	FSDIRTREE_MBLK
#define	FSDIRTREE_MLINK		FSDIRTREE_MLNK

struct fsdirtree_flags {
	uint		feof:1 ;
	uint		dir:1 ;
	uint		dirids:1 ;	/* dir-id tracking */
} ; /* end struct (fsdirtree_flags) */

struct fsdirtree_head {
	cchar		**prune ;
	char		*bnbuf ;
	char		*nbuf ;		/* "name" buffer */
	char		*lbuf ;		/* "link" buffer */
	void		*bsp ;		/* Bit-Set-Pointer */
	fifostr		*dqp ;		/* directory-queue-pointer */
	fsdir		*dirp ;		/* directory-pointer */
	hdb		*dip ;		/* directory-id-pointer */
	FSDIRTREE_FL	fl ;
	uint		magic ;
	int		opts ;
	int		bndlen ;
	int		cdnlen ;
	int		bnlen ;
	int		nlen ;		/* "name" length */
	int		llen ;		/* "link" length */
	ushort		selset ;	/* selection-set of DTs */
} ; /* end struct (fsdirtree_head) */

typedef FSDIRTREE	fsdirtree ;
typedef	FSDIRTREE_FL	fsdirtree_fl ;

EXTERNC_begin

extern int fsdirtree_open(fsdirtree *,cchar *,int) noex ;
extern int fsdirtree_prune(fsdirtree *,cchar **) noex ;
extern int fsdirtree_read(fsdirtree *,ustat *,char *,int) noex ;
extern int fsdirtree_close(fsdirtree *) noex ;

EXTERNC_end

#ifdef	__cplusplus
extern const fsdirtreems	fsdirtreem ;
#endif /* __cplusplus */


#endif /* FSDIRTREE_INCLUDE */


