/* fexts_main SUPPORT (fexts) */
/* charset=ISO8859-1 */
/* lang=C++23 */

/* print out matching filenames */
/* version %I% last-modified %G% */


/* revision history:

	= 2001-04-11, David A­D­ Morano
	Originally written for Rightcore Network Services.

*/

/* Copyright © 2001 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Name:
	fexts

	Description:
	This program prints out all files in the current directory
	that have file-name extensions that match the strings given
	as program arguments.

	Synopsis:
	$ fexts [strings]

	Arguments:
	strings		filename extensions to match

	Returns:
	EXIT_SUCCESS	OK
	EXIT_FAILURE	some problem

*******************************************************************************/

#include	<envstandards.h>	/* MUST be ordered first to configure */
#include	<cstdlib>		/* |EXIT_FAILURE| */
#include	<exception>
#include	<new>
#include	<algorithm>		/* |ranges(3c++)| + |strrchr(3c)| */
#include	<iostream>
#include	<vector>
#include	<filesystem>
#include	<clanguage.h>
#include	<usysbase.h>
#include	<localmisc.h>


/* local defines */


/* imported namespaces */

using namespace	std::filesystem ;	/* namespace */
namespace	rg = std::ranges ;	/* namespace */

using std::vector ;			/* type */
using std::min ;			/* subroutine-template */
using std::max ;			/* subroutine-template */
using std::cerr ;			/* variable */
using std::cout ;			/* variable */


/* local typedefs */

typedef directory_iterator	dirit ;
typedef	vector<cchar *>		veccp ;


/* external subroutines */


/* external variables */


/* forward references */

static int iterate(dirit &,veccp &) ;


/* local constants */


/* exported variables */


/* exported subroutines */

int main(int argc,mainv argv,mainv) {
	int		ex = EXIT_FAILURE ;
	int		rs = SR_OK ;
	if (argc > 1) {
	    dirit	dir(".") ;
	    veccp	exts ;
	    int		c = 0 ;
	    for (int i = 1 ; (i < argc) && argv[i] ; i += 1) {
		if (cchar *ep = argv[i] ; ep[0]) {
		    exts.push_back(ep) ;
		    c += 1 ;
		}
	    } /* end for */
	    if (c > 0) {
		if ((rs = iterate(dir,exts)) >= 0) {
	            ex = EXIT_SUCCESS ;
		}
	    } /* end if */
	}
	return ex ;
}
/* end subroutine (main) */


/* local subroutines */

static int iterate(dirit &dir,veccp &exts) {
	cnullptr	np{} ;
	cauto		ite = exts.end() ;
	int		rs = SR_OK ;
	int		c = 0 ;
        for (cauto &e : dir) {
            path        bn ;
            cchar       *bns ;
            if (e.is_regular_file()) {
                const path      &p = e.path() ;
                bn = p.filename() ;
                bns = bn.c_str() ;
                if (cchar *tp ; (tp = strrchr(bns,'.')) != np) {
                    cchar       *ep = (tp + 1) ;
                    if (ep[0]) {
                        cauto fif = rg::find_if ;
                        cauto strmat = [ep] (cc *s) noex -> bool {
                            return (strcmp(s,ep) == 0) ;
                        } ;
                        if (cauto &it = fif(exts,strmat) ; it != ite) {
			    c += 1 ;
                            cout << bns << '\n' ;
                        }
                    } /* end if (non-empty) */
                } /* end if (had-extension) */
            } /* end if (regular-file) */
        } /* end for */
	return (rs >= 0) ? c : rs ;
}
/* end subroutine (iterate) */


