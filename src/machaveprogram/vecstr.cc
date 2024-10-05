/* vecstr SUPPORT */
/* lang=C++20 */

/* vector string operations */
/* version %I% last-modified %G% */

#define	CF_QSORT	0		/* use |qsort(3c)| */

/* revision history:

	= 1998-12-01, David A­D­ Morano
	This subroutine was written for Rightcore Network Services.

	= 2023-10-07, David A­D­ Morano
	I updated this to match the updates applied to this object back
	in 2018 in other projects (that used this same object).

*/

/* Copyright © 1998,2023 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	These routines are used when the caller wants to store a COPY of the
	passed string data into a vector.  These routines will copy and store
	the copied data in the list.  The advantage is that the caller does not
	have to keep the orginal data around in order for the list data to be
	accessed later.  String data (unlike "element" data) can not contain
	nullptr characters-bytes.

	Notes:
	The method |vecstr_find| is equivalent to:
		vecstr_finder(lp,string,nullptr,nullptr)
	but is shorter (and hopefully faster).


	Name:
	vecstr_adduniq

	Description:
	This subroutine adds a string to the object, but first
	checks to see if it is already present.  Specified strings
	that already match an existing entry are not added.

	Synopsis:
	int vecstr_adduniq(vecstr *op,cchar *sp,int sl) noex

	Arguments:
	op		vecstr string to add to
	sp		pointer to key string to add
	sl		length of string to add

	Returns:
	INT_MAX		string was already in container
	>=0		OK
	<0		error

	Implementation reminders:
	1. The central vector table is zeroth based. That is, read
	c-strings -- the pointers to those c-strings -- occupt the
	zeroth slot and then continuing upwards.
	2. When making a "string-table" and the assciated record-table,
	the zero-th entry in both the string table and the record
	table are special in that they are not (really) used.  The
	zero-th index into the string-table references a NUL
	(zero-terminated) c-string (which is just a single zero
	(NUL) character.  And the zero-th element of the record
	table contains a zero value (which could be interpreted as
	referencing the corresponding zero-th entry in the string-table.
	But that entry is made-up, in that it was not actually
	encountered in the data (the data accummulated).
	3. Why the zero entries in both the string-table and the
	record-table? Because when making index tables, we want
	the number zero in any slot of the index table to mean that
	the table entry is "empty" and not being used to reference
	any strings anywhere (either in the string-table or in the
	record-table).
	4. Enjoy.

*******************************************************************************/

#include	<envstandards.h>	/* MUST be first to configure */
#include	<sys/types.h>
#include	<cstdlib>
#include	<cstring>		/* <- |strlen(3c)| */
#include	<algorithm>
#include	<usystem.h>
#include	<usupport.h>
#include	<nulstr.h>
#include	<nleadstr.h>
#include	<strwcpy.h>
#include	<vstrcmpx.h>
#include	<intceil.h>
#include	<localmisc.h>

#include	"vecstr.h"


/* local defines */


/* namespaces */

using std::sort ;


/* local typedefs */

extern "C" {
    typedef int (*vs_f)(cvoid *,cvoid *) noex ;
    typedef int (*qsort_f)(cvoid *,cvoid *) noex ;
}


/* external subroutines */


/* forward references */

static int	vecstr_ctor(vecstr *) noex ;
static int	vecstr_setopts(vecstr *,int) noex ;
static int	vecstr_extvec(vecstr *) noex ;
static int	vecstr_addsp(vecstr *,cchar *) noex ;
static int	vecstr_insertsp(vecstr *,int,cchar *) noex ;
static int	vecstr_validx(vecstr *,int) noex ;
static void	vecstr_arrsort(vecstr *,vecstr_vcmp) noex ;


/* local subroutines */

consteval int mkoptmask() noex {
	int		m = 0 ;
	m |= VECSTR_OREUSE ;
	m |= VECSTR_OCOMPACT ;
	m |= VECSTR_OSWAP ;
	m |= VECSTR_OSTATIONARY ;
	m |= VECSTR_OCONSERVE ;
	m |= VECSTR_OSORTED ;
	m |= VECSTR_OORDERED ;
	return m ;
}
/* end subroutine (mkoptmask) */


/* local variables */

constexpr bool		f_qsort = CF_QSORT ;

constexpr int		optmask = mkoptmask() ;

