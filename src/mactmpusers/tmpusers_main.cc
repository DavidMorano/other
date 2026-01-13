/* tmpusers_main SUPPORT (macmktmpusers) */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* create the '/tmp/users' directory */
/* version %I% last-modified %G% */

#define	CF_DEBUG	0		/* debugging */

/* revision history:

	= 1989-03-01, David A-D- Morano
	This subroutine was originally written.

	= 2007-08-22, David A-D- Morano
	I refactored and enhanced this for use on MacOS. 

*/

/* Copyright © 1989,2007 David A-D- Morano.  All rights reserved. */

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
	$ mktmpusers
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

#include	<envstandards.h>	/* must be ordered first to configure */
#include	<sys/types.h>
#include	<sys/param.h>
#include	<unistd.h>		/* |getusershell(3c)| */
#include	<climits>
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>		/* |getenv(3c)| */
#include	<cstdio>
#include	<cstring>		/* |strcmp(3c)| */
#include	<algorithm>		/* |min(3c++)| + |max(3c++)| */
#include	<utility>		/* |pair(3c++)| */
#include	<string>
#include	<clanguage.h>
#include	<usysbase.h>
#include	<usyscalls.h>
#include	<usupport.h>
#include	<uclibmem.h>
#include	<ucsysconf.h>
#include        <getfdfile.h>           /* |FD_STDERR| */
#include	<sfx.h>
#include	<rmx.h>
#include	<strwcpy.h>
#include	<pwd.h>
#include	<grp.h>
#include	<matstr.h>
#include	<hasx.h>
#include	<isoneof.h>
#include	<isnot.h>
#include	<mapex.h>
#include	<exitcodes.h>
#include	<localmisc.h>		/* |MAXPATHLEN| + |USERNAMELEN| */
#include	<debprintf.h>

#pragma		GCC dependency		"mod/umisc.ccm"
#pragma		GCC dependency		"mod/usysconf.ccm"
#pragma		GCC dependency		"mod/ulibvals.ccm"
#pragma		GCC dependency		"mod/uconstants.ccm"
#pragma		GCC dependency		"mod/debug.ccm"

import umisc ;				/* |snadd{x}(3u)| */
import usysconf ;			/* |usysconfstr(3u)| */
import ulibvals ;
import uconstants ;			/* |varname(3u)| */
import debug ;

/* local defines */

#ifndef	CF_DEBUG
#define	CF_DEBUG	0		/* debugging */
#endif


/* imported namespaces */

using std::string ;			/* type (C++STD library) */
using std::min ;			/* subroutine-template (C++STD) */
using std::max ;			/* subroutine-template (C++STD) */
using libu::snwcpy ;			/* subroutine (internal from LIBU) */
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
	ustat		sb ;
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
	int		ul = 0 ;
	proginfo(int c,mainv a,mainv e) noex : argc(c), argv(a), envv(e) { 
	    start	(this,proginfomem_start) ;
	    finish	(this,proginfomem_finish) ;
	    pmbegin	(this,proginfomem_pmbegin) ;
	    pmend	(this,proginfomem_pmend) ;
	    userbegin	(this,proginfomem_userbegin) ;
	    userend	(this,proginfomem_userend) ;
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
	int tmpuserdir_link() noex ;
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
} /* end namaespace */


/* forward references */


/* local variables */

enum progmodes {
	progmode_mktmpusers,
	progmode_tmpusers,
	progmode_tmpmounts,
	progmode_tmpuserdir,
	progmode_overlast
} ; /* end enum (progmodes) */

constexpr cpcchar	prognames[] = {
	[progmode_mktmpusers]	= "mktmpusers",
	[progmode_tmpusers]	= "tmpusers",
	[progmode_tmpmounts]	= "tmpmounts",
	[progmode_tmpuserdir]	= "tmpuserdir",
	[progmode_overlast]	= nullptr
} ; /* end array (prognames) */

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
} ; /* end array (mapexs) */

