/* termstr HEADER */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* terminal display editing control sequences */
/* version %I% last-modified %G% */


/* revision history:

	= 1999-08-01, Dave morano
	File was originally written.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	TERMSTR_INCLUDE
#define	TERMSTR_INCLUDE


#include	<envstandards.h>	/* ordered first to configure */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>
#include	<buffer.h>


enum termrenditions {
    termrendition_bold,
    termrendition_dim,
    termrendition_italic,
    termrendition_under,
    termrendition_blinks,
    termrendition_blinkr,
    termrendition_reverse,
    termrendition_hidden,
    termrendition_strike,
    termrendition_overlast,
    termrendition_blink = termrendition_blanks
} ; /* end enum (termrenditions) */

/* graphic renditions */
#define	TERMSTR_GRNONE		0				/* none */
#define	TERMSTR_GRBOLD		(1<<termrendition_bold)		/* bold */
#define	TERMSTR_GRDIM		(1<<termrendition_dim)		/* dim */
#define	TERMSTR_GRITALIC	(1<<termrendition_italic)	/* italic */
#define	TERMSTR_GRUNDER		(1<<termrendition_under)	/* underline */
#define	TERMSTR_GRBLINKS	(1<<termrendition_blinks)	/* blink(s) */
#define	TERMSTR_GRBLINKR	(1<<termrendition_blinkr)	/* blink(r) */
#define	TERMSTR_GRREVERSE	(1<<termrendition_reverse)	/* reverse */
#define	TERMSTR_GRHIDDEN	(1<<termrendition_hidden)	/* hidden */
#define	TERMSTR_GRSTRIKE	(1<<termrendition_strike)	/* strike */
#define	TERMSTR_GRBLINK	TERMSTR_GRBLINKS	/* graphic blinking */
#define	TERMSTR_GRREV	TERMSTR_GRREVERSE	/* reverse */

/* graphic rendition codes */
#define	TERMSTR_BOLD	"\033[1m"	/* bold */
#define	TERMSTR_DIM	"\033[2m"	/* dim */
#define	TERMSTR_ITALIC	"\033[3m"	/* italic */
#define	TERMSTR_UNDER	"\033[4m"	/* underline */
#define	TERMSTR_BLINKS	"\033[5m"	/* blink (slow) */
#define	TERMSTR_BLINKR	"\033[6m"	/* blink (repid) */
#define	TERMSTR_REVERSE	"\033[7m"	/* reverse */
#define	TERMSTR_HIDDEN	"\033[8m"	/* hidden */
#define	TERMSTR_SRIKE	"\033[9m"	/* strike (through) */
#define	TERMSTR_BLINK	TERMSTR_BLINKS	/* blink (slow) */

/* cursor saving and restoring (VT1xx "Fp" escape squences) */
#define	TERMSTR_VCURS	"\0337"		/* cursor save-restore (VT) */
#define	TERMSTR_VCURR	"\0338"		/* cursor save-restore (VT) */

#define	TERMSTR_ACURS	"\033[s"	/* cursor save-restore (ANSI) */
#define	TERMSTR_ACURR	"\033[u"	/* cursor save-restore (ANSI) */

#define	TERMSTR_SAVE	TERMSTR_VCURS	/* cursor save-restore (VT) */
#define	TERMSTR_RESTORE	TERMSTR_VCURR	/* cursor save-restore (VT) */

/* standard "Fs" ISO/IEC 2022 escape sequences */
#define	TERMSTR_SS2	"\033N"		/* single-shift-2 */
#define	TERMSTR_SS3	"\033O"		/* single-shift-2 */
#define	TERMSTR_DCS	"\033P"		/* device-control-string */
#define	TERMSTR_CSI	"\033["		/* control-sequence-introducer */
#define	TERMSTR_ST	"\033\\"	/* string-terminator */
#define	TERMSTR_OSC	"\033]"		/* operating-system-command */
#define	TERMSTR_PM	"\033^"		/* privacy-message */
#define	TERMSTR_APC	"\033_"		/* application-program-command */

/* more "Fs" sequences */
#define	TERMSTR_IND	"\033D"		/* index (UP) */
#define	TERMSTR_RI	"\033M"		/* reverse-index (DOWN) */
#define	TERMSTR_NEL	"\033E"		/* new-line */
#define	TERMSTR_HTS	"\033H"		/* horizontal-tab-set */
#define	TERMSTR_SOS	"\033X"		/* start-of-string */
#define	TERMSTR_DECID	"\033Z"		/* VT-indentification */

/* cursor positioning */
#define	TERMSTR_CURU	"\033[A"	/* cursor up */
#define	TERMSTR_CURD	"\033[B"	/* cursor down */
#define	TERMSTR_CURR	"\033[C"	/* cursor right */
#define	TERMSTR_CURL	"\033[D"	/* cursor left */
#define	TERMSTR_CURH	"\033[H"	/* cursor home */

#define	TERMSTR_UP	TERMSTR_CURU
#define	TERMSTR_DOWN	TERMSTR_CURD
#define	TERMSTR_RIGHT	TERMSTR_CURR
#define	TERMSTR_LEFT	TERMSTR_CURL
#define	TERMSTR_HOME	TERMSTR_CURH

