# MAKEFILE (libuc)

T= libuc

ALL= $(T).so $(T).a


BINDIR= $(REPOROOT)/bin
INCDIR= $(REPOROOT)/include
LIBDIR= $(REPOROOT)/lib
MANDIR= $(REPOROOT)/man

INFODIR= $(REPOROOT)/info
HELPDIR= $(REPOROOT)/share/help
LDRPATH= $(REPOROOT)/lib

CRTDIR= $(CGS_CRTDIR)
VALDIR= $(CGS_VALDIR)


CPP= cpp
CC= gcc
CXX= gpp
LD= gld
RANLIB= granlib
AR= gar
NM= gnm
COV= gcov

LORDER= lorder
TSORT= tsort
LINT= lint
RM= rm -f
TOUCH= touch
LINT= lint


DEFS +=


INCS += usystem.h localmisc.h
INCS += uclibsubs.h


LIBS=


INCDIRS=

LIBDIRS= -L$(LIBDIR)


# flag setting
CPPFLAGS= $(DEFS) $(INCDIRS) $(MAKECPPFLAGS)
CFLAGS= $(MAKECFLAGS)
CCFLAGS= $(MAKECCFLAGS)
ARFLAGS= $(MAKEARFLAGS)
LDFLAGS= $(MAKELDFLAGS)


OBJ001= uclibmemalloc.o ucmemalloc.o ucmallocx.o
OBJ002= uc_open.o uc_openinfo.o 
OBJ003= uc_opensocket.o uc_openproto.o uc_openprog.o uc_openpass.o uc_openuser.o
OBJ004= uc_openfsvc.o uc_openfint.o uc_openusvc.o uc_opendialer.o
OBJ005= uc_opensys.o uc_opendev.o uc_create.o uc_createfile.o uc_joinus.o
OBJ006= uc_readn.o uc_reade.o uc_readline.o 
OBJ007= uc_write.o uc_writen.o uc_writefile.o uc_writedesc.o
OBJ008= uc_recve.o uc_recvfrome.o uc_recvmsge.o
OBJ009= uc_remove.o uc_connecte.o uc_stat.o uc_lstat.o uc_statvfs.o uc_fstat.o
OBJ010= uc_fsync.o uc_fdatasync.o uc_fchmodsuid.o uc_chmodsuid.o uc_fsize.o
OBJ011= uc_moveup.o uc_closeonexec.o uc_setappend.o uc_keepalive.o
OBJ012= uc_nonblock.o uc_ndelay.o uc_msgdiscard.o uc_reuseaddr.o uc_linger.o
OBJ013=
OBJ014= uc_fattach.o uc_fdetach.o ucmkfifo.o uc_msync.o
OBJ015= uc_accepte.o uc_lockf.o uc_lockfile.o uc_lockend.o uc_fpassfd.o
OBJ016= uc_close.o
OBJ017= uc_realpath.o uc_readlink.o uc_rename.o
OBJ018= uc_getcwd.o uc_initgroups.o uc_getnetname.o uc_procpid.o
OBJ019= uc_chmod.o uc_chown.o
OBJ020= uc_getproto.o uc_getnet.o uc_gethost.o uc_getserv.o 
OBJ021= uc_getipnodeby.o uc_getnameinfo.o uc_getaddrinfo.o 
OBJ022=
OBJ023= uc_ftime.o uc_mktime.o uc_clock.o
OBJ024= uc_getlogin.o uc_getrusage.o uc_getloadavg.o
OBJ025= uc_system.o uc_localtime.o uc_gmtime.o ucztime.o uc_sigset.o 
OBJ026=
OBJ027= uc_raise.o uc_killpg.o uc_sigqueue.o uc_syncer.o
OBJ028= uc_sigwaitinfo.o uc_sigpause.o
OBJ029= uc_sigdefault.o uc_sigignore.o uc_sighold.o uc_sigrelease.o
OBJ030= uc_settimeofday.o uc_gettimeofday.o

OBJ031=
OBJ032= ucgetdefaultproj.o ucinproj.o 
OBJ033= ucgetuserattr.o uckvamatch.o

OBJ034= ucnprocs.o ucinetconv.o
OBJ035= uc_cfsetispeed.o uc_cfsetospeed.o
OBJ036= uc_openpt.o uc_grantpt.o uc_unlockpt.o uc_ptsname.o uc_ttyname.o
OBJ037= uc_truncate.o uc_ftruncate.o
OBJ038=
OBJ039= uc_safesleep.o uc_exit.o 
OBJ040= uc_setpriority.o uc_getpriority.o 
OBJ041= uc_openshm.o uc_openshmto.o uc_unlinkshm.o 
OBJ042= uc_openpsem.o uc_unlinkpsem.o
OBJ043= uc_isatty.o uc_fminmod.o uc_minmod.o uc_link.o uc_symlink.o uc_unlink.o 
OBJ044= uc_mkdir.o uc_rmdir.o uc_access.o uc_utime.o uc_utimes.o
OBJ045= 
OBJ046= uc_madvise.o uc_mlock.o uc_munlock.o uc_mlockall.o uc_munlockall.o
OBJ047= uc_plock.o 
OBJ048= uc_fork.o uc_execve.o uc_isaexecve.o 
OBJ049= uc_gethostid.o uc_confstr.o uc_gethostname.o
OBJ050= uc_mallocstrw.o uc_mallocbuf.o
OBJ051= uc_waitwritable.o
OBJ052= 
OBJ053= uc_piper.o
OBJ054=
OBJ055=
OBJ056= uc_libmemalloc.o
OBJ057= uc_getrandom.o uc_getsocktype.o
OBJ058=
OBJ059= pt_sigmask.o pt_atfork.o
OBJ060=
OBJ061= getrandom.o
OBJ062= bufsizevar.o digbfsizes.o
OBJ063=

