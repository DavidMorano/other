/* main SUPPORT (dups) */
/* lang=C++20 */

/* print duplicate (by name) files */
/* version %I% last-modified %G% */


/* revision history:

	= 1989-03-01, David A.D. Morano
	This subroutine was originally written.

*/

/* Copyright © 1989 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Names:
	dups

	Description:
	This program does something w/ duplicates (by names).

	Synopsis:
	$ dups [<file(s)>] [-]

	Arguments:
	<file(s)>	file(s) to process
	-		read standard-input for file(s) to process

	Returns:
	0		for normal operation success
	126		haveprogram: some program found but not executable
	127		haveprogram: program not found

*******************************************************************************/

#include	<envstandards.h>	/* ordered first to configure */
#include	<sys/param.h>
#include	<unistd.h>
#include	<fcntl.h>
#include	<climits>
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<cstring>		/* <- for |strlen(3c)| */
#include	<utility>		/* |pair(3c++)| */
#include	<string>
#include	<string_view>
#include	<vector>
#include	<iostream>
#include	<usystem.h>
#include	<varnames.hh>
#include	<strn.h>
#include	<sfx.h>
#include	<matstr.h>
#include	<strwcpy.h>
#include	<strnul.hh>
#include	<sncpyx.h>
#include	<getourenv.h>
#include	<mapblock.hh>
#include	<readln.hh>
#include	<rmx.h>
#include	<isnot.h>
#include	<mapex.h>
#include	<exitcodes.h>
#include	<localmisc.h>


/* local defines */

#ifndef	MAXPATH
#define	MAXPATH		4096
#endif


/* imported namespaces */

using std::nullptr_t ;			/* type */
using std::string ;			/* type */
using std::string_view ;		/* type */
using std::unordered_map ;		/* type */
using std::vector ;			/* type */
using std::istream ;			/* type */
using std::pair ;			/* type */
using std::cin;				/* variable */
using std::cout ;			/* variable */
using std::cerr ;			/* variable */
using std::nothrow ;			/* constant */


/* local typedefs */

typedef string_view	strview ;

typedef const strview	cstrview ;
typedef const string	cstring ;


/* external subroutines */


/* external variables */


/* local structures */

namespace {
    struct devino {
	dev_t		dev ;
	ino_t		ino ;
	devino(dev_t d,ino_t i) noex : dev(d), ino(i) { } ;
	size_t hashval() const noex {
	    return size_t(dev | ino) ;
	} ;
	bool equal_to(const devino &o) noex {
	    return ((dev == o.dev) && (ino == o.ino)) ;
	} ;
    } ; /* end struct (devino) */
    struct filenode {
	string		fn ;		/* "file-name" */
	size_t		fsize ;		/* "file-size" */
	int		rc = 0 ;	/* success==1 */
	filenode() noex = default ;
	filenode(size_t sz,cchar *pp,int pl = -1) noex {
	    if (pl < 0) pl = strlen(pp) ;
	    fsize = sz ;
	    {
	        strview	s(pp,pl) ;
	        try {
	            fn = s ;
		    rc = 1 ;
	        } catch (...) {
		    rc = 0 ;
	        }
	    } /* end block */
	} ; /* end if (ctor) */
    } ; /* end struct (filenode) */
    enum proginfomems {
	proginfomem_start,
	proginfomem_finish,
	proginfomem_flistbegin,
	proginfomem_flistend,
	proginfomem_overlast
    } ;
    struct proginfo ;
    struct proginfo_co {
	proginfo	*op = nullptr ;
	int		w = -1 ;
	void operator () (proginfo *p,int m) noex {
	    op = p ;
	    w = m ;
	} ;
	int operator () (int = 0) noex ;
	operator int () noex {
	    return operator () (0) ;
	} ;
    } ; /* end struct (proginfo_co) */
    struct proginfo {
	typedef mapblock<devino,filenode>		nodedb ;
	typedef mapblock<devino,filenode>::iterator	flit_t ;
	friend proginfo_co ;
	proginfo_co	start ;
	proginfo_co	finish ;
	proginfo_co	flistbegin ;
	proginfo_co	flistend ;
	mainv		argv ;
	mainv		envv ;
	cchar		*pn = nullptr ;
	nodedb		flist ;
	int		argc ;
	int		pm = 0 ;
	proginfo(int c,mainv a,mainv e) noex : argc(c), argv(a), envv(e) { 
	    start(this,proginfomem_start) ;
	    finish(this,proginfomem_finish) ;
	    flistbegin(this,proginfomem_flistbegin) ;
	    flistend(this,proginfomem_flistend) ;
	} ;
	proginfo() noex : proginfo(0,nullptr,nullptr) { } ;
	void operator () (int c,mainv a,mainv e) noex {
	    argc = c ;
	    argv = a ;
	    envv = e ;
	} ;
	int filecandidate(cchar *,int = -1) noex ;
	int filealready(dev_t,ino_t) noex ;
	int output() noex ;
    private:
	int istart() noex ;
	int ifinish() noex ;
	int getpn(mainv) noex ;
	int iflistbegin() noex ;
	int iflistend() noex ;
	int readin() noex ;
    } ; /* end struct (proginfo) */
}