/* editing */
#define	TERMSTR_ED	"\033[J"	/* erase to end of display */
#define	TERMSTR_EL	"\033[K"	/* erase to end of line */
#define	TERMSTR_EC	"\033[X"	/* erase character */

#define	TERMSTR_EDIS	"\033[J"	/* erase to end of display */
#define	TERMSTR_X	"\033[X"	/* erase character */
#define	TERMSTR_ECH	"\033[X"	/* erase character */

#define	TERMSTR_DL	"\033[M"	/* delete line */
#define	TERMSTR_DC	"\033[P"	/* delete character */
#define	TERMSTR_DCH	"\033[P"	/* delete character */

#define	TERMSTR_IL	"\033[L"	/* insert line */
#define	TERMSTR_IC	"\033[@"	/* insert character */
#define	TERMSTR_ICH	"\033[@"	/* insert character */

/* character renditions */
#define	TERMSTR_NORM	"\033[m"	/* no attributes */
#define	TERMSTR_BOLD	"\033[1m"	/* bold */
#define	TERMSTR_DIM	"\033[2m"	/* dim */
#define	TERMSTR_ITALIC	"\033[3m"	/* italic */
#define	TERMSTR_UNDER	"\033[4m"	/* underline */
#define	TERMSTR_BLINK	"\033[5m"	/* (slow) blink */
#define	TERMSTR_BLINKS	"\033[5m"	/* (slow) blink */
#define	TERMSTR_BLINKR	"\033[6m"	/* (rapid) blink */
#define	TERMSTR_REVERSE	"\033[7m"	/* reverse */
#define	TERMSTR_HIDDEN	"\033[8m"	/* hidden */
#define	TERMSTR_SRIKE	"\033[9m"	/* strike (through) */

/* character renditions for advanced terminals */
#define	TERMSTR_NOBOLD	"\033[22m"	/* no-bold */
#define	TERMSTR_NOUNDER	"\033[24m"	/* no-underline */
#define	TERMSTR_NOBLINK	"\033[27m"	/* no-blink */

/* insert-replacement mode */
#define	TERMSTR_S_IRM	"\033[4h"	/* insert-replacement set */
#define	TERMSTR_R_IRM	"\033[4l"	/* insert-replacement clear */

/* cursor visibility */
#define	TERMSTR_S_VCUR	"\033[?25h"	/* set cursor ON (VT) */
#define	TERMSTR_R_VCUR	"\033[?25l"	/* set cursor OFF (VT) */

#define	TERMSTR_S_ACUR	"\033[34h"	/* set cursor ON (ANSI) */
#define	TERMSTR_R_ACUR	"\033[34l"	/* set cursor OFF (ANSI) */

#define	TERMSTR_S_SCUR	"\0337p"	/* set cursor ON (SCREEN) */
#define	TERMSTR_R_SCUR	"\0336p"	/* set cursor OFF (SCREEN) */

#define	TERMSTR_S_CUR	TERMSTR_S_VCUR	/* cursor ON (VT) */
#define	TERMSTR_R_CUR	TERMSTR_R_VCUR	/* cursor OFF (VT) */

/* status display */
#define	TERMSTR_S_SD	"\033[1$\175"	/* status display SET */
#define	TERMSTR_R_SD	"\033[0$\175"	/* status display RESET */

#define	TERMSTR		struct termstr_head


struct termstr_head {
	buffer		*sbp ;		/* storage buffer pointer */
	uint		magic ;
	int		ti ;		/* index into database */
} ;

typedef TERMSTR		termstr ;

EXTERNC_begin

extern int termstr_start	(termstr *,cchar *) noex ;
extern int termstr_clean	(termstr *) noex ;
extern int termstr_char		(termstr *,int) noex ;
extern int termstr_write	(termstr *,cchar *,int) noex ;
extern int termstr_writegr	(termstr *,int,cchar *,int) noex ;
extern int termstr_get		(termstr *,cchar **) noex ;
extern int termstr_ed		(termstr *,int) noex ;
extern int termstr_el		(termstr *,int) noex ;
extern int termstr_ec		(termstr *,int) noex ;
extern int termstr_curu		(termstr *,int) noex ;
extern int termstr_curd		(termstr *,int) noex ;
extern int termstr_curl		(termstr *,int) noex ;
extern int termstr_curr		(termstr *,int) noex ;
extern int termstr_curh		(termstr *,int,int) noex ;
extern int termstr_ssr		(termstr *,int,int) noex ;
extern int termstr_csr		(termstr *,int) noex ;
extern int termstr_il		(termstr *,int) noex ;
extern int termstr_ic		(termstr *,int) noex ;
extern int termstr_dl		(termstr *,int) noex ;
extern int termstr_dc		(termstr *,int) noex ;
extern int termstr_irm		(termstr *,int) noex ;
extern int termstr_cvis		(termstr *,int) noex ;
extern int termstr_finish	(termstr *) noex ;

EXTERNC_end


#endif /* TERMSTR_INCLUDE */


