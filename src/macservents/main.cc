/* main (servents) */
/* lang=C++20 */

/* Server-Entries */
/* version %I% last-modified %G% */


/******************************************************************************

	Name:
	servents

	Description:
	This program prints out all of the service entries in the
	system 'services' database.

	Synopsis:
	$ servents

	Returns:
	EXIT_SUCCESS		OK
	EXIT_FAILURE		error (of some kind)

******************************************************************************/

#include	<envstandards.h>
#include	<sys/types.h>
#include	<netdb.h>
#include	<cstddef>
#include	<cstdlib>
#include	<cstdio>
#include	<utypedefs.h>
#include	<clanguage.h>
#include	<localmisc.h>


/* local defines */


/* exported subroutines */

int main(int,mainv,mainv) {
	struct servent	*sep ;
	setservent(1) ;
	while ((sep = getservent()) != nullptr) {
	    cint	port = ntohs(sep->s_port) ;
	    cchar	*fmt = "%-24s %-8s %5u" ;
	    printf(fmt,sep->s_name,sep->s_proto,port) ;
	    if (sep->s_aliases != nullptr) {
	        for (int i = 0 ; sep->s_aliases[i] != nullptr ; i += 1) {
	            printf(" %s",sep->s_aliases[i]) ;
	        } /* end for */
	    } /* end if (aliases) */
	    printf("\n") ;
	} /* end while */
	endservent() ;
	return EXIT_SUCCESS ;
}
/* end subroutine (main) */


