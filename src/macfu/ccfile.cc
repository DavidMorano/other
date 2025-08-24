/* ccfile SUPPORT */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* C++ file I-O operations */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-04-10, David A­D­ Morano
        This code was originally written.

	= 2023-12-28, David A­D­ Morano
	This is (substantially) refactored.  I went from using
	|std::getline(3c++)| to |std::iostream::getline(3c++)|.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

/******************************************************************************* 
	object:
	ccfile

	Description:
	This object tries to mimic the old normal 'read' function
	from the old days.  The "modern" |istream::getline(3c++)|
	and |std::getline(3c++)| want to be in the middle of a
	sequence of input operations (like reading various typed
	fields from the input file).  So they return something like
	the type |istream &| so that they can be chained together.
	But I want to have the older style semantic where either
	the number of characters read is returned or an |errno(2)|
	error code is returned

	Synopsis:
	int readln(char *ibuf,int ilen,int chdelim = eol) noex

	Arguments:
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
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<string>
#include	<string_view>
#include	<fstream>
#include	<usystem.h>
#include	<snwcpy.h>
#include	<strnul.hh>
#include	<stdfnames.h>
#include	<matstr.h>
#include	<mkchar.h>
#include	<localmisc.h>

#include	"ccfile.hh"


/* local defines */

#ifndef	MAXLINE
#define	MAXLINE		(4*1024)
#endif


/* imported namespaces */

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

typedef string_view		strview ;
typedef	ios_base::openmode	omode ;


/* external subroutines */


/* external variables */


/* local structures */

namespace {
    struct devnames {
	cchar	*name[stdfile_overlast] ;
	constexpr devnames() noex {
	    name[stdfile_in]	= "/dev/fd/0" ;
	    name[stdfile_out]	= "/dev/fd/1" ;
	    name[stdfile_err]	= "/dev/fd/2" ;
	    name[stdfile_log]	= "/dev/fd/3" ;
	} ; /* end ctor */
    } ; /* end struct (devnames) */
} /* end namespace */


/* forward references */

static ios_base::openmode getopenmode(cchar *) noex ;

static bool	isnomode(omode) noex ;


/* local variables */

static const devnames	dev ;


/* exported variables */


/* exported subroutines */

int ccfile::open(cchar *fn,cchar *ofs,mode_t) noex {
	int		rs = SR_FAULT ;
	if (fn) ylikely {
	    rs = SR_INVALID ;
	    if (fn[0]) ylikely {
		openmode	of{} ;
		rs = SR_OK ;
		if (ofs) {
		    rs = SR_INVALID ;
		    if (ofs[0]) {
			rs = SR_OK ;
			of = getopenmode(ofs) ;
		    }
		} /* end if (had open-flags) */
		if (rs >= 0) ylikely {
		    mainv	sfn = stdfnames ;
		    if (int fni ; (fni = matstr(sfn,fn,-1)) >= 0) ylikely {
			fn = dev.name[fni] ;
			switch (fni) {
			case stdfile_in:
			    if (isnomode(of)) of |= ios::in ;
			    break ;
			case stdfile_out:
			case stdfile_err:
			case stdfile_log:
			    if (isnomode(of)) of |= ios::out ;
			    break ;
			case stdfile_null:
			    fl.fnulling = true ;
			    break ;
			} /* end switch */
		    } /* end if (match on special names) */
		    if (! fl.fnulling) {
		        if (isnomode(of)) of |= ios::in ;
			fstream::open(fn,of) ;
		        rs = SR_NOENT ;
		        if (good()) {
			    if (of & ios::in) fl.freading = true ;
			    fl.fopened = true ;
		            rs = SR_OK ;
		        }
		    } /*& end if (not fnulling) */
		} /* end if (ok) */
	    } /* end if (valid) */
	} /* end if (non-null) */
	return rs ;
}
/* end method (ccfile::open) */

int ccfile::open(const strview &sv,cchar *ofs,mode_t om) noex {
	csize		svz = sv.length() ;
	cchar		*svp = sv.data() ;
	int		rs = SR_FAULT ;
	if (svp && ofs) ylikely {
	    cint	svl = int(svz) ;
	    {
	        strnul	fn(svp,svl) ;
		rs = open(fn,ofs,om) ;
	    } /* end block */
	} /* end if (non-null) */
	return rs ;
}
/* end method (ccfile::open) */

