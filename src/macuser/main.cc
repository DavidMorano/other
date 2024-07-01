/* main (userhome) */
/* lang=C++20 */

/* generic front-end for SHELL built-ins */
/* version %I% last-modified %G% */

#define	CF_GETUSERSHELL		1		/* use |getusershell(3c)| */

/* revision history:

	= 1989-03-01, David A.D. Morano
	This subroutine was originally written.

	= 2007-08-22, David A.D. Morano
	I refactored and enhanced this for use on MacOS.  MacOS is
	a brain-damaged system and needs some help.  But I could
	not easily port the full (featured) versions of these same
	utilities from Solaris® and other System V systems because
	MacOS does not support a moderately extended POSIX® development
	environment.  The stuff below is a total hack, very quickly
	written.

*/

/* Copyright © 1989,2007 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Names:
	username
	userhome
	usershell
	gid
	groupname
	uid
	sid
	sessionid
	shells
	userents
	groupents

	Description:
	Print out the home directory (or some other user-related
	information) for the given user.

	Synopsis:
	$ username
	$ userhome
	$ usershell
	$ gid
	$ groupname
	$ uid
	$ sid
	$ sessionid
	$ shells
	$ userents
	$ groupents

	Returns:
	EXIT_SUCCESS	user was found and processed successfully
	EXIT_FAILURE	some kind of error (there could be many)

	Notes:
	This subroutine (whole program) is written in very low-level
	functions and utilities.  So there is quite a bit of code
	in order to get the functionality needed and required for
	the uses this program will be subjected to.  Also this code
	is written using very poor style and abstractions (which
	are noramlly very important in my real-live code).  In the
	real world (in real life) essentially all of the code below
	is encapulated within one subroutine call (via my very
	extensive libraries for system-related operations).  So
	please exuse both the mess and size of the code below.
	Also, the implementation below of some functions is not the
	most efficient.  For example, note that I unnecessarily
	double-tap the system PASSWD database (effectively access
	it twice in a row with the same key) in both the subroutines
	|findutmp_stdin()| and |fundutmp_env()|.  These kinds of
	suboptimal coding design choices occur due to programmer
	fatigue (like I am experiencing right now).  Happlily, these
	particular inefficiencies hardly matter for the intected
	use context of this program (also the system PASSWD entry
	will be cached on the second access w/ the same key).

	Strange-note:
	Yes, the issue is Apple Darwin -- again!  For some weirdo
	unknown crazy reason, Darwin returns ENOTTY on an |fstat(2)|
	when the input file-descrptor is not owned by the current
	user!  I would have thought that ENOACCES should have been
	returned, but NO..OOO...OOO!  We live, learn, and work around.

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
#include	<string>
#include	<fstream>
#include	<iostream>
#include	<algorithm>
#include	<unordered_set>
#include	<utility>
#include	<usystem.h>
#include	<sfx.h>
#include	<matstr.h>
#include	<rmx.h>
#include	<strwcpy.h>
#include	<utmpx.h>
#include	<pwd.h>
#include	<grp.h>
#include	<ccfile.hh>
#include	<snx.h>
#include	<hasx.h>
#include	<isoneof.h>
#include	<isnot.h>
#include	<localmisc.h>


/* local defines */

#define	MAXLINE		(4*1024)
#define	FNSHELLS	"/etc/shells"

#ifndef	DEVPREFIX
#define	DEVPREFIX	"/dev/"
#endif

#ifndef	FD_STDIN
#define	FD_STDIN	0
#endif

#ifndef	TERMBUFLEN
#define	TERMBUFLEN	256		/* terminal-device buffer length */
#endif

#ifndef	UT_IDSIZE
#define	UT_IDSIZE	4
#endif
#ifndef	UT_NAMESIZE
#define	UT_NAMESIZE	8
#endif
#ifndef	UT_LINESIZE
#define	UT_LINESIZE	8
#endif
#ifndef	UT_HOSTSIZE
#define	UT_HOSTSIZE	16
#endif