namespace std {
    template<> struct hash<devino> {
	size_t operator() (const devino &di) const noex {
	    return di.hashval() ;
	} ;
    } ; /* end struct-template (hash<devino>) */
    template<> struct equal_to<devino> {
	size_t operator() (const devino &lhs,const devino &rhs) const noex {
	    return ((lhs.dev == rhs.dev) && (lhs.ino == rhs.ino)) ;
	} ;
    } ; /* end struct-template (hash<devino>) */
}


/* forward references */

typedef mapblock<devino,filenode>::iterator	nodeit_t ;
typedef pair<devino,filenode>			elem_t ;

static bool isnodesame(csize,nodeit_t &) noex ;


/* local variables */

enum progmodes {
	progmode_dup,
	progmode_overlast
} ;

constexpr cpcchar	prognames[] = {
	"dup",
	nullptr
} ;

constexpr MAPEX		mapexs[] = {
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
	{ SR_NOMSG,	EX_OSERR },
	{ SR_NOSYS,	EX_OSFILE },
	{ 0, 0 }
} ;

constexpr int		maxpathlen = MAXPATH ;


/* exported variables */


/* exported subroutines */

int main(int argc,mainv argv,mainv envv) noex {
	proginfo	pi(argc,argv,envv) ;
	int		ex = EX_OK ;
	int		rs ;
	int		rs1 ;
	if ((rs = pi.start) >= 0) {
	    if ((rs = pi.flistbegin) >= 0) {
                switch (pi.pm) {
                case progmode_dup:
                    rs = pi.output() ;
                    break ;
		default:
		    rs = SR_BUGCHECK ;
		    break ;
                } /* end switch */
		rs1 = pi.flistend ;
		if (rs >= 0) rs = rs1 ;
	    } /* end if (flist-) */
	    rs1 = pi.finish ;
	    if (rs >= 0) rs = rs1 ;
	} /* end if (proginfo) */
	if ((rs < 0) && (ex == EX_OK)) {
	    ex = mapex(mapexs,rs) ;
	}
	return ex ;
}
/* end subroutine (main) */


/* local subroutines */

int proginfo::istart() noex {
	int		rs ;
	if ((rs = getpn(prognames)) >= 0) {
	    rs = 0 ;
	} /* end if (proginfo::getpn) */
	return rs ;
}
/* end method (proginfo::istart) */

int proginfo::ifinish() noex {
	return SR_OK ;
}
/* end method (proginfo::ifinish) */

int proginfo::getpn(mainv names) noex {
	int		rs = SR_FAULT ;
	if (argv) {
	    rs = SR_NOMSG ;
	    if ((argc > 0) && argv[0]) {
	        int	bl ;
		cchar	*arg0 = argv[0] ;
	        cchar	*bp{} ;
	        if ((bl = sfbasename(arg0,-1,&bp)) > 0) {
		    int		pl = rmchr(bp,bl,'.') ;
		    cchar	*pp = bp ;
		    if (pl > 0) {
	                if ((pm = matstr(names,pp,pl)) >= 0) {
			    pn = names[pm] ;
		            rs = pm ;
	                }
		    } /* end if (non-zero positive progname) */
		} /* end if (have base-name) */
	    } /* end if (have first argument) */
	} /* end if (non-null) */
	return rs ;
}
/* end method (proginfo::getpn) */

int proginfo::iflistbegin() noex {
	int		rs ;
	if ((rs = flist.start) >= 0) {
	    if (argc > 1) {
	        for (int ai = 1 ; (rs >= 0) && (ai < argc) ; ai += 1) {
		    cchar	*fn = argv[ai] ;
		    if (fn[0]) {
		        if ((fn[0] == '-') && (fn[1] == '\0')) {
			    rs = readin() ;
		        } else {
			    rs = filecandidate(fn) ;
		        }
		    } /* end if */
	        } /* end for */
	    } else {
	        rs = readin() ;
	    }
	    if (rs < 0) {
		flist.finish() ;
	    }
	} /* end if (mapblock::start) */
	return rs ;
}
/* end method (proginfo::iflistbegin) */