constexpr int		rsn = SR_NOTFOUND ;
constexpr int		resz = sizeof(int) ;


/* exported subroutines */

int vecstr_start(vecstr *op,int n,int opts) noex {
	int		rs ;
	if (n <= 1) n = VECSTR_DEFENTS ;
	if ((rs = vecstr_ctor(op)) >= 0) {
	    op->n = n ;
	    if ((rs = vecstr_setopts(op,opts)) >= 0) {
	        cint	size = (n + 1) * sizeof(cchar **) ;
	        void	*va{} ;
	        if ((rs = uc_libmalloc(size,&va)) >= 0) {
	            op->va = (cchar **) va ;
	            op->va[0] = nullptr ;
	            op->stsize = 1 ;
		    op->f.stsize = true ;	/* starts off true */
	        }
	    } /* end if */
	} /* end if (non-null) */
	return (rs >= 0) ? n : rs ;
}
/* end subroutine (vecstr_start) */

int vecstr_finish(vecstr *op) noex {
	int		rs = SR_FAULT ;
	int		rs1 ;
	if (op) {
	    rs = SR_OK ;
	    if (op->va) {
	        for (int i = 0 ; i < op->i ; i += 1) {
		    cchar	*ep = op->va[i] ;
	            if (ep) {
	                rs1 = uc_libfree(ep) ;
		        if (rs >= 0) rs = rs1 ;
	            }
	        } /* end for */
		{
	            rs1 = uc_libfree(op->va) ;
	            if (rs >= 0) rs = rs1 ;
	            op->va = nullptr ;
		}
	    } /* end if (populated) */
	    op->i = 0 ;
	    op->n = 0 ;
	    op->c = 0 ;
	} /* end if (non-null) */
	return rs ;
}
/* end subroutine (vecstr_finish) */

int vecstr_audit(vecstr *op) noex {
	int		rs = SR_FAULT ;
	int		c = 0 ;
	if (op) {
	    rs = SR_OK ;
	    if (op->va) {
	        for (int i = 0 ; i < op->i ; i += 1) {
		    cchar	*ep = op->va[i] ;
	            if (ep) {
	                cchar	*cp = (cchar *) ep ;
	                rs |= *cp ;/* access might SEGFAULT (as wanted) */
		        c += 1 ;
	            }
	        } /* end for */
	        rs = (c == op->c) ? SR_OK : SR_BADFMT ;
	    } /* end if (populated) */
	} /* end if (non-null) */
	return (rs >= 0) ? c : rs ;
}
/* end subroutine (vecstr_audit) */

int vecstr_add(vecstr *op,cchar *sp,int sl) noex {
	return vecstr_store(op,sp,sl,nullptr) ;
}
/* end subroutine (vecstr_add) */

int vecstr_adduniq(vecstr *op,cchar *sp,int sl) noex {
	int		rs = SR_FAULT ;
	if (op && sp) {
	    if (sl < 0) sl = strlen(sp) ;
	    if ((rs = vecstr_findn(op,sp,sl)) >= 0) {
	        rs = INT_MAX ;
	    } else if (rs == SR_NOTFOUND) {
	        rs = vecstr_add(op,sp,sl) ;
	    }
	} /* end if (non-null) */
	return rs ;
}
/* end subroutine (vecstr_adduniq) */

int vecstr_addkeyval(vecstr *op,cchar *kp,int kl,cchar *vp,int vl) noex {
	int		rs = SR_FAULT ;
	int		i = 0 ;
	if (op && kp) {
	    rs = SR_OK ;
	    if (kl < 0) kl = strlen(kp) ;
	    if ((vl < 0) || ((vp == nullptr) && (vl != 0))) {
	        vl = (vp) ? strlen(vp) : 0 ;
	    }
	    if ((op->i + 1) > op->n) {
	        rs = vecstr_extvec(op) ;
	    }
	    if (rs >= 0) {
	        cint	size = (kl + 1 + vl + 1) ;
	        char	*ap{} ;
	        if ((rs = uc_libmalloc(size,&ap)) >= 0) {
		    char	*bp = ap ;
	            bp = strwcpy(bp,kp,kl) ;
	            *bp++ = '=' ;
	            if (vp) bp = strwcpy(bp,vp,vl) ;
	            *bp = '\0' ;
	            op->stsize += size ;
	            i = vecstr_addsp(op,ap) ;
	        } /* end if (memory-allocation) */
	    } /* end if (ok) */
	} /* end if (non-null) */
	return (rs >= 0) ? i : rs ;
}
/* end subroutine (vecstr_addkeyval) */

