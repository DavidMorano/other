/* obuf HEADER */
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
	obuf

	Description:
	See the source support file ('obuf.cc') for a description
	of this object.

*******************************************************************************/

#ifndef	OBUF_INCLUDE
#define	OBUF_INCLUDE
#ifdef	__cplusplus


#include	<envstandards.h>	/* MUST be first to configure */
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<string>		/* |string(3c++)| */
#include	<clanguage.h>
#include	<usysbase.h>

enum obufmems {
	obufmem_start,
	obufmem_adv,
	obufmem_count,
	obufmem_len,
	obufmem_finish,
	obufmem_overlast
} ; /* end enum (obufmems) */

struct obuf_fl {
    	uint		open:1 ;
} ; /* end struct (obuf_fl) */

struct obuf ;

struct obuf_rem {
	obuf		*op = nullptr ;
	void operator () (obuf *p,int) noex {
	    op = p ;
	} ;
	int operator () (char * = nullptr) noex ;
	operator int () noex {
	    return operator () (nullptr) ;
	} ;
} ; /* end struct (obuf_rem) */

struct obuf_co {
	obuf		*op = nullptr ;
	int		w = -1 ;
	void operator () (obuf *p,int m) noex {
	    op = p ;
	    w = m ;
	} ;
	int operator () (int = -1) noex ;
	operator int () noex {
	    return operator () (-1) ;
	} ;
} ; /* end struct (obuf_co) */

class obuf {
    	friend		obuf_co ;
    	friend		obuf_rem ;
	std::string	b ;
	int		oi ;		/* output index */
	int istart() noex ;
	int ifinish() noex ;
	int ilen() const noex ;
	int iadv(int) noex ;
	int iremove(char *) noex ;
public:
	obuf_co		start ;
	obuf_co		finish ;
	obuf_co		adv ;
	obuf_co		count ;
	obuf_co		len ;
	obuf_rem	rem ;
	obuf_fl		fl{} ;
	obuf(const obuf &) = delete ;
	obuf &operator = (const obuf &) = delete ;
	obuf(cchar * = nullptr,int = -1) noex ;
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
	destruct obuf() {
	    if (fl.open) dtor() ;
	} ; /* end dtor */
} ; /* end class (obuf) */


#endif /* __cplusplus */
#endif /* OBUF_INCLUDE */


