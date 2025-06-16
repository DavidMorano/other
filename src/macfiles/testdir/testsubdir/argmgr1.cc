/* argmgr1 MODULE */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* manage program arguments */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-09-10, David A­D­ Morano
	This subroutine was written for Rightcore Network Services.

	= 2020-08-13, David A­D­ Morano
	This code has been modularized (w/ C++20 modules).

*/

/* Copyright © 1998,2020 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

  	Object:
	argmgr

	Description:
	This object manages program arguments.

*******************************************************************************/

module ;

#include	<envstandards.h>	/* MUST be first to configure */
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<cstdio>
#include	<cstring>		/* |strchr(3c)| */
#include	<new>			/* |nothrow(3c++)| */
#include	<string_view>
#include	<iostream>
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>
#include	<ulogerror.h>
#include	<sfx.h>
#include	<strn.h>
#include	<mkchar.h>
#include	<ischarx.h>
#include	<localmisc.h>

module argmgr ;

import libutil ;

/* local defines */

#define	ARGMGR_MAGIC		0x97873787


/* imported namespaces */

using std::nullptr_t ;			/* type */
using std::string_view ;		/* type */
using std::cerr ;			/* variable */
using std::nothrow ;			/* constant */


/* local typedefs */

typedef string_view		strview ;


/* external subroutines */


/* local structures */


/* forward references */


/* forward refernces */

template<typename ... Args>
static inline int argmgr_magic(argmgr *op,Args ... args) noex {
	int		rs = SR_FAULT ;
	if (op && (args && ...)) {
	    rs = (op->magic == ARGMGR_MAGIC) ? SR_OK : SR_NOTOPEN ;
	}
	return rs ;
}
/* end subroutine (argmgr_magic) */


/* local variables */


/* exported variables */


/* exported subroutines */


/* local subroutines */

int argmgr::istart() noex {
    	int		rs = SR_FAULT ;
	if (argv) {
	    rs = SR_INVALID ;
	    if (argc >= 0) {
	        if ((rs = amap.start) >= 0) {
	            magic = ARGMGR_MAGIC ;
	        }
	    } /* end if (valid) */
	} /* end if (non-null) */
	return rs ;
} /* end method (argmgr::istart) */

int argmgr::ifinish() noex {
    	int		rs ;
	int		rs1 ;
	if ((rs = argmgr_magic(this)) >= 0) {
	    {
	        rs1 = amap.finish ;
	        if (rs >= 0) rs = rs1 ;
	    }
	    {
		argv = nullptr ;
		argc = 0 ;
	    }
	    magic = 0 ;
	} /* end if (magic) */
	return rs ;
} /* end method (argmgr::ifinish) */

int argmgr::iarg() noex {
    	int		rs ;
	if ((rs = argmgr_magic(this)) >= 0) {
	    rs = int(++ai < argc) ;
	} /* end if (magic) */
	return rs ;
} /* end method (argmgr::iarg) */

int argmgr::argopt(cchar **rpp) noex {
    	cnullptr	np{} ;
    	int		rs = SR_OK ;
	fprintf(stderr,"argopt: ent ai=%d c=%d\n",ai,cntpos) ;
	if (ai < argc) {
	    cchar	*ap = argv[ai] ;
	    if (isplusminus(ap[0])) {
		fl.plus = (ap[0] == '+') ;
		if (cint ch = mkchar(ap[1]) ; ch) {
		    if (isalphalatin(ch)) {
			if ((rs = amap.set[ai]) >= 0) {
		            if (rpp) *rpp = (ap + 1) ;
			    cntpos -= 1 ;
			    if (cc *tp ; (tp = strchr((ap + 1),'=')) != np) {
				valp = (tp + 1) ;
				rs = intconv(tp - (ap + 1)) ;
			    } else {
				valp = nullptr ;
		                rs = xstrlen(ap + 1) ;
			    }
			} /* end if (amap,set) */
		    }
		} /* end if (have option) */
	    }
	}
	{
	    if (rs >= 0) {
		strview sk(*rpp,rs) ;
		fprintf(stderr,"argopt: kp=>") ; cerr << sk << "<" << eol ;
		if (valp) {
		    cint vall= xstrlen(valp) ;
		    strview sv (valp,vall) ;
		    fprintf(stderr,"argopt: vp=>") ; cerr << sv << "<" << eol ;
		}
	    }
	    fprintf(stderr,"argopt: ret rs=%d ai=%d c=%d\n",rs,ai,cntpos) ;
	}
	return rs ;
} /* end method (argmgr::argopt) */