#ifndef	VARUTMPLINE
#define	VARUTMPLINE	"UTMPLINE"
#endif
#ifndef	VARLOGLINE
#define	VARLOGLINE	"LOGLINE"
#endif

#ifndef	CF_GETUSERSHELL
#define	CF_GETUSERSHELL		1		/* use |getusershell(3c)| */
#endif


/* imported namespaces */

using std::string ;			/* type */
using std::ifstream ;			/* type */
using std::unordered_set ;		/* type */
using std::pair ;			/* type */
using std::clog ;			/* variable */
using std::cerr ;			/* variable */
using std::cout ;			/* variable */
using std::nothrow ;			/* constant */


/* local typedefs */

using setiter = std::unordered_set<string>::iterator ;


/* external subroutines */


/* external variables */


/* local structures */

enum prognames {
	progmode_username,
	progmode_userhome,
	progmode_usershell,
	progmode_gid,
	progmode_groupname,
	progmode_uid,
	progmode_sid,
	progmode_sessionid,
	progmode_shells,
	progmode_userents,
	progmode_groupents,
	progmode_overlast
} ;

constexpr cpcchar	prognames[] = {
	"username",
	"userhome",
	"usershell",
	"gid",
	"groupname",
	"uid",
	"sid",
	"sessionid",
	"shells",
	"userents",
	"groupents",
	nullptr
} ;

constexpr cpcchar	envs[] = {
	"USERNAME",
	"USER",
	"LOGNAME",
	"HOME",
	"MAIL",
	nullptr
} ;

namespace {
    struct userinfo {
	PASSWD		*hintpwp = nullptr ;
	string		hintuser ;
	string		un ;
	string		uh ;
	string		us ;
	gid_t		gid ;
	int find(uid_t) noex ;
	int findhint(uid_t) noex ;
	int findenv(uid_t) noex ;
	int findutmp(uid_t) noex ;
	int findutmp_sid(uid_t) noex ;
	int findutmp_stdin(uid_t) noex ;
	int findutmp_env(uid_t) noex ;
	int findutmp_stat(uid_t) noex ;
	int finduid(uid_t) noex ;
	int load(PASSWD *) noex ;
	int printdef(int,int,mainv,uid_t) noex ;
	int printone(int,uid_t) noex ;
    } ; /* end struct (userinfo) */
}

constexpr bool		f_getusershell = CF_GETUSERSHELL ;


/* forward references */

static int	getpn(int,mainv,mainv) noex ;
static int	procgroup(int,const userinfo *) noex ;
static int	printgroup(const userinfo *) noex ;
static int	printshells() noex ;
static int	printuserents() noex ;
static int	printgroupents() noex ;

static UTMPX	*getutxliner(UTMPX *) noex ;

static char	*strtcpy(char *,cchar *,int) noex ;


/* local variables */

constexpr cpcchar	utmpvars[] = {
	VARUTMPLINE,
	VARLOGLINE
} ;

constexpr gid_t		gidend = gid_t(-1) ;

constexpr int		utl_id		= UT_IDSIZE ;
constexpr int		utl_user	= UT_NAMESIZE ;
constexpr int		utl_line	= UT_LINESIZE ;
constexpr int		utl_host	= UT_HOSTSIZE ;

constexpr cchar		fnshells[] = FNSHELLS ;


/* exported variables */


/* exported subroutines */

int main(int argc,mainv argv,mainv) noex {
	const uid_t	uid = getuid() ;
	int		ex = EXIT_SUCCESS ;
	int		rs ;
	if ((rs = getpn(argc,argv,prognames)) >= 0) {
	    userinfo	ui ;
	    cint	pm = rs ;
	    switch (pm) {
	    case progmode_uid:
		cout << uid << eol ;
		break ;
	    case progmode_sid:
	    case progmode_sessionid:
		{
		    cint	sid = getsid(0) ;
		    cout << sid << eol ;
		}
		break ;
	    case progmode_shells:
		rs = printshells() ;
		break ;
	    case progmode_userents:
		rs = printuserents() ;
		break ;
	    case progmode_groupents:
		rs = printgroupents() ;
		break ;
	    default:
		rs = ui.printdef(pm,argc,argv,uid) ;
		break ;
	    } /* end switch */
	} /* end if (getpn) */
	if (rs < 0) ex = EXIT_FAILURE ;
	return ex ;
}
/* end subroutine (main) */


