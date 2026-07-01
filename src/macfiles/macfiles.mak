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
MODS += strfilter.ccm 

LIBS += -lf -lu -lsecdb -lnss


DEPS_MAIN += cmdutils.o matxstr.o strfilter.o 
DEPS_MAIN += argmgr.o filerec.o 
DEPS_MAIN += strfilter.o tardir.o
DEPS_MAIN += modproc.o sif.o

OBJ0= files_main.o files_show.o
OBJ1= argmgr.o filerec.o tardir.o
OBJ2= modproc.o matxstr.o nleadx.o
OBJ3= sif.o cmdutils.o strcpyx.o
OBJ4= filelinker.o ucx.o inetaddrx.o
OBJ5= strfilter.o strx.o strn.o
OBJ6= char.o isx.o rmx.o six.o sfx.o
OBJ7= deb.o strnxcmp.o strxcmp.o

OBJ8= strw.o strwcpy.o
OBJ9=
OBJ10=
OBJ11=
OBJ12=
OBJ13=
OBJ14=
OBJ15=

OBJA= obj0.o obj1.o obj2.o obj3.o
OBJB= obj4.o obj5.o obj6.o obj7.o
OBJC= obj8.o 

OBJ= obja.o objb.o objc.o


INCDIRS=
LIBDIRS= -L lib

RUNINFO= -rpath $(RUNDIR)
LIBINFO= $(LIBDIRS) $(LIBS)

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
	gxx -c -x c++ -o $@ -O $<


$(T).x:			obj.o
	$(CXX) -o $@ $(LDFLAGS) $(RUNINFO) $^ $(LIBINFO)

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


obj8.o:			$(OBJ8)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj9.o:			$(OBJ9)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj10.o:		$(OBJ10)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj11.o:		$(OBJ11)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj12.o:		$(OBJ12)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj13.o:		$(OBJ13)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj14.o:		$(OBJ14)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj15.o:		$(OBJ15)
	$(LD) -r $(LDFLAGS) -o $@ $^


obja.o:			$(OBJA)
	$(CXX) -r -o $@ $(LDFLAGS) $^

objb.o:			$(OBJB)
	$(CXX) -r -o $@ $(LDFLAGS) $^

objc.o:			$(OBJC)
	$(CXX) -r -o $@ $(LDFLAGS) $^

objd.o:			$(OBJD)
	$(CXX) -r -o $@ $(LDFLAGS) $^


obj.o:			$(OBJ)
	$(CXX) -r -o $@ $(LDFLAGS) $^


files_main.o:		files_main.cc $(DEPS_MAIN)		$(INCS)
files_show.o:		files_show.cc $(DEPS_MAIN)		$(INCS)
files_helpdata.o:	files_helpdata.cc			$(INCS)
ilelinker.o:		filelinker.cc	filelinker.hh		$(INCS)

# MODS
mods.o:			$(DEPS_MAIN)
	$(CXX) -r -o $@ $(LDFLAGS) $^

# FILEREC	(libu)
filerec.o:		filerec.dir
filerec.dir:
	makesubdir $@

# DEBUG		(libu)
deb.o:			deb.dir
deb.dir:
	makesubdir $@

# UCX		(libuc)
ucx.o:			ucx.dir
ucx.dir:
	makesubdir $@

# STRFILER	(libuc)
strfilter.o:		strfilter.dir
strfilter.dir:
	makesubdir $@

# SIF		(libuc)
sif.o:			sif.dir
sif.dir:
	makesubdir $@

# SIX		(libuc)
six.o:			six.dir
six.dir:
	makesubdir $@

# SFX		(libuc)
sfx.o:			sfx.dir
sfx.dir:
	makesubdir $@

# QUEUE		(libuc)
queue.o:		queue.dir
queue.dir:
	makesubdir $@

# ARGMGR	(libuc)
argmgr.o:		argmgr.dir
argmgr.dir:
	makesubdir $@

# TARDIR	(libuc)
tardir.o:		tardir.dir
tardir.dir:
	makesubdir $@

# FILEOBJ	(libuc)
fileobj.o:		fileobj.dir
fileobj.dir:
	makesubdir $@

# STRX		(libuc)
strn.o:			strn.dir
strn.dir:
	makesubdir $@

# STRCPYX	(libuc)
strcpyx.o:		strcpyx.dir
strcpyx.dir:
	makesubdir $@

# STRX		(libuc)
strx.o:			strx.dir
strx.dir:
	makesubdir $@

# STRW		(libuc)
strw.o:			strw.dir
strw.dir:
	makesubdir $@

# STRWCPY		(libuc)
strwcpy.o:		strwcpy.dir
strwcpy.dir:
	makesubdir $@

# RMX		(libuc)
rmx.o:			rmx.dir
rmx.dir:
	makesubdir $@

# MATXSTR	(libuc)
matxstr.o:		matxstr.dir
matxstr.dir:
	makesubdir $@

# NLEADX	(libuc)
nleadx.o:		nleadx.dir
nleadx.dir:
	makesubdir $@

# ISX		(libuc)
isx.o:			isx.dir
isx.dir:
	makesubdir $@

# STRNXCMP	(libuc)
strnxcmp.o:		strnxcmp.dir
strnxcmp.dir:
	makesubdir $@

# STRXCMP	(libuc)
strxcmp.o:		strxcmp.dir
strxcmp.dir:
	makesubdir $@

# INETADDRX	(libuc)
inetaddrx.o:		inetaddrx.dir
inetaddrx.dir:
	makesubdir $@

# MODPROC	(libdam)
modproc.o:		modproc.dir
modproc.dir:
	makesubdir $@

# KEYOPT	(libdam)
keyopt.o:		keyopt.dir
keyopt.dir:
	makesubdir $@

# CMDUTILS	(libdam)
cmdutils.o:		cmdutils.dir
cmdutils.dir:
	makesubdir $@

char.o:			char.cc		char.h
ischarx.o:		ischarx.cc	ischarx.h


