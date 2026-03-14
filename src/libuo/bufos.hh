/* bufos HEADER */
/* charset=ISO8859-1 */
/* lang=C++98 */

/* Output Buffer */
/* version %I% last-modified %G% */


/* revision history:

	= 2016-06-29, David A­D­ Morano
	This code was originally written.

*/

/* Copyright © 2016 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

  	Object:
	bufos

	Description:
	See the source support file ('bufos.cc') for a description
	of this object.

*******************************************************************************/

#ifndef	BUFOS_INCLUDE
#define	BUFOS_INCLUDE
#ifdef	__cplusplus


#include	<envstandards.h>	/* MUST be first to configure */
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<string>		/* |string(3c++)| */
#include	<clanguage.h>
#include	<usysbase.h>

enum bufosmems {
	bufosmem_start,
	bufosmem_adv,
	bufosmem_count,
	bufosmem_len,
	bufosmem_finish,
	bufosmem_overlast
} ; /* end enum (bufosmems) */

struct bufos_fl {
    	uint		open:1 ;
} ; /* end struct (bufos_fl) */

struct bufos ;

struct bufos_rem {
	bufos		*op = nullptr ;
	void operator () (bufos *p,int) noex {
	    op = p ;
	} ;
	int operator () (char * = nullptr) noex ;
	operator int () noex {
	    return operator () (nullptr) ;
	} ;
} ; /* end struct (bufos_rem) */

struct bufos_co {
	bufos		*op = nullptr ;
	int		w = -1 ;
	void operator () (bufos *p,int m) noex {
	    op = p ;
	    w = m ;
	} ;
	int operator () (int = -1) noex ;
	operator int () noex {
	    return operator () (-1) ;
	} ;
} ; /* end struct (bufos_co) */

class bufos {
    	friend		bufos_co ;
    	friend		bufos_rem ;
	std::string	b ;
	int		oi ;		/* output index */
	int istart() noex ;
	int ifinish() noex ;
	int ilen() const noex ;
	int iadv(int) noex ;
	int iremove(char *) noex ;
public:
	bufos_co		start ;
	bufos_co		finish ;
	bufos_co		adv ;
	bufos_co		count ;
	bufos_co		len ;
	bufos_rem	rem ;
	bufos_fl		fl{} ;
	bufos(const bufos &) = delete ;
	bufos &operator = (const bufos &) = delete ;
	bufos(cchar * = nullptr,int = -1) noex ;
	int push	(int) noex ;
	int ins		(int ch) noex {
	    return push(ch) ;
	} ;
	int add		(int ch) noex {
	    return push(ch) ;
	} ;
	int add		(cchar *,int = -1) noex ;
	int at		(int) const noex ;
	int readat	(int,char *,int) const noex ;
	int read	(char *,int) const noex ;
	void dtor() noex ;
	int operator [] (int) const noex ;
	operator int () const noex {
	    return ilen() ;
	} ;
	destruct bufos() {
	    if (fl.open) dtor() ;
	} ; /* end dtor */
} ; /* end class (bufos) */


#endif /* __cplusplus */
#endif /* BUFOS_INCLUDE */


