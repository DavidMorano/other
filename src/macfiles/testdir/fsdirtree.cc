/* fsdirtree SUPPORT */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* file-system directory tree (traversing) */
/* version %I% last-modified %G% */


/* revision history:

	= 2000-04-27, David A­D­ Morano
	I wanted an interative enumeration.

*/

/* Copyright © 2000 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Obsolete:
	fsdirtree

	Description:
	This subroutine is not similar to the 'ftw' subroutine.
	That subroutine "walks" the directory tree and calls a
	user-supplied function at each entry.  This module is quite
	different from 'ftw' and 'wdt' in that it allows the caller
	to intermix directory functions with other caller activities.
	It allows this by "reading" and entry in a similar way as
	FSDIR allows.

	The caller "opens" an object and then performs operations
	on it like reading a directory entry.  After the caller is
	finished with all directory functions on this object, the
	object should be "closed".

	Synopsis:
	int fsdirtree_open(fsdirtree *op,cchar *dname,int opts) noex

	Arguments:
	op		object pointer
	dname		directory name to process
	opts		options for processing

	Returns:
	>=0		good
	<0		bad in some way! (system-return)

*******************************************************************************/

#include	<envstandards.h>	/* MUST be first to configure */
#include	<sys/types.h>
#include	<sys/param.h>
#include	<sys/stat.h>
#include	<fcntl.h>
#include	<unistd.h>
#include	<climits>
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<cstdint>
#include	<cstring>
#include	<new>			/* |nothrow(3c++)| */
#include	<bitset>
#include	<clanguage.h>
#include	<usysbase.h>
#include	<usyscalls.h>
#include	<uclibmem.h>
#include	<getbufsize.h>
#include	<fsdir.h>
#include	<fifostr.h>
#include	<strwcpy.h>
#include	<matxstr.h>
#include	<filetype.h>
#include	<hasx.h>
#include	<ischarx.h>
#include	<ismisc.h>
#include	<isnot.h>
#include	<localmisc.h>

#include	"fsdirtree.h"

#pragma		GCC dependency		"mod/libutil.ccm"

import libutil ;			/* |memclear(3u)| */
import umisc ;				/* |mknpath1(3u)| */

/* local defines */

#define	FSDIRTREE_NENTS		50

#define	FSDIRTREE_MTYPE1	(FSDIRTREE_MREG | FSDIRTREE_MBLOCK)
#define	FSDIRTREE_MTYPE2	(FSDIRTREE_MPIPE | FSDIRTREE_MSOCK)
#define	FSDIRTREE_MTYPE3	(FSDIRTREE_MDIR | FSDIRTREE_MCHAR)
#define	FSDIRTREE_MTYPE		\
	    (FSDIRTREE_MTYPE1 | FSDIRTREE_MTYPE2 | FSDIRTREE_MTYPE3)

#define	DI		dirid

#define	MAXLINKLEN_MULT	4


/* imported namespaces */

using std::bitset ;			/* type */
using libu::sncpy ;			/* subroutine */
using std::nothrow ;			/* constant */


/* local typedefs */

typedef ino_t		ui ;


/* external subroutines */

extern "C" {
    extern int uc_getcwd(char *,int) noex ;
    extern int uc_readlink(cchar *,char *,int) noex ;
    extern int uc_stat(cchar *,ustat *) noex ;
}


/* external variables */


/* local structures */

struct dirid {
	ino_t		ino ;
	dev_t		dev ;
} ;

namespace {
    struct vars {
	int		maxpathlen ;
	int		maxlinklen ;
	operator int () noex ;
    } ; /* end struct (vars) */
}


/* forward references */

/****
	fifostr		*dqp ;		directory-queue-pointer
	fsdir		*dirp ;		directory-pointer
	hdb		*dip ;		directory-id-pointer
****/

