/* main SUPPORT (mactmpusers) */
/* lang=C++20 */

/* create the '/tmp/users' directory */
/* version %I% last-modified %G% */


/* revision history:

	= 1989-03-01, David A.D. Morano
	This subroutine was originally written.

	= 2007-08-22, David A.D. Morano
	I refactored and enhanced this for use on MacOS. 

*/

/* Copyright © 1989,2007 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Names:
	mktmpusers
	tmpusers
	tmpmounts
	tmpuserdir

	Description:
	[tmpusers] Make the directory:
		/tmp/users/
	[tmpmounts] Make the symbolic links to various "tmp"
	directories under the directory:
		${HOME}/var

	Synopsis:
	$ tmpusers
	$ tmpmounts
	$ tmpuserdir

	Returns:
	0		OK
	>0		various error codes (see |exitcodes|)

	Notes:
	Some things are considered errors.  Some errors are printed
	out, whether the program returns an error code on it or not.
	Other "errors" are silently ignored (like we do not really
	care overly too much about them).  Feel free to fix up
	whatever errors you think the program should terminate on
	(or not).

*******************************************************************************/

#include	<envstandards.h>
#include	<sys/types.h>
#include	<sys/param.h>
#include	<unistd.h>		/* |getusershell(3c)| */
#include	<climits>
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<cstdio>
#include	<cstring>		/* for |strlen(3c)| + |strnlen(3c)| */
#include	<iostream>
#include	<algorithm>		/* |min(3c++)| + |max(3c++)| */
#include	<utility>		/* |pair(3c++)| */
#include	<usystem.h>
#include	<uvariables.hh>
#include	<sfx.h>
#include	<rmx.h>
#include	<matstr.h>
#include	<snadd.h>
#include	<strwcpy.h>
#include	<pwd.h>
#include	<grp.h>
#include	<hasx.h>
#include	<isoneof.h>
#include	<isnot.h>
#include	<mapex.h>
#include	<exitcodes.h>
#include	<localmisc.h>		/* |MAXPATHLEN| + |USERNAMELEN| */


/* local defines */


/* imported namespaces */

using std::nullptr_t ;			/* type */
using std::nothrow ;			/* constant */


/* local typedefs */


/* external subroutines */


/* external variables */


/* local structures */

