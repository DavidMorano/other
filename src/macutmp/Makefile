# MAKEFILE (macutmp)

T= macutmp

ALL= $(T).x


BINDIR= $(REPOROOT)/bin
INCDIR= $(REPOROOT)/include
LIBDIR= $(REPOROOT)/lib
MANDIR= $(REPOROOT)/man

INFODIR= $(REPOROOT)/info
HELPDIR= $(REPOROOT)/share/help

CRTDIR= $(CGS_CRTDIR)
VALDIR= $(CGS_VALDIR)
RUNDIR= $(USRLOCAL)/lib


CPP=	cpp
CC=	gcc
CXX=	gpp
LD=	gld
RANLIB=	granlib
AR=	gar
NM=	gnm
COV=	gcov

LORDER=	lorder
TSORT=	tsort
LINT=	lint
RM=	rm -f
TOUCH=	touch
LINT=	lint


DEFS=

INCS=

LIBS= -lmacuser -lu


INCDIRS=

LIBDIRS= -L$(LIBDIR)


LIBINFO= $(LIBDIRS) $(LIBS)

# flag setting
CPPFLAGS= $(DEFS) $(INCDIRS) $(MAKECPPFLAGS)
CFLAGS= $(MAKECFLAGS)
CCFLAGS= $(MAKECCFLAGS)
ARFLAGS= $(MAKEARFLAGS)
LDFLAGS= $(MAKELDFLAGS) -rpath $(RUNDIR)


OBJ_UTMP= main.o


default:		$(T).x

all:			$(ALL)

.c.ln:
	$(LINT) -c $(LINTFLAGS) $(CPPFLAGS) $<

.c.ls:
	$(LINT) $(LINTFLAGS) $(CPPFLAGS) $<

.c.i:
	$(CPP) $(CPPFLAGS) $< > $(*).i

.c.o:
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $<

.cc.o:
	$(CXX)  $(CPPFLAGS) $(CCFLAGS) -c $<


$(T).x:			$(OBJ_UTMP)
	$(LD) -o $@ $(LDFLAGS) $(OBJ_UTMP) $(LIBINFO)

$(T).o:			$(OBJ_UTMP)
	$(LD) $(LDFLAGS) -r -o $@ $(OBJ_UTMP)

$(T).a:			$(OBJ_UTMP)
	$(AR) $(ARFLAGS) -rc $@ $?

$(T).nm:		$(T).o
	$(NM) $(NMFLAGS) $(T).o > $(T).nm

$(T).order:		$(OBJ_UTMP) $(T).a
	$(LORDER) $(T).a | $(TSORT) > $(T).order
	$(RM) $(T).a
	while read O ; do $(AR) $(ARFLAGS) -cr $(T).a $${O} ; done < $(T).order

again:
	rm -f $(ALL)

clean:
	makeclean $(ALL)

control:
	(uname -n ; date) > Control


main.o:			main.cc


