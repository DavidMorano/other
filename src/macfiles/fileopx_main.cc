/* fileopx_main SUPPORT (filex) */
/* charset=ISO8859-1 */
/* lang=C++11 */

/* front-end subroutine for the FILEOPX program */
/* version %I% last-modified %G% */

#define	CF_DEBUGS	0		/* compile-time debug print-outs */
#define	CF_DEBUG	0		/* run-time debug print-outs */
#define	CF_DEBUGMALL	1		/* debug memory-allocations */
#define	CF_FOLLOWFILES	0		/* follow symbolic links of files */
#define	CF_DIRS		1		/* dirs */
#define	CF_NEWPRUNE	1		/* try new-prune */

/* revision history:

	= 1998-02-01, David A­D­ Morano
	The program was written from scratch to do what the previous
	program by the same name did.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

  	Name:
	fileopx_main

	Description:
	This is a fairly generic front-end subroutine for a program.

*******************************************************************************/

#include	<envstandards.h>	/* MUST be first to configure */
#include	<sys/types.h>
#include	<sys/param.h>
#include	<sys/stat.h>
#include	<unistd.h>
#include	<ctime>
#include	<csignal>
#include	<climits>
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<cstring>
#include	<clanguage.h>
#include	<usysbase.h>
#include	<usyscalls.h>
#include	<uclibmem.h>
#include	<getourenv.h>
#include	<estrings.h>
#include	<sighand.h>
#include	<sigblocker.h>
#include	<bits.h>
#include	<keyopt.h>
#include	<paramopt.h>
#include	<baops.h>
#include	<bfile.h>
#include	<hdb.h>
#include	<fsdirtree.h>
#include	<removes.h>
#include	<mkdirs.h>
#include	<fileobject.h>
#include	<filelines.h>
#include	<strn.h>
#include	<sfx.h>
#include	<bwops.h>
#include	<cfdec.h>
#include	<field.h>
#include	<nulstr.h>
#include	<vstrcmp.h>
#include	<strwcmp.h>
#include	<nleadstr.h>
#include	<matxstr.h>
#include	<timestr.h>
#include	<isnot.h>
#include	<exitcodes.h>
#include	<localmisc.h>

#include	"config.h"
#include	"proginfo.h"

#pragma		GCC dependency		"mod/libutil.ccm"

import libutil ;			/* |lenstr(3u)| + |memclear(3u)| */

/* local defines */

#ifndef	LINEBUFLEN
#define	LINEBUFLEN	MAX((MAXPATHLEN + 20),2048)
#endif

#define	PI		proginfo
#define	LI		linkinfo
#define	FID		struct fileid

#define	DIRID		struct dirid
#define	FILEID		struct fileid

#define	LINKINFO	struct linkinfo

#define	FILEINFO	struct fileinfo
#define	FILEINFO_FL	struct fileinfo_flags

#define	TARDIR		struct tardir

#define	DMODE		0775

#define	NDF		"fileop.deb"


/* local namespace */


/* local typedefs */


/* external subroutines */

extern "C" int	mkpathuser(char *,cchar *,cchar *,int) noex ;

extern "C" int	printhelp(void *,cchar *,cchar *,cchar *) noex ;
extern "C" int	proginfo_setpiv(PI *,cchar *,const pivars *) noex ;

#if	CF_DEBUGS || CF_DEBUG
extern "C" int	debugopen(cchar *) noex ;
extern "C" int	debugprintf(cchar *,...) noex ;
extern "C" int	debugprinthexblock(cchar *,int,cvoid *,int) noex ;
extern "C" int	debugclose() noex ;
extern "C" int	strlinelen(cchar *,int,int) noex ;
extern "C" int	nprintf(cchar *,cchar *,...) noex ;
#endif


/* external variables */


/* local structures */

enum fts {
	ft_r, /* regular-file */
	ft_d,
	ft_b,
	ft_c,
	ft_p,
	ft_n, /* name-file (XENIX) */
	ft_l,
	ft_s,
	ft_D, /* door (Solaris) */
	ft_e, /* exists */
	ft_overlast
} ; /* end enum (fts) */

struct fileinfo_flags {
	uint		dangle:1 ;
} ; /* end struct (fileinfo_flags) */

struct fileinfo {
	FILEINFO_FL	f ;
	uint		fts ;
} ; /* end struct (fileinfo) */

struct dirid {
	ino_t		ino ;
	dev_t		dev ;
} ; /* end struct (dirid) */

FID {
	ino_t		ino ;
	dev_t		dev ;
} ; /* end struct (fileid) */

struct tardir {
	cchar		*dname ;
	ino_t		ino ;
	dev_t		dev ;
} ; /* end struct (tardir) */

LI {
	cchar		*fname ;
	ino_t		ino ;
	dev_t		dev ;
	mode_t		mode ;
} ; /* end struct (linkinfo) */


/* forward references */

local uint	linkhash(cvoid *,int) noex ;
local uint	diridhash(cvoid *,int) noex ;
local uint	fileidhash(cvoid *,int) noex ;

local int	usage(PI *) noex ;

local int	loadfnos(PI *) noex ;

local int	procsig(PI *) noex ;
local int	procopts(PI *,keyopt *) noex ;
local int	procfts(PI *) noex ;
local int	process(PI *,ARGINFO *,bits *,cchar *,cchar *) noex ;
local int	procargs(PI *,ARGINFO *,bits *,cchar *) noex ;
local int	procnames(PI *,cchar *,int) noex ;
local int	procname(PI *,cchar *) noex ;
local int	procdir(PI *,cchar *,ustat *) noex ;
local int	procdirs(PI *,cchar *,int,ustat *) noex ;
local int	procother(PI *,cchar *,ustat *) noex ;
local int	procothers(PI *,cchar *,ustat *,FILEINFO *) noex ;
local int	procprune(PI *,cchar *) noex ;
local int	procprintfts(PI *,cchar *) noex ;
local int	procprintsufs(PI *,cchar *) noex ;

local int	proctars_begin(PI *) noex ;
local int	proctars_end(PI *) noex ;
local int	proctars_check(PI *) noex ;
local int	proctars_checkerr(PI *,cchar *,int) noex ;
local int	proctars_same(PI *,ustat *) noex ;
local int	proctars_load(PI *,cchar *,ustat *) noex ;
local int	proctars_fins(PI *) noex ;
local int	proctars_notalready(PI *,ustat *) noex ;

local int	procsuf_begin(PI *) noex ;
local int	procsuf_load(PI *,int,cchar *,int) noex ;
local int	procsuf_have(PI *,cchar *,int) noex ;
local int	procsuf_end(PI *) noex ;

local int	procrm_begin(PI *) noex ;
local int	procrm_add(PI *,cchar *,int) noex ;
local int	procrm_end(PI *) noex ;

local int	proclines_begin(PI *) noex ;
local int	proclines_end(PI *) noex ;

local int	procdir_begin(PI *) noex ;
local int	procdir_have(PI *,dev_t,ino_t,cchar *,int) noex ;
local int	procdir_addid(PI *,dev_t,ino_t) noex ;
local int	procdir_haveprefix(PI *,cchar *,int) noex ;
local int	procdir_addprefix(PI *,cchar *,int) noex ;
local int	procdir_end(PI *) noex ;

local int	procuniq_begin(PI *) noex ;
local int	procuniq_end(PI *) noex ;
local int	procuniq_have(PI *,dev_t,ino_t) noex ;
local int	procuniq_addid(PI *,dev_t,ino_t) noex ;

local int	procprune_begin(PI *,cchar *) noex ;
local int	procprune_end(PI *) noex ;
local int	procprune_loadfile(PI *,cchar *) noex ;
local int	procprune_size(PI *,int *) noex ;

local int	proclink_begin(PI *) noex ;
local int	proclink_add(PI *,dev_t,ino_t,mode_t,cchar *) noex ;
local int	proclink_have(PI *,dev_t,ino_t,LI **) noex ;
local int	proclink_end(PI *) noex ;
local int	proclink_fins(PI *) noex ;

local int	procsize(PI *,cchar *,ustat *,FILEINFO *) noex ;
local int	proclink(PI *,cchar *,ustat *,FILEINFO *) noex ;
local int	procsync(PI *,cchar *,ustat *,FILEINFO *) noex ;
local int	procrm(PI *,cchar *,ustat *,FILEINFO *) noex ;
local int	proclines(PI *,cchar *,ustat *,FILEINFO *) noex ;

local int	procsynclink(PI *,cchar *,ustat *,LI *) noex ;
local int	procsyncer(PI *,cchar *,ustat *) noex ;

local int	procsyncer_reg(PI *,cchar *,ustat *) noex ;
local int	procsyncer_dir(PI *,cchar *,ustat *) noex ;
local int	procsyncer_lnk(PI *,cchar *,ustat *) noex ;
local int	procsyncer_fifo(PI *,cchar *,ustat *) noex ;
local int	procsyncer_sock(PI *,cchar *,ustat *) noex ;

local int	tardir_start(TARDIR *,cchar *,ustat *) noex ;
local int	tardir_finish(TARDIR *) noex ;
local int	tardir_match(TARDIR *,ustat *) noex ;

local int	fileinfo_loadfts(FILEINFO *,ustat *) noex ;

local int	linkinfo_start(LI *,dev_t,ino_t,mode_t,cchar *) noex ;
local int	linkinfo_finish(LI *) noex ;

local int	dirid_start(DIRID *,dev_t,ino_t) noex ;
local int	dirid_finish(DIRID *) noex ;

local int	fileid_start(FILEID *,dev_t,ino_t) noex ;
local int	fileid_finish(FILEID *) noex ;

local int	mkpdirs(cchar *,mode_t) noex ;

local int	linkcmp(LI *,LI *,int) noex ;
local int	diridcmp(struct dirid *,struct dirid *,int) noex ;
local int	fileidcmp(FID *,FID *,int) noex ;

local book	isNotStat(int) noex ;

#if	CF_DEBUG
local cchar	*strfiletype(ustat *) noex ;
#endif

local void	main_sighand(int,siginfo_t *,void *) noex ;


/* local variables */

static volatile int	if_exit ;
static volatile int	if_intr ;

constexpr int		sigblocks[] = {
	SIGUSR1,
	SIGUSR2,
	SIGCHLD,
	0
} ; /* end array (sigblocks) */

constexpr int		sigignores[] = {
	SIGHUP,
	SIGPIPE,
	SIGPOLL,
#if	defined(SIGXFSZ)
	SIGXFSZ,
#endif
	0
} ; /* end array (sigignores) */

constexpr int		sigints[] = {
	SIGINT,
	SIGTERM,
	SIGQUIT,
	0
} ; /* end array (sigints) */

constexpr pivars	initvars = {
	VARPROGRAMROOT1,
	VARPROGRAMROOT2,
	VARPROGRAMROOT3,
	PROGRAMROOT,
	VARPRLOCAL
} ; /* end struct */

enum progmodes {
	progmode_filesize,
	progmode_filefind,
	progmode_filelinker,
	progmode_filesyncer,
	progmode_filerm,
	progmode_filelines,
	progmode_overlast
} ; /* end enum (progmodes) */

constexpr cpcchar	prognames[] = {
	"filesize",
	"filefind",
	"filelinker",
	"filesyncer",
	"filerm",
	"filelines",
	nullptr
} ; /* end array (prognames) */

enum argopts {
	argopt_root,
	argopt_version,
	argopt_verbose,
	argopt_tmpdir,
	argopt_help,
	argopt_pm,
	argopt_sn,
	argopt_af,
	argopt_ef,
	argopt_of,
	argopt_pf,
	argopt_yf,
	argopt_sa,
	argopt_sr,
	argopt_yi,
	argopt_option,
	argopt_set,
	argopt_follow,
	argopt_cores,
	argopt_iacc,
	argopt_ignacc,
	argopt_im,
	argopt_nice,
	argopt_ff,
	argopt_readable,
	argopt_older,
	argopt_accessed,
	argopt_prune,
	argopt_summary,
	argopt_noprog,
	argopt_overlast
} ; /* end enum (argopts) */

constexpr cpcchar	argopts[] = {
	"ROOT",
	"VERSION",
	"VERBOSE",
	"TMPDIR",
	"HELP",
	"pm",
	"sn",
	"af",
	"ef",
	"of",
	"pf",
	"yf",
	"sa",
	"sr",
	"yi",
	"option",
	"set",
	"follow",
	"cores",
	"iacc",
	"ignacc",
	"im",
	"nice",
	"ff",
	"readable",
	"older",
	"accessed",
	"prune",
	"summary",
	"noprog",
	nullptr
} ; /* end array (argopts) */

enum progopts {
	progopt_uniq,
	progopt_name,
	progopt_cores,
	progopt_prog,
	progopt_noprog,
	progopt_nosock,
	progopt_nopipe,
	progopt_nofifo,
	progopt_nodev,
	progopt_noname,
	progopt_nolink,
	progopt_noextra,
	progopt_nodotdir,
	progopt_nosuf,
	progopt_sr,
	progopt_sa,
	progopt_s,
	progopt_follow,
	progopt_younger,
	progopt_yi,
	progopt_iacc,
	progopt_ignacc,
	progopt_im,
	progopt_quiet,
	progopt_nice,
	progopt_ff,
	progopt_readable,
	progopt_rmfile,
	progopt_older,
	progopt_accessed,
	progopt_prune,
	progopt_summary,
	progopt_ignasscomm,
	progopt_igncomm,
	progopt_overlast
} ; /* end enum (progopts) */

constexpr cpcchar	progopts[] = {
	"uniq",
	"name",
	"cores",
	"prog",
	"noprog",
	"nosock",
	"nopipe",
	"nofifo",
	"nodev",
	"noname",
	"nolink",
	"noextra",
	"nodotdir",
	"nosuf",
	"sr",
	"sa",
	"s",
	"follow",
	"younger",
	"yi",
	"iacc",
	"ignacc",
	"im",
	"quiet",
	"nice",
	"ff",
	"readable",
	"rmfile",
	"older",
	"accessed",
	"prune",
	"summary",
	"ignasscomm",
	"igncomm",
	nullptr
} ; /* end array (progopts) */

enum ftstrs {
	ftstr_file,
	ftstr_directory,
	ftstr_block,
	ftstr_character,
	ftstr_pipe,
	ftstr_fifo,
	ftstr_name,
	ftstr_socket,
	ftstr_link,
	ftstr_door,
	ftstr_exists,
	ftstr_regular,
	ftstr_overlast
} ; /* end enum (ftstrs) */

constexpr cpcchar	ftstrs[] = {
	"file",
	"directory",
	"block",
	"character",
	"pipe",
	"fifo",
	"name",
	"socket",
	"link",
	"door",
	"exists",
	"regular",
	nullptr
} ; /* end array (ftstrs) */

enum whiches {
	which_uniq,
	which_name,
	which_noprog,
	which_nosock,
	which_nopipe,
	which_nodev,
	which_nolink,
	which_overlast
} ; /* end enum (whiches) */

#ifdef	COMMENT

constexpr cpcchar	whiches[] = {
	"uniq",
	"name",
	"noprog",
	"nosock",
	"nopipe",
	"nodev",
	"nolink",
	nullptr
} ; /* end array (whiches) */

#endif /* COMMENT */

constexpr cchar		po_ftsp[]	= PO_FTS ;
constexpr cchar		po_sufreq[]	= PO_SUFREQ ;
constexpr cchar		po_sufacc[]	= PO_SUFACC ;
constexpr cchar		po_sufrej[]	= PO_SUFREJ ;
constexpr cchar		po_prune[]	= PO_PRUNE ;

enum sufs {
	suf_req,
	suf_acc,
	suf_rej,
	suf_overlast
} ; /* end enum (sufs) */

constexpr cpcchar	sufs[] = {
	PO_SUFREQ,
	PO_SUFACC,
	PO_SUFREJ,
	nullptr
} ; /* end array (sufs) */

constexpr int		rsnostat[] = {
	SR_NOTDIR,
	SR_NOENT,
	0
} ; /* end array (rsnostat) */

constexpr char		aterms[] = {
	0x00, 0x2E, 0x00, 0x00,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00
} ; /* end array (aterms) */

constexpr mapex		mapexs[] = {
	{ SR_NOENT, EX_NOUSER },
	{ SR_AGAIN, EX_TEMPFAIL },
	{ SR_DEADLK, EX_TEMPFAIL },
	{ SR_NOLCK, EX_TEMPFAIL },
	{ SR_TXTBSY, EX_TEMPFAIL },
	{ SR_ACCESS, EX_NOPERM },
	{ SR_REMOTE, EX_PROTOCOL },
	{ SR_NOSPC, EX_TEMPFAIL },
	{ SR_INTR, EX_INTR },
	{ SR_EXIT, EX_TERM },
	{ 0, 0 }
} ; /* end array )mapex) */


/* exported variables */


/* exported subroutines */

