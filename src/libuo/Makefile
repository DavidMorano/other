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

MODS += libutil.ccm fonce.ccm argmgr.ccm

LIBS= -lu


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


OBJ00_LUO= ucx.o
OBJ01_LUO= sfx.o six.o rmx.o isx.o
OBJ02_LUO= strn.o strdcpy.o strx.o strkeyx.o
OBJ03_LUO= strw.o strwcpy.o

OBJ04_LUO= fonce.o ccfile.o strmgr.o 
#OBJ04_LUO= fonce.o ccfile.o strmgr.o argmgr.o
OBJ05_LUO= matxstr.o char.o mapex.o
OBJ06_LUO= nleadx.o
OBJ07_LUO= strnxcmp.o

OBJ08_LUO= strcpyxc.o
OBJ09_LUO= readln.o
OBJ10_LUO= getourenv.o dictdiff.o
OBJ11_LUO= inetconv.o inetaddrx.o 

OBJA_LUO= obj00_luo.o obj01_luo.o obj02_luo.o obj03_luo.o
OBJB_LUO= obj04_luo.o obj05_luo.o obj06_luo.o obj07_luo.o
OBJC_LUO= obj08_luo.o obj09_luo.o obj10_luo.o obj11_luo.o
OBJD_LUO= 
OBJE_LUO= 
OBJF_LUO= 

OBJ_LUO= obja_luo.o objb_luo.o objc_luo.o
#OBJ_LUO= objd_luo.o obje_luo.o objf_luo.o


.SUFFIXES:		.hh .ii .ccm


default:		$(T).o

all:			$(ALL)

so:			$(T).so

a:			$(T).a


.c.i:
	$(CPP) $(CPPFLAGS) $< > $(*).i

.cc.ii:
	$(CPP) $(CPPFLAGS) $< > $(*).ii

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
	$(LD) -shared -o $@ $(LDFLAGS) $(OBJ_LUO) $(LIBINFO)

$(T).o:			$(OBJ_LUO)
	$(LD) -r -o $@ $(LDFLAGS) $(OBJ_LUO)

$(T).a:			$(OBJ_LUO)
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


obj00_luo.o:		$(OBJ00_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $(OBJ00_LUO)

obj01_luo.o:		$(OBJ01_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $(OBJ01_LUO)

obj02_luo.o:		$(OBJ02_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $(OBJ02_LUO)

obj03_luo.o:		$(OBJ03_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $(OBJ03_LUO)

obj04_luo.o:		$(OBJ04_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $(OBJ04_LUO)

obj05_luo.o:		$(OBJ05_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $(OBJ05_LUO)

obj06_luo.o:		$(OBJ06_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $(OBJ06_LUO)

obj07_luo.o:		$(OBJ07_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $(OBJ07_LUO)

obj08_luo.o:		$(OBJ08_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $(OBJ08_LUO)

obj09_luo.o:		$(OBJ09_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $(OBJ09_LUO)

obj10_luo.o:		$(OBJ10_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $(OBJ10_LUO)

obj11_luo.o:		$(OBJ11_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $(OBJ11_LUO)


obja_luo.o:		$(OBJA_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $(OBJA_LUO)

objb_luo.o:		$(OBJB_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $(OBJB_LUO)

objc_luo.o:		$(OBJC_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $(OBJC_LUO)

objd_luo.o:		$(OBJD_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $(OBJD_LUO)

obje_luo.o:		$(OBJE_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $(OBJC_LUO)

objf_luo.o:		$(OBJF_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $(OBJF_LUO)


obj_luo.o:		$(OBJ_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $(OBJ_LUO)

six.o:			six.dir
six.dir:
	makesubdir $@

isx.o:			isx.dir
isx.dir:
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

fmtstr.o:		fmtstr.dir
fmtstr.dir:
	makesubdir $@

bufsizevar.o:		bufsizevar.dir
bufsizevar.dir:
	makesubdir $@

# Hierarchical Objects

# SBUF
sbuf.o:			sbuf.dir
sbuf.dir:
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

# UCX
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

# FONCE
fonce.o:		fonce.dir
fonce.dir:
	makesubdir $@

# ARGMGR
argmgr.o:		argmgr.dir
argmgr.dir:
	makesubdir $@

# Flat Objects
storebuf.o:		storebuf.cc storebuf.h
strmgr.o:		strmgr.cc strmgr.h
ids.o:			ids.cc ids.h
ccfile.o:		ccfile.cc ccfile.hh

# GROUPS
calstrs.o:		calstrs.cc calstrs.h
char.o:			char.cc char.h
inetconv.o:		inetconv.cc inetconv.h
rmx.o:			rmx.cc rmx.h
strcpyxc.o:		strcpyxc.cc strcpyxc.h strcpy.h strcpyx.h

# SINGLES
mapex.o:		mapex.cc mapex.h
dictdiff.o:		dictdiff.cc dictdiff.h
inet4int.o:		inet4int.cc inet4int.h
readln.o:		readln.cc readln.hh
getourenv.o:		getourenv.cc getourenv.h


