# MAKEFILE (mkfindbit)

T= mkfindbit

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

MODS +=

LIBS += -lf -lu


DEPS_MAIN +=

OBJ0= mkfindbit_main.o
OBJ1= deb.o
OBJ2=
OBJ3=

OBJ= obj0.o obj1.o


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


obj0.o:			$(OBJ0)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj1.o:			$(OBJ1)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj2.o:			$(OBJ2)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj3.o:			$(OBJ3)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj4.o:			$(OBJ4)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj5.o:			$(OBJ5)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj6.o:			$(OBJ6)
	$(LD) -r $(LDFLAGS) -o $@ $^

obj7.o:			$(OBJ7)
	$(LD) -r $(LDFLAGS) -o $@ $^


obja.o:			$(OBJA)
	$(LD) -r $(LDFLAGS) -o $@ $^

objb.o:			$(OBJB)
	$(LD) -r $(LDFLAGS) -o $@ $^


obj.o:			$(OBJ)
	$(LD) -r $(LDFLAGS) -o $@ $^


mkfindbit_main.o:	mkfindbit_main.cc $(DEPS_MAIN)		$(INCS)

# FONCE		(libu)
fonce.o:		fonce.dir
fonce.dir:
	makesubdir $@

# DEB		(libuc)
deb.o:			deb.dir
deb.dir:
	makesubdir $@


