/* machello_main SUPPORT (machello) */
/* charset=ISO8859-1 */
/* lang=C++11 */

/* Hello-World */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-04-13, David A-D- Morano
	Originally written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */
/* Use is subject to license terms. */

#include	<envstandards.h>	/* ordered first to configure */
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<iostream>
#include	<iomanip>
#include	<clanguage.h>
#include	<usysbase.h>
#include	<prognamevar.hh>
#include	<localmisc.h>


using namespace std ;

/* ARGSUSED */
int main(int argc,mainv argv,mainv envv) {
    	prognamevar	name(argc,argv,envv) ;
	cout << "Hello world!\n" ;
	if (argc) {
	    cout << "pn=" << ccp(name) << eol ;
	    for (int ai = 0 ; argv[ai] ; ai += 1) {
		cout << "a=" << argv[ai] << "\n" ;
	    }
	}
	return 0 ;
}
/* end subroutine (main) */


