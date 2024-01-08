# MAKEFILES (makewhatis)

T= makewhatis

ALL= $(T).x


# extra flags here (usually to turn OFF warnings
CCFLAGSEXTRA= -Wno-format-truncation


BINDIR= $(REPOROOT)/bin
INCDIR= $(REPOROOT)/include
LIBDIR= $(REPOROOT)/lib
MANDIR= $(REPOROOT)/man
INFODIR= $(REPOROOT)/info
HELPDIR= $(REPOROOT)/share/help


CRTDIR= $(CGS_CRTDIR)
VALDIR= $(CGS_VALDIR)
LIBDIR= $(CGS_LIBDIR)

CPP=	cpp
CC=	gcc
CXX=	gpp
LD=	gld
RANLIB=	granlin
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

LDRPATH= $(EXTRA)/lib

LIBDIRS= -L$(LIBDIR) -L/usr/lib

LIBS= -lz


# flag setting
CPPFLAGS= $(DEFS) $(INCDIRS) $(MAKECPPFLAGS)
CFLAGS= $(MAKECFLAGS)
CCFLAGS= $(MAKECCFLAGS) $(CCFLAGSEXTRA)
ARFLAGS= $(MAKEARFLAGS)
LDFLAGS= $(MAKELDFLAGS)


INCS=


OBJ_MAKEWHATIS= main.o


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


$(T).x:			$(OBJ_MAKEWHATIS)
	$(LD) $(LDFLAGS) -o $@ $(OBJ_MAKEWHATIS) $(LIBDIRS) $(LIBS)

$(T).o:			$(OBJ_MAKEWHATIS)
	$(LD) $(LDFLAGS) -r -o $@ $(OBJ_MAKEWHATIS)

$(T).a:			$(OBJ_MAKEWHATIS)
	$(AR) $(ARFLAGS) -rc $@ $?

$(T).nm:		$(T).o
	$(NM) $(NMFLAGS) $(T).o > $(T).nm

$(T).order:		$(OBJ_MAKEWHATIS) $(T).a
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


