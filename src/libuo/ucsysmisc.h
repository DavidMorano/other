/* ucsysmisc HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* slightly tuned UNIX® look-alike for |getpageize(3c)| */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-02-01, David A­D­ Morano
	This code was originally written.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	UCSYSMISC_INCLUDE
#define	UCSYSMISC_INCLUDE


#include	<envstandards.h>	/* MUST be first to configure */
#include	<clanguage.h>		/* LIBU */
#include	<usysbase.h>		/* LIBU */

#include	<ucmincore.h>


EXTERNC_begin

extern int	uc_nprocessors	(int) noex ;
extern int	uc_gethz	(int) noex ;
extern int	uc_syspages	(int) noex ;
extern int	uc_pagesize	() noex ;
extern int	uc_ftime	(TIMEB *) noex ;
extern int	uc_syncer	(int) noex ;

EXTERNC_end

EXTERNC_begin
extern int	ucsyncer_init() noex ;
extern int	ucsyncer_fini() noex ;
EXTERNC_end

#ifdef	__cplusplus

namespace libuc {
    struct ucpagesizer {
    	int		pagesz ;
    	operator int () noex ;
	int operator () () noex {
	    return operator int () ;
	} ;
    } ; /* end struct (ucpagesizer) */
} /* end namespace (libuc) */

namespace libuc {
    struct ucnprocesser {
    	operator int () noex ;
	int operator () (int = 0) noex ;
    } ; /* end struct (ucnprocesser) */
} /* end namespace (libuc) */

extern libuc::ucpagesizer	ucpagesize ;
extern libuc::ucnprocesser	ucnprocessors ;

#endif /* __cplusplus */


#endif /* UCSYSMISC_INCLUDE */