int argmgr::argoptlong(cchar **rpp) noex {
    	int		rs = SR_OK ;
	if (ai < argc) {
	    cchar	*ap = argv[ai] ;
	    if ((ap[0] == '-') && (ap[1] == '-')) {
		if (cint ch = mkchar(ap[2]) ; ch) {
		    if (isalphalatin(ch)) {
			if ((rs = amap.set[ai]) >= 0) {
			    cntpos -= 1 ;
		            if (rpp) *rpp = (ap + 2) ;
		            rs = xstrlen(ap + 2) ;
			}
		    } else {
		        argoptdone = true ;
		        aie = ai ;
		    }
		} else {
		        argoptdone = true ;
		        aie = ai ;
			cntpos -= 1 ;
		}
	    }
	}
	return rs ;
} /* end method (argmgr::argoptlong) */

int argmgr::argval(cchar **rpp) noex {
    	int		rs = SR_OK ;
	fprintf(stderr,"argval: ent\n") ;
	(void) rpp ;
	if (ai < argc) {
	    cchar *rp = nullptr ;
	    if (valp) {
		rp = valp ;
		rs = xstrlen(valp) ;
		valp = nullptr ;
	    } else {
	        rs = SR_OK ;
	    }
	    if (rpp) *rpp = rp ;
	}
	fprintf(stderr,"argval: ret rs=%d\n",rs) ;
	return rs ;
} /* end method (argmgr::argval) */

int argmgr::get(int i,ccharpp rpp) noex {
    	int		rs = SR_OK ;
	bool		f = false ;
	fprintf(stderr,"get: ent i=%d\n",i) ;
	while ((rs >= 0) && (i < argc) && (! f)) {
	    if (aie > 0) {
		if (i < aie) {
		    if ((rs = amap.tst[i]) > 0) {
		        i += 1 ;
		    } else if (rs == 0) {
		        f = true ;
		    }
		} else if (i > aie) {
		    f = true ;
		}
	    } else {
	fprintf(stderr,"get: reg i=%d\n",i) ;
		if ((rs = amap.tst[i]) > 0) {
		    i += 1 ;
		} else if (rs == 0) {
		    f = true ;
		}
	fprintf(stderr,"get: reg rs=%d\n",rs) ;
	    } /* end if */
	} /* end while */
	if (rs >= 0) {
	    if (rpp) *rpp = (f) ? argv[i] : nullptr ;
	}
	fprintf(stderr,"get: rs=%d i=%d f=%u\n",rs,i,f) ;
	return (rs >= 0) ? i : rs ;
} /* end method (argmgr::get) */

int argmgr::present(int i) noex {
    	int		rs = SR_OK ;
	int		f = false ; /* return-value */
	if ((i > 0) && (i < argc)) {
	    if (aie > 0) {
		if (i < aie) {
		    if ((rs = amap.tst[i]) == 0) {
		        f = true ;
		    }
		} else if (i > aie) {
		    f = true ;
		}
	    } else {
		    if ((rs = amap.tst[i]) == 0) {
		        f = true ;
		    }
	    }
	}
	return (rs >= 0) ? f : rs ;
} /* end method (argmgr::present) */

int argmgr::iargchar() noex {
    	int		rs = SR_OK ;
	return rs ;
}

int argmgr::ipositional() noex {
    	int		rs = SR_OK ;
	return rs ;
}

int argmgr::icount() noex {
    	return cntpos ;
} /* end method (argmgr::icount) */

argmgr_iter argmgr::begin() noex {
    	argmgr_iter	res(this,0) ;
	cchar		*ap = nullptr ;
	if (cint rs = get(1,&ap) ; rs > 0) {
	    {
		cchar *fmt = "begin: rs=%d ap=%s\n" ;
	        fprintf(stderr,fmt,rs,((ap) ? "ok" : "null")) ;
	    }
	    if (ap) res.ai = rs ;
	}
	return res ;
}

argmgr_iter argmgr::end() noex {
    	argmgr_iter	res(this,argc) ;
	return res ;
}

void argmgr::dtor() noex {
	if (cint rs = finish ; rs < 0) {
	    ulogerror("argmgr",rs,"fini-finish") ;
	}
}

argmgr::operator int () noex {
    	int		rs ;
	if ((rs = argmgr_magic(this)) >= 0) {
	    rs = argc ;
	}
	return rs ;
} /* end method (argmgr::operator) */

argmgr_co::operator int () noex {
	int		rs = SR_BUGCHECK ;
	if (op) {
	    switch (w) {
	    case argmgrmem_start:
	        rs = op->istart() ;
	        break ;
	    case argmgrmem_finish:
	        rs = op->ifinish() ;
	        break ;
	    case argmgrmem_arg:
	        rs = op->iarg() ;
	        break ;
	    case argmgrmem_argchar:
		rs = op->iargchar() ;
		break ;
	    case argmgrmem_posarg:
	    case argmgrmem_positional:
		rs = op->ipositional() ;
		break ;
	    case argmgrmem_count:
	        rs = op->icount() ;
	        break ;
	    } /* end switch */
	} /* end if (non-null) */
	return rs ;
} /* end method (argmgr_co) */