int ccfile::readln(char *ibuf,int ilen,int dch) noex {
	int		rs = SR_FAULT ;
	int		len = 0 ; /* return-value */
	if (dch == 0) dch = eol ;
	if (ibuf) ylikely {
	    ibuf[0] = '\0' ;
	    rs = SR_OK ;
	    if (! fl.fnulling) ylikely {
	        try {
		    rs = SR_BADFMT ;
	            if (bool(getline(ibuf,(ilen+1),char(dch)))) ylikely {
			cint gcnt = int(gcount()) ;
		        if ((rs = gcnt) <= ilen) {
			    len = rs ;
			    if (len > 0) {
			        ibuf[len-1] = char(dch) ;
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
	    } /* end if (not-fnulling) */
	} /* end if (non-null) */
	return (rs >= 0) ? len : rs ;
}
/* end method (ccfile::readln) */

int ccfile::readln(string &s,int dch) noex {
    	cnothrow	nt{} ;
    	cnullptr	np{} ;
	int		rs = SR_OK ;
	int		len = 0 ; /* return-value */
	if (! fl.fnulling) ylikely {
	    cint	llen = MAXLINE ;
	    rs = SR_NOMEM ;
	    if (char *lbuf ; (lbuf = new(nt) char[llen+1]) != np) ylikely {
	        if ((rs = readln(lbuf,llen,char(dch))) >= 0) ylikely {
		    len = rs ;
		    lbuf[rs] = '\0' ;
		    try {
		        s = lbuf ;
		    } catch (...) {
		        rs = SR_NOMEM ;
		    }
	        } /* end if (readln) */
	        delete [] lbuf ;
	    } /* end if (m-a-f) */
	} /* end if (not-fnulling) */
	return (rs >= 0) ? len : rs ;
}
/* end method (ccfile::readln) */

static inline bool iscont(cchar *lp,int li) noex {
    	bool	f = true ;
	f = f && (li >= 2) ;
	f = f && (lp[li - 1] == '\n') ;
	f = f && (lp[li - 2] == '\\') ;
	return f ;
} /* end subroutine (iscont) */

int ccfile::readlns(char *lbuf,int llen,int to,int *lcp) noex {
	int		rs = SR_FAULT ;
	int		i = 0 ; /* return-value */
	if (lbuf) {
	    rs = SR_OK ;
	    lbuf[0] = '\0' ;
	    if (! fl.fnulling) ylikely {
	        int	lines = 0 ;
	        bool	fcont = false ;
	        while ((lines == 0) || ((fcont = iscont(lbuf,i)))) {
	            if (fcont) i -= 2 ;
	            rs = readln((lbuf + i),(llen - i),to) ;
	            if (rs <= 0) break ;
	            i += rs ;
	            lines += 1 ;
	        } /* end while */
	        if (lcp) *lcp = lines ;
	    } /* end if (not-fnulling) */
	} /* end if (non-null) */
	return (rs >= 0) ? i : rs ;
} /* end method (ccfile::readlns) */

int ccfile::seek(off_t o,int w) noex {
	int		rs = SR_OK ;
	if (! fl.fnulling) ylikely {
	    seekdir	sv = beg ;
	    if (w < 0) w = SEEK_SET ;
	    switch (w) {
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
	    if (rs >= 0) ylikely {
	        try {
	            if (fl.freading) {
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
	} /* end if (not-fnulling) */
	return rs ;
}
/* end method (ccfile::seek) */


/* private subroutines */

int ccfile::iclose() noex {
	int		rs = SR_OK ;
	if ((! fl.fnulling) && fl.fopened) {
	    clear() ;
	    fstream::close() ; /* superclass */
	    if (fail()) {
	        rs = SR_IO ;
	    }
	    fl.fopened = false ;
	} /* end if (not-fnulling) */
	return rs ;
}
/* end method (ccfile::iclose) */

int ccfile::irewind() noex {
	int		rs = SR_OK ;
	if (! fl.fnulling) {
	    rs = seek(0z) ;
	}
	return rs ;
}
/* end method (ccfile::irewind) */

void ccfile::dtor() noex {
	if (int rs ; (rs = iclose()) < 0) {
	    ulogerror("ccfile",rs,"dtor-close") ;
	}
}
/* end method (ccfile::dtor) */

int ccfile_co::operator () (int) noex {
	int		rs = SR_BUGCHECK ;
	if (op) ylikely {
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
	while ((ch = mkchar(*sp++)) > 0) {
	    switch (ch) {
	    case 'r': om |= ios::in ; break ; 		/* input */
	    case 'w': om |= ios::out ; break ;		/* output */
	    case 'a': om |= ios::app ; break ;		/* append */
	    case 't': om |= ios::trunc ; break ;	/* truncate */
	    case 'e': om |= ios::noreplace ; break ;	/* exclusive-open */
	    case 'b': om |= ios::binary ; break ;	/* binary-file */
	    case 'T': om |= ios::ate ; break ;		/* seek-end at open */
	   } /* end switch */
	   if (om == 0) {
		om |= ios::in ;
	   }
	} /* end while */
	return om ;
}
/* end subroutine (getopenmode) */

static bool isnomode(omode of) noex {
	return ((! (of & ios::in)) && (! (of & ios::out))) ;
} /* end subroutine (isnomode) */


