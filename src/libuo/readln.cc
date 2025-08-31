/* readln SUPPORT */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* read characters from a C++ input stream */
/* version %I% last-modified %G% */

#define	CF_DEBUG	0		/* debugging */

/* revision history:

	= 1998-04-10, David A­D­ Morano
        This code was originally written.

	= 2023-12-28, David A­D­ Morano
	This is refactored.  I went from using |std::getline(3c++)|
	to |std::istream::getline(3c++)|.  Why?  I do not really
	know.

*/

/* Copyright © 1998,2023 David A­D­ Morano.  All rights reserved. */

/******************************************************************************* 
	Name:
	readln

	Description:
	This subroutine tries to mimic (somewhat) the old normal
	'read' function from the old days.  The "modern"
	|std::istream::getline(3c++)| and |std::getline(3c++)| want
	to be in the middle of a sequence of input operations (like
	reading various typed fields from the input file).  So they
	return something like the type |istream &| so that they can
	be chained together.  But I want to have the older style
	semantic where either the number of characters read is
	returned or an |errno(3)| error code is returned.  So thia
	subroutine returns an integer.

	Synopsis:
	int readln(istream *idp,char *ibuf,int ilen,int chdelim = eol) noex

	Arguments:
	isp		input stream pointer
	ibuf		input character buffer to store read characteds
	ilen		input character buffer length
	chdelim		optional delimiter character

	Returns:
	>=0		number characters read, including terminating char
	<0		error (system-return)

	Implementation-notes:
	1. The C++ standard streams interface is real garbage!  More
	specifically, but not only, the various "failure" states
	(EOF is consided a "failure") are very confusing in and of
	themselves without varefully examining the conditions under
	which each can occur.  One should carefully only check the
	"failure" states in the following order: EOF, then BAD",
	then FAIL.
	2. Since the (stupid) |std::istream::getline(3c++)| 
	interface is screwed up (by design because they thought
	the current arrangement is better), I have to add back
	any delimiter character to the caller-supplied input buffer
	so that the caller can determine if there was a delimiter
	character or not by testing the input buffer (its last
	filled postition) directly.

	Implementation notes:
	1. I hate to be repetitive, but I would just like to reiterate
	how one must test for failure in this (stupid) function
	interface (the |std::istream::getline(3c++)|).  Firstly,
	in the mind of the interface inventor, an EOF is not a
	"normal" result but rather a "failure" (as it is described).
	With this in mind, the various "failures" (of which there
	are three principal ones) need to be checked in the following
	order:
		EOF		(represented by the EOL bit)
		BAD		(represented by the BAD bit)
		FAIL		(represented by the FAIL bit)
	Thank for your attention to this matter.

*******************************************************************************/

#include	<envstandards.h>	/* MUST be first to configure */
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<istream>
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>
#include	<localmisc.h>		/* |eol(3local)| */

#include	"readln.hh"

#pragma		GCC dependency		"mod/debug.ccm"

import debug ;

/* local defines */

#ifndef	CF_DEBUG
#define	CF_DEBUG	1		/* debugging */
#endif


/* imported namespaces */

using std::istream ;			/* type */
using std::streamsize ;			/* type */


/* local typedefs */


/* external subroutines */


/* external variables */


/* local structures */


/* forward references */


/* local variables */

cbool		f_debug = CF_DEBUG ;


/* exported variables */


/* exported subroutines */

int readln(istream *isp,char *ibuf,int ilen,int dch) noex {
	int		rs = SR_FAULT ;
	int		len = 0 ; /* return-value */
	if (dch <= 0) dch = eol ;
	if (isp && ibuf) ylikely {
	    rs = SR_INVALID ;
	    if_constexpr (f_debug) {
		debprintf(__func__,"ent ilen=%d\n",ilen) ;
	    }
	    if (ilen >= 0) {
		streamsize isize = streamsize(ilen + 1) ;
	        try {
		    rs = SR_BADFMT ;
	            if (bool(isp->getline(ibuf,isize,char(dch)))) ylikely {
		        csize	qsize = isp->gcount() ;
	    		if_constexpr (f_debug) {
			    debprintf(__func__,"qsize=%ld\n",qsize) ;
	    		}
		        if ((rs = int(qsize)) <= ilen) ylikely {
			    len = rs ;
			    if ((len > 0) && (ibuf[len - 1] == '\0')) {
			        ibuf[len - 1] = char(dch) ;
			        ibuf[len] = '\0' ;
			    }
		        } else {
			    rs = SR_OVERFLOW ;
		        } /* end if (adding delimiter to input buffer) */
		    } else {
		        cbool	feof	= isp->eof() ;
			if (feof) {
		            rs = SR_OK ;
		        } else {
		            cbool	fbad	= isp->bad() ;
		            cbool	ffail	= isp->fail() ;
			    if (fbad) {
		                rs = SR_IO ;
		            } else if (ffail) {
		                rs = SR_NOMSG ;
		            }
			} /* end if */
	            } /* end if (good or failure) */
	        } catch (...) {
		    rs = SR_NOMEM ;
	        }
	    } /* end if (valid) */
	} /* end if (non-null) */
	if_constexpr (f_debug) {
	    debprintf(__func__,"ret rs=%d len=%d\n",rs,len) ;
	}
	return (rs >= 0) ? len : rs ;
}
/* end subroutine (readln) */

extern int istr_readln(istream *isp,char *ibuf,int ilen,int dch) noex {
	return readln(isp,ibuf,ilen,dch) ;
}