template<typename ... Args>
local int fsdirtree_ctor(fsdirtree *op,Args ... args) noex {
    	FSDIRTREE	*hop = op ;
	cnullptr	np{} ;
	int		rs = SR_FAULT ;
	if (op && (args && ...)) ylikely {
	    memclear(hop) ;
	    rs = SR_NOMEM ;
	    if ((op->dqp = new(nothrow) fifostr) != np) ylikely {
	        if ((op->dirp = new(nothrow) fsdir) != np) ylikely {
	            if ((op->dip = new(nothrow) hdb) != np) ylikely {
			rs = SR_OK ;
		    } /* end if (new-hdb) */
		    if (rs < 0) {
			delete op->dirp ;
			op->dirp = nullptr ;
		    }
		} /* end if (new-fsdir) */
		if (rs < 0) {
		    delete op->dqp ;
		    op->dqp = nullptr ;
		}
	    } /* end if (new-fifostr) */
	} /* end if (non-null) */
	return rs ;
}
/* end subroutine (fsdirtree_ctor) */

local int fsdirtree_dtor(fsdirtree *op) noex {
	int		rs = SR_FAULT ;
	if (op) ylikely {
	    rs = SR_OK ;
	    if (op->dip) ylikely {
		delete op->dip ;
		op->dirp = nullptr ;
	    }
	    if (op->dirp) ylikely {
		delete op->dirp ;
		op->dirp = nullptr ;
	    }
	    if (op->dqp) ylikely {
		delete op->dqp ;
		op->dqp = nullptr ;
	    }
	} /* end if (non-null) */
	return rs ;
} /* end subroutine (fsdirtree_dtor) */

template<typename ... Args>
local inline int fsdirtree_magic(fsdirtree *op,Args ... args) noex {
	int		rs = SR_FAULT ;
	if (op && (args && ...)) ylikely {
	    rs = (op->magic == FSDIRTREE_MAGIC) ? SR_OK : SR_NOTOPEN ;
	}
	return rs ;
} /* end subroutine (fsdirtree_magic) */

local int	fsdirtree_sel(fsdirtree *) noex ;
local int	fsdirtree_trackbegin(fsdirtree *) noex ;
local int	fsdirtree_trackend(fsdirtree *) noex ;
local int	fsdirtree_dirbegin(fsdirtree *) noex ;
local int	fsdirtree_diradd(fsdirtree *,dev_t,ino_t) noex ;
local int	fsdirtree_dirhave(fsdirtree *,dev_t,ino_t,dirid **) noex ;
local int	fsdirtree_dirend(fsdirtree *) noex ;

local int	dirid_start(dirid *,dev_t,ino_t) noex ;
local int	dirid_finish(dirid *) noex ;

local int	diridcmp(dirid *,dirid *,int) noex ;

static uint	diridhash(cvoid *,int) noex ;

static inline bool btst(ushort v,int n) noex {
    	return bool((v >> n) & 1) ;
}

static inline bool interested(ushort sel,mode_t m) noex {
    	cint	ft = filetype(m) ;
	return btst(sel,ft) ;
}


/* local variables */

static vars		var ;


/* exported variables */

cint	fsdirtreems::pipe		= (1 << fsdirtreeo_pipe) ;
cint	fsdirtreems::chr		= (1 << fsdirtreeo_chr) ;
cint	fsdirtreems::dir		= (1 << fsdirtreeo_dir) ;
cint	fsdirtreems::name		= (1 << fsdirtreeo_name) ;
cint	fsdirtreems::blk		= (1 << fsdirtreeo_blk) ;
cint	fsdirtreems::reg		= (1 << fsdirtreeo_reg) ;
cint	fsdirtreems::lnk		= (1 << fsdirtreeo_lnk) ;
cint	fsdirtreems::sock		= (1 << fsdirtreeo_sock) ;
cint	fsdirtreems::door		= (1 << fsdirtreeo_door) ;
cint	fsdirtreems::wht		= (1 << fsdirtreeo_wht) ;
cint	fsdirtreems::follow		= (1 << fsdirtreeo_follow) ;
cint	fsdirtreems::noent		= (1 << fsdirtreeo_noent) ;
cint	fsdirtreems::uniqfile		= (1 << fsdirtreeo_uniqfile) ;
cint	fsdirtreems::uniqdir		= (1 << fsdirtreeo_uniqdir) ;
cint	fsdirtreems::igndotfile		= (1 << fsdirtreeo_igndotfile) ;
cint	fsdirtreems::igndotdir		= (1 << fsdirtreeo_igndotdir) ;

