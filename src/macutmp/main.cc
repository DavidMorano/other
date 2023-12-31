/* main (utmp) */
/* lang=C++20 */

/* print out the UTMP entry 'line' field if terminal session is registered */
/* version %I% last-modified %G% */


/* revision history:

	= 2023-12-15, David A.D. Morano
	This is originally written.

*/

/*******************************************************************************

	Names:
	utmp
	utmpid
	utmpname
	utmpline
	utmphost
	logid
	logname
	logline
	loghost
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
	of f*cked up operating system environments (like those found
	routinely on GNU-Linux based systems) that requires me to
	write programs like this in the first place.

*******************************************************************************/

#include	<sys/types.h>
#include	<sys/stat.h>		/* <- for |struct stat| */
#include	<unistd.h>		/* <- for |ttyname_r(3c)| */
#include	<utmpx.h>		/* <- for |getutxline(3c)| */
#include	<cstdlib>		/* <- for |EXIT_SUCCESS| */
#include	<cstdio>		/* <- for |printf(3c)| */
#include	<cstring>		/* <- |strncmp(3c)| + |strlen(3c)| */
#include	<iostream>
#include	<usysrets.h>		/* for "system-return" (SR) constants */
#include	<clanguage.h>		/* for |fallthrough| */


/* local defines */

#ifndef	FD_STDIN
#define	FD_STDIN	0
#endif

#ifndef	TIMEBUFLEN
#define	TIMEBUFLEN	40
#endif

#ifndef	TERMBUFLEN
#define	TERMBUFLEN	256
#endif

#ifndef	HOSTLEN
#define	HOSTLEN		1024		/* guessed that this is big enough! */
#endif

#ifndef	DEVPREFIX
#define	DEVPREFIX	"/dev/"
#endif


/* local namespaces */

using std::cout ;			/* variable */
using std::min ;			/* subroutine */


/* local typedefs */

typedef const int		cint ;
typedef const char		cchar ;
typedef const char *const	*mainv ;


/* forward references */

static int getpm(int,mainv,mainv) noexcept ;
static int utmp() noexcept ;
static int findsid(int) noexcept ;
static int findline(int) noexcept ;
static int printval(int,struct utmpx *) noexcept ;
static int sirchr(cchar *,int,int) noexcept ;

static bool isourtype(struct utmpx *up) noexcept {
	bool	f = false ;
	f = f || (up->ut_type == INIT_PROCESS) ;
	f = f || (up->ut_type == LOGIN_PROCESS) ;
	f = f || (up->ut_type == USER_PROCESS) ;
	return f ;
}


/* local variables */

enum progmodes {
	progmode_utmp,
	progmode_utmpid,
	progmode_utmpname,
	progmode_utmpline,
	progmode_utmphost,
	progmode_logid,
	progmode_logname,
	progmode_logline,
	progmode_loghost,
	progmode_logged,
	progmode_iverlast
} ;

constexpr const char *const	prognames[] = {
	"utmp",
	"utmpid",
	"utmpname",
	"utmpline",
	"utmphost",
	"logid",
	"logname",
	"logline",
	"loghost",
	"logged",
	nullptr
} ;

constexpr int		lid = 4 ;
constexpr int		luser = 8 ;
constexpr int		lline = 16 ;
constexpr int		lhost = 27 ;
constexpr int		tlen = TIMEBUFLEN ;


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
		rs = utmp() ;
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
	    default:
	        if ((rs = findsid(pm)) == rsn) {
	            rs = findline(pm) ;
	        }
		break ;
	    } /* end switch */
	} /* end if (getpm) */
	if ((rs < 0) && (ex == 0)) ex = EXIT_FAILURE ;
	return ex ;
}
/* end subroutine (main) */


/* local subroutines */

