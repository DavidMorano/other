/* readln HEADER */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* read characters from a C++ input stream */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-04-10, David A­D­ Morano
	This subroutine was written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	READLN_INCLUDE
#define	READLN_INCLUDE
#ifdef	__cplusplus


#include	<envstandards.h>	/* MUST be first to configure */
#include	<istream>
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>


extern int readln(std::istream *,char *,int,int = 0) noex ;
extern int istr_readln(std::istream *,char *,int,int = 0) noex ;

#endif /* __cplusplus */
#endif /* READLN_INCLUDE */


