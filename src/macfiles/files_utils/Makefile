# MAKEFILES (files_utils)

T= files_utils

ALL= $(T).o


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
CXX		?= gpp
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

INCS += files_utils.h

MODS +=

LIBS +=


OBJ0= files_utils0.o files_utils1.o
OBJ1= files_utils2.o

OBJA= obj0.o obj1.o

OBJ= obja.o


INCDIRS +=

LIBDIRS += -L$(LIBDIR)


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


$(T).o:			$(OBJ)
	makemodule files_utils
	$(LD) -r -o $@ $(LDFLAGS) $^

$(T).nm:		$(T).o
	$(NM) $(NMFLAGS) $(T).o > $(T).nm

again:
	$(RM) $(ALL)

clean:
	makeclean $(ALL)

control:
	(uname -n ; date) > Control


obj0.o:			$(OBJ0)
	$(LD) -r -o $@ $(LDFLAGS) $^

obj1.o:			$(OBJ1)
	$(LD) -r -o $@ $(LDFLAGS) $^

obj2.o:			$(OBJ2)
	$(LD) -r -o $@ $(LDFLAGS) $^

obj3.o:			$(OBJ3)
	$(LD) -r -o $@ $(LDFLAGS) $^


obja.o:			$(OBJA)
	$(LD) -r -o $@ $(LDFLAGS) $^

objb.o:			$(OBJB)
	$(LD) -r -o $@ $(LDFLAGS) $^


files_utils0.o:		files_utils.ccm
	makemodule files_utils

files_utils1.o:		files_utils1.cc files_utils.ccm
	makemodule files_utils
	$(COMPILE.cc) $<

files_utils2.o:		files_utils2.cc files_utils.ccm
	makemodule files_utils
	$(COMPILE.cc) $<


