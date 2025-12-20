/* files_main SUPPORT (files) */
/* charset=ISO8859-1 */
/* lang=C++20 (conformance reviewed) */

/* enumerate filenames */
/* version %I% last-modified %G% */

#define	CF_DEBUG	0		/* debug */
#define	CF_FILELINES	1		/* use |filelines()| */

/* revision history:

	= 2017-03-21, David A-D- Morano
	This subroutine was originally written.

*/

/* Copyright © 2017 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Name:
	files_main

	Description:
	This program enumerates directories and files.

	Synopsis:
	main(int argc,mainv argv,mainv argv)

	Arguments:

	Returns:
	==0		for normal operation success (EX_OK)
	!=0		some error (! EX_OK)

*******************************************************************************/

#include	<envstandards.h>	/* ordered first to configure */
#include	<sys/stat.h>
#include	<unistd.h>
#include	<fcntl.h>
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<cstdarg>
#include	<cstdio>
#include	<new>			/* |nothrow(3c++)| */
#include	<string>
#include	<string_view>
#include	<filesystem>
#include	<iostream>
#include	<usystem.h>
#include	<getfdfile.h>		/* |FD_STDERR| */
#include	<varnames.hh>
#include	<strn.h>
#include	<strw.h>		/* |strwcmp(3uc)| */
#include	<sfx.h>			/* |sfbasename(3uc)| + |sfext(3uc)| */
#include	<six.h>			/* |sisub(3uc)| */
#include	<rmx.h>
#include	<strwcpy.h>
#include	<strnul.hh>
#include	<ccfile.hh>
#include	<readln.hh>
#include	<filetype.h>
#include	<matstr.h>
#include	<mkchar.h>
#include	<isnot.h>
#include	<mapex.h>
#include	<exitcodes.h>
#include	<localmisc.h>
#include	<debprintf.h>		/* |DEBPRINTF| */

#pragma		GCC dependency	"mod/libutil.ccm"
#pragma		GCC dependency	"mod/ulibvals.ccm"
#pragma		GCC dependency	"mod/ureserve.ccm"
#pragma		GCC dependency	"mod/strfilter.ccm"
#pragma		GCC dependency	"mod/argmgr.ccm"
#pragma		GCC dependency	"mod/fonce.ccm"
#pragma		GCC dependency	"mod/sif.ccm"
#pragma		GCC dependency	"mod/bitop.ccm"
#pragma		GCC dependency	"mod/tardir.ccm"
#pragma		GCC dependency	"mod/filerec.ccm"
#pragma		GCC dependency	"mod/modproc.ccm"
#pragma		GCC dependency	"mod/files_utils.ccm"
#pragma		GCC dependency	"mod/debug.ccm"

import libutil ;			/* |lenstr(3u)| + |getlenstr(3u)| */
import ulibvals ;
import ureserve ;
import strfilter ;
import argmgr ;
import fonce ;
import sif ;
import bitop ;
import debug ;				/* |debprintf(3uc)| */
import tardir ;
import filerec ;
import modproc ;
import files_utils ;

/* local defines */

#define	NENTS		1000

#ifndef	CF_DEBUG
#define	CF_DEBUG	0		/* debug */
#endif
#ifndef	CF_FILELINES
#define	CF_FILELINES	1		/* use |filelines()| */
#endif


/* imported namespaces */

namespace fs = std::filesystem ;

using fs::recursive_directory_iterator ; /* type */
using std::string ;			/* type */
using std::string_view ;		/* type */
using std::istream ;			/* type */
using libu::snprintf ;			/* subroutine */
using libu::snvprintf ;			/* subroutine */
using libu::umem ;			/* variable */
using std::cin ;			/* variable */
using std::cout ;			/* variable */
using std::cerr ;			/* variable */
using std::nothrow ;			/* constant */


/* local typedefs */

typedef string_view			strview ;
typedef recursive_directory_iterator	rdi ;


/* external subroutines */


/* external variables */


/* local structures */

