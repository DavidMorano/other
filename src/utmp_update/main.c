/* main */


#define	DAM		1		/* David A�D� Morano */

#define	CF_NPRINTF	1		/* debug w/ 'nprintf(3nprintf)' */


/*
 * Copyright (c) 1999 by Sun Microsystems, Inc.
 * All rights reserved.
 */

#pragma ident	"@(#)utmp_update.c	1.16	99/02/18 SMI"

/*
 * utmp_update		- Update the /var/adm/utmpx file
 *
 *			As of on28, the utmp interface is obsolete,
 *			so we only handle updating the utmpx file now.
 *			The utmpx routines in libc "simulate" calls
 *			to manipulate utmp entries.
 *
 *			This program runs set uid root on behalf of
 *			non-privileged user programs.  Normal programs cannot
 *			write to /etc/utmpx. Non-root callers of pututxline
 *			will invoke this program to write the utmpx entry.
 */

/*
 * Header files
 */
#include	<sys/types.h>
#include	<sys/param.h>
#include	<sys/stat.h>
#include	<stropts.h>
#include	<unistd.h>
#include	<fcntl.h>
#include	<stdlib.h>
#include	<string.h>
#include	<ctype.h>
#include	<errno.h>
#include	<utmpx.h>
#include	<syslog.h>
#include	<pwd.h>
#include	<stdio.h>

/*
 * Invocation argument definitions
 */

#define	UTMPX_NARGS	14

/*
 * Return codes
 */
#define	NORMAL_EXIT		0
#define	BAD_ARGS		1
#define	PUTUTXLINE_FAILURE	2
#define	FORK_FAILURE		3
#define	SETSID_FAILURE		4
#define	ALREADY_DEAD		5
#define	ENTRY_NOTFOUND		6
#define	ILLEGAL_ARGUMENT	7

/*
 * Sizes
 */

#define	MAX_SYSLEN	256		/* From utmpx.h host length */
#define	BUF_SIZE	256



/*
 * Other defines
 */
#define	ROOT_UID	0

#define		DEBFNAME	"/tmp/utmpx.deb"

#ifndef	LOGIDLEN
#define	LOGIDLEN	15
#endif

#ifndef	NODENAMELEN
#define	NODENAMELEN	257
#endif



/* external subroutines */

extern int	getnodename(char *,int) ;
extern int	mklogid(char *,int,const char *,int) ;
extern int	nprintf(const char *,const char *,...) ;


/*
 * Debugging support
 */
#ifdef DEBUG
#define	dprintf	printf
#define	dprintf3 printf
static void display_args();
#else /* DEBUG */
#define	dprintf(x, y)
#define	dprintf3(w, x, y, z)
#endif

/*
 * Local functions
 */

static void load_utmpx_struct(struct utmpx *, char **);
static void check_utmpx(struct utmpx *);

static int	usage(int) ;
static int bad_hostname(char *, int);
static int hex2bin(char);

static int invalid_utmpx(struct utmpx *, struct utmpx *) ;
static int sameuser(struct utmpx *, struct utmpx *) ;
static int bad_line(char *);

static void check_id(char *, char *);


/* global variables */

char	logid[LOGIDLEN + 1] ;


/* exported subroutines */


int main(int argc, char *argv[])
{
	struct utmpx *rutmpx;
	struct utmpx entryx;

	pid_t	pid = getpid() ;

	int	rs1 ;
	int	v ;
	int error = 0;
	int num_args;
#ifdef	DEBUG
	int	debugger = 1;
	printf("%d\n", getpid());
	/*	Uncomment the following for attaching with dbx(1)	*/
	/* while  (debugger) ; */
	display_args(argc, argv);
#endif	/*	DEBUG	*/

	const char	*cp ;

	char	nodename[NODENAMELEN + 1] ;


	rs1 = getnodename(nodename,NODENAMELEN) ;

	cp = (rs1 >= 0) ? nodename : "pid" ;

	logid[0] = '\0' ;
	v = pid ;
	mklogid(logid,LOGIDLEN,cp,v) ;

#if	CF_NPRINTF
	nprintf(DEBFNAME,"%-16s uid=%u\n",logid,((uint) getuid())) ;
#endif

	/*
	 * We will always be called by pututxline, so simply
	 * verify the correct number of args
	 */

	if (argc != UTMPX_NARGS) {
		usage(1);
		exit(BAD_ARGS);
	}
	/*
	 * we should never be called by root the code in libc already
	 * updates the file for root so no need to do it here. This
	 * assumption simpilfies the rest of code since we no longer
	 * have to do special processing for the case when we are called
	 * by root
	 *
	 */
	if (getuid() == ROOT_UID) {
		usage(2);
		exit(ILLEGAL_ARGUMENT);
	}
	/*
	 * Search for matching entry by line name before put operation
	 * (scan over the whole file using getutxent(3C) to ensure
	 * that the line name is the same. We can not use getutline(3C)
	 * because that will return LOGIN_PROCESS and USER_PROCESS
	 * records. Also check that the entry is for either a dead
	 * process or a current process that is valid (see
	 * invalid_utmpx() for details of validation criteria).
	 */
	load_utmpx_struct(&entryx, argv);

	check_utmpx(&entryx);

	while ((rutmpx = getutxent()) != NULL) {

		if (strncmp(entryx.ut_line, rutmpx->ut_line,
		    sizeof(entryx.ut_line)) == 0) {

			if (rutmpx->ut_type == DEAD_PROCESS) {
				break;
			}

			if (rutmpx->ut_type == USER_PROCESS) {

#ifdef	DAM

			if ((strncmp(entryx.ut_id,rutmpx->ut_id,4) == 0) &&
				sameuser(&entryx,rutmpx))
				break ;

#else /* DAM */

				if (invalid_utmpx(&entryx, rutmpx)) {
					usage(3);
					exit(ILLEGAL_ARGUMENT);
				} else {
					break;
				}

#endif /* DAM */

			} /* end if (existing entry is alive) */

		} /* end if (line match) */

	} /* end while */

	if (pututxline(&entryx) == (struct utmpx *) NULL) {

#if	CF_NPRINTF
		nprintf(DEBFNAME,"%-16s put dailed\n",
			logid) ;
#endif

		exit(PUTUTXLINE_FAILURE);
	}

	exit(NORMAL_EXIT);
}

