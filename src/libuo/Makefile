# MAKEFILE (libuo)

T= libuo

ALL= $(T).o ${T}.so
EXTRA ?= /usr/extra


BINDIR		?= $(REPOROOT)/bin
INCDIR		?= $(REPOROOT)/include
LIBDIR		?= $(REPOROOT)/lib
MANDIR		?= $(REPOROOT)/man
INFODIR		?= $(REPOROOT)/info
HELPDIR		?= $(REPOROOT)/share/help
CRTDIR		?= $(CGS_CRTDIR)
VALDIR		?= $(CGS_VALDIR)
RUNDIR		?= $(CGS_RUNDIR)

CPP		?= cpp
CC		?= gcc
CXX		?= gxx
LD		?= gld
RANLIB		?= granlib
AR		?= gar
NM		?= gnm
COV		?= gcov
LORDER		?= lorder
TSORT		?= tsort
LINT		?= lint
RM		?= rm -f
TOUCH		?= touch
LINT		?= lint


DEFS=

INCS= libuc.h

MODS += libutil.ccm argmgr.ccm strfilter.ccm sif.ccm 
MODS += tardir.ccm

LIBS= -lut -lu -lsecdb -lnss


OBJUC00= ucsys.o ucent.o ucget.o ucenum.o
OBJUC01= ucsig.o ucsysconf.o ucx.o 
OBJUC02= ucstrto.o uclibmem.o ucpwcache.o
OBJUC03= ucsupport.o ucstream.o ucpts.o

OBJUC04= ucdescbase.o ucdescmisc.o
OBJUC05= ucdescread.o ucdescwrite.o
OBJUC06= ucdescsock.o ucdesclock.o
OBJUC07= ucgetx.o ucsysmisc.o

OBJUC08= ucproc.o ucdata.o ucsysauxinfo.o
OBJUC09= ucfileop.o
OBJUC10= ucrand.o ucmem.o
OBJUC11= ucopen.o

OBJUC_A= objuc00.o objuc01.o objuc02.o objuc03.o
OBJUC_B= objuc04.o objuc05.o objuc06.o objuc07.o
OBJUC_C= objuc08.o objuc09.o objuc10.o objuc11.o

OBJUC= objuc_a.o objuc_b.o objuc_c.o

OBJ00_LUO= snwcpy.o
OBJ01_LUO= strn.o six.o mkx.o mnw.o
OBJ02_LUO= sfx.o rmx.o snx.o mkui.o
OBJ03_LUO= isx.o ismisc.o isfiledesc.o hasx.o 

OBJ04_LUO= sif.o ctx.o cfx.o
OBJ05_LUO= strcpyx.o strdcpy.o strx.o 
OBJ06_LUO= strxcmp.o
OBJ07_LUO= strw.o strwcpy.o

OBJ08_LUO= filer.o hdb.o hdbstr.o mklogid.o
OBJ09_LUO= strfilter.o paramfile.o
OBJ10_LUO= fsdirtree.o fsdir.o
OBJ11_LUO= queue.o field.o bits.o

OBJ12_LUO= tardir.o langx.o
OBJ13_LUO= nleadx.o matxstr.o 
OBJ14_LUO= char.o toxc.o
OBJ15_LUO= strnxcmp.o

OBJ16_LUO= checkbase.o matparam.o
OBJ17_LUO= bitrotate.o stdorder.o
OBJ18_LUO= ncolx.o hash.o permx.o
OBJ19_LUO= inetaddrx.o 

OBJ20_LUO= addrset.o ids.o gecos.o
OBJ21_LUO= sigobj.o pwentry.o serialbuf.o
OBJ22_LUO= lookaside.o dupstr.o comparse.o
OBJ23_LUO= vec.o recip.o posname.o

OBJ24_LUO= wsfx.o 
OBJ25_LUO= wsnx.o 
OBJ26_LUO= wsix.o
OBJ27_LUO= wswcpyx.o

OBJ28_LUO= bufsize.o openx.o open.o
OBJ29_LUO= sysval.o path.o termx.o
OBJ30_LUO= vstr.o xfile.o absfn.o
OBJ31_LUO= randlc.o pow.o calstrs.o digval.o