namespace {
    enum proginfomems {
	proginfomem_start,
	proginfomem_finish,
	proginfomem_argbegin,
	proginfomem_argend,
	proginfomem_flistbegin,
	proginfomem_flistend,
	proginfomem_overlast
    } ; /* end enum (proginfomems) */
    struct proginfo ;
    struct proginfo_fl {
	uint		uniqfile:1 ;		/* 'u' arg-opt */
	uint		uniqdir:1 ;		/* 'ud' arg-opt */
	uint		followarg:1 ;		/* 'H' arg-opt */
	uint		followall:1 ;		/* 'L' arg-opt */
	uint		exts:1 ;		/* extensions */
	uint		dirs:1 ;		/* target-directories */
	uint		afs:1 ;			/* argument-files */
	uint		afsread:1 ;		/* argument-files-read */
	uint		argfile:1 ;		/* have an arg-file */
	uint		version:1 ;
	uint		help:1 ;
	uint		quiet:1 ;
	uint		verbose:1 ;
	uint		debug:1 ;
	uint		suffix:1 ;		/* have a suffix */
	uint		modes:1 ;		/* have a file-type */
	uint		seens:1 ;
	uint		recs:1 ;
	uint		mods:1 ;
	uint		ot:2 ;			/* type-out */
    } ; /* end struct (proginfo_fl) */
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
	proginfo_co	argbegin ;
	proginfo_co	argend ;
	proginfo_co	flistbegin ;
	proginfo_co	flistend ;
	proginfo_fl	fl{} ;
	strfilter	exts ;		/* extensions */
	tardir		dirs ;		/* target-directories */
	fonce		seen ;
	filerec		afs ;		/* argument-files */
	filerec		recs ;
	modproc		mods ;
	string		pnstr ;
	mainv		argv ;
	mainv		envv ;
	cchar		*pn = nullptr ;
	char		*lbuf = nullptr ;
	FILE		*ofp ;		/* output-file-pointer */
	int		argc ;
	int		pm = -1 ;
	int		debuglevel = 0 ;
	int		llen = 0 ;
	int		lines = 0 ;
	ushort		modes = 0 ;	/* file-modes (matched) */
	bool		fexit = false ;
	proginfo(int c,mainv a,mainv e) noex : argc(c), argv(a), envv(e) { 
	    start	(this,proginfomem_start) ;
	    finish	(this,proginfomem_finish) ;
	    argbegin	(this,proginfomem_argbegin) ;
	    argend	(this,proginfomem_argend) ;
	    flistbegin	(this,proginfomem_flistbegin) ;
	    flistend	(this,proginfomem_flistend) ;
	    fl.verbose = true ;
	    ofp = stdout ;
	} ; /* end ctor */
	proginfo() noex : proginfo(0,nullptr,nullptr) { } ;
	void operator () (int c,mainv a,mainv e) noex {
	    argc = c ;
	    argv = a ;
	    envv = e ;
	} ;
	int fileuniq(custat *) noex ;
	int argproc() noex ;
	int args(argmgr *) noex ;
	int argoptstr(argmgr *,int) noex ;
	int argoptlong(argmgr *,cchar *,int) noex ;
	int argoptchr(argmgr *,cchar *,int) noex ;
	int argpm(argmgr *) noex ;
	int argfile(argmgr *) noex ;
	int argfileread() noex ;
	int argsuf(argmgr *) noex ;
	int argtype(argmgr *) noex ;
	int argtardir(argmgr *) noex ;
	int argstat(cchar *,ustat *) noex ;
	int argprocer(argmgr *) noex ;
	int argreadin() noex ;
	int argprocname(cchar *,int = -1) noex ;
	int argdebug(argmgr *) noex ;
	int preamble() noex ;
	int process_pmbegin() noex ;
	int process_pmend() noex ;
	int procfile_list(custat *,cchar *,int = -1) noex ;
	int procfile_lc(custat *,cchar *,int = -1) noex ;
	int procfile_mods(custat *,cchar *,int = -1) noex ;
	int procdir(custat *,cchar *,int = -1) noex ;
	int procdirsubs(custat *,cchar *,int = -1) noex ;
	int procdirself(custat *,cchar *,int = -1) noex ;
	int procent(custat *,cchar *,int = -1) noex ;
	int procfile(custat *,cchar *,int = -1) noex ;
	int sufadd(cchar *,int = -1) noex ;
	int sufready() noex ;
	int sufhave(cchar *,int) noex ;
	int modeadd(cchar *,int) noex ;
	int modehave(custat *) noex ;
	int tardirbegin() noex ;
	int tardirend() noex ;
	int tardiravail() noex ;
	int tardiradd(cchar *,int) noex ;
	int typeout(cchar *,int) noex ;
	int printf(cchar *,...) noex ;
    private:
	int istart() noex ;
	int ifinish() noex ;
	int getpnames(mainv,cc *,int) noex ;
	int getpn(mainv) noex ;
	int iargbegin() noex ;
	int iargend() noex ;
	int iflistbegin() noex ;
	int iflistend() noex ;
    } ; /* end struct (proginfo) */
} /* end namespace */


/* forward references */


/* local variables */

enum progmodes {
	progmode_files,
	progmode_filelines,
	progmode_filesyner,
	progmode_filelinker,
	progmode_depmods,
	progmode_overlast
} ;

constexpr cpcchar	prognames[] = {
	[progmode_files]	= "files",
	[progmode_filelines]	= "filelines",
	[progmode_filesyner]	= "filesyncer",
	[progmode_filelinker]	= "filelinker",
	[progmode_depmods]	= "depmods",
	[progmode_overlast]	= nullptr
} ;

enum argopts {
    	argopt_debug,
    	argopt_help,
    	argopt_version,
    	argopt_pm,
    	argopt_sn,
    	argopt_ef,
    	argopt_of,
    	argopt_af,
    	argopt_ud,
    	argopt_ot,
	argopt_overlast
} ;

constexpr cpcchar	argopts[] = {
    	[argopt_debug]		= "DEBUG",
    	[argopt_help]		= "HELP",
    	[argopt_version]	= "VERSION",
    	[argopt_pm]		= "pm",
    	[argopt_sn]		= "sn",
    	[argopt_ef]		= "ef",
    	[argopt_of]		= "of",
    	[argopt_af]		= "af",
    	[argopt_ud]		= "ud",
    	[argopt_ot]		= "ot",
	[argopt_overlast]	= nullptr
} ;

enum argoptlongs {
    	argoptlong_version,
	argoptlong_overlast
} ;

constexpr cpcchar	argoptlongs[] = {
    	[argoptlong_version]	= "version",
	[argoptlong_overlast]	= nullptr
} ;

constexpr MAPEX		mapexs[] = {
    	{ SR_INVALID,	EX_USAGE },
	{ SR_NOENT,	EX_NOUSER },
	{ SR_AGAIN,	EX_TEMPFAIL },
	{ SR_DEADLK,	EX_TEMPFAIL },
	{ SR_NOLCK,	EX_TEMPFAIL },
	{ SR_TXTBSY,	EX_TEMPFAIL },
	{ SR_ACCESS,	EX_NOPERM },
	{ SR_PERM,	EX_NOPERM },
	{ SR_REMOTE,	EX_PROTOCOL },
	{ SR_NOSPC,	EX_TEMPFAIL },
	{ SR_INTR,	EX_INTR },
	{ SR_EXIT,	EX_TERM },
	{ SR_NOMSG,	EX_OSERR },
	{ SR_NOSYS,	EX_OSFILE },
	{ 0, 0 }
} ; /* end array (mapex) */