static int
hex2bin(char c)
{
	return (c > '9' ? 10 + (c-'A') : c - '0');
}


/*
 * load_utmpx_struct	- Load up the utmpx structure with information supplied
 *			as arguments in argv.
 */

static void
load_utmpx_struct(struct utmpx *entryx, char *argv[])
{
	char *user, *id, *line, *pid, *type, *term, *time_usec,
	    *exitstatus, *xtime, *session, *pad, *syslen, *host;
	int temp, i;

	(void) memset(entryx, 0, sizeof(struct utmpx));

	user 	= argv[1];
	id 	= argv[2];
	line 	= argv[3];
	pid 	= argv[4];
	type 	= argv[5];
	term 	= argv[6];
	exitstatus = argv[7];
	xtime 	= argv[8];
	time_usec = argv[9];
	session = argv[10];
	pad 	= argv[11];
	syslen	= argv[12];
	host	= argv[13];

	(void) strncpy(entryx->ut_user, user, sizeof(entryx->ut_user));
	(void) strncpy(entryx->ut_id, id, sizeof(entryx->ut_id));
	(void) strncpy(entryx->ut_line, line, sizeof(entryx->ut_line));

	(void) sscanf(pid, "%d", &temp);
	entryx->ut_pid = temp;

	(void) sscanf(type, "%d", &temp);
	entryx->ut_type = temp;

	(void) sscanf(term, "%d", &temp);
	entryx->ut_exit.e_termination = temp;

	(void) sscanf(exitstatus, "%d", &temp);
	entryx->ut_exit.e_exit = temp;
	/*
	 * Here's where we stamp the exit field of a USER_PROCESS
	 * record so that we know it was written by a normal user.
	 */

	if (entryx->ut_type == USER_PROCESS)
		setuserx(*entryx);

	(void) sscanf(xtime, "%d", &temp);
	entryx->ut_tv.tv_sec = temp;

	(void) sscanf(time_usec, "%d", &temp);
	entryx->ut_tv.tv_usec = temp;

	(void) sscanf(session, "%d", &temp);
	entryx->ut_session = temp;

	temp = strlen(pad);
	for (i = 0; i < temp; i += 2)
		entryx->pad[i>>2] = hex2bin(pad[i]) << 4 |
		    hex2bin(pad[i+1]);

	(void) sscanf(syslen, "%d", &temp);
	entryx->ut_syslen = temp;

	(void) strncpy(entryx->ut_host, host, sizeof(entryx->ut_host));
}

/*
 * usage	- There's no need to say more.  This program isn't supposed to
 *		be executed by normal users directly.
 */

static int
usage(int n)
{
	syslog(LOG_ERR, 
		"wrong number of arguments or invalid user (%u)\n",
		n);
}

/*
 * check_utmpx	- Verify the utmpx structure
 */

