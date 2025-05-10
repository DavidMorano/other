/* main SUPPORT (macutmp) */
/* lang=C++20 */

/* print out the UTMP entry 'line' field if terminal session is registered */
/* version %I% last-modified %G% */


/* revision history:

	= 2023-12-15, David A.D. Morano
	This is originally written.

*/

/* Copyright © 2017 David A­D­ Morano.  All rights reserved. */
/* Use is subject to license terms. */

/*******************************************************************************

	Names:
	utmp
	boottime
	nusers
	utmpid
	utmpname
	utmpline
	utmphost
	utmpsid
	logid
	logname
	logline
	loghost
	logsid
	logged

	Description:
	See the assciated README file ('README.txt') for why and what
	this program does.

	Implementation-note:
	Of course, the code below is not how I normally program (no
	way).  The crap below looks like that because I wanted this
	program to be as portable as possible without any additional
	libraries (which I normally use extremely extensively and
	without abandon).  I actually find this kind of "raw"
	programming style below totally abhorrent.  Abhorrent to
	both the mind and the senses. So please excuse the crap
	below given the circumstances. It is only due to the presence
	of messed up operating system environments (like those found
	routinely on GNU-Linux based systems) that requires me to
	write programs like this in the first place.

*******************************************************************************/

#include	<envstandards.h>	/* ordered first to configure */
#include	<sys/types.h>
#include	<sys/stat.h>		/* <- for |USTAT| */
#include	<unistd.h>		/* <- for |ttyname_r(3c)| */
#include	<utmpx.h>		/* <- for |getutxline(3c)| */
#include	<pwd.h>			/* <- for |getpwnam(3c)| */
#include	<cstdlib>		/* <- for |EXIT_SUCCESS| */
#include	<cstdio>		/* <- for |printf(3c)| */
#include	<cstring>		/* <- |strncmp(3c)| + |strlen(3c)| */
#include	<iostream>
#include	<usystem.h>		/* for |u_fstat(3u)| */
#include	<snx.h>
#include	<isoneof.h>
#include	<isnot.h>
#include	<localmisc.h>		/* |TIMEBUFLEN| */


/* local defines */

#ifndef	FD_STDIN
#define	FD_STDIN	0
#endif

#ifndef	TERMBUFLEN
#define	TERMBUFLEN	256		/* terminal-device buffer length */
#endif

#ifndef	HOSTLEN
#define	HOSTLEN		1024		/* guessed that this is big enough! */
#endif

#ifndef	DEVPREFIX
#define	DEVPREFIX	"/dev/"
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

#ifndef	STAT
#define	STAT		USTAT
#endif

#ifndef	TM
#define	TM		struct tm
#endif

#ifndef	PASSWD
#define	PASSWD		struct passwd
#endif

#ifndef	UTMPX
#define	UTMPX		struct utmpx
#endif

#ifndef	eol
#define	eol		'\n'
#endif

#ifndef	VARUTMPLINE
#define	VARUTMPLINE	"UTMPLINE"
#endif
#ifndef	VARLOGLINE
#define	VARLOGLINE	"LOGLINE"
#endif


/* imported namespaces */

using std::min ;			/* subroutine-template */
using std::cout ;			/* variable */
using std::cerr ;			/* variable */


/* local typedefs */

typedef const int		cint ;
typedef const char		cchar ;
typedef const char *const	cpcchar ;
typedef const char *const *	cpccharp ;
typedef const char *const *	mainv ;


/* forward references */

static int getpm(int,mainv,mainv) noex ;
static int utmp(bool) noex ;
static int boottime() noex ;
static int findsid(int) noex ;
static int findstdin(int) noex ;
static int findenv(int) noex ;
static int findstat(int) noex ;
static int printutxval(int,UTMPX *) noex ;
static int sirchr(cchar *,int,int) noex ;

static bool isourtype(UTMPX *up) noex {
	bool	f = false ;
	f = f || (up->ut_type == INIT_PROCESS) ;
	f = f || (up->ut_type == LOGIN_PROCESS) ;
	f = f || (up->ut_type == USER_PROCESS) ;
	return f ;
}