OBJ064= getprotofamily.o geteaddrinfo.o getaf.o getaflen.o getpwd.o
OBJ065= getnodedomain.o getutmpent.o getax.o 
OBJ066= getbufsize.o getbufsizenames.o gethz.o getngroups.o getproto.o getserv.o
OBJ067= getusername.o getuserhome.o getgroupname.o getprojname.o
OBJ068= getportnum.o getenver.o getourenv.o
OBJ069= nextpowtwo.o ndigits.o ipow.o llpow.o findbit.o
OBJ070= isFailOpen.o isFailConn.o
OBJ071= isnot.o iserror.o isoneof.o

OBJ072= 
OBJ073= inetpton.o xperm.o getprogpath.o findxfile.o xfile.o
OBJ074=
OBJ075=
OBJ076=
OBJ077=
OBJ078= pta.o ptm.o ptma.o ptc.o ptca.o psem.o lockrw.o envhelp.o
OBJ079= storeitem.o storebuf.o buffer.o field.o char.o mkprogenv.o

OBJ080= fsdir.o pmq.o sigblock.o userattr.o filemap.o uname.o 
OBJ081=
OBJ082= envlist.o hdb.o lookaside.o ids.o linefold.o nulstr.o 
OBJ083= dirseen.o dirseen_not.o
OBJ084= strpack.o strmgr.o strtab.o
OBJ085= varray.o randomvar.o sigign.o ucsem.o
OBJ086= sbuf.o
OBJ087= hostinfo.o hostaddr.o hostent.o inetaddr.o sockaddress.o ucpwcache.o

OBJ088= strsub.o
OBJ089=
OBJ090=
OBJ091=
OBJ092=
OBJ093=
OBJ094= strnxcmp.o
OBJ095=

OBJ096=
OBJ097=
OBJ098=
OBJ099=
OBJ100= pathadd.o pathclean.o
OBJ101=
OBJ102= mkvarpath.o mkuserpath.o mkcdpath.o mkexpandpath.o
OBJ103= mkquoted.o mksofname.o 

OBJ104= six.o rmx.o
OBJ105=
OBJ106=
OBJ107=
OBJ108=
OBJ109= matstr.o matostr.o matkeystr.o 
OBJ110= strkeycmp.o strwcmp.o
OBJ111= vstrcmpx.o vstrkeycmpx.o vstrkeydictcmp.o

OBJ112= strcpyxc.o
OBJ113=
OBJ114=
OBJ115= upt.o uptspawn.o tmtime.o spawnproc.o dupup.o
OBJ116= mkcleanline.o checkbase.o ncol.o tabexpand.o tabcols.o 
OBJ117= mapex.o calstrs.o
OBJ118=
OBJ119= toxc.o base64.o 

OBJ120= uinfo.o umask.o uxygetpw.o utmpacc.o ucproguser.o ucprogdata.o
OBJ121= nisdomainname.o
OBJ122= opentmpfile.o opensysfs.o opensys_banner.o
OBJ123= opensockaddr.o openaddrinfo.o
OBJ124=
OBJ125= getxid.o
OBJ126= passwdent.o shadowent.o groupent.o projectent.o userattrent.o
OBJ127= utmpaccent.o

OBJ128= sigaction.o sigevent.o 
OBJ129= timeval.o itimerspec.o timespec.o timeout.o
OBJ130= sigignores.o sigdefaults.o
OBJ131= isfiledesc.o isinetaddr.o isindomain.o
OBJ132=
OBJ133= nleadstr.o nleadkeystr.o
OBJ134= format.o
OBJ135= getrootdname.o getdig.o randlc.o

OBJ136= ctxxx.o ctbin.o ctoct.o ctdec.o cthex.o ctdecp.o ctdecf.o cvtdig.o
OBJ137= cfxxx.o cfbin.o cfoct.o cfhex.o cfdec.o cfdecmf.o cfdect.o cfxxxx.o 
OBJ138= cfnum.o cfhexs.o
OBJ139= digval.o
OBJ140=
OBJ141=
OBJ142=
OBJ143=

OBJ144= intceil.o intfloor.o intsat.o
OBJ145= ucontext_rtn.o
OBJ146= dialopts.o
OBJ147=
OBJ148= dialuss.o dialussmux.o dialussnls.o
OBJ149= dialtcp.o dialtcpmux.o dialtcpnls.o 
OBJ150= dialudp.o dialusd.o dialpass.o
OBJ151= dialprog.o dialhttp.o dialfinger.o

OBJ152= openshmtmp.o
OBJ153= mallocxx.o
OBJ154= strtoxlonglong.o
OBJ155=
OBJ156=
OBJ157=
OBJ158=
OBJ159=

OBJAa= $(OBJ000) $(OBJ001) $(OBJ002) $(OBJ003)
OBJAb= $(OBJ004) $(OBJ005) $(OBJ006) $(OBJ007) 
OBJA= $(OBJAa) $(OBJAb)

OBJBa= $(OBJ008) $(OBJ009) $(OBJ010) $(OBJ011)
OBJBb= $(OBJ012) $(OBJ013) $(OBJ014) $(OBJ015)
OBJB= $(OBJBa) $(OBJBb)

OBJCa= $(OBJ016) $(OBJ017) $(OBJ018) $(OBJ019)
OBJCb= $(OBJ020) $(OBJ021) $(OBJ022) $(OBJ023)
OBJC= $(OBJCa) $(OBJCb)

OBJDa= $(OBJ024) $(OBJ025) $(OBJ026) $(OBJ027)
OBJDb= $(OBJ028) $(OBJ029) $(OBJ030) $(OBJ031)
OBJD= $(OBJDa) $(OBJDb)