int main(int argc,mainv argv,mainv envv) {
	PROGINFO	pi, *pip = &pi ;
	ARGINFO		ainfo ;
	SIGHAND		sm ;
	bits		pargs ;
	keyopt		akopts ;
	bfile		errfile ;

#if	(CF_DEBUGS || CF_DEBUG) && CF_DEBUGMALL
	uint		mo_start = 0 ;
#endif

	int		argr, argl, aol, akl, avl, kwi ;
	int		ai, ai_max, ai_pos ;
	int		rs ;
	int		rs1 ;
	int		rs2 = SR_OK ;
	int		cl ;
	int		v ;
	int		ex = EX_INFO ;
	int		f_optminus, f_optplus, f_optequal ;
	int		f_usage = false ;
	int		f_version = false ;
	int		f_help = false ;

	cchar		*argp, *aop, *akp, *avp ;
	cchar		*argval = nullptr ;
	cchar		*pr = nullptr ;
	cchar		*sn = nullptr ;
	cchar		*pmspec = nullptr ;
	cchar		*afname = nullptr ;
	cchar		*efname = nullptr ;
	cchar		*ofname = nullptr ;
	cchar		*yfname = nullptr ;
	cchar		*pfname = nullptr ;
	cchar		*cp ;

	if_exit = false ;
	if_intr = false ;

	rs = sighand_start(&sm,sigblocks,sigignores,sigints,main_sighand) ;
	if (rs < 0) goto badsighand ;

#if	CF_DEBUGS || CF_DEBUG
	if ((cp = getourenv(envv,VARDEBUGFNAME)) != nullptr) {
	    rs2 = debugopen(cp) ;
	    debugprintf("main: starting DFD=%d\n",rs2) ;
	}
#endif /* CF_DEBUGS */

#if	(CF_DEBUGS || CF_DEBUG) && CF_DEBUGMALL
	uc_mallset(1) ;
	uc_mallout(&mo_start) ;
#endif

	rs = proginfo_start(pip,envv,argv[0],VERSION) ;
	if (rs < 0) {
	    ex = EX_OSERR ;
	    goto badprogstart ;
	}

	if ((cp = getenv(VARBANNER)) == nullptr) cp = BANNER ;
	rs = proginfo_setbanner(pip,cp) ;

/* early things to initialize */

	pip->namelen = MAXNAMELEN ;
	pip->verboselevel = 1 ;
	pip->progmode = -1 ;

	if (rs >= 0) {
	    if ((cp = getourenv(envv,VARDEBUGLEVEL)) != nullptr) {
	        rs = optvalue(cp,-1) ;
	        pip->debuglevel = rs ;
	    }
	}

/* process program arguments */

	if (rs >= 0) rs = bits_start(&pargs,1) ;
	if (rs < 0) goto badpargs ;

	rs = keyopt_start(&akopts) ;
	pip->open.akopts = (rs >= 0) ;

	if (rs >= 0) {
	    rs = paramopt_start(&pip->aparams) ;
	    pip->open.aparams = (rs >= 0) ;
	}

	ai_max = 0 ;
	ai_pos = 0 ;
	argr = argc ;
	for (ai = 0 ; (ai < argc) && (argv[ai] != nullptr) ; ai += 1) {
	    if (rs < 0) break ;
	    argr -= 1 ;
	    if (ai == 0) continue ;

	    argp = argv[ai] ;
	    argl = lenstr(argp) ;

	    f_optminus = (*argp == '-') ;
	    f_optplus = (*argp == '+') ;
	    if ((argl > 1) && (f_optminus || f_optplus)) {
	        cint ach = MKCHAR(argp[1]) ;

	        if (isdigitlatin(ach)) {

	            argval = (argp+1) ;

	        } else if (ach == '-') {

	            ai_pos = ai ;
	            break ;

	        } else {

	            aop = argp + 1 ;
	            akp = aop ;
	            aol = argl - 1 ;
	            f_optequal = false ;
	            if ((avp = strchr(aop,'=')) != nullptr) {
	                f_optequal = true ;
	                akl = avp - aop ;
	                avp += 1 ;
	                avl = aop + argl - 1 - avp ;
	                aol = akl ;
	            } else {
	                avp = nullptr ;
	                avl = 0 ;
	                akl = aol ;
	            }

	            if ((kwi = matostr(argopts,2,akp,akl)) >= 0) {

	                switch (kwi) {

/* program root */
	                case argopt_root:
	                    if (f_optequal) {
	                        f_optequal = false ;
	                        if (avl) {
	                            pr = avp ;
				}
	                    } else {
	                        if (argr > 0) {
	                            argp = argv[++ai] ;
	                            argr -= 1 ;
	                            argl = lenstr(argp) ;
	                            if (argl) {
	                                pr = argp ;
				    }
	                        } else {
	                            rs = SR_INVALID ;
				}
	                    }
	                    break ;

/* the user specified some progopts */
	                case argopt_option:
	                    if (argr > 0) {
	                        argp = argv[++ai] ;
	                        argr -= 1 ;
	                        argl = lenstr(argp) ;
	                        if (argl) {
	                            keyopt	*kop = &akopts ;
	                            rs = keyopt_loads(kop,argp,argl) ;
	                        }
	                    } else {
	                        rs = SR_INVALID ;
			    }
	                    break ;

/* version */
	                case argopt_version:
	                    f_version = true ;
	                    if (f_optequal) {
	                        rs = SR_INVALID ;
			    }
	                    break ;

/* verbose */
	                case argopt_verbose:
	                    pip->verboselevel = 2 ;
	                    if (f_optequal) {
	                        f_optequal = false ;
	                        if (avl) {
	                            rs = optvalue(avp,avl) ;
	                            pip->verboselevel = rs ;
	                        }
	                    }
	                    break ;

/* temporary directory */
	                case argopt_tmpdir:
	                    if (f_optequal) {
	                        f_optequal = false ;
	                        if (avl) {
	                            pip->tmpdname = avp ;
				}
	                    } else {
	                        if (argr > 0) {
	                            argp = argv[++ai] ;
	                            argr -= 1 ;
	                            argl = lenstr(argp) ;
	                            if (argl) {
	                                pip->tmpdname = argp ;
				    }
	                        } else {
	                            rs = SR_INVALID ;
				}
	                    }
	                    break ;

	                case argopt_help:
	                    f_help = true ;
	                    break ;

/* the user specified some progopts */
	                case argopt_set:
	                    if (argr > 0) {
	                        argp = argv[++ai] ;
	                        argr -= 1 ;
	                        argl = lenstr(argp) ;
	                        if (argl) {
	                            paramopt	*app = &pip->aparams ;
	                            rs = paramopt_loadu(app,argp,argl) ;
	                        }
	                    } else {
	                        rs = SR_INVALID ;
			    }
	                    break ;

/* program mode */
	                case argopt_pm:
	                    if (f_optequal) {
	                        f_optequal = false ;
	                        if (avl) {
	                            pmspec = avp ;
				}
	                    } else {
	                        if (argr > 0) {
	                            argp = argv[++ai] ;
	                            argr -= 1 ;
	                            argl = lenstr(argp) ;
	                            if (argl) {
	                                pmspec = argp ;
				    }
	                        } else {
	                            rs = SR_INVALID ;
				}
	                    }
	                    break ;

/* search name */
	                case argopt_sn:
	                    if (f_optequal) {
	                        f_optequal = false ;
	                        if (avl) {
	                            sn = avp ;
				}
	                    } else {
	                        if (argr > 0) {
	                            argp = argv[++ai] ;
	                            argr -= 1 ;
	                            argl = lenstr(argp) ;
	                            if (argl) {
	                                sn = argp ;
				    }
	                        } else {
	                            rs = SR_INVALID ;
				}
	                    }
	                    break ;

/* argument files */
	                case argopt_af:
	                    if (f_optequal) {
	                        f_optequal = false ;
	                        if (avl) {
	                            afname = avp ;
				}
	                    } else {
	                        if (argr > 0) {
	                            argp = argv[++ai] ;
	                            argr -= 1 ;
	                            argl = lenstr(argp) ;
	                            if (argl) {
	                                afname = argp ;
				    }
	                        } else {
	                            rs = SR_INVALID ;
				}
	                    }
	                    break ;

/* error file */
	                case argopt_ef:
	                    if (f_optequal) {
	                        f_optequal = false ;
	                        if (avl)
	                            efname = avp ;
	                    } else {
	                        if (argr > 0) {
	                            argp = argv[++ai] ;
	                            argr -= 1 ;
	                            argl = lenstr(argp) ;
	                            if (argl) {
	                                efname = argp ;
				    }
	                        } else {
	                            rs = SR_INVALID ;
				}
	                    }
	                    break ;

/* output file */
	                case argopt_of:
	                    if (f_optequal) {
	                        f_optequal = false ;
	                        if (avl) {
	                            ofname = avp ;
				}
	                    } else {
	                        if (argr > 0) {
	                            argp = argv[++ai] ;
	                            argr -= 1 ;
	                            argl = lenstr(argp) ;
	                            if (argl) {
	                                ofname = argp ;
				    }
	                        } else {
	                            rs = SR_INVALID ;
				}
	                    }
	                    break ;

/* prune-file */
	                case argopt_pf:
	                    if (argr > 0) {
	                        argp = argv[++ai] ;
	                        argr -= 1 ;
	                        argl = lenstr(argp) ;
	                        if (argl) {
	                            pfname = argp ;
				}
	                    } else {
	                        rs = SR_INVALID ;
			    }
	                    break ;

/* younger-file */
	                case argopt_yf:
	                    if (argr > 0) {
	                        argp = argv[++ai] ;
	                        argr -= 1 ;
	                        argl = lenstr(argp) ;
	                        if (argl) {
	                            yfname = argp ;
				}
	                    } else {
	                        rs = SR_INVALID ;
			    }
	                    break ;

/* follow symbolic links */
	                case argopt_follow:
	                    pip->fin.follow = true ;
	                    pip->have.follow = true ;
	                    pip->fl.follow = true ;
	                    if (f_optequal) {
	                        f_optequal = false ;
	                        if (avl) {
	                            rs = optbool(avp,avl) ;
	                            pip->fl.follow = (rs > 0) ;
	                        }
	                    }
	                    break ;

	                case argopt_cores:
	                    pip->fin.cores = true ;
	                    pip->fl.cores = true ;
	                    if (f_optequal) {
	                        f_optequal = false ;
	                        if (avl) {
	                            rs = optbool(avp,avl) ;
	                            pip->fl.cores = (rs > 0) ;
	                        }
	                    }
	                    break ;

/* ignore inaccessible files */
	                case argopt_iacc:
	                case argopt_ignacc:
	                    pip->fin.iacc = true ;
	                    pip->fl.iacc = true ;
	                    if (f_optequal) {
	                        f_optequal = false ;
	                        if (avl) {
	                            rs = optbool(avp,avl) ;
	                            pip->fl.iacc = (rs > 0) ;
	                        }
	                    }
	                    break ;

	                case argopt_im:
	                    pip->fin.im = true ;
	                    pip->fl.im = true ;
	                    if (f_optequal) {
	                        f_optequal = false ;
	                        if (avl) {
	                            rs = optbool(avp,avl) ;
	                            pip->fl.im = (rs > 0) ;
	                        }
	                    }
	                    break ;

/* nice value */
	                case argopt_nice:
	                    cp = nullptr ;
	                    cl = -1 ;
	                    pip->fin.nice = true ;
	                    pip->have.nice = true ;
	                    if (f_optequal) {
	                        f_optequal = false ;
	                        if (avl) {
	                            cp = avp ;
	                            cl = avl ;
	                        }
	                    } else {
	                        if (argr > 0) {
	                            argp = argv[++ai] ;
	                            argr -= 1 ;
	                            argl = lenstr(argp) ;
	                            if (argl) {
	                                cp = argp ;
	                                cl = argl ;
	                            }
	                        } else {
	                            rs = SR_INVALID ;
				}
	                    }
	                    if ((rs >= 0) && (cp != nullptr)) {
	                        rs = optvalue(cp,cl) ;
	                        pip->nice = rs ;
	                    }
	                    break ;

/* first-follow */
	                case argopt_ff:
	                    pip->fin.ff = true ;
	                    pip->have.ff = true ;
	                    pip->fl.ff = true ;
	                    if (f_optequal) {
	                        f_optequal = false ;
	                        if (avl) {
	                            rs = optbool(avp,avl) ;
	                            pip->fl.ff = (rs > 0) ;
	                        }
	                    }
	                    break ;

/* readable */
	                case argopt_readable:
	                    pip->fin.readable = true ;
	                    pip->have.readable = true ;
	                    pip->fl.readable = true ;
	                    if (f_optequal) {
	                        f_optequal = false ;
	                        if (avl) {
	                            rs = optbool(avp,avl) ;
	                            pip->fl.readable = (rs > 0) ;
	                        }
	                    }
	                    break ;

/* older */
	                case argopt_older:
	                    if (argr > 0) {
	                        argp = argv[++ai] ;
	                        argr -= 1 ;
	                        argl = lenstr(argp) ;
	                        if (argl) {
	                            pip->fin.older = true ;
	                            pip->have.older = true ;
	                            rs = cfdecti(argp,argl,&v) ;
	                            pip->older = v ;
	                        }
	                    } else {
	                        rs = SR_INVALID ;
			    }
	                    break ;

/* accessed */
	                case argopt_accessed:
	                    if (argr > 0) {
	                        argp = argv[++ai] ;
	                        argr -= 1 ;
	                        argl = lenstr(argp) ;
	                        if (argl) {
	                            pip->fin.accessed = true ;
	                            pip->have.accessed = true ;
	                            rs = cfdecti(argp,argl,&v) ;
	                            pip->accessed = v ;
	                        }
	                    } else {
	                        rs = SR_INVALID ;
			    }
	                    break ;

/* suffix-accept */
	                case argopt_sa:
	                    if (argr > 0) {
	                        argp = argv[++ai] ;
	                        argr -= 1 ;
	                        argl = lenstr(argp) ;
	                        if (argl) {
	                            pip->fin.sufacc = true ;
	                            rs = procsuf_load(pip,suf_acc,argp,argl) ;
	                        }
	                    } else {
	                        rs = SR_INVALID ;
			    }
	                    break ;

/* suffix-reject */
	                case argopt_sr:
	                    if (argr > 0) {
	                        argp = argv[++ai] ;
	                        argr -= 1 ;
	                        argl = lenstr(argp) ;
	                        if (argl) {
	                            pip->fin.sufrej = true ;
	                            rs = procsuf_load(pip,suf_rej,argp,argl) ;
	                        }
	                    } else {
	                        rs = SR_INVALID ;
			    }
	                    break ;

/* younger-interval */
	                case argopt_yi:
	                    if (argr > 0) {
	                        argp = argv[++ai] ;
	                        argr -= 1 ;
	                        argl = lenstr(argp) ;
	                        if (argl) {
	                            pip->have.younger = true ;
	                            pip->fin.younger = true ;
	                            rs = cfdecti(argp,argl,&v) ;
	                            pip->younger = v ;
	                        }
	                    } else {
	                        rs = SR_INVALID ;
			    }
	                    break ;

/* prune components */
	                case argopt_prune:
	                    if (argr > 0) {
	                        argp = argv[++ai] ;
	                        argr -= 1 ;
	                        argl = lenstr(argp) ;
	                        if (argl) {
	                            paramopt	*app = &pip->aparams ;
	                            cchar	*po = po_prune ;
	                            rs = paramopt_loads(app,po,argp,argl) ;
	                            pip->fl.prune |= (rs > 0) ;
	                        }
	                    } else {
	                        rs = SR_INVALID ;
			    }
	                    break ;

/* summary mode (for "lines") */
	                case argopt_summary:
	                    pip->fin.summary = true ;
	                    pip->fl.summary = true ;
	                    if (f_optequal) {
	                        f_optequal = false ;
	                        if (avl) {
	                            rs = optbool(avp,avl) ;
	                            pip->fl.summary = (rs > 0) ;
	                        }
	                    }
	                    break ;

	                case argopt_noprog:
	                    if (! pip->fin.f_noprog) {
	                        pip->fin.f_noprog = true ;
	                        pip->have.f_noprog = true ;
	                        pip->fl.f_noprog = true ;
	                        if (avl > 0) {
	                            rs = optbool(avp,avl) ;
	                            pip->fl.f_noprog = (rs > 0) ;
	                        }
	                    }
	                    break ;

	                default:
	                    rs = SR_INVALID ;
	                    break ;

	                } /* end switch (key words) */

	            } else {

	                while (akl--) {
	                    cint	kc = MKCHAR(*akp) ;

	                    switch (kc) {

	                    case 'D':
	                        pip->debuglevel = 1 ;
	                        if (f_optequal) {
	                            f_optequal = false ;
	                            if (avl) {
	                                rs = optvalue(avp,avl) ;
	                                pip->debuglevel = rs ;
	                            }
	                        }
	                        break ;

/* quiet */
	                    case 'Q':
	                        pip->fl.quiet = true ;
	                        break ;

/* program-root */
	                    case 'R':
	                        if (argr > 0) {
	                            argp = argv[++ai] ;
	                            argr -= 1 ;
	                            argl = lenstr(argp) ;
	                            if (argl)
	                                pr = argp ;
	                        } else {
	                            rs = SR_INVALID ;
				}
	                        break ;

	                    case 'V':
	                        f_version = true ;
	                        if (f_optequal) {
	                            rs = SR_INVALID ;
				}
	                        break ;

/* continue on error */
	                    case 'c':
	                        pip->fin.nostop = true ;
	                        pip->have.nostop = true ;
	                        pip->fl.nostop = true ;
	                        if (f_optequal) {
	                            f_optequal = false ;
	                            if (avl) {
	                                rs = optbool(avp,avl) ;
	                                pip->fl.nostop = (rs > 0) ;
	                            }
	                        }
	                        break ;

/* target directory */
	                    case 'd':
	                        if (argr > 0) {
	                            argp = argv[++ai] ;
	                            argr -= 1 ;
	                            argl = lenstr(argp) ;
	                            if (argl) {
	                                paramopt	*pop = &pip->aparams ;
	                                cchar		*po = PO_TARDIRS ;
	                                rs = paramopt_loads(pop,po,argp,argl) ;
	                            }
	                        } else {
	                            rs = SR_INVALID ;
				}
	                        break ;

/* follow symbolic links */
	                    case 'f':
	                        pip->fin.follow = true ;
	                        pip->have.follow = true ;
	                        pip->fl.follow = true ;
	                        if (f_optequal) {
	                            f_optequal = false ;
	                            if (avl) {
	                                rs = optbool(avp,avl) ;
	                                pip->fl.follow = (rs > 0) ;
	                            }
	                        }
	                        break ;

/* file name length restriction */
	                    case 'l':
	                        cp = nullptr ;
	                        cl = -1 ;
	                        if (f_optequal) {
	                            f_optequal = false ;
	                            if (avl) {
	                                cp = avp ;
	                                cl = avl ;
	                            }
	                        } else {
	                            if (argr > 0) {
	                                argp = argv[++ai] ;
	                                argr -= 1 ;
	                                argl = lenstr(argp) ;
	                                if (argl) {
	                                    cp = argp ;
	                                    cl = argl ;
	                                }
	                            } else {
	                                rs = SR_INVALID ;
				    }
	                        }
	                        if ((rs >= 0) && (cp != nullptr)) {
	                            rs = optvalue(cp,cl) ;
	                            pip->namelen = rs ;
	                        }
	                        break ;

/* no-change */
	                    case 'n':
	                        pip->fl.nochange = true ;
	                        break ;

/* options */
	                    case 'o':
	                        if (argr > 0) {
	                            argp = argv[++ai] ;
	                            argr -= 1 ;
	                            argl = lenstr(argp) ;
	                            if (argl) {
	                                keyopt	*kop = &akopts ;
	                                rs = keyopt_loads(kop,argp,argl) ;
	                            }
	                        } else {
	                            rs = SR_INVALID ;
				}
	                        break ;

/* quiet */
	                    case 'q':
	                        pip->fin.quiet = true ;
	                        pip->have.quiet = true ;
	                        pip->fl.quiet = true ;
	                        if (f_optequal) {
	                            f_optequal = false ;
	                            if (avl) {
	                                rs = optbool(avp,avl) ;
	                                pip->fl.quiet = (rs > 0) ;
	                            }
	                        }
	                        break ;

/* remove before over-writing */
	                    case 'r':
	                        pip->fin.rmfile = true ;
	                        pip->have.rmfile = true ;
	                        pip->fl.rmfile = true ;
	                        if (f_optequal) {
	                            f_optequal = false ;
	                            if (avl) {
	                                rs = optbool(avp,avl) ;
	                                pip->fl.rmfile = (rs > 0) ;
	                            }
	                        }
	                        break ;

/* require a suffix for file names */
	                    case 's':
	                        cp = nullptr ;
	                        if (f_optequal) {
	                            f_optequal = false ;
	                            if (avl) {
	                                cp = avp ;
	                                cl = avl ;
	                            }
	                        } else {
	                            if (argr > 0) {
	                                argp = argv[++ai] ;
	                                argr -= 1 ;
	                                argl = lenstr(argp) ;
	                                if (argl) {
	                                    cp = argp ;
	                                    cl = argl ;
	                                }
	                            } else {
	                                rs = SR_INVALID ;
				    }
	                        }
	                        if ((rs >= 0) && (cp != nullptr)) {
	                            pip->fin.sufreq = true ;
	                            rs = procsuf_load(pip,suf_req,cp,cl) ;
	                        }
	                        break ;

/* file types */
	                    case 't':
	                        if (argr > 0) {
	                            argp = argv[++ai] ;
	                            argr -= 1 ;
	                            argl = lenstr(argp) ;
	                            if (argl) {
	                                paramopt	*app = &pip->aparams ;
	                                cchar		*po = po_fts ;
	                                rs = paramopt_loads(app,po,argp,argl) ;
	                            }
	                        } else {
	                            rs = SR_INVALID ;
				}
	                        break ;

/* unique */
	                    case 'u':
	                        pip->fin.f_uniq = true ;
	                        pip->have.f_uniq = true ;
	                        pip->fl.f_uniq = true ;
	                        if (f_optequal) {
	                            f_optequal = false ;
	                            if (avl) {
	                                rs = optbool(avp,avl) ;
	                                pip->fl.f_uniq = (rs > 0) ;
	                            }
	                        }
	                        break ;

/* verbose output */
	                    case 'v':
	                        pip->verboselevel = 2 ;
	                        if (f_optequal) {
	                            f_optequal = false ;
	                            if (avl) {
	                                rs = optvalue(avp,avl) ;
	                                pip->verboselevel = rs ;
	                            }
	                        }
	                        break ;

	                    case 'y':
	                        if (argr > 0) {
	                            argp = argv[++ai] ;
	                            argr -= 1 ;
	                            argl = lenstr(argp) ;
	                            if (argl) {
	                                pip->fin.younger = true ;
	                                pip->have.younger = true ;
	                                rs = cfdecti(argp,argl,&v) ;
	                                pip->younger = v ;
	                            }
	                        } else {
	                            rs = SR_INVALID ;
				}
	                        break ;

/* allow zero number of arguments */
	                    case 'z':
	                        pip->fin.zargs = true ;
	                        pip->fin.zargs = true ;
	                        pip->fl.zargs = true ;
	                        if (f_optequal) {
	                            f_optequal = false ;
	                            if (avl) {
	                                rs = optbool(avp,avl) ;
	                                pip->fl.zargs = (rs > 0) ;
	                            }
	                        }
	                        break ;

	                    case '?':
	                        f_usage = true ;
	                        break ;

	                    default:
	                        rs = SR_INVALID ;
	                        break ;

	                    } /* end switch */
	                    akp += 1 ;

	                    if (rs < 0) break ;
	                } /* end while */

	            } /* end if (individual option key letters) */

	        } /* end if (digits or progopts) */

	    } else {

	        rs = bits_set(&pargs,ai) ;
	        ai_max = ai ;

	    } /* end if (key letter/word or positional) */

	    ai_pos = ai ;

	} /* end while (all command line argument processing) */

	if (efname == nullptr) efname = getenv(VAREFNAME) ;
	if (efname == nullptr) efname = BFILE_STDERR ;
	if ((rs1 = bopen(&errfile,efname,"wca",0666)) >= 0) {
	    pip->efp = &errfile ;
	    pip->open.errfile = true ;
	    bcontrol(&errfile,BC_SETBUFLINE,true) ;
	} else if (! isFailOpen(rs1)) {
	    if (rs >= 0) rs = rs1 ;
	}

	if (pip->debuglevel > 0) {
	    bfile	*efp = (bfile *) pip->efp ;
	    if ((cp = getourenv(envv,VARDEBUGFNAME)) != nullptr) {
	        cchar	*pn = pip->progname ;
	        bprintf(efp,"%s: extended debugging=%s\n",pn,cp) ;
	    }
	}

	if (rs < 0) {
	    bfile	*efp = (bfile *) pip->efp ;
	    cchar	*fmt = "%s: invalid argument specified (%d)\n" ;
	    ex = EX_USAGE ;
	    bprintf(efp,fmt,pip->progname,rs) ;
	    usage(pip) ;
	    goto retearly ;
	}

	if (f_version || (pip->debuglevel > 0)) {
	    bfile	*efp = (bfile *) pip->efp ;
	    cchar	*pn = pip->progname ;
	    bprintf(efp,"%s: version %s\n",pn,VERSION) ;
	    if (pip->debuglevel > 0) {
	        bprintf(efp,"%s: debuglevel=%u\n",pn,pip->debuglevel) ;
	        bprintf(efp,"%s: extended (%d)\n",pn,rs2) ;
	    }
	}

/* get the program root */

	if ((rs = proginfo_setpiv(pip,pr,&initvars)) >= 0) {
	    rs = proginfo_setsearchname(pip,VARSEARCHNAME,sn) ;
	}

	if (rs < 0) {
	    ex = EX_OSERR ;
	    goto retearly ;
	}

#if	CF_DEBUG
	if (DEBUGLEVEL(4)) {
	    debugprintf("main: pr=%s\n",pip->pr) ;
	    debugprintf("main: sn=%s\n",pip->searchname) ;
	}
#endif

	if (pip->debuglevel > 0) {
	    bfile	*efp = (bfile *) pip->efp ;
	    bprintf(efp,"%s: pr=%s\n",pip->progname,pip->pr) ;
	    bprintf(efp,"%s: sn=%s\n",pip->progname,pip->searchname) ;
	} /* end if */

/* get our program mode */

	if (pmspec == nullptr) pmspec = pip->progname ;

	pip->progmode = matstr(prognames,pmspec,-1) ;

	if (pip->progmode < 0)
	    pip->progmode = progmode_filesize ;

#if	CF_DEBUG
	if (DEBUGLEVEL(4)) {
	    if (pip->progmode >= 0) {
	        debugprintf("main: progmode=%s(%u)\n",
	            prognames[pip->progmode],pip->progmode) ;
	    } else {
	        debugprintf("main: progmode=NONE\n") ;
	    }
	}
#endif /* CF_DEBUG */

	if (pip->debuglevel > 0) {
	    bfile	*efp = (bfile *) pip->efp ;
	    cchar	*pn = pip->progname ;
	    cchar	*fmt = "%s: progmode=%s(%u)\n" ;
	    bprintf(efp,fmt,pn,prognames[pip->progmode],pip->progmode) ;
	}

	if (f_usage)
	    usage(pip) ;

/* help file */

	if (f_help) {
	    printhelp(nullptr,pip->pr,pip->searchname,HELPFNAME) ;
	}

	if (f_version || f_help || f_usage)
	    goto retearly ;

	ex = EX_OK ;

/* initialization */

	if (afname == nullptr) afname = getenv(VARAFNAME) ;

	pip->daytime = time(nullptr) ;
	pip->euid = geteuid() ;
	pip->uid = getuid() ;

/* younger interval? */

	if ((rs >= 0) && (pip->younger == 0)) {
	    if (argval != nullptr) {
	        pip->have.younger = true ;
	        rs = cfdecti(argval,-1,&v) ;
	        pip->younger = v ;
	    }
	}

	if ((rs >= 0) && (pip->younger == 0)) {
	    if ((yfname != nullptr) && (yfname[0] != '\0')) {
	        if (ustat sb ; (rs = uc_stat(yfname,&sb)) >= 0) {
	            pip->have.younger = true ;
	            pip->younger = (pip->daytime - sb.st_mtime) ;
	        } else if (isNotPresent(rs)) {
	            rs = SR_OK ;
	        }
	    }
	}

#if	CF_DEBUG
	if (DEBUGLEVEL(2))
	    debugprintf("main: younger=%u\n",pip->younger) ;
#endif

	if (pip->debuglevel > 0) {
	    bfile	*efp = (bfile *) pip->efp ;
	    cchar	*pn = pip->progname ;
	    cchar	*fmt = "%s: younger=%u\n" ;
	    bprintf(efp,fmt,pn,pip->younger) ;
	}

/* get more program options */

	if (rs >= 0) {
	    if ((rs = procopts(pip,&akopts)) >= 0) {
	        if ((rs = procfts(pip)) >= 0) {
	            if (pip->fl.cores) pip->fts |= (1 << ft_r) ;
	            if ((rs >= 0) && (pip->debuglevel > 0)) {
	                rs = procprintfts(pip,po_fts) ;
	            }
	        }
	    }
	}

	if (rs >= 0) {
	    cchar	*vp = getourenv(envv,VARPRUNE) ;
	    if (vp != nullptr) {
	        paramopt	*pop = &pip->aparams ;
	        cchar		*po = po_prune ;
	        rs = paramopt_loads(pop,po,vp,-1) ;
	        pip->fl.prune |= (rs > 0) ;
	    }
	}

	if (pip->fl.f_noextra) {
	    pip->fl.f_nodev = true ;
	    pip->fl.f_nopipe = true ;
	    pip->fl.f_nosock = true ;
	    pip->fl.f_noname = true ;
	    pip->fl.f_nodoor = true ;
	}

/* create the 'fnos' value */

	if (rs >= 0) {
	    rs = loadfnos(pip) ;
	}

#if	CF_DEBUG
	if (DEBUGLEVEL(2)) {
	    debugprintf("main: f_follow=%u\n",pip->fl.follow) ;
	    debugprintf("main: f_nostop=%u\n",pip->fl.nostop) ;
	    debugprintf("main: f_iacc=%u\n",pip->fl.iacc) ;
	}
#endif /* CF_DEBUG */

	if (pip->debuglevel > 0) {
	    bfile	*efp = (bfile *) pip->efp ;
	    cchar	*pn = pip->progname ;
	    bprintf(efp,"%s: follow=%u\n",pn,pip->fl.follow) ;
	    bprintf(efp,"%s: nostop=%u\n",pn,pip->fl.nostop) ;
	    bprintf(efp,"%s: iacc=%u\n",pn,pip->fl.iacc) ;
	}

/* check a few more things */

	if (pip->tmpdname == nullptr) pip->tmpdname = getenv(VARTMPDNAME) ;
	if (pip->tmpdname == nullptr) pip->tmpdname = TMPDNAME ;

#ifdef	COMMENT
	if (pip->debuglevel > 0)
	    bfile	*efp = (bfile *) pip->efp ;
	    bprintf(efp,"%s: follow=%u\n",pip->progname,pip->fl.follow) ;
#endif

	if ((rs >= 0) && (pip->nice > 0)) {
	    int	n = MIN(pip->nice,19) ;
	    rs = u_nice(n) ;
	}

/* get ready */

	if (rs >= 0) {
	    if ((rs = paramopt_havekey(&pip->aparams,po_sufreq)) > 0) {
	        pip->have.sufreq = true ;
	        if (pip->debuglevel > 0) {
	            rs = procprintsufs(pip,po_sufreq) ;
		}
	    } /* end if */
	} /* end if */

/* continue */

	memclear(&ainfo,szof(ARGINFO)) ;
	ainfo.argc = argc ;
	ainfo.ai = ai ;
	ainfo.argv = argv ;
	ainfo.ai_max = ai_max ;
	ainfo.ai_pos = ai_pos ;

	if (rs >= 0) {
	    if ((rs = proctars_begin(pip)) >= 0) {
	        if ((rs = procsuf_begin(pip)) >= 0) {
	            if ((rs = procuniq_begin(pip)) >= 0) {
	                if ((rs = procprune_begin(pip,pfname)) >= 0) {
	                    {
	                        ARGINFO	*aip = &ainfo ;
	                        bits	*bop = &pargs ;
	                        cchar	*ofn = ofname ;
	                        cchar	*afn = afname ;
	                        rs = process(pip,aip,bop,ofn,afn) ;
	                    }
	                    rs1 = procprune_end(pip) ;
	                    if (rs >= 0) rs = rs1 ;
	                } /* end if (procprune) */
	                rs1 = procuniq_end(pip) ;
	                if (rs >= 0) rs = rs1 ;
	            } /* end if (procuniq) */
	            rs1 = procsuf_end(pip) ;
	            if (rs >= 0) rs = rs1 ;
	        } /* end if (procsuf) */
	        rs1 = proctars_end(pip) ;
	        if (rs >= 0) rs = rs1 ;
	    } /* end if (proctars) */
	} else if (ex == EX_OK) {
	    bfile	*efp = (bfile *) pip->efp ;
	    cchar	*pn = pip->progname ;
	    cchar	*fmt = "%s: invalid argument or configuration (%d)\n" ;
	    ex = EX_USAGE ;
	    bprintf(efp,fmt,pn,rs) ;
	    usage(pip) ;
	} /* end if (ok) */

	if (pip->debuglevel > 0) {
	    bfile	*efp = (bfile *) pip->efp ;
	    bprintf(efp,"%s: files=%u processed=%u\n",
	        pip->progname,pip->c_files,pip->c_processed) ;
	}

#ifdef	COMMENT
	if ((rs >= 0) && (pan == 0) && (! pip->fl.zargs) && (! pip->fl.quiet)) {
	    ex = EX_USAGE ;
	    bprintf(pip->efp,"%s: no files or directories were specified\n",
	        pip->progname) ;
	}
#endif /* COMMENT */

/* done */
	if ((rs < 0) && (ex == EX_OK)) {
	    bfile	*efp = (bfile *) pip->efp ;
	    switch (rs) {
	    case SR_INVALID:
	        ex = EX_USAGE ;
	        if (! pip->fl.quiet) {
		    cchar	*fmt = "%s: invalid usage (%d)\n" ;
	            bprintf(efp,fmt,pip->progname,rs) ;
	        }
	        break ;
	    case SR_NOENT:
	        ex = EX_CANTCREAT ;
	        break ;
	    case SR_AGAIN:
	        ex = EX_TEMPFAIL ;
	        break ;
	    default:
	        ex = mapex(mapexs,rs) ;
	        break ;
	    } /* end switch */
	} /* end if */

retearly:
	if (pip->debuglevel > 0) {
	    bfile	*efp = (bfile *) pip->efp ;
	    bprintf(efp,"%s: exiting ex=%u (%d)\n",pip->progname,ex,rs) ;
	}

#if	CF_DEBUG
	if (DEBUGLEVEL(2))
	    debugprintf("main: exiting ex=%u (%d)\n",ex,rs) ;
#endif

	if (pip->efp != nullptr) {
	    bfile	*efp = (bfile *) pip->efp ;
	    pip->open.errfile = true ;
	    bclose(efp) ;
	    pip->efp = nullptr ;
	}

	if (pip->open.aparams) {
	    pip->open.aparams = false ;
	    paramopt_finish(&pip->aparams) ;
	}

	if (pip->open.akopts) {
	    pip->open.akopts = false ;
	    keyopt_finish(&akopts) ;
	}

	bits_finish(&pargs) ;

badpargs:
	proginfo_finish(pip) ;

badprogstart:

#if	(CF_DEBUGS || CF_DEBUG) && CF_DEBUGMALL
	{
	    uint	mi[12] ;
	    uint	mo ;
	    uint	mdiff ;
	    uc_mallout(&mo) ;
	    mdiff = (mo-mo_start) ;
	    debugprintf("main: final mallout=%u\n",mdiff) ;
	    if (mdiff > 0) {
	        UCMALLREG_CUR	cur ;
	        UCMALLREG_REG	reg ;
	        cint	size = (10*szof(uint)) ;
	        cchar		*ids = "main" ;
	        uc_mallinfo(mi,size) ;
	        debugprintf("main: MIoutnum=%u\n",mi[ucmallreg_outnum]) ;
	        debugprintf("main: MIoutnummax=%u\n",mi[ucmallreg_outnummax]) ;
	        debugprintf("main: MIoutsize=%u\n",mi[ucmallreg_outsize]) ;
	        debugprintf("main: MIoutsizemax=%u\n",
	            mi[ucmallreg_outsizemax]) ;
	        debugprintf("main: MIused=%u\n",mi[ucmallreg_used]) ;
	        debugprintf("main: MIusedmax=%u\n",mi[ucmallreg_usedmax]) ;
	        debugprintf("main: MIunder=%u\n",mi[ucmallreg_under]) ;
	        debugprintf("main: MIover=%u\n",mi[ucmallreg_over]) ;
	        debugprintf("main: MInotalloc=%u\n",mi[ucmallreg_notalloc]) ;
	        debugprintf("main: MInotfree=%u\n",mi[ucmallreg_notfree]) ;
	        ucmallreg_curbegin(&cur) ;
	        while (ucmallreg_curenum(&cur,&reg) >= 0) {
	            debugprintf("main: MIreg.addr=%p\n",reg.addr) ;
	            debugprintf("main: MIreg.size=%u\n",reg.size) ;
	            debugprinthexblock(ids,80,reg.addr,reg.size) ;
	        }
	        ucmallreg_curend(&cur) ;
	    }
	    uc_mallset(0) ;
	}
#endif /* CF_DEBUGMALL */

#if	(CF_DEBUGS || CF_DEBUG)
	debugclose() ;
#endif

	sighand_finish(&sm) ;

badsighand:
	return ex ;
}
/* end subroutine (main) */


