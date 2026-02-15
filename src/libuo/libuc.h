/* uclibsubs HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* version %I% last-modified %G% */
/* UNIX® C-language library (standard) subroutines */


/* revision history:

	= 1998-03-21, David A­D­ Morano
	This module was originally written.

	= 2017-08-01, David A­D­ Morano
	Updated for lack of interfaces in MacOS Darwin

*/

/* Copyright © 1998,2017 David A­D­ Morano.  All rights reserved. */

#ifndef	UCLIBSUBS_INCLUDE
#define	UCLIBSUBS_INCLUDE


#include	<envstandards.h>	/* MUST be first to configure */

#include	<usysnative.h>		/* native system headers */

#include	<clanguage.h>
#include	<usysbase.h>
#include	<usyscalls.h>

#include	<absfn.h>
#include	<acceptpass.h>
#include	<altzone.h>
#include	<ascii.h>
#include	<attachso.h>
#include	<baops.h>
#include	<base64.h>
#include	<bits.h>
#include	<buffer.h>
#include	<bufprintf.h>
#include	<bufsize.h>
#include	<bufstr.h>
#include	<bwops.h>
#include	<cachetime.h>
#include	<calstrs.h>
#include	<cfx.h>
#include	<char.h>
#include	<chartrans.h>
#include	<cheaprand.h>
#include	<checkbase.h>
#include	<chrono.h>
#include	<clanguage.h>
#include	<clusterdb.h>
#include	<conmsghdr.h>
#include	<csem.h>
#include	<ctx.h>
#include	<dater.h>
#include	<defproc.h>
#include	<dictdiff.h>
#include	<digval.h>
#include	<dirempty.h>
#include	<dirent.h>
#include	<dstr.h>
#include	<dupstr.h>
#include	<envhelp.h>
#include	<envlist.h>
#include	<envmgr.h>
#include	<envs.h>
#include	<envstandards.h>	/* MUST be first to configure */
#include	<envstandards.h>	/* ordered first to configure */
#include	<epsem.h>
#include	<errno.h>
#include	<estrings.h>
#include	<exitcodes.h>
#include	<expcook.h>
#include	<ffbs.h>
#include	<fibonacci.h>
#include	<field.h>
#include	<filegrp.h>
#include	<filemap.h>
#include	<filer.h>
#include	<findfilepath.h>
#include	<findinline.h>
#include	<findxfile.h>
#include	<format.h>
#include	<fsdir.h>
#include	<fsdirtree.h>
#include	<gecos.h>
#include	<getbufsize.h>
#include	<getexecname.h>
#include	<getmjd.h>
#include	<getngroups.h>
#include	<getutmp.h>
#include	<getx.h>
#include	<getxname.h>
#include	<getxx.h>
#include	<gncache.h>
#include	<grcache.h>
#include	<grmems.h>
#include	<hash.h>
#include	<hasx.h>
#include	<hdb.h>
#include	<hdbstr.h>
#include	<hostaddr.h>
#include	<hostent.h>
#include	<hostinfo.h>
#include	<ids.h>
#include	<inet4int.h>
#include	<inetaddr.h>
#include	<inetaddrparse.h>
#include	<inetconv.h>
#include	<initnow.h>
#include	<inttypesx.h>
#include	<ischarx.h>
#include	<isfiledesc.h>
#include	<ismisc.h>
#include	<isort.h>
#include	<isx.h>
#include	<keyopt.h>
#include	<kvsfile.h>
#include	<limits.h>
#include	<liner.h>
#include	<localmisc.h>
#include	<lockfile.h>
#include	<lookaside.h>
#include	<mallocstuff.h>
#include	<mapex.h>
#include	<mapshmtmp.h>
#include	<mapstrint.h>
#include	<mapstrs.h>
#include	<matparam.h>
#include	<matstr.h>
#include	<matxstr.h>
#include	<maxvecaddr.h>
#include	<memfile.h>
#include	<mkdirs.h>
#include	<mkfile.h>
#include	<mklogid.h>
#include	<mkprogenv.h>
#include	<mkdirs.h>
#include	<mkfile.h>
#include	<mklogid.h>
#include	<mkpathxx.h>
#include	<mkprogenv.h>
#include	<mktmp.h>
#include	<mkx.h>
#include	<modload.h>
#include	<namecache.h>
#include	<ncol.h>
#include	<ndig.h>
#include	<ndigit.h>
#include	<netdb.h>
#include	<netgroupcheck.h>
#include	<netorder.h>
#include	<nleadkeystr.h>
#include	<nleadstr.h>
#include	<nodedb.h>
#include	<nonpath.h>
#include	<nulstr.h>
#include	<numsign.h>
#include	<osetint.h>
#include	<osetstr.h>
#include	<ourmjd.h>
#include	<outbuf.h>
#include	<paramfile.h>
#include	<pathadd.h>
#include	<pathclean.h>
#include	<permx.h>
#include	<pow.h>
#include	<prmktmpdir.h>
#include	<psem.h>
#include	<pwcache.h>
#include	<pwentry.h>
#include	<pwfile.h>
#include	<quoteshellarg.h>
#include	<randlc.h>
#include	<randomvar.h>
#include	<rmdirfiles.h>
#include	<rmx.h>
#include	<rpsem.h>
#include	<rsfree.h>
#include	<sbuf.h>
#include	<secdb.h>
#include	<serialbuf.h>
#include	<sesmsg.hh>
#include	<setint.h>
#include	<setstr.h>
#include	<sfx.h>
#include	<shellunder.h>
#include	<sigobj.h>
#include	<sigx.h>
#include	<slist.h>
#include	<snwcpy.h>
#include	<snx.h>
#include	<sockaddress.h>
#include	<spawner.h>
#include	<spawnproc.h>
#include	<spwdent.h>
#include	<stdorder.h>
#include	<storebuf.h>
#include	<storeitem.h>
#include	<stpcpyxc.h>
#include	<stpncpyxc.h>
#include	<strcpy.h>
#include	<strcpyx.h>
#include	<strcpyxc.h>
#include	<strdcpy.h>
#include	<strdcpyx.h>
#include	<strdcpyxw.h>
#include	<strlinelen.h>
#include	<strlocktype.h>
#include	<strman.h>
#include	<strmgr.h>
#include	<strn.h>
#include	<strncpyxc.h>
#include	<strnwcpyxc.h>
#include	<strnxcmp.h>
#include	<strop.h>
#include	<strpack.h>
#include	<strstdfname.h>
#include	<strstore.h>
#include	<strtab.h>
#include	<strtabfind.h>
#include	<strval.h>
#include	<strw.h>
#include	<strwcpy.h>
#include	<strwcpyxc.h>
#include	<strx.h>

