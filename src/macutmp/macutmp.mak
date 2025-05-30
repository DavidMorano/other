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


DEFS=

INCS=

MODS +=

LIBS= -lu

ELIBINFO= $(EXTRA)/lib/libu.o -L$(EXTRA)/lib -lmacuser -liconv -lproc


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


OBJ00_LIBUO= isx.o snx.o ucx.o
OBJ01_LIBUO= sbuf.o storebuf.o ctx.o
OBJ02_LIBUO= sfx.o strdcpy.o strx.o
OBJ03_LIBUO= calstrs.o char.o

OBJ04_LIBUO= 
OBJ05_LIBUO= 
OBJ06_LIBUO= 
OBJ07_LIBUO= 

OBJA_LIBUO= obj00_libuo.o obj01_libuo.o 
OBJB_LIBUO= obj02_libuo.o obj03_libuo.o

OBJ_LIBUO= obja_libuo.o objb_libuo.o

OBJ_UTMP= utmp_main.o $(OBJ_LIBUO)


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


consoleid.x:		$(OBJ_UTMP)
	$(LD) -o $@ $(LDFLAGS) $(RUNINFO) $(OBJ_UTMP) $(ELIBINFO)

$(T).x:			$(OBJ_UTMP)
	$(LD) -o $@ $(LDFLAGS) $(RUNINFO) $(OBJ_UTMP) $(LIBINFO)

$(T).o:			$(OBJ_UTMP)
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


obj00_libuo.o:		$(OBJ00_LIBUO)
	$(LD) -r $(LDFLAGS) -o $@ $(OBJ00_LIBUO)

obj01_libuo.o:		$(OBJ01_LIBUO)
	$(LD) -r $(LDFLAGS) -o $@ $(OBJ01_LIBUO)

obj02_libuo.o:		$(OBJ02_LIBUO)
	$(LD) -r $(LDFLAGS) -o $@ $(OBJ02_LIBUO)

obj03_libuo.o:		$(OBJ03_LIBUO)
	$(LD) -r $(LDFLAGS) -o $@ $(OBJ03_LIBUO)


obja_libuo.o:		$(OBJA_LIBUO)
	$(LD) -r $(LDFLAGS) -o $@ $(OBJA_LIBUO)

objb_libuo.o:		$(LIBB_LIBUO)
	$(LD) -r $(LDFLAGS) -o $@ $(OBJB_LIBUO)


utmp_main.o:		utmp_main.cc

libuo.o:		$(OBJ_LIBUO)
	$(LD) -r $(LDFLAGS) -o $@ $(OBJ_LIBUO)

isx.o:			isx.dir
isx.dir:
	makesubdir $@

snx.o:			snx.dir
snx.dir:
	makesubdir $@

sbuf.o:			sbuf.dir
sbuf.dir:
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

strx.o:		strx.dir
strx.dir:
	makesubdir $@

calstrs.o:		calstrs.cc calstrs.h
storebuf.o:		storebuf.cc storebuf.h
char.o:			char.cc char.h

ucx.o:			uctc.o ucttyname.o
	$(LD) -r $(LDFLAGS) -o $@ uctc.o ucttyname.o

uctc.o:			uctc.cc
ucttyname.o:		ucttyname.cc
ucmemalloc.o:		ucmemalloc.cc ucmemalloc.h