/* local subroutines */

local void main_sighand(int sn,siginfo_t *sip,void *vcp) noex {
	(void) sip ;
	(void) vcp ;
	switch (sn) {
	case SIGINT:
	    if_intr = true ;
	    break ;
	case SIGKILL:
	    if_exit = true ;
	    break ;
	default:
	    if_exit = true ;
	    break ;
	} /* end switch */
}
/* end subroutine (main_sighand) */

local int usage(PI *pip) noex {
	bfile		*efp = (bfile *) pip->efp ;
	int		rs = SR_OK ;
	int		wlen = 0 ;
	cchar		*pn = pip->progname ;
	cchar		*fmt ;

	fmt = "%s: USAGE> %s [<dir(s)> ...] [-af {<afile>|-}] \n" ;
	if (rs >= 0) rs = bprintf(efp,fmt,pn,pn) ;
	wlen += rs ;

	fmt = "%s:  [-f] [-c] [-ia] [-s <suffix(es)>]\n" ;
	if (rs >= 0) rs = bprintf(efp,fmt,pn) ;
	wlen += rs ;

	fmt = "%s:  [-sa <sufacc(s)>] [-sr <sufrej(s)>] [-t <type(s)>]\n" ;
	if (rs >= 0) rs = bprintf(efp,fmt,pn) ;
	wlen += rs ;

	fmt = "%s:  [-o <option(s)] [-prune <name(s)>]\n" ;
	if (rs >= 0) rs = bprintf(efp,fmt,pn) ;
	wlen += rs ;

	fmt = "%s:  [-Q] [-D] [-v[=<n>]] [-HELP] [-V]\n" ;
	if (rs >= 0) rs = bprintf(efp,fmt,pn) ;
	wlen += rs ;

	return (rs >= 0) ? wlen : rs ;
}
/* end subroutine (usage) */

local int procsig(PI *pip) noex {
	int		rs = SR_OK ;
	if (pip == nullptr) return SR_FAULT ;
	if (if_exit) {
	    rs = SR_EXIT ;
	} else if (if_intr) {
	    rs = SR_INTR ;
	}
	return rs ;
}
/* end subroutine (procsig) */

local int loadfnos(PI *pip) noex {
	if (pip->fl.f_nodev) {
	    bwset(pip->fnos,ft_c) ;
	    bwset(pip->fnos,ft_b) ;
	}
	if (pip->fl.f_noname) bwset(pip->fnos,ft_n) ;
	if (pip->fl.f_nopipe) bwset(pip->fnos,ft_p) ;
	if (pip->fl.f_nolink) bwset(pip->fnos,ft_l) ;
	if (pip->fl.f_nosock) bwset(pip->fnos,ft_s) ;
	if (pip->fl.f_nodoor) bwset(pip->fnos,ft_d) ;
	return SR_OK ;
}
/* end if (loadfnos) */

local int procopts(PI *pip,keyopt *kop) noex {
	keyopt_cur	kcur ;
	int		rs = SR_OK ;
	int		c = 0 ;
	cchar		*cp ;

	if ((cp = getenv(VAROPTS)) != nullptr) {
	    rs = keyopt_loads(kop,cp,-1) ;
	}

	if (rs >= 0) {
	    if ((rs = keyopt_curbegin(kop,&kcur)) >= 0) {
	        int	oi ;
	        int	kl, vl ;
	        int	v ;
	        cchar	*kp, *vp ;

	        while ((kl = keyopt_enumkeys(kop,&kcur,&kp)) >= 0) {

	            if ((oi = matostr(progopts,1,kp,kl)) >= 0) {

	                vl = keyopt_fetch(kop,kp,nullptr,&vp) ;

	                switch (oi) {
	                case progopt_uniq:
	                    if (! pip->fin.f_uniq) {
	                        pip->fin.f_uniq = true ;
	                        pip->have.f_uniq = true ;
	                        pip->fl.f_uniq = true ;
	                        if (vl > 0) {
	                            rs = optbool(vp,vl) ;
	                            pip->fl.f_uniq = (rs > 0) ;
	                        }
	                    }
	                    break ;
/* [what is this?] */
	                case progopt_name:
	                    if (! pip->fin.f_name) {
	                        pip->fin.f_name = true ;
	                        pip->fl.f_name = true ;
	                        if (vl > 0) {
	                            rs = optbool(vp,vl) ;
	                            pip->fl.f_name = (rs > 0) ;
	                        }
	                    }
	                    break ;
	                case progopt_prog:
	                    if (! pip->fin.f_prog) {
	                        pip->fin.f_prog = true ;
	                        pip->have.f_prog = true ;
	                        pip->fl.f_prog = true ;
	                        if (vl > 0) {
	                            rs = optbool(vp,vl) ;
	                            pip->fl.f_prog = (rs > 0) ;
	                        }
	                    }
			    break ;
	                case progopt_noprog:
	                    if (! pip->fin.f_noprog) {
	                        pip->fin.f_noprog = true ;
	                        pip->have.f_noprog = true ;
	                        pip->fl.f_noprog = true ;
	                        if (vl > 0) {
	                            rs = optbool(vp,vl) ;
	                            pip->fl.f_noprog = (rs > 0) ;
	                        }
	                    }
	                    break ;
	                case progopt_nosock:
	                    if (! pip->fin.f_nosock) {
	                        pip->fin.f_nosock = true ;
	                        pip->fl.f_nosock = true ;
	                        if (vl > 0) {
	                            rs = optbool(vp,vl) ;
	                            pip->fl.f_nosock = (rs > 0) ;
	                        }
	                    }
	                    break ;
	                case progopt_nopipe:
	                case progopt_nofifo:
	                    if (! pip->fin.f_nopipe) {
	                        pip->fin.f_nopipe = true ;
	                        pip->fl.f_nopipe = true ;
	                        if (vl > 0) {
	                            rs = optbool(vp,vl) ;
	                            pip->fl.f_nopipe = (rs > 0) ;
	                        }
	                    }
	                    break ;
	                case progopt_nodev:
	                    if (! pip->fin.f_nodev) {
	                        pip->fin.f_nodev = true ;
	                        pip->fl.f_nodev = true ;
	                        if (vl > 0) {
	                            rs = optbool(vp,vl) ;
	                            pip->fl.f_nodev = (rs > 0) ;
	                        }
	                    }
	                    break ;
	                case progopt_noname:
	                    if (! pip->fin.f_noname) {
	                        pip->fin.f_noname = true ;
	                        pip->fl.f_noname = true ;
	                        if (vl > 0) {
	                            rs = optbool(vp,vl) ;
	                            pip->fl.f_noname = (rs > 0) ;
	                        }
	                    }
	                    break ;
	                case progopt_nolink:
	                    if (! pip->fin.f_nolink) {
	                        pip->fin.f_nolink = true ;
	                        pip->fl.f_nolink= true ;
	                        if (vl > 0) {
	                            rs = optbool(vp,vl) ;
	                            pip->fl.f_nolink = (rs > 0) ;
	                        }
	                    }
	                    break ;
	                case progopt_noextra:
	                    if (! pip->fin.f_noextra) {
	                        pip->fin.f_noextra = true ;
	                        pip->fl.f_noextra = true ;
	                        if (vl > 0) {
	                            rs = optbool(vp,vl) ;
	                            pip->fl.f_noextra = (rs > 0) ;
	                        }
	                    }
	                    break ;
	                case progopt_nodotdir:
	                    if (! pip->fin.f_nodotdir) {
	                        pip->fin.f_nodotdir = true ;
	                        pip->fl.f_nodotdir = true ;
	                        if (vl > 0) {
	                            rs = optbool(vp,vl) ;
	                            pip->fl.f_nodotdir = (rs > 0) ;
	                        }
	                    }
	                    break ;
	                case progopt_cores:
	                    if (! pip->fin.cores) {
	                        pip->fin.cores = true ;
	                        pip->fl.cores = true ;
	                        if (vl > 0) {
	                            rs = optbool(vp,vl) ;
	                            pip->fl.cores = (rs > 0) ;
	                        }
	                    }
	                    break ;
	                case progopt_summary:
	                    if (! pip->fin.summary) {
	                        pip->fin.summary = true ;
	                        pip->fl.summary = true ;
	                        if (vl > 0) {
	                            rs = optbool(vp,vl) ;
	                            pip->fl.summary = (rs > 0) ;
	                        }
	                    }
	                    break ;
/* ignore assembly comments */
	                case progopt_ignasscomm:
	                    if (! pip->fin.ignasscomm) {
	                        pip->fin.ignasscomm = true ;
	                        pip->fl.ignasscomm = true ;
	                        if (vl > 0) {
	                            rs = optbool(vp,vl) ;
	                            pip->fl.ignasscomm = (rs > 0) ;
	                        }
	                    }
	                    break ;
/* default action and user specified help */
	                case progopt_s:
	                    if ((vl > 0) && (! pip->fin.sufreq)) {
	                        rs = procsuf_load(pip,suf_req,vp,vl) ;
	                    }
	                    break ;
	                case progopt_sa:
	                    if ((vl > 0) && (! pip->fin.sufacc)) {
	                        rs = procsuf_load(pip,suf_acc,vp,vl) ;
	                    }
	                    break ;
	                case progopt_sr:
	                case progopt_nosuf:
	                    if ((vl > 0) && (! pip->fin.sufrej)) {
	                        rs = procsuf_load(pip,suf_rej,vp,vl) ;
	                    }
	                    break ;
	                case progopt_prune:
	                    if (vl > 0) {
	                        paramopt	*pop = &pip->aparams ;
	                        cchar		*po = po_prune ;
	                        rs = paramopt_loads(pop,po,vp,vl) ;
	                        pip->fl.prune |= (rs > 0) ;
	                    }
	                    break ;
	                case progopt_younger:
	                case progopt_yi:
	                    if ((vl > 0) && (! pip->fin.younger)) {
	                        pip->fin.younger = true ;
	                        pip->have.younger = true ;
	                        rs = cfdecti(vp,vl,&v) ;
	                        pip->younger = v ;
	                    }
	                    break ;
	                case progopt_nice:
	                    if ((vl > 0) && (! pip->fin.nice)) {
	                        pip->fin.nice = true ;
	                        pip->have.nice = true ;
	                        rs = optvalue(vp,vl) ;
	                        pip->nice = rs ;
	                    }
	                    break ;
	                case progopt_igncomm:
	                    if (! pip->fin.igncomm) {
	                        pip->fin.igncomm = true ;
	                        pip->fl.igncomm = true ;
	                        if (vl > 0) {
	                            cchar	*cp ;
	                            if (sfshrink(vp,vl,&cp) > 0) {
	                                pip->cch = MKCHAR(cp[0]) ;
	                            }
	                        }
	                    }
	                    break ;
	                case progopt_follow:
	                    if (! pip->fin.follow) {
	                        pip->fin.follow = true ;
	                        pip->have.follow = true ;
	                        pip->fl.follow = true ;
	                        if (vl > 0) {
	                            rs = optbool(vp,vl) ;
	                            pip->fl.follow = (rs > 0) ;
	                        }
	                    }
	                    break ;
	                case progopt_iacc:
	                case progopt_ignacc:
	                    if (! pip->fin.iacc) {
	                        pip->fin.iacc = true ;
	                        pip->fl.iacc = true ;
	                        if (vl > 0) {
	                            rs = optbool(vp,vl) ;
	                            pip->fl.iacc = (rs > 0) ;
	                        }
	                    }
	                    break ;
	                case progopt_im:
	                    if (! pip->fin.im) {
	                        pip->fin.im = true ;
	                        pip->fl.im = true ;
	                        if (vl > 0) {
	                            rs = optbool(vp,vl) ;
	                            pip->fl.im = (rs > 0) ;
	                        }
	                    }
	                    break ;
	                case progopt_quiet:
	                    if (! pip->fin.quiet) {
	                        pip->fin.quiet = true ;
	                        pip->have.quiet = true ;
	                        pip->fl.quiet = true ;
	                        if (vl > 0) {
	                            rs = optbool(vp,vl) ;
	                            pip->fl.quiet = (rs > 0) ;
	                        }
	                    }
	                    break ;
	                case progopt_ff:
	                    if (! pip->fin.ff) {
	                        pip->fin.ff = true ;
	                        pip->have.ff = true ;
	                        pip->fl.ff = true ;
	                        if (vl > 0) {
	                            rs = optbool(vp,vl) ;
	                            pip->fl.ff = (rs > 0) ;
	                        }
	                    }
	                    break ;
	                case progopt_readable:
	                    if (! pip->fin.readable) {
	                        pip->fin.readable = true ;
	                        pip->have.readable = true ;
	                        pip->fl.readable = true ;
	                        if (vl > 0) {
	                            rs = optbool(vp,vl) ;
	                            pip->fl.readable = (rs > 0) ;
	                        }
	                    }
	                    break ;
	                case progopt_rmfile:
	                    if (! pip->fin.rmfile) {
	                        pip->fin.rmfile = true ;
	                        pip->have.rmfile = true ;
	                        pip->fl.rmfile = true ;
	                        if (vl > 0) {
	                            rs = optbool(vp,vl) ;
	                            pip->fl.rmfile = (rs > 0) ;
	                        }
	                    }
	                    break ;
	                case progopt_older:
	                    if (! pip->fin.older) {
	                        pip->fin.older = true ;
	                        if (vl > 0) {
	                            rs = cfdecti(vp,vl,&v) ;
	                            pip->older = v ;
	                        }
	                    }
	                    break ;
	                case progopt_accessed:
	                    if (! pip->fin.accessed) {
	                        pip->fin.accessed = true ;
	                        if (vl > 0) {
	                            rs = cfdecti(vp,vl,&v) ;
	                            pip->accessed = v ;
	                        }
	                    }
	                    break ;
	                } /* end switch */

	                c += 1 ;
	            } else {
	                rs = SR_INVALID ;
		    }

	            if (rs < 0) break ;
	        } /* end while */

	        keyopt_curend(kop,&kcur) ;
	    } /* end if (keyopts-cursor) */
	} /* end if (ok) */

#if	CF_DEBUG
	if (DEBUGLEVEL(4))
	    debugprintf("procopts: ret rs=%d c=%u\n",rs,c) ;
#endif

	return (rs >= 0) ? c : rs ;
}
/* end subroutine (procopts) */

