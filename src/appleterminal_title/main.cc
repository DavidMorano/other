/* main (appleterminal_title) */
/* lang=C++20 */

/* print out a character sequence to send a command to Apple-Terminal */
/* version %I% last-modified %G% */


/* revision history:

	= 2023-12-15, David A-D- Morano
	This is originally written.

*/

/*******************************************************************************

	See the assciated README file ('README.txt') for why and what
	this program does.

*******************************************************************************/

#include	<sys/types.h>
#include	<sys/stat.h>		/* <- for |USTAT| */
#include	<unistd.h>		/* <- for |ttyname_r(3c)| */
#include	<utmpx.h>		/* <- for |getutxline(3c)| */
#include	<cstdlib>		/* <- for |EXIT_SUCCESS| */
#include	<cstring>		/* <- for |strpbrk(3c)| */
#include	<cctype>
#include	<algorithm>
#include	<string>
#include	<iostream>


/* local defines */

#ifndef	FD_STDIN
#define	FD_STDIN	0
#endif

#ifndef	FD_STDOUT
#define	FD_STDOUT	1
#endif

#define	ENCBUFLEN	10

#define	noex		noexcept


/* local namespaces */

using std::cout ;
using std::string ;


/* local typedefs */

typedef const int		cint ;
typedef const char		cchar ;
typedef const char *const *	mainv ;


/* forward references */

static int hexchar(int) noex ;
static int perenc(char *,int,int) noex ;
static bool isnotbadchar(int) noex ;


/* local variables */

static constexpr char		badchars[] = R"xx(%:\'"&)xx" ;


/* exported variables */


/* exported subroutines */

int main(int argc,mainv argv,mainv) {
	cint		fd = FD_STDIN ;
	int		ex = EXIT_FAILURE ;
	if (argc >= 2) {
	    string	s("\033]7;") ;
	    cchar	prefix[] = "file://" ;
	    cchar	postfix[] = "\a" ;
	    cchar	*fn = argv[1] ;
	    int		vl = 0 ;
	    s += prefix ;
	    if (argc >= 3) {
		cchar	*hn = argv[2] ;
		if (hn[0]) {
	            s += hn ;
		    vl = strlen(hn) ;
		}
	    } /* end if (possible hostname) */
	    if (fn) {
		if (fn[0]) {
		    cint	elen = ENCBUFLEN ;
		    char	ebuf[ENCBUFLEN+1] = {} ;
		    int		fl = strlen(fn) ;
		    while ((fl > 1) && (fn[fl-1] == '/')) fl -= 1 ;
	            for (int i = 0 ; (i < fl) && fn[i] ; i += 1) {
		        vl += perenc(ebuf,elen,fn[i]) ;
	                s += ebuf ;
	            }
		} /* end if (non-zero positive) */
	    } /* end if (possible filename) */
	    s += postfix ;
	    if (vl > 0) {
		cchar	*sp = s.c_str() ;
		cint	sl = strlen(sp) ;
	        write(fd,sp,sl) ;
		ex = EXIT_SUCCESS ;
	    }
	} /* end if (at least one argument) */
	return ex ;
}
/* end subroutine (main) */


/* local subroutines */

static int perenc(char *ebuf,int,int ch) noex {
	char		*ep = ebuf ;
	if (isprint(ch) && isnotbadchar(ch)) {
	    *ep++ = ch ;
	    *ep = '\0' ;
	} else {
	    *ep++ = '%' ;
	    *ep++ = hexchar((ch >> 4) & 0x0F) ;
	    *ep++ = hexchar((ch >> 0) & 0x0F) ;
	    *ep = '\0' ;
	}
	return (ep - ebuf) ;
}
/* end subroutine (perenc) */

static int hexchar(int ch) noex {
	int	xch = 0 ;
	if ((ch >= 0) && (ch <= 9)) {
	    xch = ('0' + ch) ;
	} else {
	    xch = ('A' + (ch - 10)) ;
	}
	return xch ;
}
/* end subroutine (hexchar) */

static bool isnotbadchar(int ch) noex {
	return (strchr(badchars,ch) == nullptr) ;
}