constexpr fsdirtreems	fsdirtreem ;	/* FSDIRTREE mask */


/* exported subroutines */

local int	fsdirtree_opener(fsdirtree *,cchar *) noex ;

int fsdirtree_open(fsdirtree *op,cchar *dname,int opts) noex {
	int		rs ;
	if ((rs = fsdirtree_ctor(op,dname)) >= 0) ylikely {
	    static cint		rsv = var ;
	    if ((rs = rsv) >= 0) ylikely {
	        op->opts = opts ;
	        if ((rs = fifostr_start(op->dqp)) >= 0) ylikely {
		    cint	sz = (var.maxlinklen + 1) ;
		    if (char *lp ; (rs = lm_mall(sz,&lp)) >= 0) ylikely {
			op->llen = rs ;
			op->lbuf = lp ;
			if (char *bp ; (rs = lm_mn(&bp)) >= 0) ylikely {
			    op->nlen = rs ;
			    op->nbuf = bp ;
			    if ((rs = lm_mp(&bp)) >= 0) {
				op->bnbuf = bp ;
				op->bnlen = rs ;
			        if ((rs = fsdirtree_sel(op)) >= 0) {
			            rs = fsdirtree_opener(op,dname) ;
			        }
				if (rs < 0) {
				    lm_free(op->bnbuf) ;
				    op->bnbuf = nullptr ;
				    op->bnlen = 0 ;
				}
			    } /* end if (memory-allocation) */
			    if (rs < 0) {
			        lm_free(op->nbuf) ;
			        op->nbuf = nullptr ;
				op->nlen = 0 ;
			    } /* end if (error-handle) */
			} /* end if (memory-allocation) */
			if (rs < 0) {
			    lm_free(op->lbuf) ;
			    op->lbuf = nullptr ;
			    op->llen = 0 ;
			} /* end if (error-handle) */
		    } /* end if (memory_allocation) */
	            if (rs < 0) {
	                fifostr_finish(op->dqp) ;
	            }
	        } /* end if (fifostr_start) */
	    } /* end if (vars) */
	    if (rs < 0) {
		fsdirtree_dtor(op) ;
	    }
	} /* end if (fsdirtree_ctor) */
	return rs ;
}
/* end subroutine (fsdirtree_open) */

local int fsdirtree_opener(fsdirtree *op,cchar *dname) noex {
    	cint		maxpath = var.maxpathlen ;
	int             rs = SR_OK ;
        cchar		*bdp = dname ;
        if ((bdp == nullptr) || (strcmp(bdp,".") == 0)) {
	    bdp = "" ;
	}
        if (bdp[0] != '/') {
            if ((rs = uc_getcwd(op->bnbuf,maxpath)) >= 0) {
                op->bndlen = rs ;
                if (bdp[0] != '\0') {
                    op->bnbuf[op->bndlen++] = '/' ;
		}
            }
        }
        if (rs >= 0) ylikely {
            if (bdp[0] != '\0') {
                cint	cl = maxpath - op->bndlen ;
                rs = sncpy((op->bnbuf + op->bndlen),cl,bdp) ;
                op->bndlen += rs ;
            }
            if (rs >= 0) ylikely {
                if ((rs = fsdir_open(op->dirp,op->bnbuf)) >= 0) {
                    op->fl.dir = true ;
                    if (op->bnbuf[op->bndlen - 1] != '/') {
                        op->bnbuf[op->bndlen++] = '/' ;
                    }
		    if ((rs = fsdirtree_trackbegin(op)) >= 0) {
                        op->cdnlen = op->bndlen ;
                        op->magic = FSDIRTREE_MAGIC ;
                    }
                    if (rs < 0) {
                        fsdir_close(op->dirp) ;
                    } /* end if (error) */
                } /* end if (fsdir) */
            } /* end if (ok) */
        } /* end if (ok) */
	return rs ;
}
/* end ubroutine (fsdirtree_opener) */