OBJEa= $(OBJ032) $(OBJ033) $(OBJ034) $(OBJ035)
OBJEb= $(OBJ036) $(OBJ037) $(OBJ038) $(OBJ039)
OBJE= $(OBJEa) $(OBJEb)

OBJFa= $(OBJ040) $(OBJ041) $(OBJ042) $(OBJ043)
OBJFb= $(OBJ044) $(OBJ045) $(OBJ046) $(OBJ047)
OBJF= $(OBJFa) $(OBJFb)

OBJGa= $(OBJ048) $(OBJ049) $(OBJ050) $(OBJ051)
OBJGb= $(OBJ052) $(OBJ053) $(OBJ054) $(OBJ055)
OBJG= $(OBJGa) $(OBJGb)

OBJHa= $(OBJ056) $(OBJ057) $(OBJ058) $(OBJ059)
OBJHb= $(OBJ060) $(OBJ061) $(OBJ062) $(OBJ063)
OBJH= $(OBJHa) $(OBJHb)

OBJIa= $(OBJ064) $(OBJ065) $(OBJ066) $(OBJ067)
OBJIb= $(OBJ068) $(OBJ069) $(OBJ070) $(OBJ071)
OBJI= $(OBJIa) $(OBJIb)

OBJJa= $(OBJ072) $(OBJ073) $(OBJ074) $(OBJ075)
OBJJb= $(OBJ076) $(OBJ077) $(OBJ078) $(OBJ079)
OBJJ= $(OBJJa) $(OBJJb)

OBJKa= $(OBJ080) $(OBJ081) $(OBJ082) $(OBJ083)
OBJKb= $(OBJ084) $(OBJ085) $(OBJ086) $(OBJ087)
OBJK= $(OBJKa) $(OBJKb)

OBJLa= $(OBJ088) $(OBJ089) $(OBJ090) $(OBJ091)
OBJLb= $(OBJ092) $(OBJ093) $(OBJ094) $(OBJ095)
OBJL= $(OBJLa) $(OBJLb)

OBJMa= $(OBJ096) $(OBJ097) $(OBJ098) $(OBJ099)
OBJMb= $(OBJ100) $(OBJ101) $(OBJ102) $(OBJ103)
OBJM= $(OBJMa) $(OBJMb)

OBJNa= $(OBJ104) $(OBJ105) $(OBJ106) $(OBJ107)
OBJNb= $(OBJ108) $(OBJ109) $(OBJ110) $(OBJ111)
OBJN= $(OBJNa) $(OBJNb)

OBJOa= $(OBJ112) $(OBJ113) $(OBJ114) $(OBJ115)
OBJOb= $(OBJ116) $(OBJ117) $(OBJ118) $(OBJ119)
OBJO= $(OBJOa) $(OBJOb)

OBJPa= $(OBJ120) $(OBJ121) $(OBJ122) $(OBJ123)
OBJPb= $(OBJ124) $(OBJ125) $(OBJ126) $(OBJ127)
OBJP= $(OBJPa) $(OBJPb)

OBJQa= $(OBJ128) $(OBJ129) $(OBJ130) $(OBJ131)
OBJQb= $(OBJ132) $(OBJ133) $(OBJ134) $(OBJ135)
OBJQ= $(OBJQa) $(OBJQb)

OBJRa= $(OBJ136) $(OBJ137) $(OBJ138) $(OBJ139)
OBJRb= $(OBJ140) $(OBJ141) $(OBJ142) $(OBJ143)
OBJR= $(OBJRa) $(OBJRb)

OBJSa= $(OBJ144) $(OBJ145) $(OBJ146) $(OBJ147)
OBJSb= $(OBJ148) $(OBJ149) $(OBJ150) $(OBJ151)
OBJS= $(OBJSa) $(OBJSb)

OBJTa= $(OBJ152) $(OBJ153) $(OBJ154) $(OBJ155)
OBJTb= $(OBJ156) $(OBJ157) $(OBJ158) $(OBJ159)
OBJT= $(OBJTa) $(OBJTb)


OBJg0= $(OBJA) $(OBJB) $(OBJC) $(OBJD) $(OBJE) $(OBJF) $(OBJG) $(OBJH) 
OBJg1= $(OBJI) $(OBJJ) $(OBJK) $(OBJL) $(OBJM) $(OBJN) $(OBJO) $(OBJP)
OBJg2= $(OBJQ) $(OBJR) $(OBJS) $(OBJT)

OBJ= $(OBJg0) $(OBJg1) $(OBJg2)


UOBJ00= matxstr.o toxc.o char.o 
UOBJ01= strn.o strnxcmp.o sfx.o six.o rmx.o sif.o
UOBJ02= snwcpy.o strcpyxc.o strwcpy.o strdcpy.o
UOBJ03= varnames.o syswords.o stdfnames.o

UOBJ04= isnot.o isoneof.o hasx.o ischarx.o 
UOBJ05= nleadstr.o nleadkeystr.o
UOBJ06= mapex.o strnul.o
UOBJ07= getourenv.o

UOBJ08= strkeycmp.o strwcmp.o
UOBJ09= readln.o ccfile.o
UOBJ10= cfx.o sncpyx.o sncpyxc.o sncpyxw.o
UOBJ11= strmgr.o strop.o field.o

UOBJ12= ufstat.o ucttyname.o
UOBJ13=
UOBJ14= ucsys.o
UOBJ15=

UOBJ=
UOBJ+= $(UOBJ00) $(UOBJ01) $(UOBJ02) $(UOBJ03) 
UOBJ+= $(UOBJ04) $(UOBJ05) $(UOBJ06) $(UOBJ07)
UOBJ+= $(UOBJ08) $(UOBJ09) $(UOBJ10) $(UOBJ11)
UOBJ+= $(UOBJ12) $(UOBJ13) $(UOBJ14) $(UOBJ15)


.SUFFIXES:		.ls .i .cx .cs