#include	<sysdbfiles.h>
#include	<sysdbfn.h>
#include	<syshas.h>
#include	<syshasutmpx.h>
#include	<sysmemutil.h>
#include	<sysnoise.h>
#include	<tabcols.h>
#include	<tabexpand.h>
#include	<tcx.h>
#include	<termcmd.h>
#include	<termconseq.h>
#include	<termescseq.h>
#include	<termios_cf.h>
#include	<termstr.h>
#include	<termtypemat.h>
#include	<thrbase.h>
#include	<thrcomm.h>
#include	<timeout.h>
#include	<timestr.h>
#include	<tmpx.h>
#include	<toxc.h>
#include	<ucaddrinfo.h>
#include	<ucclock.h>
#include	<ucclustername.h>
#include	<ucent.h>
#include	<ucenum.h>
#include	<ucfdmanip.h>
#include	<ucfiledesc.h>
#include	<ucfileop.h>
#include	<ucfork.h>
#include	<ucfuncodes.h>
#include	<ucget.h>
#include	<ucgethost.h>
#include	<ucgetloadavg.h>
#include	<ucgetpid.h>
#include	<ucgetrandom.h>
#include	<ucgetx.h>
#include	<ucids.h>
#include	<ucinetconv.h>
#include	<uckvamatch.h>
#include	<uclibmem.h>
#include	<uclock.h>
#include	<ucmem.h>
#include	<ucmemla.h>
#include	<ucontext.h>
#include	<ucopen.h>
#include	<ucopeninfo.h>
#include	<ucpeek.h>
#include	<ucprochave.h>
#include	<ucproctypes.h>
#include	<ucprogdata.h>
#include	<ucproguser.h>
#include	<ucpts.h>
#include	<ucpts.h>		/* pseudo-terminal management */
#include	<ucpwcache.h>
#include	<ucrand.h>
#include	<ucread.h>
#include	<ucsig.h>
#include	<ucsock.h>
#include	<ucsthr.h>
#include	<ucstrtof.h>
#include	<ucstrtof.h>		/* |float| + |double| + |longdouble| */
#include	<ucstrtox.h>
#include	<ucsys.h>
#include	<ucsysauxinfo.h>
#include	<ucsysconf.h>
#include	<ucsysmisc.h>
#include	<uctc.h>
#include	<uctc.h>		/* terminal interface */
#include	<uctim.h>
#include	<uctimeconv.h>
#include	<uctimer.h>
#include	<ucuserattr.h>
#include	<ucutility.h>
#include	<ucwrite.h>
#include	<udomain.h>
#include	<uinfo.h>
#include	<umask.h>
#include	<unameo.h>
#include	<upt.h>
#include	<userattrdb.h>
#include	<userports.h>
#include	<usyscalls.h>
#include	<usysdefs.h>
#include	<usysrets.h>
#include	<uterm.h>
#include	<utypealiases.h>
#include	<utypedefs.h>
#include	<varray.h>
#include	<vars.h>
#include	<vec.h>
#include	<vstrcmp.h>
#include	<vstrkeycmp.h>
#include	<willaddover.h>
#include	<xfile.h>


