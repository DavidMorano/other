# MAKEFILE (cutlines)

T= cutlines

ALL= $(T) cl

SRCROOT= $(USRLOCAL)


BINDIR= $(SRCROOT)/bin


all:		$(ALL)

$(T):		$(T).ksh
	rm -f $(T)
	ln $(T).ksh $@

cl:		$(T).ksh
	ln $(T).ksh $@

install:	$(ALL)
	makenewer -r $(ALL) $(BINDIR)

clean:
	rm -f $(T)

