/* process SUPPORT */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* process a name */
/* version %I% last-modified %G% */

#define	CF_DEBUG	1

/* revision history:

	= 1996-03-01, David A­D­ Morano
	The subroutine was adapted from others programs that
	did similar types of functions.

*/

/* Copyright © 1996 David A­D­ Morano.  All rights reserved. */
/* Use is subject to license terms. */

/******************************************************************************

  	Description:
	This module just provides optional expansion of directories.
	The real work is done by the 'checkname' module.

******************************************************************************/

#include	<envstandards.h>	/* ordered first to configure */
#include	<sys/types.h>
#include	<sys/param.h>
#include	<sys/stat.h>
#include	<unistd.h>
#include	<fcntl.h>
#include	<signal.h>
#include	<termios.h>
#include	<ctime>
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>		/* |getenv(3c)| */
#include	<cstring>
#include	<clanguage.h>
#include	<usysbase.h>
#include	<baops.h>
#include	<localmisc.h>

#include	"config.h"
#include	"defs.h"


/* local defines */


/* external subroutines */


/* external variables */


/* local structures */


/* forward references */


/* local variables */


/* exported variables */


/* exported subroutines */

int process(pip,namespec,trunclen)
struct proginfo	*pip ;
char		namespec[] ;
ULONG		trunclen ;
{
	USTAT	sb ;

	ULONG	flen ;

	int	rs ;
	int	fd ;
	int	f_flen ;

	char	tmpfname[MAXPATHLEN + 1] ;
	char	*fname ;
	char	*cp ;


	if (namespec == NULL)
	    return SR_FAULT ;

	if (namespec[0] == '\0')
	    return SR_OK ;

#if	CF_DEBUG
	if (pip->debuglevel >= 3)
	    debugprintf("process: namespec=\"%s\"\n",namespec) ;
#endif

	flen = trunclen ;
	f_flen = pip->f.trunclen ;

	if (! pip->f.literal) {

#if	CF_DEBUG
	if (pip->debuglevel >= 3)
	    debugprintf("process: not literal\n") ;
#endif

	    fname = namespec ;
	    if ((cp = strchr(namespec,'=')) != NULL) {

	        fname = tmpfname ;
	        strwcpy(tmpfname,namespec,(cp - namespec)) ;

#if	CF_DEBUG
	if (pip->debuglevel >= 3)
	    debugprintf("process: fname=%s\n",fname) ;
#endif

	        rs = cfdecmfull((cp + 1),-1,&flen) ;

	        f_flen = (rs >= 0) ;

#if	CF_DEBUG
	if (pip->debuglevel >= 3)
	    debugprintf("process: f_flen=%d flen=%llu\n",f_flen,flen) ;
#endif

	    }

	} else
	    fname = namespec ;


/* do it */

	fd = FD_STDIN ;
	if (strcmp(fname,"-") != 0) {

		rs = u_open64(fname,O_WRONLY,0666) ;

		fd = rs ;

	} else
		rs = permf64(fd,-1,-1,NULL,W_OK) ;

#if	CF_DEBUG
	if (pip->debuglevel >= 3)
	    debugprintf("process: u_open()/permf() rs=%d\n",rs) ;
#endif

	if (rs < 0)
	    return rs ;


	rs = u_fstat64(fd,&sb) ;

#if	CF_DEBUG
	if (pip->debuglevel >= 3) {
	    debugprintf("process: u_fstat() rs=%d\n",rs) ;
	    debugprintf("process: isreg=%d\n",
			S_ISREG(sb.st_mode)) ;
	}
#endif

	if ((rs < 0) || (! S_ISREG(sb.st_mode)))
	    goto done ;

#if	CF_DEBUG
	if (pip->debuglevel >= 3)
	    debugprintf("process: filesize=%lld flen=%llu\n",
		sb.st_size,flen) ;
#endif

	if (pip->debuglevel > 0) {

		bprintf(pip->efp,"%s: len=%llu file=%s\n",
			pip->progname,flen,fname) ;

	}

	rs = 0 ;
	if (f_flen && (sb.st_size > flen)) {

#if	CF_DEBUG
	if (pip->debuglevel >= 3)
	    debugprintf("process: truncating to len=%llu\n",flen) ;
#endif

	    rs = uc_ftruncate64(fd,(off_t) flen) ;

#if	CF_DEBUG
	if (pip->debuglevel >= 3)
	    debugprintf("process: uc_ftruncate() rs=%d\n",rs) ;
#endif

	    rs = 1 ;

	} /* end if (file needed truncating) */

done:
	u_close64(fd) ;

	return rs ;
}
/* end subroutine (process) */



