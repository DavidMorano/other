/* files_helpdata SUPPORT */
/* charset=ISO8859-1 */
/* lang=C++20 (conformance reviewed) */

/* help-usage data */
/* version %I% last-modified %G% */


/* revision history:

	= 2000-05-14, David A­D­ Morano
	Originally written for Rightcore Network Services.

*/

/* Copyright © 2000 David A­D­ Morano.  All rights reserved. */

#include	<envstandards.h>	/* ordered first to configure */
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>		/* |getenv(3c)| */
#include	<clanguage.h>
#include	<usysbase.h>
#include	<localmisc.h>

#include	"files_helpdata.h"


const cpcchar	helpdata[] = {
    "%s: USAGE> %s <file(s)>\n",
    "%s:  [-Q] [-D] [-v[=<n>]] [-HELP] [-V]\n",
    nullptr
} ; /* end struct (helpdata) */