default:		all

all:			$(ALL)

so:			$(T).so

a:			$(T).a

.c.ln:
	$(LINT) -c $(LINTFLAGS) $(CPPFLAGS) $<

.c.ls:
	$(LINT) $(LINTFLAGS) $(CPPFLAGS) $<

.c.i:
	$(CPP) $(CPPFLAGS) $< > $(*).i

.cc.ii:
	$(CPP) $(CPPFLAGS) $< > $(*).ii

.c.s:
	$(CC) -S $(CPPFLAGS) $(CFLAGS) $<

.cc.s:
	$(CXX) -S $(CPPFLAGS) $(CCFLAGS) $<

.c.o:
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $<

.cc.o:
	$(CXX) -c $(CPPFLAGS) $(CCFLAGS) $<


libmacuser.a:		$(UOBJ)
	$(AR) -rc $@ $?

$(TT).a:		$(TOBJ)
	$(AR) -rc $@ $?

$(T).so:		$(SOBJ) Makefile $(T).a
	$(LD) -G -o $@ $(SLDFLAGS) $(SOBJ) $(SLIBINFO) > $(T).lm

$(T).a:			$(OBJ)
	$(AR) -rc $@ $?

$(T).nm:		$(T).so
	$(NM) $(NMFLAGS) $(T).so > $(T).nm

$(T).order:		$(OBJ) $(T).a
	$(LORDER) $(T).a | $(TSORT) > $(T).order
	$(RM) $(T).a
	while read O ; do $(AR) -cr $(T).a $${O} ; done < $(T).order

intall-pre:
	filefind . -s h | makenewer -af - -d $(INCDIR)

install:		$(ALL) Makefile install-incs
	ranlib $(T).a
	makenewer -r $(ALL) $(LIBDIR)

install-incs:
	makenewer -im -z -m o=h $(OBJA) -d $(INCDIR)
	makenewer -im -z -m o=h $(OBJB) -d $(INCDIR)
	makenewer -im -z -m o=h $(OBJC) -d $(INCDIR)
	makenewer -im -z -m o=h $(OBJD) -d $(INCDIR)
	makenewer -im -z -m o=h $(OBJE) -d $(INCDIR)
	makenewer -im -z -m o=h $(OBJF) -d $(INCDIR)
	makenewer -im -z -m o=h $(OBJG) -d $(INCDIR)
	makenewer -im -z -m o=h $(OBJH) -d $(INCDIR)
	makenewer -im -z -m o=h $(OBJI) -d $(INCDIR)
	makenewer -im -z -m o=h $(OBJJ) -d $(INCDIR)
	makenewer -im -z -m o=h $(OBJK) -d $(INCDIR)
	makenewer -im -z -m o=h $(OBJL) -d $(INCDIR)
	makenewer -im -z -m o=h $(OBJM) -d $(INCDIR)
	makenewer -im -z -m o=h $(OBJN) -d $(INCDIR)
	makenewer -im -z -m o=h $(OBJO) -d $(INCDIR)
	makenewer -im -z -m o=h $(OBJP) -d $(INCDIR)
	makenewer -im -z -m o=h $(OBJQ) -d $(INCDIR)
	makenewer -im -z -m o=h $(OBJR) -d $(INCDIR)
	makenewer -im -z -m o=h $(OBJS) -d $(INCDIR)
	makenewer -im -z -m o=h $(OBJT) -d $(INCDIR)

safe:
	makesafe -v=3 -I $(INCDIR) $(OBJA)
	makesafe -v=3 -I $(INCDIR) $(OBJB)
	makesafe -v=3 -I $(INCDIR) $(OBJC)
	makesafe -v=3 -I $(INCDIR) $(OBJD)
	makesafe -v=3 -I $(INCDIR) $(OBJE)
	makesafe -v=3 -I $(INCDIR) $(OBJF)
	makesafe -v=3 -I $(INCDIR) $(OBJG)
	makesafe -v=3 -I $(INCDIR) $(OBJH)
	makesafe -v=3 -I $(INCDIR) $(OBJI)
	makesafe -v=3 -I $(INCDIR) $(OBJJ)
	makesafe -v=3 -I $(INCDIR) $(OBJK)
	makesafe -v=3 -I $(INCDIR) $(OBJL)
	makesafe -v=3 -I $(INCDIR) $(OBJM)
	makesafe -v=3 -I $(INCDIR) $(OBJN)
	makesafe -v=3 -I $(INCDIR) $(OBJO)
	makesafe -v=3 -I $(INCDIR) $(OBJP)
	makesafe -v=3 -I $(INCDIR) $(OBJQ)
	makesafe -v=3 -I $(INCDIR) $(OBJR)
	makesafe -v=3 -I $(INCDIR) $(OBJS)
	makesafe -v=3 -I $(INCDIR) $(OBJT)

again:
	rm -f $(ALL)

clean:
	makeclean $(ALL)

control:
	(uname -n ; date) > Control


# base
OBJ0_BASE= varnames.o syswords.o valuelims.o digbufsizes.o ucvariables.o
OBJ1_BASE= ucsysconf.o 
OBJ2_BASE= bufsizedata.o getbufsizenames.o
OBJ3_BASE= getbufsize.o bufsizevar.o 

OBJ_BASE= obj0_base.o obj1_base.o obj2_base.o obj3_base.o

obj0_base.o:	$(OBJ0_BASE)
	$(LD) -r -o $@ $(OBJ0_BASE)

obj1_base.o:	$(OBJ1_BASE)
	$(LD) -r -o $@ $(OBJ1_BASE)

obj2_base.o:	$(OBJ2_BASE)
	$(LD) -r -o $@ $(OBJ2_BASE)

obj3_base.o:	$(OBJ3_BASE)
	$(LD) -r -o $@ $(OBJ3_BASE)

