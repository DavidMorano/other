/* main SUPPORT */
/* lang=C++23 */

/* the COMPOSE fixer program */
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
	R"xx(¡	_!!_)xx",
	R"xx(¿	_??_)xx",
	R"xx(¢	_c|_	_c/_)xx",
	R"xx(£	_L-_	_L=_)xx",
	R"xx(¥	_Y-_	_Y=_)xx",
	R"xx(§	_SO_	_S0_	_S!_)xx",
	R"xx(¤	_XO_	_X0_)xx",
	R"xx(©	_CO_	_C0_)xx",
	R"xx(®	_RO_	_R0_)xx",
	R"xx(ª	_A__)xx",
	R"xx(º	_O__)xx",
	R"xx(«	_<<_)xx",
	R"xx(»	_>>_)xx",
	R"xx(°	_0^_	_^0_)xx",
	R"xx(¨	_" _	_""_)xx",
	R"xx(´	_''_)xx",
	R"xx(±	_+-_)xx",
	R"xx(¹	_1^_)xx",
	R"xx(²	_2^_)xx",
	R"xx(³	_3^_)xx",
	R"xx(µ	_/U_)xx",
	R"xx(¶	_P!_)xx",
	R"xx(·	_.^_)xx",
	R"xx(¼	_14_)xx",
	R"xx(½	_12_)xx",
	R"xx( 	_  _)xx",
	R"xx(¦	_||_)xx",
	R"xx(¬	_-,_)xx",
	R"xx(­	_--_)xx",
	R"xx(¯	_-^_)xx",
	R"xx(÷	_-:_)xx",
	R"xx(×	_xx_)xx",
	R"xx(¸	_,,_)xx",
	R"xx(À	_A`_)xx",
	R"xx(Á	_A'_)xx",
	R"xx(Â	_A^_)xx",
	R"xx(Ã	_A~_)xx",
	R"xx(Ä	_A"_)xx",
	R"xx(Å	_A*_)xx",
	R"xx(Æ	_AE_)xx",
	R"xx(Ç	_C,_)xx",
	R"xx(È	_E`_)xx",
	R"xx(É	_E'_)xx",
	R"xx(Ê	_E^_)xx",
	R"xx(Ë	_E"_)xx",
	R"xx(Ì	_I`_)xx",
	R"xx(Í	_I'_)xx",
	R"xx(Î	_I^_)xx",
	R"xx(Ï	_I"_)xx",
	R"xx(Ð	_-D_)xx",
	R"xx(Ñ	_N~_)xx",
	R"xx(Ò	_O`_)xx",
	R"xx(Ó	_O'_)xx",
	R"xx(Ô	_O^_)xx",
	R"xx(Õ	_O~_)xx",
	R"xx(Ö	_O"_)xx",
	R"xx(Ø	_O/_)xx",
	R"xx(Þ	_TH_)xx",
	R"xx(Ù	_U`_)xx",
	R"xx(Ú	_U'_)xx",
	R"xx(Û	_U^_)xx",
	R"xx(Ü	_U"_)xx",
	R"xx(Ý	_Y'_)xx",
	R"xx(á	_A'_)xx",
	R"xx(á	_a'_)xx",
	R"xx(â	_a^_)xx",
	R"xx(ã	_a~_)xx",
	R"xx(ä	_a"_)xx",
	R"xx(å	_a*_)xx",
	R"xx(æ	_ae_)xx",
	R"xx(ç	_c,_)xx",
	R"xx(è	_e`_)xx",
	R"xx(é	_e'_)xx",
	R"xx(ê	_e^_)xx",
	R"xx(ë	_e"_)xx",
	R"xx(ì	_i`_)xx",
	R"xx(í	_i'_)xx",
	R"xx(î	_i^_)xx",
	R"xx(ï	_i"_)xx",
	R"xx(ð	_-d_)xx",
	R"xx(ñ	_n~_)xx",
	R"xx(ò	_o`_)xx",
	R"xx(ó	_o'_)xx",
	R"xx(ô	_o^_)xx",
	R"xx(õ	_o~_)xx",
	R"xx(ö	_o"_)xx",
	R"xx(ø	_o/_)xx",
	R"xx(þ	_th_)xx",
	R"xx(ß	_ss_)xx",
	R"xx(ù	_u`_)xx",
	R"xx(ú	_u'_)xx",
	R"xx(û	_u^_)xx",
	R"xx(ü	_u"_)xx",
	R"xx(ý	_y'_)xx",
	R"xx(ÿ	_y"_)xx",
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