EXTERNC_begin

/* system group */
extern int	uc_nprocs(int) noex ;

/* time group */
extern int	uc_clockset(clockid_t,const TIMESPEC *) noex ;
extern int	uc_clockget(clockid_t,TIMESPEC *) noex ;
extern int	uc_clockres(clockid_t,TIMESPEC *) noex ;

/* process group */
extern int	uc_atfork(void_f,void_f,void_f) noex ;

extern int	uc_execve(cchar *,mainv,mainv) noex ;
extern int	uc_isaexecve(cchar *,mainv,mainv) noex ;
extern int	uc_initgroups(cchar *,gid_t) noex ;
extern int	uc_exit(int) noex ;
extern int	uc_atexit(void_f) noex ;

extern int	uc_swapcontext(ucontext_t *,const ucontext_t *) noex ;
extern int	uc_msync(caddr_t,size_t,int) noex ;

extern int	uc_setpriority(int,id_t,int) noex ;
extern int	uc_procpid(cchar *,uid_t) noex ;

/* double-special open group */
extern int	uc_pipe(int *) noex ;
extern int	uc_pipes(int *,int) noex ;
extern int	uc_piper(int *,int,int) noex ;
extern int	uc_sockpair(int,int,int,int *) noex ;

/* operate group */
extern int	uc_mknod(cchar *,mode_t,dev_t) noex ;
extern int	uc_statvfs(cchar *,USTATVFS *) noex ;
extern int	uc_stat(cchar *,ustat *) noex ;
extern int	uc_lstat(cchar *,ustat *) noex ;
extern int	uc_readlink(cchar *,char *,int) noex ;
extern int	uc_chmod(cchar *,mode_t) noex ;
extern int	uc_chown(cchar *,uid_t,gid_t) noex ;
extern int	uc_rename(cchar *,cchar *) noex ;
extern int	uc_realpath(cchar *,char *) noex ;
extern int	uc_resolvepath(cchar *,char *,int) noex ;
extern int	uc_truncate(cchar *,off_t) noex ;

/* use group */
extern int	uc_fcntl(int,int,...) noex ;
extern int	uc_ioctl(int,int,...) noex ;
extern int	uc_fpassfd(int,int) noex ;
extern int	uc_ftruncate(int,off_t) noex ;
extern int	uc_fstat(int,ustat *) noex ;
extern int	uc_fchown(int,uid_t,gid_t) noex ;
extern int	uc_isatty(int) noex ;
extern int	uc_fsync(int) noex ;
extern int	uc_fsyncdata(int) noex ;
extern int	uc_fattach(int,cchar *) noex ;
extern int	uc_fdetach(cchar *) noex ;
extern int	uc_minmod(cchar *,mode_t) noex ;
extern int	uc_fminmod(int,mode_t) noex ;
extern int	uc_fsize(int) noex ;
extern int	uc_fuid(int) noex ;
extern int	uc_fgid(int) noex ;
extern int	uc_seeko(int,off_t,int,off_t *) noex ;
extern int	uc_seek(int,off_t,int) noex ;
extern int	uc_tell(int,off_t *) noex ;
extern int	uc_rewind(int) noex ;

extern int	uc_copy(int,int,int) noex ;
extern int	uc_peek(int,void *,int) noex ;
extern int	uc_keepalive(int,int) noex ;
extern int	uc_moveup(int,int) noex ;
extern int	uc_ndelay(int,int) noex ;
extern int	uc_nonblock(int,int) noex ;
extern int	uc_msgdiscard(int) noex ;
extern int	uc_setappend(int,int) noex ;
extern int	uc_closeonexec(int,int) noex ;
extern int	uc_close(int) noex ;

extern int	uc_remove(cchar *) noex ;
extern int	uc_link(cchar *,cchar *) noex ;
extern int	uc_symlink(cchar *,cchar *) noex ;
extern int	uc_unlink(cchar *) noex ;
extern int	uc_mkdir(cchar *,mode_t) noex ;
extern int	uc_rmdir(cchar *) noex ;
extern int	uc_access(cchar *,int) noex ;
extern int	uc_pathconf(cchar *,int,long *) noex ;

/* special */
extern int	uc_safesleep(int) noex ;
extern int	uc_ttyname(int,char *,int) noex ;
extern int	uc_mkfifo(cchar *,mode_t) noex ;

/* project related */
extern int	uc_inproj(cchar *,cchar *,char *,int) noex ;

/* POSIX® shared memory operations */
extern int	uc_unlinkshm(cchar *) noex ;

/* POSIX® semaphore */
extern int	uc_unlinksem(cchar *) noex ;

EXTERNC_end


#endif /* UCLIBSUBS_INCLUDE */