base.o:		$(OBJ_BASE)
	$(LD) -r -o $@ $(OBJ_BASE)


# CHARACTER
OBJA_CHAR= toxc.o char.o rmeol.o
OBJB_CHAR= ischarx.o hasx.o

OBJ_CHAR= obja_char.o objb_char.o

obja_char.o:	$(OBJA_CHAR)
	$(LD) -r -o $@ $(OBJA_CHAR)

objb_char.o:	$(OBJB_CHAR)
	$(LD) -r -o $@ $(OBJB_CHAR)

character.o:	$(OBJ_CHAR)
	$(LD) -r -o $@ $(OBJ_CHAR)


# LIBU
ugetloadavg.o:		ugetloadavg.cc ugetloadavg.h

# LIBUC
ucsysmisc.o:		ucsysmisc.cc
ucnprocs.o:		ucnprocs.cc
ucpathconf.o:		ucpathconf.cc
ucmain.o:		ucmain.cc
ucatfork.o:		ucatfork.cc
ucatexit.o:		ucatexit.cc
ucfork.o:		ucfork.cc
ucgetloadavg.o:		ucgetloadavg.cc ucgetloadavg.h ugetloadavg.h
ucygetpw.o:		ucygetpw.cc ucygetpw.h ucpwcache.h recarr.h
ucgetpid.o:		ucgetpid.cc ucgetpid.h
ucproguser.o:		ucproguser.cc ucproguser.h
ucprogdata.o:		ucprogdata.cc ucprogdata.h
ucclustername.o:	ucclustername.cc ucclustername.h
uclibmemalloc.o:	uclibmemalloc.cc uclibmemalloc.h
ucpwcache.o:		ucpwcache.cc ucpwcache.h recarr.h

# UNIX C-language system library string-to-integer interface
ucstrtox.o:		ucstrtox.cc ucstrtox.h strtox.h
ucstrtod.o:		ucstrtod.cc ucstrtod.h

# string-to-x
ucstr.o:	ucstrtox.o ucstrtod.o
ucsysconf.o:	ucsysconf.cc

# uctimeout (time-out call-backs)
uctimeout.o:	uctimeout.cc


obja.o:			$(OBJA)
	$(LD) -r -o $@ $(RLDFLAGS) $(OBJA)

objb.o:			$(OBJB)
	$(LD) -r -o $@ $(RLDFLAGS) $(OBJB)

objc.o:			$(OBJC)
	$(LD) -r -o $@ $(RLDFLAGS) $(OBJC)

objd.o:			$(OBJD)
	$(LD) -r -o $@ $(RLDFLAGS) $(OBJD)

obje.o:			$(OBJE)
	$(LD) -r -o $@ $(RLDFLAGS) $(OBJE)

objf.o:			$(OBJF)
	$(LD) -r -o $@ $(RLDFLAGS) $(OBJF)

objg.o:			$(OBJG)
	$(LD) -r -o $@ $(RLDFLAGS) $(OBJG)

objh.o:			$(OBJH)
	$(LD) -r -o $@ $(RLDFLAGS) $(OBJH)

obji.o:			$(OBJI)
	$(LD) -r -o $@ $(RLDFLAGS) $(OBJI)

objj.o:			$(OBJJ)
	$(LD) -r -o $@ $(RLDFLAGS) $(OBJJ)

objk.o:			$(OBJK)
	$(LD) -r -o $@ $(RLDFLAGS) $(OBJK)

objl.o:			$(OBJL)
	$(LD) -r -o $@ $(RLDFLAGS) $(OBJL)

objm.o:			$(OBJM)
	$(LD) -r -o $@ $(RLDFLAGS) $(OBJM)

objn.o:			$(OBJN)
	$(LD) -r -o $@ $(RLDFLAGS) $(OBJN)

objo.o:			$(OBJO)
	$(LD) -r -o $@ $(RLDFLAGS) $(OBJO)

objp.o:			$(OBJP)
	$(LD) -r -o $@ $(RLDFLAGS) $(OBJP)

objq.o:			$(OBJQ)
	$(LD) -r -o $@ $(RLDFLAGS) $(OBJQ)

objr.o:			$(OBJR)
	$(LD) -r -o $@ $(RLDFLAGS) $(OBJR)

objs.o:			$(OBJS)
	$(LD) -r -o $@ $(RLDFLAGS) $(OBJS)

objt.o:			$(OBJT)
	$(LD) -r -o $@ $(RLDFLAGS) $(OBJT)


uc_safesleep.o:		uc_safesleep.c

ucgetpuid.o:		ucgetpuid.cc

uc_openinfo.o:		uc_openinfo.c opensysfs.h
uc_openuser.o:		uc_openuser.c opensysfs.h

uc_timeout.o:		uc_timeout.cc timeout.h
	$(GPP) -c $(GPPFLAGS) $(CFLAGS) $(CPPFLAGS) uc_timeout.cc


# FSDIR
posixdirent.o:		posixdirent.cc posixdirent.hh
fsdir.o:		fsdir.cc fsdir.h
sunos_fsdir.o:		sunos_fsdir.cc sunos_fsdir.h posixdirent.hh
darwin_fsdir.o:		darwin_fsdir.cc darwin_fsdir.h posixdirent.hh

# POSIX Thread Attribute
pta.o:			pta.cc pta.h

# POSIX Thread Mutex Attribute
ptma.o:			ptma.cc ptma.h

# POSIX Thread Condition Attribute
ptca.o:			ptca.cc ptca.h

# POSIX Thread Mutex
ptm.o:			ptm.cc ptm.h

# POSIX Thread Condition Variable
ptc.o:			ptc.cc ptc.h

# POSIX Semaphore
psem.o:			psem.cc psem.h