static cint		maxpathlen = ulibval.maxpathlen ;
static cint		maxlinelen = ulibval.maxline ;

constexpr cchar		version[]	= "0.0.1" ; /* semantic-versioning */
constexpr int		nents		= NENTS ;
constexpr bool		f_debug		= CF_DEBUG ;
constexpr bool		f_filelines	= CF_FILELINES ;


/* exported variables */


/* exported subroutines */

int main(int argc,mainv argv,mainv envv) {
    	constexpr int	dfd = (f_debug) ? FD_STDERR : -1 ;
	int		ex = EX_OK ;
	int		rs ;
	int		rs1 ;
	debfd(dfd) ;
	DEBPRINTF("ent\n") ;
	if (proginfo pi(argc,argv,envv) ; (rs = pi.start) >= 0) {
	    {
                rs = pi.argproc() ;
	    }
	    rs1 = pi.finish ;
	    if (rs >= 0) rs = rs1 ;
	} /* end if (proginfo) */
	if ((ex == EX_OK) && (rs < 0)) {
	    cerr << "files" << ": error (" << rs << ")" << eol ;
	    ex = mapex(mapexs,rs) ;
	}
	DEBPRINTF("ret ex=%d\n",ex) ;
	return ex ;
}
/* end subroutine (main) */


/* local subroutines */

int proginfo::istart() noex {
	int		rs ;
	if ((rs = exts.start) >= 0) {
	    fl.exts = true ;
	    if ((rs = dirs.start) >= 0) {
		fl.dirs = true ;
		if ((rs = afs.start) >= 0) {
		    fl.afs = true ;
		    {
		        rs = SR_OK ;
		    }
		} /* end if (afs.start) */
	        if ((rs < 0) && fl.dirs) {
		    fl.dirs = false ;
		    dirs.finish() ;
	        } /* end if (error) */
	    } /* end if (dirs.start) */
	    if ((rs < 0) && fl.exts) {
		fl.exts = false ;
		exts.finish() ;
	    } /* end if (error) */
	} /* end if (exts.start) */
	return rs ;
}
/* end method (proginfo::istart) */

int proginfo::ifinish() noex {
    	int		rs = SR_OK ;
	int		rs1 ;
	if (fl.afs) {
	    rs1 = afs.finish ;
	    if (rs >= 0) rs = rs1 ;
	    fl.afs = false ;
	}
	if (fl.dirs) {
	    rs1 = dirs.finish ;
	    if (rs >= 0) rs = rs1 ;
	    fl.dirs = false ;
	}
	if (fl.exts) {
	    rs1 = exts.finish ;
	    if (rs >= 0) rs = rs1 ;
	    fl.exts = false ;
	}
	return rs ;
}
/* end method (proginfo::ifinish) */

int proginfo::getpnames(mainv names,cc *sp,int sl) noex {
    	int		rs = SR_OK ;
	if ((pm = matstr(names,sp,sl)) >= 0) {
	    pn = names[pm] ;
	    rs = pm ;
	}
	return rs ;
} /* end method (proginfo::getpnames) */

int proginfo::getpn(mainv names) noex {
    	cint		pnlen = intconv(pnstr.size()) ;
	int		rs = SR_NOMSG ;
	cchar		*bp{} ; /* used-multiple */
	if (int bl = pnlen ; bl > 0) {
	    bp = pnstr.c_str() ;
	    rs = getpnames(names,bp,bl) ;
	} else if ((argc > 0) && argv[0]) {
	    cchar	*argz = argv[0] ;
	    if ((bl = sfbasename(argz,-1,&bp)) > 0) {
		if (cint rl = rmchr(bp,bl,'.') ; rl > 0) {
		    rs = getpnames(names,bp,rl) ;
		}
	    } /* end if (sfbasename) */
	} /* end if */
	return rs ;
} /* end method (proginfo::getpn) */

int proginfo::iargbegin() noex {
    	int		rs ;
	rs = tardirbegin() ;
	return rs ;
} /* end method (proginfo::iargbegin) */

int proginfo::iargend() noex {
    	int		rs = SR_OK ;
	int		rs1 ;
	{
	    rs1 = tardirend() ;
	    if (rs >= 0) rs = rs1 ;
	}
	return rs ;
} /* end method (proginfo::iargend) */

int proginfo::iflistbegin() noex {
    	int		rs = SR_OK ;
	switch (pm) {
	case progmode_files:
	case progmode_filelines:
	    if ((rs = seen.start(nents)) >= 0) {
	        fl.seens = true ;
	    }
	    break ;
	case progmode_depmods:
	    if ((rs = seen.start(nents)) >= 0) {
	        fl.seens = true ;
		if ((rs = mods.start) >= 0) {
	            fl.mods = true ;
		}
		if (rs < 0) {
		    fl.seens = false ;
		    seen.finish() ;
		}
	    }
	    break ;
	case progmode_filesyner:
	case progmode_filelinker:
	    if ((rs = recs.start) >= 0) {
	        fl.recs = true ;
	    }
	    break ;
	} /* end switch */
	return rs ;
}
/* end method (proginfo::iflistbegin) */

