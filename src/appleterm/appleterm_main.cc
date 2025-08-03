/* appleterm_main SUPPORT (appleterm_title) */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* print out a character sequence to send a command to Apple-Terminal */
/* version %I% last-modified %G% */


/* revision history:

	= 2023-12-15, David A-D- Morano
	This is originally written.

*/

/*******************************************************************************

  	Description:
	See the assciated README file ('README.txt') for why and what
	this program does.

*******************************************************************************/

#include	<envstandards.h>	/* must be ordered first to configure */
#include	<sys/types.h>
#include	<cstddef>
#include	<cstdlib>		/* |EXIT_SUCCESS| */
#include	<cstring>		/* |strchr(3c)|  + |strlen(3c)| */
#include	<string>
#include	<cctype>		/* |isprint(3c)| */
#include	<algorithm>
#include	<iostream>


/* local defines */

#ifndef	FD_STDIN
#define	FD_STDIN	0
#endif

#ifndef	FD_STDOUT
#define	FD_STDOUT	1
#endif

#define	ENCBUFLEN	10

#ifndef	eol
#define	eol		'\n'
#endif

#define	noex		noexcept


/* local namespaces */

using std::string ;			/* type */
using std::cout ;			/* variable */
using std::cerr ;			/* variable */


/* local typedefs */

typedef const int		cint ;
typedef const char		cchar ;
typedef const char *const	cpcchar ;
typedef const char *const *	mainv ;


/* forward references */

static int	form(int,cchar *) noex ;
static int	perenc(char *,int,int) noex ;
static char	hexchar(int) noex ;
static bool	isnotbadchar(int) noex ;


/* local variables */

constexpr char		badchars[] = R"xx(%:\'"&)xx" ;
constexpr char		docprefix[] = "file://" ;

enum types {
    	type_pwd,
	type_doc,
	type_tab,
	type_win,
	type_overlast
} ; /* end enum (types) */

constexpr cpcchar	typenames[] = {
    	[type_pwd] = "pwd",
    	[type_doc] = "doc",
    	[type_tab] = "tab",
    	[type_win] = "win",
    	[type_overlast] = nullptr
} ; /* end array (types) */

constexpr cpcchar	typecodes[] = {
    	[type_pwd] = "6",
    	[type_doc] = "7",
    	[type_tab] = "1",
    	[type_win] = "2",
    	[type_overlast] = nullptr
} ; /* end array (typecodes) */


/* exported variables */


/* exported subroutines */

int main(int argc,mainv argv,mainv) {
	cint		fd = FD_STDIN ;
	int		rs = 0 ;
	int		ex = EXIT_SUCCESS ;
	if (argc >= 3) {
	    cchar *tn = argv[1] ;
	    bool f = false ;
	    int	 type ; /* used-afterwards */
	    rs = -1 ;
	    for (type = 0 ; typenames[type] ; type += 1) {
		f = (strcmp(typenames[type],tn) == 0) ;
		if (f) break ;
	    } /* end for */
	    if (f) {
		cchar	*text = argv[2] ;
	        rs = form(type,text) ;
	    }
	} else {
	    rs = -1 ;
	} /* end if (had arguments) */
	if ((rs == EXIT_SUCCESS) && (rs < 0)) ex = EXIT_FAILURE ;
	return ex ;
}
/* end subroutine (main) */

static string mkprefix(int type) noex {
    	string ps ;
    	cchar	*tc = typecodes[type] ;
	ps += "\033]" ;
	ps += tc ;
	ps += ";" ;
	return ps ;
} /* end subroutine (mkprefix) */

static int form(int type,cchar *text) noex {
    	const decltype(nullptr)	np{} ;
    	int		rs = -1 ;
	if (text[0]) {
	    string	s ;
	    cint	elen = ENCBUFLEN ;
	    char	ebuf[ENCBUFLEN+1] = {} ;
	    cchar	*fn = text ;
	    int		fl = strlen(text) ;
	    if ((type == type_pwd) || (type == type_doc)) {
		s += docprefix ;
		while ((fl > 1) && (fn[fl-1] == '/')) {
		    fl -= 1 ;
		}
		for (int i = 0 ; (i < fl) && fn[i] ; i += 1) {
		    perenc(ebuf,elen,fn[i]) ;
		    s += ebuf ;
	        } /* end for (encoding) */
	    } else {
		for (int i = 0 ; (i < fl) && fn[i] ; i += 1) {
		    cint ch = fn[i] ;
		    if (strchr(badchars,ch) == np) {
			s += char(ch) ;
		    }
	        } /* end for (encoding) */
	    } /* end if (type specific) */
	    {
	        string	ws = mkprefix(type) + s + "\a" ;
	        cout << ws ;
		rs = 0 ;
	    }
	} /* end if (valid) */
	return rs ;
}
/* end subroutine (form) */


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
	return int(ep - ebuf) ;
}
/* end subroutine (perenc) */

static char hexchar(int ch) noex {
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