int vecstr_store(vecstr *op,cchar *sp,int sl,cchar **rpp) noex {
	int		rs = SR_FAULT ;
	int		i = 0 ;
	if (op) {
	    rs = SR_OK ;
	    if (sl < 0) sl = strlen(sp) ;
	    if ((op->i + 1) > op->n) {
	        rs = vecstr_extvec(op) ;
	    }
	    if (rs >= 0) {
	        cint	size = (sl+1) ;
	        char	*bp ;
	        if ((rs = uc_libmalloc(size,&bp)) >= 0) {
	            strwcpy(bp,sp,sl) ;
	            op->stsize += size ;
	            i = vecstr_addsp(op,bp) ;
		    if (rpp) *rpp = bp ;
	        } /* end if (m-a) */
	    } /* end if (ok) */
	    if (rpp && (rs < 0)) *rpp = nullptr ;
	} /* end if (non-null) */
	return (rs >= 0) ? i : rs ;
}
/* end subroutine (vecstr_store) */

int vecstr_already(vecstr *op,cchar *sp,int sl) noex {
	return vecstr_findn(op,sp,sl) ;
}
/* end subroutine (vecstr_already) */

int vecstr_get(vecstr *op,int i,cchar **spp) noex {
	int		rs = SR_FAULT ;
	if (op) {
	    rs = SR_NOTFOUND ;
	    if (op->va) {
		rs = vecstr_validx(op,i) ;
	        if (spp) {
	            *spp = (rs >= 0) ? op->va[i] : nullptr ;
	        }
	    } /* end if (populated) */
	} /* end if (non-null) */
	return rs ;
}
/* end subroutine (vecstr_get) */

int vecstr_getlast(vecstr *op,cchar **spp) noex {
	int		rs = SR_FAULT ;
	int		i = 0 ;
	if (op) {
	    rs = SR_NOTFOUND ;
	    if (op->va && (op->c > 0)) {
	        i = (op->i-1) ;
	        while ((i >= 0) && (op->va[i] == nullptr)) {
		    i -= 1 ;
	        }
	        if (i >= 0) rs = SR_OK ;
	    } /* end if (populated) */
	    if (spp) {
	         *spp = (rs >= 0) ? op->va[i] : nullptr ;
	    }
	} /* end if (non-null) */
	return (rs >= 0) ? i : rs ;
}
/* end subroutine (vecstr_getlast) */

int vecstr_del(vecstr *op,int i) noex {
	int		rs = SR_FAULT ;
	if (op) {
	    rs = SR_NOTFOUND ;
	    if (op->va) {
		if ((rs = vecstr_validx(op,i)) >= 0) {
		    bool	f_fi = false ;
	            if (op->va[i] != nullptr) {
	                op->c -= 1 ;		/* decrement list count */
	                if (op->f.stsize) {
	                    op->stsize -= (strlen(op->va[i]) + 1) ;
	                }
	    		uc_libfree(op->va[i]) ;
		    } /* end if (freeing the actual string data) */
		    if (op->f.ostationary) {
	    	        op->va[i] = nullptr ;
	    	        if (i == (op->i - 1)) op->i -= 1 ;
	    	        f_fi = true ;
		    } else if (op->f.issorted || op->f.oordered) {
	                if (op->f.ocompact) {
		            int	j ;
	                    op->i -= 1 ;
	                    for (j = i ; j < op->i ; j += 1) {
	                        op->va[j] = op->va[j + 1] ;
		            }
	                    op->va[op->i] = nullptr ;
	                } else {
	                    op->va[i] = nullptr ;
	                    if (i == (op->i - 1)) op->i -= 1 ;
	                    f_fi = true ;
	                } /* end if */
	            } else {
			bool	f = (op->f.oswap || op->f.ocompact) ;
			f = f && (i < (op->i - 1)) ;
			if (f) {
	                    op->va[i] = op->va[op->i - 1] ;
	                    op->va[--op->i] = nullptr ;
	                    op->f.issorted = false ;
	                } else {
	                    op->va[i] = nullptr ;
	                    if (i == (op->i - 1)) op->i -= 1 ;
	                    f_fi = true ;
	                } /* end if */
	            } /* end if */
	            if (op->f.oconserve) {
	                while (op->i > i) {
	                    if (op->va[op->i - 1] != nullptr) break ;
	                    op->i -= 1 ;
	                } /* end while */
	            } /* end if */
	            if (f_fi && (i < op->fi)) op->fi = i ;
	        } /* end if (valid index) */
		op->f.stsize = false ;
		op->stsize = 0 ;
	    } /* end if (populated) */
	} /* end if (non-null) */
	return (rs >= 0) ? op->c : rs ;
}
/* end subroutine (vecstr_del) */

