/* have_main SUPPORT (haveprogram) */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* determine if a given name is an executable program */
/* version %I% last-modified %G% */

#define	CF_DEBUG	0		/* debug switch */

/* revision history:

	= 1989-03-01, David A-D- Morano
	This code was originally written.

*/

/* Copyright © 1989 David A-D- Morano.  All rights reserved. */

/*******************************************************************************

	Names:
	haveprogram
	havefunction
	pathenum
	pathto
	fpathto
	manto

	Description:
	Print out the home directory for the given user.

	Synopsis:
	$ haveprogram
	$ havefunction
	$ pathenum
	$ fpathto
	$ pathto
	$ manto

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
#include	<unistd.h>
#include	<fcntl.h>
#include	<climits>
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<cstring>		/* |strcmp(3c)| | |strchr(3c)| */
#include	<string>
#include	<string_view>
#include	<vector>
#include	<iostream>
#include	<usystem.h>
#include	<getourenv.h>
#include        <getfdfile.h>           /* |FD_STDERR| */
#include	<varnames.hh>
#include	<strn.h>
#include	<sfx.h>
#include	<rmx.h>
#include	<strwcpy.h>
#include	<strnul.hh>
#include	<matstr.h>
#include	<ascii.h>
#include	<isnot.h>
#include	<mapex.h>
#include	<exitcodes.h>
#include	<localmisc.h>

#pragma		GCC dependency	"mod/libutil.ccm"
#pragma		GCC dependency	"mod/umisc.ccm"
#pragma		GCC dependency	"mod/ulibvals.ccm"

import libutil ;			/* |lenstr(3u)| */
import umisc ;				/* |mknpath(3u)| */
import ulibvals ;
import debug ;

/* local defines */

#ifndef CF_DEBUG
#define CF_DEBUG        0               /* debug */
#endif


/* imported namespaces */

using std::string ;			/* type */
using std::string_view ;		/* type */
using std::vector ;			/* type */
using std::cout ;			/* variable */
using std::cerr ;			/* variable */
using std::nothrow ;			/* constant */


/* local typedefs */

typedef string_view	strview ;


/* external subroutines */


/* external variables */


/* local structures */

