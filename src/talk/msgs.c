/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)msgs.c	1.3	94/10/04 SMI"	/* SVr4.0 1.2	*/

/*
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * 		PROPRIETARY NOTICE (Combined)
 *
 * This source code is unpublished proprietary information
 * constituting, or derived under license from AT&T's UNIX(r) System V.
 * In addition, portions of such source code were derived from Berkeley
 * 4.3 BSD under license from the Regents of the University of
 * California.
 *
 *
 *
 * 		Copyright Notice
 *
 * Notice of copyright on this source code product does not indicate
 * publication.
 *
 * 	(c) 1986,1987,1988.1989  Sun Microsystems, Inc
 * 	(c) 1983,1984,1985,1986,1987,1988,1989  AT&T.
 *		All rights reserved.
 *
 */


/*
 * a package to display what is happening every MSG_INTERVAL seconds
 * if we are slow connecting.
 */

#include <sys/time.h>
#include <signal.h>
#include <stdio.h>

#include "talk.h"

#ifdef SYSV
#define	signal(s, f)	sigset(s, f)
#endif /* SYSV */

#define	MSG_INTERVAL 4
#define	LONG_TIME 100000

char *current_state;
int current_line = 0;

static struct itimerval itimer;
static struct timeval wait = { MSG_INTERVAL, 0};


static void
disp_msg()
{
	message(current_state);
}

void
start_msgs()
{
	message(current_state);
	signal(SIGALRM, (void (*)())disp_msg);
	itimer.it_value = itimer.it_interval = wait;
	setitimer(ITIMER_REAL, &itimer, (struct itimerval *)0);
}

void
end_msgs()
{
	signal(SIGALRM, SIG_IGN);
	timerclear(&itimer.it_value);
	timerclear(&itimer.it_interval);
	setitimer(ITIMER_REAL, &itimer, (struct itimerval *)0);
}
