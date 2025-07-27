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
#include	<cstring>		/* |strchr(3c)| */
#include	<new>			/* |nothrow(3c++)| */
#include	<string_view>
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>
#include	<ulogerror.h>
#include	<sfx.h>
#include	<strn.h>
#include	<strnul.hh>
#include	<mkchar.h>
#include	<ischarx.h>
#include	<localmisc.h>

#pragma		GCC dependency	"mod/libutil.ccm"
#pragma		GCC dependency	"mod/debug.ccm"

module argmgr ;

import libutil ;
import debug ;

/* local defines */

#define	ARGMGR_MAGIC		0x97873787


/* imported namespaces */

using std::nullptr_t ;			/* type */
using std::string_view ;		/* type */
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
	if (argv) ylikely {
	    rs = SR_INVALID ;
	    if (argc >= 0) ylikely {
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
	if ((rs = argmgr_magic(this)) >= 0) ylikely {
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
	if ((rs = argmgr_magic(this)) >= 0) ylikely {
	    rs = int(++ai < argc) ;
	} /* end if (magic) */
	return rs ;
} /* end method (argmgr::iarg) */

int argmgr::argopt(cchar **rpp) noex {
    	int		rs ;
	if ((rs = argmgr_magic(this)) >= 0) ylikely {
	    if_constexpr (f_debug) {
	        debprintf(__func__,"ent ai=%d c=%d\n",ai,cntpos) ;
	    }
	    if (ai < argc) ylikely {
	        cchar	*ap = argv[ai] ;
	        if (isplusminus(ap[0])) {
		    fl.plus = (ap[0] == '+') ;
		    if (cint ch = mkchar(ap[1]) ; ch) {
		        rs = iargopt(ap,ch,rpp) ;
		    } /* end if (have option) */
	        }
	        if (rs > 0) ylikely {
		    if_constexpr (f_debug) {
		        strnul sk((ap+1),rs) ;
		        debprintf(__func__,"kp=>%s<\n",ccp(sk)) ;
		    }
		    if (valp) {
		        cint vall= lenstr(valp) ;
			if_constexpr (f_debug) {
		            strnul sv (valp,vall) ;
		            debprintf(__func__,"vp=>%s<\n",ccp(sv)) ;
			}
		    }
	        } /* end if (non-zero positive) */
		if_constexpr (f_debug) {
	            debprintf(__func__,"ret rs=%d ai=%d c=%d\n",rs,ai,cntpos) ;
		}
	    } /* end if (valid) */
	} /* end if (magic) */
	return rs ;
} /* end method (argmgr::argopt) */

int argmgr::iargopt(cchar *ap,int ch,cchar **rpp) noex {
    	cnullptr	np{} ;
    	int		rs = SR_OK ;
	if (isalphalatin(ch)) ylikely {
	    if ((rs = amap.set[ai]) >= 0) {
		if (rpp) *rpp = (ap + 1) ;
		cntpos -= 1 ;
		if (cc *tp ; (tp = strchr((ap + 1),'=')) != np) {
		    valp = (tp + 1) ;
		    rs = intconv(tp - (ap + 1)) ;
		} else {
		    valp = nullptr ;
		    rs = lenstr(ap + 1) ;
		}
	    } /* end if (amap,set) */
	} else if (isdigitlatin(ch)) {
	    digvalp = (ap + 1) ;
	} /* end if (isalphalatin) */
	return rs ;
} /* end method (argmgr::iargopt) */

int argmgr::argoptlong(cchar **rpp) noex {
    	int		rs ;
	if ((rs = argmgr_magic(this)) >= 0) ylikely {
	    if (ai < argc) ylikely {
	        cchar	*ap = argv[ai] ;
	        if ((ap[0] == '-') && (ap[1] == '-')) {
		    if (cint ch = mkchar(ap[2]) ; ch) {
		        if (isalphalatin(ch)) ylikely {
			    if ((rs = amap.set[ai]) >= 0) {
			        cntpos -= 1 ;
		                if (rpp) *rpp = (ap + 2) ;
		                rs = lenstr(ap + 2) ;
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
	        } /* end if (ch) */
	    } /* end if (valid) */
	} /* end if (magic) */
	return rs ;
} /* end method (argmgr::argoptlong) */

int argmgr::argval(cchar **rpp) noex {
    	int		rs ;
	int		al = 0 ; /* return-value */
	if_constexpr (f_debug) {
	    debprintf(__func__,"ent\n") ;
	}
	if ((rs =  argmgr_magic(this)) >= 0) ylikely {
    	    rs = SR_INVALID ;
	    if (ai < argc) ylikely {
	        cchar *rp = nullptr ;
	        if (valp) {
		    rs = SR_OK ;
		    rp = valp ;
		    al = lenstr(valp) ;
		    valp = nullptr ;
	        } else {
	            if ((rs = get((ai + 1),&rp)) > 0) {
		        ai = rs ;
		        if (rp) {
			    if ((rs = amap.set[ai]) >= 0) {
			        cntpos -= 1 ;
		                al = lenstr(rp) ;
			    }
		        } else {
			    rs = SR_INVALID ;
		        }
		    } else if (rs == 0) {
		        rs = SR_INVALID ;
		    }
	        } /* end if */
	        if (rpp) *rpp = rp ;
	    } /* end if (within possible range) */
	} /* end if (magic) */
	if_constexpr (f_debug) {
	    debprintf(__func__,"rs=%d al=%d\n",rs,al) ;
	}
	return (rs >= 0) ? al : rs ;
} /* end method (argmgr::argval) */

int argmgr::get(int i,ccharpp rpp) noex {
    	int		rs = SR_OK ;
	bool		f = false ;
	if_constexpr (f_debug) {
	    debprintf(__func__,"ent i=%d\n",i) ;
	}
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
	        if_constexpr (f_debug) {
		    debprintf(__func__,"reg i=%d\n",i) ;
	        }
		if ((rs = amap.tst[i]) > 0) {
		    i += 1 ;
		} else if (rs == 0) {
		    f = true ;
		}
	        if_constexpr (f_debug) {
	            debprintf(__func__,"reg rs=%d\n",rs) ;
		}
	    } /* end if */
	} /* end while */
	if (rs >= 0) ylikely {
	    if (rpp) *rpp = (f) ? argv[i] : nullptr ;
	}
	if_constexpr (f_debug) {
	    debprintf(__func__,"rs=%d i=%d f=%u\n",rs,i,f) ;
	}
	return (rs >= 0) ? i : rs ;
} /* end method (argmgr::get) */

int argmgr::present(int i) noex {
    	int		rs = SR_OK ;
	int		f = false ; /* return-value */
	if ((rs =  argmgr_magic(this)) >= 0) ylikely {
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
	    } /* end if (valid) */
	} /* end if (magic) */
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
    	int		rs ;
	if ((rs = argmgr_magic(this)) >= 0) ylikely {
    	    rs = cntpos ;
	} /* end if (magic) */
    	return rs ;
} /* end method (argmgr::icount) */

void argmgr::dtor() noex {
	if (cint rs = finish ; rs < 0) {
	    ulogerror("argmgr",rs,"fini-finish") ;
	}
}

argmgr::operator int () noex {
    	int		rs ;
	if ((rs = argmgr_magic(this)) >= 0) ylikely {
	    rs = argc ;
	}
	return rs ;
} /* end method (argmgr::operator) */

argmgr_co::operator int () noex {
	int		rs = SR_BUGCHECK ;
	if (op) ylikely {
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