# POSIX Read-Write Lock (broken in dynamically loadable modeules)
ptrwlock.o:		ptrwlock.cc ptrwlock.h

# POSIX Thread Read-Write Lock Attribute
ptrwa.o:		ptrwa.cc ptrwa.h

# POSIX Message Queue
pmq.o:			pmq.cc pmq.h

sighand.o:		sighand.cc sighand.h
sigblock.o:		sigblock.cc sigblock.h
sigign.o:		sigign.cc sigign.h

hostinfo.o:		hostinfo.cc hostinfo.h
hostaddr.o:		hostaddr.cc hostaddr.h
hostent.o:		hostent.cc hostent.h
inetaddr.o:		inetaddr.cc inetaddr.h
sockaddress.o:		sockaddress.cc sockaddress.h

storeitem.o:		storeitem.cc storeitem.h
storebuf.o:		storebuf.cc storebuf.h

obuf.o:			obuf.cc obuf.hh

baops.o:		baops.c baops.h
strop.o:		strop.cc strop.h
dstr.o:			dstr.cc dstr.h

varray.o:		varray.cc varray.h

hdb.o:			hdb.cc hdb.h
lookaside.o:		lookaside.cc lookaside.h
linefold.o:		linefold.cc linefold.h

envlist.o:		envlist.cc envlist.h
envhelp.o:		envhelp.cc envhelp.h

mkpr.o:			mkpr.cc
mkprogenv.o:		mkprogenv.cc mkprogenv.h

spawner.o:		spawner.cc spawner.h

nulstr.o:		nulstr.cc nulstr.h

lockrw.o:		lockrw.cc lockrw.h
csem.o:			csem.cc csem.h
ucsem.o:		ucsem.cc ucsem.h

fdliner.o:		fdliner.cc fdliner.h
fbliner.o:		fbliner.cc fbliner.h

dirlist.o:		dirlist.cc dirlist.h

dirseen.o:		dirseen.cc dirseen.h
dirseen_not.o:		dirseen_not.cc dirseen.h

randomvar.o:		randomvar.cc randomvar.h

strpack.o:		strpack.cc strpack.h
strtab.o:		strtab.cc strtab.h
strstore.o:		strstore.cc strstore.h
strmgr.o:		strmgr.cc strmgr.h

serialbuf.o:		serialbuf.cc serialbuf.h stdorder.h
stdorder.o:		stdorder.cc stdorder.h


getbufsize.o:		getbufsize.cc getbufsize.h getbufsizenames.h usysdefs.h
getbufsize.o:		getbufsizenames.h usysdefs.h bufsizedata.hh

getbufsizenames.o:	getbufsizenames.c getbufsizenames.h
varnames.o:		varnames.cc varnames.hh
syswords.o:		syswords.cc syswords.hh
valuelims.o:		valuelims.cc valuelims.hh
digbufsizes.o:		digbufsizes.cc digbufsizes.hh
bufsizedata.o:		bufsizedata.cc bufsizedata.hh
bufsizevar.o:		bufsizevar.cc bufsizevar.hh

mapex.o:		mapex.cc mapex.h

sigevent.o:		sigevent.cc sigevent.h

timeval.o:		timeval.cc timeval.h
timespec.o:		timespec.cc timespec.h
itimerspec.o:		itimerspec.cc itimerspec.h
timeout.o:		timeout.cc timeout.h

upt.o:			upt.cc upt.h

spawnproc.o:		spawnproc.cc spawnproc.h

getusername.o:		getusername.cc getusername.h
getuserhome.o:		getuserhome.cc getuserhome.h
getgroupname.o:		getgroupname.cc getgroupname.h
getprojname.o:		getprojname.cc getprojname.h

getutmpent.o:		getutmpent.cc getutmpent.h
mkutmpid.o:		mkutmpid.cc mkutmpid.h


memfile.o:		memfile.cc memfile.h
filemap.o:		filemap.cc filemap.h

numsign.o:		numsign.cc numsign.h


# GETXX
getax.o:		getax.cc getax.h
getpe.o:		getpe.cc getpe.h
getne.o:		getne.cc getne.h
gethe.o:		gethe.cc gethe.h
getse.o:		getse.cc getse.h

passwdent.o:		passwdent.cc passwdent.h
spwdent.o:		spwdent.cc spwdent.h
groupent.o:		groupent.cc groupent.h
projectent.o:		projectent.cc projectent.h
userattrent.o:		userattrent.cc userattrent.h

matstr.o:		matstr.cc matstr.h
matostr.o:		matostr.cc matostr.h

sncpyarray.o:		sncpyarray.c sncpy.h
sncpyxc.o:		sncpyxc.cc sncpyxc.h
sncpyx.o:		sncpyx.cc sncpyx.h
sncpyxw.o:		sncpyxw.cc sncpyxw.h

mkpathx.o:		mkpathx.c mkpathx.h
mkpathxw.o:		mkpathxw.cc mkpathxw.h

mknpathx.o:		mknpathx.cc mknpathx.h
mknpathxw.o:		mknpathxw.cc mknpathxw.h

mkvarpath.o:		mkvarpath.cc
mkuserpath.o:		mkuserpath.cc
mkcdpath.o:		mkcdpath.cc

strcpyxc.o:		strcpyxc.cc strcpyxc.h

opensysfs.o:		opensysfs.cc opensysfs.h

quoteshellarg.o:	quoteshellarg.c
mkquoted.o:		mkquoted.cc mkquoted.h
termconseq.o:		termconseq.cc termconseq.h

tmtime.o:		tmtime.cc tmtime.h

format.o:		format.cc format.h

strshrink.o:		strshrink.c
 
# digit-character management
ndigits.o:		ndigits.c
getdig.o:		getdig.cc getdig.h
checkbase.o:		checkbase.cc

# malloc
mallocxx.o:		mallocxx.cc mallocxx.h
mallocstuff.o:		mallocstuff.cc mallocstuff.h

