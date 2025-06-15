/* sysdata_main SUPPORT (macsysdata) */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* print-out System-Data */
/* version %I% last-modified %G% */


/* revision history:

	= 2000-03-01, David A-D- Morano
	This subroutine was originally written.  This program is a
	hack for use on MacOS.

*/

/* Copyright © 2000 David A-D- Morano.  All rights reserved. */

/*******************************************************************************

	Name:
	macsysdata

	Description:
	Print some operating system (OS) data.

	Synopsis:
	$ macsysdata [<arg(s)>]

	Arguments:
	<arg(s)>	optional argument(s)

	Returns:
	==0		for normal operation success
	!=0		some error

*******************************************************************************/

#include	<envstandards.h>	/* ordered first to configure */
#include	<sys/types.h>
#include	<sys/sysctl.h>		/* "macOS" specific */
#include	<unistd.h>
#include	<fcntl.h>
#include	<climits>
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<cstring>		/* |strchr(3c)| */
#include	<string>
#include	<string_view>
#include	<iostream>
#include	<iomanip>
#include	<usystem.h>
#include	<sfx.h>
#include	<rmx.h>			/* |rmchr(3uc)| */
#include	<matstr.h>
#include	<strnul.hh>
#include	<mapex.h>
#include	<exitcodes.h>
#include	<localmisc.h>		/* |DIGBUFLEN| + |REALNAMELEN| */

import ulibvals ;

/* local defines */


/* imported namespaces */

using std::nullptr_t ;			/* type */
using std::string ;			/* type */
using std::string_view ;		/* type */
using std::istream ;			/* type */
using std::hex ;			/* subroutine */
using libu::uloadavgd ;			/* subroutine */
using libu::snloadavgd ;		/* subroutine */
using std::cout ;			/* variable */
using std::nothrow ;			/* constant */


/* local typedefs */

typedef string_view	strview ;


/* external subroutines */


/* external variables */


/* local structures */

namespace {
    enum proginfomems {
	proginfomem_start,
	proginfomem_finish,
	proginfomem_overlast
    } ;
    struct proginfo ;
    struct proginfo_co {
	proginfo	*op = nullptr ;
	int		w = -1 ;
	void operator () (proginfo *p,int m) noex {
	    op = p ;
	    w = m ;
	} ;
	int operator () (int = 0) noex ;
	operator int () noex {
	    return operator () (0) ;
	} ;
    } ; /* end struct (proginfo_co) */
    struct proginfo {
	friend		proginfo_co ;
	proginfo_co	start ;
	proginfo_co	finish ;
	proginfo_co	flistbegin ;
	proginfo_co	flistend ;
	mainv		argv ;
	mainv		envv ;
	cchar		*pn = nullptr ;
	int		argc ;
	int		pm = 0 ;
	bool		ffound = false ;
	proginfo(int c,mainv a,mainv e) noex : argc(c), argv(a), envv(e) { 
	    start	(this,proginfomem_start) ;
	    finish	(this,proginfomem_finish) ;
	} ;
	proginfo() noex : proginfo(0,nullptr,nullptr) { } ;
	void operator () (int c,mainv a,mainv e) noex {
	    argc = c ;
	    argv = a ;
	    envv = e ;
	} ;
	int uname() noex ;
	int output() noex ;
	int lax() noex ;
    private:
	int istart() noex ;
	int ifinish() noex ;
	int getpn(mainv) noex ;
    } ; /* end struct (proginfo) */
} /* end namespace */


/* forward references */


/* local variables */

enum progmodes {
	progmode_sysname,
	progmode_release,
	progmode_nodename,
	progmode_version,
	progmode_architecture,
	progmode_machine,
	progmode_platform,
	progmode_systype,
	progmode_nisdomain,
	progmode_sysuuid,
	progmode_symfile,
	progmode_hostid,
	progmode_lax,
	progmode_systime,
	progmode_unixtime,
	progmode_overlast
} ; /* end enum (progmodes) */