int proginfo::iflistend() noex {
    	int		rs = SR_OK ;
	int		rs1 ;
	if (fl.recs) {
	    rs1 = recs.finish ;
	    if (rs >= 0) rs = rs1 ;
	    fl.recs = false ;
	}
	if (fl.seens) {
	    rs1 = seen.finish ;
	    if (rs >= 0) rs = rs1 ;
	    fl.seens = false ;
	}
	if (fl.mods) {
	    rs1 = mods.finish ;
	    if (rs >= 0) rs = rs1 ;
	    fl.mods = false ;
	}
	return rs ;
}
/* end method (proginfo::iflistend) */

int proginfo::argproc() noex {
	int		rs ;
	int		rs1 ;
	int		c = 0 ;
	if ((rs = argbegin) >= 0) {
	    if (argmgr am(argc,argv) ; (rs = am.start) >= 0) {
	        if ((rs = args(&am)) >= 0) {
		    if ((rs = preamble()) > 0) {
	                if ((rs = flistbegin) >= 0) {
			    {
		                rs = argprocer(&am) ;
		                c = rs ;
			    }
		            rs1 = flistend ;
		            if (rs >= 0) rs = rs1 ;
		        } /* end if (flist) */
	            } /* end if (preamble) */
	        } /* end if (arg) */
	        rs1 = am.finish ;
	        if (rs >= 0) rs = rs1 ;
	    } /* end if (argmgr) */
	    rs1 = argend ;
	    if (rs >= 0) rs = rs1 ;
	} /* end if (arg) */
	return (rs >= 0) ? c : rs ;
} /* end method (proginfo::argproc) */

int proginfo::args(argmgr *amp) noex {
    	int		rs ;
	while ((rs = amp->arg) > 0) {
	    if (cc *sp ; (rs = amp->argopt(&sp)) > 1) {
		if (int wi ; (wi = matostr(argopts,2,sp,rs)) >= 0) {
		    rs = argoptstr(amp,wi) ;
		} else {
		    rs = argoptchr(amp,sp,rs) ;
		} /* end if (matostr) */
	    } else if (rs == 1) {
		rs = argoptchr(amp,sp,rs) ;
	    } else if ((rs >= 0) && ((rs = amp->argoptlong(&sp)) > 0)) {
		rs = argoptlong(amp,sp,rs) ;
	    }
	    if ((rs < 0) || amp->argoptdone) break ;
	} /* end while */
	return rs ;
} /* end method (proginfo::args) */

int proginfo::argoptstr(argmgr *amp,int wi) noex {
    	int		rs = SR_OK ;
	(void) amp ;
	switch (wi) {
	case argopt_version:
	    fl.version = true ;
	    break ;
	case argopt_debug:
	    debuglevel = 1 ;
	    fl.debug = true ;
	    break ;
	case argopt_help:
	    fl.help = true ;
	    break ;
	case argopt_pm:
	    rs = argpm(amp) ;
	    break ;
	case argopt_sn:
	    if (cc *cp ; (rs = amp->argval(&cp)) > 0) {
	        rs = 0 ;
	    }
	    break ;
	case argopt_af:
	    rs = argfile(amp) ;
	    break ;
	case argopt_ud:
	    fl.uniqdir = true ;
	    break ;
	case argopt_ot:
	    if (cc *cp ; (rs = amp->argval(&cp)) > 0) {
	        rs = typeout(cp,rs) ;
	    }
	    break ;
	} /* end switch */
	return rs ;
} /* end method (proginfo::argkeustr) */

int proginfo::argoptlong(argmgr *amp,cchar *sp,int sl) noex {
	int		rs = SR_OK ;
	(void) amp ;
	if (int wi ; (wi = matostr(argoptlongs,2,sp,sl)) >= 0) {
	    switch (wi) {
	    case argoptlong_version:
		fl.version = true ;
		break ;
	    } /* end switch */
	} else {
	    cerr << pn << ": " << "invalid option" << eol ;
	    rs = SR_INVALID ;
	} /* end if (matostr) */
	return rs ;
} /* end method (proginfo::argoptlong) */

int proginfo::argoptchr(argmgr *amp,cchar *sp,int sl) noex {
    	int		rs = SR_OK ;
	while (sl-- && *sp) {
	    switch (cint key = mkchar(*sp++) ; key) {
	    case 'D':
		debuglevel = 1 ;
		rs = argdebug(amp) ;
		break ;
	    case 'H':
		fl.followarg = true ;
		break ;
	    case 'L':
		fl.followall = true ;
		break ;
	    case 'Q':
		fl.quiet = true ;
		break ;
	    case 'V':
		fl.verbose = true ;
		break ;
	    case 'd':
		rs = argtardir(amp) ;
		break ;
	    case 'q':
		fl.verbose = false ;
		break ;
	    case 's':
	    case 'x':
		rs = argsuf(amp) ;
		break ;
	    case 't':
		rs = argtype(amp) ;
		break ;
	    case 'u':
		fl.uniqfile = true ;
		break ;
	    default:
		rs = SR_INVALID ;
		break ;
	    } /* end switch */
	} /* end while */
	return rs ;
} /* end method (proginfo::argoptchr) */

int proginfo::argfile(argmgr *amp) noex {
    	int		rs ;
	int		c = 0 ;
	if (cc *sp ; (rs = amp->argval(&sp)) >= 0) {
    	    sif		so(sp,rs,',') ;
	    cchar	*cp ;
	    fl.argfile = true ;
	    for (int cl ; (rs >= 0) && ((cl = so(&cp)) > 0) ; ) {
		if ((cp[0] == '-') && (strwcmp("-",cp,cl) == 0)) {
		    fl.afsread = true ;
		} else {
	            rs = afs.add(cp,rs) ;
		    c += (rs < INT_MAX) ;
		}
	    } /* end for */
	} /* end if (argval) */
	return (rs >= 0) ? c : rs ;
} /* end method (proginfo::argfile) */