static UTMPX	*getutxliner(UTMPX *) noex ;

static char	*strtcpy(char *,cchar *,int) noex ;


/* local variables */

enum progmodes {
	progmode_utmp,
	progmode_nusers,
	progmode_boottime,
	progmode_utmpid,
	progmode_utmpname,
	progmode_utmpline,
	progmode_utmphost,
	progmode_utmpsid,
	progmode_logid,
	progmode_logname,
	progmode_logline,
	progmode_loghost,
	progmode_logsid,
	progmode_logged,
	progmode_iverlast
} ;

constexpr cpcchar	prognames[] = {
	"utmp",
	"nusers",
	"boottime",
	"utmpid",
	"utmpname",
	"utmpline",
	"utmphost",
	"utmpsid",
	"logid",
	"logname",
	"logline",
	"loghost",
	"logsid",
	"logged",
	nullptr
} ;

constexpr cpcchar	utmpvars[] = {
	VARUTMPLINE,
	VARLOGLINE
} ;

constexpr int		utl_id		= UT_IDSIZE ;
constexpr int		utl_user	= UT_NAMESIZE ;
constexpr int		utl_line	= UT_LINESIZE ;
constexpr int		utl_host	= UT_HOSTSIZE ;
constexpr int		tlen = TIMEBUFLEN ;


/* exported variables */


/* exported subroutines */

int main(int argc,mainv argv,mainv) {
	int		ex = EXIT_SUCCESS ;
	int		rs ;
	if ((rs = getpm(argc,argv,prognames)) >= 0) {
	    cint	rsn = SR_NOTFOUND ;
	    int		pm = rs ;
	    int		fpm = 0 ;
	    switch (pm) {
	    case progmode_utmp:
		rs = utmp(true) ;
		break ;
	    case progmode_nusers:
		rs = utmp(false) ;
		cout << rs << eol ;
		break ;
	    case progmode_boottime:
		rs = boottime() ;
		break ;
	    case progmode_logid:
		if (!fpm++) pm = progmode_utmpid ;
		fallthrough ;
		/* FALLTHROUGH */
	    case progmode_logname:
		if (!fpm++) pm = progmode_utmpname ;
		fallthrough ;
		/* FALLTHROUGH */
	    case progmode_logline:
		if (!fpm++) pm = progmode_utmpline ;
		fallthrough ;
		/* FALLTHROUGH */
	    case progmode_loghost:
		if (!fpm++) pm = progmode_utmphost ;
		fallthrough ;
		/* FALLTHROUGH */
	    case progmode_logsid:
		if (!fpm++) pm = progmode_utmpsid ;
		fallthrough ;
		/* FALLTHROUGH */
	    default:
	        if ((rs = findsid(pm)) == rsn) {
	            if ((rs = findstdin(pm)) == rsn) {
			if ((rs = findenv(pm)) == rsn) {
			    rs = findstat(pm) ;
			}
		    }
	        }
		break ;
	    } /* end switch */
	} /* end if (getpm) */
	if ((rs < 0) && (ex == 0)) ex = EXIT_FAILURE ;
	return ex ;
}
/* end subroutine (main) */


/* local subroutines */

static int getpm(int argc,mainv argv,mainv names) noex {
	int		rs = SR_NOMSG ;
	if (argc > 0) {
	    cchar	*argz = argv[0] ;
	    if (argz[0]) {
		int	al = strlen(argz) ;
		cchar	*ap = argz ;
		while ((al > 1) && (ap[al-1] == '/')) al -= 1 ;
		if (int si ; (si = sirchr(ap,al,'/')) >= 0) {
		    ap += (si+1) ;
		    al -= (si+1) ;
		}
		if (al > 0) {
		    char	abuf[al+1] ;
		    strncpy(abuf,ap,al) ;
		    abuf[al] = '\0' ;
	            for (int i = 0 ; names[i] ; i += 1) {
		        cchar	*n = names[i] ;
		        bool	f = true ;
		        f = f && (abuf[0] == n[0]) ;
		        f = f && (strcmp(abuf,n) == 0) ;
		        if (f) {
	    		    rs = i ;
			    break ;
		        }
	           } /* end for */
		} /* end if (positive) */
	    } /* end if (non-empty argz) */
	} /* end if */
	return rs ;
}
/* end subroutine (getpm) */

