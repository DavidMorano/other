/* appleterm_main SUPPORT (appleterm_title) */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* print out a character sequence to send a command to Apple-Terminal */
/* version %I% last-modified %G% */

#define	CF_DEBUG	0		/* debugging */

/* revision history:

	= 2023-12-15, David A-D- Morano
	This is originally written.

*/

/* Copyright © 2023 David A­D­ Morano.  All rights reserved. */
/* Use is subject to license terms. */

/*******************************************************************************

  	Description:
	See the assciated README file ('README.txt') for why and what
	this program does.

*******************************************************************************/

#include	<envstandards.h>	/* must be ordered first to configure */
#include	<cstddef>		/* CSTD */
#include	<cstdlib>		/* CSTD |EXIT_SUCCESS| */
#include	<cstring>		/* CSTD |strchr(3c)|  + |lenstr(3c)| */
#include	<string>		/* C++STD */
#include	<algorithm>		/* C++STD */
#include	<iostream>		/* C++STD */
#include	<clanguage.h>		/* LIBU */
#include	<usysbase.h>		/* LIBU */
#include	<getfdfile.h>		/* LIBU */
#include	<localmisc.h>		/* LIBU |eol| */
#include	<dprint.hh>		/* LIBU |DPRINTF(3u)| */

#pragma		GCC dependency		"mod/libutil.ccm"

import libutil ;			/* |lenstr(3u)| */
import ureserve ;			/* |isprintlatin(3u)| */

/* local defines */

#define	ENCBUFLEN	10

#ifndef	CF_DEBUG
#define	CF_DEBUG	1		/* debugging */
#endif


/* local namespaces */

using std::string ;			/* type */
using std::cout ;			/* variable */
using std::cerr ;			/* variable */


/* local typedefs */


/* external subrutines */


/* external variables */


/* local structures */


/* forward references */

local int	getwhich	(cchar *) noex ;
local int	form		(int,cchar *) noex ;
local int	perenc		(char *,int,int) noex ;
local char	hexchar		(int) noex ;
local bool	isnotbadchar	(int) noex ;


/* local variables */

constexpr char		badchars	[] = R"xx(%:\'"&)xx" ;
constexpr char		docprefix	[] = "file://" ;

enum types {
    	type_pwd,
	type_doc,
	type_tab,
	type_win,
	type_overlast
} ; /* end enum (types) */

namespace {
    struct typenames {
	cchar		*n[type_overlast + 1] ;
	consteval typenames() noex {
    	    n[type_pwd]		= "pwd" ;
    	    n[type_doc]		= "doc" ;
    	    n[type_tab]		= "tab" ;
    	    n[type_win]		= "win" ;
    	    n[type_overlast]	= nullptr ;
	} ; /* end ctor */
    } ; /* end struct (typenames) */
    struct typecodes {
	cchar		*n[type_overlast + 1] ;
	consteval typecodes() noex {
    	    n[type_pwd]		= "6" ;
    	    n[type_doc]		= "7" ;
    	    n[type_tab]		= "1" ;
    	    n[type_win]		= "2" ;
    	    n[type_overlast]	= nullptr ;
	} ; /* end ctor */
    } ; /* end struct (typecodes) */
} /* end namespace */

constexpr typenames	termname ;
constexpr typecodes	termcode ;
constexpr bool		f_debug	= CF_DEBUG ;


/* exported variables */


/* exported subroutines */

int main(int argc,con mainv argv,con mainv) {
	int		rs = SR_OK ;
	int		ex = EXIT_SUCCESS ;
	if (argc >= 3) ylikely {
	    rs = SR_BADRQC ;
	    if (cchar *tn = argv[1] ; tn[0]) ylikely {
		DPRINTF("tn=%s\n",tn) ;
	        if (cint w = getwhich(tn) ; w >= 0) {
		    DPRINTF("w=%d\n",w) ;
		    cchar *text = argv[2] ;
	            rs = form(w,text) ;
		} else {
		    cerr << "appleterm: " << "invalid type" << eol ;
		    rs = SR_INVALID ;
		} /* end if (getwhich) */
	    } /* end if (non-empty) */
	} else {
	    rs = SR_NOMSG ;
	} /* end if (had arguments) */
	if ((rs == EXIT_SUCCESS) && (rs < 0)) {
	    ex = EXIT_FAILURE ;
	} /* end if (error) */
	return ex ;
} /* end subroutine (main) */


/* local subroutines */

local int getwhich(cchar *tn) noex {
	int	type ; /* used-afterwards */
	bool	f = false ;
	DPRINTF("ent tn=%s\n",tn) ;
	for (type = 0 ; termname.n[type] ; type += 1) {
	    DPRINTF("test type=%d\n",type) ;
	    if ((f = (strcmp(termname.n[type],tn) == 0))) break ;
	} /* end for */
	DPRINTF("ret f=%u type=%d\n",f,type) ;
	return (f) ? type : -1 ;
} /* end subroutine (getwhich) */

local string mkprefix(int type) noex {
    	string	ps ;
    	cchar	*tc = termcode.n[type] ;
	ps += "\033]" ;
	ps += tc ;
	ps += ";" ;
	return ps ;
} /* end subroutine (mkprefix) */

local int form(int type,cchar *text) noex {
	cnullptr	np{} ;
    	int		rs = SR_NOMSG ;
	if (text[0]) ylikely {
	    string	s ;
	    cint	elen = ENCBUFLEN ;
	    char	ebuf[ENCBUFLEN+1] = {} ;
	    cchar	*fn = text ;
	    int		fl = lenstr(text) ;
	    rs = SR_OK ;
	    if ((type == type_pwd) || (type == type_doc)) {
		s += docprefix ;
		while ((fl > 1) && (fn[fl-1] == '/')) {
		    fl -= 1 ;
		} /* end while */
		for (int i = 0 ; (i < fl) && fn[i] ; i += 1) {
		    perenc(ebuf,elen,fn[i]) ;
		    s += ebuf ;
	        } /* end for (encoding) */
	    } else {
		for (int i = 0 ; (i < fl) && fn[i] ; i += 1) {
		    if (cint ch = fn[i] ; strchr(badchars,ch) == np) {
			s += char(ch) ;
		    }
	        } /* end for (encoding) */
	    } /* end if (type specific) */
	    if (rs >= 0) ylikely {
	        string	ws = mkprefix(type) + s + "\a" ;
	        cout << ws ;
		rs = SR_OK ;
	    } /* end if (ok) */
	} /* end if (valid) */
	return rs ;
} /* end subroutine (form) */

local int perenc(char *ebuf,int,int ch) noex {
	char		*ep = ebuf ;
	if (isprintlatin(ch) && isnotbadchar(ch)) {
	    *ep++ = char(ch) ;
	    *ep = '\0' ;
	} else {
	    *ep++ = '%' ;
	    *ep++ = hexchar((ch >> 4) & 0x0F) ;
	    *ep++ = hexchar((ch >> 0) & 0x0F) ;
	    *ep = '\0' ;
	} /* end if */
	return int(ep - ebuf) ;
} /* end subroutine (perenc) */

local char hexchar(int ch) noex {
	int	xch = 0 ;
	if ((ch >= 0) && (ch <= 9)) {
	    xch = ('0' + ch) ;
	} else {
	    xch = ('A' + (ch - 10)) ;
	} /* end */
	return charconv(xch) ;
} /* end subroutine (hexchar) */

local bool isnotbadchar(int ch) noex {
	return (strchr(badchars,ch) == nullptr) ;
} /* end subroutine (isnotbadchar) */