int proginfo::argfileread() noex {
    	int		rs = SR_OK ;
	if (fl.afsread) {
	    rs = SR_OK ;
	}
    	return rs ;
} /* end method (proginfo::argfileread) */

int proginfo::argsuf(argmgr *amp) noex {
    	int		rs ;
	if (cc *cp ; (rs = amp->argval(&cp)) >= 0) {
	    rs = sufadd(cp,rs) ;
	} /* end if (argval) */
	return rs ;
} /* end method (proginfo::argsuf) */

int proginfo::argpm(argmgr *amp) noex {
    	int		rs ;
	if (cc *cp ; (rs = amp->argval(&cp)) >= 0) {
	    try {
	        strview s(cp,rs) ;
	        pnstr = s ;
	    } catch (...) {
		rs = SR_NOMEM ;
	    }
	} /* end if (argval) */
	return rs ;
} /* end method (proginfo::argpm) */

int proginfo::argtype(argmgr *amp) noex {
    	int		rs ;
	if (cc *cp ; (rs = amp->argval(&cp)) >= 0) {
	    rs = modeadd(cp,rs) ;
	} /* end if (argval) */
	return rs ;
} /* end method (proginfo::argtype) */

int proginfo::argtardir(argmgr *amp) noex {
    	int		rs ;
	if (cc *cp ; (rs = amp->argval(&cp)) >= 0) {
	    switch (pm) {
	    case progmode_filesyner:
	    case progmode_filelinker:
	        rs = tardiradd(cp,rs) ;
	        break ;
	    } /* end switch */
	} /* end if (argval) */
	return rs ;
} /* end method (proginfo::argtardir) */

int proginfo::argstat(cchar *fn,ustat *sbp) noex {
    	int		rs ;
	if (fl.followarg || fl.followall) {
	    rs = u_stat(fn,sbp) ;
	} else {
	    rs = u_lstat(fn,sbp) ;
	}
	return rs ;
} /* end method (proginfo::argstat) */

int proginfo::preamble() noex {
    	int		rs ;
	int		fcont = false ; /* return-value */
	if ((rs = getpn(prognames)) >= 0) {
	            if (fl.version) {
	                rs = printf("version=%s\n",version) ;
	            } else {
		        if (debuglevel) {
			    if (rs >= 0) {
				rs = printf("pm=%s\n",pn) ;
			    }
			    if (rs >= 0) {
				rs = printf("debuglevel=%d\n",debuglevel) ;
			    }
		        } /* end if (debuglevel) */
		        fcont = true ;
	            } /* end if */
		    if (rs >= 0) {
		        switch (pm) {
		        case progmode_depmods:
			    {
				cint mi = fl.ot ;
	    		        rs = printf("typeout=%s\n",typeouts[mi]) ;
			    }
			    break ;
		        } /* end switch */
		    } /* end if (ok) */
	} /* end if (proginfo::getpn) */
	return (rs >= 0) ? fcont : rs ;
} /* end method (proginfo::preamble) */

int proginfo::argprocer(argmgr *amp) noex {
	int		rs = SR_OK ;
	int		rs1 ;
	int		c = 0 ;
	if ((argc > 1) && (fl.argfile || ((rs = amp->count) > 0))) {
	    if ((rs = process_pmbegin()) > 0) {
		if ((rs = amp->count) > 0) {
	            for (cauto &fn : *amp) {
	                if (fn && fn[0]) {
		            if (fn[0] == '-') {
			        if (fn[1] == '\0') {
		                    rs = argreadin() ;
		                    c += rs ;
			        }
		            } else {
	                        rs = argprocname(fn) ;
		                c += rs ;
		            }
	                } /* end if */
	                if ((rs < 0) || fexit || (fn == nullptr)) break ;
	            } /* end for */
		} /* end if (argmgr_count) */
		if (rs >= 0) {
		    rs = argfileread() ;
		}
	        rs1 = process_pmend() ;
	        if (rs >= 0) rs = rs1 ;
	    } /* end if (process_pm) */
	} /* end if (have argument) */
	return (rs >= 0) ? c : rs ;
} /* end method (proginfo::argprocer) */

int proginfo::process_pmbegin() noex {
	int		rs = SR_OK ;
	int		fcontinue = (! fexit) ;
	switch (pm) {
        case progmode_files:
	    break ;
        case progmode_filelines:
	    if ((rs = maxlinelen) >= 0) {
	        llen = rs ;
		rs = SR_NOMEM ;
	        if ((lbuf = new(nothrow) char[llen + 1]) != nullptr) {
		    rs = SR_OK ;
	        } else {
		    llen = 0 ;
		}
	    } /* end block */
	    break ;
	} /* end switch */
	return (rs >= 0) ? fcontinue : rs ;
}
/* end subroutine (proginfo::process_pmbegin) */

int proginfo::process_pmend() noex {
	int		rs = SR_OK ;
	switch (pm) {
        case progmode_filelines:
	    if (lbuf) {
		if (fl.verbose) {
		    cout << lines << eol ;
		}
		delete [] lbuf ;
		lbuf = nullptr ;
	        llen = 0 ;
	    } /* end if (end lines*/
	    break ;
	} /* end switch */
	return rs ;
}
/* end subroutine (proginfo::process_pmend) */