static int utmp(bool fprint) noex {
	TM		ts ;
	UTMPX		*up ;
	int		rs = SR_OK ;
	int		c = 0 ;
	cchar		*fmt = "%-4s %-8s %-16s %s %5u %s\n" ;
	cchar		*tmt = "%Y%m%d-%H%M%S" ;
	char		tbuf[tlen+1] ;
	char		ibuf[utl_id+1] ;
	char		ubuf[utl_user+1] ;
	char		lbuf[utl_line+1] ;
	char		hbuf[utl_host+1] ;
	setutxent() ;
	while ((up = getutxent()) != nullptr) {
	   if (isourtype(up)) {
		const time_t	t = time_t(up->ut_tv.tv_sec) ;
		c += 1 ;
		if (fprint) {
		    cint	sid = up->ut_pid ;
		    cchar	*lp = up->ut_line ;
		    strncpy(ibuf,up->ut_id,utl_id) ;
		    strncpy(ubuf,up->ut_user,utl_user) ;
		    while (*lp == ' ') lp += 1 ;
		    strncpy(lbuf,lp,utl_line) ;
		    strncpy(hbuf,up->ut_host,utl_host) ;
		    localtime_r(&t,&ts) ;
		    strftime(tbuf,tlen,tmt,&ts) ;
	            printf(fmt,ibuf,ubuf,lbuf,tbuf,sid,hbuf) ;
		} /* end if (fprint) */
	   } /* end if (type match) */
	} /* end while */
	return (rs >= 0) ? c : rs ;
}
/* end subroutine (utmp) */

static int boottime() noex {
	TM		ts ;
	UTMPX		*up ;
	int		rs = SR_OK ;
	cchar		*tmt = "%Y%m%d-%H%M%S" ;
	char		tbuf[tlen+1] ;
	setutxent() ;
	while ((up = getutxent()) != nullptr) {
	   if (up->ut_type == BOOT_TIME) {
		const time_t	t = time_t(up->ut_tv.tv_sec) ;
		localtime_r(&t,&ts) ;
		strftime(tbuf,tlen,tmt,&ts) ;
	        cout << tbuf << eol ;
	   } /* end if (type match) */
	} /* end while */
	return rs ;
}
/* end subroutine (boottime) */

static int findsid(int pm) noex {
	cint		sid = getsid(0) ;	/* get our SID */
	int		rs = SR_NOTFOUND ;
	UTMPX		*up ;
	setutxent() ;
	while ((up = getutxent()) != nullptr) {
	   if ((up->ut_pid == sid) && isourtype(up)) {
	        rs = printutxval(pm,up) ;
	        if (rs < 0) break ;
	   }
	} /* end while */
	return rs ;
}
/* end subroutine (findsid) */

static int findstdin(int pm) noex {
	STAT		sb ;
	cint		tlen = TERMBUFLEN ;
	cint		fd = FD_STDIN ;
	int		rs ;
	bool		f = false ;
	if ((rs = u_fstat(fd,&sb)) >= 0) {
	    cchar	*devprefix = DEVPREFIX ;
	    char	tbuf[tlen+1] ;
	    if ((rs = uc_ttyname(fd,tbuf,tlen)) >= 0) {
		cint	n = strlen(devprefix) ;
		if (strncmp(tbuf,devprefix,n) == 0) {
		    UTMPX	ut{} ;
		    UTMPX	*up ;
		    cchar	*line = (tbuf+n) ;
		    strncpy(ut.ut_line,line,utl_line) ;
		    if ((up = getutxliner(&ut)) != nullptr) {
			f = true ;
			rs = printutxval(pm,up) ;
		    }
		} /* end if (matched) */
	    } else if (isNotTerm(rs)) {
		rs = SR_OK ;
	    } /* end if (ttyname) */
	} else if (isNotAccess(rs)) {
	    rs = SR_OK ;
	} /* end if (stat) */
	if ((rs >= 0) && (!f)) rs = SR_NOTFOUND ;
	return rs ;
}
/* end subroutine (findstdin) */

