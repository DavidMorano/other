# MAKEFILE (appleterm)

T= appleterm

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

LIBS += -lu


DEPS_MAIN += 
DEPS_MAIN +=
DEPS_MAIN += 

OBJ0= appleterm_main.o
OBJ1=
OBJ2=
OBJ3= 
OBJ4=
OBJ5=
OBJ6=
OBJ7=

OBJA= obj0.o
OBJB= obj4.o 
#OBJB= obj4.o obj5.o obj6.o obj7.o

OBJ= obja.o


INCDIRS=
LIBDIRS= -L lib

RUNINFO= -rpath $(RUNDIR)
LIBINFO= $(LIBDIRS) $(LIBS)

# flag setting
CPPFLAGS	?= $(DEFS) $(INCDIRS) $(MAKECPPFLAGS)
CFLAGS		?= $(MAKECFLAGS)
CXXFLAGS	?= $(MAKECXXFLAGS) -g
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


appleterm_main.o:	appleterm_main.cc $(DEPS_MAIN)	$(INCS)

# USTREAM
ustream.o:		ustream.dir
ustream.dir:
	makesubdir $@

# STRN
strn.o:			strn.dir
strn.dir:
	makesubdir $@

# SFX
sfx.o:			sfx.dir
sfx.dir:
	makesubdir $@

# SIX
six.o:			six.dir
six.dir:
	makesubdir $@

# RMX
rmx.o:			rmx.dir
rmx.dir:
	makesubdir $@

# HASX
hasx.o:			hasx.dir
hasx.dir:
	makesubdir $@

# ISX
isx.o:			isx.dir
isx.dir:
	makesubdir $@

# BUFOS
bufos.o:		bufos.dir
bufos.dir:
	makesubdir $@

# STRCPTX
strcpyx.o:		strcpyx.dir
strcpyx.dir:
	makesubdir $@

# STRWCPY
strwcpy.o:		strwcpy.dir
strwcpy.dir:
	makesubdir $@

# STRW
strw.o:			strw.dir
strw.dir:
	makesubdir $@

# STRX
strx.o:			strx.dir
strx.dir:
	makesubdir $@

# NLEADX
nleadx.o:		nleadx.dir
nleadx.dir:
	makesubdir $@

# MATXSTR
matxstr.o:		matxstr.dir
matxstr.dir:
	makesubdir $@

# STRNXCMP
strnxcmp.o:		strnxcmp.dir
strnxcmp.dir:
	makesubdir $@

# STRXCMP
strxcmp.o:		strxcmp.dir
strxcmp.dir:
	makesubdir $@

# VARITHMETIC
varithmetic.o:		varithmetic.dir
varithmetic.dir:
	makesubdir $@

# DIGTAB
digtab.o:		digtab.dir
digtab.dir:
	makesubdir $@


char.o:			char.cc		char.h
inetconv.o:		inetconv.cc	inetconv.h
base64.o:		base64.cc	base64.h
base128.o:		base128.cc	base128.h
dictdiff.o:		dictdiff.cc	dictdiff.h

ccfile.o:		ccfile.cc	ccfile.hh
readln.o:		readln.cc	readln.hh

ureade.o:		ureade.cc	ureade.h
ucinetconv.o:		ucinetconv.cc	ucinetconv.h

timeval.o:		timeval.cc	timeval.hh

# TESTINT
testint.o:		testint.ccm	intext.o varithmetic.o loadvals.o

# INTX
intext.o:		intext.ccm	loadvals.o
	gxx -c -x c++ -o $@ -O $<

arithsteps.o:		arithsteps.ccm

loadvals.o:		loadvals.ccm

xxtostr.o:		xxtostr.cc	xxtostr.h

strkeycmp.o:		strkeycmp.cc	strkeycmp.h
matkeystr.o:		matkeystr.cc	matkeystr.h