static void
check_utmpx(struct utmpx *entryx)
{
	char buf[BUF_SIZE];
	char *line = buf;
	struct passwd *pwd;
	int uid;
	int fd;
	char	*user;
	uid_t	ruid = getuid();

	(void) memset(buf, 0, BUF_SIZE);
	user = malloc(sizeof(entryx->ut_user) +1);
	(void) strncpy(user, entryx->ut_user, sizeof(entryx->ut_user));
	user[sizeof(entryx->ut_user)] = '\0';
	pwd = getpwnam(user);
	(void) free(user);

	(void) strncat(strcpy(line, "/dev/"), entryx->ut_line,
	    sizeof(line) - 6);

#if	CF_NPRINTF
	nprintf(DEBFNAME,"%-16s rec-user=%s PW=%s\n",
		logid,entryx->ut_user,
		((pwd != NULL) ? "non-NULL" : "NULL")) ;
#endif

	if (pwd != (struct passwd *) NULL) {

		uid = pwd->pw_uid;
		/*
		 * We nolonger permit the UID of the caller to be different
		 * than the UID to be written to the utmp file. This was thought
		 * necessary to allow the utmp file to be updated when
		 * logging out from an xterm(1) window after running
		 * exec login. Instead we now rely upon utmpd(1) to update
		 * the utmp file for us.
		 *
		 */

		if (ruid != uid) {

#if	CF_NPRINTF
		nprintf(DEBFNAME,"%-16s u uid-mismatch record_uid=%u\n",
			logid,uid) ;
#endif

			dprintf3("Bad uid: user %s  = %d uid = %d \n",
					entryx->ut_user, uid, getuid());
			exit(ILLEGAL_ARGUMENT);
		}

	} else if (entryx->ut_type != DEAD_PROCESS) {

#if	CF_NPRINTF
		nprintf(DEBFNAME,"%-16s rec-type is not D\n",
			logid) ;
#endif


		dprintf("Bad user name: %s \n", entryx->ut_user);
		exit(ILLEGAL_ARGUMENT);
	}

	/*
	 * Only USER_PROCESS and DEAD_PROCESS entries may be updated
	 */
	if (!(entryx->ut_type == USER_PROCESS ||
					entryx->ut_type == DEAD_PROCESS)) {

#if	CF_NPRINTF
		nprintf(DEBFNAME,"%-16s rec-type not U or D\n",
			logid) ;
#endif


		dprintf("Bad type type = %d\n", entryx->ut_type);
		exit(ILLEGAL_ARGUMENT);
	}

	/*
	 * Verify that the pid of the entry field is the same pid as our
	 * parent, who should be the guy writing the entry.  This is commented
	 * out for now because this restriction is overkill.
	 */
#ifdef	VERIFY_PID
	if (entryx->ut_type == USER_PROCESS && entryx->ut_pid != getppid()) {
		dprintf("Bad pid = %d\n", entryx->ut_pid);
		exit(ILLEGAL_ARGUMENT);
	}
#endif	/* VERIFY_PID */

	if (bad_line(line) == 1) {

#if	CF_NPRINTF
		nprintf(DEBFNAME,"%-16s bad line\n",
			logid) ;
#endif

		dprintf("Bad line = %s\n", line);
		exit(ILLEGAL_ARGUMENT);
	}

	if (bad_hostname(entryx->ut_host, entryx->ut_syslen) == 1) {

#if	CF_NPRINTF
		nprintf(DEBFNAME,"%-16s bad hostname\n",
			logid) ;
#endif

		dprintf("Bad hostname name = %s\n", entryx->ut_host);
		exit(ILLEGAL_ARGUMENT);
	}
	check_id(entryx->ut_id, entryx->ut_line);
}

/*
 * bad_hostname		- Previously returned an error if a non alpha numeric
 *			was in the host field, but now just clears those so
 *			cmdtool entries will work.
 */

static int
bad_hostname(char *name, int len)
{
	int i;

	if (len < 0 || len > MAX_SYSLEN)
		return (1);
	/*
	 * Scan for non-alpha numerics
	 */
	for (i = 0; i < len; i++)
		if (isprint(name[i]) == 0)
			name[i] = ' ';
	return (0);
}

/*
 * Workaround until the window system gets fixed.  Look for id's with
 * a '/' in them.  That means they are probably from libxview.
 * Then create a new id that is unique using the last 4 chars in the line.
 */

static void
check_id(char *id, char *line)
{
	char temp[4];
	int i, len;

	if (id[1] == '/' && id[2] == 's' && id[3] == 't') {
		len = strlen(line);
		if (len > 0)
			len--;
		for (i = 0; i < 4; i++)
			id[i] = len - i < 0 ? 0 : line[len-i];
	}
}


/*
 * The function invalid_utmpx() enforces the requirement that the record
 * being updating in the utmpx file can not have been created by login(1)
 * or friends. Also that the id and username of the record to be written match
 * those found in the utmpx file. We need this both for security and to ensure
 * that pututxline(3C) will NOT reposition the file pointer in the utmpx file,
 * so that the record is updated in place.
 *
 */
