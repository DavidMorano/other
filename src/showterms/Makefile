# MAKEFILE (showterms)

T= showterms

ALL= $(T).x


BINDIR= $(REPOROOT)/bin
INCDIR= $(REPOROOT)/include
LIBDIR= $(REPOROOT)/lib
MANDIR= $(REPOROOT)/man

INFODIR= $(REPOROOT)/info
HELPDIR= $(REPOROOT)/share/help
LDRPATH= $(REPOROOT)/lib

CRTDIR= $(CGS_CRTDIR)
VALDIR= $(CGS_VALDIR)


CPP= cpp
CC= gcc
CXX= gpp
LD= gld
RANLIB= granlib
AR= gar
NM= gnm
COV= gcov

LORDER= lorder
TSORT= tsort
LINT= lint
RM= rm -f
TOUCH= touch
LINT= lint


DEFS +=


INCS += sif.hh ccfile.hh


LIBS += -lmacuser


#LIB= field.a
LIB= 


INCDIRS +=

LIBDIRS += -L$(LIBDIR) -L$(PWD)

# flag setting
CPPFLAGS= $(DEFS) $(INCDIRS) $(MAKECPPFLAGS)
CFLAGS= $(MAKECFLAGS)
CCFLAGS= $(MAKECCFLAGS)
ARFLAGS= $(MAKEARFLAGS)
LDFLAGS= $(MAKELDFLAGS)


OBJ_SHOWTERMS= main.o


default:		$(T).x

all:			$(ALL)

.c.ln:
	$(LINT) -c $(LINTFLAGS) $(CPPFLAGS) $<

.c.ls:
	$(LINT) $(LINTFLAGS) $(CPPFLAGS) $<

.c.i:
	$(CPP) $(CPPFLAGS) $< > $(*).i

.c.o:
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $<

.cc.o:
	$(CXX) -c $(CPPFLAGS) $(CCFLAGS) $<


$(T).x:			$(OBJ_SHOWTERMS) $(LIB)
	$(CXX) $(LDFLAGS) -o $@ $(OBJ_SHOWTERMS) $(LIBDIRS) $(LIBS)

$(T).nm:		$(T).x
	$(NM) $(NMFLAGS) $(T).so > $(T).nm

again:
	rm -f $(TALL)

clean:
	makeclean $(ALL)

control:
	(uname -n ; date) > Control


# FIELD
field.a:		field.dir
field.o:		field.dir
field.dir:
	makesubdir $@

main.o:			main.cc		$(INCS)

sif.o:			sif.cc sif.hh
ccfile.o:		ccfile.cc ccfile.hh

