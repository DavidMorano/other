/* main (conterm) */
/* lang=C++23 */

/* print out the session ID of the controlling terminal (if there is one) */
/* version %I% last-modified %G% */


/* revision history:

	= 2001-04-11, David A­D­ Morano
	Originally written for Rightcore Network Services.

*/

/* Copyright © 2001 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	This program tries to open the controlling terminal of the
	current process (if there is one).  If successful, we set
	our program-exit status and print out the session ID from
	the controlling terminal.

*******************************************************************************/

#include	<sys/types.h>
#include	<sys/stat.h>
#include	<termios.h>
#include	<unistd.h>
#include	<fcntl.h>
#include	<cstdlib>
#include	<cerrno>
#include	<iostream>


/* local defines */

#define	TO_NOMEM	5

#define	TERMDEV		"/dev/tty"

#ifndef	noex
#define	noex		noexcept
#endif

#ifndef	repeat
#define repeat		do
#define	until(cond)	while(!(cond))
#endif

#ifndef	SR_NOMEM
#define	SR_NOMEM	(-ENOMEM)
#endif

#ifndef	SR_INTR
#define	SR_INTR		(-EINTR)
#endif


/* namespaces */

using namespace		std ;

/* typedefs */

typedef const char	cchar ;
typedef struct stat	statblock ;


/* forward references */

static int uc_tcgetsid(int) noex ;


/* local constants */

constexpr const char	termdev[] = TERMDEV ;


/* exported subroutines */

int main(int argc,cchar **argv,cchar **) noexcept {
	int	ex = 1 ;
	int	rs ;
	if ((rs = open(termdev,O_RDONLY,0)) >= 0) {
	    const int	fd = rs ;
	    if ((rs = uc_tcgetsid(fd)) >= 0) {
		ex = 0 ;
	    } /* end if (uc_tcgetsid) */
	    close(fd) ;
	} /* end if (open) */
	return ex ;
}
/* end subroutine (main) */


/* local subroutines */

static int uc_tcgetsid(int fd) noex {
	int		to_nomem = TO_NOMEM ;
	int		rs ;
	bool		f_exit = false ;
	repeat {
	    if ((rs = tcgetsid(fd)) < 0) rs = (- errno) ;
	    if (rs < 0) {
	        switch (rs) {
	        case SR_NOMEM:
		    if (to_nomem-- > 0) {
	                sleep(1) ;
		    } else {
	                f_exit = true ;
		    }
		    break ;
	        case SR_INTR:
	            break ;
		default:
		    f_exit = true ;
		    break ;
	        } /* end switch */
	    } /* end if (error) */
	} until ((rs >= 0) || f_exit) ;
	return rs ;
}
/* end subroutine (uc_tcgetsid) */