int proginfo::argreadin() noex {
	cnullptr	np{} ;
	int		rs ;
	int		c = 0 ;
	if ((rs = maxpathlen) >= 0) {
	    cint	plen = rs ;
	    rs = SR_NOMEM ;
	    if (char *pbuf ; (pbuf = new(nothrow) char[plen + 1]) != np) {
	        while ((rs = readln(&cin,pbuf,plen)) > 0) {
		    if (cint pl = rmeol(pbuf,rs) ; pl > 0) {
		        rs = argprocname(pbuf,pl) ;
			c += rs ;
		    }
		    if (rs < 0) break ;
	        } /* end if (reading lines) */
	        delete [] pbuf ;
	    } /* end if (m-a-f) */
	} /* end if (maxpathlen) */
	return (rs >= 0) ? c : rs ;
}
/* end method (proginfo::argreadin) */

int proginfo::argprocname(cchar *sp,int sl) noex {
	strnul		s(sp,sl) ;
	int		rs ;
	int		c = 0 ;
	DEBPRINTF("ent\n") ;
	if (ustat sb ; (rs = argstat(s,&sb)) >= 0) {
	    switch (cint ft = filetype(sb.st_mode) ; ft) {
	    case filetype_dir:
		rs = procdir(&sb,sp,sl) ;
		c = rs ;
		break ;
	    case filetype_reg:
	    case filetype_blk:
	    case filetype_chr:
	    case filetype_lnk:
	    case filetype_fifo:
	    case filetype_name:
	    case filetype_sock:
		rs = procfile(&sb,sp,sl) ;
		c = rs ;
		break ;
	    } /* end switch */
	} else if (isNotAccess(rs)) {
	    rs = SR_OK ;
	}
	DEBPRINTF("ret rs=%d c=%d\n",rs,c) ;
	return (rs >= 0) ? c : rs ;
} /* end method (proginfo::argprocname) */

int proginfo::procdir(custat *sbp,cchar *sp,int µsl) noex {
    	int		rs = SR_OK ;
	int		c = 0 ;
	if (int sl ; (sl = getlenstr(sp,µsl)) > 0) {
	    if (sp[0] != '.') {
	        if ((rs = procdirsubs(sbp,sp,sl)) >= 0) {
		    c += rs ;
		    rs = procdirself(sbp,sp,sl) ;
		    c += rs ;
	        } /* end if (procdirsubs) */
	    } /* end if (non-empty) */
	} /* end if (getlenstr) */
	return (rs >= 0) ? c : rs ;
} /* end method (proginfo::procdir) */

int proginfo::procdirsubs(custat *sbp,cchar *sp,int sl) noex {
    	using enum	fs::path::format ;
    	using enum	fs::directory_options ;
    	int		rs = SR_OK ;
	int		c = 0 ;
	(void) sbp ;
	if (sl && sp[0] && (sp[0] != '.')) {
    	    fs::directory_options dopt = skip_permission_denied ;
	    try {
		std::error_code ec ;
	        fs::path p(sp,(sp + sl),native_format) ;
		if (fl.followall) {
    	            dopt |= follow_directory_symlink ;
		}
		{
		    rdi it(p,dopt,ec) ;
		    for (cauto &de : it) {
			const fs::path depath = de ;
			{
			    cchar *fn = depath.c_str() ;
			    if (ustat sb ; (rs = u_lstat(fn,&sb)) >= 0) {
				rs = procent(&sb,fn) ;
			        c = rs ;
			    }
			} /* end block */
			if (rs < 0) break ;
		    } /* end for */
		} /* end block */
	    } catch (...) {
		rs = SR_NOMEM ;
	    }
	} /* end if (non-empty) */
	return (rs >= 0) ? c : rs ;
} /* end method (proginfo::procdirsubs) */

int proginfo::procdirself(custat *sbp,cchar *sp,int sl) noex {
    	return procfile(sbp,sp,sl) ;
} /* end method (proginfo::procdirself) */

int proginfo::procent(custat *sbp,cchar *sp,int µsl) noex {
    	int		rs = SR_OK ;
	if (int sl ; (sl = getlenstr(sp,µsl)) > 0) {
	    rs = procfile(sbp,sp,sl) ;
	}
	return rs ;
} /* end method (proginfo::procent) */

int proginfo::procfile(custat *sbp,cchar *sp,int sl) noex {
    	int		rs = SR_OK ;
	int		c = 0 ;
	DEBPRINTF("ent pi\n") ;
	if (sisub(sp,sl,".git") < 0) {
	    if ((! fl.modes) || (rs = modehave(sbp)) > 0) {
	        if ((! fl.suffix) || ((rs = sufhave(sp,sl)) > 0)) {
	            if ((! fl.uniqfile) || ((rs = fileuniq(sbp)) > 0)) {
	                switch (pm) {
	                case progmode_files:
		            rs = procfile_list(sbp,sp,sl) ;
		            c = rs ;
	                    break ;
	                case progmode_filelines:
		            rs = procfile_lc(sbp,sp,sl) ;
		            c = rs ;
	                    break ;
	                case progmode_depmods:
		            rs = procfile_mods(sbp,sp,sl) ;
		            c = rs ;
	                    break ;
	                } /* end switch */
	            } /* end if (fileuniq) */
	        } /* end if (sufhave) */
	    } /* end if (modehave) */
	} /* end if (sisub) */
	DEBPRINTF("ret rs=%d c=%d\n",rs,c) ;
	return (rs >= 0) ? c : rs ;
} /* end method (proginfo::procfile) */

