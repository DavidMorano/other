/* termcolor_main SUPPORT */
/* charset=ISO8859-1 */
/* lang=C++23 */

/* the COMPOSE fixer program */
/* version %I% last-modified %G% */


/* revision history:

	= 2001-04-11, David A­D­ Morano
	Originally written for Rightcore Network Services.

*/

/* Copyright © 2001 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

  	Name:
	main

	Description:
	This program prints out a table of the ASCII control characters.

*******************************************************************************/

#include	<envstandards.h>	/* ordered first to configure */
#include	<unistd.h>		/* POSIX® */
#include	<cstddef>		/* CSTD */
#include	<cstdlib>		/* CSTD */
#include	<cstdio>		/* CSTD */
#include	<string>		/* C++STD yes: "string" */
#include	<iostream>		/* C++STD */
#include	<clanguage.h>		/* LIBU */
#include	<usysbase.h>		/* LIBU */
#include	<mkchar.h>		/* LIBU */
#include	<mapex.h>		/* LIBU */
#include	<localmisc.h>		/* LIBU */


/* local defines */


/* imported namespaces */


/* local typedefs */


/* external subroutines */


/* external variables */


/* local structures */

struct codeset {
    	int	nback ;			/* background */
	int	nfore ;			/* foreground */
} ; /* end struct */


/* forward references */

local int	printc(int,int) noex ;


/* local variables */

constexpr codeset	sets[] = {
	{ 40,	30 },
	{ 100,	90 }
} ; /* end array */

constexpr cpcchar	colors[] = {
    	"Black",
	"Red",
	"Green",
	"Yellow",
	"Blue",
	"Magenta",
	"Cyan",
	"White",
	nullptr
} ; /* end array */


/* exported variables */


/* exported subroutines */

int main(int argc,con mainv,con mainv) {
	int		ex = EXIT_SUCCESS ;
	int		rs = SR_OK ;
	if (argc > 0) {
	    for (cauto &se : sets) {
	        rs = printc(se.nback,se.nfore) ;
		if (rs < 0) break ;
	    } /* end for */
	} /* end if */
	if ((ex == EXIT_SUCCESS) && (rs < 0)) {
	    ex = EXIT_FAILURE ;
	}
	return ex ;
} /* end subroutine (main) */


/* local subroutines */

local int printc(int nback,int nfore) noex {
    	int		rs = SR_OK ;
	cchar *fmt = "\033[%dm        \033[0m  \033[%dm%s\033[0m\n" ;
	for (int i = 0 ; i < 8 ; i += 1) {
	    printf(fmt,(i + nback),(i + nfore),colors[i]) ;
	} /* end for */
	return rs ;
} /* end subroutine (printc) */


