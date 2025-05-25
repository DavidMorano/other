/* main SUPPORT (haveprogram) */
/* encoding=ISO8859-1 */
/* lang=C++20 */

/* determine if a given name is an executable program */
/* version %I% last-modified %G% */


/* revision history:

	= 1989-03-01, David A.D. Morano
	This code was originally written.

*/

/* Copyright © 1989 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Names:
	haveprogram
	havefunction
	fpathto
	pathto
	pathenum

	Description:
	Print out the home directory for the given user.

	Synopsis:
	$ haveprogram
	$ havefunction
	$ fpathto
	$ pathto
	$ pathenum

	Returns:
	0		for normal operation success
	126		haveprogram: some program found but not executable
	127		haveprogram: program not found

	Implementation-note:
	The whole purpose of this program (in all three forms) is
	to be able to be built without using any of the more elaborate
	system services that are normally provided by the libraries
	|libdam|, |libuc|, and |libu|.  Several minimalistic services
	are required from |libuc| and at least one (|u_stat(3u)|)
	from |libu|.

*******************************************************************************/

#include	<envstandards.h>	/* MUST be ordered first to configure */
#include	<sys/param.h>
#include	<unistd.h>
#include	<fcntl.h>
#include	<climits>
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<cstring>		/* <- for |strlen(3c)| + |strcmp(3c)| */
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
#include	<varnames.hh>
#include	<strnul.hh>
#include	<sncpyx.h>
#include	<rmx.h>
#include	<isnot.h>
#include	<mapex.h>
#include	<getourenv.h>
#include	<exitcodes.h>
#include	<localmisc.h>		/* |NAXPATHLEN| */


/* local defines */

#ifndef	MAXPATH
#define	MAXPATH		4096
#endif


/* imported namespaces */

using std::nullptr_t ;			/* type */
using std::string ;			/* type */
using std::string_view ;		/* type */
using std::vector ;			/* type */
using std::cout ;			/* variable */
using std::nothrow ;			/* constant */


/* local typedefs */

typedef string_view	strview ;


/* external subroutines */

extern "C" {
    extern int u_stat(cchar *,USTAT *) noex ;
}


/* external variables */


/* local structures */