int vecstr_delall(vecstr *op) noex {
	int		rs = SR_FAULT ;
	int		rs1 ;
	if (op) {
	    rs = SR_OK ;
	    if (op->va) {
	        for (int i = 0 ; i < op->i ; i += 1) {
	            if (op->va[i] != nullptr) {
		        rs1 = uc_libfree(op->va[i]) ;
		        if (rs >= 0) rs = rs1 ;
	            }
	        } /* end for */
	        op->i = 0 ;
	        op->fi = 0 ;
	        op->va[0] = nullptr ;
	        op->c = 0 ;
	        op->stsize = 1 ;
	    } /* end if (populated) */
	} /* end if (non-null) */
	return rs ;
}
/* end subroutine (vecstr_delall) */

int vecstr_count(vecstr *op) noex {
	int	rs = SR_FAULT ;
	if (op) {
	    rs = op->c ;
	} /* end if (non-null) */
	return rs ;
}
/* end subroutine (vecstr_count) */

int vecstr_sort(vecstr *op,vecstr_vcmp vcf) noex {
	int		rs = SR_FAULT ;
	int		c = 0 ;
	if (op) {
	    rs = SR_OK ;
	    if (op->va) {
	        if (vcf == nullptr) vcf = vstrcmp ;
	        if ((! op->f.issorted) && (op->c > 1)) {
		    vecstr_arrsort(op,vcf) ;
	        } /* end if (sorting) */
	        op->f.issorted = true ;
	    } /* end if (populated) */
	} /* end if (non-null) */
	return (rs >= 0) ? c : rs ;
}
/* end subroutine (vecstr_sort) */

int vecstr_search(vecstr *op,cchar *sp,vecstr_vcmp vcf,cchar **rpp) noex {
	int		rs  = SR_FAULT ;
	if (op) {
	    rs = SR_NOTFOUND ;
	    if (op->va) {
	        cint	esize = sizeof(char *) ;
		int	i = 0 ;
	        cchar	**spp{} ;
	        if (vcf == nullptr) vcf = vstrcmp ;
	        if (op->f.osorted && (! op->f.issorted)) {
	            op->f.issorted = true ;
	            if (op->c > 1) {
		        vecstr_arrsort(op,vcf) ;
	            }
	        } /* end if (sorting) */
	        if (op->f.issorted) {
	            qsort_f	scf = qsort_f(vcf) ;
	            spp = (cchar **) bsearch(&sp,op->va,op->i,esize,scf) ;
	            rs = SR_NOTFOUND ;
	            if (spp) {
	                i = (spp - op->va) ;
	                rs = i ;
	            }
	        } else {
	            for (i = 0 ; i < op->i ; i += 1) {
	                spp = (op->va + i) ;
	                if (*spp) {
	                    if ((*vcf)(&sp,spp) == 0) break ;
		        }
	            } /* end for */
		    if (i < op->i) rs = i ;
	        } /* end if (sorted or not) */
	        if (rpp) {
	            *rpp = (rs >= 0) ? op->va[i] : nullptr ;
	        }
	    } /* end if (populated) */
	} /* end if (non-null) */
	return rs ;
}
/* end subroutine (vecstr_search) */

