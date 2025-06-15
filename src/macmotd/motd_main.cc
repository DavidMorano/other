/* motd_main SUPPORT (macmotd) */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* small Message-Of-The-Day (MOTD) server for macOS */
/* version %I% last-modified %G% */


/* revision history:

	= 1989-03-01, David A-D- Morano
	This subroutine was originally written.

*/

/* Copyright © 1989 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Name:
	main

	Description:
	This is a small Message-Of-The-Day (MOTD) server for macOS.

	Synopsis:
	$ macmotd [<file>]

	Arguments:
	<file>		optional MOTD file to write

	Returns:
	==0		for normal operation success
	!=0		some error

*******************************************************************************/

#include	<envstandards.h>	/* ordered first to configure */
#include	<sys/param.h>		/* |MAXPATHLEN| */
#include	<unistd.h>
#include	<fcntl.h>
#include	<ctime>
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<new>			/* |nothrow(3c++)| */
#include	<utility>		/* |unreachable(3c++)| */
#include	<fstream>
#include	<usystem.h>
#include	<sfx.h>
#include	<rmx.h>			/* |rmchr(3uc)| */
#include	<matstr.h>
#include	<mapex.h>
#include	<exitcodes.h>
#include	<localmisc.h>		/* system constants */


/* local defines */

#define	INTRUN		(30*60)		/* seconds */
#define	INTUPDATE	9		/* seconds */

#define	FN_MOTD		"/etc/motd"


/* imported namespaces */

using std::nullptr_t ;			/* type */
using libu::uloadavgd ;			/* subroutine (libu) */
using libu::ustrftime ;			/* subroutine (libu) */
using libu::snprintf ;			/* subroutine (libu) */
using libu::snloadavgd ;		/* subroutine (libu) */
using libu::snwcpy ;			/* subroutine (libu) */
using std::nothrow ;			/* constant */


/* local typedefs */


/* external subroutines */


/* external variables */


/* local structures */

namespace {
    enum proginfomems {
	proginfomem_start,
	proginfomem_finish,
	proginfomem_servbegin,
	proginfomem_servend,
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
	friend		proginfo_co ;
	proginfo_co	start ;
	proginfo_co	finish ;
	proginfo_co	servbegin ;
	proginfo_co	servend ;
	mainv		argv ;
	mainv		envv ;
	cchar		*pn = nullptr ;
	cchar		*fn = FN_MOTD ;
	char		*lbuf = nullptr ;
	int		argc ;
	int		pm = 0 ;
	int		llen = 0 ;
	proginfo(int c,mainv a,mainv e) noex : argc(c), argv(a), envv(e) { 
	    start	(this,proginfomem_start) ;
	    finish	(this,proginfomem_finish) ;
	    servbegin	(this,proginfomem_servbegin) ;
	    servend	(this,proginfomem_servend) ;
	} ;
	proginfo() noex : proginfo(0,nullptr,nullptr) { } ;
	void operator () (int c,mainv a,mainv e) noex {
	    argc = c ;
	    argv = a ;
	    envv = e ;
	} ;
	int process() noex ;
	int process_pmbegin() noex ;
	int process_pmend() noex ;
	int procfile(time_t) noex ;
	int procline(time_t) noex ;
	int procline_node(int) noex ;
	int procline_str(int,cchar *) noex ;
	int procline_date(int,const tm *) noex ;
	int procline_la(int) noex ;
	int procline_eol(int) noex ;
    private:
	int istart() noex ;
	int ifinish() noex ;
	int getpn(mainv) noex ;
	int iservbegin() noex ;
	int iservend() noex ;
    } ; /* end struct (proginfo) */
} /* end namespace */


/* forward references */


/* local variables */

enum progmodes {
	progmode_macmotd,
	progmode_overlast
} ;

static constexpr cpcchar	prognames[] = {
	[progmode_macmotd]	= "macmotd",
	[progmode_overlast]	= nullptr
} ;

