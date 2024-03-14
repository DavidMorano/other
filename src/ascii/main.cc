/* main SUPPORT (ascii) */
/* lang=C++23 */

/* print out an ASCII table (of some sort) */
/* version %I% last-modified %G% */


/* revision history:

	= 2001-04-11, David A­D­ Morano
	Originally written for Rightcore Network Services.

*/

/* Copyright © 2001 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	This program prints out a table of the ASCII control characters.

*******************************************************************************/

#include	<envstandards.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<unistd.h>
#include	<iostream>
#include	<cstdio>


/* namespaces */

using namespace		std ;

/* typedefs */

typedef const char	cchar ;
typedef struct stat	statblock ;

/* local structures */

struct tabent {
	cchar	*cname ;
	cchar	*explain ;
} ;

/* local variables */

static constexpr tabent		table[] = {
	{ "NUL", "null character" },
	{ "SOH", "start of heading" },
	{ "STX", "start of text" },
	{ "ETX", "end of text" },
	{ "EOT", "end of transmission" },
	{ "ENQ", "enquiry" },
	{ "ACK", "acknowledgement" },
	{ "BEL", "bell sound" },
	{ "BS", "backspace" },
	{ "HT", "horizontal tab" },
	{ "LF", "line feed" },
	{ "VT", "vertical tab" },
	{ "FF", "form feed" },
	{ "CR", "carriage return" },
	{ "SO", "shift out" },
	{ "SI", "shift in" },
	{ "DLE", "data link escape" },
	{ "DC1", "device control-1 (XON)" },
	{ "DC2", "device control-2" },
	{ "DC3", "device control-3 (XOFF)" },
	{ "DC4", "device control-4" },
	{ "NAK", "negative acknowledgment" },
	{ "SYN", "synchronous idle" },
	{ "ETB", "end of transmission block" },
	{ "CAN", "cancel" },
	{ "EM", "end of medium" },
	{ "SUB", "substitute" },
	{ "ESC", "escape" },
	{ "FS", "file separator" },
	{ "GS", "group separator" },
	{ "RS", "record separator" },
	{ "US", "unit separator" },
	{ nullptr, nullptr }
} ;

/* exported subroutines */

int main(int argc,cchar **,cchar **) noexcept {
	int	ex = 1 ;
	if (argc > 0) {
	    cchar	*fmt = "%3d %2X %03o %-3s %s\n" ;
	    for (int i = 0 ; i < 0x20 ; i += 1) {
		printf(fmt,i,i,i,table[i].cname,table[i].explain) ;
	    } /* end for */
	    ex = 0 ;
	} /* end if */
	return ex ;
}
/* end subroutine (main) */


