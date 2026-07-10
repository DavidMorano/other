/* termdisp HEADER (Terminal-Display) */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* Terminal-Display manager */
/* version %I% last-modified %G% */


/* revision history:

	= 1999-08-01, Dave morano
	File was originally written.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	TERMDISP_INCLUDE
#define	TERMDISP_INCLUDE


#include	<envstandards.h>	/* MUST be first to configure */
#include	<stdarg.h>		/* CSTD |va_list| */
#include	<clanguage.h>		/* LIBU */
#include	<usysbase.h>		/* LIBU */
#include	<vecitem.h>		/* LIBUC */
#include	<termstr.h>		/* LIBUC */


#define	TERMDISP		struct termdisp_head
#define	TERMDISP_POS		struct termdisp_position
#define	TERMDISP_WIN		struct termdisp_window
#define	TERMDISP_FL		struct termdisp_flags
#define	TERMDISP_MAGIC		0x02652103

/* control codes */
enum termdispcmds {
    	termdispcmd_eol,	/* set "erase to EOL" mode */
    	termdispcmd_lines,	/* change number of lines in window */
    	termdispcmd_scroll,	/* scroll the window up or down */
    	termdispcmd_subwin,	/* create a subwindow */
    	termdispcmd_cursor,	/* position the cursor */
    	termdispcmd_overlast
} ; /* end enum (termdispcmds) */

/* graphic renditions */
enum termdispgrs {
    	termdispgr_bold,	/* graphic-rendition bold */
    	termdispgr_under,	/* graphic-rendition underline */
    	termdispgr_blink,	/* graphic-rendition blinking */
    	termdispgr_rev,		/* graphic-rendition reverse-video */
    	termdispgr_overlast
} ; /* end enum (termdispgrs) */

struct termdispgrms {
    static constexpr int	bold	= (1 << termdispgr_bold) ;
    static constexpr int	under	= (1 << termdispgr_under) ;
    static constexpr int	blink	= (1 << termdispgr_blink) ;
    static constexpr int	rev	= (1 << termdispgr_rev) ;
} ; /* end enum (termdispgrs) */

struct termdisp_flags {
	uint		statusdisplay:1 ;
	uint		meol:1 ;	/* erase-to-EOL mode */
	uint		linebuf:1 ;	/* line buffer mode */
	uint		smallscroll:1 ;
	uint		nlcr:1 ;	/* NL-character display behavior */
} ; /* end struct (termdisp_flags) */

struct termdisp_position {
	uint		timecount ;
	int		row ;
	int		col ;
} ; /* end struct (termdisp_position) */

struct termdisp_window {
	TERMDISP_POS	move ;		/* move is outstanding */
	TERMDISP_POS	cur ;		/* current position */
	int		srow ;		/* starting absolute row */
	int		scol ;		/* starting absolute col */
	int		rows ;		/* rows */
	int		cols ;		/* cols */
} ; /* end struct (termdisp_window) */

struct termdisp_head {
	cchar		*termname ;	/* given terminal type-name */
	char		*buf ;		/* storage buffer */
	termstr		*tsp ;		/* term-string-pointer */
	vecitem		*wlp ;		/* window-list-pointer */
	TERMDISP_POS	cur ;		/* current position */
	TERMDISP_FL	fl ;
	uint		timecounter ;
	uint		magval ;
	int		tfd ;
	int		termcap ;	/* terminal capabilities */
	int		buflen ;	/* buffer extent */
	int		curlen ;	/* current buffer occupancy */
	int		rows ;
	int		cols ;
} ; /* end struct (termdisp_head) */

typedef TERMDISP	termdisp ;
typedef TERMDISP_FL	termdisp_fl ;
typedef TERMDISP_POS	termdisp_pos ;
typedef TERMDISP_WIN	termdisp_win ;

EXTERNC_begin

extern int termdisp_start	(termdisp *,int,cchar *,int,int) noex ;
extern int termdisp_finish	(termdisp *) noex ;
extern int termdisp_flush	(termdisp *) noex ;
extern int termdisp_subnew	(termdisp *,int,int,int,int) noex ;
extern int termdisp_subdel	(termdisp *,int) noex ;
extern int termdisp_getlines	(termdisp *,int) noex ;
extern int termdisp_setlines	(termdisp *,int,int) noex ;
extern int termdisp_move	(termdisp *,int,int,int) noex ;
extern int termdisp_scroll	(termdisp *,int,int) noex ;
extern int termdisp_control	(termdisp *,int,...) noex ;
extern int termdisp_setsize	(termdisp *,int,int) noex ;
extern int termdisp_suspend	(termdisp *,int,int) noex ;
extern int termdisp_write	(termdisp *,int,cchar *,int) noex ;
extern int termdisp_pwrite	(termdisp *,int,int,int,cchar *,int) noex ;
extern int termdisp_pwritegr	(termdisp *,int,int,int,int,cchar *,int) noex ;
extern int termdisp_vprintf	(termdisp *,int,cchar *,va_list) noex ;
extern int termdisp_vpprintf	(termdisp *,int,int,int,cchar *,va_list) noex ;
extern int termdisp_printf	(termdisp *,int,cchar *,...) noex ;
extern int termdisp_pprintf	(termdisp *,int,int,int,cchar *,...) noex ;
extern int termdisp_clear	(termdisp *,int) noex ;
extern int termdisp_ew		(termdisp *,int,int,int) noex ;
extern int termdisp_el		(termdisp *,int,int) noex ;
extern int termdisp_ec		(termdisp *,int,int) noex ;
extern int termdisp_check	(termdisp *) noex ;
extern int termdisp_getpos	(termdisp *,int,termdisp_pos *) noex ;

EXTERNC_end

#ifdef	__cplusplus
extern const termdispgrms	termdispgrm ;
#endif /* __cplusplus */


#endif /* TERMDISP_INCLUDE */


