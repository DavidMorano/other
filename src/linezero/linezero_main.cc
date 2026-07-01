/* linezero_main SUPPORT (linezero) */
/* charset=ISO8859-1 */
/* lang=C++20 (conformance reviewed) */

/* test some aspect of environment handling */
/* version %I% last-modified %G% */

#define	CF_DEBUG	0		/* special debugging */

/* revision history:

	= 2001-11-01, David A­D­ Morano
	This subroutine was written for use as a front-end for Korn
	Shell (KSH) commands that are compiled as stand-alone
	programs.

*/

/* Copyright © 2001 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

  	Name:
	linezero

	Description:
	Read lines from STDIN and write them on STDOUT but with the
	new-line (NL) character changed into a zero (NUL) character.

*******************************************************************************/

#include	<envstandards.h>	/* ordered first to configure */
#include	<cstddef>		/* CSTD */
#include	<cstdlib>		/* CSTD */
#include	<cstdio>		/* CSTD */
#include	<string_view>		/* C++STD */
#include	<clanguage.h>		/* C++STD */
#include	<usysbase.h>		/* LIBU */
#include	<usupport.h>		/* LIBU |cfdec(3u)| */
#include	<usyscalls.h>		/* LIBU */
#include	<intceil.h>		/* LIBU */
#include	<strkeycmp.h>		/* LIBUC */
#include	<matkeystr.h>		/* LIBUC */
#include	<ascii.h>		/* LIBU */
#include	<stdfnames.h>		/* LIBU */
#include	<strnul.hh>		/* LIBU */
#include	<ccfile.hh>		/* LIBU */
#include	<mapex.h>		/* LIBU */
#include	<localmisc.h>		/* LIBU |COLUMNS| + |DECBUFLEN| */
#include	<dprint.hh>		/* LIBU |DPRINTF(3u)| */
#include	<libf.h>		/* LIBF |fwriter(3f)| */
#include	<prognamevar.hh>	/* LIBDAM */


/* local defines */

#ifndef	CF_DEBUG
#define	CF_DEBUG	0		/* special debugging */
#endif


/* imported namespaces */


/* typ-defs */


/* external subroutines */


/* external variables */


/* local structures */


/* forward references */


/* local variables */

constexpr mapex_map	mapexs[] = {
	{ SR_NOENT,	EX_NOUSER },
	{ SR_AGAIN,	EX_TEMPFAIL },
	{ SR_DEADLK,	EX_TEMPFAIL },
	{ SR_NOLCK,	EX_TEMPFAIL },
	{ SR_TXTBSY,	EX_TEMPFAIL },
	{ SR_ACCESS,	EX_NOPERM },
	{ SR_REMOTE,	EX_PROTOCOL },
	{ SR_NOSPC,	EX_TEMPFAIL },
	{ SR_INTR,	EX_INTR },
	{ SR_EXIT,	EX_TERM },
	{ SR_DOM,	EX_NOPROG },
	{ 0, 0 }
} ; /* end array (mapexs) */

cint			maxpathlen = MAXPATHLEN ;
cbool			f_debug = CF_DEBUG ;


/* exported variables */


/* exported subroutines */

int main(int argc,con mainv argv,con mainv envv) {
    	FILE		*ofp = stdout ;
	prognamevar	pn(argc,argv,envv) ;
    	cnullptr	np{} ;
    	cnothrow	nt{} ;
	int		rs = SR_OK ;
	int		rs1 ;
	int		ex = EX_OK ;
	DPRINTF("ent\n") ;
	if (char *lbuf ; (lbuf = new(nt) char[maxpathlen + 1]) != np) {
	    cint llen = maxpathlen ;
	    if (ccfile fin ; (rs = fin.open(STDFNIN)) >= 0) {
		while ((rs = fin.readln(lbuf,llen)) > 0) {
		    if (lbuf[rs - 1] == CH_NL) {
			lbuf[rs - 1] = '\0' ;
			rs = fwriter(ofp,lbuf,rs) ;
		    } /* end if */
		    if (rs < 0) break ;
		} /* end while */
	        rs1 = fin.close ;
	        if (rs >= 0) rs = rs1 ;
	    } /* end if (ccfile) */
	    delete [] lbuf ;
	} /* end if (m-a-f) */
	if ((ex == EX_OK) && (rs < 0)) {
	    ex = mapex(mapexs,rs) ;
	    fprintf(stderr,"%s: error ex=%d rs=%d\n",ccp(pn),ex,rs) ;
	}
	DPRINTF("ret rs=%d ex=%u\n",rs,ex) ;
	return ex ;
} /* end subroutine (main) */


/* local subroutines */