static constexpr cpcchar	prognames[] = {
	[progmode_sysname]	= "sysname",
	[progmode_release]	= "release",
	[progmode_nodename]	= "nodename",
	[progmode_version]	= "version",
	[progmode_architecture]	= "architecture",
	[progmode_machine]	= "machine",
	[progmode_platform]	= "platform",
	[progmode_systype]	= "systype",
	[progmode_nisdomain]	= "nisdomain",
	[progmode_sysuuid]	= "sysuuid",
	[progmode_symfile]	= "symfile",
	[progmode_hostid]	= "hostid",
	[progmode_lax]		= "lax",
	[progmode_systime]	= "systime",
	[progmode_unixtime]	= "unixtime",
	[progmode_overlast]	= nullptr
} ; /* end array (prognames) */

static constexpr MAPEX	mapexs[] = {
	{ SR_NOENT,	EX_NOUSER },
	{ SR_AGAIN,	EX_TEMPFAIL },
	{ SR_DEADLK,	EX_TEMPFAIL },
	{ SR_NOLCK,	EX_TEMPFAIL },
	{ SR_TXTBSY,	EX_TEMPFAIL },
	{ SR_ACCESS,	EX_NOPERM },
	{ SR_REMOTE,	EX_PROTOCOL },
	{ SR_NOSPC,	EX_TEMPFAIL },
	{ SR_INTR,	EX_INTR },
	{ SR_EXIT,	EX_TERM },
	{ SR_NOMSG,	EX_OSERR },
	{ SR_NOSYS,	EX_OSFILE },
	{ 0, 0 }
} ; /* end array (mapexs) */

static cint		maxpathlen = ulibval.maxpathlen ;

constexpr int		nlas = 3 ;	/* by long convention */


/* exported variables */


/* exported subroutines */

int main(int argc,mainv argv,mainv envv) noex {
	int		ex = EX_OK ;
	int		rs ;
	int		rs1 ;
	if (proginfo pi(argc,argv,envv) ; (rs = pi.start) >= 0) {
            switch (pi.pm) {
            case progmode_sysname:
            case progmode_release:
            case progmode_nodename:
            case progmode_version:
		rs = pi.uname() ;
		break ;
            case progmode_architecture:
            case progmode_machine:
            case progmode_platform:
            case progmode_systype:
            case progmode_nisdomain:
            case progmode_sysuuid:
            case progmode_symfile:
                rs = pi.output() ;
                break ;
	    case progmode_hostid:
		{
		    clong id = gethostid() ;
		    cout << ulong(id) << eol ;
		}
		break ;
	    case progmode_lax:
		rs = pi.lax() ;
		break ;
	    case progmode_systime:
	    case progmode_unixtime:
		{
		    const time_t	t = time(nullptr) ;
		    cout << ulong(t) << " - " ;
		    cout << hex << ulong(t) << eol ;
		}
		break ;
	    default:
		rs = SR_BUGCHECK ;
		break ;
            } /* end switch */
	    rs1 = pi.finish ;
	    if (rs >= 0) rs = rs1 ;
	} /* end if (proginfo) */
	if ((rs < 0) && (ex == EX_OK)) {
	    ex = mapex(mapexs,rs) ;
	}
	return ex ;
}
/* end subroutine (main) */


/* local subroutines */

int proginfo::istart() noex {
	int		rs ;
	if ((rs = getpn(prognames)) >= 0) {
	    rs = 0 ;
	} /* end if (proginfo::getpn) */
	return rs ;
}
/* end method (proginfo::istart) */

int proginfo::ifinish() noex {
	return SR_OK ;
}
/* end method (proginfo::ifinish) */