int vecstr_searchl(vecstr *op,cchar *sp,int sl,vecstr_vcmp vcf,cc **rpp) noex {
	int		rs = SR_FAULT ;
	int		rs1 ;
	int		i = -1 ;
	if (op && sp) {
	    nulstr	ns ;
	    cchar	*s{} ;
	    if ((rs = ns.start(sp,sl,&s)) >= 0) {
		if ((rs = vecstr_search(op,s,vcf,rpp)) >= 0) {
		    i = rs ;
		} else if (rs == rsn) {
		    rs = SR_OK ;
		}
	        rs1 = ns.finish ;
		if (rs >= 0) rs = rs1 ;
	    } /* end if (nulstr) */
	} /* end if (non-null) */
	if ((rs >= 0) && (i < 0)) {
	    rs = SR_NOTFOUND ; /* real errors priority */
	}
	return (rs >= 0) ? i : rs ;
}
/* end subroutine (vecstr_searchl) */

int vecstr_finder(vecstr *op,cchar *sp,vecstr_vcmp vcf,cchar **rpp) noex {
	int		rs = SR_FAULT ;
	if (op) {
	    rs = SR_NOTFOUND ;
	    if (op->va) {
		int	i ;
		if (vcf == nullptr) vcf = vstrcmp ;
	        for (i = 0 ; i < op->i ; i += 1) {
	            if (op->va[i]) {
	                if ((*vcf)(&sp,(op->va + i)) == 0) break ;
	            }
	        } /* end for */
	        if (i < op->i) rs = i ;
	        if (rpp) {
	            *rpp = (rs >= 0) ? op->va[i] : nullptr ;
	        }
	    } /* end if (populated) */
	} /* end if (non-null) */
	return rs ;
}
/* end subroutine (vecstr_finder) */

int vecstr_find(vecstr *op,cchar *sp) noex {
	int		rs = SR_FAULT ;
	if (op && sp) {
	    rs = SR_NOTFOUND ;
	    if (op->va) {
		cint	sch = sp[0] ; /* ok: since all get promoted similarly */
		int	i ;
	        for (i = 0 ; i < op->i ; i += 1) {
	            cchar	*ep = op->va[i] ;
	            if (ep) {
	                if ((sch == ep[0]) && (strcmp(sp,ep) == 0)) break ;
		    }
	        } /* end for */
	        if (i < op->i) rs = i ;
	    } /* end if (populated) */
	} /* end if (non-null) */
	return rs ;
}
/* end subroutine (vecstr_find) */

int vecstr_findn(vecstr *op,cchar *sp,int sl) noex {
	int		rs = SR_FAULT ;
	if (op && sp) {
	    rs = SR_NOTFOUND ;
	    if (sl < 0) sl = strlen(sp) ;
	    if (op->va) {
	        cint	sch = sp[0] ; /* ok: since all get promoted similarly */
	    	int	i{} ;		/* <- used afterwards */
	        for (i = 0 ; i < op->i ; i += 1) {
	            cchar	*ep = op->va[i] ;
	            if (ep && (sch == ep[0])) {
		        cint	m = nleadstr(ep,sp,sl) ;
		        if ((m == sl) && (ep[m] == '\0')) break ;
		    }
	        } /* end for */
	        if (i < op->i) rs = i ;
	    } /* end if (populated) */
	} /* end if (non-null) */
	return rs ;
}
/* end subroutine (vecstr_findn) */

int vecstr_findaddr(vecstr *op,cchar *addr) noex {
	int		rs = SR_FAULT ;
	if (op) {
	    rs = SR_NOTFOUND ;
	    if (op->va) {
	        for (int i = 0 ; i < op->i ; i += 1) {
	            cchar	*ep = op->va[i] ;
	            if (ep && (addr == ep)) {
		        rs = i ;
		        break ;
	            }
	        } /* end for */
	    } /* end if (populated) */
	} /* end if (non-null) */
	return rs ;
}
/* end subroutine (vecstr_findaddr) */

int vecstr_getvec(vecstr *op,cchar ***rppp) noex {
	int	rs = SR_FAULT ;
	if (op && rppp) {
	    if ((rs = vecstr_extvec(op)) >= 0) {
	        if (op->va) {
		    rs = op->c ;
		    *rppp = op->va ;
		}
	    }
	} /* end if (non-null) */
	return rs ;
}
/* end subroutine (vecstr_getvec) */