static int findenv(int pm) noex {
	int		rs = SR_OK ;
	bool		f = false ;
	for (auto const &vn : utmpvars) {
	    if (cchar *line ; (line = getenv(vn)) != nullptr) {
	        if (line[0]) {
	            UTMPX	ut{} ;
	            UTMPX	*up ;
	            strncpy(ut.ut_line,line,utl_line) ;
	            if ((up = getutxliner(&ut)) != nullptr) {
		        f = true ;
		        rs = printutxval(pm,up) ;
	            }
	        } /* end if (non-empty) */
	    } /* end if (getenv) */
	    if ((rs < 0) || f) break ;
	} /* end for (utmpvars) */
	if ((rs >= 0) && (!f)) rs = SR_NOTFOUND ;
	return rs ;
}
/* end subroutine (findenv) */

static int findstat(int pm) noex {
	UTMPX		*up ;
	static cint	sid = getsid(0) ;
	cint		tlen = TERMBUFLEN ;
	int		rs ;
	int		rs1 ;
	cchar		*devprefix = DEVPREFIX ;
	char		tbuf[tlen+1] ;
	bool		f = false ;
	if ((rs = sncpy(tbuf,tlen,devprefix)) >= 0) {
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
				    f = true ;
				    rs = printutxval(pm,up) ;
				}
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
		if (rs < 0) break ;
	    } /* end while */
	} /* end if (sncpy) */
	if ((rs >= 0) && (!f)) rs = SR_NOTFOUND ;
	return rs ;
}
/* end subroutine (findstat) */

static int printutxval(int pm,UTMPX *up) noex {
	cint		olen = HOSTLEN ;
	int		rs = SR_OK ;
	int		fl = 0 ;
	int		ml = 0 ;
	char		obuf[HOSTLEN+1] ;
	obuf[0] = '\0' ;
	switch (pm) {
	case progmode_utmpid:
	    fl = int(sizeof(up->ut_id)) ;
	    ml = min(olen,fl) ;
	    strtcpy(obuf,up->ut_id,ml) ;
	    break ;
	case progmode_utmpname:
	    fl = int(sizeof(up->ut_user)) ;
	    ml = min(olen,fl) ;
	    strtcpy(obuf,up->ut_user,ml) ;
	    break ;
	case progmode_utmpline:
	    fl = int(sizeof(up->ut_line)) ;
	    ml = min(olen,fl) ;
	    strtcpy(obuf,up->ut_line,ml) ;
	    break ;
	case progmode_utmphost:
	    fl = int(sizeof(up->ut_host)) ;
	    ml = min(olen,fl) ;
	    strtcpy(obuf,up->ut_host,ml) ;
	    break ;
	case progmode_utmpsid:
	    cout << up->ut_pid << '\n' ;
	    break ;
	case progmode_logged:
	    break ;
	default:
	    rs = SR_BUGCHECK ;
	    break ;
	} /* end switch */
	if ((rs >= 0) && fl && (pm != progmode_logged)) {
	    cout << obuf << '\n' ;
	}
	return rs ;
}
/* end subroutine (printutxval) */

static int sirchr(cchar *sp,int sl,int sch) noex {
	int		i ; /* used-afterwards */
	if (sl < 0) sl = strlen(sp) ;
	for (i = (sl-1) ; i >= 0 ; i -= 1) {
	    if (sp[i] == sch) break ;
	} /* end for */
	return i ;
}
/* end subroutine (sirchr) */

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
static char *strtcpy(char *dp,cchar *sp,int sl) noex {
	if (sl >= 0) {
	    dp = strncpy(dp,sp,sl) + sl ;
	    *dp = '\0' ;
	} else {
	    dp = nullptr ;
	}
	return dp ;
}
/* end subroutine (strtcpy) */