namespace {
    struct pathent {
	string		ps ;
	dev_t		dev ;
	ino_t		ino ;
	pathent() noex = default ;
	pathent(dev_t d,ino_t i,cchar *pp,int pl = -1) noex {
	    if (pl < 0) pl = strlen(pp) ;
	    strview	s(pp,pl) ;
	    try {
	        ps = s ;
	        dev = d ;
	        ino = i ;
	    } catch (...) {
		dev = 0 ;
		ino = 0 ;
	    }
	} ; /* end if (ctor) */
    } ; /* end struct (pathent) */
    enum proginfomems {
	proginfomem_start,
	proginfomem_finish,
	proginfomem_pathend,
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
    struct proginfo_pabeg {
	proginfo	*op = nullptr ;
	void init(proginfo *p) noex {
	    op = p ;
	} ;
	int operator () (cchar * = nullptr) noex ;
	operator int () noex {
	    return operator () (nullptr) ;
	} ;
    } ; /* end struct (proginfo_pabeg) */
    struct proginfo {
	friend proginfo_co ;
	friend proginfo_pabeg ;
	proginfo_co	start ;
	proginfo_co	finish ;
	proginfo_co	pathend ;
	proginfo_pabeg	pathbegin ;
	mainv		argv ;
	mainv		envv ;
	cchar		*pn = nullptr ;
	vector<pathent>	pl ;
	int		argc ;
	int		pm ;
	bool		ffound = false ;
	proginfo(int c,mainv a,mainv e) noex : argc(c), argv(a), envv(e) { 
	    start(this,proginfomem_start) ;
	    finish(this,proginfomem_finish) ;
	    pathend(this,proginfomem_pathend) ;
	    pathbegin.init(this) ;
	} ;
	proginfo() noex : proginfo(0,nullptr,nullptr) { } ;
	void operator () (int c,mainv a,mainv e) noex {
	    argc = c ;
	    argv = a ;
	    envv = e ;
	} ;
	int pathcandidate(cchar *,int) noex ;
	int pathalready(dev_t,ino_t) noex ;
	int pathenum() noex ;
	int pathtox() noex ;
	int pathto(cchar * = nullptr) noex ;
	int getpn(mainv) noex ;
	int getvn(cchar **) noex ;
    private:
	int istart() noexcept ;
	int ifinish() noexcept ;
	int ipathbegin(cchar *) noexcept ;
	int ipathend() noexcept ;
	int pathenumone(cchar *) noex ;
    } ; /* end struct (proginfo) */
}


/* forward references */


/* local variables */

enum progmodes {
	progmode_pathenum,
	progmode_pe,
	progmode_pathto,
	progmode_pt,
	progmode_fpathto,
	progmode_manto,
	progmode_haveprogram,
	progmode_havefunction,
	progmode_overlast
} ;

static constexpr cpcchar	prognames[] = {
	"pathenum",
	"pe",
	"pathto",
	"pt",
	"fpathto",
	"manto",
	"haveprogram",
	"havefunction",
	nullptr
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
} ;

constexpr int		maxpathlen = MAXPATH ;


/* exported variables */


/* exported subroutines */

int main(int argc,mainv argv,mainv envv) noexcept {
	proginfo	pi(argc,argv,envv) ;
	int		ex = EX_OK ;
	int		rs ;
	int		rs1 ;
	if ((rs = pi.start) >= 0) {
            switch (pi.pm) {
            case progmode_pathenum:
            case progmode_pe:
                rs = pi.pathenum() ;
                break ;
            case progmode_pt:
                pi.pm = progmode_pathto ;
		fallthrough ;
                /* FALLTHROUGH */
            case progmode_pathto:
            case progmode_fpathto:
                rs = pi.pathto() ;
                break ;
            case progmode_haveprogram:
            case progmode_havefunction:
                if ((rs = pi.pathtox()) == 0) {
                    ex = (pi.ffound) ? EX_NOEXEC : EX_NOPROG ;
                } else if (rs < 0) {
                    switch (rs) {
                    case SR_NOTFOUND:
                        ex = EX_NOPROG ;
                        break ;
                    case SR_ACCESS:
                        ex = EX_NOEXEC ;
                        break ;
                    } /* end switch */
                } /* end if (pathto) */
                break ;
            } /* end switch */
	    rs1 = pi.finish ;
	    if (rs >= 0) rs = rs1 ;
	} /* end if (proginfo) */
	if ((rs < 0) && (ex == EX_OK)) ex = mapex(mapexs,rs) ;
	return ex ;
}
/* end subroutine (main) */


/* local subroutines */

int proginfo::istart() noexcept {
	int		rs = SR_FAULT ;
	if (argv) {
	    rs = getpn(prognames) ;
	} /* end if (non-null) */
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
	        cchar	*bp ;
	        if (int bl ; (bl = sfbasename(argv[0],-1,&bp)) > 0) {
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

int proginfo::ipathbegin(cchar *vn) noex {
	int		rs = SR_FAULT ;
	if (vn) {
	    rs = SR_INVALID ;
	    if (vn[0]) {
	        cchar	*valp ;
		rs = SR_OK ;
	        if ((valp = getourenv(envv,vn)) != nullptr) {
	            cchar	*sp = valp ;
	            char	*tp ;
	            int		sl = strlen(sp) ;
	            while ((tp = strnchr(sp,sl,':')) != nullptr) {
	 	        rs = pathcandidate(sp,(tp-sp)) ;
		        sl -= ((tp+1) - sp) ;
		        sp = (tp+1) ;
		        if (rs < 0) break ;
	            } /* end while */
	            if ((rs >= 0) && (sl > 0)) {
	 	        rs = pathcandidate(sp,sl) ;
	            }
	        } /* end if (got value) */
	    } /* end if (valid) */
	} /* end if (non-null) */
	return rs ;
}
/* end method (proginfo::ipathbegin) */

int proginfo::pathcandidate(cchar *sp,int sl) noex {
	strnul		s(sp,sl) ;
	int		rs ;
	cint		fl = rmtrailchr(sp,sl,'/') ;
	cchar		*fp = sp ;
	if (USTAT sb ; (rs = u_stat(s,&sb)) >= 0) {
	    if (S_ISDIR(sb.st_mode)) {
	        const dev_t	d = sb.st_dev ;
	        const ino_t	i = sb.st_ino ;
	        if ((rs = pathalready(d,i)) == 0) {
		    pathent	e(d,i,fp,fl) ;
		    try {
			if ((e.dev != 0) && (e.ino != 0)) {
		            pl.push_back(e) ;
			} else {
			    rs = SR_NOMEM ;
			}
		    } catch (...) {
			rs = SR_NOMEM ;
		    }
	        } /* end if (not-already) */
	    } /* end if (is-dir) */
	} else if (isNotAccess(rs)) {
	    rs = SR_OK ;
	}
	return rs ;
}
/* end method (proginfo::pathcandidate) */

int proginfo::ipathend() noex {
	pl.clear() ;
	return SR_OK ;
}
/* end method (proginfo::ipathend) */

int proginfo::pathalready(dev_t d,ino_t i) noex {
	int		rs = SR_OK ;
	bool		f = false ;
	for (cauto &e : pl) {
	    f = ((e.dev == d) && (e.ino == i)) ;
	    if (f) break ;
	} /* end for */
	return (rs >= 0) ? f : rs ;
}
/* end method (proginfo::pathalready) */

int proginfo::pathenum() noex {
	int		rs ;
	if (argc > 1) {
	    rs = SR_OK ;
	    for (int ai = 1 ; (rs >= 0) && (ai < argc) && argv[ai] ; ai += 1) {
	        cchar	*vn = argv[ai] ;
		if (vn[0]) {
	            rs = pathenumone(vn) ;
		}
	    } /* end for (looping over program arguments) */
	} else {
	    cchar	*vn = varname.path ;
	    rs = pathenumone(vn) ;
	} /* end if (program arguments) */
	return rs ;
}
/* end subroutine (proginfo::pathenum) */

int proginfo::pathenumone(cchar *vn) noex {
	cchar		*manpath = varname.manpath ;
	int		rs ;
	int		rs1 ;
	if (strcmp(vn,manpath) == 0) {
	    cchar	*vv = getourenv(envv,vn) ;
	    if ((vv == nullptr) || (vv[0] == '\0')) {
		vn = varname.manxpath ;
	    }
	} /* end if (manpath match) */
	if ((rs = pathbegin(vn)) >= 0) {
	    for (cauto &e : pl) {
		cout << e.ps << eol ;
	    } /* end for */
	    rs1 = pathend ;
	    if (rs >= 0) rs = rs1 ;
	} /* end if (proginfo::path) */
	return rs ;
}
/* end subroutine (proginfo::pathenumone) */

int proginfo::pathtox() noex {
	int		rs ;
	cchar		*vn{} ;
	if ((rs = getvn(&vn)) >= 0) {
	    rs = pathto(vn) ;
	} /* end if (getvn) */
	return rs ;
}
/* end subroutine (proginfo::pathtox) */

int proginfo::pathto(cchar *vn) noex {
	cint		pm_pt = progmode_pathto ;
	cint		pm_fp = progmode_fpathto ;
	cint		pm_hp = progmode_haveprogram ;
	cint		pm_hf = progmode_havefunction ;
	int		rs ;
	int		rs1 ;
	int		ctotal = 0 ;
	if ((vn == nullptr) && (pm == pm_fp)) {
	    vn = varname.fpath ;
	}
	if ((rs = pathbegin(vn)) >= 0) {
	    if ((rs = maxpathlen) >= 0) {
	        cint	plen = rs ;
	        char	*pbuf ;
	        rs = SR_NOMEM ;
	        if ((pbuf = new(nothrow) char[plen+1]) != nullptr) {
		    rs = SR_OK ;
	            for (int ai = 1 ; (ai < argc) && argv[ai] ; ai += 1) {
	                cchar	*ap = argv[ai] ;
		        int	c = 0 ;
	                if (ap[0]) {
			    ffound = false ;
			    bool	f = false ;
	                    for (cauto &e : pl) {
			        cchar	*pp = e.ps.c_str() ; /* <- throw? */
			        if ((rs = sncpy(pbuf,plen,pp,"/",ap)) >= 0) {
				    USTAT	sb ;
			 	    if ((rs = u_stat(pbuf,&sb)) >= 0) {
				        cmode	fm = sb.st_mode ;
				        ffound = true ;
				        if (S_ISREG(fm)) {
					    mode_t	xxx = 0 ;
					    f = f || (pm == pm_pt) ;
					    f = f || (pm == pm_fp) ;
					    switch (pm) {
					    case progmode_pathto:
					    case progmode_haveprogram:
						xxx = 0111 ;
						break ;
					    case progmode_fpathto:
					    case progmode_havefunction:
						xxx = 0440 ;
						break ;
					    } /* end switch */
					    if ((fm & xxx) == xxx) {
					        c += 1 ;
					        if (f) {
	                                            cout << pbuf << eol ;
					        }
					    } /* end if is-exec) */
				        } /* end if (is-reg) */
				    } else if (isNotAccess(rs)) {
				        rs = SR_OK ;
				    } /* end if (u_stat) */
			        } /* end if (sncpyx) */
			        if ((pm == pm_hp) && (c > 0)) break ;
			        if (rs < 0) break ;
	                    } /* end for (paths) */
			    f = ((pm == pm_hp) || (pm == pm_hf)) ;
			    if ((rs >= 0) && f && (c == 0)) {
			        rs = (ffound) ? SR_ACCESS : SR_NOTFOUND ;
			    } /* end if */
	                } /* end if (non-empty) */
		        ctotal += c ;
		        if (rs < 0) break ;
	            } /* end for (args) */
	            delete [] pbuf ;
	        } /* end if (m-a-f) */
	    } /* end if (maxpathlen) */
	    rs1 = pathend ;
	    if (rs >= 0) rs = rs1 ;
	} /* end if (proginfo::path) */
	return (rs >= 0) ? ctotal : rs ;
}
/* end subroutine (proginfo::pathto) */

int proginfo::getvn(cchar **rpp) noex {
	const nullptr_t	np{} ;
	int		rs = SR_FAULT ;
	if (rpp) {
	    cchar	*vn = nullptr ;
	    rs = SR_OK ;
	    switch (pm) {
	    case progmode_haveprogram:
	    case progmode_pathto:
	    case progmode_pt:
	        vn = varname.path ;
	        break ;
	    case progmode_havefunction:
	    case progmode_fpathto:
	        vn = varname.fpath ;
	        break ;
	    case progmode_manto:
	        vn = varname.manpath ;
		{
		    cchar	*vv = getourenv(envv,vn) ;
		    if ((vv == np) || (vv[0] == '\0')) {
			vn = varname.manxpath ;
		    } /* end if */
	        } /* end block */
	        break ;
	    default:
		rs = SR_BUGCHECK ;
		break ;
	    } /* end switch */
	    *rpp = (rs >= 0) ? vn : nullptr ;
	} /* end if (non-null) */
	return rs ;
}
/* end method (proginfo::getvn) */

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
	    case proginfomem_pathend:
	        rs = op->ipathend() ;
	        break ;
	    } /* end switch */
	} /* end if (non-null) */
	return rs ;
}
/* end method (proginfo_co::operator) */

int proginfo_pabeg::operator () (cchar *vn) noex {
	int	rs = SR_BUGCHECK ;
	if (op) {
	    if (vn == nullptr) vn = varname.path ;
	    rs = op->ipathbegin(vn) ;
	}
	return rs ;
}
/* end method (proginfo_pabeg::operator) */


