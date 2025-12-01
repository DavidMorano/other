# MAKEFILE (libuo)

T= libuo

ALL= $(T).o ${T}.so $(T).a
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

INCS=

MODS += libutil.ccm argmgr.ccm strfilter.ccm sif.ccm 
MODS += tardir.ccm

LIBS= -lu -lsecdb


OBJUC00= ucsys.o ucent.o ucget.o ucenum.o
OBJUC01= ucsig.o ucsysconf.o ucx.o 
OBJUC02= ucstrtox.o
OBJUC03= ucsupport.o ucstream.o

OBJUC_A= objuc00.o objuc01.o objuc02.o objuc03.o
OBJUC_B=

OBJ00_LUO= debug.o
OBJ01_LUO= strn.o six.o mkx.o
OBJ02_LUO= sfx.o rmx.o snx.o snwcpy.o
OBJ03_LUO= isx.o ismisc.o hasx.o 

OBJ04_LUO= sif.o ctx.o cfx.o
OBJ05_LUO= strdcpy.o strx.o 
OBJ06_LUO= strkeyx.o
OBJ07_LUO= strw.o strwcpy.o

OBJ08_LUO= ccfile.o filer.o filemap.o hdb.o
OBJ09_LUO= strop.o strmgr.o strfilter.o 
OBJ10_LUO= fsdirtree.o fsdir.o
OBJ11_LUO= queue.o field.o nulstr.o

OBJ12_LUO= tardir.o langx.o
OBJ13_LUO= nleadx.o matxstr.o 
OBJ14_LUO= char.o toxc.o
OBJ15_LUO= dictdiff.o strnxcmp.o

OBJ16_LUO= readln.o checkbase.o matparam.o
OBJ17_LUO= bitrotate.o
OBJ18_LUO= mapex.o ncolx.o hash.o permx.o
OBJ19_LUO= inetconv.o inetaddrx.o 

OBJ20_LUO= addrset.o ids.o gecos.o
OBJ21_LUO= sigobj.o pwentry.o
OBJ22_LUO= lookaside.o
OBJ23_LUO= vec.o recip.o

OBJ24_LUO= wsfx.o 
OBJ25_LUO= wsnx.o 
OBJ26_LUO= wsix.o
OBJ27_LUO= wswcpyx.o

OBJ28_LUO= bufsize.o
OBJ29_LUO= sysval.o path.o termx.o
OBJ30_LUO= vstr.o xfile.o
OBJ31_LUO= randlc.o pow.o calstrs.o

OBJ32_LUO= realname.o nodedb.o
OBJ33_LUO= ema.o tmpx.o
OBJ34_LUO= sbuf.o buffer.o
OBJ35_LUO= storebuf.o storeitem.o

OBJ36_LUO= fmtflag.o fmtstr.o sysdbfiles.o
OBJ37_LUO= getx.o sysdbfn.o strlibval.o
OBJ38_LUO= utmpxx.o uinfo.o caches.o
OBJ39_LUO= fileread.o getxx.o

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
OBJK_LUO=

OBJ_LUO += obja_luo.o objb_luo.o objc_luo.o objd_luo.o
OBJ_LUO += obje_luo.o objf_luo.o objg_luo.o objh_luo.o
OBJ_LUO += obji_luo.o objj_luo.o

OBJ= objuc_a.o obj_luo.o


INCDIRS=

LIBDIRS= -L$(LIBDIR)

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
	makemodule $(*)


$(T).so:		$(T).o
	$(CXX) -shared -o $@ $(LDFLAGS) $(OBJ) $(RUNINFO) $(LIBINFO)

$(T).o:			$(OBJ)
	$(LD) -r -o $@ $(LDFLAGS) $(OBJ)

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


objuc_a.o:		$(OBJUC_A)
	$(LD) -r $(LDFLAGS) -o $@ $^

objuc_b.o:		$(OBJUC_B)
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


obj_luo.o:		$(OBJ_LUO)
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

mkx.o:			mkx.dir
mkx.dir:
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