OBJ32_LUO= realname.o
OBJ33_LUO= ema.o tmpx.o getxname.o getutmp.o
OBJ34_LUO= sbuf.o buffer.o
OBJ35_LUO= storebuf.o storeitem.o

OBJ36_LUO= fmtflag.o fmtstr.o sysdbfiles.o
OBJ37_LUO= getx.o host.o gethe.o sysdbfn.o strlibval.o
OBJ38_LUO= utmpxx.o ucinfo.o caches.o optval.o
OBJ39_LUO= filex.o fileobj.o getxx.o bufos.o bufslide.o 

OBJ40_LUO= dirs.o dirseen.o sockaddress.o 
OBJ41_LUO= envx.o envs.o strpack.o strobj.o
OBJ42_LUO= chrono.o timestr.o lockfile.o
OBJ43_LUO= nodedb.o clusterdb.o varsub.o

OBJ44_LUO= kvsfile.o memtrack.o randomvar.o
OBJ45_LUO= nonpath.o msghdr.o conmsghdr.o
OBJ46_LUO= userattrdb.o userinfo.o expcook.o
OBJ47_LUO= spawner.o spawnproc.o

OBJ48_LUO= findxfile.o sigx.o dial.o conallof.o
OBJ49_LUO= argutils.o cmdutils.o logfile.o linex.o
OBJ50_LUO= ndig.o ndigit.o bufprintf.o
OBJ51_LUO=

OBJA_LUO= obj00_luo.o obj01_luo.o obj02_luo.o obj03_luo.o
OBJB_LUO= obj04_luo.o obj05_luo.o obj06_luo.o obj07_luo.o
OBJC_LUO= obj08_luo.o obj09_luo.o obj10_luo.o obj11_luo.o
OBJD_LUO= obj12_luo.o obj13_luo.o obj14_luo.o obj15_luo.o
OBJE_LUO= obj16_luo.o obj17_luo.o obj18_luo.o obj19_luo.o
OBJF_LUO= obj20_luo.o obj21_luo.o obj22_luo.o obj23_luo.o
OBJG_LUO= obj24_luo.o obj25_luo.o obj26_luo.o obj27_luo.o
OBJH_LUO= obj28_luo.o obj29_luo.o obj30_luo.o obj31_luo.o
OBJI_LUO= obj32_luo.o obj33_luo.o obj34_luo.o obj35_luo.o
OBJJ_LUO= obj36_luo.o obj37_luo.o obj38_luo.o obj39_luo.o
OBJK_LUO= obj40_luo.o obj41_luo.o obj42_luo.o obj43_luo.o
OBJL_LUO= obj44_luo.o obj45_luo.o obj46_luo.o obj47_luo.o
#OBJM_LUO= obj48_luo.o obj49_luo.o obj50_luo.o obj51_luo.o
OBJM_LUO= obj48_luo.o obj49_luo.o obj50_luo.o

OBJ_LUO += obja_luo.o objb_luo.o objc_luo.o objd_luo.o
OBJ_LUO += obje_luo.o objf_luo.o objg_luo.o objh_luo.o
OBJ_LUO += obji_luo.o objj_luo.o objk_luo.o objl_luo.o
OBJ_LUO += objm_luo.o

OBJ += objuc.o
OBJ += obj_luo.o


INCDIRS=

LIBDIRS= -L /usr/local/lib

RUNINFO= -rpath $(RUNDIR)
LIBINFO= $(LIBDIRS) $(LIBS)

# flag setting
CPPFLAGS	?= $(DEFS) $(INCDIRS) $(MAKECPPFLAGS)
CFLAGS		?= $(MAKECFLAGS)
CXXFLAGS	?= $(MAKECXXFLAGS)
ARFLAGS		?= $(MAKEARFLAGS)
LDFLAGS		?= $(MAKELDFLAGS)


.SUFFIXES:		.hh .ii .iim .ccm


default:		$(T).o

all:			$(ALL)

so:			$(T).so

a:			$(T).a


.c.i:
	$(CPP) $(CPPFLAGS) $< > $(*).i

.cc.ii:
	$(CPP) $(CPPFLAGS) $< > $(*).ii

.ccm.iim:
	$(CPP) $(CPPFLAGS) $< > $(*).iim

.c.s:
	$(CC) -S $(CPPFLAGS) $(CFLAGS) $<