# part of string-to-integer interface
strtox.o:		strtox.cc strtox.h

# UNIX C-language system library data-base interface (support)
spwd.o:			spwd.cc spwd.h
userattr.o:		userattr.cc userattr.h
ucpasswd.o:		ucpasswd.cc ucpasswd.h
ucgroup.o:		ucgroup.cc ucgroup.h
ucproject.o:		ucproject.cc ucproject.h

# UNIX C-language system library support
libmallocxx.o:		libmallocxx.cc libmallocxx.h

# UNIX C-language system library memory management
mapblock.o:		mapblock.cc mapblock.hh
memtrack.o:		memtrack.cc memtrack.hh mapblock.hh
ucmemalloc.o:		ucmemalloc.cc ucmemalloc.h memtrack.hh mapblock.hh
ucmallocx.o:		ucmallocx.cc ucmallocx.h

# UNIX C-language system library timer management
uctimer.o:		uctimer.cc uctimer.h

# misc-character
toxc.o:			toxc.c toxc.h
char.o:			char.cc char.h
ischarx.o:		ischarx.cc ischarx.h
hasx.o:			hasx.cc hasx.h char.h ischarx.h

# RMX
rmx.o:			rmx.cc rmx.h

# STRWCPY
strwcpy.o:		strwcpy.dir
strwcpy.dir:
	makesubdir $@

# STRDCPY
strdcpy.o:		strdcpy.dir
strdcpy.dir:
	makesubdir $@

# system-auxillary-information
ucsysauxinfo.o:		ucsysauxinfo.dir
ucsysauxinfo.dir:
	makesubdir $@

open.o:			open.dir
open.dir:
	makesubdir $@

dial.o:			dial.dir
dial.dir:
	makesubdir $@

six.o:			six.dir
six.dir:
	makesubdir $@

sfx.o:			sfx.dir
sfx.dir:
	makesubdir $@

snx.o:			snx.dir
snx.dir:
	makesubdir $@

snwcpy.o:		snwcpy.dir
snwcpy.dir:
	makesubdir $@

# MATXSTR
matxstr.o:		matxstr.dir
matxstr.dir:
	makesubdir $@

# HASH
hash.o:			hash.dir
hash.dir:
	makesubdir $@

# UCSYS
ucsys.o:		ucsys.dir
ucsys.dir:
	makesubdir $@

# UCENT
ucent.o:		ucent.dir
ucent.dir:
	makesubdir $@

# UCGET
ucget.o:		ucget.dir
ucget.dir:
	makesubdir $@

# UCENUM
ucenum.o:		ucenum.dir
ucenum.dir:
	makesubdir $@

# STRN
strn.o:			strn.dir
strn.dir:
	makesubdir $@

# STRNXCMP
strnxcmp.o:		strnxcmp.dir
strnxcmp.dir:
	makesubdir $@

# VEC
vec.o:			vec.dir
vec.dir:
	makesubdir $@

# MKX
mkx.o:			mkx.dir
mkx.dir:
	makesubdir $@

# MKTMP
mktmp.o:		mktmp.dir
mktmp.dir:
	makesubdir $@

# MKFILE
mkfile.o:		mkfile.dir
mkfile.dir:
	makesubdir $@

# EMA
ema.o:		ema.dir
ema.dir:
	makesubdir $@

# QUEUE
queue.o:		queue.dir
queue.dir:
	makesubdir $@

# CFX
cfx.o:			cfx.dir
cfx.dir:
	makesubdir $@

# CTX
ctx.o:			ctx.dir
ctx.dir:
	makesubdir $@

# CHRONO
chrono.o:		chrono.dir
chrono.dir:
	makesubdir $@

# DATER
dater.o:		dater.dir
dater.dir:
	makesubdir $@

# LOCALSET
localset.o:		localset.dir
localset.dir:
	makesubdir $@

# LOCALGET
localget.o:		localget.dir
localget.dir:
	makesubdir $@

# SBUF
sbuf.o:			sbuf.dir
sbuf.dir:
	makesubdir $@

# BUFFER
buffer.o:		buffer.dir
buffer.dir:
	makesubdir $@

# ISMISC
ismisc.o:		ismisc.dir
ismisc.dir:
	makesubdir $@

# ISFILEDESC
isfiledesc.o:		isfiledesc.dir
isfiledesc.dir:
	makesubdir $@

# GETXX
getxx.o:		getxx.dir
getxx.dir:
	makesubdir $@

# FIELD
field.o:		field.dir
field.dir:
	makesubdir $@

# VARSUB
varsub.o:		varsub.dir
varsub.dir:
	makesubdir $@

# ENVS
envs.o:			envs.dir
envs.dir:
	makesubdir $@

# LOGFILE
logfile.o:		logfile.dir
logfile.dir:
	makesubdir $@

# FILER
filer.o:		filer.dir
filer.dir:
	makesubdir $@

# TMPX
tmpx.o:			tmpx.dir
tmpx.dir:
	makesubdir $@

# TIMESTR
timestr.o:		timestr.dir
timestr.dir:
	makesubdir $@

# STRLIST
strlistx.o:		strlistx.dir
strlistx.dir:
	makesubdir $@

# MAILALIS
mailalias.o:		mailalias.dir
mailalias.dir:
	makesubdir $@

# POW
pow.o:			pow.dir
pow.dir:
	makesubdir $@


# UCINET
ucinetconv.o:		ucinetconv.cc ucinetconv.h

# ENCODE-DECODE
b64decover.o:		b64decoder.cc b64decoder.h

# tab handling
tabcols.o:		tabcols.cc tabcols.h
ncol.o:			ncol.cc ncol.h tabcols.h
tabexpand.o:		tabexpand.cc tabexpand.h tabcols.h