int fsdirtree_close(fsdirtree *op) noex {
	int		rs ;
	int		rs1 ;
	if ((rs = fsdirtree_magic(op)) >= 0) ylikely {
	    {
	    	rs1 = fsdirtree_trackend(op) ;
                if (rs >= 0) rs = rs1 ;
	    }
            if (op->fl.dir) ylikely {
                op->fl.dir = false ;
                rs1 = fsdir_close(op->dirp) ;
                if (rs >= 0) rs = rs1 ;
            }
            if (op->bnbuf) ylikely {
                rs1 = lm_free(op->bnbuf) ;
                if (rs >= 0) rs = rs1 ;
                op->bnbuf = nullptr ;
            }
            if (op->nbuf) ylikely {
                rs1 = lm_free(op->nbuf) ;
                if (rs >= 0) rs = rs1 ;
                op->nbuf = nullptr ;
                op->nlen = 0 ;
            }
            if (op->lbuf) ylikely {
                rs1 = lm_free(op->lbuf) ;
                if (rs >= 0) rs = rs1 ;
                op->lbuf = nullptr ;
                op->llen = 0 ;
            }
            if (op->dqp) ylikely {
                rs1 = fifostr_finish(op->dqp) ;
                if (rs >= 0) rs = rs1 ;
            }
            {
                rs1 = fsdirtree_dtor(op) ;
                if (rs >= 0) rs = rs1 ;
            }
            op->magic = 0 ;
	} /* end if (magic) */
	return rs ;
}
/* end subroutine (fsdirtree_close) */