/* local subroutines */

int userinfo::printdef(int pm,int argc,mainv argv,uid_t uid) noex {
	int		rs = SR_OK ;
	if ((argc > 1) && argv[1]) {
	    PASSWD	*pwp ;
	    for (int ai = 1 ; (ai < argc) && argv[ai] ; ai += 1) {
		if (cchar *aun = argv[ai] ; aun[0]) {
		    if ((pwp = getpwnam(aun)) != nullptr) {
		        hintuser = aun ;
		        hintpwp = pwp ;
			rs = printone(pm,pwp->pw_uid) ;
		    }
		}
		if (rs < 0) break ;
	    } /* end for */
	} else {
	    rs = printone(pm,uid) ;
	}
	return rs ;
}
/* end method (userinfo::printdef) */

int userinfo::printone(int pm,uid_t uid) noex {
	int		rs ;
        if ((rs = find(uid)) > 0) {
            switch (pm) {
            case progmode_username:
                cout << un << eol ;
                break ;
            case progmode_userhome:
                cout << uh << eol ;
                break ;
            case progmode_usershell:
                cout << us << eol ;
                break ;
            case progmode_gid:
            case progmode_groupname:
                rs = procgroup(pm,this) ;
                break ;
            default:
                rs = SR_BUGCHECK ;
                break ;
            } /* end switch */
        } /* end if (userinfo::find) */
	return rs ;
}
/* end method (userinfo::printone) */

static int getpn(int argc,mainv argv,mainv names) noex {
	int		rs = SR_FAULT ;
	if (argv) {
	    rs = SR_NOMSG ;
	    if ((argc > 0) && argv[0]) {
	        cchar	*bp{} ;
	        if (int bl ; (bl = sfbasename(argv[0],-1,&bp)) > 0) {
		    cint	pl = rmchr(bp,bl,'.') ;
		    cchar	*pp = bp ;
		    if (pl > 0) {
	                if (int i ; (i = matstr(names,pp,pl)) >= 0) {
		            rs = i ;
	                }
		    } /* end if (non-zero positive progname) */
		} /* end if (have base-name) */
	    } /* end if (have first argument) */
	} /* end if (non-null) */
	return rs ;
}
/* end subroutine (getpn) */

static int procgroup(int pm,const userinfo *uip) noex {
	int		rs = SR_INVALID ;
	if (uip->gid != gidend) {
	    switch (pm) {
	    case progmode_gid:
	        {
		    cout << uip->gid << eol ;
		    rs = SR_OK ;
	        }
	        break ;
	    case progmode_groupname:
	        rs = printgroup(uip) ;
	        break ;
	    } /* end switch */
	} /* end if (valid) */
	return rs ;
}
/* end subroutine (procgroup) */

static int printgroup(const userinfo *uip) noex {
	int		rs = SR_NOTFOUND ;
	const GROUP	*grp = getgrgid(uip->gid) ;
	if (grp) {
	    cout << grp->gr_name << eol ;
	    rs = SR_OK ;
	} /* end if */
	return rs ;
}
/* end subroutine (printgroup) */

static int printshells() noex {
	char		*lbuf ;
	int		rs = SR_NOMEM ;
	int		rs1 ;
	if_constexpr (f_getusershell) {
	    rs = SR_OK ;
	    while ((lbuf = getusershell()) != nullptr) {
		cout << lbuf << eol ;
	    } /* end while */
	} else {
	    cint	llen = MAXLINE ;
	    if ((lbuf = new(nothrow) char[llen+1]) != nullptr) {
	        try {
		    if (ccfile fis ; (rs = fis.open(fnshells)) >= 0) {
		        while ((rs = fis.readln(lbuf,llen)) > 0) {
			    if (hasnotempty(lbuf,rs) && (lbuf[0] != '#')) {
			        cout << lbuf ;
			    } /* end if (hasnotempty) */
		        } /* end while */
		        rs1 = fis.close ;
		        if (rs >= 0) rs = rs1 ;
 	            } /* end if (opened) */
	        } catch (...) {
		    rs = SR_IO ;
	        }
	        delete [] lbuf ;
	    } /* end if (m-a-f) */
	} /* end if_constexpr (f_getusershell) */
	return rs ;
}
/* end subroutine (printshells) */

