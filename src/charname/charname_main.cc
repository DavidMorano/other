/* charname_main SUPPORT (charnames) */
/* charset=ISO8859-1 */
/* lang=C++20 (conformance reviewed) */

/* print out the name of a character given a value */
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
	charname

	Description:
	Print out the name of a character given a value.

*******************************************************************************/

#include	<envstandards.h>	/* ordered first to configure */
#include	<climits>		/* CSTD |UCHAR_MAX| */
#include	<cstddef>		/* CSTD */
#include	<cstdlib>		/* CSTD */
#include	<string_view>		/* C++STD */
#include	<clanguage.h>		/* LIBU */
#include	<usysbase.h>		/* LIBU */
#include	<usyscalls.h>		/* LIBU */
#include	<usupport.h>		/* LIBU |cfdec(3u)| */
#include	<intceil.h>		/* LIBU */
#include	<strkeycmp.h>		/* LIBUC */
#include	<charnames.h>		/* **local** */
#include	<matkeystr.h>		/* LIBUC */
#include	<strnul.hh>		/* LIBU */
#include	<mapex.h>		/* LIBU */
#include	<localmisc.h>		/* LIBU |COLUMNS| + |DECBUFLEN| */
#include	<dprint.hh>		/* LIBU |DPRINTF(3u)| */

#pragma		GCC dependency		"mod/libutil.ccm"

import libutil ;			/* |lenstr(3u)| */

/* local defines */

#ifndef	CF_DEBUG
#define	CF_DEBUG	0		/* special debugging */
#endif


/* imported namespaces */

using libu::cfdec ;			/* subroutine */
using libu::strwcmp ;			/* subroutine */


/* typ-defs */


/* external subroutines */


/* external variables */


/* local structures */


/* forward references */

local int	cfnum(cchar *,int,int *) noex ;


/* local variables */

constexpr mapex_map	mapexs[] = {
	{ SR_NOENT,	EX_NOUSER	},
	{ SR_AGAIN,	EX_TEMPFAIL	},
	{ SR_DEADLK,	EX_TEMPFAIL	},
	{ SR_NOLCK,	EX_TEMPFAIL	},
	{ SR_TXTBSY,	EX_TEMPFAIL	},
	{ SR_ACCESS,	EX_NOPERM	},
	{ SR_REMOTE,	EX_PROTOCOL	},
	{ SR_NOSPC,	EX_TEMPFAIL	},
	{ SR_INTR,	EX_INTR		},
	{ SR_EXIT,	EX_TERM		},
	{ SR_DOM,	EX_NOPROG	},
	{ 0, 0 }
} ; /* end array (mapexs) */

cbool			f_debug = CF_DEBUG ;


/* exported variables */


/* exported subroutines */

int main(int argc,con mainv argv,con mainv) {
	int		rs = SR_OK ;
	int		ex = EX_OK ;
	DPRINTF("ent\n") ;
	if (argc > 1) {
	    for (int ai = 1 ; ai < argc ; ai += 1) {
	        if (cchar *ap = argv[ai] ; ap[0]) {
	            if (int v ; (rs = cfnum(ap,-1,&v)) >= 0) {
			v &= UCHAR_MAX ;
			cchar *cp = charname[v] ;
			printf("0x%02X %s\n",v,cp) ;
		    } /* end if (cfdec) */
	        } /* end if (non-empty) */
		if (rs < 0) break ;
	    } /* end for */
	} /* end if (arguments) */
	if ((ex == EX_OK) && (rs < 0)) {
	    ex = mapex(mapexs,rs) ;
	}
	DPRINTF("ret rs=%d ex=%u\n",rs,ex) ;
	return ex ;
} /* end subroutine (main) */


/* local subroutines */

local int cfnum(cchar *sp,int µsl,int *rp) noex {
    	int		rs = SR_FAULT ;
	if (int sl = getlenstr(sp,µsl) ; sl >= 0) {
	    rs = SR_INVALID ;
	    if (sl > 0) {
		if ((sl > 2) && (strwcmp("0x",sp,2) == 0)) {
		    strnul ss((sp+2),(sl-2)) ;
		    DPRINTF("hex >%s<\n",ccp(ss)) ;
		    *rp = strtoi(ccp(ss),nullptr,16) ;
		    DPRINTF("errno=%d\n",errno) ;
		    rs = (errno) ? (neg errno) : SR_OK ;
		} else {
		    rs = cfdec(sp,sl,rp) ;
		}
	    } /* end if (valid) */
	} /* end if (getlenstr) */
	DPRINTF("ret rs=%d\n",rs) ;
	return rs ;
} /* end subroutine (cfnum) */


