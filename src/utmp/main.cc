/* main (utmp) */
/* lang=C++20 */

/* print out the UTMP entry (including its ID field) */
/* version %I% last-modified %G% */


/* revision history:

	= 2023-12-15, David A.D. Morano
	This is originally written.

*/

/*******************************************************************************

	See the assciated README file ('README.txt') for why and what
	this program does.

*******************************************************************************/

#include	<sys/types.h>
#include	<sys/stat.h>		/* <- for |struct stat| */
#include	<unistd.h>		/* <- for |ttyname_r(3c)| */
#include	<utmpx.h>		/* <- for |getutxline(3c)| */
#include	<cstdlib>		/* <- for |EXIT_SUCCESS| */
#include	<cstring>		/* <- for |strncmp(3c)| */
#include	<cstdio>
#include	<string>
#include	<algorithm>
#include	<iostream>


/* local defines */

#ifndef	TIMEBUFLEN
#define	TIMEBUFLEN	40
#endif


/* local namespaces */

using std::cout ;
using std::string_view ;
using std::min ;


/* local typedefs */

typedef const int	cint ;
typedef const char	cchar ;


/* forward references */

static int findsid() noexcept ;
static int findline() noexcept ;

static bool isourtype(struct utmpx *up) noexcept {
	bool	f = false ;
	f = f || (up->ut_type == INIT_PROCESS) ;
	f = f || (up->ut_type == LOGIN_PROCESS) ;
	f = f || (up->ut_type == USER_PROCESS) ;
	return f ;
}


/* local variables */

constexpr int		lid = 4 ;
constexpr int		luser = 8 ;
constexpr int		lline = 16 ;
constexpr int		lhost = 27 ;
constexpr int		tlen = TIMEBUFLEN ;


/* exported variables */


/* exported subroutines */

int  main(int,const char *const *,const char *const *) {
	struct tm	ts ;
	struct utmpx	*up ;
	int		ex = EXIT_FAILURE ;
	cchar		*fmt = "%-4s %-8s %-16s %s %5u %s\n" ;
	cchar		*tmt = "%Y%m%d-%H%M%S" ;
	char		tbuf[tlen+1] ;
	char		ibuf[lid+1] ;
	char		ubuf[luser+1] ;
	char		lbuf[lline+1] ;
	char		hbuf[lhost+1] ;
	while ((up = getutxent()) != nullptr) {
	   ex = EXIT_SUCCESS ;
	   if (isourtype(up)) {
		const time_t	t = time_t(up->ut_tv.tv_sec) ;
		cint	sid = up->ut_pid ;
		cchar	*lp = up->ut_line ;
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
	return ex ;
}
/* end subroutine (main) */


/* local subroutines */


