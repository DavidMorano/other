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
#include	<string_view>
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>
#include	<usyscalls.h>		/* |msleep(3u)| */
#include	<ulogerror.h>
#include	<sfx.h>
#include	<strn.h>
#include	<strnul.hh>
#include	<mkchar.h>
#include	<ischarx.h>
#include	<localmisc.h>

#include	"argmgr.h"

#pragma		GCC dependency	"mod/libutil.ccm"
#pragma		GCC dependency	"mod/debug.ccm"

module argmgr ;

import libutil ;
import debug ;

/* local defines */


/* imported namespaces */

using std::string_view ;		/* type */


/* local typedefs */

typedef string_view		strview ;


/* external subroutines */


/* external variables */


/* local structures */


/* forward references */


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
	            magval = argmgr_magicval ;
	        }
	    } /* end if (valid) */
	} /* end if (non-null) */
	return rs ;
} /* end method (argmgr::istart) */

int argmgr::ifinish() noex {
    	int		rs ;
	int		rs1 ;
	if ((rs = magic) >= 0) ylikely {
	    {
	        rs1 = amap.finish ;
	        if (rs >= 0) rs = rs1 ;
	    }
	    {
		argv = nullptr ;
		argc = 0 ;
	    }
	    magval = 0 ;
	} /* end if (magic) */
	return rs ;
} /* end method (argmgr::ifinish) */

int argmgr::iarg() noex {
    	int		rs ;
	if ((rs = magic) >= 0) ylikely {
	    rs = int(++ai < argc) ;
	} /* end if (magic) */
	return rs ;
} /* end method (argmgr::iarg) */

int argmgr::argopt(cchar **rpp) noex {
    	int		rs ;
	int		klen = 0 ; /* return-value */
	DEBPR("ent ai=%d c=%d\n",ai,cntpos) ;
	if ((rs = magic) >= 0) ylikely {
	    DEBPR("ai=%d c=%d\n",ai,cntpos) ;
	    if (ai < argc) ylikely { /* valid argument range */
	        cchar	*ap = argv[ai] ;
	        DEBPR("arg=>%s<\n",ap) ;
	        if (isplusminus(ap[0])) {
		    fl.plus = (ap[0] == '+') ;
		    if (cint ch = mkchar(ap[1]) ; ch) {
		        rs = iargopt(ap,ch,rpp) ;
			klen = rs ;
		    } /* end if (have option) */
	        } /* end if (isplusminus) */ 
		/**** debug block begin */
		if_constexpr (f_debug) {
	            if (rs >= 0) ylikely {
		        strnul sk((ap+1),rs) ; /* string-key */
		        DEBPR("kp=>%s<\n",ccp(sk)) ;
		        if (valp) {
		            cint vall = lenstr(valp) ;
			    if_constexpr (f_debug) {
		                strnul sv(valp,vall) ; /* string-val */
		                DEBPR("vp=>%s<\n",ccp(sv)) ;
			    }
		        } /* end if (had non-null 'valp') */
		    } /* end if (ok) */
		} /* end if_constexpr (f_debug) */
		/**** debug block end */
	        DEBPR("done rs=%d ai=%d c=%d\n",rs,ai,cntpos) ;
	    } /* end if (valid argument range) */
	} /* end if (magic) */
	DEBPR("ret rs=%d klen=%d\n",rs,klen) ;
	return (rs >= 0) ? klen : rs ;
} /* end method (argmgr::argopt) */

int argmgr::iargopt(cchar *ap,int ch,cchar **kpp) noex {
    	cnullptr	np{} ;
    	int		rs = SR_OK ;
	int		klen = 0 ; /* return-value ("key-length"?) */
	DEBPR("ent ai=%d c=%d\n",ai,cntpos) ;
	if (isalphalatin(ch)) ylikely {
	    DEBPR("latin ch=%02X\n",ch) ;
	    if ((rs = amap.set[ai]) >= 0) { /* "set" means not pos-arg */
		if (kpp) *kpp = (ap + 1) ;
		cntpos -= 1 ;
		if (cc *tp ; (tp = strchr((ap + 1),'=')) != np) {
		    valp = (tp + 1) ;
		    fargoptval = true ; /* public flag */
		    klen = intconv(tp - (ap + 1)) ;
		} else {
		    valp = nullptr ;
		    fargoptval = false ; /* public flag */
		    klen = lenstr(ap + 1) ;
		} /* end if */
	    } /* end if (amap.set) */
	} else if (isdigitlatin(ch)) {
	    digvalp = (ap + 1) ;
	} /* end if (isalphalatin) */
	DEBPR("ret rs=%d\n",rs) ;
	return (rs >= 0) ? klen : rs ;
} /* end method (argmgr::iargopt) */

