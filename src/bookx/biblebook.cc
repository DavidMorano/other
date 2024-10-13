/* biblebook SUPPORT */
/* lang=C++20 */

/* BIBLEBOOK object-load management */
/* version %I% last-modified %G% */


/* revision history:

	- 2008-10-01, David A­D­ Morano
	This module was originally written.

*/

/* Copyright © 2008 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

  	Name:
	biblebook

	Description:
	This module implements an interface (a trivial one) that
	allows access to the BIBLEBOOK datbase.

*******************************************************************************/

#include	<envstandards.h>	/* MUST be first to configure */
#include	<sys/types.h>
#include	<sys/param.h>
#include	<unistd.h>
#include	<fcntl.h>
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<cstring>
#include	<usystem.h>
#include	<vecstr.h>
#include	<char.h>
#include	<localmisc.h>

#include	"biblebook.h"
#include	"biblebooks.h"


/* local defines */

#define	BIBLEBOOK_MODBNAME	"biblebooks"
#define	BIBLEBOOK_OBJNAME	"biblebooks"

#ifndef	SYMNAMELEN
#define	SYMNAMELEN	60
#endif

#define	tolc(c)		CHAR_TOLC(c)
#define	touc(c)		CHAR_TOUC(c)
#define	tofc(c)		CHAR_TOFC(c)


/* external subroutines */

extern int	sncpy1(char *,int,cchar *) ;
extern int	sncpy2(char *,int,cchar *,cchar *) ;
extern int	sncpy3(char *,int,cchar *,cchar *,cchar *) ;
extern int	snwcpy(char *,int,cchar *,int) ;
extern int	mkpath2(char *,cchar *,cchar *) ;
extern int	mkpath3(char *,cchar *,cchar *,cchar *) ;
extern int	mkpath4(char *,cchar *,cchar *,cchar *,cchar *) ;
extern int	matstr(cchar **,cchar *,int) ;
extern int	matostr(cchar **,int,cchar *,int) ;
extern int	matocasestr(cchar **,int,cchar *,int) ;
extern int	matpstr(cchar **,int,cchar *,int) ;

extern char	*strwcpy(char *,cchar *,int) ;


/* local structures */


/* forward references */

static int	biblebook_openload(BIBLEBOOK *,cchar *,cchar *) noex ;
static int	biblebook_openlocal(BIBLEBOOK *) noex ;

static int	biblebook_objloadbegin(BIBLEBOOK *,cchar *,cchar *) noex ;
static int	biblebook_objloadend(BIBLEBOOK *) noex ;
static int	biblebook_loadcalls(BIBLEBOOK *,cchar *) noex ;

static int	biblebook_matcher(BIBLEBOOK *,cchar *,int) noex ;
static int	biblebook_loadnames(BIBLEBOOK *) noex ;
static int	biblebook_loadnameslocal(BIBLEBOOK *) noex ;
static int	biblebook_loadnamesremote(BIBLEBOOK *) noex ;

static bool	isrequired(int) noex ;

static char	*strwcpyspecial(char *,cchar *,int) noex ;


/* external variables */


/* local variables */

enum subs {
	sub_open,
	sub_count,
	sub_max,
	sub_lookup,
	sub_get,
	sub_match,
	sub_size,
	sub_audit,
	sub_close,
	sub_overlast
} ;

constexpr cpcchar		subs[] = {
	"open",
	"count",
	"max",
	"lookup",
	"get",
	"match",
	"size",
	"audit",
	"close",
	nullptr
} ;

constexpt cpcchar		booknames[] = {
	"Bible",
	"Genesis",
	"Exodus",
	"Leviticus",
	"Numbers",
	"Deuteronomy",
	"Joshua",
	"Judges",
	"Ruth",
	"I Samuel",
	"II Samuel",
	"I Kings",
	"II Kings",
	"I Chronicles",
	"II Chronicles",
	"Ezra",
	"Nehemiah",
	"Esther",
	"Job",
	"Psalms",
	"Proverbs",
	"Ecclesiastes",
	"Song of Solomon",
	"Isaiah",
	"Jeremiah",
	"Lamentations",
	"Ezekiel",
	"Daniel",
	"Hosea",
	"Joel",
	"Amos",
	"Obadiah",
	"Jonah",
	"Micah",
	"Nahum",
	"Habakkuk",
	"Zephaniah",
	"Haggai",
	"Zechariah",
	"Malachi",
	"Matthew",
	"Mark",
	"Luke",
	"John",
	"Acts",
	"Romans",
	"I Corinthians",
	"II Corinthians",
	"Galatians",
	"Ephesians",
	"Philippians",
	"Colossians",
	"I Thessalonians",
	"II Thessalonians",
	"I Timothy",
	"II Timothy",
	"Titus",
	"Philemon",
	"Hebrews",
	"James",
	"I Peter",
	"II Peter",
	"I John",
	"II John",
	"III John",
	"Jude",
	"Revelation",
	nullptr
} ;


