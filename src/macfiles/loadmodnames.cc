/* loadmodnames SUPPORT */
/* charset=ISO8859-1 */
/* lang=C++20 (conformance reviewed) */

/* load module names from a C++ source code file */
/* version %I% last-modified %G% */


/* revision history:

	= 2017-03-21, David A-D- Morano
	This subroutine was originally written.

*/

/* Copyright © 2017 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Names:
	loadmodnames

	Description:
	I parse a C++ source code file and find all of the module
	names that I can that are being imported to rhat source file.

	Synopsis:
	loadmodnames(vecstr *op,cchar *fname) noex

	Arguments:
	op		ureserve::vecstr
	fname		file-name to parse

	Returns:
	>=0		number of imported modules found
	<0		error (system-return)

*******************************************************************************/

#include	<envstandards.h>	/* ordered first to configure */
#include	<sys/stat.h>
#include	<unistd.h>
#include	<fcntl.h>
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<cstdio>
#include	<new>			/* |nothrow(3c++)| */
#include	<iostream>
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>
#include	<getourenv.h>
#include	<getfdfile.h>		/* |FD_STDERR| */
#include	<strn.h>
#include	<strw.h>		/* |strwcmp(3uc)| */
#include	<sfx.h>			/* |sfbasename(3uc)| + |sfext(3uc)| */
#include	<six.h>			/* |sisub(3uc)| + |siext(3uc)| */
#include	<rmx.h>
#include	<strwcpy.h>
#include	<strnul.hh>
#include	<ccfile.hh>
#include	<readln.hh>
#include	<matstr.h>
#include	<mkchar.h>
#include	<isnot.h>
#include	<localmisc.h>

#pragma		GCC dependency	"mod/ureserve.ccm"

import ureserve ;			/* |vecstr(3u)| */

/* local defines */


/* imported namespaces */


/* local typedefs */


/* external subroutines */


/* external variables */


/* local structures */

namespace {
    struct vecmgr {
	vecstr		*vop ;
	cchar		*fname ;
	vecmgr(vecstr *p,cchar *f) noex : vop(p), fname(f) { } ;
	operator int () noex ;
    } ;
} /* end namespace */


/* forward references */


/* local variables */


/* exported variables */


/* exported subroutines */

int loadmodnames(vecstr *op,cchar *fname) {
    	vecmgr		mgr(op,fname) ;
	return mgr ;
}
/* end subroutine (loadmodname) */


/* local subroutines */

vecmgr::operator int () noex {
    	int		rs = SR_OK ;
    	return rs ;
}


