/* strtime HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* convert a UNIX® time value into a c-string */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-04-10, David A­D­ Morano
	This subroutine was written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	STRTIME_INCLUDE
#define	STRTIME_INCLUDE


#include	<envstandards.h>	/* MUST be first to configure */
#include	<time.h>		/* CSTD |time_t| */
#include	<clanguage.h>		/* LIBU */
#include	<utypedefs.h>		/* LIBU */
#include	<utypealiases.h>	/* LIBU */
#include	<usysdefs.h>		/* LIBU */
#include	<nistinfo.h>		/* LIBUC */

#include	<strtime_val.h>

enum strtimetypes {
	strtimetype_std,	/* standard (and MSG envelope) */
	strtimetype_gmstd,	/* standard for GMT */
	strtimetype_msg,	/* RFC-822 message */
	strtimetype_log,	/* "log" format */
	strtimetype_gmlog,	/* "log" format for GMT */
	strtimetype_logz,	/* "logz" format */
	strtimetype_gmlogz,	/* "logz" format for GMT */
	strtimetype_overlast,
	strtimetype_lolog	= strtimetype_log,
	strtimetype_lologz	= strtimetype_logz
} ; /* end enum (strtimetypes) */


EXTERNC_begin

extern char *strtime_log	(time_t,char *) noex ;
extern char *strtime_msg	(time_t,char *) noex ;
extern char *strtime_std	(time_t,char *) noex ;
extern char *strtime_logz	(time_t,char *) noex ;
extern char *strtime_edate	(time_t,char *) noex ;
extern char *strtime_gmlog	(time_t,char *) noex ;
extern char *strtime_hdate	(time_t,char *) noex ;
extern char *strtime_gmlogz	(time_t,char *) noex ;
extern char *strtime_gmtstd	(time_t,char *) noex ;
extern char *strtime_date	(time_t,char *,strtimetypes) noex ;
extern char *strtime_scandate	(time_t,char *) noex ;
extern char *strtime_elapsed	(time_t,char *) noex ;
extern char *strtime_nist	(time_t,char *,nistinfo *) noex ;

EXTERNC_end


#endif /* STRTIME_INCLUDE */


