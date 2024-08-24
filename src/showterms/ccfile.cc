/* ccfile SUPPORT */
/* lang=C++20 */

/* C++ file I-O operations */
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
	from the old days.  The "modern" |istream::getline(3c++)|
	and |std::getline(3c++)| want to be in the middle of a
	sequence of input operations (like reading various typed
	fields from the input file).  So they return something like
	the type |istream &| so that they can be chained together.
	But I want to have the older style semantic where either
	the number of characters read is returned or an |errno(2)|
	error code is returned

	Synopsis:
	int readln(istream &cin,char *ibuf,int ilen,int chdelim = eol) noex

	Arguments:
	cin		input |istream(3c++)| to read from
	ibuf		input character buffer to store read characteds
	ilen		input character buffer length
	chdelim		optional delimiter character

	Returns:
	>=0		number characters read, including terminating char
	<0		error code (system-return)

	Implementation-node:
	:-)
	This is the second implementation of this (2023-12-28).

*******************************************************************************/

#include	<envstandards.h>	/* MUST be first to configure */
#include	<string>
#include	<string_view>
#include	<istream>
#include	<usystem.h>
#include	<storebuf.h>		/* <- not used! */
#include	<sncpyx.h>		/* <- not used! */
#include	<snwcpy.h>
#include	<strnul.hh>
#include	<mkchar.h>
#include	<localmisc.h>

#include	"ccfile.hh"


/* local defines */

#ifndef	MAXLINE
#define	MAXLINE		(4*1024)
#endif


/* imported namespaces */

using std::istream ;			/* type */
using std::fstream ;			/* type */
using std::ios_base ;			/* type */
using std::basic_ios ;			/* type */
using std::error_code ;			/* type */
using std::ios ;			/* type (|basic_ios<char>|) */
using std::string ;			/* type */
using std::string_view ;		/* type */
using std::nothrow ;			/* constant */

using openmode = std::ios_base::openmode ; /* type */


/* local typedefs */

typedef string_view	strview ;


/* external subroutines */


/* external variables */


/* forward references */

static ios_base::openmode getopenmode(cchar *) noex ;


/* local variables */


/* exported variables */


/* exported subroutines */

int ccfile::open(cchar *fn,cchar *ofs,mode_t) noex {
	int		rs = SR_FAULT ;
	if (fn) {
	    rs = SR_INVALID ;
	    if (fn[0]) {
		openmode	of = ios::in ;
		rs = SR_OK ;
		if (ofs) {
		    rs = SR_INVALID ;
		    if (ofs[0]) {
			rs = SR_OK ;
			of = getopenmode(ofs) ;
		    }
		} /* end if (had open-flags) */
		if (rs >= 0) {
		    fstream::open(fn,of) ;
		    rs = SR_NOENT ;
		    if (good()) {
			if (of & ios::in) freading = true ;
		        rs = SR_OK ;
		    }
		} /* end if (ok) */
	    } /* end if (valid) */
	} /* end if (non-null) */
	return rs ;
}
/* end method (ccfile::open) */

int ccfile::open(strview sv,cchar *ofs,mode_t om) noex {
	csize		svz = sv.length() ;
	cchar		*svp = sv.data() ;
	int		rs = SR_FAULT ;
	if (svp && ofs) {
	    cint	svl = int(svz) ;
	    {
	        strnul	fn(svp,svl) ;
		rs = open(fn,ofs,om) ;
	    } /* end block */
	}
	return rs ;
}
/* end method (ccfile::open) */