static int printuserents() noex {
	unordered_set<string>	seen ;
	pair<setiter,bool>	ret ;
	int		rs = SR_OK ;
	try {
	    PASSWD	*pwp ;
	    while ((pwp = getpwent()) != nullptr) {
	        cchar	*un = pwp->pw_name ;
		ret = seen.emplace(un) ;
		if (ret.second) cout << un << eol ;
	    } /* end while */
	} catch (...) {
	    rs = SR_NOMEM ;
	}
	return rs ;
}
/* end subroutine (printuserents) */

static int printgroupents() noex {
	unordered_set<string>	seen ;
	pair<setiter,bool>	ret ;
	int		rs = SR_OK ;
	try {
	    GROUP	*grp ;
	    while ((grp = getgrent()) != nullptr) {
		cchar	*gn = grp->gr_name ;
		ret = seen.emplace(gn) ;
		if (ret.second) cout << gn << eol ;
	    } /* end while */
	} catch (...) {
	    rs = SR_NOMEM ;
	}
	return rs ;
}
/* end subroutine (printgroupents) */

static bool isourtype(const UTMPX *up) noex {
	bool	f = false ;
	f = f || (up->ut_type == INIT_PROCESS) ;
	f = f || (up->ut_type == LOGIN_PROCESS) ;
	f = f || (up->ut_type == USER_PROCESS) ;
	return f ;
}
/* end subroutine (isourtype) */

int userinfo::find(uid_t uid) noex {
	int		rs ;
	if ((rs = findhint(uid)) == 0) {
	    if ((rs = findenv(uid)) == 0) {
	        if ((rs = findutmp(uid)) == 0) {
	            rs = finduid(uid) ;
	        }
	    }
	}
	return rs ;
}
/* end method (userinfo::find) */

int userinfo::findhint(uid_t) noex {
	int		rs = SR_OK ;
	int		len = 0 ;
	cchar		*hup = hintuser.c_str() ;
	if (hup[0] && hintpwp) {
	    len = load(hintpwp) ;
	}
	return (rs >= 0) ? len : rs ;
}

int userinfo::findenv(uid_t uid) noex {
	int		rs = SR_OK ;
	int		len = 0 ;
	for (int i = 0 ; envs[i] ; i += 1) {
	    cchar	*vn = envs[i] ;
	    if (cchar *rp ; (rp = getenv(vn)) != nullptr) {
		cchar	*cp{} ;
		if (int cl ; (cl = sfbasename(rp,-1,&cp)) > 0) {
		    PASSWD	*pwp ;
		    if ((pwp = getpwnam(rp)) != nullptr) {
		        if (pwp->pw_uid == uid) {
	    		    len = load(pwp) ;
		        } /* end if (match) */
		    } /* end if (getpwnam) */
		} /* end if (sfbasename) */
	    } /* end if (getenv) */
	    if ((rs < 0) || (len > 0)) break ;
	} /* end for */
	return (rs >= 0) ? len : rs ;
}
/* end method (userinfo::findenv) */

int userinfo::findutmp(uid_t uid) noex {
	int		rs ;
	if ((rs = findutmp_sid(uid)) == 0) {
	    if ((rs = findutmp_stdin(uid)) == 0) {
	        if ((rs = findutmp_env(uid)) == 0) {
	            rs = findutmp_stat(uid) ;
	        }
	    }
	}
	return rs ;
}
/* end method (userinfo::findutmp) */

