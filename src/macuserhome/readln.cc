/* readln SUPPORT */
/* lang=C++20 */

/* read characters from Standard-Input (STDIN) */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-04-10, David A­D­ Morano
        This subroutine was originally written.

	= 2023-12-28, David A­D­ Morano
	This is (substantially) refactored. I went from using
	|std::getline(3c++)| to |std::iostream::getline(3c++)|.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

/******************************************************************************* 
	Name:
	readln

	Description:
	This subroutine tries to mimic the old normal 'read' function
	from the old days.  The "modern" |istread::getline(3c++)|
	and |std::getline(3c++)| want to be in the middle of a
	sequence of input operations (like reading various typed
	fields from the input file).  So they return something like
	the type |istream &| so that they can be chained together.
	But I want to have the older style semantic where either
	the number of characters read is returned or an |errno(3)|
	error code is returned.

	Synopsis:
	int readln(istream &cin,char *ibuf,int ilen,int chdelim = eol) noex

	Arguments:
	cin		input |istream(3c++)| to read from
	ibuf		input character buffer to store read characteds
	ilen		input character buffer length
	chdelim		optional delimiter character

	Returns:
	>=0		number characters read, including terminating char
	<0		error (system-return)

	Implementation-node:
	:-)
	This is the second implementation of this (2023-12-28).

*******************************************************************************/

#include	<envstandards.h>	/* MUST be first to configure */
#include	<istream>
#include	<usystem.h>
#include	<storebuf.h>		/* <- not used! */
#include	<sncpyx.h>		/* <- not used! */
#include	<snwcpy.h>
#include	<localmisc.h>		/* |eol(3uc)| */

#include	"readln.hh"


/* local defines */


/* imported namespaces */

using std::istream ;			/* type */


/* local typedefs */


/* external subroutines */


/* external variables */


/* forward references */


/* local variables */


/* exported variables */


/* exported subroutines */

int readln(istream *isp,char *ibuf,int ilen,int dch) noex {
	int		rs = SR_FAULT ;
	int		len = 0 ;
	if (dch == 0) dch = eol ;
	if (isp && ibuf) {
	    try {
		rs = SR_BADFMT ;
	        if (bool(isp->getline(ibuf,(ilen+1),char(dch)))) {
		    if ((rs = isp->gcount()) <= ilen) {
			len = rs ;
			if (len > 0) {
			    ibuf[len-1]= dch ;
			    ibuf[len] = '\0' ;
			}
		    } else {
			rs = SR_OVERFLOW ;
		    } /* end if (adding delimiter to input buffer) */
		} else {
		    cbool	feof	= isp->eof() ;
		    cbool	ffail	= isp->fail() ;
		    cbool	fbad	= isp->bad() ;
		    if (feof) {
		        rs = SR_OK ;
		    } else if (ffail) {
		        rs = SR_NOMSG ;
		    } else if (fbad) {
		        rs = SR_IO ;
	            }
	        } /* end block */
	    } catch (...) {
		rs = SR_NOMEM ;
	    }
	} /* end if (non-null) */
	return (rs >= 0) ? len : rs ;
}
/* end subroutine (readln) */