local int procfts(PI *pip) noex {
	paramopt	*pop = &pip->aparams ;
	paramopt_cur	cur ;
	int		rs = SR_OK ;
	int		vl ;
	int		fti ;
	int		c = 0 ;
	cchar		*varftypes = getenv(VARFTYPES) ;
	cchar		*vp ;

	if (varftypes != nullptr) {
	    rs = paramopt_loads(pop,po_fts,varftypes,-1) ;
	}

	if (rs >= 0) {
	    if ((rs = paramopt_curbegin(pop,&cur)) >= 0) {
	        while (rs >= 0) {
	            vl = paramopt_curfetch(pop,po_fts,&cur,&vp) ;
	            if (vl == SR_NOTFOUND) break ;
	            rs = vl ;
	            if (rs < 0) break ;

	            if (vl > 0) {
	                if ((fti = matostr(ftstrs,1,vp,vl)) >= 0) {
	                    switch (fti) {
	                    case ftstr_regular:
	                    case ftstr_file:
	                        pip->fts |= (1 << ft_r) ;
	                        break ;
	                    case ftstr_directory:
	                        pip->fts |= (1 << ft_d) ;
	                        break ;
	                    case ftstr_block:
	                        pip->fts |= (1 << ft_b) ;
	                        break ;
	                    case ftstr_character:
	                        pip->fts |= (1 << ft_c) ;
	                        break ;
	                    case ftstr_pipe:
	                    case ftstr_fifo:
	                        pip->fts |= (1 << ft_p) ;
	                        break ;
	                    case ftstr_socket:
	                        pip->fts |= (1 << ft_s) ;
	                        break ;
	                    case ftstr_link:
	                        pip->fts |= (1 << ft_l) ;
	                        break ;
	                    case ftstr_name:
	                        pip->fts |= (1 << ft_n) ;
	                        break ;
	                    case ftstr_door:
	                        pip->fts |= (1 << ft_D) ;
	                        break ;
	                    case ftstr_exists:
	                        pip->fts |= (1 << ft_e) ;
	                        break ;
	                    } /* end switch */
	                    c += 1 ;
	                } /* end if (matostr) */
	            } /* end if (positive) */

	        } /* end while */
	        paramopt_curend(pop,&cur) ;
	    } /* end if (cursor) */
	} /* end if (ok) */

	return (rs >= 0) ? c : rs ;
}
/* end subroutine (procfts) */

local int process(PI *pip,ARGINFO *aip,bits *bop,cchar *ofn,cchar *afn) noex {
	bfile		ofile, *ofp = &ofile ;
	int		rs ;
	int		rs1 ;
	cchar		*pn = pip->progname ;
	cchar		*fmt ;

	if ((ofn == nullptr) || (ofn[0] == '\0') || (ofn[0] == '-')) {
	    ofn = BFILE_STDOUT ;
	}

	if ((rs = bopen(ofp,ofn,"wct",0644)) >= 0) {
	    pip->ofp = (bfile *) ofp ;

#if	CF_DIRS
	    if (rs >= 0) {
	        if (pip->fl.follow || pip->fl.f_uniq) {
	            rs = procdir_begin(pip) ;
	        }
	    }
#endif /* CF_DIRS */

	    if (rs >= 0) {
	        switch (pip->progmode) {
	        case progmode_filesyncer:
	            rs = proclink_begin(pip) ;
	            break ;
	        case progmode_filerm:
	            rs = procrm_begin(pip) ;
	            break ;
	        case progmode_filelines:
	            rs = proclines_begin(pip) ;
	            break ;
	        } /* end switch */
	    } /* end if */

	    if (rs >= 0) {
	        rs = procargs(pip,aip,bop,afn) ;
	    }

	    {
	        switch (pip->progmode) {
	        case progmode_filesyncer:
	            rs1 = proclink_end(pip) ;
	            break ;
	        case progmode_filerm:
	            rs1 = procrm_end(pip) ;
	            break ;
	        case progmode_filelines:
	            rs1 = proclines_end(pip) ;
	            break ;
	        } /* end switch */
	        if (rs >= 0) rs = rs1 ;
	    } /* end block */

#if	CF_DIRS
	    if (pip->fl.follow || pip->fl.f_uniq) {
	        rs1 = procdir_end(pip) ;
	        if (rs >= 0) rs = rs1 ;
	    }
#endif /* CF_DIRS */

	    pip->ofp = nullptr ;
	    rs1 = bclose(ofp) ;
	    if (rs >= 0) rs = rs1 ;
	} else {
	    bfile	*efp = (bfile *) pip->efp ;
	    fmt = "%s: inaccessible output (%d)\n" ;
	    bprintf(efp,fmt,pn,rs) ;
	    bprintf(efp,"%s: ofile=%s\n",pn,ofn) ;
	}

	return rs ;
}
/* end subroutine (process) */

local int procargs(PI *pip,ARGINFO *aip,bits *bop,cchar *afn) noex {
	bfile		*efp = (bfile *) pip->efp ;
	int		rs = SR_OK ;
	int		rs1 ;
	int		cl ;
	int		pan = 0 ;
	cchar		*pn = pip->progname ;
	cchar		*fmt ;
	cchar		*cp ;

	if (rs >= 0) {
	    int		ai ;
	    int		f ;
	    for (ai = 1 ; ai < aip->argc ; ai += 1) {

	        f = (ai <= aip->ai_max) && (bits_test(bop,ai) > 0) ;
	        f = f || ((ai > aip->ai_pos) && (aip->argv[ai] != nullptr)) ;
	        if (f) {
	            cp = aip->argv[ai] ;
	            if (cp[0] != '\0') {
	                if ((rs = procsig(pip)) >= 0) {
	                    pan += 1 ;
	                    rs = procname(pip,cp) ;
	                    if (rs < 0) {
	                        fmt = "%s: error name=%s (%d)\n" ;
	                        bprintf(efp,fmt,pn,cp,rs) ;
	                    }
	                } /* end if (procsig) */
	            }
	        }

	        if (rs < 0) break ;
	    } /* end for (looping through requested circuits) */
	} /* end if (ok) */

/* process any files in the argument filename list file */

	if ((rs >= 0) && (afn != nullptr) && (afn[0] != '\0')) {
	    bfile	afile, *afp = &afile ;

	    if (strcmp(afn,"-") == 0) afn = BFILE_STDIN ;

	    if ((rs = bopen(afp,afn,"r",0666)) >= 0) {
	        cint	llen = LINEBUFLEN ;
	        int		len ;
	        char		lbuf[LINEBUFLEN + 1] ;

	        while ((rs = breadln(afp,lbuf,llen)) > 0) {
	            len = rs ;

	            if (lbuf[len - 1] == '\n') len -= 1 ;
	            lbuf[len] = '\0' ;

	            if ((cl = sfskipwhite(lbuf,len,&cp)) > 0) {
	                if (cp[0] != '#') {
	                    if ((rs = procsig(pip)) >= 0) {
	                        pan += 1 ;
	                        lbuf[((cp-lbuf)+cl)] = '\0' ;
	                        rs = procnames(pip,cp,cl) ;
	                    } /* end if (procsig) */
	                }
	            }

	            if (rs < 0) {
	                fmt = "%s: error name=%s (%d)\n" ;
	                bprintf(efp,fmt,pn,cp,rs) ;
	            }

	            if (rs < 0) break ;
	        } /* end while (reading lines) */

	        rs1 = bclose(afp) ;
	        if (rs >= 0) rs = rs1 ;
	    } else {
	        if (! pip->fl.quiet) {
	            fmt = "%s: inacessible argument-list (%d)\n" ;
	            bprintf(efp,fmt,pn,rs) ;
	            bprintf(efp,"%s: afile=%s\n",pn,afn) ;
	        }
	    } /* end if */

	} /* end if (processing file argument file list) */

	if ((rs >= 0) && (pip->progmode == progmode_filesize)) {
	    if ((pip->verboselevel > 0) && (! pip->fl.zargs)) {
	        bfile	*ofp = (bfile *) pip->ofp ;
	        long	blocks, blockbytes ;

	        fmt = "%lu megabyte%s and %lu bytes\n" ;
	        bprintf(ofp,fmt,
	            pip->megabytes,
	            ((pip->megabytes == 1) ? "" : "s"),
	            pip->bytes) ;

/* calculate UNIX® blocks */

	        blocks = pip->megabytes * 1024 * 2 ;
	        blocks += (pip->bytes / UNIXBLOCK) ;
	        blockbytes = (pip->bytes % UNIXBLOCK) ;

	        fmt = "%lu UNIX® blocks and %lu bytes\n" ;
	        bprintf(ofp,fmt,blocks,blockbytes) ;

	    } /* end if (verbosity allowed) */
	} /* end if (program mode=filesize) */

	return rs ;
}
/* end subroutine (procargs) */

local int procnames(PI *pip,cchar *lbuf,int llen) noex {
	field		fsb ;
	int		rs ;
	int		rs1 ;
	int		c = 0 ;
	if (pip == nullptr) return SR_FAULT ;
	if ((rs = field_start(&fsb,lbuf,llen)) >= 0) {
	    int		fl ;
	    cchar	*fp ;
	    while ((fl = field_get(&fsb,aterms,&fp)) >= 0) {
	        if (fl > 0) {
		    nulstr	n ;
		    cchar	*name ;
		    if ((rs = nulstr_start(&n,fp,fl,&name)) >= 0) {
			{
	     	        rs = procname(pip,name) ;
	                c += rs ;
			}
			rs1 = nulstr_finish(&n) ;
			if (rs >= 0) rs = rs1 ;
		    } /* end if (nulstr) */
	        } /* end if (positive) */
	        if (fsb.term == '#') break ;
	        if (rs < 0) break ;
	    } /* end while */
	    rs1 = field_finish(&fsb) ;
	    if (rs >= 0) rs = rs1 ;
	} /* end if (field) */
	return (rs >= 0) ? c : rs ;
}
/* end subroutine (procnames) */

int procname(PI *pip,cchar *name) noex {
	ustat		sb, ssb, *sbp = &sb ;
	int		rs = SR_OK ;
	int		f_go = true ;
	int		f_islink = false ;
	int		f_isdir = false ;
	char		tmpfname[MAXPATHLEN+1] ;

	if (name == nullptr) return SR_FAULT ;

	if (name[0] == '\0') return SR_INVALID ;

#if	CF_DEBUG
	if (DEBUGLEVEL(3))
	    debugprintf("main/procname: ent name=%s\n",name) ;
#endif

	if ((name[0] == '/') && (name[1] == 'u')) {
	    rs = mkpathuser(tmpfname,nullptr,name,-1) ;
	    if (rs > 0) name = tmpfname ;
	}

	if (rs >= 0) {
	    rs = uc_lstat(name,&sb) ;
	}

#if	CF_DEBUG
	if (DEBUGLEVEL(3)) {
	    if (rs < 0) sb.st_mode = 0 ;
	    debugprintf("main/procname: uc_lstat() rs=%d mode=%0o\n", 
	        rs,sb.st_mode) ;
	}
#endif

	if (pip->fl.im && isNotStat(rs)) {
	    rs = SR_OK ;
	    f_go = false ;
	}

	f_isdir = S_ISDIR(sb.st_mode) ;
	f_islink = S_ISLNK(sb.st_mode) ;

#if	CF_DEBUG
	if (DEBUGLEVEL(3)) {
	    debugprintf("main/procname: f_isdir=%u\n",f_isdir) ;
	    debugprintf("main/procname: f_islink=%u\n",f_islink) ;
	}
#endif

	if ((rs >= 0) && f_go && f_islink && (pip->fl.follow || pip->fl.ff)) {

	    if ((rs = uc_stat(name,&ssb)) >= 0) {
	        f_isdir = S_ISDIR(ssb.st_mode) ;
	        f_islink = S_ISLNK(sb.st_mode) ;
	        sbp = &ssb ;
	    } else if (isNotPresent(rs)) {
	        rs = SR_OK ;
	    } else if (rs == SR_LOOP) {
	        rs = SR_OK ;
	        if (! pip->fl.quiet) {
	    	    bfile	*efp = (bfile *) pip->efp ;
	            cchar	*pn = pip->progname ;
	            cchar	*fmt ;
	            fmt = "%s: symbolic link exhaustion\n" ;
	            bprintf(efp,fmt,pn) ;
	            fmt = "%s: name=%s\n" ;
	            bprintf(efp,fmt,pn,name) ;
	        }
	    }

#if	CF_DEBUG
	    if (DEBUGLEVEL(3))
	        debugprintf("main/procname: symlink dangle=%u\n",
	            ((rs < 0)?1:0)) ;
#endif

	} /* end if */

	if ((rs >= 0) && f_go) {
	    if (pip->fl.readable && (! f_islink)) {
	        if ((rs = uc_access(name,R_OK)),isNotAccess(rs)) {
	            f_go = false ;
	            rs = SR_OK ;
	        }
	    }
	    if ((rs >= 0) && f_go && pip->fl.prune) {
	        rs = procprune(pip,name) ;
	        f_go = (rs > 0) ;
	    }
	    if ((rs >= 0) && f_go) {
	        if (f_isdir) {
	            rs = procdir(pip,name,sbp) ;
	        } else {
	            rs = procother(pip,name,sbp) ;
	        }
	    } /* end if */
	} /* end if (go) */

#if	CF_DEBUG
	if (DEBUGLEVEL(3))
	    debugprintf("main/procname: ret rs=%d\n",rs) ;
#endif

	return rs ;
}
/* end subroutine (procname) */

local int procdir(PI *pip,cchar *np,ustat *sbp) noex {
	int		rs = SR_OK ;
	int		nl = lenstr(np) ;
	int		c = 0 ;
	int		f_cont = true ;

#if	CF_DEBUG
	if (DEBUGLEVEL(4))
	    debugprintf("main/procdir: ent name=%s\n",np) ;
#endif

	while ((nl > 0) && (np[nl-1] == '/')) nl -= 1 ;

	if (pip->fl.f_nodotdir && (nl > 0)) {
	    if (np[0] == '.') f_cont = false ;
	}

/* optionally register ourselves */

#if	CF_DIRS
	if (f_cont) {
	    if (pip->fl.follow || pip->fl.f_uniq) {
	        const dev_t	dev = sbp->st_dev ;
	        const ino_t	ino = sbp->st_ino ;
	        int		f = true ;
	        if ((rs = procdir_have(pip,dev,ino,np,nl)) == 0) {
	            f = false ;
	            if ((rs = procdir_haveprefix(pip,np,nl)) > 0) {
	                f = true ;
	                rs = procdir_addprefix(pip,np,nl) ;
	            }
	        }
	        if ((rs >= 0) && f) f_cont = false ;
	    }
	} /* end if (continue) */
#endif /* CF_DIRS */

	if ((rs >= 0) && f_cont) {
	    if ((rs = procdirs(pip,np,nl,sbp)) >= 0) {
	        c += rs ;
	        rs = procother(pip,np,sbp) ;
	        c += rs ;
	    }
	}

#if	CF_DEBUG
	if (DEBUGLEVEL(4))
	    debugprintf("main/procdir: ret rs=%d c=%u\n",rs,c) ;
#endif

	return (rs >= 0) ? c : rs ;
}
/* end subroutine (procdir) */

local int procdirs(PI *pip,cchar *sp,int nl,ustat *sbp) noex {
	bfile		*efp = (bfile *) pip->efp ;
	cint		sz = (nl + 1 + MAXPATHLEN + 1) ;
	int		rs ;
	int		rs1 ;
	int		c = 0 ;
	char		*p ;
#if	CF_DEBUG
	if (DEBUGLEVEL(4))
	    debugprintf("main/procdirs: ent\n") ;
#endif
	if ((rs = lm_mall(sz,&p)) >= 0) {
	    ustat	fsb ;
	    int		opts = 0 ;
	    cchar	*pn = pip->progname ;
	    cchar	*fmt ;
	    char	*fname = p ;
	    char	*bp ;

	    bp = strwcpy(fname,sp,nl) ;
	    *bp++ = '/' ;

	    if (pip->fl.follow) {
	        opts |= FSDIRTREE_MFOLLOW ;
	        opts |= FSDIRTREE_MUNIQDIR ;
	    }
	    if (pip->fl.f_uniq) {
	        opts |= FSDIRTREE_MUNIQDIR ;
	    }

	    if (fsdirtree d ; (rs = fsdirtree_open(&d,sp,opts)) >= 0) {
	        cint	mpl = MAXPATHLEN ;
	        if (pip->fl.prune) {
	            rs = fsdirtree_prune(&d,pip->prune) ;
	        }
	        if (rs >= 0) {
	            while ((rs = fsdirtree_read(&d,&fsb,bp,mpl)) > 0) {
	                if ((rs = procsig(pip)) >= 0) {
	                    int		f_go = true ;
#if	CF_DEBUG
	                    if (DEBUGLEVEL(4))
	                        debugprintf("main/procdirs: direntry=%s\n",bp) ;
#endif
	                    if (pip->debuglevel >= 2) {
	                        fmt = "%s: looking fn=%s (%d)\n" ;
	                        bprintf(efp,fmt,pn,fname,rs) ;
	                    }
	                    if ((rs >= 0) && f_go) {
	                        c += 1 ;
	                        rs = procother(pip,fname,&fsb) ;
	                        if (rs >= 0) c += 1 ;
	                    }
#if	CF_DEBUG
	                    if (DEBUGLEVEL(4))
	                        debugprintf("main/procdirs: while-bot rs=%d\n",
	                            rs) ;
#endif
	                } /* end if (procsig) */
	                if (rs < 0) break ;
	            } /* end while (reading entries) */
#if	CF_DEBUG
	            if (DEBUGLEVEL(4))
	                debugprintf("main/procdirs: while-out rs=%d\n",rs) ;
#endif
	        } /* end if (ok) */
	        rs1 = fsdirtree_close(&d) ;
	        if (rs >= 0) rs = rs1 ;
	    } else if (isNotAccess(rs)) {
	        if (! pip->fl.quiet) {
	            fmt = "%s: no-access dir=%s (%d)\n" ;
	            bprintf(efp,fmt,pn,np,rs) ;
	        }
	        if (pip->fl.iacc) rs = SR_OK ;
	    } /* end if (fsdirtree) */

#if	CF_DEBUG
	    if (DEBUGLEVEL(4))
	        debugprintf("main/procdirs: dir-out rs=%d\n",rs) ;
#endif

	    rs1 = lm_free(p) ;
	    if (rs >= 0) rs = rs1 ;
	} /* end if (memory allocation) */
#if	CF_DEBUG
	if (DEBUGLEVEL(4))
	    debugprintf("main/procdirs: ret rs=%d c=%u\n",rs,c) ;
#endif
	return (rs >= 0) ? c : rs ;
}
/* end subroutine (procdirs) */