int fsdirtree_read(fsdirtree *op,ustat *sbp,char *rbuf,int rlen) noex {
	int		rs ;
	int		len = 0 ;
	if ((rs = fsdirtree_magic(op,sbp,rbuf)) >= 0) ylikely {
	USTAT		se ;
	int		mlen ;
	int		clen ;
	int		flen ;
	int		ndir = 0 ;
	char		*fnp ;
	char		*cdnp = nullptr ;
	if (rlen < 0) rlen = var.maxpathlen ;
	while ((rs >= 0) && (! op->fl.feof)) {
	    fsdir_ent	de ;
	    cdnp = nullptr ;
	    if ((rs = fsdir_read(op->dirp,&de,op->nbuf,op->nlen)) > 0) {
	        int	enl = rs ;
	        cchar	*enp = op->nbuf ;
	        bool	f_proc = true ;
	        if (hasNotDots(enp,enl)) {
		    if (op->prune != nullptr) {
			f_proc = (matstr(op->prune,enp,enl) < 0) ;
		    }
		    if (f_proc) {
	            if ((op->cdnlen > 0) && 
	                (op->bnbuf[op->cdnlen - 1] != '/')) {
	                op->bnbuf[op->cdnlen++] = '/' ;
		    }
	            fnp = op->bnbuf + op->cdnlen ;
	            mlen = var.maxpathlen - op->cdnlen ;
	            flen = intconv(strwcpy(fnp,enp,mlen) - fnp) ;
	            if ((rs = u_lstat(op->bnbuf,sbp)) >= 0) {
	                if (S_ISLNK(sbp->st_mode)) {
	                    if (op->opts & FSDIRTREE_MFOLLOW) {
	                        cint	llen = op->llen ;
	                        cchar	*fn = op->bnbuf ;
	                        char	*lbuf = op->lbuf ;
	                        if ((rs = uc_readlink(fn,lbuf,llen)) >= 0) {
				    if (! isDotDir(lbuf)) {
	                                if ((rs = uc_stat(fn,&se)) >= 0) {
	                                    sbp = &se ;
	                                } else if (rs == SR_NOENT) {
	                                    rs = SR_OK ;
	                                }
				    } /* end if (not-dots) */
	                        }
	                    } /* end if (follow-option specified) */
	                } /* end if (is-link) */
	            } /* end if (u_lstat) */
		    } /* end if (not pruned) */
	        } else {
	            f_proc = false ;
		}
	        if ((rs >= 0) && f_proc) {
		    cbool	f_uniq = (op->opts & FSDIRTREE_MUNIQFILE) ;
	            if (S_ISDIR(sbp->st_mode) && f_uniq) {
	                dev_t	dev = sbp->st_dev ;
	                ino_t	ino = sbp->st_ino ;
	                int	rs1 = fsdirtree_dirhave(op,dev,ino,nullptr) ;
	                if (rs1 >= 0) {
	                    f_proc = false ;
	                } else if (rs1 == SR_NOTFOUND) {
	                    rs = fsdirtree_diradd(op,dev,ino) ;
	                } else {
	                    rs = rs1 ;
			}
	            } /* end if (directory-uniqueness check) */
	        } /* end if (directory-uniqueness check) */
	        if ((rs >= 0) && f_proc) {
	            cdnp = op->bnbuf + op->bndlen ;
	            clen = op->cdnlen - op->bndlen + flen ;
	            if (S_ISDIR(sbp->st_mode)) {
	                ndir += 1 ;
	                rs = fifostr_add(op->dqp,cdnp,clen) ;
	                if (rs < 0) break ;
	            } /* end if (special handling for directories) */
	            if ((op->opts & FSDIRTREE_MTYPE) == 0) break ;
	            if (interested(op->selset,sbp->st_mode)) break ;
	        } else if ((rs == SR_NOENT) || (rs == SR_ACCESS)) {
	            if (op->opts & FSDIRTREE_MNOENT) break ;
	            rs = SR_OK ;
	        } /* end if */
	    } else if (rs == 0) { /* EOF on directory read */
	        len = 0 ;
	        op->fl.dir = false ;
	        rs = fsdir_close(op->dirp) ;
	        if ((op->bndlen > 0) && (op->bnbuf[op->bndlen - 1] != '/')) {
	            op->bnbuf[op->bndlen++] = '/' ;
		}
	        while (rs >= 0) {
	            cdnp = op->bnbuf + op->bndlen ;
	            mlen = var.maxpathlen - op->bndlen ;
	            rs = fifostr_rem(op->dqp,cdnp,mlen) ;
	            len = rs ;
	            if ((rs < 0) && (rs != SR_NOTFOUND)) break ;
	            if (rs == SR_NOTFOUND) {
	                cdnp = nullptr ;
	                rs = SR_OK ;
	                len = 0 ;
	                op->fl.feof = true ;
	                break ;
	            }
	            cdnp[len] = '\0' ; /* not needed? */
	            op->cdnlen = op->bndlen + len ;
	            if ((rs = fsdir_open(op->dirp,op->bnbuf)) >= 0) {
	                op->fl.dir = true ;
	                break ;
	            } else if (isNotPresent(rs)) {
	                rs = SR_OK ;
	            }
	        } /* end while */
	    } /* end if (directory-read) */
	} /* end while (outer) */
	rbuf[0] = '\0' ;
	if ((rs >= 0) && (cdnp != nullptr) && (! op->fl.feof)) {
	    rs = mknpath1(rbuf,rlen,cdnp) ;
	    len = rs ;
	}
	} /* end if (magic) */
	return (rs >= 0) ? len : rs ;
}
/* end subroutine (fsdirtree_read) */

int fsdirtree_prune(fsdirtree *op,cchar **prune) noex {
    	int		rs ;
	if ((rs = fsdirtree_magic(op,prune)) >= 0) ylikely {
	    op->prune = prune ;
	}
	return rs ;
}
/* end subroutine (fsdirtree_prune) */


/* private subroutines */

constexpr cint	selsz = DT_OVERLAST ;

static void bset(bitset<selsz> &v,int n) noex {
    	v[n] = true ;
}