namespace {
    struct confstritem ;
    enum proginfomems {
	proginfomem_start,
	proginfomem_finish,
	proginfomem_pmbegin,
	proginfomem_pmend,
	proginfomem_userbegin,
	proginfomem_userend,
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
    typedef int (proginfo::*proginfo_m)() noex ;
    struct proginfo {
	friend		proginfo_co ;
	proginfo_co	start ;
	proginfo_co	finish ;
	proginfo_co	pmbegin ;
	proginfo_co	pmend ;
	proginfo_co	userbegin ;
	proginfo_co	userend ;
	USTAT		sb ;
	proginfo_m	m ;
	mainv		argv ;
	mainv		envv ;
	cchar		*pn = nullptr ;
	char		*pbuf = nullptr ;
	char		*dbuf = nullptr ;
	char		*ubuf = nullptr ;
	int		argc ;
	int		pm = 0 ;
	int		plen = 0 ;
	int		dlen = 0 ;
	int		ulen = 0 ;
	int		pl = 0 ;
	int		dl = 0 ;
	int		dl_homevar ;
	proginfo(int c,mainv a,mainv e) noex : argc(c), argv(a), envv(e) { 
	    start(this,proginfomem_start) ;
	    finish(this,proginfomem_finish) ;
	    pmbegin(this,proginfomem_pmbegin) ;
	    pmend(this,proginfomem_pmend) ;
	    userbegin(this,proginfomem_userbegin) ;
	    userend(this,proginfomem_userend) ;
	} ;
	proginfo() noex : proginfo(0,nullptr,nullptr) { } ;
	void operator () (int c,mainv a,mainv e) noex {
	    argc = c ;
	    argv = a ;
	    envv = e ;
	} ;
	int tmpusers() noex ;
	int tmpusers_present() noex ;
	int tmpusers_wait() noex ;
	int tmpusers_make() noex ;
	int tmpusers_mode() noex ;
	int tmpmounts() noex ;
	int tmpmounts_one(const confstritem *) noex ;
	int tmpmounts_oner(const confstritem *) noex ;
	int tmpmounts_oners() noex ;
	int tmpmounts_mklink() noex ;
	int tmpmounts_cklink() noex ;
	int tmpmounts_same(cchar *) noex ;
	int tmpmounts_vardir() noex ;
	int tmpmounts_vardirs(cchar *) noex ;
	int tmpuserdir() noex ;
	int tmpuserdir_base() noex ;
	int tmpuserdir_already() noex ;
    private:
	int istart() noex ;
	int ifinish() noex ;
	int ipmbegin() noex ;
	int ipmend() noex ;
	int getpn(mainv) noex ;
	int revertuser() noex ;
	int iuserbegin() noex ;
	int iuserend() noex ;
    } ; /* end struct (proginfo) */
    struct confstritem {
	cchar		*dname ;
	int		req ;
    } ;
}


/* forward references */


/* local variables */

enum progmodes {
	progmode_mktmpusers,
	progmode_tmpusers,
	progmode_tmpmounts,
	progmode_tmpuserdir,
	progmode_overlast
} ;

constexpr cpcchar	prognames[] = {
	"mktmpusers",
	"tmpusers",
	"tmpmounts",
	"tmpuserdir",
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

constexpr confstritem	confstritems[] = {
	{ "public",	_CS_PUBLICDIR },
	{ "tmp",	_CS_TMPDIR },
	{ "cache",	_CS_CACHEDIR }
} ;

constexpr cpcchar	uservars[] = {
	varname.username,
	varname.logname,
	varname.user,
	varname.home,
	varname.mail,
	varname.maildir
} ;

constexpr proginfo_m	tmpusers_mems[] = {
	&proginfo::tmpusers_wait,
	&proginfo::tmpusers_make,
	&proginfo::tmpusers_mode
} ;
constexpr proginfo_m	tmpuserdir_mems[] = {
	&proginfo::tmpuserdir_base,
	&proginfo::tmpuserdir_already
} ;

constexpr int		maxpathlen = MAXPATHLEN ;
constexpr int		usernamelen = USERNAMELEN ;
constexpr mode_t	dm = (0777|S_ISVTX) ;


/* exported variables */


/* exported subroutines */

int main(int argc,mainv argv,mainv envv) noex {
	proginfo	pi(argc,argv,envv) ;
	int		ex = EX_OK ;
	int		rs ;
	int		rs1 ;
	if ((rs = pi.start) >= 0) {
	    if ((rs = pi.pmbegin) >= 0) {
                switch (pi.pm) {
                case progmode_mktmpusers:
                case progmode_tmpusers:
                    rs = pi.tmpusers() ;
                    break ;
                case progmode_tmpmounts:
                    rs = pi.tmpmounts() ;
                    break ;
		case progmode_tmpuserdir:
		    rs = pi.tmpuserdir() ;
                    break ;
		default:
		    rs = SR_BUGCHECK ;
		    break ;
                } /* end switch */
		rs1 = pi.pmend ;
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
	    if ((rs = maxpathlen) >= 0) {
		cint	pathlen = rs ;
	        rs = SR_NOMEM ;
	        if ((pbuf = new(nothrow) char[pathlen + 1]) != nullptr) {
		    plen = pathlen ;
	            if ((dbuf = new(nothrow) char[pathlen + 1]) != nullptr) {
			dlen = pathlen ;
		        rs = SR_OK ;
	            } /* end if (new-char) */
		    if (rs < 0) {
			delete pbuf ;
			pbuf = nullptr ;
			plen = 0 ;
		    }
	        } /* end if (new-char) */
	    } /* end if (maxpathlen) */
	} /* end if (proginfo::getpn) */
	return rs ;
}
/* end method (proginfo::istart) */

int proginfo::ifinish() noex {
	int		rs = SR_OK ;
	if (dbuf) {
	    delete [] dbuf ;
	    dbuf = nullptr ;
	    dlen = 0 ;
	    dl = 0 ;
	}
	if (pbuf) {
	    delete [] pbuf ;
	    pbuf = nullptr ;
	    plen = 0 ;
	    pl = 0 ;
	}
	return rs ;
}
/* end method (proginfo::ifinish) */

int proginfo::ipmbegin() noex {
	int		rs = SR_OK ;
	switch (pm) {
	case progmode_tmpmounts:
	    rs = revertuser() ;
	    break ;
	} /* end switch */
	return rs ;
}
/* end method (proginfo::ipmbegin) */

int proginfo::ipmend() noex {
	int		rs = SR_OK ;
	return rs ;
}
/* end method (proginfo::ipmend) */

int proginfo::iuserbegin() noex {
	int		rs = SR_NOMEM ;
	if ((ubuf = new(nothrow) char[usernamelen + 1]) != nullptr) {
	    ulen = usernamelen ;
	} /* end if (new-char) */
	return rs ;
}
/* end method (proginfo::iuserbegin) */

int proginfo::iuserend() noex {
	int		rs = SR_OK ;
	if (ubuf) {
	    delete [] ubuf ;
	    ubuf = nullptr ;
	    ulen = 0 ;
	}
	return rs ;
}
/* end method (proginfo::iuserend) */

int proginfo::getpn(mainv names) noex {
	int		rs = SR_FAULT ;
	if (argv) {
	    rs = SR_NOMSG ;
	    if ((argc > 0) && argv[0]) {
		cchar	*argz = argv[0] ;
	        cchar	*bp{} ;
	        if (int bl ; (bl = sfbasename(argz,-1,&bp)) > 0) {
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

int proginfo::revertuser() noex {
	const uid_t	uid = getuid() ;
	const uid_t	euid = geteuid() ;
	int		rs = SR_OK ;
	if (uid != euid) {
	    rs = u_seteuid(uid) ; /* revert effective to real ID */
	} /* end if */
	return rs ;
}
/* end method (proginfo::revertuser) */

int proginfo::tmpusers() noex {
	int		rs = SR_OK ;
	for (cauto &m : tmpusers_mems) {
	    rs = (this->*m)() ;
	    if (rs < 0) break ;
	} /* end for */
	return rs ;
}
/* end method (proginfo::tmpusers) */

int proginfo::tmpusers_present() noex {
	int		rs = SR_BUGCHECK ;
	int		fpresent = false ;
	if (pbuf) {
	    if (pbuf[0]) {
	        if ((rs = u_stat(pbuf,&sb)) >= 0) {
		    fpresent = true ;
	        } else if (isNotPresent(rs)) {
	            rs = SR_OK ;
	        } /* end if (u_stat) */
	    } /* end if (valid) */
	} /* end if (non-null) */
	return (rs >= 0) ? fpresent : rs ;
}
/* end method (proginfo::tmpusers_present) */

int proginfo::tmpusers_wait() noex {
	int		rs ;
	if ((rs = snadd(pbuf,plen,pl,"/tmp")) >= 0) {
	    pl += rs ;
	    while ((rs = tmpusers_present()) == 0) {
		sleep(1) ;
	    } /* end while */
	} /* end if (snadd) */
	return rs ;
}
/* end subroutine (proginfo::tmpusers_wait) */

int proginfo::tmpusers_make() noex {
	int		rs ;
	int		c = 0 ;
	if ((rs = snadd(pbuf,plen,pl,"/users")) >= 0) {
	    pl += rs ;
	    if ((rs = u_stat(pbuf,&sb)) >= 0) {
		c = 0 ;
	    } else if (isNotPresent(rs)) {
		if ((rs = u_mkdir(pbuf,dm)) >= 0) {
		    if ((rs = u_stat(pbuf,&sb)) >= 0) {
		        c = 1 ;
		    }
		} else if (rs == SR_EXISTS) {	/* race condition? */
		    rs = SR_OK ;
		}
	    }
	} /* end if (snadd) */
	return (rs >= 0) ? c : rs ;
}
/* end subroutine (proginfo::tmpusers_make) */

int proginfo::tmpusers_mode() noex {
	int		rs = SR_OK ;
	int		c = 0 ;
	cmode		efm = (sb.st_mode & (~ S_IFMT)) ;
	if ((efm & dm) != dm) {
	    rs = u_minmod(pbuf,(efm | dm)) ;
	    c = 1 ;
	} /* end if (fix mode) */
	return (rs >= 0) ? c : rs ;
}
/* end method (proginfo::tmpusers_mode) */

int proginfo::tmpmounts() noex {
	int		rs ;
	int		c = 0 ;
	if ((rs = tmpmounts_vardir()) > 0) {
	    for (cauto &item : confstritems) {
	        rs = tmpmounts_one(&item) ;
		c += rs ;
	        if (rs < 0) break ;
	    } /* end for */
	} /* end if (tmpmounts_vardir) */
	return (rs >= 0) ? c : rs ;
}
/* end method (proginfo::tmpmounts) */

int proginfo::tmpmounts_one(const confstritem *itp) noex {
	int		rs ;
	int		c = 0 ;
	dl = dl_homevar ; /* start fresh each time */
	if ((rs = snadd(dbuf,dlen,dl,"/",itp->dname)) >= 0) {
	    dl += rs ;
	    {
		rs = tmpmounts_oner(itp) ;
		c = rs ;
	    }
	} /* end if (snadd) */
	return (rs >= 0) ? c : rs ;
}
/* end method (proginfo::tmpmounts_one) */

int proginfo::tmpmounts_oner(const confstritem *itp) noex {
	int		rs = SR_OK ;
	int		c = 0 ;
	if (itp->req >= 0) {
	    if ((rs = uc_sysconfstr(pbuf,plen,itp->req)) > 0) {
	        cint	pl = rmtrailchr(pbuf,rs,'/') ;
		if (pl > 1) { /* <- my own restriction */
	            pbuf[pl] = '\0' ;
	            if ((rs = u_stat(pbuf,&sb)) >= 0) {
		        if (S_ISDIR(sb.st_mode)) {
    			    rs = tmpmounts_oners() ;
			    c = rs ;
		        } /* end if (is-dir) */
	            } else if (isNotPresent(rs)) {
		        rs = SR_OK ;
	            }
		} /* end if (length-check) */
	    } else if (isNotSupport(rs)) {
	        rs = SR_OK ;
	    }
	} /* end if (valid-request) */
	return (rs >= 0) ? c : rs ;
}
/* end method (proginfo::tmpmounts_oner) */

int proginfo::tmpmounts_oners() noex {
	int		rs ;
	int		c = 0 ;
	if ((rs = u_lstat(dbuf,&sb)) >= 0) {
	    cmode	sm = sb.st_mode ;
	    if (S_ISLNK(sm)) {
		rs = tmpmounts_cklink() ;
		c = rs ;
	    } else if (S_ISREG(sm) || S_ISFIFO(sm) || S_ISSOCK(sm)) {
		if ((rs = u_unlink(dbuf)) >= 0) {
		    rs = tmpmounts_mklink() ;
		    c = rs ;
		}
	    }
	} else if (isNotPresent(rs)) {
	    rs = tmpmounts_mklink() ;
	    c = rs ;
	}
	return (rs >= 0) ? c : rs ;
}
/* end method (proginfo::tmpmounts_oners) */

int proginfo::tmpmounts_mklink() noex {
	int		rs ;
	if ((rs = u_symlink(pbuf,dbuf)) >= 0) {
	    rs = 1 ;
	} else if (rs == SR_EXISTS) {
	    rs = SR_OK ;
	}
	return rs ;
}
/* end method (proginfo::tmpmounts_mklink) */

int proginfo::tmpmounts_cklink() noex {
	int		rs ;
	int		c = 0 ;
	if ((rs = maxpathlen) >= 0) {
	    cnullptr	np{} ;
	    cint	llen = rs ;
	    if (char *lbuf ; (lbuf = new(nothrow) char[llen + 1]) != np) {
		if ((rs = u_readlink(dbuf,lbuf,llen)) > 0) {
		    cint	ll = rmtrailchr(lbuf,rs,'/') ;
		    if (ll > 0) {
		        lbuf[ll] = '\0' ;
		        if ((rs = tmpmounts_same(lbuf)) == 0) {
			    if ((rs = u_unlink(dbuf)) >= 0) {
		    	        rs = tmpmounts_mklink() ;
		    	        c = rs ;
			    }
		        }
		    } /* end if (length-check) */
		} /* end if (u_readlink) */
		delete [] lbuf ;
	    } /* end if (new-char) */
	} /* end if (maxpathlen) */
	return (rs >= 0) ? c : rs ;
}
/* end method (proginfo::tmpmounts_cklink) */

int proginfo::tmpmounts_same(cchar *lbuf) noex {
	int		rs = SR_OK ;
	int		fsame = false ;
	if ((lbuf[0] == '/') && (pbuf[0] == '/')) {
	    USTAT	sb_link ;
	    if ((rs = u_stat(lbuf,&sb_link)) >= 0) {
	        USTAT	sb_tmpconf ;
	        if ((rs = u_stat(pbuf,&sb_tmpconf)) >= 0) {
		    fsame = true ;
		    fsame = fsame && (sb_link.st_dev == sb_tmpconf.st_dev) ;
		    fsame = fsame && (sb_link.st_ino == sb_tmpconf.st_ino) ;
	        }  /* end if (already 'stat'ed, so should not fail) */
	    } else if (isNotPresent(rs)) {
	        rs = SR_OK ;
	    }
	} else {
	    fsame = (strcmp(lbuf,pbuf) == 0) ;
	} /* end if (absolute path or not) */
	return (rs >= 0) ? fsame : rs ;
}
/* end method (proginfo::tmpmounts_same) */

int proginfo::tmpmounts_vardir() noex {
	static cchar	*homedname = getenv(varname.home) ;
	int		rs = SR_OK ;
	int		fok = false ;
	cchar		*fmt = "%s: no-home-directory (%d)\n" ;
	if (homedname) {
	    if (homedname[0]) {
		if ((rs = u_stat(homedname,&sb)) >= 0) {
		    rs = tmpmounts_vardirs(homedname) ;
		    fok = rs ;
		} else if (isNotPresent(rs)) {
	    	    fprintf(stderr,fmt,pn,rs) ;
		}
	    }
	} else {
	    fprintf(stderr,fmt,pn,rs) ;
	}
	return (rs >= 0) ? fok : rs ;
}
/* end method (proginfo::tmpmounts_vardir) */

int proginfo::tmpmounts_vardirs(cchar *homedname) noex {
	int		rs ;
	int		fok = false ;
	if ((rs = snadd(dbuf,dlen,dl,homedname)) >= 0) {
	    dl += rs ;
	    if ((rs = snadd(dbuf,dlen,dl,"/var")) >= 0) {
		dl += rs ;
		if ((rs = u_stat(dbuf,&sb)) >= 0) {
		    if (S_ISDIR(sb.st_mode)) {
			fok = true ;
		    }
		} else if (isNotPresent(rs)) {
		    cmode	vdm = 0775 ;
		    if ((rs = u_mkdir(dbuf,vdm)) >= 0) {
			if ((rs = u_minmod(dbuf,vdm)) >= 0) {
			    fok = true ;
			}
		    } else if (rs == SR_EXISTS) {	/* race condition? */
			rs = SR_OK ;
		    }
		} /* end if */
	        dl_homevar = dl ;
	    } /* end if (snadd) */
	}
	return (rs >= 0) ? fok : rs ;
}
/* end method (proginfo::tmpmounts_vardirs) */

int proginfo::tmpuserdir() noex {
	int		rs ;
	int		rs1 ;
	if ((rs = userbegin) >= 0) {
	    for (cauto &m : tmpuserdir_mems) {
	        rs = (this->*m)() ;
		if (rs < 0) break ;
	    } /* end for */
	    rs1 = userend ;
	    if (rs >= 0) rs = rs1 ;
	} /* end if (user-) */
	return rs ;
}
/* end method (proginfo::tmpuserdir) */

int proginfo::tmpuserdir_base() noex {
	int		rs ;
	int		fpresent = false ;
	if ((rs = snadd(pbuf,plen,pl,"/tmp/users")) >= 0) {
	    pl += rs ;
	    if ((rs = u_stat(pbuf,&sb)) >= 0) {
		fpresent = true ;
	    } else if (isNotPresent(rs)) {
		if ((rs = tmpusers()) >= 0) {
		    fpresent = true ;
		    rs = revertuser() ;
		}
	    }
	}
	return (rs >= 0) ? fpresent : rs ;
}
/* end method (proginfo::tmpuserdir_base) */

int proginfo::tmpuserdir_already() noex {
	int		rs ;
	int		fpresent = false ;
	if ((rs = snadd(pbuf,plen,pl,"/tmp/users")) >= 0) {
	    pl += rs ;
	    if ((rs = u_stat(pbuf,&sb)) >= 0) {
		rs = 1 ;
	    }
	}
	return (rs >= 0) ? fpresent : rs ;
}
/* end method (proginfo::tmpuserdir_already) */

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
	    case proginfomem_pmbegin:
	        rs = op->ipmbegin() ;
	        break ;
	    case proginfomem_pmend:
	        rs = op->ipmend() ;
	        break ;
	    case proginfomem_userbegin:
	        rs = op->iuserbegin() ;
	        break ;
	    case proginfomem_userend:
	        rs = op->iuserend() ;
	        break ;
	    } /* end switch */
	} /* end if (non-null) */
	return rs ;
}
/* end method (proginfo_co::operator) */


