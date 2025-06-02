# MAKEFILE (macutmp)

T= macutmp

ALL= $(T).x
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


DEFS +=

INCS +=

MODS +=

LIBS += -lu

ELIBS += -lproc -liconv

ELIBINFO= $(EXTRA)/lib/libu.o -L$(EXTRA)/lib $(ELIBS)


INCDIRS=

LIBDIRS= -L$(LIBDIR)


RUNINFO= -rpath $(RUNDIR)
LIBINFO= $(LIBDIRS) $(LIBS)

# flag setting
CPPFLAGS	?= $(DEFS) $(INCDIRS) $(MAKECPPFLAGS)
CFLAGS		?= $(MAKECFLAGS)
CXXFLAGS	?= $(MAKECXXFLAGS) -g
ARFLAGS		?= $(MAKEARFLAGS)
LDFLAGS		?= $(MAKELDFLAGS)

DEPMODS += libutil.ccm
DEPMODS += usigset.ccm usigblock.ccm umisc.ccm ureserve.ccm
DEPMODS += ureserve.ccm

OBJ00_LUO= isx.o six.o mkx.o snx.o sfx.o 
OBJ01_LUO= cfx.o ctx.o inetaddrx.o
OBJ02_LUO= strdcpy.o strwcpy.o strx.o
OBJ03_LUO= calstrs.o

OBJ04_LUO= strn.o
OBJ05_LUO= bufsizevar.o
OBJ06_LUO= ucx.o 
OBJ07_LUO= char.o fmtstr.o

OBJ08_LUO= sbuf.o storebuf.o 
OBJ09_LUO= strmgr.o 
OBJ10_LUO= ids.o 
OBJ11_LUO= ema.o realname.o

OBJA_LUO= obj00_luo.o obj01_luo.o obj02_luo.o obj03_luo.o
OBJB_LUO= obj04_luo.o obj05_luo.o obj06_luo.o obj07_luo.o
OBJC_LUO= obj08_luo.o obj09_luo.o obj10_luo.o obj11_luo.o
OBJD_LUO= 
OBJE_LUO= 
OBJF_LUO= 

OBJ_LUO= obja_luo.o objb_luo.o objc_luo.o
#OBJ_LUO= objd_luo.o obje_luo.o objf_luo.o

OBJ_UTMP= utmp_main.o ucx.o


.SUFFIXES:		.hh .ii .ccm


default:		$(T).x

all:			$(ALL)


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


consoleid.x:		$(OBJ_UTMP) Makefile
	$(LD) -o $@ $(LDFLAGS) $(RUNINFO) $(OBJ_UTMP) $(ELIBINFO)

$(T).x:			$(OBJ_UTMP) Makefile
	$(LD) -o $@ $(LDFLAGS) $(RUNINFO) $(OBJ_UTMP) $(LIBINFO)

$(T).o:			$(OBJ_UTMP) Makefile
	$(LD) -r $(LDFLAGS) -o $@ $(OBJ_UTMP)

$(T).nm:		$(T).o
	$(NM) $(NMFLAGS) $(T).o > $(T).nm

$(T).order:		$(OBJ_UTMP) $(T).a
	$(LORDER) $(T).a | $(TSORT) > $(T).order
	$(RM) $(T).a
	while read O ; do $(AR) $(ARFLAGS) -cr $(T).a $${O} ; done < $(T).order

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


utmp_main.o:		utmp_main.cc $(DEPMODS) ucx.h
	makemodule libutil
	makemodule usigset
	makemodule usigblock
	makemodule umisc
	makemodule ureserve
	$(COMPILE.cc) $<


libutil.ccm:
	ln mod/lbutil.ccm .

usigset.ccm:
	ln mod/usigset.ccm .

usigblock.ccm:
	ln mod/usigblock.ccm .

umisc.ccm:
	ln mod/umisc.ccm .

ureserve.ccm:
	ln mod/ureserve.ccm .


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

# Flat Objects
storebuf.o:		storebuf.cc storebuf.h
strmgr.o:		strmgr.cc strmgr.h
ids.o:			ids.cc ids.h

# GROUPS
calstrs.o:		calstrs.cc calstrs.h
char.o:			char.cc char.h

ucx.o:			uctc.o ucttyname.o
	$(LD) -r $(LDFLAGS) -o $@ $^

uctc.o:			uctc.cc		uctc.h		$(INCS)
ucttyname.o:		ucttyname.cc	ucttyname.h	$(INCS)
ucmemalloc.o:		ucmemalloc.cc	ucmemalloc.h	$(INCS)


