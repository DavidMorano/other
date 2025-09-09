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

LIBS += -luo -lu

ELIBS += -luo -lproc -liconv


DEPMODS += libutil.o
DEPMODS += umisc.o ureserve.o

OBJ_UTMP= utmp_main.o


INCDIRS=

LIBDIRS= -L$(LIBDIR)


RUNINFO= -rpath $(RUNDIR)
LIBINFO= $(LIBDIRS) $(LIBS)
ELIBINFO= $(EXTRA)/lib/libu.o -L$(EXTRA)/lib $(ELIBS)

# flag setting
CPPFLAGS	?= $(DEFS) $(INCDIRS) $(MAKECPPFLAGS)
CFLAGS		?= $(MAKECFLAGS)
CXXFLAGS	?= $(MAKECXXFLAGS)
ARFLAGS		?= $(MAKEARFLAGS)
LDFLAGS		?= $(MAKELDFLAGS)


.SUFFIXES:		.hh .ii .iim .ccm


default:		$(T).x

all:			$(ALL)


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


consoleid.x:		$(OBJ_UTMP) Makefile
	$(LD) -o $@ $(LDFLAGS) $(RUNINFO) $(OBJ_UTMP) $(ELIBINFO)

$(T).x:			$(OBJ_UTMP) Makefile
	$(LD) -o $@ $(LDFLAGS) $(RUNINFO) $(OBJ_UTMP) $(LIBINFO)

$(T).o:			$(OBJ_UTMP) Makefile
	$(LD) -r $(LDFLAGS) -o $@ $(OBJ_UTMP)

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
	$(LD) -r $(LDFLAGS) -o $@ $^

obj01_luo.o:		$(OBJ01_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj02_luo.o:		$(OBJ02_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj03_luo.o:		$(OBJ03_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^


obja_luo.o:		$(OBJA_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

objb_luo.o:		$(OBJB_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

objc_luo.o:		$(OBJC_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^

objd_luo.o:		$(OBJD_LUO)
	$(LD) -r $(LDFLAGS) -o $@ $^


utmp_main.o:		utmp_main.cc $(DEPMODS)

utmp_support.o:		utmp_support.cc

libutil.o:		libutil.dir
libutil.dir:
	makesubdir $@

umisc.o:		umisc.dir
umisc.dir:
	makesubdir $@

ureserve.o:		ureserve.dir
ureserve.dir:
	makesubdir $@