constexpr confstritem	confstritems[] = {
	{ "public",	_CS_PUBLICDIR },
	{ "tmp",	_CS_TMPDIR },
	{ "cache",	_CS_CACHEDIR }
} ; /* end array (confstritems) */

constexpr cpcchar	uservars[] = {
	varname.username,
	varname.logname,
	varname.user,
	varname.home,
	varname.mail
} ; /* end array (uservars) */

constexpr proginfo_m	tmpusers_mems[] = {
	&proginfo::tmpusers_wait,
	&proginfo::tmpusers_make,
	&proginfo::tmpusers_mode
} ; /* end array (tmpusers_mems) */

constexpr proginfo_m	tmpuserdir_mems[] = {
	&proginfo::tmpuserdir_base,
	&proginfo::tmpuserdir_already
} ; /* end array (tmpuserdir_mems) */

static cint		maxpathlen	= ulibval.maxpathlen ;
static cint		usernamelen	= ulibval.usernamelen ;

static cint		varbuflen	= max(usernamelen,DIGBUFLEN) ;

constexpr char		tmpdir[]	= "/tmp" ;
constexpr char		usersdir[]	= "/users" ;

constexpr int		to_tmpwait	= 30 ; /* seconds */

constexpr mode_t	dm = (0777 | S_ISVTX) ;

cbool			f_debug = CF_DEBUG ;


/* exported variables */


/* exported subroutines */