static int
invalid_utmpx(struct utmpx *eutmpx, struct utmpx *rutmpx)
{

#define	SUTMPX_ID	(sizeof(eutmpx->ut_id))
#define	SUTMPX_USER	(sizeof(eutmpx->ut_user))

	return (!nonuserx(*rutmpx) ||
		strncmp(eutmpx->ut_id, rutmpx->ut_id, SUTMPX_ID) != 0 ||
		strncmp(eutmpx->ut_user, rutmpx->ut_user, SUTMPX_USER) != 0);
}


static int sameuser(struct utmpx *eutmpx, struct utmpx *rutmpx)
{
	struct passwd	*pwp ;

	uid_t		ouid = getuid() ;


	if (strncmp(eutmpx->ut_user, rutmpx->ut_user, SUTMPX_USER) == 0)
		return 1 ;

	if (rutmpx->ut_user[0] == '\0')
		return 0 ;

	pwp = getpwnam(rutmpx->ut_user) ;

	return (pwp->pw_uid == ouid) ;
}
/* end subroutine (sameuser) */


static int
bad_line(char *line)
{
	struct ustat statbuf;
	int	fd;

	/*
	 * The line field must be a device file that we can write to,
	 * it should live under /dev which is enforced by requiring
	 * its name not to contain "../" and opening it as the user for
	 * writing.
	 */
	if (strstr(line, "../") != 0) {
		dprintf("Bad line = %s\n", line);
		return (1);
	}

	/*
	 * It has to be a tty. It can't be a bogus file, e.g. ../tmp/bogus.
	 */
	if (seteuid(getuid()) != 0)
		return (1);

	/*
	 * Check that the line refers to a character
	 * special device see bugid: 1136978
	 */
	if ((stat(line, &statbuf) < 0) || (statbuf.st_mode & S_IFCHR) == 0) {
		dprintf("Bad line (stat failed) (Not S_IFCHR) = %s\n", line);
		return (1);
	}

	/*
	 * We need to open the line without blocking so that it does not hang
	 */
	if ((fd = open(line, O_WRONLY|O_NOCTTY|O_NONBLOCK)) == -1) {
		dprintf("Bad line (Can't open/write) = %s\n", line);
		return (1);
	}

	/*
	 * Check that fd is a tty, if this fails all is not lost see below
	 */
	if (isatty(fd) == 1) {
		/*
		 * It really is a tty, so return success
		 */
		close(fd);
		if (seteuid(ROOT_UID) != 0)
			return (1);
		return (0);
	}

	/*
	 * Check that the line refers to a character
	 * special device see bugid: 1136978
	 */
	if ((fstat(fd, &statbuf) < 0) || (statbuf.st_mode & S_IFCHR) == 0) {
		dprintf("Bad line (fstat failed) (Not S_IFCHR) = %s\n", line);
		close(fd);
		return (1);
	}

	/*
	 * Check that the line refers to a streams device
	 */
	if (isastream(fd) != 1) {
		dprintf("Bad line (isastream failed) = %s\n", line);
		close(fd);
		return (1);
	}

	/*
	 * if isatty(3C) failed above we assume that the ptem module has
	 * been popped already and that caused the failure, so we push it
	 * and try again
	 */
	if (ioctl(fd, I_PUSH, "ptem") == -1) {
		dprintf("Bad line (I_PUSH of \"ptem\" failed) = %s\n", line);
		close(fd);
		return (1);
	}

	if (isatty(fd) != 1) {
		dprintf("Bad line (isatty failed) = %s\n", line);
		close(fd);
		return (1);
	}

	if (ioctl(fd, I_POP, 0) == -1) {
		dprintf("Bad line (I_POP of \"ptem\" failed) = %s\n", line);
		close(fd);
		return (1);
	}

	close(fd);

	if (seteuid(ROOT_UID) != 0)
		return (1);

	return (0);
}
/* end subroutine (bad_line) */


#ifdef	DEBUG

/*
 * display_args		- This code prints out invocation arguments
 *			This is helpful since the program is called with
 *			up to 15 argumments.
 */

static void
display_args(argc, argv)
	int argc;
	char **argv;
{
	int i = 0;

	while (argc--) {
		printf("Argument #%d = %s\n", i, argv[i]);
		i++;
	}
}

fputmpx(struct utmpx *rutmpx)
{
	printf("ut_user = \"%-32.32s\" \n", rutmpx->ut_user);
	printf("ut_id = \"%-4.4s\" \n", rutmpx->ut_id);
	printf("ut_line = \"%-32.32s\" \n", rutmpx->ut_line);
	printf("ut_pid = \"%d\" \n", rutmpx->ut_pid);
	printf("ut_type = \"%d\" \n", rutmpx->ut_type);
	printf("ut_exit.e_termination = \"%d\" \n",
					rutmpx->ut_exit.e_termination);
	printf("ut_exit.e_exit = \"%d\" \n", rutmpx->ut_exit.e_exit);
}

#endif /* DEBUG */



