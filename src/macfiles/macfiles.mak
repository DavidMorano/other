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


DEFS+=

INCS+=

MODS+= argmgr.ccm filerec.ccm

LIBS+= -luo -lu


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


DEPS_MAIN += argmgr.o filerec.o 
DEPS_MAIN += ureserve.o fonce.o
DEPS_MAIN += strfilter.o 
DEPS_MAIN += modproc.o modloadnames.o

OBJ_FILES += files_main.o 
OBJ_FILES += argmgr.o filerec.o
OBJ_FILES += modproc.o modloadnames.o

OBJ_MAIN= obj_main.o


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


$(T).x:			$(OBJ_MAIN)
	$(CXX) -o $@ $(LDFLAGS) $(RUNINFO) $(OBJ_MAIN) $(LIBINFO)

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


obj_main.o:		$(OBJ_FILES)
	$(CXX) -r -o $@ $(LDFLAGS) $^

files_main.o:		files_main.cc $(DEPS_MAIN)		$(INCS)

files_tardir.o:		files_tardir0.o files_tardir1.o		$(INCS)

files_tardir0.o:	files_tardir.ccm			$(INCS)

files_tardir1.o:	files_tardir1.cc files_tardir.ccm	$(INCS)
	makemodule files_tardir
	$(COMPILE.cc) $<

# MODS
mods.o:			$(DEPS_MAIN)
	$(CXX) -r -o $@ $(LDFLAGS) $^

# URESERVE (libu)
ureserve.o:		ureserve.dir
ureserve.dir:
	makesubdir $@

# FONCE (libu)
fonce.o:		fonce.dir
fonce.dir:
	makesubdir $@

# STRFILER (libuc)
strfilter.o:		strfilter.dir
strfilter.dir:
	makesubdir $@

# SIF (libuc)
sif.o:			sif.dir
sif.dir:
	makesubdir $@

# FILEREC
filerec.o:		filerec.dir
filerec.dir:
	makesubdir $@

# ARGMGR
argmgr.o:		argmgr.dir
argmgr.dir:
	makesubdir $@

# TARDIR
tardir.o:		tardir.dir
tardir.dir:
	makesubdir $@

# LOADMODNAMES
modloadnames.o:		modloadnames.dir
modloadnames.dir:
	makesubdir $@

# DEBUG
debug.o:		debug.dir
debug.dir:
	makesubdir $@

# MODPROC
modproc.o:		modproc.dir
modproc.dir:
	makesubdir $@


