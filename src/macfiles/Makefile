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


DEFS +=

INCS +=

MODS += argmgr.ccm filerec.ccm
MODS += cmdutils.ccm
MODS += modproc.ccm 

LIBS += -luo -lu


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


DEPS_MAIN += cmdutils.o
DEPS_MAIN += argmgr.o filerec.o 
DEPS_MAIN += ureserve.o fonce.o
DEPS_MAIN += strfilter.o tardir.o
DEPS_MAIN += langx.o modproc.o

OBJ0= files_main.o cmdutils.o
OBJ1= argmgr.o filerec.o tardir.o
OBJ2= modproc.o langx.o
OBJ3= shortq.o ischarx.o
OBJ4= six.o
OBJ5=
OBJ6=
OBJ7=

OBJA= obj0.o obj1.o obj2.o obj3.o
OBJB= obj4.o

OBJ_MAIN= obja.o objb.o


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


obj_main.o:		$(OBJ_MAIN)
	$(CXX) -r -o $@ $(LDFLAGS) $^


obj0.o:			$(OBJ0)
	$(CXX) -r -o $@ $(LDFLAGS) $^

obj1.o:			$(OBJ1)
	$(CXX) -r -o $@ $(LDFLAGS) $^

obj2.o:			$(OBJ2)
	$(CXX) -r -o $@ $(LDFLAGS) $^

obj3.o:			$(OBJ3)
	$(CXX) -r -o $@ $(LDFLAGS) $^

obj4.o:			$(OBJ4)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj5.o:			$(OBJ5)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj6.o:			$(OBJ6)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj7.o:			$(OBJ7)
	$(LD) -r $(LDFLAGS) -o $@ $^


obja.o:			$(OBJA)
	$(CXX) -r -o $@ $(LDFLAGS) $^

objb.o:			$(OBJB)
	$(CXX) -r -o $@ $(LDFLAGS) $^


files_main.o:		files_main.cc $(DEPS_MAIN)		$(INCS)
files_helpdata.o:	files_helpdata.cc			$(INCS)

# MODS
mods.o:			$(DEPS_MAIN)
	$(CXX) -r -o $@ $(LDFLAGS) $^

# URESERVE	(libu)
ureserve.o:		ureserve.dir
ureserve.dir:
	makesubdir $@

# FONCE		(libu)
fonce.o:		fonce.dir
fonce.dir:
	makesubdir $@

# FILEREC	(libu)
filerec.o:		filerec.dir
filerec.dir:
	makesubdir $@

# DEBUG		(libu)
debug.o:		debug.dir
debug.dir:
	makesubdir $@

# STRFILER	(libuc)
strfilter.o:		strfilter.dir
strfilter.dir:
	makesubdir $@

# SIF		(libuc)
sif.o:			sif.dir
sif.dir:
	makesubdir $@

# ARGMGR	(libuc)
argmgr.o:		argmgr.dir
argmgr.dir:
	makesubdir $@

# TARDIR	(libuc)
tardir.o:		tardir.dir
tardir.dir:
	makesubdir $@

# LANGX		(libdam)
langx.o:		langx.dir
langx.dir:
	makesubdir $@

# MODPROC	(libdam)
modproc.o:		modproc.dir
modproc.dir:
	makesubdir $@

# CMD_UTILS
cmdutils.o:		cmdutils.dir
cmdutils.dir:
	makesubdir $@

# SIX
six.o:			six.dir
six.dir:
	makesubdir $@

shortq.o:		shortq.cc	shortq.h		$(INCS)
ischarx.o:		ischarx.cc	ischarx.h