int vecstr_strsize(vecstr *op) noex {
	int		rs = SR_FAULT ;
	int		stsize = 1 ;
	if (op) {
	    rs = SR_OK ;
	    if (op->va) {
	        if (! op->f.stsize) {
	            for (int i = 0 ; i < op->i ; i += 1) {
			cchar	*ep = op->va[i] ;
	                if (ep) {
	                    stsize += (strlen(ep) + 1) ;
		        }
	            } /* end for */
	            op->stsize = stsize ;
	            op->f.stsize = true ;
	        } /* end if (calculating size) */
	        stsize = iceil(op->stsize,sizeof(int)) ;
	    } /* end if (populated) */
	} /* end if (non-null) */
	return (rs >= 0) ? stsize : rs ;
}
/* end subroutine (vecstr_strsize) */

int vecstr_strmk(vecstr *op,char *tab,int tabs) noex {
	int		rs = SR_FAULT ;
	int		c = 0 ;
	if (op && tab) {
	    cint 	stsize = iceil(op->stsize,resz) ;
	    rs = SR_OVERFLOW ;
	    if (tabs >= stsize) {
	        char	*bp = tab ;
	        rs = SR_OK ;
	        *bp++ = '\0' ;	/* <- no chunks: do it ourselves */
		c += 1 ;	/* and count it */
	    	if (op->va) {
	            for (int i = 0 ; i < op->i ; i += 1) {
			cchar	*ep = op->va[i] ;
	                if (ep) {
	    	            c += 1 ;
	                    bp = strwcpy(bp,ep,-1) + 1 ;
		        }
	            } /* end for */
	        } /* end if (populated) */
		while (bp < (tab + tabs)) {
	            *bp++ = '\0' ;
	        }
	    } /* end if (supplied table large enough) */
	} /* end if (non-null) */
	return (rs >= 0) ? c : rs ;
}
/* end subroutine (vecstr_strmk) */

int vecstr_recsize(vecstr *op) noex {
	int		rs = SR_FAULT ;
	int		rsize = (2 * resz) ;
	if (op) {
	    rs = SR_OK ;
	    if (op->va) {
	        rsize += (op->c * resz) ;
	    }
	} /* end if (non-null) */
	return (rs >= 0) ? rsize : rs ;
}
/* end subroutine (vecstr_recsize) */

int vecstr_recmk(vecstr *op,int *rec,int recs) noex {
	int		rs = SR_FAULT ;
	int		c = 0 ;
	if (op && rec) {
	    cint	rsize = ((op->c + 2) * resz) ;
	    rs = SR_OVERFLOW ;
	    if (recs >= rsize) {
	        int	si = 0 ;
		rs = SR_OK ;
	        rec[c++] = si ;		/* <- zeroth entry is zero */
	        si += 1 ;
		if (op->va) {
	            for (int i = 0 ; i < op->i ; i += 1) {
			cchar	*ep = op->va[i] ;
	                if (ep) {
	                    rec[c++] = si ;
	                    si += (strlen(ep) + 1) ;
		        }
	            } /* end for */
		} /* end if (populated) */
	        rec[c] = -1 ;			/* <- last record entry */
	    } /* end if (user-supplied size was OK) */
	} /* end if (non-null) */
	return (rs >= 0) ? c : rs ;
}
/* end subroutine (vecstr_recmk) */

int vecstr_recmkstr(vecstr *op,int *rec,int recs,char *tab,int tabs) noex {
	int		rs = SR_FAULT ;
	int		c = 0 ;
	if (op && rec && tab) {
	    rs = SR_OK ;
	    if (op->stsize == 0) {
		rs = vecstr_strsize(op) ;
	    }
	    if (rs >= 0) {
		cint 	stsize = iceil(op->stsize,resz) ;
		rs = SR_OVERFLOW ;
	        if (tabs >= stsize) {
	            cint	rsize = ((op->c + 2) * resz) ;
	            if (recs >= rsize) {
	    		char	*bp = tab ;
	    		rec[c++] = 0 ;		/* <- zeroth entry is zero */
	    		*bp++ = '\0' ;
	                    for (int i = 0 ; i < op->i ; i += 1) {
				cchar	*ep = op->va[i] ;
	                        if (ep) {
	                            rec[c++] = (bp - tab) ;
	                            bp = (strwcpy(bp,ep,-1) + 1) ;
		                }
	                    } /* end for */
	                rec[c] = -1 ;		/* <- last record entry */
	            } /* end if (record-size OK) */
		} /* end if (string-size OK) */
	    } /* end if (OK so far) */
	} /* end if (non-null) */
	return (rs >= 0) ? c : rs ;
}
/* end subroutine (vecstr_recmkstr) */