bufsizevar.o:		bufsizevar.dir
bufsizevar.dir:
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

# UCX		(libuc)
ucx.o:			ucx.dir
ucx.dir:
	makesubdir $@

# STRNXCMP
strnxcmp.o:		strnxcmp.dir
strnxcmp.dir:
	makesubdir $@

# STRKEYX
strkeyx.o:		strkeyx.dir
strkeyx.dir:
	makesubdir $@

# CHRSET	(libu)
chrset.o:		chrset.dir
chrset.dir:
	makesubdir $@

# DEBUG		(libu)
debug.o:		debug.dir
debug.dir:
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
strfilter.o:		strfilter.dir sif.o
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

# FILEREAD
fileread.o:		fileread.dir
fileread.dir:
	makesubdir $@

# GETXX
getxx.o:		getxx.dir
getxx.dir:
	makesubdir $@

# Objects
uinfo.o:		uinfo.cc	uinfo.h			$(INCS)
storebuf.o:		storebuf.cc	storebuf.h		$(INCS)
storeitem.o:		storeitem.cc	storeitem.h		$(INCS)
ids.o:			ids.cc		ids.h			$(INCS)
ccfile.o:		ccfile.cc	ccfile.hh		$(INCS)
vechand.o:		vechand.cc	vechand.h		$(INCS)
vecitem.o:		vecitem.cc	vecitem.h		$(INCS)
vecelem.o:		vecelem.cc	vecelem.h		$(INCS)
lookaside.o:		lookaside.cc	lookaside.h		$(INCS)
termstr.o:		termstr.cc	termstr.h		$(INCS)
td.o:			td.cc		td.h			$(INCS)
nulstr.o:		nulstr.cc	nulstr.h		$(INCS)
fsdirtree.o:		fsdirtree.cc	fsdirtree.h		$(INCS)
filemap.o:		filemap.cc	filemap.h		$(INCS)
nodedb.o:		nodedb.cc	nodedb.h		$(INCS)
gecos.o:		gecos.cc	gecos.h			$(INCS)
pwentry.o:		pwentry.cc	pwentry.h		$(INCS)

# Temporary Objects
strop.o:		strop.cc	strop.h			$(INCS)
strmgr.o:		strmgr.cc	strmgr.h		$(INCS)

# GROUPS
calstrs.o:		calstrs.cc calstrs.h
char.o:			char.cc char.h
toxc.o:			toxc.cc toxc.h
inetconv.o:		inetconv.cc inetconv.h
strcpyxc.o:		strcpyxc.cc strcpyxc.h strcpy.h strcpyx.h
bitrotate.o:		bitrotate.cc bitrotate.h		$(INCS)

# SINGLES
mapex.o:		mapex.cc mapex.h			$(INCS)
dictdiff.o:		dictdiff.cc dictdiff.h			$(INCS)
inet4int.o:		inet4int.cc inet4int.h			$(INCS)
readln.o:		readln.cc readln.hh			$(INCS)
getourenv.o:		getourenv.cc getourenv.h		$(INCS)
optval.o:		optval.cc	optval.h	$(INCS)
getbufsize.o:		getbufsize.cc	getbufsize.h	$(INCS)
sysval.o:		sysval.cc	sysval.hh	$(INCS)
randlc.o:		randlc.cc	randlc.h	$(INCS)
calstrs.o:		calstrs.cc	calstrs.h	$(INCS)
checkbase.o:		checkbase.cc	checkbase.h	$(INCS)
strlibval.o:		strlibval.cc	strlibval.hh	$(INCS)
matparam.o:		matparam.cc	matparam.h	$(INCS)
sysdbfn.o:		sysdbfn.cc	sysdbfn.h	$(INCS)
sysdbfiles.o:		sysdbfiles.c	sysdbfiles.h	$(INCS)
mallocstuff.o:		mallocstuff.cc	mallocstuff.h	$(INCS)
xfile.o:		xfile.cc	xfile.h		$(INCS)

# LIBUC
ucstrtox.o:		ucstrtox.cc		ucstrtox.h	$(INCS)


