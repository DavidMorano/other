/* termcolor_config HEADER */
/* charset=ISO8859-1 */
/* lang=C++20 (conformance reviewed) */

/* print a sample of the current terminal colors */
/* version %I% last-modified %G% */


/* revision history:

	= 2000-05-14, David A­D­ Morano
	Originally written for Rightcore Network Services.

*/

/* Copyright © 2000 David A­D­ Morano.  All rights reserved. */

#ifndef	TERMCOLORCONFIG_INCLUDE
#define	TERMCOLORCONFIG_INCLUDE


#define	VERSION		"0a"
#define	WHATINFO	"@(#)termcolor "
#define	BANNER		"Look"
#define	SEARCHNAME	"termcolor"
#define	VARPRNAME	"LOCAL"

#ifndef	PROGRAMROOT
#define	PROGRAMROOT	"/usr/add-on/local"
#endif

#define	VARPROGRAMROOT1	"TERMCOLOR_PROGRAMROOT"
#define	VARPROGRAMROOT2	VARPRNAME
#define	VARPROGRAMROOT3	"PROGRAMROOT"

#define	VARBANNER	"TERMCOLOR_BANNER"
#define	VARSEARCHNAME	"TERMCOLOR_NAME"
#define	VAROPTS		"TERMCOLOR_OPTS"
#define	VARWORDS	"TERMCOLOR_WORDS"
#define	VARFILEROOT	"TERMCOLOR_FILEROOT"
#define	VARAFNAME	"TERMCOLOR_AF"
#define	VAREFNAME	"TERMCOLOR_EF"
#define	VARLFNAME	"TERMCOLOR_LF"
#define	VARERRORFNAME	"TERMCOLOR_ERRORFILE"

#define	VARDEBUGFNAME	"TERMCOLOR_DEBUGFILE"
#define	VARDEBUGFD1	"TERMCOLOR_DEBUGFD"
#define	VARDEBUGFD2	"DEBUGFD"

#define	TMPDNAME	"/tmp"
#define	WORKDNAME	"/tmp"

#define	DEFINITFNAME	"/etc/default/init"
#define	DEFLOGFNAME	"/etc/default/login"
#define	NISDOMAINNAME	"/etc/defaultdomain"

#define	CONFIGFNAME	"conf"
#define	ENVFNAME	"environ"
#define	PATHSFNAME	"paths"
#define	HELPFNAME	"help"
#define	IPASSWDFNAME	"ipasswd"

#define	PIDFNAME	"run/termcolor"		/* mutex PID file */
#define	LOGFNAME	"var/log/termcolor"	/* activity log */
#define	LOCKFNAME	"spool/locks/termcolor"	/* lock mutex file */


#endif /* TERMCOLORCONFIG_INCLUDE */