local int fsdirtree_sel(fsdirtree *op) noex {
    	const fsdirtreems	*dtp = &fsdirtreem ;
	bitset<selsz>	sel ;
    	cint		opts = op->opts ;
    	int		rs = SR_OK ;
	if (opts & dtp->pipe)	bset(sel,DT_PIPE) ;
	if (opts & dtp->chr)	bset(sel,DT_CHR) ;
	if (opts & dtp->dir)	bset(sel,DT_DIR) ;
	if (opts & dtp->name)	bset(sel,DT_NAME) ;
	if (opts & dtp->blk)	bset(sel,DT_BLK) ;
	if (opts & dtp->reg)	bset(sel,DT_REG) ;
	if (opts & dtp->lnk)	bset(sel,DT_LNK) ;
	if (opts & dtp->sock)	bset(sel,DT_SOCK) ;
	if (opts & dtp->door)	bset(sel,DT_DOOR) ;
	if (opts & dtp->wht)	bset(sel,DT_WHT) ;
	{
	    culong vset = sel.to_ulong() ;
	    op->selset = ushortconv(vset) ;
	}
	return rs ;
} /* end subroutine (fsdirtree_sel) */

local int fsdirtree_trackbegin(fsdirtree *op) noex {
    	int		rs = SR_OK ;
        if (op->opts & FSDIRTREE_MUNIQFILE) ylikely {
            if ((rs = fsdirtree_dirbegin(op)) >= 0) ylikely {
                if (ustat sb ; (rs = uc_stat(op->bnbuf,&sb)) >= 0) ylikely {
                    dev_t   dev = sb.st_dev ;
                    ino_t   ino = sb.st_ino ;
                    rs = fsdirtree_diradd(op,dev,ino) ;
                } /* end if (stat) */
                if (rs < 0) {
                    fsdirtree_dirend(op) ;
                }
            } /* end if (dir-tracking) */
        } /* end if (uniq traversal requested) */
	return rs ;
} /* end subroutine (fsdirtree_trackbegin) */

local int fsdirtree_trackend(fsdirtree *op) noex {
    	int		rs = SR_OK ;
	int		rs1 ;
        if (op->fl.dirids) {
            rs1 = fsdirtree_dirend(op) ;
            if (rs >= 0) rs = rs1 ;
        }
	return rs ;
} /* end subroutine (fsdirtree_trackend) */

local int fsdirtree_dirbegin(fsdirtree *op) noex {
	hdb		*dbp = op->dip ;
	cint		ne = FSDIRTREE_NENTS ;
	cint		at = 1 ;	/* use |lookaside(3dam)| */
	int		rs ;
	hdbcmp_f	cmp = hdbcmp_f(diridcmp) ;
	if ((rs = hdb_start(dbp,ne,at,diridhash,cmp)) >= 0) ylikely {
	    op->fl.dirids = true ;
	}
	return rs ;
}
/* end subroutine (fsdirtree_dirbegin) */

local int fsdirtree_dirend(fsdirtree *op) noex {
	int		rs = SR_OK ;
	int		rs1 ;
	if (op->fl.dirids) {
	    hdb		*dbp = op->dip ;
	    hdb_dat	key ;
	    hdb_dat	val ;
	    op->fl.dirids = false ;
	    if (hdb_cur	cur ; (rs1 = hdb_curbegin(dbp,&cur)) >= 0) ylikely {
	        dirid	*dip ;
	        while (hdb_curenum(dbp,&cur,&key,&val) >= 0) {
	            dip = (dirid *) val.buf ;
	            if (dip != nullptr) {
	                rs1 = dirid_finish(dip) ;
	                if (rs >= 0) rs = rs1 ;
	                rs1 = lm_free(dip) ;
	                if (rs >= 0) rs = rs1 ;
	            }
	        } /* end while */
	        hdb_curend(dbp,&cur) ;
	    } /* end if (cursor) */
	    if (rs >= 0) rs = rs1 ;
	    {
	        rs1 = hdb_finish(op->dip) ;
	        if (rs >= 0) rs = rs1 ;
	    }
	} /* end if (was activated) */
	return rs ;
}
/* end subroutine (fsdirtree_dirend) */