# INET
inetconv.o:		inetconv.cc inetconv.h

# LIBUC
ucmemla.o:		ucmemla.cc ucmemla.h
ucpts.o:		ucpts.cc ucpts.h

# other
nodedb.o:		nodedb.cc	nodedb.h
expcook.o:		expcook.cc	expcook.h
keyopt.o:		keyopt.cc	keyopt.h
utf8decoder.o:		utf8decoder.cc	utf8decoder.h
termstr.o:		termstr.cc	termstr.h
td.o:			td.cc		td.h termstr.h
recip.o:		recip.cc	recip.h
querystr.o:		querystr.cc	querystr.h
absfn.o:		absfn.cc	absfn.h
ids.o:			ids.cc		ids.h
groupids.o:		groupids.cc	groupids.hh
realname.o:		realname.cc	realname.h
bits.o:			bits.cc		bits.h
userattrdb.o:		userattrdb.cc	userattrdb.h
hdb.o:			hdb.cc		hdb.h
pmq.o:			pmq.cc		pmq.h
filegrp.o:		filegrp.cc	filegrp.h
unameo.o:		unameo.cc	unameo.h
hostaddr.o:		hostaddr.cc	hostaddr.h
b64decoder.o:		b64decoder.cc	b64decoder.h
lookaside.o:		lookaside.cc	lookaside.h
strpack.o:		strpack.cc	strpack.h
strtab.o:		strtab.cc	strtab.h
strstore.o:		strstore.cc	strstore.h
expcook.o:		expcook.cc	expcook.h
hostinfo.o:		hostinfo.cc	hostinfo.h
paramfile.o:		paramfile.cc	paramfile.h
memfile.o:		memfile.cc	memfile.h
dirlist.o:		dirlist.cc	dirlist.h
varray.o:		varray.cc	varray.h
csem.o:			csem.cc		csem.h
pwcache.o:		pwcache.cc	pwcache.h
lockrw.o:		lockrw.cc	lockrw.h
nodedb.o:		nodedb.cc	nodedb.h
mailmsg.o:		mailmsg.cc	mailmsg.h
ccfile.o:		ccfile.cc	ccfile.hh
thrcomm.o:		thrcomm.cc	thrcomm.h
thrbase.o:		thrbase.cc	thrbase.h thrcomm.h
pwentry.o:		pwentry.cc	pwentry.h
pwfile.o:		pwfile.cc	pwfile.h
sif.o:			sif.cc		sif.hh


# sring-comparisons
vstrcmpx.o:		vstrcmpx.c vstrcmpx.h
vstrkeycmpx.o:		vstrkeycmpx.c vstrkeycmpx.h
vstrkeydictcmp.o:	vstrkeydictcmp.c vstrkeycmpx.h

# string-constants
stdfnames.o:		stdfnames.c stdfnames.h
syhsdbfnames.o:		sysdbfnames.c sysdbfnames.h
sysdbfname.o:		sysdbfname.cc sysdbfname.h

# PATH
pathadd.o:		pathadd.cc pathadd.h
pathclean.o:		pathclean.cc pathclean.h

# UTILITY
splitfname.o:		splitfname.cc splitfname.h
strtabfind.o:		strtabfind.cc strtabfind.h
findbit.o:		findbit.cc findbit.h
termtypemat.o:		termtypemat.cc termtypemat.h
termcmd.o:		termcmd.cc termcmd.h
matparam.o:		matparam.cc matparam.h
typenonpath.o:		typenonpath.cc typenonpath.h
getpwetry.o:		getpwentry.cc getpwentry.h pwentry.h
intsat.o:		intsat.cc intsat.h
intfloor.o:		intfloor.cc intfloor.h
intceil.o:		intceil.cc intceil.h
digval.o:		digval.cc digval.h
willaddover.o:		willaddover.cc willaddover.h
getxid.o:		getxid.cc getxid.h
getngroups.o:		getngroups.cc getngroups.h
getnodename.o:		getnodename.cc getnodename.h
getnodedomain.o:	getnodedomain.cc getnodename.h
getuserorg.o:		getuserorg.cc getuserorg.h
getrealname.o:		getrealname.cc getrealname.h
getourenv.o:		getourenv.cc getourenv.h
getaddr.o:		getaddr.cc getaddr.h
getmjd.o:		getmjd.cc getmjd.h
gethz.o:		gethz.cc gethz.h
getpassword.o:		getpassword.cc getpassword.h
inetaddrparse.o:	inetaddrparse.cc inetaddrparse.h
readln.o:		readln.cc readln.hh
strlibval.o:		strlibval.cc strlibval.hh
strenv.o:		strenv.cc strenv.hh
strwcpy.o:		strwcmp.cc strwcmp.h
isort.o:		isort.cc isort.h
xperm.o:		xperm.cc xperm.h ids.h
sysnoise.o:		sysnoise.cc sysnoise.h
findfilepath.o:		findfilepath.cc findfilepath.h
calstrs.o:		calstrs.cc calstrs.h
ipow.o:			ipow.cc ipow.h
base64.o:		base64.cc base64.h
#
isproc.o:		isproc.cc isproc.h
#
isinetaddr.o:		isinetaddr.cc isinetaddr.h
isindomain.o:		isindomain.cc isindomain.h
isoneof.o:		isoneof.cc isoneof.h
isnot.o:		isnot.cc isnot.h isoneof.h
iserror.o:		iserror.cc iserror.h isoneof.h

# emulated system kernel calls
uinfo.o:		uinfo.cc uinfo.h
umask.o:		umask.cc umask.h
unameo.o:		unameo.cc unameo.h
utmpacc.o:		utmpacc.cc utmpacc.h
utmpaccent.o:		utmpaccent.cc utmpaccent.h
ucrand.o:		ucrand.cc ucrand.h