local int procother(PI *pip,cchar *name,ustat *sbp) noex {
	FILEINFO	ck, *ckp = &ck ;
#if	CF_FOLLOWFILES
	ustat		ssb ;
#endif
	int		rs = SR_OK ;
	int		rs1 ;
	int		bnl = 0 ;
	int		f_process = (sbp->st_ctime > 0) ;
	int		f_accept = false ;
	int		f_islink = false ;
	int		f_suf ;
	cchar		*bnp ;

	memclear(ckp,szof(FILEINFO)) ;

#if	CF_DEBUG
	if (DEBUGLEVEL(4)) {
	    cchar	*cp = strfiletype(sbp) ;
	    debugprintf("main/procother: ent name=%s ft=%s\n",name,cp) ;
	    debugprintf("main/procother: f_prog=%u\n",pip->fl.f_prog) ;
	}
#endif /* CF_DEBUG */

	pip->c_files += 1 ;

/* fill in some information */

	if (f_process) {
	    fileinfo_loadfts(ckp,sbp) ;
	}

#if	CF_DEBUG
	if (DEBUGLEVEL(3)) {
	    int	i ;
	    for (i = 0 ; i < ft_overlast ; i += 1) {
	        if (BATSTI(&ckp->fts,i))
	            debugprintf("main/procother: ft[%u]=true\n",i) ;
	    }
	}
#endif /* CF_DEBUG */

/* check age */

	if (f_process && (pip->younger > 0)) {
	    if ((pip->daytime - sbp->st_mtime) >= pip->younger) {
	        f_process = false ;
	    }
	}

	if (f_process && (pip->older > 0)) {
	    if ((pip->daytime - sbp->st_mtime) < pip->older) {
	        f_process = false ;
	    }
	}

	if (f_process && (pip->accessed > 0)) {
	    if ((pip->daytime - sbp->st_atime) < pip->accessed) {
	        f_process = false ;
	    }
	}

	if (f_process && (pip->fts > 0) && ((ckp->fts & pip->fts) == 0)) {
	    f_process = false ;
	}

	if (f_process && (pip->fnos > 0) && ((ckp->fts & pip->fnos) != 0)) {
	    f_process = false ;
	}

/* if this is a file link, see if it is a directory */

#if	CF_DEBUG
	if (DEBUGLEVEL(4))
	    debugprintf("main/procother: symbolic link check?\n") ;
#endif

	if (f_process && S_ISLNK(sbp->st_mode) && pip->fl.f_nolink) {
	    f_process = false ;
	}

	if ((rs >= 0) && f_process && S_ISLNK(sbp->st_mode)) {
	    f_islink = true ;

#if	CF_DEBUG
	    if (DEBUGLEVEL(4))
	        debugprintf("main/procother: symbolic_link YES\n") ;
#endif

#if	CF_FOLLOWFILES
	    if (pip->fl.follow) {
	        sbp = &ssb ;
	        if ((rs = uc_stat(name,&ssb)) >= 0) { /* STAT */
	            f_islink = S_ISLNK(ssb.st_mode) ;
#if	CF_DEBUG
	            if (DEBUGLEVEL(4))
	                debugprintf("main/procother: uc_stat() rs=%d\n",rs) ;
#endif
	        } else if (isNotPresent(rs)) {
#if	CF_DEBUG
	            if (DEBUGLEVEL(4))
	                debugprintf("main/procother: symlink DANGLING\n") ;
#endif
	            if (! pip->fl.nostop) f_process = false ;
	            rs = SR_OK ;
	        }
	    } /* end if (follow link) */
#endif /* CF_FOLLOWFILES */
	} /* end if (symbolic-link-file) */

	if ((rs >= 0) && f_process && pip->fl.f_uniq) {
	    dev_t	dev = sbp->st_dev ;
	    ino_t	ino = sbp->st_ino ;
	    if ((rs = procuniq_have(pip,dev,ino)) > 0) {
	        f_process = false ;
	    }
	}

#if	CF_DIRS
	if ((rs >= 0) && f_process && (pip->fl.follow || pip->fl.f_uniq)) {
	    cint	nl = lenstr(name) ;
	    int		f = true ;
	    if ((rs = procdir_haveprefix(pip,name,nl)) >= 0) {
	        f = (rs > 0) ;
	        if (S_ISDIR(sbp->st_mode)) {
	            dev_t	dev = sbp->st_dev ;
	            ino_t	ino = sbp->st_ino ;
	            if ((rs = procdir_have(pip,dev,ino,name,nl)) > 0) {
	                f = true ;
	            }
	        } /* end if (is-directory) */
	        if ((rs >= 0) && f) {
	            rs = procdir_addprefix(pip,name,nl) ;
	        }
	    } /* end if (does not have disallowed prefix) */
	    f_process = (!f) ;
	} /* end if (uniqueness check) */
#endif /* CF_DIRS */

/* prepare for suffix checks */

	f_suf = (pip->have.sufreq || pip->fl.sufacc || pip->fl.sufrej) ;

#if	CF_DEBUG
	if (DEBUGLEVEL(4))
	    debugprintf("main/procother: f_suf=%u\n",f_suf) ;
#endif

	if (f_suf || pip->fl.cores) {
	    bnl = sfbasename(name,-1,&bnp) ;
	}

	if (f_process && f_suf) {
	    if (bnl <= 0) f_suf = false ;
	    if (f_suf && (bnl > 0) && (bnp[0] == '.')) {
	        if ((bnl == 1) || ((bnl == 2) && (bnp[1] == '.'))) {
	            f_process = false ;
	        }
	    } /* end if */
	} /* end if (funny name check) */

/* check if it has a suffix already */

	if ((rs >= 0) && f_process && f_suf) {
	    vecpstr	*slp ;
	    int		sl ;
	    cchar	*tp, *sp ;
	    if ((tp = strnrchr(bnp,bnl,'.')) != nullptr) {
	        sp = (tp+1) ;
	        sl = ((bnp+bnl)-(tp+1)) ;

#if	CF_DEBUG
	        if (DEBUGLEVEL(4))
	            debugprintf("main/procother: suf=%r\n",sp,sl) ;
#endif

/* check against the suffix-required list */

	        if (pip->have.sufreq) {
	            if ((rs = procsuf_have(pip,sp,sl)) > 0) {
	                f_accept = true ;
	            } else if (rs == 0) {
	                f_process = false ;
	            }
	        } else {

/* check against the suffix-acceptance list */

	            if ((rs >= 0) && f_process) {
		        if (pip->fl.sufacc && (! f_accept)) {
	                    slp = (pip->sufs + suf_acc) ;
	                    if ((rs = vecpstr_findn(slp,sp,sl)) >= 0) {
	                        f_accept = true ;
	                    } else if (rs == SR_NOTFOUND) {
	                        rs = SR_OK ;
	                    }
		        }
	            } /* end if */

/* check against the suffix-rejectance list */

	            if ((rs >= 0) && f_process) {
		        if (pip->fl.sufrej && (! f_accept)) {
	                    slp = (pip->sufs + suf_rej) ;
	                    if ((rs1 = vecpstr_findn(slp,sp,sl)) >= 0) {
	                        f_process = false ;
	                    } else if (rs == SR_NOTFOUND) {
	                        rs = SR_OK ;
		            }
	                }
	            } /* end if */

		} /* end if (suffix required or not) */

#if	CF_DEBUG
	        if (DEBUGLEVEL(4))
	            debugprintf("main/procother: suf fa=%u f_process=%u\n",
	                f_accept,f_process) ;
#endif

	    } else {
	        f_suf = false ;
	        if (pip->have.sufreq) f_process = false ;
	    }
	} /* end if (suffix lists) */

#if	CF_DEBUG
	if (DEBUGLEVEL(4))
	    debugprintf("main/procother: mid1 rs=%d f_suf=%u f_c=%u f_p=%u\n",
	        rs,f_suf,f_process,f_process) ;
#endif

/* readable */

	if ((rs >= 0) && f_process) {
	    if (pip->fl.readable && (! f_islink)) {
	        if ((rs = uc_access(name,R_OK)), isNotAccess(rs)) {
	            f_process = false ;
	            rs = SR_OK ;
	        }
	    }
	} /* end if (readable) */

#if	CF_DEBUG
	if (DEBUGLEVEL(4))
	    debugprintf("main/procother: mid2 rs=%d f_process=%u\n",
	        rs,f_process) ;
#endif

/* check if it is a program (and disallowed) */

	if ((rs >= 0) && f_process) {
	    if (pip->fl.f_noprog && (! f_accept)) {
	        if (S_ISREG(sbp->st_mode)) {
	            rs = fileobject(name) ;
	            f_process = (rs == 0) ;
	        }
	    }
	} /* end if (no-program) */

#if	CF_DEBUG
	if (DEBUGLEVEL(4))
	    debugprintf("main/procother: mid3 rs=%d f_process=%u\n",
	        rs,f_process) ;
#endif

	if ((rs >= 0) && f_process) {
	    if (pip->fl.cores && (! f_accept)) {
	        f_process = (strwcmp("core",bnp,bnl) == 0) ;
	    }
	} /* end if (yes-core) */

/* check if it is a program (and required) */

#if	CF_DEBUG
	if (DEBUGLEVEL(4))
	    debugprintf("main/procother: mid3b rs=%d f_process=%u\n",
	        rs,f_process) ;
#endif

	if ((rs >= 0) && f_process) {
	    if (pip->fl.f_prog) {
	        if (S_ISREG(sbp->st_mode)) {
	            rs = fileobject(name) ;
	            f_process = (rs > 0) ;
	        } else {
		    f_process = false ;
		}
	    }
	} /* end if (program) */

/* process this file */

#if	CF_DEBUG
	if (DEBUGLEVEL(4))
	    debugprintf("main/procother: mid4 rs=%d f_process=%u\n",
	        rs,f_process) ;
#endif

	if ((rs >= 0) && f_process) {
	    pip->c_processed += 1 ;
	    switch (pip->progmode) {
	    case progmode_filesize:
	        rs = procsize(pip,name,sbp,ckp) ;
	        break ;
	    case progmode_filefind:
	        if (! pip->fl.quiet) {
		    bfile	*ofp = (bfile *) pip->ofp ;
	            rs = bprintln(ofp,name,-1) ;
	        }
	        break ;
	    case progmode_filelinker:
	    case progmode_filesyncer:
	        rs = procothers(pip,name,sbp,ckp) ;
	        break ;
	    case progmode_filerm:
	        rs = procrm(pip,name,sbp,ckp) ;
	        break ;
	    case progmode_filelines:
	        rs = proclines(pip,name,sbp,ckp) ;
	        break ;
	    default:
	        rs = SR_NOANODE ;
	        break ;
	    } /* end switch */
	} /* end if (processed) */

/* done */

	if ((pip->debuglevel > 0) || ((rs < 0) && (! pip->fl.quiet))) {
	    int	f = false ;
	    f = f || (pip->debuglevel > 1) ;
	    f = f || (! isNotAccess(rs))  ;
	    f = f || (! pip->fl.iacc) ;
	    if (f) {
	        bfile	*efp = (bfile *) pip->efp ;
	        cchar	*pn = pip->progname ;
	        cchar	*fmt = "%s: fname=%s (%d:%u)%s\n" ;
	        cchar	*e = (rs < 0) ? " failure" : "" ;
	        bprintf(efp,fmt,pn,name,rs,f_process,e) ;
	    }
	    if (isNotAccess(rs) && pip->fl.iacc) {
	        rs = SR_OK ;
	    }
	} /* end if (print-out) */

#if	CF_DEBUG
	if (DEBUGLEVEL(3))
	    debugprintf("main/procother: ret rs=%d f_process=%u\n",
	        rs,f_process) ;
#endif

	return (rs >= 0) ? f_process : rs ;
}
/* end subroutine (procother) */

local int procothers(PI *pip,cchar *name,ustat *sbp,FILEINFO *ckp) noex {
	TARDIR		*tdp ;
	vechand		*tlp = &pip->tardirs ;
	cint		rsn = SR_NOTFOUND ;
	int		rs = SR_OK ;
	int		rs1 ;
#if	CF_DEBUG
	if (DEBUGLEVEL(4)) {
	    cchar	*cp = strfiletype(sbp) ;
	    debugprintf("main/procothers: ent name=%s ft=%s\n",name,cp) ;
	}
#endif
	for (int i = 0 ; (rs1 = vechand_get(tlp,i,&tdp)) >= 0 ; i += 1) {
	    if (tdp) {
	        pip->tardname = tdp->dname ;
	        pip->tardev = tdp->dev ;
#if	CF_DEBUG
	        if (DEBUGLEVEL(4)) {
	            debugprintf("main/procothers: tar=%s\n",tdp->dname) ;
	            debugprintf("main/procothers: dev=%08x\n",tdp->dev) ;
	        }
#endif
	        switch (pip->progmode) {
	        case progmode_filelinker:
	            rs = proclink(pip,name,sbp,ckp) ;
	            break ;
	        case progmode_filesyncer:
	            rs = procsync(pip,name,sbp,ckp) ;
	            break ;
	        } /* end switch */
	    }
	    if (rs < 0) break ;
	} /* end for */
	if ((rs >= 0) && (rs1 != rsn)) rs = rs1 ;
	return rs ;
}
/* end subroutine (procothers) */

#if	CF_NEWPRUNE
local int procprune(PI *pip,cchar *name) noex {
	int		rs = SR_OK ;
	int		f_go = true ;

#if	CF_DEBUG
	if (DEBUGLEVEL(4))
	    debugprintf("main/procprune: ent name=%s\n",name) ;
#endif

	if (pip->prune != nullptr) {
	    cchar	*cp ;
	    if (int cl ; (cl = sfbasename(name,-1,&cp)) > 0) {
	        f_go = (matstr(pip->prune,cp,cl) < 0) ;
	    }
	} /* end if (basename) */

#if	CF_DEBUG
	if (DEBUGLEVEL(4))
	    debugprintf("main/procprune: ret rs=%d f_go=%u\n",rs,f_go) ;
#endif

	return (rs >= 0) ? f_go : rs ;
}
/* end subroutine (procprune) */
#else /* CF_NEWPRUNE */
local int procprune(PI *pip,cchar *name) noex {
	int		rs = SR_OK ;
	int		f_go = true ; /* return-value */
	cchar		*cp ;

#if	CF_DEBUG
	if (DEBUGLEVEL(4))
	    debugprintf("main/procprune: ent name=%s\n",name) ;
#endif

	if (int cl ; (cl = sfbasename(name,-1,&cp)) > 0) {
	    paramopt		*pop = &pip->aparams ;
	    paramopt_cur	cur ;
	    if ((rs = paramopt_curbegin(pop,&cur)) >= 0) {
	        int	vl ;
	        cchar	*po = po_prune ;
	        cchar	*vp ;
	        while (rs >= 0) {
	            vl = paramopt_curfetch(pop,po,&cur,&vp) ;
	            if (vl == SR_NOTFOUND) break ;
	            rs = vl ;
#if	CF_DEBUG
	            if (DEBUGLEVEL(4)) {
	                debugprintf("main/procprune: cl=%u vl=%u\n",cl,vl) ;
	                debugprintf("main/procprune: c=%r\n",cp,cl) ;
	                debugprintf("main/procprune: v=%r\n",vp,vl) ;
	            }
#endif
	            if ((rs >= 0) && (cl == vl) && (cp[0] == vp[0])) {
	                f_go = (strwcmp(cp,vp,vl) != 0) ;
	                if (! f_go) break ;
	            }
	        } /* end while */
	        paramopt_curend(pop,&cur) ;
	    } /* end if (paramopt-cur) */
	} /* end if (basename) */

#if	CF_DEBUG
	if (DEBUGLEVEL(4))
	    debugprintf("main/procprune: ret rs=%d f_go=%u\n",rs,f_go) ;
#endif

	return (rs >= 0) ? f_go : rs ;
}
/* end subroutine (procprune) */
#endif /* CF_NEWPRUNE */

local int procprintfts(PI *pip,cchar *po) noex {
	bfile		*efp = (bfile *) pip->efp ;
	int		rs = SR_OK ;
	int		rs1 ;
	int		wlen = 0 ;

	if (pip->debuglevel > 0) {
	    paramopt		*pop = &pip->aparams ;
	    paramopt_cur	cur ;
	    if ((rs = paramopt_curbegin(pop,&cur)) >= 0) {
	        int	vl ;
	        cchar	*pn = pip->progname ;
	        cchar	*vp ;

	        while ((vl = paramopt_curfetch(pop,po,&cur,&vp)) >= 0) {
	            rs1 = bprintf(efp,"%s: ft=%r\n",pn,vp,vl) ;
	            if (rs1 > 0) wlen += rs1 ;
	            if (rs1 < 0) break ;
	        } /* end while */

	        paramopt_curend(pop,&cur) ;
	    } /* end if (paramopt-cur) */
	} /* end if */

	return (rs >= 0) ? wlen : rs ;
}
/* end subroutine (procprintfts) */

local int procprintsufs(PI *pip,cchar *po) noex {
	paramopt	*pop = &pip->aparams ;
	paramopt_cur	cur ;
	bfile		*efp = (bfile *) pip->efp ;
	int		rs = SR_OK ;
	int		rs1 ;
	int		wlen = 0 ;

	if (pip->debuglevel > 0) {
	    if ((rs = paramopt_curbegin(pop,&cur)) >= 0) {
	        int	vl ;
	        cchar	*pn = pip->progname ;
	        cchar	*vp ;

	        while ((vl = paramopt_curfetch(pop,po,&cur,&vp)) >= 0) {
	            rs1 = bprintf(efp,"%s: suf=%r\n",pn,vp,vl) ;
	            if (rs1 > 0) wlen += rs1 ;
	            if (rs1 < 0) break ;
	        } /* end while */

	        paramopt_curend(pop,&cur) ;
	    } /* end if (cursor) */
	} /* end if */

	return (rs >= 0) ? wlen : rs ;
}
/* end subroutine (procprintsufs) */

local int procrm(PI *pip,cchar *name,ustat *sbp,FILEINFO *ckp) noex {
	int		rs = SR_OK ;

	if (pip == nullptr) return SR_FAULT ;

#if	CF_DEBUG
	if (DEBUGLEVEL(3))
	    debugprintf("main/procrm: name=%s\n",name) ;
#endif

	if (S_ISDIR(sbp->st_mode)) {
	    rs = procrm_add(pip,name,-1) ;
	} else {
	    rs = uc_unlink(name) ;
	}

	if ((rs >= 0) && (pip->verboselevel > 1)) {
	    bfile	*ofp = (bfile *) pip->ofp ;
	    rs = bprintf(ofp,"%s (%d)\n",name,rs) ;

	}

#if	CF_DEBUG
	if (DEBUGLEVEL(3))
	    debugprintf("main/procrm: ret rs=%d\n",rs) ;
#endif

	return rs ;
}
/* end subroutine (procrm) */

local int proclines(PI *pip,cchar *name,ustat *sbp,FILEINFO *ckp) noex {
	int		rs = SR_OK ;
	if (pip == nullptr) return SR_FAULT ;
#if	CF_DEBUG
	if (DEBUGLEVEL(4))
	    debugprintf("fileop/proclines: ent n=%s\n",name) ;
#endif
	if (S_ISREG(sbp->st_mode)) {
	    int		cch = 0 ;
	    if (pip->fl.ignasscomm) {
	        cch = ';' ;
	    } else if (pip->fl.igncomm) {
	        cch = pip->cch ;
	    }
#if	CF_DEBUG
	    if (DEBUGLEVEL(4))
	        debugprintf("fileop/proclines: f_ignasscomm=%u\n",
	            pip->fl.ignasscomm) ;
#endif
	    if ((rs = filelines(name,cch)) >= 0) {
	        pip->c_lines += rs ;
	        if (! pip->fl.summary) {
	    	    bfile	*ofp = (bfile *) pip->ofp ;
	            bprintf(ofp,"%10u %s\n",rs,name) ;
	        }
	    }
	}
#if	CF_DEBUG
	if (DEBUGLEVEL(4))
	    debugprintf("fileop/proclines: ret rs=%d\n",rs) ;
#endif
	return rs ;
}
/* end subroutine (proclines) */

local int proctars_begin(PI *pip) noex {
	int		rs = SR_OK ;
	int		c = 0 ;
	int		f = false ;
	f = f || (pip->progmode == progmode_filelinker) ;
	f = f || (pip->progmode == progmode_filesyncer) ;
	if (f) {
	    vechand	*tlp = &pip->tardirs ;
	    if ((rs = vechand_start(tlp,1,0)) >= 0) {
	        cchar	*cp ;
	        pip->open.tardirs = true ;
	        if ((cp = getenv(VARTARDNAME)) != nullptr) {
	            paramopt	*pop = &pip->aparams ;
	            cchar	*po = PO_TARDIRS ;
	            rs = paramopt_loads(pop,po,cp,-1) ;
	        }
	        if (rs >= 0) {
	            rs = proctars_check(pip) ;
	            c = rs ;
	        }
	        if (rs < 0) {
	            proctars_fins(pip) ;
	            vechand_finish(tlp) ;
	            pip->open.tardirs = false ;
	        }
	    } /* end if (vechand_start) */
	} /* end if (program-mode) */
#if	CF_DEBUG
	if (DEBUGLEVEL(3))
	    debugprintf("main/proctars_begin: ret rs=%d c=%u\n",rs,c) ;
#endif
	return (rs >= 0) ? c : rs ;
}
/* end subroutine (proctars_begin) */

local int proctars_end(PI *pip) noex {
	int		rs = SR_OK ;
	int		rs1 ;
#if	CF_DEBUG
	if (DEBUGLEVEL(3))
	    debugprintf("main/proctars_end: ent\n") ;
#endif
	if (pip->open.tardirs) {
	    vechand	*tlp = &pip->tardirs ;
	    {
	    rs1 = proctars_fins(pip) ;
	    if (rs >= 0) rs = rs1 ;
	    pip->open.tardirs = false ;
	    }
	    {
	    rs1 = vechand_finish(tlp) ;
	    if (rs >= 0) rs = rs1 ;
	    }
	}
#if	CF_DEBUG
	if (DEBUGLEVEL(3))
	    debugprintf("main/proctars_end: ret rs=%d\n",rs) ;
#endif
	return rs ;
}
/* end subroutine (proctars_end) */

local int proctars_check(PI *pip) noex {
	paramopt	*pop = &pip->aparams ;
	bfile		*efp = (bfile *) pip->efp ;
	int		rs ;
	int		rs1 ;
	int		c = 0 ;
	cchar		*pn = pip->progname ;
	cchar		*po = PO_TARDIRS ;
	cchar		*fmt = nullptr ;
#if	CF_DEBUG
	if (DEBUGLEVEL(4))
	    debugprintf("main/proctars_check: ent\n") ;
#endif
	if ((rs = paramopt_countvals(pop,po)) > 0) {
	    paramopt_cur	cur ;
	    if ((rs = paramopt_curbegin(pop,&cur)) >= 0) {
	    	ustat		sb ;
	        int		vl ;
	        cchar		*vp ;
	        char		td[MAXPATHLEN+1] ;
	        while ((rs1 = paramopt_curenumval(pop,po,&cur,&vp)) >= 0) {
	            vl = rs1 ;
	            if ((rs1 > 0) && (vp != nullptr)) {
	                if (pip->debuglevel > 0) {
	                    bprintf(efp,"%s: target=%r\n",pn,vp,vl) ;
	                }
	                if ((rs = mkpath1w(td,vp,vl)) >= 0) {
	                    if ((rs = uc_stat(td,&sb)) >= 0) {
	                        if (S_ISDIR(sb.st_mode)) {
	                            if ((rs = proctars_same(pip,&sb)) >= 0) {
	                                rs = proctars_load(pip,td,&sb) ;
	                                c += rs ;
	                            }
	                        } else {
#if	CF_DEBUG
	if (DEBUGLEVEL(4))
	    debugprintf("main/proctars_check: nto-dir\n") ;
#endif
	                            rs = SR_NOTDIR ;
	                        }
	                    }
			    if (rs < 0) {
				proctars_checkerr(pip,td,rs) ;
			    }
	                } /* end if (mkpath) */

	            } /* end if (positive) */
	            if (rs < 0) break ;
	        } /* end while */
	        if ((rs >= 0) && (rs1 != SR_NOTFOUND)) rs = rs1 ;
#if	CF_DEBUG
	if (DEBUGLEVEL(4))
	    debugprintf("main/proctars_check: while-out rs=%d c=%u\n",rs) ;
#endif
	        rs1 = paramopt_curend(pop,&cur) ;
	        if (rs >= 0) rs = rs1 ;
	    } /* end if (paramopt-cur) */
	} else if (rs == 0) {
	    fmt = "%s: no target directory specified\n" ;
	    rs = SR_INVALID ;
	    bprintf(efp,fmt,pip->progname) ;
	} /* end if */
#if	CF_DEBUG
	if (DEBUGLEVEL(4))
	    debugprintf("main/proctars_check: ret rs=%d c=%u\n",rs,c) ;
#endif
	return (rs >= 0) ? c : rs ;
}
/* end subroutine (proctars_check) */

local int proctars_checkerr(PI *pip,cchar *td,int rs) noex {
	bfile		*efp = (bfile *) pip->efp ;
	cchar		*pn = pip->progname ;
	cchar		*fmt = nullptr ;
#if	CF_DEBUG
	if (DEBUGLEVEL(4))
	    debugprintf("main/proctars_checker: gen-error rs=%d\n",rs) ;
#endif
	    switch (rs) {
	    case SR_NOENT:
	    case SR_ACCESS:
		fmt = "%s: inaccessible target (%d)\n" ;
		break ;
	    case SR_XDEV:
	        fmt = "%s: all targets must be on the same file-system (%d)\n" ;
	        break ;
	    case SR_NOTDIR:
	        fmt = "%s: all targets must be directories (%d)\n" ;
	        break ;
	    } /* end switch */
	    if (fmt != nullptr) {
	        bprintf(efp,fmt,pn,rs) ;
	        bprintf(efp,"%s: tar=%s\n",pn,td) ;
	    }
	return rs ;
}
/* end subroutine (proctars_checkerr) */

local int proctars_same(PI *pip,ustat *sbp) noex {
	int		rs = SR_OK ;
	if (pip->progmode == progmode_filelinker) {
	    if (pip->tardev == 0) {
	        pip->tardev = sbp->st_dev ;
	    } else {
	        if (sbp->st_dev != pip->tardev) {
	            rs = SR_XDEV ;
	        }
	    }
	}
	return rs ;
}
/* end subroutine (proctars_same) */

