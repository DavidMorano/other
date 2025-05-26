# MAKEFILE (macfiles)

T= macfiles

ALL= $(T).x


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

MODS += argmgr.ccm

LIBS= -lmacuser -lu


INCDIRS= -I$(INCDIR)

LIBDIRS= -L$(LIBDIR)


RUNINFO= -rpath $(RUNDIR)

LIBINFO= $(LIBDIRS) $(LIBS)

# flag setting
CPPFLAGS	?= $(DEFS) $(INCDIRS) $(MAKECPPFLAGS)
CFLAGS		?= $(MAKECFLAGS)
CXXFLAGS	?= $(MAKECXXFLAGS)
ARFLAGS		?= $(MAKEARFLAGS)
LDFLAGS		?= $(MAKELDFLAGS)


OBJ_MACFU= files_main.o fonce.o


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


$(T).x:			$(OBJ_MACFU)
	$(CXX) -o $@ $(LDFLAGS) $(RUNINFO) $(OBJ_MACFU) $(LIBINFO)

$(T).nm:		$(T).x
	$(NM) $(NMFLAGS) $(T).so > $(T).nm

again:
	rm -f $(T).x

clean:
	makeclean $(ALL)

control:
	(uname -n ; date) > Control

install:		$(T).x
	makeinstall $(T).x


files_main.o:		mods.o files_main.cc fonce.hh		$(INCS)
fonce.o:		fonce.cc fonce.hh

mods.o:			argmgr.o
	$(CXX) -r -o  $@ $(LDFLAGS) argmgr.o

# ARGMGR
argmgr.o:		argmgr0.o argmgr1.o
	$(CXX) -r -o  $@ $(LDFLAGS) argmgr0.o argmgr1.o

argmgr0.o:		argmgr.ccm
	makemodule argmgr

argmgr1.o:		argmgr.ccm argmgr1.cc
	makemodule argmgr
	$(COMPILE.cc) argmgr1.cc