int ccfile::readln(char *ibuf,int ilen,int dch) noex {
	int		rs = SR_FAULT ;
	int		len = 0 ;
	if (dch == 0) dch = eol ;
	if (ibuf) {
	    try {
		rs = SR_BADFMT ;
	        if (bool(getline(ibuf,(ilen+1),char(dch)))) {
		    if ((rs = gcount()) <= ilen) {
			len = rs ;
			if (len > 0) {
			    ibuf[len-1] = dch ;
			    ibuf[len] = '\0' ;
			}
		    } else {
			rs = SR_OVERFLOW ;
		    } /* end if (adding delimiter to input buffer) */
		} else {
		    cbool	feof	= eof() ;
		    cbool	ffail	= fail() ;
		    cbool	fbad	= bad() ;
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
/* end method (ccfile::readln) */

int ccfile::readln(string &s,int dch) noex {
	cint		llen = MAXLINE ;
	char		*lbuf ;
	int		rs = SR_NOMEM ;
	if ((lbuf = new(nothrow) char[llen+1]) != nullptr) {
	    if ((rs = readln(lbuf,llen,char(dch))) >= 0) {
		lbuf[rs] = '\0' ;
		try {
		    s = lbuf ;
		} catch (...) {
		    rs = SR_NOMEM ;
		}
	    } /* end if (readln) */
	    delete [] lbuf ;
	} /* end if (m-a-f) */
	return rs ;
}
/* end method (ccfile::readln) */

int ccfile::seek(off_t o,int w) noex {
	int		rs = SR_OK ;
	seekdir		sv = beg ;
	switch (w) {
	case -1:
	case SEEK_SET:
	    sv = beg ;
	    break ;
	case SEEK_CUR:
	    sv = cur ;
	    break ;
	case SEEK_END:
	    sv = end ;
	    break ;
	default:
	    rs = SR_INVALID ;
	    break ;
	} /* end switch */
	if (rs >= 0) {
	    try {
	        if (freading) {
		    seekg(o,sv) ;	
		} else {
		    seekp(o,sv) ;
		}
		if (fail()) {
		    rs = SR_IO ;
		}
	    } catch (failure &e) {
		error_code	ec = e.code() ;
		cchar		*es = e.what() ;
		int		ev ;
		rs = SR_IO ;
		ev = ec.value() ;
		ulogerror("ccfile",ev,es) ;
	    } catch (...) {
		rs = SR_BADFMT ;
	    }
	} /* end if (ok) */
	return rs ;
}
/* end method (ccfile::seek) */


/* private subroutines */

int ccfile::iclose() noex {
	int		rs = SR_OK ;
	clear() ;
	fstream::close() ; /* superclass */
	if (fail()) {
	    rs = SR_IO ;
	}
	return rs ;
}
/* end method (ccfile::iclose) */

int ccfile::irewind() noex {
	return seek(0z) ;
}
/* end method (ccfile::irewind) */

void ccfile::dtor() noex {
	cint		rs = iclose() ;
	if (rs < 0) {
	    ulogerror("ccfile",rs,"dtor-close") ;
	}
}
/* end method (ccfile::dtor) */

int ccfile_co::operator () (int) noex {
	int		rs = SR_BUGCHECK ;
	if (op) {
	    switch (w) {
	    case ccfilemem_rewind:
		rs = op->irewind() ;
		break ;
	    case ccfilemem_close:
		rs = op->iclose() ;
		break ;
	    } /* end switch */
	} /* end if (non-null) */
	return rs ;
}
/* end method (ccfile_co::operator) */

static openmode getopenmode(cchar *sp) noex {
	openmode	om{} ;
	int		ch ;
	while ((ch = mkchar(*sp++))) {
	    switch (ch) {
	    case 'r': om |= ios::in ; break ; 
	    case 'w': om |= ios::out ; break ;
	    case 'a': om |= ios::app ; break ;
	    case 't': om |= ios::trunc ; break ;
	    case 'e': om |= ios::noreplace ; break ;
	    case 'b': om |= ios::binary ; break ;
	    case 'T': om |= ios::ate ; break ;
	   } /* end switch */
	   if (om == 0) {
		om |= ios::in ;
	   }
	} /* end while */
	return om ;
}
/* end subroutine (getopenmode) */