int vecstr_insert(vecstr *op,int ii,cchar *sp,int sl) noex {
	int		rs = SR_FAULT ;
	int		i = 0 ;
	if (op && sp) {
	     if ((rs = vecstr_validx(op,ii)) >= 0) {
	        if (sl < 0) sl = strlen(sp) ;
	        if ((op->i + 1) > op->n) {
	            rs = vecstr_extvec(op) ;
	        }
	        if (rs >= 0) {
	            char	*bp ;
	            cint	size = (sl+1) ;
	            if ((rs = uc_libmalloc(size,&bp)) >= 0) {
	                strwcpy(bp,sp,sl) ;
	                op->stsize += size ;
	                i = vecstr_insertsp(op,ii,bp) ;
	            }
	        } /* end if (OK) */
	    } /* end if (valid index) */
	} /* end if (non-null) */
	return (rs >= 0) ? i : rs ;
}
/* end subroutine (vecstr_insert) */


/* private subroutines */

static int vecstr_ctor(vecstr *op) noex {
	int		rs = SR_FAULT ;
	if (op) {
	    rs = SR_OK ;
	    op->va = nullptr ;
	    op->c = 0 ;
	    op->i = 0 ;
	    op->n = 0 ;
	    op->fi = 0 ;
	    op->stsize = 0 ;
	} /* end if (non-null) */
	return rs ;
}
/* end subroutine (vecstr_ctor) */

static int vecstr_setopts(vecstr *op,int vo) noex {
	int		rs = SR_INVALID ;
	if ((vo & (~optmask)) == 0) {
	    rs = SR_OK ;
	    op->f = {} ;
	    if (vo & VECSTR_OREUSE) op->f.oreuse = 1 ;
	    if (vo & VECSTR_OSWAP) op->f.oswap = 1 ;
	    if (vo & VECSTR_OSTATIONARY) op->f.ostationary = 1 ;
	    if (vo & VECSTR_OCOMPACT) op->f.ocompact = 1 ;
	    if (vo & VECSTR_OSORTED) op->f.osorted = 1 ;
	    if (vo & VECSTR_OORDERED) op->f.oordered = 1 ;
	    if (vo & VECSTR_OCONSERVE) op->f.oconserve = 1 ;
	} /* end if (valid options) */
	return rs ;
}
/* end subroutine (vecstr_setopts) */

static int vecstr_extvec(vecstr *op) noex {
	int		rs = SR_OK ;
	if ((op->i + 1) > op->n) {
	    int		nn, size ;
	    void	*na{} ;
	    if (op->va == nullptr) {
	        nn = VECSTR_DEFENTS ;
	        size = (nn + 1) * sizeof(char **) ;
	        rs = uc_libmalloc(size,&na) ;
	    } else {
	        nn = (op->n + 1) * 2 ;
	        size = (nn + 1) * sizeof(char **) ;
	        rs = uc_librealloc(op->va,size,&na) ;
	    }
	    if (rs >= 0) {
	        op->va = (cchar **) na ;
		op->va[op->i] = nullptr ;
	        op->n = nn ;
	    }
	} /* end if (extension needed) */
	return rs ;
}
/* end subroutine (vecstr_extvec) */

static int vecstr_addsp(vecstr *op,cchar *sp) noex {
	int		i = 0 ;
	cbool		f = op->f.oreuse || op->f.oconserve ;
	bool		f_done = false ;
	if (f && (op->c < op->i)) {
	    i = op->fi ;
	    while ((i < op->i) && (op->va[i] != nullptr)) {
	        i += 1 ;
	    }
	    op->fi = i + 1 ;
	    if (i < op->i) {
	        op->va[i] = (char *) sp ;
	        f_done = true ;
	    }
	} /* end if (reuse or conserve) */
	if (! f_done) {
	    i = op->i ;
	    op->va[op->i++] = sp ;
	    op->va[op->i] = nullptr ;
	}
	op->c += 1 ;
	op->f.issorted = false ;
	return i ;
}
/* end subroutine (vecstr_addsp) */