int proginfo::procfile_list(custat *,cchar *sp,int sl) noex {
	int		rs = SR_OK ;
	int		c = 0 ;
	if (sp) {
	    strnul fn(sp,sl) ;
	    if (fl.verbose) {
	        cout << ccp(fn) << eol ;
	    }
	    c += 1 ;
	} /* end if (non-null) */
	return (rs >= 0) ? c : rs ;
}
/* end method (proginfo::procfile_list) */

int proginfo::procfile_lc(custat *sbp,cchar *sp,int sl) noex {
	int		rs = SR_OK ;
	int		rs1 ;
	int		c = 0 ;
	if (sbp && sp && sl) ylikely {
	    if (S_ISREG(sbp->st_mode)) {
	        strnul fn(sp,sl) ;
		c += 1 ;
		if_constexpr (f_filelines) {
		    rs = filelines(fn) ;
		    lines += rs ;
		} else {
	            if (ccfile rf ; (rs = rf.open(fn,"r",0)) >= 0) {
		        int	nl = 0 ; /* used-afterwards */
		        while ((rs = rf.readln(lbuf,llen)) > 0) {
			    nl += 1 ;
		        } /* end while */
		        lines += nl ;
		        rs1 = rf.close ;
		        if (rs >= 0) rs = rs1 ;
		    } /* end if (ccfile) */
		} /* end if_constexpr (f_filelines) */
	    } /* end if (is-reg) */
	} /* end if (non-null) */
	return (rs >= 0) ? c : rs ;
}
/* end method (proginfo::procfile_lc) */

int proginfo::procfile_mods(custat *sbp,cchar *sp,int sl) noex {
	int		rs = SR_FAULT ;
	int		c = 0 ;
	if (f_debug && debon) {
	    strnul se(sp,sl) ;
	    DEBPRINTF("ent sl=%d s=>%s<\n",sl,ccp(se)) ;
	}
	if (sp) {
	    rs = SR_OK ;
	    if (S_ISREG(sbp->st_mode)) {
	        if (fl.verbose) {
	            if (strnul fn(sp,sl) ; (rs = mods.procfile(fn)) >= 0) {
			cint	ot = fl.ot ;
			c += rs ;
			rs = mods.procout(ofp,ot) ;
		    } /* end if (modproc::procfile) */
	        } /* end if (verbose) */
	    } /* end if (regular file) */
	} /* end if (non-null) */
	if_constexpr (f_debug) {
	    DEBPRINTF("ret rs=%d c=%d\n",rs,c) ;
	}
	return (rs >= 0) ? c : rs ;
}
/* end method (proginfo::procfile_mods) */

int proginfo::sufadd(cchar *sp,int sl) noex {
    	int		rs = SR_OK ;
	int		c = 0 ;
	if (f_debug && debon) {
	    strnul se(sp,sl) ;
	    DEBPRINTF("ent sl=%d s=>%s<\n",sl,ccp(se)) ;
	}
	if ((rs = sufready()) >= 0) {
    	    sif		so(sp,sl,',') ;
	    cchar	*cp ;
	    for (int cl ; (rs >= 0) && ((cl = so(&cp)) > 0) ; ) {
	        if (f_debug && debon) {
	            strnul sone(cp,cl) ;
	            DEBPRINTF("cl=%d c=>%s<\n",cl,ccp(sone)) ;
	        }
	        if ((rs = exts.add(cp,cl)) > 0) {
		    c += 1 ;
		    fl.suffix = true ;
	        }
	    } /* end for */
	} /* end if (sufready) */
	if_constexpr (f_debug) {
	    DEBPRINTF("ret rs=%d c=%d\n",rs,c) ;
	}
	return (rs >= 0) ? c : rs ;
} /* end method (proginfo::sufadd) */

int proginfo::sufready() noex {
    	int		rs = SR_OK ;
	if (fl.suffix) {
	    rs = exts.ready ;
	}
    	return rs ;
} /* end method (proginfo::sufready) */

int proginfo::sufhave(cchar *sp,int sl) noex {
    	int		rs = SR_OK ;
	int		f = true ; /* return-value */
	if (fl.suffix) {
	    cchar	*cp ;
	    f = false ;
	    if (int cl ; (cl = sfext(sp,sl,&cp)) > 0) {
	        rs = exts.have(cp,cl) ;
		f = rs ;
	    } /* end if (sfext) */
	} /* end if (suffix check) */
	if_constexpr (f_debug) {
	    DEBPRINTF("ret rs=%d f=%u\n",rs,f) ;
	}
	return (rs >= 0) ? f : rs ;
} /* end method (proginfo::sufhave) */

int proginfo::modeadd(cchar *sp,int sl) noex {
    	sif		so(sp,sl,',') ;
    	int		rs = SR_OK ;
	int		c = 0 ; /* return-value */
	cchar		*cp ;
	if_constexpr (f_debug) {
	    strnul s(sp,sl) ;
	    DEBPRINTF("ent s=>%s<\n",ccp(s)) ;
	}
	for (int cl ; (rs >= 0) && ((cl = so(&cp)) > 0) ; ) {
	    int	dt = -1 ;
	    if (f_debug && debon) {
		strnul sm(cp,cl) ;
	        DEBPRINTF("cl=%d mode=>%s<\n",cl,ccp(sm)) ;
	    }
	    switch (cint ch = mkchar(*cp) ; ch) {
	    case 'f':
	    case 'p':
		dt = DT_FIFO ;
		break ;
	    case 'c':
		dt = DT_CHR ;
		break ;
	    case 'd':
		dt = DT_DIR ;
	 	break ;
	    case 'n':
		dt = DT_NAME ;
	 	break ;
	    case 'b':
		dt = DT_BLK ;
	 	break ;
	    case 'r':
		dt = DT_REG ;
		break ;
	    case 'l':
		dt = DT_LNK ;
		break ;
	    case 's':
		dt = DT_SOCK ;
		break ;
	    case 'w':
		dt = DT_WHT ;
		break ;
	    default:
		rs = SR_INVALID ;
		break ;
	    } /* end switch */
	    if (dt >= 0) {
		fl.modes = true ;
		bitop_set(modes,dt) ;
	        c += 1 ;
	    }
	} /* end for */
	if_constexpr (f_debug) {
	    DEBPRINTF("ret rs=%d c=%d\n",rs,c) ;
	}
	return (rs >= 0) ? c : rs ;
} /* end method (proginfo::modeadd) */

