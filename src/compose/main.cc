/* main SUPPORT */
/* lang=C++23 */

/* the COMPOSE fixer program */
/* version %I% last-modified %G% */


/* revision history:

	= 2001-04-11, David A�D� Morano
	Originally written for Rightcore Network Services.

*/

/* Copyright � 2001 David A�D� Morano.  All rights reserved. */

/*******************************************************************************

	This program prints out a table of the ASCII control characters.

*******************************************************************************/

#include	<envstandards.h>
#include	<unistd.h>
#include	<iostream>
#include	<string>		/* yes: C++ "string" obect */
#include	<cstdio>
#include	<utypedefs.h>
#include	<clanguage.h>
#include	<mkchar.h>
#include	<localmisc.h>


/* imported namespaces */

using namespace		std ;


/* local typedefs */


/* local structures */


/* local variables */

static constexpr cchar	*lines[] = {
	R"xx(�	_!!_)xx",
	R"xx(�	_??_)xx",
	R"xx(�	_c|_	_c/_)xx",
	R"xx(�	_L-_	_L=_)xx",
	R"xx(�	_Y-_	_Y=_)xx",
	R"xx(�	_SO_	_S0_	_S!_)xx",
	R"xx(�	_XO_	_X0_)xx",
	R"xx(�	_CO_	_C0_)xx",
	R"xx(�	_RO_	_R0_)xx",
	R"xx(�	_A__)xx",
	R"xx(�	_O__)xx",
	R"xx(�	_<<_)xx",
	R"xx(�	_>>_)xx",
	R"xx(�	_0^_	_^0_)xx",
	R"xx(�	_" _	_""_)xx",
	R"xx(�	_''_)xx",
	R"xx(�	_+-_)xx",
	R"xx(�	_1^_)xx",
	R"xx(�	_2^_)xx",
	R"xx(�	_3^_)xx",
	R"xx(�	_/U_)xx",
	R"xx(�	_P!_)xx",
	R"xx(�	_.^_)xx",
	R"xx(�	_14_)xx",
	R"xx(�	_12_)xx",
	R"xx(�	_  _)xx",
	R"xx(�	_||_)xx",
	R"xx(�	_-,_)xx",
	R"xx(�	_--_)xx",
	R"xx(�	_-^_)xx",
	R"xx(�	_-:_)xx",
	R"xx(�	_xx_)xx",
	R"xx(�	_,,_)xx",
	R"xx(�	_A`_)xx",
	R"xx(�	_A'_)xx",
	R"xx(�	_A^_)xx",
	R"xx(�	_A~_)xx",
	R"xx(�	_A"_)xx",
	R"xx(�	_A*_)xx",
	R"xx(�	_AE_)xx",
	R"xx(�	_C,_)xx",
	R"xx(�	_E`_)xx",
	R"xx(�	_E'_)xx",
	R"xx(�	_E^_)xx",
	R"xx(�	_E"_)xx",
	R"xx(�	_I`_)xx",
	R"xx(�	_I'_)xx",
	R"xx(�	_I^_)xx",
	R"xx(�	_I"_)xx",
	R"xx(�	_-D_)xx",
	R"xx(�	_N~_)xx",
	R"xx(�	_O`_)xx",
	R"xx(�	_O'_)xx",
	R"xx(�	_O^_)xx",
	R"xx(�	_O~_)xx",
	R"xx(�	_O"_)xx",
	R"xx(�	_O/_)xx",
	R"xx(�	_TH_)xx",
	R"xx(�	_U`_)xx",
	R"xx(�	_U'_)xx",
	R"xx(�	_U^_)xx",
	R"xx(�	_U"_)xx",
	R"xx(�	_Y'_)xx",
	R"xx(�	_A'_)xx",
	R"xx(�	_a'_)xx",
	R"xx(�	_a^_)xx",
	R"xx(�	_a~_)xx",
	R"xx(�	_a"_)xx",
	R"xx(�	_a*_)xx",
	R"xx(�	_ae_)xx",
	R"xx(�	_c,_)xx",
	R"xx(�	_e`_)xx",
	R"xx(�	_e'_)xx",
	R"xx(�	_e^_)xx",
	R"xx(�	_e"_)xx",
	R"xx(�	_i`_)xx",
	R"xx(�	_i'_)xx",
	R"xx(�	_i^_)xx",
	R"xx(�	_i"_)xx",
	R"xx(�	_-d_)xx",
	R"xx(�	_n~_)xx",
	R"xx(�	_o`_)xx",
	R"xx(�	_o'_)xx",
	R"xx(�	_o^_)xx",
	R"xx(�	_o~_)xx",
	R"xx(�	_o"_)xx",
	R"xx(�	_o/_)xx",
	R"xx(�	_th_)xx",
	R"xx(�	_ss_)xx",
	R"xx(�	_u`_)xx",
	R"xx(�	_u'_)xx",
	R"xx(�	_u^_)xx",
	R"xx(�	_u"_)xx",
	R"xx(�	_y'_)xx",
	R"xx(�	_y"_)xx",
	R"xx(#	_++_)xx",
	R"xx(@	_AA_)xx",
	R"xx('	_' _)xx",
	R"xx([	_((_)xx",
	R"xx(]	_))_)xx",
	R"xx(\	_//_)xx",
	R"xx(^	_^ _)xx",
	R"xx(`	_` _)xx",
	R"xx({	_(-_)xx",
	R"xx(}	_)-_)xx",
	R"xx(|	_/^_)xx",
	R"xx(~	_~ _)xx",
	nullptr
} ;


/* exported variables */


/* exported subroutines */

int main(int argc,cchar **,cchar **) noexcept {
	int	ex = 1 ;
	if (argc > 0) {
	    cchar	*fmt = "%3d %2X %3o %s\n" ;
	    for (int i = 0 ; lines[i] ; i += 1) {
		int	code = mkchar(lines[i][0]) ;
	        cchar	*lp = lines[i] ;
		printf(fmt,code,code,code,lp) ;
	    } /* end while */
	    ex = 0 ;
	} /* end if */
	return ex ;
}
/* end subroutine (main) */