int argmgr::argoptlong(cchar **kpp) noex {
    	int		rs ;
	int		klen = 0 ; /* return-value ("key-length") */
	if ((rs = magic) >= 0) ylikely {
	    if (ai < argc) ylikely { /* valid argumen range */
	        cchar	*ap = argv[ai] ;
	        if ((ap[0] == '-') && (ap[1] == '-')) {
		    if (cint ch = mkchar(ap[2]) ; ch) {
		        if (isalphalatin(ch)) ylikely {
			    if ((rs = amap.set[ai]) >= 0) {
			        cntpos -= 1 ;
		                if (kpp) *kpp = (ap + 2) ;
		                klen = lenstr(ap + 2) ;
			    }
		        } else {
		            fargoptdone = true ; /* public flag */
		            aie = ai ;
		        }
		    } else {
		            fargoptdone = true ; /* public flag */
		            aie = ai ;
			    cntpos -= 1 ;
		    }
	        } /* end if (long-opt) */
	    } /* end if (valid argument range) */
	} /* end if (magic) */
	return (rs >= 0) ? klen : rs ;
} /* end method (argmgr::argoptlong) */

/* this gets the next value, whether the next arg or an arg-opt-val */
int argmgr::argval(cchar **vpp) noex {
    	int		rs ;
	int		vlen = 0 ; /* return-value ("val-length") */
	DEBPR("ent\n") ;
	if ((rs =  magic) >= 0) ylikely {
    	    rs = SR_INVALID ;
	    if (ai < argc) ylikely { /* valid arguement range */
	        cchar *rp = nullptr ;
	        if (valp) {
		    rs = SR_OK ;
		    rp = valp ;
		    vlen = lenstr(valp) ;
		    valp = nullptr ;
		    fargoptval = false ; /* public flag */
	        } else {
	            if ((rs = get((ai + 1),&rp)) > 0) {
		        ai = rs ;
		        if (rp) {
			    if ((rs = amap.set[ai]) >= 0) {
			        cntpos -= 1 ;
		                vlen = lenstr(rp) ;
			    }
		        } else {
			    rs = SR_INVALID ;
		        }
		    } else if (rs == 0) {
		        rs = SR_INVALID ;
		    }
	        } /* end if (existing or new value) */
	        if (vpp) *vpp = rp ;
	    } /* end if (within possible range) */
	} /* end if (magic) */
	DEBPR("ret rs=%d vlen=%d\n",rs,vlen) ;
	return (rs >= 0) ? vlen : rs ;
} /* end method (argmgr::argval) */

int argmgr::get(int ri,ccharpp rpp) noex {
    	int		rs = SR_OK ;
	bool		f = false ;
	DEBPR("ent ri=%d aie=%d\n",ri,aie) ;
	while ((rs >= 0) && (ri < argc) && (! f)) {
	    DEBPR("loop-top ri=%d\n",ri) ;
	    if (aie > 0) {
	        DEBPR("arg-end-case ri=%d\n",ri) ;
		if (ri < aie) {
	            DEBPR("request-before ri=%d\n",ri) ;
		    if ((rs = amap.tst[ri]) > 0) {
		        ri += 1 ;
		    } else if (rs == 0) {
		        f = true ;
		    }
		} else if (ri == aie) {
	            DEBPR("request-deadon ri=%d\n",ri) ;
		    ri += 1 ;
		} else if (ri > aie) {
	            DEBPR("request-after ri=%d\n",ri) ;
		    f = true ; /* <- automatically assumed pos-arg */
		} /* end if (type of request) */
	    } else {
		DEBPR("reg-case ri=%d\n",ri) ;
		if ((rs = amap.tst[ri]) > 0) {
		    ri += 1 ;
		} else if (rs == 0) {
		    f = true ;
		}
	        DEBPR("reg-case rs=%d\n",rs) ;
	    } /* end if (ehether "arg-end" present) */
	} /* end while */
	if (rs >= 0) ylikely {
	    if (rpp) {
		if (f) {
		    *rpp = argv[ri] ;
		} else {
		    *rpp = nullptr ;
		    ri = 0 ;
		}
	    } /* end if (result disposition) */
	} /* end if (finish-up) */
	DEBPR("ret rs=%d ri=%d f=%u\n",rs,ri,f) ;
	return (rs >= 0) ? ri : rs ;
} /* end method (argmgr::get) */

int argmgr::present(int i) noex {
    	int		rs = SR_OK ;
	int		f = false ; /* return-value */
	if ((rs = magic) >= 0) ylikely {
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
	        } /* end if */
	    } /* end if (valid) */
	} /* end if (magic) */
	return (rs >= 0) ? f : rs ;
} /* end method (argmgr::present) */

int argmgr::icount() noex {
    	int		rs ;
	if ((rs = magic) >= 0) ylikely {
    	    rs = cntpos ;
	} /* end if (magic) */
    	return rs ;
} /* end method (argmgr::icount) */

void argmgr::dtor() noex {
	if (cint rs = finish ; rs < 0) {
	    ulogerror("argmgr",rs,"fini-finish") ;
	}
} /* end method (argmgr::dtor) */

argmgr::operator int () noex {
    	int		rs ;
	if ((rs = magic) >= 0) ylikely {
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
	    case argmgrmem_count:
	        rs = op->icount() ;
	        break ;
	    } /* end switch */
	} /* end if (non-null) */
	return rs ;
} /* end method (argmgr_co) */


