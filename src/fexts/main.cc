/* main SUPPORT (fexts) */
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
#include	<algorithm>		/* |ranges(3c++)| */
#include	<iostream>
#include	<vector>
#include	<filesystem>
#include	<usystem.h>
#include	<sfx.h>


/* local defines */


/* imported namespaces */

using namespace	std::filesystem ;	/* namespace */
namespace	rg = std::ranges ;	/* namespace */

using std::vector ;			/* type */
using std::cerr ;			/* variable */
using std::cout ;			/* variable */


/* local typedefs */


/* forward references */


/* local constants */


/* exported variables */


/* exported subroutines */

int main(int argc,mainv argv,mainv) {
	int	ex = EXIT_FAILURE ;
	if (argc > 1) {
	    directory_iterator	dir(".") ;
	    vector<cchar *>	exts ;
	    int			c = 0 ;
	    for (int i = 1 ; (i < argc) && argv[i] ; i += 1) {
		exts.push_back(argv[i]) ;
		c += 1 ;
	    } /* end for */
	    if (c > 0) {
		auto ite = exts.end() ;
	        for (auto const &e : dir) {
		    path	bn ;
		    cchar	*bns ;
		    if (e.is_regular_file()) {
		        const path	p = e.path() ;
		        cchar		*ep ;
		        bn = p.filename() ;
		        bns = bn.c_str() ;
		        if (int el ; (el = sfrchr(bns,-1,'.',&ep)) > 0) {
			    auto fif = rg::find_if ;
	    		    auto strmat = [ep] (cc *s) noex -> bool {
				return (strcmp(s,ep) == 0) ;
	    		    } ;
			    if (auto it = fif(exts,strmat) ; it != ite) {
				cout << bns << '\n' ;
			    }
		        } /* end if (had-extension) */
		    } /* end if (regular-file) */
	        } /* end for */
	        ex = EXIT_SUCCESS ;
	    } /* end if */
	}
	return ex ;
}
/* end subroutine (main) */