static int vecstr_insertsp(vecstr *op,int ii,cchar *sp) noex {
	if (ii == op->i) {
	    op->i += 1 ;
	    op->va[op->i] = nullptr ;
	} else if (op->va[ii] != nullptr) {
	    int		i ;
	    for (i = (ii + 1) ; i < op->i ; i += 1) {
		if (op->va[i] == nullptr) break ;
	    }
	    if (i == op->i) {
	        op->i += 1 ;
	        op->va[op->i] = nullptr ;
	    }
	    for (int j = i ; j > ii ; j -= 1) {
		op->va[j] = op->va[j-1] ;
	    }
	} /* end if */
	op->va[ii] = sp ;
	op->c += 1 ;
	op->f.issorted = false ;
	return ii ;
}
/* end subroutine (vecstr_insertsp) */

static int vecstr_validx(vecstr *op,int i) noex {
	return ((i >= 0) && (i < op->i)) ? SR_OK : SR_NOTFOUND ;
}
/* end subroutine (vecstr_validx) */

static void vecstr_arrsort(vecstr *op,vecstr_vcmp vcf) noex {
	qsort_f		scf = qsort_f(vcf) ;
	if constexpr (f_qsort) {
	    cint	esize = sizeof(char *) ;
	    qsort(op->va,op->i,esize,scf) ;
	} else {
	    cint	i = op->i ;
	    cchar	**va = op->va ;
	    sort(va,(va+i),scf) ;
	}
}
/* end subroutine (vecstr_arrsort) */

int vecstr::start(int n,int o) noex {
	return vecstr_start(this,n,o) ;
}

int vecstr::add(cchar *sp,int sl) noex {
	return vecstr_add(this,sp,sl) ;
}

int vecstr::adduniq(cchar *sp,int sl) noex {
	return vecstr_adduniq(this,sp,sl) ;
}

int vecstr::insert(int i,cchar *sp,int sl) noex {
	return vecstr_insert(this,i,sp,sl) ;
}

int vecstr::del(int i) noex {
	if (i < 0) i = 0 ;
	return vecstr_del(this,i) ;
}

void vecstr::dtor() noex {
	cint	rs = int(finish) ;
	if (rs < 0) {
	    ulogerror("vecstr",rs,"fini-finish") ;
	}
}

vecstr_co::operator int () noex {
	int	rs = SR_BUGCHECK ;
	switch (w) {
	case vecstrmem_count:
	    rs = vecstr_count(op) ;
	    break ;
	case vecstrmem_delall:
	    rs = vecstr_delall(op) ;
	    break ;
	case vecstrmem_strsize:
	    rs = vecstr_strsize(op) ;
	    break ;
	case vecstrmem_recsize:
	    rs = vecstr_recsize(op) ;
	    break ;
	case vecstrmem_audit:
	    rs = vecstr_audit(op) ;
	    break ;
	case vecstrmem_finish:
	    rs = vecstr_finish(op) ;
	    break ;
	} /* end switch */
	return rs ;
}
/* end method (vecstr_co::operator) */

bool vecstr_iter::operator == (const vecstr_iter &oit) noex {
	return (va == oit.va) && (i == oit.i) && (ii == oit.ii) ;
}

bool vecstr_iter::operator != (const vecstr_iter &oit) noex {
	bool		f = false ;
	f = f || (va != oit.va) ;
	f = f || (ii != oit.ii) ;
	if (!f) {
	    f = (i < oit.i) ;
	}
	return f ;
}
/* end method (vecstr_iter::operator) */

vecstr_iter vecstr_iter::operator + (int n) const noex {
	vecstr_iter	rit(va,i,i) ;
	rit.i = ((rit.i + n) >= 0) ? (rit.i + n) : 0 ;
	return rit ;
}

vecstr_iter vecstr_iter::operator += (int n) noex {
	vecstr_iter	rit(va,i,i) ;
	i = ((i + n) >= 0) ? (i + n) : 0 ;
	rit.i = i ;
	return rit ;
}

vecstr_iter vecstr_iter::operator ++ () noex { /* pre */
	increment() ;
	return (*this) ;
}

vecstr_iter vecstr_iter::operator ++ (int) noex { /* post */
	vecstr_iter	pre(*this) ;
	increment() ;
	return pre ;
}

void vecstr_iter::increment(int n) noex {
	if ((i + n) < 0) n = -i ;
	if (n != 0) {
	    i += n ;
	    while ((i < ii) && (va[i] == nullptr)) {
	        i += 1 ;
	    }
	}
}
/* end method (vecstr_iter::increment) */


