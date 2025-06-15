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

MODS += vecbool.cc argmgr.ccm

LIBS= -luo -lu


INCDIRS=
#INCDIRS= -I$(INCDIR)

LIBDIRS= -L$(LIBDIR)


RUNINFO= -rpath $(RUNDIR)
LIBINFO= $(LIBDIRS) $(LIBS)

# flag setting
CPPFLAGS	?= $(DEFS) $(INCDIRS) $(MAKECPPFLAGS)
CFLAGS		?= $(MAKECFLAGS)
CXXFLAGS	?= $(MAKECXXFLAGS)
ARFLAGS		?= $(MAKEARFLAGS)
LDFLAGS		?= $(MAKELDFLAGS)


DEPS_MAIN += vecbool.o argmgr.o

OBJ_FILES += files_main.o $(DEPS_MAIN)


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
	echo "upper default-rule"


$(T).x:			$(OBJ_FILES)
	$(CXX) -o $@ $(LDFLAGS) $(RUNINFO) $(OBJ_FILES) $(LIBINFO)

$(T).nm:		$(T).x
	$(NM) $(NMFLAGS) $(T).x > $(T).nm

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

# VECBOOL
vecbool.o:		vecbool.dir
vecbool.dir:
	makesubdir $@

# ARGMGR
argmgr.o:		argmgr.dir
argmgr.dir:
	makesubdir $@