static constexpr MAPEX	mapexs[] = {
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
} ; /* end array (mapex) */

constexpr int		maxlinelen = MAXLINELEN ;
constexpr int		nodenamelen = NODENAMELEN ;
constexpr int		nlas = 3 ;	/* max is fixed by convention */


/* exported variables */


/* exported subroutines */

int main(int argc,mainv argv,mainv envv) noex {
	int		ex = EX_OK ;
	int		rs ;
	int		rs1 ;
	if (proginfo pi(argc,argv,envv) ; (rs = pi.start) >= 0) {
	    if ((rs = pi.servbegin) >= 0) {
                switch (pi.pm) {
                case progmode_macmotd:
                    rs = pi.process() ;
                    break ;
		default:
		    rs = SR_BUGCHECK ;
		    break ;
                } /* end switch */
		rs1 = pi.servend ;
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
		cchar	*arg0 = argv[0] ;
	        cchar *bp ; 
		if (int bl ; (bl = sfbasename(arg0,-1,&bp)) > 0) {
		    if (int pl = rmchr(bp,bl,'.') ; pl > 0) {
	                if ((pm = matstr(names,bp,pl)) >= 0) {
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

int proginfo::iservbegin() noex {
	int		rs ;
	if ((rs = maxlinelen) >= 0) {
	    cint	maxline = rs ;
	    rs = SR_NOMEM ;
	    if ((lbuf = new(nothrow) char[maxline + 1]) != nullptr) {
		llen = maxline ;
		rs = SR_OK ;
	    }
	} /* end if (maxlinelen) */
	return rs ;
}
/* end method (proginfo::iservbegin) */

int proginfo::iservend() noex {
	int		rs = SR_OK ;
	if (lbuf) {
	    delete [] lbuf ;
	    lbuf = nullptr ;
	    llen = 0 ;
	}
	return rs ;
}
/* end method (proginfo::iservend) */

int proginfo::process() noex {
	custime		tistart = getustime ;
	int		rs ;
	int		rs1 ;
	int		c = 0 ;
	if ((rs = process_pmbegin()) >= 0) {
	    ustime	ticur = tistart ;
	    cauto lamb = [this,tistart,&ticur] () noex {
		int	rsl = SR_OK ;
	        if ((ticur - tistart) < INTRUN) {
	    	    rsl = procfile(ticur) ;
		}
		return rsl ;
	    } ;
	    while ((rs = lamb()) > 0) {
		sleep(INTUPDATE) ;
		ticur = getustime ;
	    } /* end while */
	    rs1 = process_pmend() ;
	    if (rs >= 0) rs = rs1 ;
	} /* end if (process_pm) */
	return (rs >= 0) ? c : rs ;
}
/* end subroutine (proginfo::process) */

int proginfo::procfile(time_t ti) noex {
	int		rs = SR_FAULT ;
	int		rs1 ;
	int		wlen = 0 ;
	if (fn) {
	    rs = SR_INVALID ;
	    if (fn[0]) {
		cint	of = O_WRONLY ;
		cmode	om = 0664 ;
		if ((rs = u_open(fn,of,om)) >= 0) {
	    	    cint	fd = rs ;
		    if ((rs = procline(ti)) >= 0) {
			if ((rs = u_write(fd,lbuf,rs)) >= 0) {
			    coff	fo = off_t(rs) ;
			    wlen = rs ;
			    rs = u_ftruncate(fd,fo) ;
			}
		    } /* end if (procline) */
	    	    rs1 = u_close(fd) ;
	    	    if (rs >= 0) rs = rs1 ;
		} /* end if (file) */
	    } /* end if (valid) */
	} /* end if (non-null) */
	return (rs >= 0) ? wlen : rs ;
}
/* end subroutine (proginfo::procfile) */

int proginfo::procline(time_t ti) noex {
	constexpr cchar	sep[] = " - " ;
	int		rs = SR_OK ;
	int		wl = 0 ;
	if (tm ts ; localtime_r(&ti,&ts) != nullptr) {
	    if ((rs = procline_node(wl)) >= 0) {
	        wl += rs ;
	        if ((rs = procline_str(wl,sep)) >= 0) {
	            wl += rs ;
	            if ((rs = procline_date(wl,&ts)) >= 0) {
	                wl += rs ;
	                if ((rs = procline_str(wl,sep)) >= 0) {
	                    wl += rs ;
	                    if ((rs = procline_la(wl)) >= 0) {
	                        wl += rs ;
	                        if ((rs = procline_eol(wl)) >= 0) {
	                            wl += rs ;
				}
			    }
			}
		    }
		}
	    }
	} /* end if (conversion ok) */
	return (rs >= 0) ? wl : rs ;
}
/* end subroutine (proginfo::procline) */

int proginfo::procline_node(int i) noex {
	cnullptr	np{} ;
	int		rs ;
	if ((rs = nodenamelen) >= 0) {
	    cint	nlen = rs ;
	    rs = SR_NOMEM ;
	    if (char *nbuf ; (nbuf = new(nothrow) char[nlen+1]) != np) {
	        if ((rs = u_getnodename(nbuf,nlen)) >= 0) {
		    cint	nl = rs ;
		    cint	bl = (llen - i) ;
		    char	*bp = (lbuf + i) ;
		    rs = snwcpy(bp,bl,nbuf,nl) ;
	        } /* end if (u_getnodename) */
		delete [] nbuf ;
	    } /* end if (m-a-f) */
	} /* end if (nodenamelen) */
	return rs ;
}
/* end subroutine (proginfo::procline_node) */

int proginfo::procline_str(int i,cchar *s) noex {
	cint		bl = (llen - i) ;
	char		*bp = (lbuf + i) ;
	return snwcpy(bp,bl,s) ;
}
/* end subroutine (proginfo::procline_str) */

int proginfo::procline_date(int i,const tm *tsp) noex {
	constexpr cchar	fmt[] = "%a %e %b %H:%M" ;
	cint		bl = (llen - i) ;
	char		*bp = (lbuf + i) ;
	return ustrftime(bp,bl,fmt,tsp) ;
}
/* end subroutine (proginfo::procline_date) */

int proginfo::procline_la(int i) noex {
	cint		bl = (llen - i) ;
	int		rs ;
	char		*bp = (lbuf + i) ;
	if (double dla[nlas] ; (rs = uloadavgd(dla,nlas)) >= 0) {
	    cint	prec = 1 ;
	    rs = snloadavgd(bp,bl,prec,dla,nlas) ;
	}
	return rs ;
}
/* end subroutine (proginfo::procline_la) */

int proginfo::procline_eol(int i) noex {
	cint		bl = (llen - i) ;
	int		rs = SR_OVERFLOW ;
	char		*bp = (lbuf + i) ;
	if (bl > 0) {
	    *bp++ = eol ;
	    *bp = '\0' ;
	    rs = 1 ;
	}
	return rs ;
}
/* end subroutine (proginfo::procline_date) */

int proginfo::process_pmbegin() noex {
	int		rs = SR_OK ;
	if ((argc > 1) && argv[1]) {
	    fn = argv[1] ;
	}
	return rs ;
}
/* end subroutine (proginfo::process_pmbegin) */

int proginfo::process_pmend() noex {
	return SR_OK ;
}
/* end subroutine (proginfo::process_pmend) */

int proginfo_co::operator () (int) noex {
	int		rs = SR_BUGCHECK ;
	if (op) {
	    switch (w) {
	    case proginfomem_start:
	        rs = op->istart() ;
	        break ;
	    case proginfomem_finish:
	        rs = op->ifinish() ;
	        break ;
	    case proginfomem_servbegin:
	        rs = op->iservbegin() ;
	        break ;
	    case proginfomem_servend:
	        rs = op->iservend() ;
	        break ;
	    } /* end switch */
	} /* end if (non-null) */
	return rs ;
}
/* end method (proginfo_co::operator) */