/* exported variables */


/* exported subroutines */

int biblebook_open(BIBLEBOOK *op,cchar *pr,cchar *dbname) noex {
	int		rs ;

	if (op == nullptr) return SR_FAULT ;
	if (pr == nullptr) return SR_FAULT ;

	if (pr[0] == '\0') return SR_INVALID ;

	memset(op,0,sizeof(BIBLEBOOK)) ;

	if ((rs = biblebook_openload(op,pr,dbname)) == SR_NOENT) {
	    rs = biblebook_openlocal(op) ;
	}
	if (rs >= 0) {
	    op->magic = BIBLEBOOK_MAGIC ;
	}

	return rs ;
}
/* end subroutine (biblebook_open) */

int biblebook_close(BIBLEBOOK *op) noex {
	int		rs = SR_OK ;
	int		rs1 ;

	if (op == nullptr) return SR_FAULT ;

	if (op->magic != BIBLEBOOK_MAGIC) return SR_NOTOPEN ;

	if (op->names != nullptr) {
	    rs1 = uc_free(op->names) ;
	    if (rs >= 0) rs = rs1 ;
	    op->names = nullptr ;
	}

	if (! op->f.localdb) {

	    rs1 = (*op->call.close)(op->obj) ;
	    if (rs >= 0) rs = rs1 ;

	    rs1 = biblebook_objloadend(op) ;
	    if (rs >= 0) rs = rs1 ;

	} /* end if */

	op->magic = 0 ;
	return rs ;
}
/* end subroutine (biblebook_close) */

int biblebook_count(BIBLEBOOK *op) noex {
	int		rs = SR_NOSYS ;

	if (op == nullptr) return SR_FAULT ;

	if (op->magic != BIBLEBOOK_MAGIC) return SR_NOTOPEN ;

	if (op->f.localdb) {
	    rs = nelem(booknames) - 1 ;
	} else {
	    if (op->call.count != nullptr) {
	        rs = (*op->call.count)(op->obj) ;
	    }
	} /* end if */

	return rs ;
}
/* end subroutine (biblebook_count) */

int biblebook_max(BIBLEBOOK *op) noex {
	int		rs ;
	int		max = 0 ;

	if ((rs = biblebook_count(op)) >= 0) {
	    max = (rs - 1) ;
	}

	return (rs >= 0) ? max : rs ;
}
/* end subroutine (biblebook_max) */

int biblebook_audit(BIBLEBOOK *op) noex {
	int		rs = SR_OK ;

	if (op == nullptr) return SR_FAULT ;

	if (op->magic != BIBLEBOOK_MAGIC) return SR_NOTOPEN ;

	if (! op->f.localdb) {
	    rs = SR_NOSYS ;
	    if (op->call.audit != nullptr) {
	        rs = (*op->call.audit)(op->obj) ;
	    }
	} /* end if */

	return rs ;
}
/* end subroutine (biblebook_audit) */

int biblebook_lookup(BIBLEBOOK *op,char *rbuf,int rlen,int bi) noex {
	int		rs ;

	if (op == nullptr) return SR_FAULT ;
	if (rbuf == nullptr) return SR_FAULT ;

	if (op->magic != BIBLEBOOK_MAGIC) return SR_NOTOPEN ;

	if (bi < 0) return SR_INVALID ;

	if (op->f.localdb) {
	    cint	n = (nelem(booknames) - 1) ;
	    if (bi < n) {
	        rs = sncpy1(rbuf,rlen,booknames[bi]) ;
	    } else {
	        rs = SR_NOTFOUND ;
	    }
	} else {
	    rs = (*op->call.lookup)(op->obj,rbuf,rlen,bi) ;
	}

	return rs ;
}
/* end subroutine (biblebook_lookup) */

