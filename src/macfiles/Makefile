# MAKEFILE (files)

T= files

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

MODS += vecbool.ccm fonce.ccm argmgr.ccm

LIBS= -luo -lu


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


DEPS_MAIN += fonce.o vecbool.o

OBJ_MACFU= files_main.o


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


files_main.o:		files_main.cc $(DEPS_MAIN)		$(INCS)

# MODS
mods.o:			$(DEPS_MAIN)
	$(CXX) -r -o $@ $(LDFLAGS) $^

vecbool.ccm:
	makemodcurrent
argmgr.ccm:
	makemodcurrent
fonce.ccm:
	makemodcurrent

# VECBOOL
vecbool.o:		vecbool0.o vecbool1.o
	$(CXX) -r -o $@ $(LDFLAGS) vecbool0.o vecbool1.o

vecbool0.o:		vecbool.ccm
	makemodule vecbool

vecbool1.o:		vecbool1.cc vecbool.ccm
	makemodule vecbool
	$(COMPILE.cc) $<

# ARGMGR
argmgr.o:		argmgr0.o argmgr1.o
	$(CXX) -r -o $@ $(LDFLAGS) argmgr0.o argmgr1.o

argmgr0.o:		argmgr.ccm
	makemodule argmgr

argmgr1.o:		argmgr1.cc argmgr.ccm
	makemodule argmgr
	$(COMPILE.cc) $<

# FONCE
fonce.o:		fonce0.o fonce1.o		$(INCS)
	makemodule fonce
	$(LD) -r -o $@ $(LDFLAGS) fonce0.o fonce1.o
fonce0.o:		fonce.ccm			$(INCS)
	makemodule fonce
fonce1.o:		fonce1.cc fonce.ccm		$(INCS)
	makemodule fonce
	$(COMPILE.cc) $<