int proginfo::iflistend() noex {
	return flist.finish ;
}
/* end method (proginfo::iflistend) */

int proginfo::readin() noex {
	istream		*isp = &cin ;
	int		rs ;
	if ((rs = maxpathlen) >= 0) {
	    cint	plen = rs ;
	    char	*pbuf ;
	    rs = SR_NOMEM ;
	    if ((pbuf = new(nothrow) char[plen+1]) != nullptr) {
	        while ((rs = readln(isp,pbuf,plen)) > 0) {
		    int		pl = rs ;
		    if ((pl > 0) && (pbuf[pl - 1] == eol)) pl -= 1 ;
		    if (pl > 0) {
		        rs = filecandidate(pbuf,pl) ;
		    }
		    if (rs < 0) break ;
	        } /* end if (reading lines) */
	        delete [] pbuf ;
	    } /* end if (m-a-f) */
	} /* end if (maxpathlen) */
	return rs ;
}
/* end method (proginfo::readin) */

int proginfo::filecandidate(cchar *sp,int sl) noex {
	USTAT		sb ;
	strnul		s(sp,sl) ;
	int		rs ;
	if ((rs = u_stat(s,&sb)) >= 0) {
	    csize	fsz = size_t(sb.st_size) ;
	    if (S_ISREG(sb.st_mode)) {
	        const dev_t	d = sb.st_dev ;
	        const ino_t	i = sb.st_ino ;
	        if ((rs = filealready(d,i)) == 0) {
		    devino	di(d,i) ;
		    filenode	e(fsz,sp,sl) ;
		    rs = flist.ins(di,e) ;
	        } /* end if (not-already) */
	    } /* end if (is-dir) */
	} else if (isNotAccess(rs)) {
	    rs = SR_OK ;
	}
	return rs ;
}
/* end method (proginfo::filecandidate) */

int proginfo::filealready(dev_t d,ino_t i) noex {
	cint		rsn = SR_NOTFOUND ;
	int		rs ;
	int		f = true ;
	devino		di(d,i) ;
	if ((rs = flist.present(di)) == rsn) {
	    rs = SR_OK ;
	    f = false ;
	} /* end if (mapblock::present) */
	return (rs >= 0) ? f : rs ;
}
/* end method (proginfo::filealready) */

int proginfo::output() noex {
	int		rs = SR_OK ;
	flit_t		ite = flist.end() ;
	for (flit_t it = flist.begin() ; it != ite ; ++it) {
	    const pair<devino,filenode>	&e = *it ;
	    const filenode	&f = e.second ;
	    try {
		cint		rc = f.rc ;
		if (rc > 0) {
		    csize	fsz = f.fsize ;
		    int		c = 0 ;
		    for (nodeit_t itt = it ; itt != ite ; ++itt) {
			if (isnodesame(fsz,itt) && (itt != it)) {
			    const elem_t	&oe = *itt ;
			    if (c++ == 0) {
		                cstring	*n = &f.fn ;
	                        cout << *n << " " << f.fsize << eol ;
			    }
			    {
				const filenode	&of = oe.second ;
				cout << of.fn << " " << of.fsize << eol ;
			    }
			}
		    } /* end while */
		} /* end if */
	    } catch (...) {
		rs = SR_IO ;
	    }
	    if (rs < 0) break ;
	} /* end for */
	return rs ;
}
/* end subroutine (proginfo::output) */

static bool isnodesame(csize fsz,nodeit_t &itt) noex {
	const pair<devino,filenode>	&e = *itt ;
	bool		f = false ;
	{
	    const filenode	&fm = e.second ;
	    f = (fsz == fm.fsize) ;
	}
	return f ;
}

int proginfo_co::operator () (int) noex {
	int	rs = SR_BUGCHECK ;
	if (op) {
	    switch (w) {
	    case proginfomem_start:
	        rs = op->istart() ;
	        break ;
	    case proginfomem_finish:
	        rs = op->ifinish() ;
	        break ;
	    case proginfomem_flistbegin:
	        rs = op->iflistbegin() ;
	        break ;
	    case proginfomem_flistend:
	        rs = op->iflistend() ;
	        break ;
	    } /* end switch */
	} /* end if (non-null) */
	return rs ;
}
/* end method (proginfo_co::operator) */