int biblebook_read(BIBLEBOOK *op,char *rbuf,int rlen,int bi) noex {
	return biblebook_lookup(op,rbuf,rlen,bi) ;
}
/* end subroutine (biblebook_read) */

int biblebook_get(BIBLEBOOK *op,int bi,char *rbuf,int rlen) noex {
	return biblebook_lookup(op,rbuf,rlen,bi) ;
}
/* end subroutine (biblebook_get) */

int biblebook_match(BIBLEBOOK *op,cchar *mbuf,int mlen) noex {
	int		rs = SR_NOSYS ;

	if (op == nullptr) return SR_FAULT ;
	if (mbuf == nullptr) return SR_FAULT ;

	if (op->magic != BIBLEBOOK_MAGIC) return SR_NOTOPEN ;

	if (! op->f.localdb) {
	    rs = SR_NOSYS ;
	    if (op->call.match != nullptr) {
	        rs = (*op->call.match)(op->obj,mbuf,mlen) ;
	    }
	} 

	if (op->f.localdb || (rs == SR_NOSYS)) {
	    rs = biblebook_matcher(op,mbuf,mlen) ;
	}

	return rs ;
}
/* end subroutine (biblebook_match) */

int biblebook_size(BIBLEBOOK *op) noex {
	if (op == nullptr) return SR_FAULT ;
	if (op->magic != BIBLEBOOK_MAGIC) return SR_NOTOPEN ;
	return biblebook_loadnames(op) ;
}
/* end subroutine (biblebook_size) */


/* private subroutines */

static int biblebook_openload(BIBLEBOOK *op,cchar *pr,cchar *dbname) noex {
	int		rs ;
	cchar	*objname = BIBLEBOOK_OBJNAME ;

	if ((rs = biblebook_objloadbegin(op,pr,objname)) >= 0) {
	    rs = (*op->call.open)(op->obj,pr,dbname) ;
	    if (rs < 0)
	        biblebook_objloadend(op) ;
	}

	return rs ;
}
/* end subroutine (biblebook_openload) */

static int biblebook_openlocal(BIBLEBOOK *op) noex {
	op->f.localdb = true ;
	return SR_OK ;
}
/* end subroutine (biblebook_openlocal) */

static int biblebook_objloadbegin(BIBLEBOOK *op,cchar *pr,cchar *objname) noex {
	MODLOAD		*lp = &op->loader ;
	vecstr		syms ;
	cint	n = nelem(subs) ;
	int		rs ;
	int		rs1 ;
	int		opts ;

	opts = VECSTR_OCOMPACT ;
	if ((rs = vecstr_start(&syms,n,opts)) >= 0) {
	    cint	nlen = SYMNAMELEN ;
	    int		i ;
	    int		f_modload = false ;
	    char	nbuf[SYMNAMELEN + 1] ;

	    for (i = 0 ; (i < n) && (subs[i] != nullptr) ; i += 1) {
	        if (isrequired(i)) {
	            if ((rs = sncpy3(nbuf,nlen,objname,"_",subs[i])) >= 0) {
			rs = vecstr_add(&syms,nbuf,rs) ;
		    }
		}
		if (rs < 0) break ;
	    } /* end for */

	    if (rs >= 0) {
		cchar	**sv ;
	        if ((rs = vecstr_getvec(&syms,&sv)) >= 0) {
	            cchar	*modbname = BIBLEBOOK_MODBNAME ;
	            opts = (MODLOAD_OLIBVAR | MODLOAD_OSDIRS) ;
	            rs = modload_open(lp,pr,modbname,objname,opts,sv) ;
		    f_modload = (rs >= 0)  ;
		}
	    }

	    rs1 = vecstr_finish(&syms) ;
	    if (rs >= 0) rs = rs1 ;
	    if ((rs < 0) && f_modload) {
		modload_close(lp) ;
	    }
	} /* end if (allocation) */

	if (rs >= 0) {
	    if ((rs = modload_getmv(lp,0)) >= 0) {
		void	*p ;

		op->objsize = rs ;
		if ((rs = uc_malloc(op->objsize,&p)) >= 0) {
		    op->obj = p ;
		    rs = biblebook_loadcalls(op,objname) ;
		    if (rs < 0) {
			uc_free(op->obj) ;
			op->obj = nullptr ;
		    }
		} /* end if (memory-allocation) */
	    } /* end if (getmva) */
	    if (rs < 0)
		modload_close(lp) ;
	} /* end if (ok) */

	return rs ;
}
/* end subroutine (biblebook_objloadbegin) */