int userinfo::findutmp_sid(uid_t uid) noex {
	const pid_t	sid = getsid(0) ;
	int		rs = SR_OK ;
	int		len = 0 ;
	UTMPX		*up ;
	setutxent() ;
	while ((up = getutxent()) != nullptr) {
	    if ((up->ut_pid == sid) && isourtype(up)) {
		cchar	*unp = up->ut_user ;
		if ((rs = strnlen(unp,sizeof(up->ut_user))) > 0) {
	            PASSWD	*pwp ;
	            char	ubuf[rs+1] ;
	            strwcpy(ubuf,unp,rs) ;
	            if ((pwp = getpwnam(ubuf)) != nullptr) {
			if (pwp->pw_uid == uid) {
	    		    len = load(pwp) ;
			} /* end if (uid match) */
	            } /* end if (getpwnam) */
		} /* end if (non-zero positive) */
	    } /* end if (match) */
	    if ((rs < 0) || (len > 0)) break ;
	} /* end while */
	endutxent() ;
	return (rs >= 0) ? len : rs ;
}
/* end method (userinfo::findutmp_sid) */

int userinfo::findutmp_stdin(uid_t uid) noex {
	USTAT		sb ;
	cint		tlen = TERMBUFLEN ;
	cint		fd = FD_STDIN ;
	int		rs ;
	int		len = 0 ;
	if ((rs = u_fstat(fd,&sb)) >= 0) {
	    cchar	*devprefix = DEVPREFIX ;
	    char	tbuf[tlen+1] ;
	    if ((rs = uc_ttyname(fd,tbuf,tlen)) >= 0) {
		cint	n = strlen(devprefix) ;
		if (strncmp(tbuf,devprefix,n) == 0) {
		    char	nbuf[utl_user+1] ;
		    UTMPX	ut{} ;
		    UTMPX	*up ;
		    PASSWD	*pwp ;
		    cchar	*line = (tbuf+n) ;
		    strncpy(ut.ut_line,line,utl_line) ;
		    if ((up = getutxliner(&ut)) != nullptr) {
		        strtcpy(nbuf,up->ut_user,utl_user) ;
		        if ((pwp = getpwnam(nbuf)) != nullptr) {
		            if (pwp->pw_uid == uid) {
			        len = load(pwp) ;
		            } /* end if (UID match w/ us) */
		        } /* end if (got PASSWD entry) */
		    } /* end if ("line" match) */
		} /* end if (have device) */
	    } else if (isNotTerm(rs)) {
		rs = SR_OK ;
	    } /* end if (ttyname) */
	} else if (isNotAccess(rs) || isNotTerm(rs)) {
	    rs = SR_OK ;
	} /* end if (stat) */
	return (rs >= 0) ? len : rs ;
}

int userinfo::findutmp_env(uid_t uid) noex {
	int		rs = SR_OK ;
	int		len = 0 ;
	char		nbuf[utl_user+1] ;
	for (auto const &vn : utmpvars) {
	    if (cchar *line ; (line = getenv(vn)) != nullptr) {
	        if (line[0]) {
	            UTMPX	ut{} ;
	            UTMPX	*up ;
		    PASSWD	*pwp ;
	            strncpy(ut.ut_line,line,utl_line) ;
	            if ((up = getutxliner(&ut)) != nullptr) {
		        strtcpy(nbuf,up->ut_user,utl_user) ;
		        if ((pwp = getpwnam(nbuf)) != nullptr) {
		            if (pwp->pw_uid == uid) {
			        len = load(pwp) ;
		            } /* end if (UID match w/ us) */
		        } /* end if (got PASSWD entry) */
		    } /* end if ("line" match) */
	        } /* end if (non-empty) */
	    } /* end if (getenv) */
	    if ((rs < 0) || (len > 0)) break ;
	} /* end for (utmpvars) */
	return (rs >= 0) ? len : rs ;
}

