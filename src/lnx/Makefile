# MAKEFILE (lnx)

T= lnx

ALL= $(T)

SRCROOT= $(USRLOCAL)


BINDIR= $(SRCROOT)/bin


all:		$(ALL)

$(T):		$(T).ksh
	rm -f $(T)
	ln $(T).ksh $@

install:	$(ALL)
	makenewer -r $(ALL) $(BINDIR)

clean:
	rm -f $(T)

