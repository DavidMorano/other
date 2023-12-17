/* main (utmpline) */
/* lang=C++20 */

/* print out the UTMP entry 'line' field if terminal session is registered */
/* version %I% last-modified %G% */


/* revision history:

	= 2023-12-15, David A.D. Morano
	This is originally written.

*/

/*******************************************************************************

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
#include	<cstring>		/* <- for |strncmp(3c)| */
#include	<iostream>

#ifndef	FD_STDIN
#define	FD_STDIN	0
#endif

#ifndef	TERMBUFLEN
#define	TERMBUFLEN	256
#endif

#ifndef	DEVPREFIX
#define	DEVPREFIX	"/dev/"
#endif

using std::cout ;
using std::string_view ;

static int findsid() noexcept ;
static int findline() noexcept ;

static bool isourtype(struct utmpx *up) noexcept {
	bool	f = false ;
	f = f || (up->ut_type == INIT_PROCESS) ;
	f = f || (up->ut_type == LOGIN_PROCESS) ;
	f = f || (up->ut_type == USER_PROCESS) ;
	return f ;
}

int  main(int,const char *const *,const char *const *) {
	int		ex ;
	if ((ex = findsid()) != EXIT_SUCCESS) {
	    ex = findline() ;
	}
	return ex ;
}
/* end subroutine (main) */


/* local subroutines */

static int findsid() noexcept {
	const int	sid = getsid(0) ;	/* get our SID */
	struct utmpx	*up ;
	int		ex = EXIT_FAILURE ;
	while ((up = getutxent()) != nullptr) {
	   if ((up->ut_pid == sid) && isourtype(up)) {
		string_view	sv(up->ut_line,sizeof(up->ut_line)) ;
		ex = EXIT_SUCCESS ;
		cout << sv << '\n' ;
		break ;
	   }
	} /* end while */
	return ex ;
}
/* end subroutine (findsid) */

static int findline() noexcept {
	struct stat	sb ;
	const int	tlen = TERMBUFLEN ;
	const int	fd = FD_STDIN ;
	int		rs ;
	int		ex = EXIT_FAILURE ;
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
		        cout << line << '\n' ;
		        ex = EXIT_SUCCESS ;
		    }
		} /* end if (matched) */
	    } /* end if (ttyname) */
	} /* end if (stat) */
	return ex ;
}
/* end subroutine (findline) */