int main(int argc,mainv argv,mainv envv) {
	constexpr int   dfd = (f_debug) ? FD_STDERR : -1 ;
	int		ex = EX_OK ;
	int		rs ;
	int		rs1 ;
	debfd(dfd) ;
        DEBPRINTF("ent\n") ;
	if (proginfo pi(argc,argv,envv) ; (rs = pi.start) >= 0) ylikely {
	    if ((rs = pi.pmbegin) >= 0) ylikely {
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
    	cnothrow	nt{} ;
	cnullptr	np{} ;
	int		rs ;
	if ((rs = getpn(prognames)) >= 0) ylikely {
	    if ((rs = maxpathlen) >= 0) ylikely {
		cint	pathlen = rs ;
	        rs = SR_NOMEM ;
	        if ((pbuf = new(nt) char[pathlen + 1]) != np) ylikely {
		    plen = pathlen ;
	            if ((dbuf = new(nt) char[pathlen + 1]) != np) ylikely {
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
	/* nothing to do */
	return rs ;
}
/* end method (proginfo::ipmend) */

int proginfo::iuserbegin() noex {
    	cnothrow	nt{} ;
	cnullptr	np{} ;
	const uid_t	uid = getuid() ;
	int		rs = SR_NOMEM ;
	cint		vlen = varbuflen ;
	char		vbuf[varbuflen + 1] ;
	if ((ubuf = new(nt) char[usernamelen + 1]) != np) ylikely {
	    ulen = usernamelen ;
	    rs = SR_OK ;
	    for (cauto &vn : uservars) {
		char	*vv = getenv(vn) ;
		if (vv && vn[0]) {
		    cchar	*bp{} ;
		    if (int bl ; (bl = sfbasename(vv,-1,&bp)) > 0) ylikely {
		        strwcpy(vbuf,bp,min(vlen,bl)) ;
		        if (PASSWD *pwp ; (pwp = getpwnam(vbuf)) != np) {
			    if (pwp->pw_uid == uid) {
			        rs = snwcpy(ubuf,ulen,pwp->pw_name) ;
			        ul = rs ;
			    }
		        } /* end if (getpwnam) */
		    } /* end if (sfbasename) */
		} /* end if */
		if (rs != 0) break ;
	    } /* end for */
	} /* end if (new-char) */
	return (rs >= 0) ? ul : rs ;
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
	if (argv) ylikely {
	    rs = SR_NOMSG ;
	    if ((argc > 0) && argv[0]) ylikely {
		cchar	*argz = argv[0] ;
	        cchar	*bp{} ;
	        if (int bl ; (bl = sfbasename(argz,-1,&bp)) > 0) ylikely {
		    if (cint rl = rmchr(bp,bl,'.') ; rl > 0) ylikely {
	                if ((pm = matstr(names,bp,rl)) >= 0) ylikely {
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
	if (pbuf) ylikely {
	    if (pbuf[0]) ylikely {
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
	if ((rs = snadd(pbuf,plen,pl,tmpdir)) >= 0) ylikely {
	    int		ti = 0 ;
	    bool	fto = false ;
	    pl += rs ;
	    while ((rs = tmpusers_present()) == 0) {
		sleep(1) ;
		fto = (ti++ >= to_tmpwait) ;
		if (fto) break ;
	    } /* end while */
	    if ((rs >= 0) && fto) {
		rs = SR_TIMEDOUT ;
	    }
	} /* end if (snadd) */
	return rs ;
}
/* end subroutine (proginfo::tmpusers_wait) */

int proginfo::tmpusers_make() noex {
	int		rs ;
	int		c = 0 ;
	if ((rs = snadd(pbuf,plen,pl,usersdir)) >= 0) ylikely {
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
	if ((rs = tmpmounts_vardir()) > 0) ylikely {
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
	if ((rs = snadd(dbuf,dlen,dl,"/",itp->dname)) >= 0) ylikely {
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
	if (itp->req >= 0) ylikely {
	    if ((rs = u_sysconfstr(itp->req,pbuf,plen)) > 0) ylikely {
	        if (cint rl = rmtrailchr(pbuf,rs,'/') ; rl > 1) ylikely {
	            pbuf[rl] = '\0' ;
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
    	cnothrow	nt{} ;
	cnullptr	np{} ;
	int		rs ;
	int		c = 0 ;
	if ((rs = maxpathlen) >= 0) ylikely {
	    cint	llen = rs ;
	    if (char *lbuf ; (lbuf = new(nt) char[llen + 1]) != np) ylikely {
		if ((rs = u_readlink(dbuf,lbuf,llen)) > 0) ylikely {
		    if (cint ll = rmtrailchr(lbuf,rs,'/') ; ll > 0) ylikely {
		        lbuf[ll] = '\0' ;
		        if ((rs = tmpmounts_same(lbuf)) == 0) ylikely {
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
	    if (ustat sb_link ; (rs = u_stat(lbuf,&sb_link)) >= 0) {
	        if (ustat sb_tmpconf ; (rs = u_stat(pbuf,&sb_tmpconf)) >= 0) {
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
	if (homedname) ylikely {
	    if (homedname[0]) ylikely {
		if ((rs = u_stat(homedname,&sb)) >= 0) ylikely {
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
	int		fok = false ; /* return-value */
	if ((rs = snadd(dbuf,dlen,dl,homedname)) >= 0) ylikely {
	    dl += rs ;
	    if ((rs = snadd(dbuf,dlen,dl,"/var")) >= 0) ylikely {
		dl += rs ;
		if ((rs = u_stat(dbuf,&sb)) >= 0) {
		    if (S_ISDIR(sb.st_mode)) {
			fok = true ;
		    }
		} else if (isNotPresent(rs)) {
		    if (cmode vdm = 0775 ; (rs = u_mkdir(dbuf,vdm)) >= 0) {
			if ((rs = u_minmod(dbuf,vdm)) >= 0) {
			    fok = true ;
			}
		    } else if (rs == SR_EXISTS) {	/* race condition? */
			rs = SR_OK ;
		    }
		} /* end if */
	        dl_homevar = dl ;
	    } /* end if (snadd) */
	} /* end if (snadd) */
	return (rs >= 0) ? fok : rs ;
}
/* end method (proginfo::tmpmounts_vardirs) */

int proginfo::tmpuserdir() noex {
	int		rs ;
	int		rs1 ;
	int		fdone = false ; /* return-value */
	debprintf(__func__,"ent\n") ;
	if ((rs = userbegin) > 0) ylikely {
	    for (cauto &m : tmpuserdir_mems) {
		if (rs > 0) {
	            rs = (this->*m)() ;
		}
		if (rs <= 0) break ;
	    } /* end for */
	    fdone = rs ;
	    rs1 = userend ;
	    if (rs >= 0) rs = rs1 ;
	} else if (rs == 0) {
	    fprintf(stderr,"%s: no-user\n",pn) ;
	} /* end if (user-) */
	debprintf(__func__,"ret rs=%d\n",rs) ;
	return (rs >= 0) ? fdone : rs ;
}
/* end method (proginfo::tmpuserdir) */

int proginfo::tmpuserdir_base() noex {
	int		rs ;
	int		fcontinue = false ; /* return-value */
	debprintf(__func__,"ent\n") ;
	if ((rs = snadd(pbuf,plen,pl,"/tmp/users")) >= 0) ylikely {
	    pl += rs ;
	    if ((rs = u_stat(pbuf,&sb)) >= 0) ylikely {
		fcontinue = true ;
	    } else if (isNotPresent(rs)) {
		pl = 0 ;		/* reset buffer */
		if ((rs = tmpusers()) >= 0) {
		    fcontinue = true ;
		}
	    }
	} /* end if (snadd) */
	debprintf(__func__,"ret rs=%d\n",rs) ;
	return (rs >= 0) ? fcontinue : rs ;
}
/* end method (proginfo::tmpuserdir_base) */

int proginfo::tmpuserdir_already() noex {
	int		rs ;
	int		fdone = false ; /* return-value */
	debprintf(__func__,"ent\n") ;
	if ((rs = revertuser()) >= 0) ylikely {
	debprintf(__func__,"1\n") ;
	    if ((rs = snadd(pbuf,plen,pl,"/",ubuf)) >= 0) ylikely {
	debprintf(__func__,"2\n") ;
	        pl += rs ;
	        if ((rs = u_stat(pbuf,&sb)) == SR_NOEXIST) {
	debprintf(__func__,"3\n") ;
		    rs = tmpuserdir_link() ;
		    fdone = rs ;
	        }
	    } /* end if (snadd) */
	} /* end if (revertuser) */
	debprintf(__func__,"ret rs=%d\n",rs) ;
	return (rs >= 0) ? fdone : rs ;
}
/* end method (proginfo::tmpuserdir_already) */

int proginfo::tmpuserdir_link() noex {
	cint		req = _CS_TMPDIR ;
	int		rs ;
	int		fmade = false ; /* return-value */
	debprintf(__func__,"ent\n") ;
	if ((rs = u_sysconfstr(req,dbuf,dlen)) > 0) ylikely {
	debprintf(__func__,"2\n") ;
	    if (cint rl = rmtrailchr(dbuf,rs,'/') ; rl > 1) ylikely {
	debprintf(__func__,"3\n") ;
	        dbuf[rl] = '\0' ;
	        if ((rs = u_stat(dbuf,&sb)) >= 0) ylikely {
	debprintf(__func__,"4\n") ;
		    if (S_ISDIR(sb.st_mode)) ylikely {
	debprintf(__func__,"5\n") ;
    			if ((rs = u_symlink(dbuf,pbuf)) >= 0) ylikely {
	debprintf(__func__,"6\n") ;
			    fmade = true ;
			} else if (rs == SR_EXISTS) {
			    rs = SR_OK ;
			}
		    } /* end if (is-dir) */
	        } else if (isNotPresent(rs)) {
		    rs = SR_OK ;
	        }
	    } /* end if (length-check) */
	} /* end if (u_sysconfstr) */
	DEBPRINTF("ret rs=%d\n",rs) ;
	return (rs >= 0) ? fmade : rs ;
}
/* end method (proginfo::tmpuserdir_link) */

int proginfo_co::operator () (int) noex {
	int		rs = SR_BUGCHECK ;
	if (op) ylikely {
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