int proginfo::getpn(mainv names) noex {
	int		rs = SR_FAULT ;
	if (argv) {
	    rs = SR_NOMSG ;
	    if ((argc > 0) && argv[0]) {
		cchar	*arg0 = argv[0] ;
	        cchar	*bp{} ;
	        if (int bl ; (bl = sfbasename(arg0,-1,&bp)) > 0) {
		    if (cint pl = rmchr(bp,bl,'.') ; pl > 0) {
	                if ((pm = matstr(names,bp,pl)) >= 0) {
			    pn = names[pm] ;
		            rs = pm ;
	                }
		    } /* end if (non-zero positive progname) */
		} /* end if (have base-name) */
	    } /* end if (have first argument) */
	} /* end if (non-null) */
	return rs ;
}
/* end method (proginfo::getpn) */

static int printnodename(cchar *valp) noex {
	int		rs = SR_OK ;
	if (cchar *tp ; (tp = strchr(valp,'.')) != nullptr) {
	    string_view		s(valp,(tp - valp)) ;
	    cout << s << eol ;
	} else {
	    cout << valp << eol ;
	}
	return rs ;
}

int proginfo::uname() noex {
	int		rs ;
	if (UTSNAME uts ; (rs = u_uname(&uts)) >= 0) {
	    cchar	*valp = nullptr ;
            switch (pm) {
            case progmode_sysname:
		valp = uts.sysname ;
		break ;
            case progmode_release:
		valp = uts.release ;
		break ;
            case progmode_nodename:
		rs = printnodename(uts.nodename) ;
		break ;
            case progmode_version:
		valp = uts.version ;
		break ;
	    } /* end switch */
	    if (valp) {
		cout << valp << eol ;
	    }
	} /* end if (u_uname) */
	return rs ;
}
/* end subroutine (proginfo::uname) */

int proginfo::output() noex {
	int		rs = SR_OK ;
	cchar		*name = nullptr ;
	switch (pm) {
	case progmode_architecture:
	    name = "hw.machine" ;
	    break ;
	case progmode_machine:
	    name = "machdep.cpu.brand_string" ;
	    break ;
	case progmode_platform:
	    name = "hw.model" ;
	    break ;
	case progmode_systype:
	    name = "kern.ostype" ;
	    break ;
	case progmode_nisdomain:
	    name = "kern.nisdomainname" ;
	    break ;
	case progmode_sysuuid:
	    name = "kern.uuid" ;
	    break ;
	case progmode_symfile:
	    name = "kern.symfile" ;
	    break ;
	default:
	    rs = SR_BUGCHECK ;
	    break ;
	} /* end switch */
	if ((rs >= 0) && name) {
	    cnullptr	np{} ;
	    cint	olen = maxpathlen ;
	    rs = SR_NOMEM ;
	    if (char *obuf ; (obuf = new(nothrow) char[olen+1]) != np) {
		size_t	rsz = size_t(olen) ; /* <- return value also */
		if ((rs = sysctlbyname(name,obuf,&rsz,np,0z)) >= 0) {
		    cint	ol = int(rsz) ;
		    obuf[ol] = '\0' ;
		    cout << obuf << eol ;
		} else {
		    rs = (- errno) ;
		}
		delete [] obuf ;
	    } /* end if (new-char) */
	} /* end if (ok) */
	return rs ;
}
/* end subroutine (proginfo::output) */

int proginfo::lax() noex {
	int		rs ;
	if (double dla[nlas] ; (rs = uloadavgd(dla,nlas)) >= 0) {
	    cint	prec = 1 ;
	    cint	dlen = DIGBUFLEN ;
	    char	dbuf[DIGBUFLEN + 1] ;
	    if ((rs = snloadavgd(dbuf,dlen,prec,dla,nlas)) >= 0) {
		cout << dbuf << eol ;
	    }
	} /* end if (uloadavgd) */
	return rs ;
}
/* end subroutine (proginfo::lax) */

int proginfo_co::operator () (int) noex {
	int	rs = SR_BUGCHECK ;
	if (op) {
	    switch (w) {
	    case proginfomem_start:
	        rs = op->istart() ;
	        break ;
	    case proginfomem_finish:
	        rs = op->ifinish() ;
	        break ;
	    } /* end switch */
	} /* end if (non-null) */
	return rs ;
}
/* end method (proginfo_co::operator) */