static int biblebook_objloadend(BIBLEBOOK *op) noex {
	int		rs = SR_OK ;
	int		rs1 ;

	if (op->obj != nullptr) {
	    rs1 = uc_free(op->obj) ;
	    if (rs >= 0) rs = rs1 ;
	    op->obj = nullptr ;
	}

	rs1 = modload_close(&op->loader) ;
	if (rs >= 0) rs = rs1 ;

	return rs ;
}
/* end subroutine (biblebook_objloadend) */

static int biblebook_loadcalls(BIBLEBOOK *op,cchar *objname) noex {
	modload		*lp = &op->loader ;
	cint		nlen = SYMNAMELEN ;
	int		rs = SR_OK ;
	int		i ;
	int		c = 0 ;
	char		nbuf[SYMNAMELEN + 1] ;
	cvoid		*snp ;

	for (i = 0 ; subs[i] != nullptr ; i += 1) {

	    if ((rs = sncpy3(nbuf,nlen,objname,"_",subs[i])) >= 0) {
	         if ((rs = modload_getsym(lp,nbuf,&snp)) == SR_NOTFOUND) {
		     snp = nullptr ;
		     if (! isrequired(i)) rs = SR_OK ;
		}
	    }

	    if (rs < 0) break ;

	    if (snp != nullptr) {
	        c += 1 ;
	        switch (i) {
	        case sub_open:
	            op->call.open = 
	                (int (*)(void *,cchar *,cchar *)) snp ;
	            break ;
	        case sub_count:
	            op->call.count = (int (*)(void *)) snp ;
	            break ;
	        case sub_max:
	            op->call.max = (int (*)(void *)) snp ;
	            break ;
	        case sub_lookup:
	            op->call.lookup = (int (*)(void *,char *,int,int)) snp ;
	            break ;
	        case sub_get:
	            op->call.get = (int (*)(void *,int,char *,int)) snp ;
	            break ;
	        case sub_match:
	            op->call.match = (int (*)(void *,cchar *,int)) snp ;
	            break ;
	        case sub_size:
	            op->call.size = (int (*)(void *)) snp ;
	            break ;
	        case sub_audit:
	            op->call.audit = (int (*)(void *)) snp ;
	            break ;
	        case sub_close:
	            op->call.close = (int (*)(void *)) snp ;
	            break ;
	        } /* end switch */
	    } /* end if (it had the call) */

	} /* end for (subs) */

	return (rs >= 0) ? c : rs ;
}
/* end subroutine (biblebook_loadcalls) */

static int biblebook_matcher(BIBLEBOOK *op,cchar *mbuf,int mlen) noex {
	int		rs = SR_OK ;
	int		i = 0 ;

	if (op->names == nullptr) {
	    rs = biblebook_loadnames(op) ;
	}

	if (rs >= 0) {
	    int		ml ;
	    int		bl ;
	    char	bbuf[BIBLEBOOK_LEN + 1] ;

	    ml = MIN(mlen,BIBLEBOOK_LEN) ;
	    bl = strwcpyspecial(bbuf,mbuf,ml) - bbuf ;

	    rs = matostr(op->names,1,bbuf,bl) ;
	    i = rs ;
	    if (rs < 0) rs = SR_NOTFOUND ;

	} /* end if (ok) */

	return (rs >= 0) ? i : rs ;
}
/* end subroutine (biblebook_matcher) */

static int biblebook_loadnames(BIBLEBOOK *op) noex {
	int		rs = SR_OK ;
	if (op->names == nullptr) {
	    if (op->f.localdb) {
	        rs = biblebook_loadnameslocal(op) ;
	    } else {
	        rs = biblebook_loadnamesremote(op) ;
	    }
	} else {
	    rs = op->namesize ;
	}
	return rs ;
}
/* end subroutine (biblebook_loadnames) */