int proginfo::modehave(custat *sbp) noex {
    	int		rs = SR_OK ;
	int		f = true ;
	if (fl.modes) {
    	    cint ft = filetype(sbp->st_mode) ;
	    f = bitop_tst(modes,ft) ;
	}
    	return (rs >= 0) ? f : rs ;
} /* end method (proginfo::modehave) */

int proginfo::tardirbegin() noex {
    	int		rs = SR_OK ;
	if (! fl.dirs) {
	    if ((rs = dirs.start) >= 0) {
	        fl.dirs = true ;
	    }
	}
	return rs ;
} /* end method (proginfo::tardirbegin) */

int proginfo::tardirend() noex {
    	int		rs = SR_OK ;
	if (fl.dirs) {
	    if ((rs = dirs.finish()) >= 0){
	        fl.dirs = false ;
	    }
	}
	return rs ;
} /* end method (proginfo::tardirend) */

int proginfo::tardiravail() noex {
    	int		rs = SR_OK ;
	if (! fl.dirs) {
	    rs = tardirbegin() ;
	}
	return rs ;
} /* end method (proginfo::tardiravail) */

int proginfo::tardiradd(cchar *dp,int dl) noex {
    	int		rs ;
	if ((rs = tardiravail()) >= 0) {
    	    rs = dirs.add(dp,dl) ;
	}
    	return rs ;
} /* end method (proginfo::tardiradd) */

int proginfo::fileuniq(custat *sbp)  noex {
    	int		rs = 1 ; /* default indicates "uniq" */
	if (fl.uniqfile) {
	    rs = seen.checkin(sbp) ;
	}
	return rs ;
} /* end method (proginfo::fileuniq) */

int proginfo::typeout(cchar *cp,int cl)  noex {
    	int		rs = SR_INVALID ;
	if_constexpr (f_debug) {
	    strnul s(cp,cl) ;
	    DEBPRINTF("ot=%s\n",ccp(s)) ;
	}
	if (int mi ; (mi = matostr(typeouts,1,cp,cl)) >= 0) {
	    if_constexpr (f_debug) {
	        DEBPRINTF(__func__,"mi=%d\n",mi) ;
	    }
	    rs = SR_OK ;
	    fl.ot = uchar(mi & 0x03) ;	/* avoiding GCC complaint */
	} else {
	    if_constexpr (f_debug) {
	        DEBPRINTF(__func__,"err mi=%d\n",mi) ;
	    }
	} /* end if (matostr) */
	return rs ;
} /* end method (proginfo::typeout) */

int proginfo::argdebug(argmgr *amp) noex {
    	int		rs = SR_OK ;
	if (amp->haveargoptval) {
	    cchar *cp ;
	    if ((rs = amp->argval(&cp)) > 0) {
		if ((rs = optval(cp,rs)) >= 0) {
		    debuglevel = rs ;
		}
	    } /* end if (argoptval) */
	} /* end if (haveargoptval) */
	return rs ;
} /* end method (proginfo::argdebug) */

int proginfo::printf(cchar *fmt,...) noex {
    	cnothrow	nt{} ;
	cnullptr	np{} ;
    	va_list		ap ;
    	int		rs = SR_FAULT ;
	int		len = 0 ;
	if (fmt) {
	    rs = SR_INVALID ;
	    if (fmt[0]) {
	        va_begin(ap,fmt) ;
	        if ((rs = maxlinelen) >= 0) {
		    cint µllen = rs ;
		    if (char *µlbuf ; (µlbuf = new(nt) char[µllen + 1]) != np) {
			if ((rs = snvprintf(µlbuf,µllen,fmt,ap)) >= 0) {
			    cerr << pn << ": " << µlbuf ;
			    len = rs ;
			} /* end if (snvprintf) */
			delete [] µlbuf ;
		    } /* end if (new-char) */
	        } /* end if (maxlinelen) */
	        va_end(ap) ;
	    } /* end if (non-empty) */
	} /* end if (non-null) */
	return (rs >= 0) ? len : rs ;
} /* end method (proginfo::argdebug) */

int proginfo_co::operator () (int) noex {
	int		rs = SR_BUGCHECK ;
	if (op) ylikely {
	    switch (w) {
	    case proginfomem_start:
	        rs = op->istart() ;
	        break ;
	    case proginfomem_finish:
	        rs = op->ifinish() ;
	        break ;
	    case proginfomem_argbegin:
	        rs = op->iargbegin() ;
	        break ;
	    case proginfomem_argend:
	        rs = op->iargend() ;
	        break ;
	    case proginfomem_flistbegin:
	        rs = op->iflistbegin() ;
	        break ;
	    case proginfomem_flistend:
	        rs = op->iflistend() ;
	        break ;
	    } /* end switch */
	} /* end if (non-null) */
	return rs ;
}
/* end method (proginfo_co::operator) */