local int proctars_load(PI *pip,cchar *name,ustat *sbp) noex {
	TARDIR		*tdp ;
	cint		osize = szof(TARDIR) ;
	int		rs ;
	int		c = 0 ;
#ifdef	COMMENT
	if (pip->debuglevel > 0) {
	    bfile	*efp = (bfile *) pip->efp ;
	    cchar	*pn = pip->progname ;
	    cchar	*fmt = "%s: target=%s\n" ;
	    bprintf(efp,fmt,pn,name) ;
	}
#endif /* COMMENT */
	if ((rs = proctars_notalready(pip,sbp)) > 0) {
	    c += 1 ;
	    if ((rs = lm_mall(osize,&tdp)) >= 0) {
	        if ((rs = tardir_start(tdp,name,sbp)) >= 0) {
	            vechand	*tlp = &pip->tardirs ;
	            rs = vechand_add(tlp,tdp) ;
	            if (rs < 0) {
	                tardir_finish(tdp) ;
	            }
	        }
	        if (rs < 0) {
	            lm_free(tdp) ;
		}
	    } /* end if (m-a) */
	} /* end if (proctars_notalready) */
	return (rs >= 0) ? c : rs ;
}
/* end subroutine (proctars_load) */

local int proctars_notalready(PI *pip,ustat *sbp) noex {
	int		rs = SR_OK ;
	int		f = false ;
	if (pip->open.tardirs) {
	    TARDIR	*tdp ;
	    vechand	*tlp = &pip->tardirs ;
	    for (int i = 0 ; vechand_get(tlp,i,&tdp) >= 0 ; i += 1) {
	        if (tdp != nullptr) {
	            rs = tardir_match(tdp,sbp) ;
	            f = rs ;
	        }
	        if (f) break ;
	        if (rs < 0) break ;
	    } /* end for */
	}
	return (rs >= 0) ? (!f) : rs ;
}
/* end subroutine (proctars_notalready) */

local int proctars_fins(PI *pip) noex {
	int		rs = SR_OK ;
	int		rs1 ;
	if (pip->open.tardirs) {
	    TARDIR	*tdp ;
	    vechand	*tlp = &pip->tardirs ;
	    for (int i = 0 ; vechand_get(tlp,i,&tdp) >= 0 ; i += 1) {
	        if (tdp) {
		    {
	            rs1 = tardir_finish(tdp) ;
	            if (rs >= 0) rs = rs1 ;
		    }
		    {
	            rs1 = lm_free(tdp) ;
	            if (rs >= 0) rs = rs1 ;
		    }
	        }
	    } /* end for */
	}
#if	CF_DEBUG
	if (DEBUGLEVEL(3))
	    debugprintf("main/proctars_fins: ret rs=%d\n",rs) ;
#endif
	return rs ;
}
/* end subroutine (proctars_load) */

local int procsuf_have(PI *pip,cchar *sp,int sl) noex {
	paramopt	*pop = &pip->aparams ;
	paramopt_cur	cur ;
	int		rs ;
	int		rs1 ;
	int		f = false ;
	if (sl < 0) sl = lenstr(sp) ;
	if ((rs = paramopt_curbegin(pop,&cur)) >= 0) {
	    int		vl ;
	    int		m ;
	    cchar	*key = po_sufreq ;
	    cchar	*vp ;
	    while (rs >= 0) {
	        vl = paramopt_curfetch(pop,key,&cur,&vp) ;
	        if (vl == SR_NOTFOUND) break ;
	        rs = vl ;
	        if ((rs >= 0) && (sl == vl)) {
	            m = nleadstr(sp,vp,vl) ;
	            f = (m == vl) ;
	            if (f) break ;
	        }
	    } /* end while */
	    rs1 = paramopt_curend(pop,&cur) ;
	    if (rs >= 0) rs = rs1 ;
	} /* end if (paramopt-cur) */
	return (rs >= 0) ? f : rs ;
}
/* end subroutine (procsuf_have) */

local int procsuf_begin(PI *pip) noex {
	paramopt	*pop = &pip->aparams ;
	vecpstr		*vlp ;
	int		rs = SR_OK ;
	int		c = 0 ;
	cchar		*po = nullptr ;
	for (int si = 0 ; si < suf_overlast ; si += 1) {
	    int	f = false ;
#if	CF_DEBUG
	    if (DEBUGLEVEL(3))
	        debugprintf("main/procsuf_begin: si=%u c=%u\n",si,c) ;
#endif
	    switch (si) {
	    case suf_req:
	        f = pip->have.sufreq ;
	        if (f) {
	            po = sufs[si] ;
	            vlp = (pip->sufs + si) ;
	        }
	        break ;
	    case suf_acc:
	        f = pip->have.sufacc ;
	        if (f) {
	            po = sufs[si] ;
	            vlp = (pip->sufs + si) ;
	        }
	        break ;
	    case suf_rej:
	        f = pip->have.sufrej ;
	        if (f) {
	            po = sufs[si] ;
	            vlp = (pip->sufs + si) ;
	        }
	        break ;
	    } /* end switch */
#if	CF_DEBUG
	    if (DEBUGLEVEL(3))
	        debugprintf("main/procsuf_begin: si=%u f=%u\n",si,f) ;
#endif
	    if (f) {
	        if ((rs = paramopt_countvals(pop,po)) > 0) {
		    cint n = rs ;
	            paramopt_cur	cur ;
	            if ((rs = vecpstr_start(vlp,n,0,0)) >= 0) {
	                switch (si) {
	                case suf_req:
	                    pip->open.sufreq = true ;
	                    break ;
	                case suf_acc:
	                    pip->open.sufacc = true ;
	                    break ;
	                case suf_rej:
	                    pip->open.sufrej = true ;
	                    break ;
	                } /* end switch */
	                if ((rs = paramopt_curbegin(pop,&cur)) >= 0) {
	                    int		vl ;
	                    cchar	*vp ;
	                    while (rs >= 0) {
	                        vl = paramopt_curfetch(pop,po,&cur,&vp) ;
	                        if (vl == SR_NOTFOUND) break ;
	                        rs = vl ;
	                        if ((rs >= 0) && (vl > 0)) {
	                            rs = vecpstr_adduniq(vlp,vp,vl) ;
	                            if (rs < INT_MAX) c += 1 ;
#if	CF_DEBUG
	                            if (DEBUGLEVEL(3) && (rs < INT_MAX))
	                                debugprintf("main/procsuf_begin: "
	                                    "suf=%r\n",
	                                    vp,vl) ;
#endif
	                        }
	                    } /* end while */
	                    paramopt_curend(pop,&cur) ;
	                } /* end if */
	                if (c > 0) {
	                    switch (si) {
	                    case suf_req:
	                        pip->fl.sufreq = true ;
	                        break ;
	                    case suf_acc:
	                        pip->fl.sufacc = true ;
	                        break ;
	                    case suf_rej:
	                        pip->fl.sufrej = true ;
	                        break ;
	                    } /* end switch */
	                } /* end if */
	            } /* end if (vecpstr_start) */
	        } /* end if (n) */
	    } /* end if */
	} /* end for */
#if	CF_DEBUG
	if (DEBUGLEVEL(3))
	    debugprintf("main/procsuf_begin: ret rs=%d c=%u\n",rs,c) ;
#endif
	return (rs >= 0) ? c : rs ;
}
/* end subroutine (procsuf_begin) */

local int procsuf_end(PI *pip) noex {
	vecpstr		*slp ;
	int		rs = SR_OK ;
	int		rs1 ;
	for (int i = 0 ; i < suf_overlast ; i += 1) {
	    slp = (pip->sufs + i) ;
	    bool f = false ;
	    switch (i) {
	    case suf_req:
	        f = pip->open.sufreq ;
	        pip->open.sufreq = false ;
	        break ;
	    case suf_acc:
	        f = pip->open.sufacc ;
	        pip->open.sufacc = false ;
	        break ;
	    case suf_rej:
	        f = pip->open.sufrej ;
	        pip->open.sufrej = false ;
	        break ;
	    } /* end switch */
	    if (f) {
	        rs1 = vecpstr_finish(slp) ;
	        if (rs >= 0) rs = rs1 ;
	    }
	} /* end for */
	return rs ;
}
/* end subroutine (procsuf_end) */

local int procsuf_load(PI *pip,int si,cchar *ap,int al) noex {
	paramopt	*pop = &pip->aparams ;
	int		rs = SR_OK ;
	int		c = 0 ;
	cchar		*po ;
	cchar		*var ;
	if (ap != nullptr) {
	    switch (si) {
	    case suf_req:
	        po = po_sufreq ;
	        var = VARSUFREQ ;
	        break ;
	    case suf_acc:
	        po = po_sufacc ;
	        var = VARSA ;
	        break ;
	    case suf_rej:
	        po = po_sufrej ;
	        var = VARSR ;
	        break ;
	    default:
	        rs = SR_NOANODE ;
	        break ;
	    } /* end switch */
	    if (rs >= 0) {
	        if (strwcmp("-",ap,al) != 0) {
	            if (strwcmp("+",ap,al) == 0) {
	                ap = getenv(var) ;
	                al = -1 ;
	            }
	            if (ap != nullptr) {
	                if ((rs = paramopt_loads(pop,po,ap,al)) > 0) {
	                    c = rs ;
	                    switch (si) {
	                    case suf_req:
	                        pip->have.sufreq = true ;
	                        break ;
	                    case suf_acc:
	                        pip->have.sufacc = true ;
	                        break ;
	                    case suf_rej:
	                        pip->have.sufrej = true ;
	                        break ;
	                    } /* end switch */
	                } /* end if (paramopt_loads) */
	            }
	        } /* end if */
	    } /* end if (ok) */
	} /* end if (non-null) */
	return (rs >= 0) ? c : rs ;
}
/* end subroutine (procsuf_load) */

local int procrm_begin(PI *pip) noex {
	int		rs ;
	if ((rs = vecpstr_start(&pip->rmdirs,0,0,0)) >= 0) {
	    pip->fl.rmdirs = true ;
	}
	return rs ;
}
/* end subroutine (procrm_begin) */

local int procrm_add(PI *pip,cchar *dp,int dl) noex {
	int		rs ;
	if (dl < 0) dl = lenstr(dp) ;
	if ((dl > 0) && (dp[dl-1] == '/')) {
	    dl -=1 ;
	}
	rs = vecpstr_add(&pip->rmdirs,dp,dl) ;
	return rs ;
}
/* end subroutine (procrm_add) */

local int procrm_end(PI *pip) noex {
	vecpstr_vcmpq	vcf = vecpstr_vcmp(vstrcmpr) ;
	int		rs = SR_OK ;
	int		rs1 ;
	if (pip->fl.rmdirs) {
	    vecpstr	*rlp = &pip->rmdirs ;
	    pip->fl.rmdirs = false ;
	    if ((rs = vecpstr_sort(rlp,cvf)) >= 0) {
	        cchar	*np{} ;
	        for (int i = 0 ; vecpstr_get(rlp,i,&np) >= 0 ; i += 1) {
	            rs1 = u_rmdir(np) ;
	            if (rs >= 0) rs = rs1 ;
	        } /* end for */
	    } /* end if (vecpstr_sort) */
	    rs1 = vecpstr_finish(&pip->rmdirs) ;
	    if (rs >= 0) rs = rs1 ;
	} /* end if (rmdirs) */
	return rs ;
}
/* end subroutine (procrm_end) */

local int proclines_begin(PI *pip) noex {
	int		rs = SR_FAULT ;
	if (pip) {
	    rs = SR_OK ;
	}
	return rs ;
}
/* end subroutine (proclines_begin) */

local int proclines_end(PI *pip) noex {
	int		rs = SR_OK ;
	if (pip == nullptr) return SR_FAULT ;
	if (pip->fl.summary) {
	    bfile	*ofp = (bfile *) pip->ofp ;
	    rs = bprintf(ofp,"%10u\n",pip->c_lines) ;
	}
	return rs ;
}
/* end subroutine (proclines_end) */

local int procdir_begin(PI *pip) noex {
	hdb		*dbp = &pip->dirs ;
	hdbhash_f	hf = (hdbhash_f) diridhash ;
	hdbcmp_f	cf = (hdbcmp_f) diridcmp ;
	cint		n = 50 ;
	cint		at = 1 ;	/* use |lookaside(3dam)| */
	int		rs ;
	if ((rs = hdb_start(dbp,n,at,hf,cf)) >= 0) {
	    hdbstr	*ndp = &pip->dirnames ;
	    if ((rs = hdbstr_start(ndp,0)) >= 0) {
	        pip->open.dirs = true ;
	    }
	    if (rs < 0)
	        hdb_finish(dbp) ;
	} /* end if (hdb_start) */
	return rs ;
}
/* end subroutine (procdir_begin) */

local int procdir_end(PI *pip) noex {
	hdb		*dbp = &pip->dirs ;
	hdb_cur		cur ;
	hdb_dat		key ;
	hdb_dat		val ;
	int		rs = SR_OK ;
	int		rs1 ;
	if (pip->open.dirs) {
	    pip->open.dirs = false ;
	    if ((rs1 = hdb_curbegin(dbp,&cur)) >= 0) {
	        DIRID	*dip ;
	        while (hdb_curenum(dbp,&cur,&key,&val) >= 0) {
	            dip = (DIRID *) val.buf ;
	            if (dip != nullptr) {
	                dirid_finish(dip) ;
	                rs1 = lm_free(dip) ;
	                if (rs >= 0) rs = rs1 ;
	            }
	        } /* end while (enum) */
	        rs1 = hdb_curend(dbp,&cur) ;
		if (rs >= 0) rs = rs1 ;
	    } /* end if (cursor) */
	    if (rs >= 0) rs = rs1 ;
	    {
	    rs1 = hdb_finish(&pip->dirs) ;
	    if (rs >= 0) rs = rs1 ;
	    }
	    {
	    rs1 = hdbstr_finish(&pip->dirnames) ;
	    if (rs >= 0) rs = rs1 ;
	    }
	} /* end if (was activated) */
	return rs ;
}
/* end subroutine (procdir_end) */

local int procdir_have(PI *pip,dev_t dev,ino_t ino,cchar *sp,int nl) noex {
	hdb		*dbp = &pip->dirs ;
	hdb_dat		key ;
	hdb_dat		val ;
	DIRID		did ;
	int		rs ;
	did.ino = ino ;
	did.dev = dev ;
	key.buf = &did ;
	key.len = szof(ino_t) + szof(dev_t) ;
	if ((rs = hdb_fetch(dbp,key,nullptr,&val)) >= 0) {
	    if ((rs = hdbstr_add(&pip->dirnames,sp,nl,nullptr,0)) >= 0) {
	        rs = 1 ;
	    }
	} else if (rs == SR_NOTFOUND) {
	    if ((rs = procdir_addid(pip,dev,ino)) >= 0) {
	        rs = 0 ;
	    }
	}
	return rs ;
}
/* end subroutine (procdir_have) */

local int procdir_addid(PI *pip,dev_t dev,ino_t ino) noex {
	DIRID		*dip ;
	cint		sz = szof(DIRID) ;
	int		rs ;
	if ((rs = lm_mall(sz,&dip)) >= 0) {
	    if ((rs = dirid_start(dip,dev,ino)) >= 0) {
	        hdb		*dbp = &pip->dirs ;
	        hdb_dat		key ;
	        hdb_dat		val ;
	        key.buf = dip ;
	        key.len = szof(ino_t) + szof(dev_t) ;
	        val.buf = dip ;
	        val.len = sz ;
	        rs = hdb_store(dbp,key,val) ;
	        if (rs < 0) {
	            dirid_finish(dip) ;
		}
	    } /* end if (dir-id) */
	    if (rs < 0) {
	        lm_free(dip) ;
	    }
	} /* end if (memory-allocation) */
	return rs ;
}
/* end subroutine (procdir_addid) */

local int procdir_haveprefix(PI *pip,cchar *fp,int fl) noex {
	hdbstr		*plp = &pip->dirnames ;
	int		rs = SR_OK ;
	int		pl ;
	cchar		*pp ;
	if ((pl = sfdirname(fp,fl,&pp)) > 0) {
	    if ((rs = hdbstr_fetch(plp,pp,pl,nullptr,nullptr)) >= 0) {
	        rs = 1 ;
	    } else if (rs == SR_NOTFOUND) {
	        rs = 0 ;
	    }
	}
	return rs ;
}
/* end subroutine (procdir_haveprefix) */

local int procdir_addprefix(PI *pip,cchar *sp,int nl) noex {
	hdbstr		*plp = &pip->dirnames ;
	int		rs ;
	if ((rs = hdbstr_fetch(plp,sp,nl,nullptr,nullptr)) >= 0) {
	    rs = 1 ;
	} else if (rs == SR_NOTFOUND) {
	    if ((rs = hdbstr_add(plp,sp,nl,nullptr,0)) >= 0) {
	        rs = 0 ;
	    }
	}
	return rs ;
}
/* end subroutine (procdir_addprefix) */

local int procuniq_begin(PI *pip) noex {
	hdb		*dbp = &pip->files ;
	cint		n = 50 ;
	cint		at = 1 ;	/* use 'lookaside(3dam)' */
	int		rs = SR_OK ;
	if (pip->fl.f_uniq) {
	    hdbhash_f	hf = (hdbhash_f) fileidhash ;
	    hdbcmp_f	cf = (hdbcmp_f) fileidcmp ;
	    if ((rs = hdb_start(dbp,n,at,hf,cf)) >= 0) {
	        pip->open.files = true ;
	    }
	}
	return rs ;
}
/* end subroutine (procuniq_begin) */

local int procuniq_end(PI *pip) noex {
	int		rs = SR_OK ;
	int		rs1 ;

	if (pip->fl.f_uniq && pip->open.files) {
	    hdb		*dbp = &pip->files ;
	    hdb_cur	cur ;
	    hdb_dat	key ;
	    hdb_dat	val ;
	    FILEID	*dip ;
	    pip->open.files = false ;

	    if ((rs1 = hdb_curbegin(dbp,&cur)) >= 0) {

	        while (hdb_curenum(dbp,&cur,&key,&val) >= 0) {
	            dip = (FILEID *) val.buf ;

	            if (dip != nullptr) {
	                fileid_finish(dip) ;
	                rs1 = lm_free(dip) ;
	                if (rs >= 0) rs = rs1 ;
	            }

	        } /* end while (enum) */

	        hdb_curend(dbp,&cur) ;
	    } /* end if (cursor) */
	    if (rs >= 0) rs = rs1 ;

	    rs1 = hdb_finish(&pip->files) ;
	    if (rs >= 0) rs = rs1 ;

	} /* end if (unique and was activated) */

	return rs ;
}
/* end subroutine (procuniq_end) */

local int procuniq_have(PI *pip,dev_t dev,ino_t ino) noex {
	hdb		*dbp = &pip->files ;
	hdb_dat		key ;
	hdb_dat		val ;
	FILEID		fid ;
	int		rs ;

	fid.ino = ino ;
	fid.dev = dev ;

	key.buf = &fid ;
	key.len = szof(ino_t) + szof(dev_t) ;
	if ((rs = hdb_fetch(dbp,key,nullptr,&val)) >= 0) {
	    rs = 1 ;
	} else if (rs == SR_NOTFOUND) {
	    if ((rs = procuniq_addid(pip,dev,ino)) >= 0) {
	        rs = 0 ;
	    }
	}

	return rs ;
}
/* end subroutine (procuniq_have) */

local int procuniq_addid(PI *pip,dev_t dev,ino_t ino) noex {
	cint		sz = szof(FILEID) ;
	int		rs ;
	if (FILEID *dip ; (rs = lm_mall(sz,&dip)) >= 0) {
	    if ((rs = fileid_start(dip,dev,ino)) >= 0) {
	        hdb	*dbp = &pip->files ;
	        hdb_dat	key, val ;
	        key.buf = dip ;
	        key.len = szof(ino_t) + szof(dev_t) ;
	        val.buf = dip ;
	        val.len = sz ;
	        rs = hdb_store(dbp,key,val) ;
	        if (rs < 0)
	            fileid_finish(dip) ;
	    } /* end if (dir-id) */
	    if (rs < 0) {
	        lm_free(dip) ;
	    }
	} /* end if (memory-allocation) */
	return rs ;
}
/* end subroutine (procuniq_addid) */

local int procprune_begin(PI *pip,cchar *pfname) noex {
	int		rs ;
	int		rs1 ;
	int		c = 0 ;
	if ((rs = procprune_loadfile(pip,pfname)) >= 0) {
	    if (pip->fl.prune) {
	        int	sz ;
	        if ((rs = procprune_size(pip,&sz)) > 0) {
	            cint	n = rs ;
	            if (char *bp ; (rs = lm_mall(sz,&bp)) >= 0) {
	                paramopt	*pop = &pip->aparams ;
	                paramopt_cur	cur ;
	                pip->prune = (cchar **) bp ;
	                if ((rs = paramopt_curbegin(pop,&cur)) >= 0) {
	                    int		vl ;
	                    cchar	*po = po_prune ;
	                    cchar	*vp ;
	                    char	**pa = (char **) bp ;
	                    bp += ((n+1)*szof(void *)) ;
	                    while (rs >= 0) {
	                        vl = paramopt_curfetch(pop,po,&cur,&vp) ;
	                        if (vl == SR_NOTFOUND) break ;
	                        rs = vl ;
	                        if ((rs >= 0) && (vl > 0)) {
	                            pa[c++] = bp ;
	                            bp = (strwcpy(bp,vp,vl)+1) ;
	                        }
	                    } /* end while */
	                    pa[c] = nullptr ;
	                    rs1 = paramopt_curend(pop,&cur) ;
	                    if (rs >= 0) rs = rs1 ;
	                } /* end if (paramopt-cur) */
	                if (rs < 0) {
	                    lm_free(pip->prune) ;
	                    pip->prune = nullptr ;
	                }
	            } /* end if (m-a) */
	        } /* end if (procprune_size) */
	    } /* end if (prune) */
	} /* end if (prune-file) */
	return (rs >= 0) ? c : rs ;
}
/* end subroutine (procprune_begin) */

local int procprune_end(PI *pip) noex {
	int		rs = SR_OK ;
	int		rs1 ;
	if (pip->prune != nullptr) {
	    rs1 = lm_free(pip->prune) ;
	    if (rs >= 0) rs = rs1 ;
	    pip->prune = nullptr ;
	}
	return rs ;
}
/* end subroutine (procuniq_end) */

local int procprune_loadfile(PI *pip,cchar *pfname) noex {
	int		rs = SR_OK ;
	int		rs1 ;
	int		c = 0 ;
	if ((pfname != nullptr) && (pfname[0] != '\0')) {
	    bfile	pfile, *pfp = &pfile ;
	    if ((rs = bopen(pfp,pfname,"r",0666)) >= 0) {
	        paramopt	*pop = &pip->aparams ;
	        cint	llen = LINEBUFLEN ;
	        int		len ;
	        int		cl ;
	        cchar		*po = po_prune ;
	        cchar		*cp ;
	        char		lbuf[LINEBUFLEN+1] ;
	        while ((rs = breadln(pfp,lbuf,llen)) > 0) {
	            len = rs ;
	            if (lbuf[len - 1] == '\n') len -= 1 ;
	            lbuf[len] = '\0' ;
	            if ((cp = strnchr(lbuf,len,'#')) != nullptr) {
	                len = (cp-lbuf) ;
	            }
	            if ((cl = sfskipwhite(lbuf,len,&cp)) > 0) {
	                if (cp[0] != '#') {
	                    lbuf[((cp-lbuf)+cl)] = '\0' ;
	                    rs = paramopt_loads(pop,po,cp,cl) ;
	                    c += rs ;
	                }
	            }
	            if (rs < 0) break ;
	        } /* end while */
	        rs1 = bclose(pfp) ;
	        if (rs >= 0) rs = rs1 ;
	    } /* end if (bfile) */
	    if (c > 0) pip->fl.prune = true ;
	} /* end if (have file) */
	return (rs >= 0) ? c : rs ;
}
/* end subroutine (procprune_loadfile) */