static int biblebook_loadnameslocal(BIBLEBOOK *op) noex {
	int		rs ;
	int		sizetab ;
	int		size = 0 ;
	int		namesize = 0 ;
	int		n ;
	char		*mp ;

	for (n = 0 ; booknames[n] != nullptr ; n += 1) {
	    namesize += (strlen(booknames[n]) + 1) ;
	}

	sizetab = (n + 1) * sizeof(cchar *) ;
	size += sizetab ;
	size += namesize ;
	if ((rs = uc_malloc(size,&mp)) >= 0) {
	    int		i ;
	    cchar	**names = (cchar **) mp ;
	    char	*bp = mp ;
	    bp = (mp + sizetab) ;
	    op->namestrs = bp ;
	    for (i = 0 ; i < n ; i += 1) {
	        names[i] = bp ;
	        bp = strwcpyspecial(bp,booknames[i],-1) + 1 ;
	    } /* end for */
	    names[i] = nullptr ;
	    op->names = names ;
	    op->namesize = namesize ;
	} /* end if (memory-allocation) */

	return (rs >= 0) ? namesize : rs ;
}
/* end subroutine (biblebook_loadnameslocal) */

static int biblebook_loadnamesremote(BIBLEBOOK *op) noex {
	cint		blen = BIBLEBOOK_LEN ;
	int		rs = SR_OK ;
	int		n = 0 ;
	int		namesize = 0 ;
	char		bbuf[BIBLEBOOK_LEN + 1] ;

	if (op->call.size != nullptr) {
	    if ((rs = (*op->call.size)(op->obj)) >= 0) {
		namesize = rs ;
		rs = (*op->call.count)(op->obj) ;
		n = rs ;
	    }
	} else {
	    for (n = 0 ; rs >= 0 ; n += 1) {
	        if ((rs = (*op->call.get)(op->obj,n,bbuf,blen)) >= 0) {
		    namesize += (rs+1) ;
	        } else {
		    if (rs == SR_NOTFOUND) rs = SR_OK ;
		    break ;
	        }
	    } /* end for */
	} /* end if */

	if (rs >= 0) {
	    cint	sizetab = (n + 1) * sizeof(cchar *) ;
	    int		size = namesize ;
	    int		bl ;
	    char	*mp ;
	    size += sizetab ;
	    if ((rs = uc_malloc(size,&mp)) >= 0) {
		int		i ;
		cchar	**names = (cchar **) mp ;
		char		*bp = (mp + sizetab) ;
	        op->namestrs = bp ;
	        for (i = 0 ; (rs >= 0) && (i < n) ; i += 1) {
	            if ((rs = (*op->call.get)(op->obj,i,bbuf,blen)) >= 0) {
	                bl = rs ;
	                names[i] = bp ;
	                bp = strwcpyspecial(bp,bbuf,bl) + 1 ;
	            }
	        } /* end for */
	        names[i] = nullptr ;
	        op->namesize = namesize ;
	        if (rs < 0) { /* should not or cannot fail! */
	            uc_free(mp) ;
	        } else
	            op->names = names ;
	    } /* end if (m-a) */
	} /* end if (ok) */

	return (rs >= 0) ? namesize : rs ;
}
/* end subroutine (biblebook_loadnamesremote) */

static bool isrequired(int i) noex {
	bool		f = false ;
	switch (i) {
	case sub_open:
	case sub_count:
	case sub_lookup:
	case sub_close:
	    f = true ;
	    break ;
	} /* end switch */
	return f ;
}
/* end subroutine (isrequired) */

static char *strwcpyspecial(char *dp,cchar *sp,int sl)
{

	if (sl >= 0) {
	    while (sl && (*sp != '\0')) {
		if (! CHAR_ISWHITE(*sp)) *dp++ = tolc(*sp) ;
		sp += 1 ;
		sl -= 1 ;
	    }
	} else {
	    while (*sp != '\0') {
		if (! CHAR_ISWHITE(*sp)) *dp++ = tolc(*sp) ;
		sp += 1 ;
	    }
	} /* end if */

	*dp = '\0' ;
	return dp ;
}
/* end subroutine (strwcpyspecial) */