.cc.s:
	$(CXX) -S $(CPPFLAGS) $(CXXFLAGS) $<

.c.o:
	$(COMPILE.c) $<

.cc.o:
	$(COMPILE.cc) $<

.ccm.o:
	gxx -c -x c++ -o $@ -O $<


$(T).so:		obj.o
	$(CXX) -shared -o $@ $(LDFLAGS) obj.o $(RUNINFO) $(LIBINFO)

$(T).o:			obj.o
	$(LD) -r -o $@ $(LDFLAGS) obj.o

$(T).a:			$(OBJ)
	$(AR) -rc $@ $?

$(T).nm:		$(T).o
	$(NM) $(NMFLAGS) $(T).o > $(T).nm

again:
	rm -f $(ALL)

clean:
	makeclean $(ALL)

control:
	(uname -n ; date) > Control

install:		$(T).x
	makeinstall $(T).x


objuc00.o:		$(OBJUC00)
	$(LD) -r $(LDFLAGS) -o $@ $^

objuc01.o:		$(OBJUC01)
	$(LD) -r $(LDFLAGS) -o $@ $^

objuc02.o:		$(OBJUC02)
	$(LD) -r $(LDFLAGS) -o $@ $^

objuc03.o:		$(OBJUC03)
	$(LD) -r $(LDFLAGS) -o $@ $^


objuc04.o:		$(OBJUC04)
	$(LD) -r $(LDFLAGS) -o $@ $^

objuc05.o:		$(OBJUC05)
	$(LD) -r $(LDFLAGS) -o $@ $^

objuc06.o:		$(OBJUC06)
	$(LD) -r $(LDFLAGS) -o $@ $^

objuc07.o:		$(OBJUC07)
	$(LD) -r $(LDFLAGS) -o $@ $^


objuc08.o:		$(OBJUC08)
	$(LD) -r $(LDFLAGS) -o $@ $^

objuc09.o:		$(OBJUC09)
	$(LD) -r $(LDFLAGS) -o $@ $^

objuc10.o:		$(OBJUC10)
	$(LD) -r $(LDFLAGS) -o $@ $^

objuc11.o:		$(OBJUC11)
	$(LD) -r $(LDFLAGS) -o $@ $^


objuc_a.o:		$(OBJUC_A)
	$(LD) -r $(LDFLAGS) -o $@ $^

objuc_b.o:		$(OBJUC_B)
	$(LD) -r $(LDFLAGS) -o $@ $^

objuc_c.o:		$(OBJUC_C)
	$(LD) -r $(LDFLAGS) -o $@ $^


objuc.o:		$(OBJUC)
	$(LD) -r $(LDFLAGS) -o $@ $^


