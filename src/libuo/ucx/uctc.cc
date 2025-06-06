/* uctc SUPPORT */
/* encoding=ISO8859-1 */
/* lang=C++20 */

/* interface component for UNIX® library-3c */
/* terminal-control */
/* version %I% last-modified %G% */


/* revision history:

	= 2000-05-14, David A­D­ Morano
	Originally written for Rightcore Network Services.

*/

/* Copyright © 2000 David A­D­ Morano.  All rights reserved. */

#include	<envstandards.h>	/* MUST be first to configure */
#include	<sys/types.h>
#include	<termios.h>
#include	<unistd.h>
#include	<fcntl.h>
#include	<poll.h>
#include	<cerrno>
#include	<cstdlib>
#include	<usystem.h>
#include	<localmisc.h>

#include	"uctc.h"


/* local defines */


/* imported namespaces */


/* local typedefs */


/* external subroutines */


/* local structures */

namespace {
    struct uctc ;
    typedef int (uctc::*mem_m)(int) noex ;
    struct uctc {
	mem_m		m ;
	WINSIZE		*wsp ;
	TERMIOS		*tip ;
	int		cmd ;
	uctc() noex { } ;
	uctc(int a) noex : cmd(a) { } ;
	uctc(WINSIZE *p) noex : wsp(p) { } ;
	uctc(const TERMIOS *p,int c = 0) noex : cmd(c) { 
	    tip = cast_const<TERMIOS *>(p) ;
	} ;
	int operator () (int) noex ;
	int drain(int) noex ;
	int flow(int) noex ;
	int flush(int) noex ;
	int getpgrp(int) noex ;
	int getsid(int) noex ;
	int attrget(int) noex ;
	int attrset(int) noex ;
	int setpgrp(int) noex ;
    } ; /* end struct (uctc) */
}


/* forward references */


/* local variables */


/* exported variables */


/* exported subroutines */

int uc_tcdrain(int fd) noex {
	uctc		to ;
	to.m = &uctc::drain ;
	return to(fd) ;
}

int uc_tcflow(int fd,int cmd) noex {
	uctc		to(cmd) ;
	to.m = &uctc::flow ;
	return to(fd) ;
}

int uc_tcflush(int fd,int cmd) noex {
	uctc		to(cmd) ;
	to.m = &uctc::flush ;
	return to(fd) ;
}

int uc_tcattrget(int fd,TERMIOS *tip) noex {
	int		rs = SR_FAULT ;
	if (tip) {
	    uctc	to(tip) ;
	    to.m = &uctc::attrget ;
	    rs = to(fd) ;
	} /* end if (non-null) */
	return rs ;
}

int uc_tcgetpgrp(int fd) noex {
	uctc		to ;
	to.m = &uctc::getpgrp ;
	return to(fd) ;
}

int uc_tcgetsid(int fd) noex {
	uctc		to ;
	to.m = &uctc::getsid ;
	return to(fd) ;
}

int uc_tcattrset(int fd,int cmd,const TERMIOS *tip) noex {
	int		rs = SR_FAULT ;
	if (tip) {
	    uctc	to(tip,cmd) ;
	    to.m = &uctc::attrset ;
	    rs = to(fd) ;
	} /* end if (non-null) */
	return rs ;
}

int uc_tcsetpgrp(int fd,pid_t pgrp) noex {
	uctc		to(pgrp) ;
	to.m = &uctc::setpgrp ;
	return to(fd) ;
}


/* local subroutines */

int uctc::operator () (int fd) noex {
	int		to_nomem = utimeout[uto_nomem] ;
	int		rs ;
	bool		f_exit = false ;
	repeat {
	    if ((rs = (this->*m)(fd)) < 0) {
	        switch (rs) {
	        case SR_NOMEM:
		    if (to_nomem-- > 0) {
	                msleep(1000) ;
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
/* end method (tcuc::operator) */

int uctc::drain(int fd) noex {
	int		rs ;
	if ((rs = tcdrain(fd)) < 0) {
	    rs = (- errno) ;
	}
	return rs ;
}

int uctc::flow(int fd) noex {
	int		rs ;
	if ((rs = tcflow(fd,cmd)) < 0) {
	    rs = (- errno) ;
	}
	return rs ;
}
/* end method (uctc::flow) */

int uctc::flush(int fd) noex {
	int		rs ;
	if ((rs = tcflush(fd,cmd)) < 0) {
	    rs = (- errno) ;
	}
	return rs ;
}

int uctc::getpgrp(int fd) noex {
	int		rs ;
	if ((rs = tcgetpgrp(fd)) < 0) {
	    rs = (- errno) ;
	}
	return rs ;
}
/* end method (uctc::getpgrp) */

int uctc::getsid(int fd) noex {
	int		rs ;
	if ((rs = tcgetsid(fd)) < 0) {
	     rs = (- errno) ;
	}
	return rs ;
}
/* end method (uctc::getsid) */

int uctc::attrget(int fd) noex {
	int		rs ;
	if ((rs = tcgetattr(fd,tip)) < 0) {
	    rs = (- errno) ;
	}
	return rs ;
}
/* end method (uctc::attrget) */

int uctc::attrset(int fd) noex {
	int		rs ;
	if ((rs = tcsetattr(fd,cmd,tip)) < 0) {
	    rs = (- errno) ;
	}
	return rs ;
}
/* end method (uctc::attrset) */

int uctc::setpgrp(int fd) noex {
	int		rs ;
	if ((rs = tcsetpgrp(fd,cmd)) < 0) {
	    rs = (- errno) ;
	}
	return rs ;
}
/* end method (uctc::setpgrp) */