namespace {
    struct pathent {
	string		ps ;
	dev_t		dev ;
	ino_t		ino ;
	pathent() noex = default ;
	pathent(dev_t d,ino_t i,cchar *pp,int pl = -1) noex {
	    if (pl < 0) pl = lenstr(pp) ;
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
	vector<pathent>	plist ;
	int		argc ;
	int		pm ;
	bool		ffound = false ;
	proginfo(int c,mainv a,mainv e) noex : argc(c), argv(a), envv(e) { 
	    start	(this,proginfomem_start) ;
	    finish	(this,proginfomem_finish) ;
	    pathend	(this,proginfomem_pathend) ;
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
	int pathadd(dev_t,ino_t,cchar *,int) noex ;
	int pathenum() noex ;
	int pathtox() noex ;
	int pathto(cchar * = nullptr) noex ;
	int pathtos(char *,int,cchar *) noex ;
	int present(cchar *) noex ;
	int getpn(mainv) noex ;
	int getvn(cchar **) noex ;
    private:
	int istart() noexcept ;
	int ifinish() noexcept ;
	int ipathbegin(cchar *) noexcept ;
	int ipathend() noexcept ;
	int pathenumone(cchar *) noex ;
    } ; /* end struct (proginfo) */
} /* end namespace */


/* forward references */


/* local variables */

enum progmodes {
	progmode_haveprogram,
	progmode_havefunction,
	progmode_pathenum,
	progmode_pathto,
	progmode_fpathto,
	progmode_manto,
	progmode_pe,
	progmode_pt,
	progmode_ft,
	progmode_mt,
	progmode_hpdebug,
	progmode_overlast
} ; /* end enum (progmodes) */

static constexpr cpcchar	prognames[] = {
	[progmode_haveprogram]	= "haveprogram",
	[progmode_havefunction]	= "havefunction",
	[progmode_pathenum]	= "pathenum",
	[progmode_pathto]	= "pathto",
	[progmode_fpathto]	= "fpathto",
	[progmode_manto]	= "manto",
	[progmode_pe]		= "pe",
	[progmode_pt]		= "pt",
	[progmode_ft]		= "ft",
	[progmode_mt]		= "mt",
	[progmode_hpdebug]	= "hpdebug",
	[progmode_overlast]	= nullptr
} ; /* end array (prognames) */

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
} ; /* end array (mapexs) */

static cint		maxpathlen = ulibval.maxpathlen ;

constexpr bool          f_debug         = CF_DEBUG ;


/* exported variables */


/* exported subroutines */

int main(int argc,mainv argv,mainv envv) {
	constexpr int   dfd = (f_debug) ? FD_STDERR : -1 ;
	int		ex = EX_OK ;
	int		rs ;
	int		rs1 ;
	debfd(dfd) ;
        debprintf(__func__,"ent\n") ;
	if (proginfo pi(argc,argv,envv) ; (rs = pi.start) >= 0) {
            switch (pi.pm) {
            case progmode_hpdebug:
            case progmode_haveprogram:
            case progmode_havefunction:
                if ((rs = pi.pathtox()) == 0) {
                    ex = (pi.ffound) ? EX_NOEXEC : EX_NOPROG ;
                } /* end if (pathto) */
                break ;
            case progmode_pe:
                pi.pm = progmode_pathenum ;
		fallthrough ;
                /* FALLTHROUGH */
            case progmode_pathenum:
                rs = pi.pathenum() ;
                break ;
            case progmode_pt:
                pi.pm = progmode_pathto ;
		fallthrough ;
                /* FALLTHROUGH */
            case progmode_pathto:
                rs = pi.pathto() ;
                break ;
            case progmode_ft:
                pi.pm = progmode_fpathto ;
		fallthrough ;
                /* FALLTHROUGH */
            case progmode_fpathto:
                rs = pi.pathto() ;
                break ;
	    case progmode_mt:
		pi.pm = progmode_manto ;
		fallthrough ;
		/* FALLTHROUGH */
            case progmode_manto:
                rs = SR_NOSYS ;
                break ;
            } /* end switch */
	    rs1 = pi.finish ;
	    if (rs >= 0) rs = rs1 ;
	} /* end if (proginfo) */
	if ((ex == EX_OK) && (rs < 0)) {
	    ex = mapex(mapexs,rs) ;
	}
	return ex ;
}
/* end subroutine (main) */


/* local subroutines */

int proginfo::istart() noexcept {
	int		rs = SR_FAULT ;
	if (argv) {
	    rs = getpn(prognames) ;
	    debprintf(__func__,"rs=%d pn=%s\n",rs,pn) ;
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
		    if (cint rl = rmchr(bp,bl,'.') ; rl > 0) {
	                if ((pm = matstr(names,bp,rl)) >= 0) {
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
    	cnullptr	np{} ;
	int		rs = SR_FAULT ;
	if (vn) {
	    rs = SR_INVALID ;
	    if (vn[0]) {
		rs = SR_OK ;
	        if (cchar *valp ; (valp = getourenv(envv,vn)) != np) {
	            cchar	*sp = valp ;
	            int		sl = lenstr(sp) ;
	            for (cc *tp ; (tp = strnchr(sp,sl,':')) != np ; ) {
			if (cint tl = intconv(tp - sp) ; tl > 0) {
	 	            rs = pathcandidate(sp,tl) ;
		        } /* end if (non-zero positive) */
		        sl -= intconv((tp + 1) - sp) ;
		        sp = (tp + 1) ;
		        if (rs < 0) break ;
	            } /* end for */
	            if ((rs >= 0) && (sl > 0)) {
	 	        rs = pathcandidate(sp,sl) ;
	            }
	        } /* end if (got value) */
	    } /* end if (valid) */
	} /* end if (non-null) */
	return rs ;
}
/* end method (proginfo::ipathbegin) */

int proginfo::pathcandidate(cchar *sp,int µsl) noex {
	int		rs ;
	int		sl = rmtrailchr(sp,µsl,'/') ;
	while ((sl > 1) && (sp[0] == '/') && (sp[1] == '/')) {
	    sp += 1 ;
	    sl -= 1 ;
	}
	if (strnul s(sp,sl) ; (s != nullptr)) {
	    if (ustat sb ; (rs = u_stat(s,&sb)) >= 0) {
	        if (S_ISDIR(sb.st_mode)) {
	            const dev_t	d = sb.st_dev ;
	            const ino_t	i = sb.st_ino ;
	            if ((rs = pathalready(d,i)) == 0) {
			rs = pathadd(d,i,sp,sl) ;
	            } /* end if (not-already) */
	        } /* end if (is-dir) */
	    } else if (isNotAccess(rs)) {
	        rs = SR_OK ;
	    }
	} else {
	    rs = SR_NOMEM ;
	} /* end if */
	return rs ;
}
/* end method (proginfo::pathcandidate) */

int proginfo::ipathend() noex {
	plist.clear() ;
	return SR_OK ;
}
/* end method (proginfo::ipathend) */

int proginfo::pathalready(dev_t d,ino_t i) noex {
	int		rs = SR_OK ;
	bool		f = false ;
	for (cauto &e : plist) {
	    f = ((e.dev == d) && (e.ino == i)) ;
	    if (f) break ;
	} /* end for */
	return (rs >= 0) ? f : rs ;
}
/* end method (proginfo::pathalready) */

int proginfo::pathadd(dev_t d,ino_t i,cchar *sp,int sl) noex {
    	int		rs = SR_OK ;
	pathent	e(d,i,sp,sl) ;
	try {
	    if ((e.dev != 0) && (e.ino != 0)) {
		plist.push_back(e) ;
	    } else {
		rs = SR_NOMEM ;
	    }
	} catch (...) {
	    rs = SR_NOMEM ;
	}
	return rs ;
} /* end method (proginfo::pathadd) */

int proginfo::pathenum() noex {
	int		rs ;
	if (argc > 1) {
	    rs = SR_OK ;
	    for (int ai = 1 ; (rs >= 0) && (ai < argc) && argv[ai] ; ai += 1) {
	        if (cchar *vn = argv[ai] ; vn[0]) {
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
	    for (cauto &e : plist) {
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
	if (cchar *vn ; (rs = getvn(&vn)) >= 0) {
	    rs = pathto(vn) ;
	} /* end if (getvn) */
	return rs ;
}
/* end subroutine (proginfo::pathtox) */

int proginfo::pathto(cchar *vn) noex {
    	cnothrow	nt{} ;
    	cnullptr	np{} ;
	cint		pm_ft = progmode_fpathto ;
	cint		pm_hf = progmode_havefunction ;
	int		rs ;
	int		rs1 ;
	int		ctotal = 0 ;
	if (vn == nullptr) {
	    bool f = false ;
	    f = f || (pm == pm_ft) ;
	    f = f || (pm == pm_hf) ;
	    if (f) vn = varname.fpath ;
	} /* end if (NULL variable-name) */
	if ((rs = pathbegin(vn)) >= 0) {
	    if ((rs = maxpathlen) >= 0) {
	        cint	plen = rs ;
	        rs = SR_NOMEM ;
	        if (char *pbuf ; (pbuf = new(nt) char[plen+1]) != np) {
		    rs = SR_OK ;
	            for (int ai = 1 ; (ai < argc) && argv[ai] ; ai += 1) {
	                cchar	*ap = argv[ai] ;
		        int	c = 0 ;
	                if (ap[0]) {
			    if (strchr(ap,CH_SLASH) != np) {
				rs = present(ap) ;
				c += rs ;
			    } else {
				rs = pathtos(pbuf,plen,ap) ;
				c += rs ;
			    }
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
} /* end subroutine (proginfo::pathto) */

int proginfo::pathtos(char *pbuf,int plen,cchar *ap) noex {
	cint		pm_hp = progmode_haveprogram ;
	cint		pm_hf = progmode_havefunction ;
	cint		pm_hd = progmode_hpdebug ;
    	int		rs = SR_OK ;
	int		c = 0 ;
        ffound = false ;
        for (cauto &e : plist) {
            cchar   *pp = e.ps.c_str() ; /* <- throw? */
            if ((rs = mknpath(pbuf,plen,pp,ap)) >= 0) {
                rs = present(pbuf) ;
                c += rs ;
            } /* end if (mknpath) */
            if ((pm == pm_hp) && (c > 0)) break ;
            if ((pm == pm_hf) && (c > 0)) break ;
            if ((pm == pm_hd) && (c > 0)) break ;
            if (rs < 0) break ;
        } /* end for (paths) */
	return (rs >= 0) ? c : rs ;
} /* end subroutine (proginfo::pathtos) */

int proginfo::present(cchar *pbuf) noex {
  	cint		pm_pathto = progmode_pathto ;
	cint		pm_fpathto = progmode_fpathto ;
	cint		pm_manto = progmode_manto ;
    	int		rs ;
	int		c = 0 ;
	if (ustat sb ; (rs = u_stat(pbuf,&sb)) >= 0) {
            cmode   fm = sb.st_mode ;
            ffound = true ;
            if (S_ISREG(fm)) {
                mode_t      xxx = 0 ;
		bool f = false ;
                f = f || (pm == pm_pathto) ;
                f = f || (pm == pm_fpathto) ;
                f = f || (pm == pm_manto) ;
                switch (pm) {
                case progmode_pathto:
                case progmode_haveprogram:
                case progmode_hpdebug:
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
	return (rs >= 0) ? c : rs ;
} /* end method (proginfo::present) */

int proginfo::getvn(cchar **rpp) noex {
	cnullptr	np{} ;
	int		rs = SR_FAULT ;
	if (rpp) {
	    cchar	*vn = nullptr ;
	    rs = SR_OK ;
	    switch (pm) {
	    case progmode_haveprogram:
	    case progmode_pathto:
	    case progmode_pt:
	    case progmode_hpdebug:
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
	    debprintf(__func__,"rs=%d vn=%s\n",rs,vn) ;
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


