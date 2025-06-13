/* ucinetconv SUPPORT */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* interface component for UNIX® library-3c */
/* Internet-Conversion functions */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-04-13, David A­D­ Morano
	Originally written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Names:
	uc_inetnetpton
	uc_inetpton
	uc_inetntop


	Name:
	uc_inetpton

	Description:
	Convert a string-like INET address into its binary
	representation.

	Synopsis:
	int uc_inetpton(int af,cchar *straddr,void *binaddr) noex

	Arguments:
	af		address family to convert from and to
	straddr		c-string representation of INET address
	binaddr		pointer to variable to hold binary result

	Returns:
	==0		successful
	<0		error (system-return)


	Name:
	uc_inetntop

	Description:
	Convert a binary INET address into its c-string character
	representation.

	Synopsis:
	int uc_inetntop(char *rbuf,int rlen,int af,cvoid *binaddr) noex

	Arguments:
	rbuf		result buffer pointer
	rlen		result buffer length
	af		address-family
	binaddr		source binary NET address to convert from

	Returns:
	>=0		length (in bytes) of result
	<0		error (system-return)

*******************************************************************************/

#include	<envstandards.h>	/* MUST be first to configure */
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<arpa/inet.h>
#include	<cerrno>
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>
#include	<uinet.h>
#include	<localmisc.h>

#include	"ucinetconv.h"

import libutil ;			/* |xstrlen(3u)| */

/* local defines */


/* local namespaces */


/* local typedefs */


/* external subroutines */


/* external variables */


/* local structures */


/* forward references */


/* local variables */

constexpr int		inet4addrlen = int(INET4ADDRLEN) ;
constexpr int		inet6addrlen = int(INET6ADDRLEN) ;


/* exported variables */


/* exported subroutines */

int uc_inetnetpton(void *dbuf,int dlen,int af,cchar *straddr) noex {
	int		rs = SR_FAULT ;
	if (dbuf && straddr) {
	    rs = SR_INVALID ;
	    if ((af >= 0) && (dlen > 0)) {
	        if (int rv ; (rv = inet_net_pton(af,straddr,dbuf,dlen)) > 0) {
		    rs = (af == AF_INET4) ? inet4addrlen : inet6addrlen ;
		} else if (rv == 0) {
	            rs = SR_INVALID ;
	        } else if (rv < 0) {
	            rs = (- errno) ;
	        }
	    } /* end if (valid) */
	} /* end if (non-null) */
	return rs ;
}
/* end subroutine (uc_inetnetpton) */

int uc_inetpton(void *binaddr,int af,cchar *straddr) noex {
	int		rs = SR_FAULT ;
	if (binaddr && straddr) {
	    rs = SR_INVALID ;
	    if (af >= 0) {
	        if (int rv ; (rv = inet_pton(af,straddr,binaddr)) > 0) {
		    rs = (af == AF_INET4) ? inet4addrlen : inet6addrlen ;
		} else if (rv == 0) {
	            rs = SR_INVALID ;
	        } else if (rv < 0) {
	            rs = (- errno) ;
	        }
	    } /* end if (valid) */
	} /* end if (non-null) */
	return rs ;
}
/* end subroutine (uc_inetpton) */

int uc_inetntop(char *rbuf,int rlen,int af,cvoid *binaddr) noex {
    	cnullptr	np{} ;
	int		rs = SR_FAULT ;
	if (rbuf && binaddr) {
	    rs = SR_INVALID ;
	    if ((af >= 0) && (rlen > 0)) {
	        if (cc *rp ; (rp = inet_ntop(af,binaddr,rbuf,rlen)) != np) {
		    rs = xstrlen(rbuf) ;
		} else {
		    rs = (- errno) ;
		}
	    } /* end if (valid) */
	} /* end if (non-null) */
	return rs ;
}
/* end subroutine (uc_inetntop) */