local int procprune_size(PI *pip,int *sizep) noex {
	paramopt	*pop = &pip->aparams ;
	paramopt_cur	cur ;
	int		rs ;
	int		rs1 ;
	int		c = 0 ;
	int		sz = 0 ;
	if ((rs = paramopt_curbegin(pop,&cur)) >= 0) {
	    int	vl ;
	    cchar	*po = po_prune ;
	    cchar	*vp ;
	    while (rs >= 0) {
	        vl = paramopt_curfetch(pop,po,&cur,&vp) ;
	        if (vl == SR_NOTFOUND) break ;
	        rs = vl ;
	        if ((rs >= 0) && (vl > 0)) {
	            c += 1 ;
	            sz += szof(void *) ;
	            sz += (strnlen(vp,vl)+1) ;
	        }
	    } /* end while */
	    sz += szof(void *) ;
	    rs1 = paramopt_curend(pop,&cur) ;
	    if (rs >= 0) rs = rs1 ;
	} /* end if (paramopt-cur) */
	if (sizep != nullptr) *sizep = sz ;
	return (rs >= 0) ? c : rs ;
}
/* end subroutine (procuniq_size) */

local int proclink_begin(PI *pip) noex {
	hdb		*dbp = &pip->links ;
	hdbhash_f	hf = (hdbhash_f) linkhash ;
	hdbcmp_f	cf = (hdbcmp_f) linkcmp ;
	cint	n = 50 ;
	cint	at = 1 ;	/* use |lookaside(3dam)| */
	int		rs ;
	{
	    rs = hdb_start(dbp,n,at,hf,cf) ;
	    pip->open.links = (rs >= 0) ;
	}
	return rs ;
}
/* end subroutine (proclink_begin) */

local int proclink_end(PI *pip) noex {
	int		rs = SR_OK ;
	int		rs1 ;
	if (pip->open.links) {
	    pip->open.links = false ;
	    {
	        rs1 = proclink_fins(pip) ;
	        if (rs >= 0) rs = rs1 ;
	    }
	    {
	        rs1 = hdb_finish(&pip->links) ;
	        if (rs >= 0) rs = rs1 ;
	    }
	} /* end if (was activated) */
	return rs ;
}
/* end subroutine (proclink_end) */

local int proclink_fins(PI *pip) noex {
	hdb		*dbp = &pip->links ;
	hdb_cur		cur ;
	hdb_dat		key ;
	hdb_dat		val ;
	int		rs = SR_OK ;
	int		rs1 ;
	if (pip == nullptr) return SR_FAULT ;
	if ((rs1 = hdb_curbegin(dbp,&cur)) >= 0) {
	    LINKINFO	*lip ;
	    while ((rs1 = hdb_curenum(dbp,&cur,&key,&val)) >= 0) {
	        lip = (LI *) val.buf ;
	        if (lip) {
		    {
	                rs1 = linkinfo_finish(lip) ;
	                if (rs >= 0) rs = rs1 ;
		    }
		    {
	                rs1 = lm_free(lip) ;
	                if (rs >= 0) rs = rs1 ;
		    }
	        }
	    } /* end while (enum) */
	    if ((rs >= 0) && (rs1 != SR_NOTFOUND)) {
		rs = rs1 ;
	    }
	    rs1 = hdb_curend(dbp,&cur) ;
	    if (rs >= 0) rs = rs1 ;
	} /* end if (hdb-cur) */
	if (rs >= 0) rs = rs1 ;
	return rs ;
}
/* end subroutine (proclink_fins) */

local int proclink_add(PI *pip,dev_t dev,ino_t ino,mode_t m,cchar *fp) noex {
	int		rs ;
	int		f = false ;
	if ((rs = proclink_have(pip,dev,ino,nullptr)) == 0) {
	    cint	lsize = szof(LINKINFO) ;
	    if (LINKINFO *lip ; (rs = lm_mall(lsize,&lip)) >= 0) {
	        if ((rs = linkinfo_start(lip,dev,ino,m,fp)) >= 0) {
	            hdb		*dbp = &pip->links ;
	            hdb_dat	key, val ;
	            f = true ;
	            key.buf = lip ;
	            key.len = lsize ;
	            val.buf = lip ;
	            val.len = lsize ;
	            rs = hdb_store(dbp,key,val) ;
	            if (rs < 0) {
	                linkinfo_finish(lip) ;
		    }
	        } /* end if (linkinfo) */
	        if (rs < 0) {
	            lm_free(lip) ;
		}
	    } /* end if (memory-allocation) */
	} /* end if (needed to add) */
	return (rs >= 0) ? f : rs ;
}
/* end subroutine (proclink_add) */

local int proclink_have(PI *pip,dev_t dev,ino_t ino,LI **rpp) noex {
	LINKINFO	li ;
	hdb		*dbp = &pip->links ;
	hdb_dat		key ;
	hdb_dat		val ;
	cint		lsize = szof(LINKINFO) ;
	int		rs ;
	{
	    li.dev = dev ;
	    li.ino = ino ;
	}
	key.buf = &li ;
	key.len = lsize ;
	if ((rs = hdb_fetch(dbp,key,nullptr,&val)) >= 0) {
	    rs = 1 ;
	    if (rpp != nullptr) *rpp = (LI *) val.buf ;
	} else if (rs == SR_NOTFOUND) {
	    rs = 0 ;
	}
	return rs ;
}
/* end subroutine (proclink_have) */

local int procsize(PI *pip,cchar *name,ustat *sbp,FILEINFO *ckp) noex {
	size_t		bytes ;
	size_t		sz = sbp->st_size ;
	int		rs ;
	if (name == nullptr) return SR_FAULT ;
	{
	    bytes = pip->bytes ;
	    bytes += (sz % MEGABYTE) ;
	}
	{
	    pip->bytes = (bytes % MEGABYTE) ;
	    pip->megabytes += (bytes / MEGABYTE) ;
	    pip->megabytes += (sz / MEGABYTE) ;
	}
	rs = int(sz & INT_MAX) ;
	return rs ;
}
/* end subroutine (procsize) */

local int proclink(PI *pip,cchar *name,ustat *sbp,FILEINFO *ckp) noex {
	int		rs = SR_OK ;
	int		f_linked = false ; /* return-value */
	if (sbp->st_dev == pip->tardev) {
	    int		w = 0 ;
	    cmode	dm = 0775 ;
	    char	tarfname[MAXPATHLEN + 1] ;
	    if ((rs = mkpath2(tarfname,pip->tardname,name)) >= 0) {
	        bool	f_dolink = true ;
	        if (ustat tsb ; (rs = uc_lstat(tarfname,&tsb)) >= 0) {
	            if (S_ISDIR(sbp->st_mode)) {
	                if (S_ISDIR(tsb.st_mode)) {
	                    f_dolink = false ;
	                } else {
	                    w = 3 ;
	                    rs = uc_unlink(tarfname) ;
	                }
		    } else {
	                bool	f = true ;
	                f = f && (tsb.st_dev == sbp->st_dev) ;
	                f = f && (tsb.st_ino == sbp->st_ino) ;
	                if (f) {
	                    f_linked = true ;
	                    f_dolink = false ;
	                } else {
	                    if (S_ISDIR(tsb.st_mode)) {
	                        w = 1 ;
	                        rs = removes(tarfname) ;
	                    } else {
	                        w = 2 ;
	                        rs = uc_unlink(tarfname) ;
	                    }
	                }
	            } /* end if */
	        } else if (isNotStat(rs)) {
	            rs = SR_OK ;
	        }

	        if ((rs >= 0) && f_dolink) {
	            if (S_ISDIR(sbp->st_mode)) {
	                w = 7 ;
	                rs = uc_mkdir(tarfname,dm) ;
	                if ((rs == SR_NOTDIR) || (rs == SR_NOENT)) {
	                    w = 8 ;
	                    if ((rs = mkpdirs(tarfname,dm)) >= 0) {
	                        w = 9 ;
	                        rs = uc_mkdir(tarfname,dm) ;
	                    }
	                }
	            } else {
	                f_linked = true ;
	                w = 4 ;
	                rs = uc_link(name,tarfname) ;
	                if ((rs == SR_NOTDIR) || (rs == SR_NOENT)) {
	                    w = 5 ;
	                    if ((rs = mkpdirs(tarfname,dm)) >= 0) {
	                        w = 6 ;
	                        rs = uc_link(name,tarfname) ;
	                    }
	                }
		    } /* end if */
	        } /* end if (dolink) */
	        if ((rs == SR_EXIST) && (! pip->fl.quiet)) {
	    	    bfile	*efp = (bfile *) pip->efp ;
	            cchar	*pn = pip->progname ;
	            bprintf(efp,"%s: exists w=%u\n",pn,w) ;
	        }
	    } /* end if (mkpath) */
	} else {
	    pip->c_linkerr += 1 ;
	    rs = SR_XDEV ;
	}
	return (rs >= 0) ? f_linked : rs ;
}
/* end subroutine (proclink) */

local int procsync(PI *pip,cchar *name,ustat *sbp,FILEINFO *ckp) noex {
	LINKINFO	*lip ;
	dev_t		dev = sbp->st_dev ;
	ino_t		ino = sbp->st_ino ;
	int		rs ;
	/* do we have a link to this file already? */
	if ((rs = proclink_have(pip,dev,ino,&lip)) > 0) {
	    cint	f_samepath = (strcmp(lip->fname,name) == 0) ;
#if	CF_DEBUG
	    if (DEBUGLEVEL(3))
	        debugprintf("main/procsync: mid1 rs=%d f_samepath=%u\n",
	            rs,f_samepath) ;
#endif
	    if (f_samepath || ((rs = procsynclink(pip,name,sbp,lip)) == 0)) {
	        rs = procsyncer(pip,name,sbp) ;
	    }
	} else if (rs == 0) {
#if	CF_DEBUG
	    if (DEBUGLEVEL(3))
	        debugprintf("main/procsync: mid2 rs=%d\n",rs) ;
#endif
	    rs = procsyncer(pip,name,sbp) ;

	} /* end if (proclink_have) */

#if	CF_DEBUG
	if (DEBUGLEVEL(3))
	    debugprintf("main/procsync: ret rs=%d\n",rs) ;
#endif

	return rs ;
}
/* end subroutine (procsync) */

local int procsynclink(PI *pip,cchar *name,ustat *sbp,LI *lip) noex {
	bfile		*efp = (bfile *) pip->efp ;
	int		rs ;
	int		f_linked = false ;
	char		pbuf[MAXPATHLEN+1] ;

#if	CF_DEBUG
	if (DEBUGLEVEL(3)) {
	    debugprintf("main/procsynclink: name=%s\n",name) ;
	    debugprintf("main/procsynclink: tar=%s\n",pip->tardname) ;
	}
#endif

	if ((rs = mkpath2(pbuf,pip->tardname,lip->fname)) >= 0) {
	    if (ustat psb ; (rs = uc_lstat(pbuf,&psb)) >= 0) {
	        const dev_t	dev = psb.st_dev ;
	        const ino_t	ino = psb.st_ino ;
	        if (dev == pip->tardev) {
	            char	dstfname[MAXPATHLEN + 1] ;
	            if ((rs = mkpath2(dstfname,pip->tardname,name)) >= 0) {
	                cmode	dm = DMODE ;
	                cmode	m = lip->mode ;
	                bool	f_dolink = true ;
	                if (ustat dsb ; (rs = uc_lstat(dstfname,&dsb)) >= 0) {
	                    if (! S_ISDIR(m)) {
	                        int	f = true ;
	                        f = f && (dev == dsb.st_dev) ;
	                        f = f && (ino == dsb.st_ino) ;
	                        if (f) {
	                            f_dolink = false ;
	                            f_linked = true ;
	                        } else {
	                            if (S_ISDIR(dsb.st_mode)) {
	                                rs = removes(dstfname) ;
	                            } else {
	                                rs = uc_unlink(dstfname) ;
	                            }
	                        }
	                    } else {
	                        if (! S_ISDIR(dsb.st_mode)) {
	                            rs = uc_unlink(dstfname) ;
	                        } else {
	                            f_dolink = false ;
	                        }
	                    }
	                } else if (isNotStat(rs)) {
	                    rs = SR_OK ;
	                } /* end if (stat) */
	                if ((rs >= 0) && f_dolink) {
	                    f_linked = true ;
	                    if (! S_ISDIR(m)) {
	                        if (pip->debuglevel > 0) {
	                            cchar	*pn = pip->progname ;
	                            cchar	*fmt ;
	                            cchar	*td = pip->tardname ;
	                            fmt = "%s: link tar=%s fn=%s\n" ;
	                            bprintf(efp,fmt,pn,td,name) ;
	                        }
	                        rs = uc_link(pbuf,dstfname) ;
	                        if ((rs == SR_NOTDIR) || (rs == SR_NOENT)) {
	                            if ((rs = mkpdirs(dstfname,dm)) >= 0) {
	                                rs = uc_link(pbuf,dstfname) ;
	                            }
	                        }
	                    } else {
	                        rs = uc_mkdir(dstfname,dm) ;
	                        if ((rs == SR_NOTDIR) || (rs == SR_NOENT)) {
	                            if ((rs = mkpdirs(dstfname,dm)) >= 0) {
	                                rs = uc_mkdir(dstfname,dm) ;
	                            }
	                        }
	                    }
	                } /* end if (dolink) */
	            } /* end if (mkpath) */
	        } /* end if (same file-system) */
	    } else if (isNotStat(rs)) {
	        rs = SR_OK ;
	    } /* end if (stat) */
	} /* end if (mkpath) */

#if	CF_DEBUG
	if (DEBUGLEVEL(3))
	    debugprintf("main/procsynclink: ret rs=%d f_linked=%u\n",
	        rs,f_linked) ;
#endif

	return (rs >= 0) ? f_linked : rs ;
}
/* end subroutine (procsynclink) */

local int procsyncer(PI *pip,cchar *name,ustat *sbp) noex {
	int		rs = SR_OK ;
	int		c = 0 ;

#if	CF_DEBUG
	if (DEBUGLEVEL(3))
	    debugprintf("main/procsyncer: name=%s\n",name) ;
#endif

	if ((! S_ISDIR(sbp->st_mode)) && (sbp->st_nlink > 1)) {
	    const dev_t		dev = sbp->st_dev ;
	    const ino_t		ino = sbp->st_ino ;
	    const mode_t	m = sbp->st_mode ;

#if	CF_DEBUG
	    if (DEBUGLEVEL(3))
	        debugprintf("main/procsyncer: proclink_add() "
	            "dev=%08x ino=%llu\n",dev,ino) ;
#endif

	    rs = proclink_add(pip,dev,ino,m,name) ;
	}
	if (rs >= 0) {
	    if (sigblocker blk ; (rs = blk.start(nullptr)) >= 0) {
	        if (S_ISREG(sbp->st_mode)) {
	            rs = procsyncer_reg(pip,name,sbp) ;
	        } else if (S_ISDIR(sbp->st_mode)) {
	            rs = procsyncer_dir(pip,name,sbp) ;
	        } else if (S_ISLNK(sbp->st_mode)) {
	            rs = procsyncer_lnk(pip,name,sbp) ;
	        } else if (S_ISFIFO(sbp->st_mode)) {
	            rs = procsyncer_fifo(pip,name,sbp) ;
	        } else if (S_ISSOCK(sbp->st_mode)) {
	            rs = procsyncer_sock(pip,name,sbp) ;
	        } /* end if */
		c = rs ;
	        rs1 = blk.finish ;
		if (rs >= 0) rs = rs1 ;
	    } /* end if (blocking signals) */
	} /* end if (ok) */

#if	CF_DEBUG
	if (DEBUGLEVEL(3))
	    debugprintf("main/procsyncer: ret rs=%d\n",rs) ;
#endif

	return (rs >= 0) ? c : rs ;
}
/* end subroutine (procsyncer) */

local int procsyncer_reg(PI *pip,cchar *name,ustat *sbp) noex {
	off_t		dfsize = 0 ;
	cmode		dm = DMODE ;
	cmode		nm = (sbp->st_mode & (~ S_IFMT)) | 0600 ;
	uid_t		duid = -1 ;
	int		rs = SR_OK ;
	int		of = 0 ;
	int		f_create = false ;
	int		f_update = false ;
	int		f_updated = false ;
	char		dstfname[MAXPATHLEN + 1] ;
	char		tmpfname[MAXPATHLEN + 1] ;

#if	CF_DEBUG
	if (DEBUGLEVEL(3)) {
	    debugprintf("main/procsyncer_reg: tardname=%s\n",pip->tardname) ;
	    debugprintf("main/procsyncer_reg: name=%s\n",name) ;
	}
#endif

/* exit on large files! */

	if (sbp->st_size > INT_MAX) goto ret0 ;

	rs = mkpath2(dstfname,pip->tardname,name) ;
	if (rs < 0) goto ret0 ;

/* continue */

#if	CF_DEBUG
	if (DEBUGLEVEL(3))
	    debugprintf("main/procsyncer_reg: dstfname=%s\n",dstfname) ;
#endif

	if (ustat dsb ; (rs = u_lstat(dstfname,&dsb)) >= 0) {
	    if (S_ISREG(dsb.st_mode)) {
	        bool	f = false ;
	        duid = dsb.st_uid ;
	        dfsize = (off_t) dsb.st_size ;
#if	CF_DEBUG
	if (DEBUGLEVEL(3))
	    debugprintf("main/procsyncer_reg: dfsize=%ull\n",dfsize) ;
#endif
	        f = f || (sbp->st_size != dsb.st_size) ;
	        f = f || (sbp->st_mtime > dsb.st_mtime) ;
	        if (f) {
	            f_update = true ;
	            if (pip->fl.rmfile) {
	                f_create = true ;
	                uc_unlink(dstfname) ;
	            }
	        }
	    } else {
	        f_create = true ;
	        if (S_ISDIR(dsb.st_mode)) {
	            rs = removes(dstfname) ;
	        } else {
	            rs = uc_unlink(dstfname) ;
	        }
	    }

	} else if (isNotStat(rs)) {
	    f_create = true ;

#if	CF_DEBUG
	    if (DEBUGLEVEL(3))
	        debugprintf("main/procsyncer_reg: dst u_lstat() rs=%d\n",rs) ;
#endif

	    if (rs == SR_NOTDIR) {
	        int	dnl ;
	        cchar	*dnp ;
	        rs = SR_OK ;
	        if ((dnl = sfdirname(dstfname,-1,&dnp)) > 0) {
#if	CF_DEBUG
	            if (DEBUGLEVEL(3))
	                debugprintf("main/procsyncer_reg: dst dname=%r\n",
	                    dnp,dnl) ;
#endif
	            if ((rs = mkpath1w(tmpfname,dnp,dnl)) >= 0) {
	                rs = uc_unlink(tmpfname) ;
	            }
	        }
	    } else {
	        rs = SR_OK ;
	    }

#if	CF_DEBUG
	    if (DEBUGLEVEL(3))
	        debugprintf("main/procsyncer_reg: no entry source rs=%d\n",
	            rs) ;
#endif

	} /* end if */

#if	CF_DEBUG
	if (DEBUGLEVEL(3))
	    debugprintf("main/procsyncer_reg: source rs=%d "
	        "f_update=%u f_create=%u\n",
	        rs,f_update,f_create) ;
#endif

	if (rs < 0) goto ret0 ;
	if (! (f_update || f_create)) goto ret0 ;

/* create any needed directories (as necessary) */

	if (f_create) {
	    int		dnl ;
	    cchar	*dnp ;
	    if ((dnl = sfdirname(dstfname,-1,&dnp)) > 0) {
	        if ((rs = mkpath1w(tmpfname,dnp,dnl)) >= 0) {
	            int		f = false ;
	            if (ustat sb ; (rs = u_lstat(tmpfname,&sb)) >= 0) {
	                if (S_ISLNK(sb.st_mode)) {
	                    if ((rs = u_stat(tmpfname,&sb)) >= 0) {
	                        f = (! S_ISDIR(sb.st_mode)) ;
	                    } else if (isNotPresent(rs)) {
	                        f = true ;
	                        rs = SR_OK ;
	                    }
	                } else if (! S_ISDIR(sb.st_mode)) {
	                    f = true ;
	                }
	                if ((rs >= 0) && f) {
	                    rs = uc_unlink(tmpfname) ;
#if	CF_DEBUG
	                    if (DEBUGLEVEL(3))
	                        debugprintf("main/procsyncer_reg: "
	                            "uc_unlink() rs=%d\n", rs) ;
#endif
	                }
	            } else if (isNotStat(rs)) {
	                f = true ;
	                rs = SR_OK ;
	            }
	            if ((rs >= 0) && f) {
	                rs = mkdirs(tmpfname,dm) ;
	            }
	        } /* end if (mkpath) */
	    } /* end if (dir-name) */
	} /* end if (need creation) */
	if (rs < 0) goto ret0 ;

/* update (or create) the target file */

#if	CF_DEBUG
	if (DEBUGLEVEL(3))
	        debugprintf("main/procsyncer_reg: rs=%d dfn=%s\n",rs,dstfname) ;
#endif

	of = O_WRONLY ;
	if (f_create) of |= O_CREAT ;

	rs = u_open(dstfname,of,nm) ;
	if ((rs == SR_ACCESS) && (! f_create)) {
	    int		dnl ;
	    cchar	*dnp ;
#if	CF_DEBUG
	    if (DEBUGLEVEL(3))
	        debugprintf("main/procsyncer_reg: SR_ACCESS\n") ;
#endif
	    if ((dnl = sfdirname(dstfname,-1,&dnp)) > 0) {
	        if ((rs = mkpath1w(tmpfname,dnp,dnl)) >= 0) {
	            if ((rs = uc_access(tmpfname,W_OK)) >= 0) {
	                f_create = true ;
	                if ((rs = uc_unlink(dstfname)) >= 0) {
	                    of |= O_CREAT ;
	                    rs = u_open(dstfname,of,nm) ;
	                }
	            }
	        }
	    }
	}
	if (rs >= 0) { /* opened */
	    int		dfd = rs ;

#if	CF_DEBUG
	    if (DEBUGLEVEL(3))
	        debugprintf("main/procsyncer_reg: u_open() rs=%d\n",rs) ;
#endif

	    if (f_create) {
	        rs = uc_fminmod(dfd,nm) ;
	    } /* end if (needed to be created) */

/* do we need to UPDATE the destination? */

#if	CF_DEBUG
	    if (DEBUGLEVEL(3))
	        debugprintf("main/procsyncer_reg: "
	            "need update? rs=%d f_update=%d\n",
	            rs,f_update) ;
#endif

	    if (rs >= 0) {
	        f_updated = true ;
	        if ((rs = u_open(name,O_RDONLY,0666)) >= 0) {
	            cint	sfd = rs ;
	            int		len ;

#if	CF_DEBUG
	    if (DEBUGLEVEL(4)) {
		ustat	sb ;
	        debugprintf("main/procsyncer_reg: sfn=%s\n",name) ;
	        debugprintf("main/procsyncer_reg: u_open() rs=%d\n",rs) ;
	        debugprintf("main/procsyncer_reg: u_open() rs=%d\n",rs) ;
		u_fstat(sfd,&sb) ;
	        debugprintf("main/procsyncer_reg: sfsize=%llu\n",sb.st_size) ;
	    }
#endif

	            if ((rs = uc_copy(sfd,dfd,-1)) >= 0) {
	                len = rs ;

#if	CF_DEBUG
	    	        if (DEBUGLEVEL(4)) {
			    ustat	sb ;
			    size_t	sfsize ;
	        	    debugprintf("main/procsyncer_reg: "
				"uc_copy() rs=%d\n",rs) ;
			    u_fstat(sfd,&sb) ;
			    sfsize = (size_t) sb.st_size ;
	        	    debugprintf("main/procsyncer_reg: "
				"sfsize=%lu\n",sfsize) ;
	        	    debugprintf("main/procsyncer_reg: "
				"dfsize=%llu\n",dfsize) ;
		        }
#endif
	                if (len < dfsize) {
	                    off_t	uoff = len ;
	                    rs = uc_ftruncate(dfd,uoff) ;
	                }
	            } /* end if (uc_copy) */

	            u_close(sfd) ;
	            if (rs >= 0) {
	                int	f_utime = false ;
	                f_utime = f_utime || (duid < 0) ;
	                f_utime = f_utime || (duid == pip->euid) ;
	                f_utime = f_utime || (pip->euid == 0) ;
	                if (f_utime) {
	                    UTIMBUF	ut ;
			    int		rs1 ;
	                    ut.actime = sbp->st_atime ;
	                    ut.modtime = sbp->st_mtime ;
			    u_close(dfd) ;
			    dfd = -1 ;
#if	CF_DEBUG
	                    if (DEBUGLEVEL(4)) {
				char	tbuf[TIMEBUFLEN+1] ;
	                        debugprintf("main/procsyncer_reg: utime()\n") ;
	                        debugprintf("main/procsyncer_reg: "
				    "dfn=%s\n",dstfname) ;
				timestr_log(ut.modtime,tbuf) ;
	                        debugprintf("main/procsyncer_reg: "
				    "mt=%s\n",tbuf) ;
			    }
#endif
	                    rs1 = uc_filetime(dstfname,&ut) ;
#if	CF_DEBUG
	                    if (DEBUGLEVEL(4))
	                        debugprintf("main/procsyncer_reg: "
				"uc_filetime() rs=%d\n",rs1) ;
#endif
	                }
	            }
	        } else if (rs == SR_NOENT) {
	            rs = SR_OK ;
	            uc_unlink(dstfname) ;
	        } /* end if (open source) */
	    } /* end if (update was needed) */

	    if (dfd >= 0) u_close(dfd) ;
	} /* end if (destination file opened) */

ret0:

#if	CF_DEBUG
	if (DEBUGLEVEL(4))
	    debugprintf("main/procsyncer_reg: ret rs=%d f_updated=%u\n",
	        rs,f_updated) ;
#endif

	return (rs >= 0) ? f_updated : rs ;
}
/* end subroutine (procsyncer_reg) */