local int fsdirtree_diradd(fsdirtree *op,dev_t dev,ino_t ino) noex {
	hdb		*dbp = op->dip ;
	hdb_dat		key ;
	hdb_dat		val ;
	cint		sz = szof(dirid) ;
	int		rs ;
	if (dirid *dip ; (rs = lm_mall(sz,&dip)) >= 0) ylikely {
	    if ((rs = dirid_start(dip,dev,ino)) >= 0) ylikely {
	        key.buf = dip ;
	        key.len = szof(ino_t) + szof(dev_t) ;
	        val.buf = dip ;
	        val.len = sz ;
	        rs = hdb_store(dbp,key,val) ;
	        if (rs < 0) {
	            dirid_finish(dip) ;
		}
	    } /* end if (dirid-start) */
	    if (rs < 0) {
	        lm_free(dip) ;
	    }
	} /* end if (memory-allocation) */
	return rs ;
}
/* end subroutine (fsdirtree_diradd) */

local int fsdirtree_dirhave(fsdirtree *op,dev_t d,ui ino,dirid **rpp) noex {
	hdb		*dbp = op->dip ;
	hdb_dat		key ;
	hdb_dat		val ;
	dirid		did ;
	int		rs ;
	did.ino = ino ;
	did.dev = d ;
	key.buf = &did ;
	key.len = szof(ino_t) + szof(dev_t) ;
	if ((rs = hdb_fetch(dbp,key,nullptr,&val)) >= 0) ylikely {
	    if (rpp) *rpp = (dirid *) val.buf ;
	}
	return rs ;
}
/* end subroutine (fsdirtree_dirhave) */

local int dirid_start(dirid *dip,dev_t dev,ino_t ino) noex {
	int		rs = SR_FAULT ;
	if (dip) ylikely {
	    rs = SR_OK ;
	    dip->dev = dev ;
	    dip->ino = ino ;
	}
	return rs ;
}
/* end subroutine (dirid_start) */

local int dirid_finish(dirid *dip) noex {
	int		rs = SR_FAULT ;
	if (dip) ylikely {
	    rs = SR_OK ;
	}
	return rs ;
}
/* end subroutine (dirid_finish) */

vars::operator int () noex {
    	int		rs ;
	if ((rs = getbufsize(bufsize_mp)) >= 0) ylikely {
	    maxpathlen = rs ;
	    maxlinklen = (rs * MAXLINKLEN_MULT) ;
	}
    	return rs ;
}
/* end method (vars::operator) */

static uint diridhash(cvoid *vp,int vl) noex {
	uint		h = 0 ;
	ushort		*sa = (ushort *) vp ;
	h = h ^ ((sa[1] << 16) | sa[0]) ;
	h = h ^ ((sa[0] << 16) | sa[1]) ;
	if (vl > szof(uint)) {
	    h = h ^ ((sa[3] << 16) | sa[2]) ;
	    h = h ^ ((sa[2] << 16) | sa[3]) ;
	    if (vl > szof(ulong)) {
	        h = h ^ ((sa[5] << 16) | sa[4]) ;
	        h = h ^ ((sa[4] << 16) | sa[5]) ;
	        if (vl > (4*3)) {
	            h = h ^ ((sa[7] << 16) | sa[6]) ;
	            h = h ^ ((sa[6] << 16) | sa[7]) ;
	        }
	    }
	}
	return h ;
}
/* end subroutine (diridhash) */

local int diridcmp(dirid *e1p,dirid *e2p,int len) noex {
	int64_t		d = int64_t(e1p->dev - e2p->dev) ;
	int		rc = 0 ;
	(void) len ;
	if (d == 0) {
	    d = (e1p->ino - e2p->ino) ;
	}
	{
	    if (d > 0) {
		rc = 1 ;
	    } else if (d < 0) {
		rc = -1 ;
	    }
	}
	return rc ;
}
/* end subroutine (diridcmp) */