static int getpm(int argc,mainv argv,mainv names) noexcept {
	int		rs = SR_NOMSG ;
	if (argc > 0) {
	    cchar	*argz = argv[0] ;
	    if (argz[0]) {
		int	al = strlen(argz) ;
		int	si ;
		cchar	*ap = argz ;
		while ((al > 1) && (ap[al-1] == '/')) al -= 1 ;
		if ((si = sirchr(ap,al,'/')) >= 0) {
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

static int utmp() noexcept {
	struct tm	ts ;
	struct utmpx	*up ;
	int		rs = SR_OK ;
	int		c = 0 ;
	cchar		*fmt = "%-4s %-8s %-16s %s %5u %s\n" ;
	cchar		*tmt = "%Y%m%d-%H%M%S" ;
	char		tbuf[tlen+1] ;
	char		ibuf[lid+1] ;
	char		ubuf[luser+1] ;
	char		lbuf[lline+1] ;
	char		hbuf[lhost+1] ;
	while ((up = getutxent()) != nullptr) {
	   if (isourtype(up)) {
		const time_t	t = time_t(up->ut_tv.tv_sec) ;
		cint		sid = up->ut_pid ;
		cchar		*lp = up->ut_line ;
		c += 1 ;
		strncpy(ibuf,up->ut_id,lid) ;
		strncpy(ubuf,up->ut_user,luser) ;
		while (*lp == ' ') lp += 1 ;
		strncpy(lbuf,lp,lline) ;
		strncpy(hbuf,up->ut_host,lhost) ;
		localtime_r(&t,&ts) ;
		strftime(tbuf,tlen,tmt,&ts) ;
	        printf(fmt,ibuf,ubuf,lbuf,tbuf,sid,hbuf) ;
	   }
	} /* end while */
	return (rs >= 0) ? c : rs ;
}
/* end subroutine (utmp) */

static int findsid(int pm) noexcept {
	const int	sid = getsid(0) ;	/* get our SID */
	struct utmpx	*up ;
	int		rs = SR_NOENT ;
	while ((up = getutxent()) != nullptr) {
	   if ((up->ut_pid == sid) && isourtype(up)) {
	        rs = printval(pm,up) ;
		break ;
	   }
	} /* end while */
	return rs ;
}
/* end subroutine (findsid) */

static int findline(int pm) noexcept {
	struct stat	sb ;
	const int	tlen = TERMBUFLEN ;
	const int	fd = FD_STDIN ;
	int		rs ;
	bool		f = false ;
	if ((rs = fstat(fd,&sb)) >= 0) {
	    const char	*devprefix = DEVPREFIX ;
	    char	tbuf[tlen+1] ;
	    if ((rs = ttyname_r(fd,tbuf,tlen)) >= 0) {
		const int	n = strlen(devprefix) ;
		if (strncmp(tbuf,devprefix,n) == 0) {
		    struct utmpx	ut{} ;
		    struct utmpx	*up ;
		    const char		*line = (tbuf+n) ;
		    strncpy(ut.ut_line,line,sizeof(ut.ut_line)) ;
		    if ((up = getutxline(&ut)) != nullptr) {
			f = true ;
			rs = printval(pm,up) ;
		    }
		} /* end if (matched) */
	    } /* end if (ttyname) */
	} /* end if (stat) */
	if ((rs >= 0) && (!f)) rs = SR_NOTFOUND ;
	return rs ;
}
/* end subroutine (findline) */

static int printval(int pm,struct utmpx *up) noexcept {
	cint		olen = HOSTLEN ;
	int		rs = SR_OK ;
	int		fl, ml ;
	char		obuf[HOSTLEN+1] = {} ;
	switch (pm) {
	case progmode_utmpid:
	    fl = int(sizeof(up->ut_id)) ;
	    ml = min(olen,fl) ;
	    strncpy(obuf,up->ut_id,ml) ;
	    break ;
	case progmode_utmpname:
	    fl = int(sizeof(up->ut_user)) ;
	    ml = min(olen,fl) ;
	    strncpy(obuf,up->ut_user,ml) ;
	    break ;
	case progmode_utmpline:
	    fl = int(sizeof(up->ut_line)) ;
	    ml = min(olen,fl) ;
	    strncpy(obuf,up->ut_line,ml) ;
	    break ;
	case progmode_utmphost:
	    fl = int(sizeof(up->ut_host)) ;
	    ml = min(olen,fl) ;
	    strncpy(obuf,up->ut_host,ml) ;
	    break ;
	case progmode_logged:
	    break ;
	default:
	    rs = SR_BADFMT ;
	    break ;
	} /* end switch */
	if ((rs >= 0) && obuf[0] && (pm != progmode_logged)) {
	    cout << obuf << '\n' ;
	}
	return rs ;
}
/* end subroutine (printval) */

static int sirchr(cchar *sp,int sl,int sch) noexcept {
	int	i = 0 ;
	if (sl < 0) sl = strlen(sp) ;
	for (i = (sl-1) ; i >= 0 ; i -= 1) {
	    if (sp[i] == sch) break ;
	} /* end for */
	return i ;
}
/* end subroutine (sirchr) */