local int procsyncer_dir(PI *pip,cchar *name,ustat *sbp) noex {
	cmode		nm = (sbp->st_mode & (~ S_IFMT)) | DMODE ;
	uid_t		duid = -1 ;
	int		rs = SR_OK ;
	int		f_create = false ;
	int		f_update = false ;
	int		f_mode = false ;
	int		f_mtime = false ;
	int		f_updated = false ;
	char		dstfname[MAXPATHLEN + 1] ;
	char		tmpfname[MAXPATHLEN + 1] ;

	rs = mkpath2(dstfname,pip->tardname,name) ;
	if (rs < 0) goto ret0 ;

/* continue */

	if (ustat dsb ; (rs = u_lstat(dstfname,&dsb)) >= 0) {
	    duid = dsb.st_uid ;
	    if (S_ISDIR(dsb.st_mode)) {
	        f_mtime = (dsb.st_mtime != sbp->st_mtime) ;
	        f_mode = (dsb.st_mode != nm) ;
	        if (f_mode || f_mtime) {
	            f_update = true ;
	        }
	    } else {
	        f_create = true ;
	        rs = uc_unlink(dstfname) ;
	    }
	} else if (isNotStat(rs)) {
	    f_create = true ;

#if	CF_DEBUG
	    if (DEBUGLEVEL(3))
	        debugprintf("main/procsyncer_dir: dst u_lstat() rs=%d\n",rs) ;
#endif

	    if (rs == SR_NOTDIR) {
	        int	dnl ;
	        cchar	*dnp ;
	        rs = SR_OK ;
	        if ((dnl = sfdirname(dstfname,-1,&dnp)) > 0) {
	            if ((rs = mkpath1w(tmpfname,dnp,dnl)) >= 0) {
	                rs = uc_unlink(tmpfname) ;
	            }
	        }
	    } else {
	        rs = SR_OK ;
	    }

#if	CF_DEBUG
	    if (DEBUGLEVEL(3))
	        debugprintf("main/procsyncer_dir: no entry for source \n") ;
#endif

	} /* end if */

#if	CF_DEBUG
	if (DEBUGLEVEL(3))
	    debugprintf("main/procsyncer_dir: source rs=%d "
	        "f_update=%u f_create=%u\n",
	        rs,f_update,f_create) ;
#endif

	if (rs < 0) goto ret0 ;
	if (! (f_update || f_create)) goto ret0 ;

/* create any needed directories (as necessary) */

	if (f_create) {
	    int		dnl ;
	    cchar	*dnp ;
	    if ((dnl = sfdirname(dstfname,-1,&dnp)) > 0) {
	        if ((rs = mkpath1w(tmpfname,dnp,dnl)) >= 0) {
	            boolt		f = false ;
	            if (ustat sb ; (rs = u_lstat(tmpfname,&sb)) >= 0) {
	                if (S_ISLNK(sb.st_mode)) {
	                    if ((rs = u_stat(tmpfname,&sb)) >= 0) {
	                        f = (! S_ISDIR(sb.st_mode)) ;
	                    } else if (isNotPresent(rs)) {
	                        f = true ;
	                        rs = SR_OK ;
	                    }
	                } else if (! S_ISDIR(sb.st_mode)) {
	                    f = true ;
	                }
	                if ((rs >= 0) && f) {
	                    rs = uc_unlink(tmpfname) ;
	                }
	            } else if (isNotStat(rs)) {
	                f = true ;
	                rs = SR_OK ;
	            }
	            if ((rs >= 0) && f) {
	                rs = mkdirs(tmpfname,nm) ;
	            }
	        }
	    }
	} /* end if (create) */

/* update (or create) the target file */

	if (rs >= 0) {
	    if (f_create) {
	        rs = mkdir(dstfname,nm) ;
	    }
	    if ((rs >= 0) && ((duid < 0) || (duid == pip->euid))) {
	        f_updated = true ;
	        if (f_mode) rs = u_chmod(dstfname,nm) ;
	        if ((rs >= 0) && f_mtime) {
	            UTIMBUF	ut ;
	            ut.actime = sbp->st_atime ;
	            ut.modtime = sbp->st_mtime ;
	            uc_filetime(dstfname,&ut) ;
	        } /* end if */
	    } /* end if */
	} /* end if (ok) */

ret0:

#if	CF_DEBUG
	if (DEBUGLEVEL(3))
	    debugprintf("main/procsyncer_dir: ret rs=%d f_updated=%u\n",
	        rs,f_updated) ;
#endif

	return (rs >= 0) ? f_updated : rs ;
}
/* end subroutine (procsyncer_dir) */

local int procsyncer_lnk(PI *pip,cchar *name,ustat *sbp) noex {
	int		rs = SR_OK ;
	int		f_create = false ;
	int		f_update = false ;
	int		f_updated = false ;
	cmode		dm = DMODE ;
	char		dstfname[MAXPATHLEN + 1] ;
	char		tmpfname[MAXPATHLEN + 1] ;
	char		dstlink[MAXPATHLEN + 1] ;

	rs = u_readlink(name,dstlink,MAXPATHLEN) ;
	if (rs < 0) goto ret0 ;

	rs = mkpath2(dstfname,pip->tardname,name) ;
	if (rs < 0) goto ret0 ;

/* continue */

	if (ustat dsb ; (rs = u_lstat(dstfname,&dsb)) >= 0) {
	    if (S_ISLNK(dsb.st_mode)) {
	        bool	f = true ;
	        f = f && (dsb.st_size == sbp->st_size) ;
	        if (f) {
	            rs = u_readlink(dstfname,tmpfname,MAXPATHLEN) ;
	            if (rs >= 0) f = (strcmp(dstlink,tmpfname) == 0) ;
	        }
	        if (! f) {
	            f_update = true ;
	        } /* end if */
	    } else {
	        f_create = true ;
	        if (S_ISDIR(dsb.st_mode)) {
	            rs = removes(dstfname) ;
	        } else {
	            rs = uc_unlink(dstfname) ;
	        }
	    }
	} else if (isNotStat(rs)) {
	    f_create = true ;
	    if (rs == SR_NOTDIR) {
	        int	dnl ;
	        cchar	*dnp ;
	        rs = SR_OK ;
	        if ((dnl = sfdirname(dstfname,-1,&dnp)) > 0) {
	            if ((rs = mkpath1w(tmpfname,dnp,dnl)) >= 0) {
	                rs = uc_unlink(tmpfname) ;
	            }
	        }
	    } else if (isNotPresent(rs)) {
	        rs = SR_OK ;
	    }

	} /* end if */

	if (rs < 0) goto ret0 ;
	if (! (f_update || f_create)) goto ret0 ;

/* create any needed directories (as necessary) */

	if (f_create) {
	    int		dnl ;
	    cchar	*dnp ;
	    if ((dnl = sfdirname(dstfname,-1,&dnp)) > 0) {
	        if ((rs = mkpath1w(tmpfname,dnp,dnl)) >= 0) {
	            int		f = false ;
	            if (ustat sb ; (rs = u_lstat(tmpfname,&sb)) >= 0) {
	                if (S_ISLNK(sb.st_mode)) {
	                    if ((rs = u_stat(tmpfname,&sb)) >= 0) {
	                        f = (! S_ISDIR(sb.st_mode)) ;
	                    } else if (isNotPresent(rs)) {
	                        f = true ;
	                        rs = SR_OK ;
	                    }
	                } else if (! S_ISDIR(sb.st_mode)) {
	                    f = true ;
	                }
	                if ((rs >= 0) && f) {
	                    rs = uc_unlink(tmpfname) ;
	                }
	            } else if (isNotStat(rs)) {
	                f = true ;
	                rs = SR_OK ;
	            }
	            if ((rs >= 0) && f) {
	                rs = mkdirs(tmpfname,dm) ;
	            }
	        }
	    }
	} /* end if (create) */

/* update (or create) the target file */

	if (rs >= 0) {
	    f_updated = true ;
	    if ((! f_create) && f_update) {
	        rs = uc_unlink(dstfname) ;
	    }
	    if (rs >= 0) {
	        rs = u_symlink(dstlink,dstfname) ;
	    }
	} /* end if (update was needed) */

ret0:

	return (rs >= 0) ? f_updated : rs ;
}
/* end subroutine (procsyncer_lnk) */

local int procsyncer_fifo(PI *pip,cchar *name,ustat *sbp) noex {
	int		rs = SR_FAULT ;
	if (pip && name && sbp) {
	    rs = SR_OK ;
	}
	return rs ;
}
/* end subroutine (procsyncer_fifo) */

local int procsyncer_sock(PI *pip,cchar *name,ustat *sbp) noex {
	int		rs = SR_FAULT ;
	if (pip && name && sbp) {
	    rs = SR_OK ;
	}
	return rs ;
}
/* end subroutine (procsyncer_sock) */

local int tardir_start(TARDIR *tdp,cchar *n,ustat *sbp) noex {
	int		rs ;
	if (cchar *cp ; (rs = lm_strw(n,-1,&cp)) >= 0) {
	    tdp->dname = cp ;
	    tdp->dev = sbp->st_dev ;
	    tdp->ino = sbp->st_ino ;
	} /* end if (memory-allocation) */
	return rs ;
}
/* end subroutine (tardir_start) */

local int tardir_finish(TARDIR *tdp) noex {
	int		rs = SR_OK ;
	int		rs1 ;
	if (tdp->dname != nullptr) {
	    rs1 = lm_free(tdp->dname) ;
	    if (rs >= 0) rs = rs1 ;
	    tdp->dname = nullptr ;
	}
	return rs ;
}
/* end subroutine (tardir_finish) */

local int tardir_match(TARDIR *tdp,ustat *sbp) noex {
	int		f = true ;
	f = f && (tdp->dev == sbp->st_dev) ;
	f = f && (tdp->ino == sbp->st_ino) ;
	return f ;
}
/* end subroutine (tardir_match) */

local int dirid_start(DIRID *dip,dev_t dev,ino_t ino) noex {
	int		rs = SR_FAULT ;
	if (dip) {
	    rs = SR_OK ;
	    dip->dev = dev ;
	    dip->ino = ino ;
	}
	return rs ;
}
/* end subroutine (dirid_start) */

local int dirid_finish(DIRID *dip) noex {
	int		rs = SR_FAULT ;
	if (dip) {
	    rs = SR_OK ;
	}
	return rs ;
}
/* end subroutine (dirid_finish) */

local int fileid_start(FILEID *dip,dev_t dev,ino_t ino) noex {
	int		rs = SR_FAULT ;
	if (dip) {
	    rs = SR_OK ;
	    dip->dev = dev ;
	    dip->ino = ino ;
	}
	return rs ;
}
/* end subroutine (fileid_start) */

local int fileid_finish(FILEID *dip) noex {
	int		rs = SR_FAULT ;
	if (dip) {
	    rs = SR_OK ;
	}
	return rs ;
}
/* end subroutine (fileid_finish) */

local int fileinfo_loadfts(FILEINFO *ckp,ustat *sbp) noex {
	cint		ftype = (sbp->st_mode & S_IFMT) ;
	int		rs = SR_OK ;
	int		fts = 0 ;
	switch (ftype) {
	case S_IFIFO:
	    fts |= (1 << ft_p) ;
	    break ;
	case S_IFCHR:
	    fts |= (1 << ft_c) ;
	    break ;
	case S_IFDIR:
	    fts |= (1 << ft_d) ;
	    break ;
	case S_IFNAM:
	    fts |= (1 << ft_n) ;
	    break ;
	case S_IFBLK:
	    fts |= (1 << ft_b) ;
	    break ;
	case S_IFREG:
	    fts |= (1 << ft_r) ;
	    break ;
	case S_IFLNK:
	    fts |= (1 << ft_l) ;
	    break ;
	case S_IFSOCK:
	    fts |= (1 << ft_s) ;
	    break ;
	case S_IFDOOR:
	    fts |= (1 << ft_D) ;
	    break ;
	} /* end switch */
	ckp->fts = fts ;
	return (rs >= 0) ? fts : rs ;
}
/* end subroutine (fileinfo_loadfts) */

local int linkinfo_start(LI *lip,dev_t dev,ino_t ino,mode_t m,cc *fp) noex {
	int		rs ;
	lip->fname = nullptr ;
	if (cchar *cp ; (rs = lm_strw(fp,-1,&cp)) >= 0) {
	    lip->dev = dev ;
	    lip->ino = ino ;
	    lip->mode = m ;
	    lip->fname = cp ;
	} /* end if (memory-allocation) */
	return rs ;
}
/* end subroutine (linkinfo_start) */

local int linkinfo_finish(LI *lip) noex {
	int		rs = SR_OK ;
	int		rs1 ;
	if (lip->fname != nullptr) {
	    rs1 = lm_free(lip->fname) ;
	    if (rs >= 0) rs = rs1 ;
	    lip->fname = nullptr ;
	}
	return rs ;
}
/* end subroutine (linkinfo_finish) */

/* make *parent* directories as needed */
local int mkpdirs(cchar *tarfname,mode_t dm) noex {
	int		rs = SR_OK ;
	cchar		*dp ;
	if (int dl ; (dl = sfdirname(tarfname,-1,&dp)) > 0) {
	    char	dname[MAXPATHLEN + 1] ;
	    if ((rs = mkpath1w(dname,dp,dl)) >= 0) {
	        if ((rs = uc_unlink(dname)) >= 0) {
		    rs = SR_OK ;
		} else if (isNotPresent(rs)) {
		    rs = SR_OK ;
		}
	        rs = mkdirs(dname,dm) ;
	    }
	} else {
	    rs = SR_NOENT ;
	}
	return rs ;
}
/* end subroutine (mkpdirs) */

local uint linkhash(cvoid *vp,int vl) noex {
	LINKINFO	*lip = (LI *) vp ;
	uint		h = 0 ;
	ushort		*sa ;
	(void) vl ;
	{
	    sa = (ushort *) &lip->dev ;
	    h = h ^ ((sa[1] << 16) | sa[0]) ;
	    h = h ^ ((sa[0] << 16) | sa[1]) ;
	}
	{
	    cint	isize = szof(ino_t) ;
	    sa = (ushort *) &lip->ino ;
	    h = h ^ ((sa[1] << 16) | sa[0]) ;
	    h = h ^ ((sa[0] << 16) | sa[1]) ;
	    if (isize > 4) {
	        h = h ^ ((sa[3] << 16) | sa[2]) ;
	        h = h ^ ((sa[2] << 16) | sa[3]) ;
	    }
	}
	return h ;
}
/* end subroutine (linkhash) */

local uint diridhash(cvoid *vp,int vl) noex {
	const uint	uvl = (uint) vl ;
	uint		h = 0 ;
	ushort		*sa = (ushort *) vp ;
	h = h ^ ((sa[1] << 16) | sa[0]) ;
	h = h ^ ((sa[0] << 16) | sa[1]) ;
	if (uvl > szof(uint)) {
	    h = h ^ ((sa[3] << 16) | sa[2]) ;
	    h = h ^ ((sa[2] << 16) | sa[3]) ;
	    if (uvl > szof(ulong)) {
	        h = h ^ ((sa[5] << 16) | sa[4]) ;
	        h = h ^ ((sa[4] << 16) | sa[5]) ;
	        if (uvl > (4*3)) {
	            h = h ^ ((sa[7] << 16) | sa[6]) ;
	            h = h ^ ((sa[6] << 16) | sa[7]) ;
	        }
	    }
	}
	return h ;
}
/* end subroutine (diridhash) */

local uint fileidhash(cvoid *vp,int vl) noex {
	const uint	uvl = (uint) vl ;
	uint		h = 0 ;
	ushort		*sa = (ushort *) vp ;
	h = h ^ ((sa[1] << 16) | sa[0]) ;
	h = h ^ ((sa[0] << 16) | sa[1]) ;
	if (uvl > szof(uint)) {
	    h = h ^ ((sa[3] << 16) | sa[2]) ;
	    h = h ^ ((sa[2] << 16) | sa[3]) ;
	    if (uvl > szof(ulong)) {
	        h = h ^ ((sa[5] << 16) | sa[4]) ;
	        h = h ^ ((sa[4] << 16) | sa[5]) ;
	        if (uvl > (4*3)) {
	            h = h ^ ((sa[7] << 16) | sa[6]) ;
	            h = h ^ ((sa[6] << 16) | sa[7]) ;
	        }
	    }
	}
	return h ;
}
/* end subroutine (fileidhash) */

local int linkcmp(LI *e1p,LINKINFO *e2p,int len) noex {
	int		rc ; /* return-value */
	(void) len ;
	if ((rc = (e1p->dev - e2p->dev)) == 0) { /* reverse sort! */
	    if (int64_t d ; (d = (e1p->ino - e2p->ino)) > 0) {
	        rc = 1 ;
	    } else if (d < 0) {
	        rc = -1 ;
	    }
	}
	return rc ;
}
/* end subroutine (linkcmp) */

local int diridcmp(struct dirid *e1p,struct dirid *e2p,int len) noex {
	int		rc ;
	(void) len ;
	if ((rc = (e1p->dev - e2p->dev)) == 0) { /* reverse sort! */
	    int64_t	d ;
	    if ((d = (e1p->ino - e2p->ino)) > 0) {
	        rc = 1 ;
	    } else if (d < 0) {
	        rc = -1 ;
	    }
	}
	return rc ;
}
/* end subroutine (diridcmp) */

local int fileidcmp(FILEID *e1p,FILEID *e2p,int len) noex {
	int		rc ;
	(void) len ;
	if ((rc = (e1p->dev - e2p->dev)) == 0) { /* reverse sort! */
	    if (int64_t d ; (d = (e1p->ino - e2p->ino)) > 0) {
	        rc = 1 ;
	    } else if (d < 0) {
	        rc = -1 ;
	    }
	}
	return rc ;
}
/* end subroutine (fileidcmp) */

local book isNotStat(int rs) noex {
	return isOneOf(rsnostat,rs) ;
}
/* end subroutine (isNotStat) */


#if	CF_DEBUG
#ifdef	COMMENT
enum ftypes {
	ftype_reg,
	ftype_dir,
	ftype_chr,
	ftype_blk,
	ftype_fifo,
	ftype_sock,
	ftype_lnk,
	ftype_other,
	ftype_overlast
} ;
constexpr cpcchar	ftypes[] = {
	"REG",	
	"DIR",
	"CHR",
	"BLK",
	"FIFO",
	"SOCK",
	"LNK",
	"OTHER",
	nullptr
} ;
#endif /* COMMENT */
static cchar *strfiletype(ustat *sbp) {
	cchar	*cp ;
	if (S_ISLNK(sbp->st_mode)) cp = "LINK" ;
	else if (S_ISDIR(sbp->st_mode)) cp = "DIR" ;
	else if (S_ISREG(sbp->st_mode)) cp = "REG" ;
	else if (S_ISFIFO(sbp->st_mode)) cp = "FIFO" ;
	else if (S_ISSOCK(sbp->st_mode)) cp = "SOCK" ;
	else if (S_ISCHR(sbp->st_mode)) cp = "CHR" ;
	else cp = "OTHER" ;
	return cp ;
} /* end if (strfiletype) */
#endif /* CF_DEBUG */


