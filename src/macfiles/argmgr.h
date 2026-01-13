/* argmgr HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* Argument-Manager */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-11-01, David A­D­ Morano
	This subroutine was written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Object:
	argmgr
	
	Descpriotn:
	This provides a helper-debug macro for debugging the
	ARGMGR object.

	Notes:
	1. Note the use of the GCC extension for providing compound
	statement-list expressions in a special way.  This extension
	is more formerly named a "statement-expression" by the GCC
	people.  Note that both CLANG and the Intel-C++ compilers
	also support this extension.  These statement-expressions
	are very useful within preprocessor macros, and that is
	exactly how I used one below.  I love this extension and
	feel that it should be incorporated into the C++ standard.

*******************************************************************************/

#ifndef	ARGMGR_INCLUDE
#define	ARGMGR_INCLUDE


#include	<envstandards.h>	/* ordered first to configure */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>
#include	<debprintf.h>


#define DEBPR(FMT, ...) ({						\
	int debrc = 0 ;							\
	if_constexpr (f_debug) {					\
	    debrc = debprintf(__func__, FMT __VA_OPT__(,) __VA_ARGS__) ;\
	}								\
	debrc ;								\
})


#endif /* ARGMGR_INCLUDE */


