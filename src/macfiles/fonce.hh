/* fonce HEADER */
/* encoding=ISO8859-1 */
/* lang=C++20 */

/* implement a map container of blocks (of a given structure) */
/* version %I% last-modified %G% */


/* revision history:

	= 2011-04-12, David A­D­ Morano
	This subroutine was originally written.  Only the introduction
	of C++11 has allowed this (finally), due to the addition
	(in C++11) of |unordered_set(3c++)|.

*/

/* Copyright © 2011 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Name:
	fonce

	Description:
	This object implements a set with a key consisting of
	device and inode numbers.

	Symopsis:
	int fonce_start(int n = 0) noex

	Arguments:
	n		suggested starting possible number of elements

	Returns:
	>=0		ok
	<0		error (system-return)

*******************************************************************************/

#ifndef	FONCE_INCLUDE
#define	FONCE_INCLUDE
#ifdef	__cplusplus /* everything is C++ only */


#include	<envstandards.h>	/* MUST be first to configure */
#include	<sys/types.h>
#include	<unistd.h>
#include	<fcntl.h>
#include	<utility>
#include	<unordered_set>
#include	<functional>
#include	<iostream>
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysrets.h>


struct fonce_devino {
	dev_t		dev ;
	ino_t		ino ;
	fonce_devino(dev_t d,ino_t i) noex : dev(d), ino(i) { } ;
	size_t hashval() const noex {
	    return size_t(dev + ino) ;
	} ;
	bool equal_to(const fonce_devino &o) noex {
	    return ((dev == o.dev) && (ino == o.ino)) ;
	} ;
} ; /* end struct (fonce_devino) */

namespace std {
    template<> struct hash<fonce_devino> {
	size_t operator() (const fonce_devino &di) const noex {
	    return di.hashval() ;
	} ;
    } ; /* end struct-template (hash<fonce_devino>) */
    template<> struct equal_to<fonce_devino> {
	typedef const fonce_devino	cdi ;
	size_t operator() (cdi &lhs,cdi &rhs) const noex {
	    return ((lhs.dev == rhs.dev) && (lhs.ino == rhs.ino)) ;
	} ;
    } ; /* end struct-template (hash<fonce_devino>) */
}

enum foncemems {
	foncemem_start,
	foncemem_finish,
	foncemem_count,
	foncemem_overlast
} ;

struct fonce ;

struct fonce_co {
	fonce		*op = nullptr ;
	int		w = -1 ;
	constexpr void operator () (fonce *p,int m) noex {
	    op = p ;
	    w = m ;
	} ;
	int operator () (int = 0) noex ;
	operator int () noex {
	    return operator () () ;
	} ;
} ; /* end struct (fonce_co) */

struct fonce {
	friend fonce_co ;
	typedef std::unordered_set<fonce_devino>	stype ;
	typedef fonce_devino	key_type ;
	fonce(const fonce &) = delete ;
	fonce &operator = (const fonce &) = delete ;
	stype		*setp = nullptr ;
	fonce_co	start ;
	fonce_co	finish ;
	fonce_co	count ;
	fonce() noex {
	    start(this,foncemem_start) ;
	    finish(this,foncemem_finish) ;
	    count(this,foncemem_count) ;
	} ; /* end ctor) */
	~fonce() noex {
	    (void) ifinish() ;
	} ; /* end dtor */
	int		checkin(CUSTAT *) noex ;
	int		istart(int) noex ;
	int		ifinish() noex ;
	int		icount() noex ;
} ; /* end class (fonce) */


#endif	/* __cplusplus */
#endif /* FONCE_INCLUDE */


