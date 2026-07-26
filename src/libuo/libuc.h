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
#include	<digval.h>
#include	<dirempty.h>
#include	<dirent.h>
#include	<dstr.h>
#include	<dupstr.h>
#include	<envs.h>
#include	<envx.h>
#include	<epsem.h>
#include	<errno.h>
#include	<estrings.h>
#include	<exitcodes.h>
#include	<expcook.h>
#include	<fibonacci.h>
#include	<field.h>
#include	<filegrp.h>
#include	<filemap.h>
#include	<filer.h>
#include	<findfilepath.h>
#include	<findxfile.h>
#include	<format.h>
#include	<fsdir.h>
#include	<fsdirtree.h>
#include	<gecos.h>
#include	<getexecname.h>
#include	<gethe.h>
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
#include	<inetaddrx.h>
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
#include	<mkdirs.h>
#include	<mkfile.h>
#include	<mklogid.h>
#include	<mkpathxx.h>
#include	<mktmp.h>
#include	<mkx.h>
#include	<modload.h>
#include	<namecache.h>
#include	<nchr.h>
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
#include	<ucontext.h>
#include	<ucdesc.h>
#include	<userattrdb.h>
#include	<userports.h>
#include	<ucfileop.h>
#include	<ucfork.h>
#include	<ucinet.h>
#include	<funcodes.h>
#include	<ucget.h>
#include	<ucgethost.h>
#include	<ucgetloadavg.h>
#include	<ucgetpid.h>
#include	<ucgetrandom.h>
#include	<ucgetx.h>
#include	<uckvamatch.h>
#include	<uclibmem.h>
#include	<ucmem.h>
#include	<ucmemla.h>
#include	<ucnprocs.h>
#include	<ucopen.h>
#include	<ucopeninfo.h>
#include	<ucpeek.h>
#include	<ucproc.h>
#include	<ucprochave.h>
#include	<ucproctypes.h>
#include	<ucprogdata.h>
#include	<ucproguser.h>
#include	<ucpts.h>
#include	<ucpts.h>		/* pseudo-terminal management */
#include	<ucpwcache.h>
#include	<ucrand.h>
#include	<ucsig.h>
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
#include	<uctimeconv.h>
#include	<uctimer.h>
#include	<ucuserattr.h>
#include	<ucutility.h>
#include	<ucinfo.h>
#include	<udomain.h>
#include	<umask.h>
#include	<unameo.h>
#include	<upt.h>
#include	<usyscalls.h>
#include	<usysdefs.h>
#include	<usysrets.h>
#include	<uterm.h>
#include	<utypealiases.h>
#include	<utypedefs.h>
#include	<varray.h>
#include	<vec.h>
#include	<vstrcmp.h>
#include	<vstrkeycmp.h>
#include	<willaddover.h>
#include	<xfile.h>


#endif /* UCLIBSUBS_INCLUDE */


