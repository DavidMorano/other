/* ucfileop HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* additional UNIX® support */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-03-21, David A­D­ Morano
	This module was originally written.

	= 2017-08-01, David A­D­ Morano
	Updated for lack of interfaces in Apple-Darwin.

*/

/* Copyright © 1998,2017 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

  	Names:
	uc_access	
	uc_chmod
	uc_chown	
	uc_link		
	uc_lstat	
	uc_minmod
	uc_mkdir
	uc_pathconf	
	uc_readlink	
	uc_remove
	uc_resolvepath	
	uc_realpath	
	uc_rmdir
	uc_stat		
	uc_statfs
	uc_statvfs	
	uc_symlink	
	uc_unlink	
	uc_unlinksem	
	uc_unlinkshm	
	uc_utime	
	uc_utimes	
	uc_truncate

  	Description:
	Additional (or supplemental) support for UNIX® limits.

	Synopsis:
	int uc_access		(cchar *,int) noex
	int uc_chmod		(cchar *fname,mode_t mm) noex
	int uc_chown		(cchar *,uid_t,gid_t) noex
	int uc_mkdir		(cchar *fname,mode_t dm) noex
	int uc_link		(cchar *,cchar *) noex
	int uc_lstat		(cchar *,ustat *) noex
	int uc_minmod		(cchar *fname,mode_t mm) noex
	int uc_pathconf		(cchar *,int,long *) noex
	int uc_readlink		(cchar *,char *,int) noex
	int uc_remove		(cchar *) noex ;
	int uc_resolvepath	(cchar *,char *,int) noex
	int uc_realpath		(cchar *,char *) noex
	int uc_rmdir		(cchar *) noex
	int uc_stat		(cchar *,ustat *) noex
	int uc_statfs		(cchar *fname,ustatfs *sbp) noex
	int uc_statvfs		(cchar *,ustatvfs *) noex
	int uc_symlink		(cchar *,cchar *) noex
	int uc_unlink		(cchar *) noex
	int uc_unlinksem	(cchar *) noex ;
	int uc_unlinkshm	(cchar *) noex ;
	int uc_utime		(cchar *,CUTIMBUF *) noex
	int uc_utimes		(cchar *,CTIMEVAL *) noex
	int uc_uctruncate	(cchar *fn,off_t len) noex

	Arguments:
	fn		file-name
	sfd		srouce file-descriptor
	dfd		destination file-descriptor
	sfname		source file-name
	dfname		destination file-name
	len		number of byte to copy or file-offset

*******************************************************************************/

#ifndef	UCFILEOP_INCLUDE
#define	UCFILEOP_INCLUDE


#include	<envstandards.h>	/* ordered first to configure */
#include	<clanguage.h>		/* LIBU */
#include	<usysbase.h>		/* LIBU */

#include	<uctruncate.h>
#include	<prefixfn.h>
#include	<ucfiletimes.h>
#include	<ucmkfifo.h>


EXTERNC_begin

extern int	uc_access	(cchar *,int) noex ;
extern int	uc_chmod	(cchar *,mode_t) noex ;
extern int	uc_chown	(cchar *,uid_t,gid_t) noex ;
extern int	uc_mkdir	(cchar *,mode_t) noex ;
extern int	uc_link		(cchar *,cchar *) noex ;
extern int	uc_lstat	(cchar *,ustat *) noex ;
extern int	uc_minmod	(cchar *,mode_t) noex ;
extern int	uc_pathconf	(cchar *,int,long *) noex ;
extern int	uc_readlink	(cchar *,char *,int) noex ;
extern int	uc_remove	(cchar *) noex ;
extern int	uc_resolvepath	(cchar *,char *,int) noex ;
extern int	uc_realpath	(cchar *,char *) noex ;
extern int	uc_rmdir	(cchar *) noex ;
extern int	uc_stat		(cchar *,ustat *) noex ;
extern int	uc_statfs	(cchar *,ustatfs *) noex ;
extern int	uc_statvfs	(cchar *,ustatvfs *) noex ;
extern int	uc_symlink	(cchar *,cchar *) noex ;
extern int	uc_unlink	(cchar *) noex ;
extern int	uc_unlinksem	(cchar *) noex ;
extern int	uc_unlinkshm	(cchar *) noex ;

EXTERNC_end


#endif /* UCFILEOP_INCLUDE */