obj00_luo.o:		$(OBJ00_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj01_luo.o:		$(OBJ01_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj02_luo.o:		$(OBJ02_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj03_luo.o:		$(OBJ03_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj04_luo.o:		$(OBJ04_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj05_luo.o:		$(OBJ05_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj06_luo.o:		$(OBJ06_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj07_luo.o:		$(OBJ07_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^


obj08_luo.o:		$(OBJ08_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj09_luo.o:		$(OBJ09_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj10_luo.o:		$(OBJ10_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj11_luo.o:		$(OBJ11_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj12_luo.o:		$(OBJ12_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj13_luo.o:		$(OBJ13_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj14_luo.o:		$(OBJ14_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj15_luo.o:		$(OBJ15_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^


obj16_luo.o:		$(OBJ16_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj17_luo.o:		$(OBJ17_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj18_luo.o:		$(OBJ18_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj19_luo.o:		$(OBJ19_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj20_luo.o:		$(OBJ20_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj21_luo.o:		$(OBJ21_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj22_luo.o:		$(OBJ22_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj23_luo.o:		$(OBJ23_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^


obj24_luo.o:		$(OBJ24_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj25_luo.o:		$(OBJ25_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj26_luo.o:		$(OBJ26_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj27_luo.o:		$(OBJ27_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^


obj28_luo.o:		$(OBJ28_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj29_luo.o:		$(OBJ29_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj30_luo.o:		$(OBJ30_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj31_luo.o:		$(OBJ31_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^


obj32_luo.o:		$(OBJ32_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj33_luo.o:		$(OBJ33_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj34_luo.o:		$(OBJ34_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj35_luo.o:		$(OBJ35_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^


obj36_luo.o:		$(OBJ36_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj37_luo.o:		$(OBJ37_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj38_luo.o:		$(OBJ38_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj39_luo.o:		$(OBJ39_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^


obj40_luo.o:		$(OBJ40_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj41_luo.o:		$(OBJ41_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj42_luo.o:		$(OBJ42_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj43_luo.o:		$(OBJ43_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^


obj44_luo.o:		$(OBJ44_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj45_luo.o:		$(OBJ45_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj46_luo.o:		$(OBJ46_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj47_luo.o:		$(OBJ47_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^


obj48_luo.o:		$(OBJ48_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj49_luo.o:		$(OBJ49_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj50_luo.o:		$(OBJ50_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj51_luo.o:		$(OBJ51_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^


obja_luo.o:		$(OBJA_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

objb_luo.o:		$(OBJB_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

objc_luo.o:		$(OBJC_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

objd_luo.o:		$(OBJD_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obje_luo.o:		$(OBJE_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

objf_luo.o:		$(OBJF_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

objg_luo.o:		$(OBJG_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

objh_luo.o:		$(OBJH_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obji_luo.o:		$(OBJI_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

objj_luo.o:		$(OBJJ_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

objk_luo.o:		$(OBJK_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

objl_luo.o:		$(OBJL_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

objm_luo.o:		$(OBJM_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

objn_luo.o:		$(OBJN_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^


obj_luo.o:		$(OBJ_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^


obj.o:			$(OBJ)
	$(LD) -r $(LDFLAGS) -o $@ $^


six.o:			six.dir
six.dir:
	makesubdir $@

isx.o:			isx.dir
isx.dir:
	makesubdir $@

ismisc.o:		ismisc.dir
ismisc.dir:
	makesubdir $@

isfiledesc.o:		isfiledesc.dir
isfiledesc.dir:
	makesubdir $@

mkx.o:			mkx.dir
mkx.dir:
	makesubdir $@

mnw.o:			mnw.dir
mnw.dir:
	makesubdir $@

snx.o:			snx.dir
snx.dir:
	makesubdir $@

snwcpy.o:		snwcpy.dir
snwcpy.dir:
	makesubdir $@

cfx.o:			cfx.dir
cfx.dir:
	makesubdir $@

ctx.o:			ctx.dir
ctx.dir:
	makesubdir $@

sfx.o:			sfx.dir
sfx.dir:
	makesubdir $@

strdcpy.o:		strdcpy.dir
strdcpy.dir:
	makesubdir $@

strwcpy.o:		strwcpy.dir
strwcpy.dir:
	makesubdir $@

strcpyx.o:		strcpyx.dir
strcpyx.dir:
	makesubdir $@

strx.o:			strx.dir
strx.dir:
	makesubdir $@

strn.o:			strn.dir
strn.dir:
	makesubdir $@

# STRW
strw.o:			strw.dir
strw.dir:
	makesubdir $@

inetaddrx.o:		inetaddrx.dir
inetaddrx.dir:
	makesubdir $@

# Hierarchical Objects

# SBUF
sbuf.o:			sbuf.dir
sbuf.dir:
	makesubdir $@

# SBUF
buffer.o:		buffer.dir
buffer.dir:
	makesubdir $@

# HDB
hdb.o:			hdb.dir
hdb.dir:
	makesubdir $@

# HDBSTR
hdbstr.o:		hdbstr.dir
hdbstr.dir:
	makesubdir $@

# EMA
ema.o:			ema.dir
ema.dir:
	makesubdir $@

# REALNAME
realname.o:		realname.dir
realname.dir:
	makesubdir $@

# MATXSTR
matxstr.o:		matxstr.dir
matxstr.dir:
	makesubdir $@

# NLEADX
nleadx.o:		nleadx.dir
nleadx.dir:
	makesubdir $@

ucsysmisc.o:		ucsysmisc.cc	ucsysmisc.h		$(INCS)

# UCSUPPORT	(libuc)
ucsupport.o:		ucsupport.dir
ucsupport.dir:
	makesubdir $@

# UCSYS		(libuc)
ucsys.o:		ucsys.dir
ucsys.dir:
	makesubdir $@

# UCENT		(libuc)
ucent.o:		ucent.dir
ucent.dir:
	makesubdir $@

# UCGET		(libuc)
ucget.o:		ucget.dir
ucget.dir:
	makesubdir $@

# UCENUM		(libuc)
ucenum.o:		ucenum.dir
ucenum.dir:
	makesubdir $@

# UCGETX		(libuc)
ucgetx.o:		ucgetx.dir
ucgetx.dir:
	makesubdir $@

# UCSIG		(libuc)
ucsig.o:		ucsig.dir
ucsig.dir:
	makesubdir $@

# UCSYSCONF	(libuc)
ucsysconf.o:		ucsysconf.dir
ucsysconf.dir:
	makesubdir $@

# UCSTREAM	(libuc)
ucstream.o:		ucstream.dir
ucstream.dir:
	makesubdir $@

# UCSTRTOX
ucstrto.o:		ucstrto.dir
ucstrto.dir:
	makesubdir $@

# UCMEM		(libuc)
ucmem.o:		ucmem.dir
ucmem.dir:
	makesubdir $@

# UCPROC	(libuc)
ucproc.o:		ucproc.dir
ucproc.dir:
	makesubdir $@

# UCDATA	(libuc)
ucdata.o:		ucdata.dir
ucdata.dir:
	makesubdir $@

# UCFILEOP
ucfileop.o:		ucfileop.dir
ucfileop.dir:
	makesubdir $@

# UCOPEN
ucopen.o:		ucopen.dir
ucopen.dir:
	makesubdir $@

ucpts.o:		ucpts.cc	ucpts.h			$(INCS)
ucpwcache.o:		ucpwcache.cc	ucpwcache.h		$(INCS)
ucdescbase.o:		ucdescbase.cc	ucdescbase.hh		$(INCS)
ucinfo.o:		ucinfo.cc	ucinfo.h		$(INCS)

# UCDESCREAD
ucdescread.o:		ucdescread.dir
ucdescread.dir:
	makesubdir $@

# UCDESCWRITE
ucdescwrite.o:		ucdescwrite.dir
ucdescwrite.dir:
	makesubdir $@

# UCESCSOCK
ucdescsock.o:		ucdescsock.dir
ucdescsock.dir:
	makesubdir $@

# UCESCLOCK
ucdesclock.o:		ucdesclock.dir
ucdesclock.dir:
	makesubdir $@

# UCESCMISC
ucdescmisc.o:		ucdescmisc.dir
ucdescmisc.dir:
	makesubdir $@

# UCX		(libuc)
ucx.o:			ucx.dir
ucx.dir:
	makesubdir $@

# MEMTRACK	(libuc)
memtrack.o:		memtrack.dir
memtrack.dir:
	makesubdir $@

# STRNXCMP
strnxcmp.o:		strnxcmp.dir
strnxcmp.dir:
	makesubdir $@

# STRKEYX
strxcmp.o:		strxcmp.dir
strxcmp.dir:
	makesubdir $@

# ADDRSET	(libuc)
addrset.o:		addrset.dir
addrset.dir:
	makesubdir $@

# QUEUE		(libuc)
queue.o:		queue.dir
queue.dir:
	makesubdir $@

# VEC		(libuc)
vec.o:			vec.dir
vec.dir:
	makesubdir $@

# RECIP		(libuc)
recip.o:		recip.dir
recip.dir:
	makesubdir $@

# SIGOBJ	(libuc)
sigobj.o:		sigobj.dir
sigobj.dir:
	makesubdir $@

# FILER		(libuc)
filer.o:		filer.dir
filer.dir:
	makesubdir $@

# SIF		(libuc)
sif.o:			sif.dir
sif.dir:
	makesubdir $@

# STRFILTER	(libuc)
strfilter.o:		strfilter.dir	sif.o
strfilter.dir:
	makesubdir $@

# ARGMGR
argmgr.o:		argmgr.dir
argmgr.dir:
	makesubdir $@

# PERMX
permx.o:		permx.dir
permx.dir:
	makesubdir $@

# TARDIR
tardir.o:		tardir.dir
tardir.dir:
	makesubdir $@

# RMX
rmx.o:			rmx.dir
rmx.dir:
	makesubdir $@

# HASX		(libuc)
hasx.o:			hasx.dir
hasx.dir:
	makesubdir $@

# STRPACK	(libuc)
strpack.o:		strpack.dir
strpack.dir:
	makesubdir $@

# STROBJ	(libuc)
strobj.o:		strobj.dir
strobj.dir:
	makesubdir $@

# FIELD		(libuc)
field.o:		field.dir
field.dir:
	makesubdir $@

# LANGX		(libuc)
langx.o:		langx.dir
langx.dir:
	makesubdir $@

# FSDIR		(libuc)
fsdir.o:		fsdir.dir
fsdir.dir:
	makesubdir $@

# HASH
hash.o:			hash.dir
hash.dir:
	makesubdir $@

# NCOLX
ncolx.o:		ncolx.dir
ncolx.dir:
	makesubdir $@

# VSTR
vstr.o:			vstr.dir
vstr.dir:
	makesubdir $@

# WSFX
wsfx.o:			wsfx.dir
wsfx.dir:
	makesubdir $@

# WSIX
wsix.o:			wsix.dir
wsix.dir:
	makesubdir $@

# WSNX
wsnx.o:			wsnx.dir
wsnx.dir:
	makesubdir $@

# WSWCPYX
wswcpyx.o:		wswcpyx.dir
wswcpyx.dir:
	makesubdir $@

# BUFSUZE
bufsize.o:		bufsize.dir
bufsize.dir:
	makesubdir $@

# PATH
path.o:			path.dir
path.dir:
	makesubdir $@

# POW
pow.o:			pow.dir
pow.dir:
	makesubdir $@

# TERMX
termx.o:		termx.dir
termx.dir:
	makesubdir $@

# FMTFLAG
fmtflag.o:		fmtflag.dir
fmtflag.dir:
	makesubdir $@

# FMTSTR
fmtstr.o:		fmtstr.dir
fmtstr.dir:
	makesubdir $@

# GETX
getx.o:			getx.dir
getx.dir:
	makesubdir $@

# GETXX
getxx.o:		getxx.dir
getxx.dir:
	makesubdir $@

# GETXNAME
getxname.o:		getxname.dir
getxname.dir:
	makesubdir $@

# GETUTMP
getutmp.o:		getutmp.dir
getutmp.dir:
	makesubdir $@

# TMPX
tmpx.o:			tmpx.dir
tmpx.dir:
	makesubdir $@

# UTMPXX
utmpxx.o:		utmpxx.dir
utmpxx.dir:
	makesubdir $@

# CACHES
caches.o:		caches.dir
caches.dir:
	makesubdir $@

# FILEX
filex.o:		filex.dir
filex.dir:
	makesubdir $@

# FILEOBJ
fileobj.o:		fileobj.dir
fileobj.dir:
	makesubdir $@

# BUFOS
bufos.o:		bufos.dir
bufos.dir:
	makesubdir $@

# BUGSLIDE
bufslide.o:		bufslide.dir
bufslide.dir:
	makesubdir $@

# OPEN
open.o:			open.dir
open.dir:
	makesubdir $@

openx.o:		openx.dir
openx.dir:
	makesubdir $@

# MKTMP
mktmp.o:		mktmp.dir
mktmp.dir:
	makesubdir $@

# MKLOGID
mklogid.o:		mklogid.dir
mklogid.dir:
	makesubdir $@

# DIRS
dirs.o:			dirs.dir
dirs.dir:
	makesubdir $@

# DIRSEEN
dirseen.o:		dirseen.dir
dirseen.dir:
	makesubdir $@

# CHRONO
chrono.o:		chrono.dir
chrono.dir:
	makesubdir $@

# TIMESTR
timestr.o:		timestr.dir
timestr.dir:
	makesubdir $@

# HOST
host.o:			host.dir
host.dir:
	makesubdir $@

# GETHE
gethe.o:		gethe.dir
gethe.dir:
	makesubdir $@

# ENVX
envx.o:			envx.dir
envx.dir:
	makesubdir $@

# ENVS
envs.o:			envs.dir
envs.dir:
	makesubdir $@

# SIGX
sigx.o:			sigx.dir
sigx.dir:
	makesubdir $@

# DIAL
dial.o:			dial.dir
dial.dir:
	makesubdir $@

# ARGUTILS
argutils.o:		argutils.dir
argutils.dir:
	makesubdir $@

# CMDUTILS
cmdutils.o:		cmdutils.dir
cmdutils.dir:
	makesubdir $@

# LOGFILE
logfile.o:		logfile.dir
logfile.dir:
	makesubdir $@

# LINEX
linex.o:		linex.dir
linex.dir:
	makesubdir $@

# MKUI
mkui.o:			mkui.dir
mkui.dir:
	makesubdir $@

# VARSUB
varsub.o:		varsub.dir
varsub.dir:
	makesubdir $@

# Objects
storebuf.o:		storebuf.cc	storebuf.h		$(INCS)
storeitem.o:		storeitem.cc	storeitem.h		$(INCS)
ids.o:			ids.cc		ids.h			$(INCS)
lookaside.o:		lookaside.cc	lookaside.h		$(INCS)
termstr.o:		termstr.cc	termstr.h		$(INCS)
td.o:			td.cc		td.h			$(INCS)
fsdirtree.o:		fsdirtree.cc	fsdirtree.h		$(INCS)
nodedb.o:		nodedb.cc	nodedb.h		$(INCS)
clusterdb.o:		clusterdb.cc	clusterdb.h		$(INCS)
gecos.o:		gecos.cc	gecos.h			$(INCS)
pwentry.o:		pwentry.cc	pwentry.h		$(INCS)
serialbuf.o:		serialbuf.cc	serialbuf.h		$(INCS)
dupstr.o:		dupstr.cc	dupstr.h		$(INCS)
comparse.o:		comparse.cc	comparse.h		$(INCS)
posname.o:		posname.cc	posname.h		$(INCS)
absfn.o:		absfn.cc	absfn.h			$(INCS)
kvsfile.o:		kvsfile.cc	kvsfile.h		$(INCS)
userattrdb.o:		userattrdb.cc	userattrdb.h		$(INCS)
spawner.o:		spawner.cc	spawner.h		$(INCS)
userinfo.o:		userinfo.cc	userinfo.h		$(INCS)
expcook.o:		expcook.cc	expcook.h		$(INCS)
bits.o:			bits.cc		bits.h			$(INCS)
paramfile.o:		paramfile.cc	paramfile.h		$(INCS)

# GROUPS
calstrs.o:		calstrs.cc	calstrs.h		$(INCS)
char.o:			char.cc		char.h			$(INCS)
toxc.o:			toxc.cc		toxc.h			$(INCS)
bitrotate.o:		bitrotate.cc	bitrotate.h		$(INCS)
stdorder.o:		stdorder.cc	stdorder.h		$(ICNS)

# SINGLES
optval.o:		optval.cc	optval.h		$(INCS)
sysval.o:		sysval.cc	sysval.hh		$(INCS)
randlc.o:		randlc.cc	randlc.h		$(INCS)
calstrs.o:		calstrs.cc	calstrs.h		$(INCS)
checkbase.o:		checkbase.cc	checkbase.h		$(INCS)
strlibval.o:		strlibval.cc	strlibval.hh		$(INCS)
matparam.o:		matparam.cc	matparam.h		$(INCS)
sysdbfn.o:		sysdbfn.cc	sysdbfn.h		$(INCS)
sysdbfiles.o:		sysdbfiles.cc	sysdbfiles.h		$(INCS)
mallocstuff.o:		mallocstuff.cc	mallocstuff.h		$(INCS)
xfile.o:		xfile.cc	xfile.h			$(INCS)
sockaddress.o:		sockaddress.cc	sockaddress.h		$(INCS)
lockfile.o:		lockfile.cc	lockfile.h		$(INCS)
digval.o:		digval.cc	digval.h		$(INCS)
nonpath.o:		nonpath.cc	nonpath.h		$(INCS)
msghdr.o:		msghdr.cc	msghdr.h		$(INCS)
conmsghdr.o:		conmsghdr.cc	conmsghdr.h		$(INCS)
spawnproc.o:		spawnproc.cc	spawnproc.h		$(INCS)
findxfile.o:		findxfile.cc	findxfile.h		$(INCS)
conallof.o:		conallof.cc	conallof.h		$(INCS)
ndig.o:			ndig.cc		ndig.h			$(INCS)
ndigit.o:		ndigit.cc	ndigit.h		$(INCS)
bufprintf.o:		bufprintf.cc	bufprintf.h		$(INCS)