int userinfo::findutmp_stat(uid_t uid) noex {
	cint		sid = getsid(0) ;
	cint		tlen = TERMBUFLEN ;
	int		rs ;
	int		rs1 ;
	int		len = 0 ;
	cchar		*devprefix = DEVPREFIX ;
	char		tbuf[tlen+1] ;
	if ((rs = sncpy(tbuf,tlen,devprefix)) >= 0) {
	    char	nbuf[utl_user+1] ;
	    UTMPX	*up ;
	    cint	tl = rs ;
	    setutxent() ;
	    while ((up = getutxent()) != nullptr) {
	       if (isourtype(up)) {
		    cint	ll = utl_line ;
		    cchar	*lp = up->ut_line ;
	            if ((rs = snaddw(tbuf,tlen,tl,lp,ll)) >= 0) {
			cint	of = (O_RDONLY | O_NOCTTY) ;
			cmode	om = 0666 ;
			if ((rs = u_open(tbuf,of,om)) >= 0) {
			    cint	fd = rs ;
			    if ((rs = uc_tcgetsid(fd)) >= 0) {
    				if (sid == rs) {
				    PASSWD	*pwp ;
		                    strtcpy(nbuf,up->ut_user,utl_user) ;
		                    if ((pwp = getpwnam(nbuf)) != nullptr) {
		                        if (pwp->pw_uid == uid) {
			                    len = load(pwp) ;
		                        } /* end if (UID match w/ us) */
		                    } /* end if (got PASSWD entry) */
				} /* end if (SID match) */
			    } else if (isNotTerm(rs)) {
				rs = SR_OK ;
			    } /* end if (uc_tcgetsid) */
			    rs1 = u_close(fd) ;
			    if (rs >= 0) rs = rs1 ;
			} else if (isNotAccess(rs)) {
			    rs = SR_OK ;
			}
		    } /* end if (snadd) */
	        } /* end if (our-type) */
		if ((rs < 0) || (len > 0)) break ;
	    } /* end while */
	} /* end if (sncpy) */
	return (rs >= 0) ? len : rs ;
}
/* end method (userinfo::findutmp_stat) */

int userinfo::finduid(uid_t uid) noex {
	int		rs = SR_OK ;
	int		len = 0 ;
	PASSWD		*pwp ;
	if ((pwp = getpwuid(uid)) != nullptr) {
	    len = load(pwp) ;
	} /* end if */
	return (rs >= 0) ? len : rs ;
}
/* end method (userinfo::finduid) */

int userinfo::load(PASSWD *pwp) noex {
	int		rs = SR_NOTFOUND ;
	int		len = 0 ;
	if (pwp) {
	    rs = SR_OK ;
	    un = pwp->pw_name ;
	    uh = pwp->pw_dir ;
	    us = pwp->pw_shell ;
	    gid = pwp->pw_gid ;
	    len = un.length() ;
	}
	return (rs >= 0) ? len : rs ;
}
/* end method (userinfo::load) */

static UTMPX *getutxliner(UTMPX *sup) noex {
	static const uid_t	uid = getuid() ;
	UTMPX		*up ;
	PASSWD		*pwp ;
	char		nbuf[utl_user+1] ;
	setutxent() ;
	while ((up = getutxent()) != nullptr) {
	   if (isourtype(up)) {
		cint	ll = utl_line ;
		cchar	*lp = sup->ut_line ;
		if (strncmp(up->ut_line,lp,ll) == 0) {
		    strtcpy(nbuf,up->ut_user,utl_user) ;
		    if ((pwp = getpwnam(nbuf)) != nullptr) {
		        if (pwp->pw_uid == uid) {
    			    break ;
		        } /* end if (UID match w/ us) */
		    } /* end if (got PASSWD entry) */
		} /* end if ("line" match) */
	   } /* end if (our type) */
	} /* end while */
	return up ;
}
/* end subroutine (getutxliner) */

/* this is similar to |strwcpy(3uc)| */
static char *strtcpy(char *dp,cchar *sp,int dl) noex {
	if (dl >= 0) {
	    dp = strncpy(dp,sp,dl) + dl ;
	    *dp = '\0' ;
	} else {
	    dp = nullptr ;
	}
	return dp ;
}
/* end subroutine (strtcpy) */


