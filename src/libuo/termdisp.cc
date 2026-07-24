/* termdisp SUPPORT (Terminal-Display) */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* Terminal-Display manager */
/* version %I% last-modified %G% */

#define	CF_SAVERESTORE	0		/* implementing cursor-save-restore? */

/* revision history:

	= 1999-08-01, Dave morano
	This object package is finally finished!

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

  	Object:
	termdisp

	Description:
	This object module facilitates managing a terminal display.
	This object contains routines (not unlike in the UNIX
	'curses' library) to display information in "windows" on a
	terminal display.  The major difference between this terminal
	display library and the standard UNIX 'curses' library is
	that THIS LIBRARY WORKS!

	Other notes:
	An 'xterm' terminal is almost en entire complete 'vt102'
	terminal.  It pretty much only lacks the blinking character
	attribute and the double height-width characters.

*******************************************************************************/

#include	<envstandards.h>	/* MUST be first to configure */
#include	<unistd.h>		/* POSIX® |isatty(3c)| */
#include	<cstddef>		/* CSTD */
#include	<cstdlib>		/* CSTD */
#include	<cstdarg>		/* CSTD */
#include	<algorithm>		/* C++STD |min(3c++)| * |max(3c++)| */
#include	<clanguage.h>		/* LIBU */
#include	<usysbase.h>		/* LIBU */
#include	<usyscalls.h>		/* LIBU */
#include	<ascii.h>		/* LIBU */
#include	<uclibmem.h>		/* LIBUC */
#include	<uctc.h>		/* LIBUC Terminal-Control */
#include	<vecitem.h>		/* LIBUC */
#include	<buffer.h>		/* LIBUC */
#include	<ansigr.h>		/* LIBUC */
#include	<snwcpy.h>		/* LIBUC */
#include	<strwcpy.h>		/* LIBUC */
#include	<termstr.h>		/* LIBUC */
#include	<termconseq.h>		/* LIBUC */
#include	<tabcols.h>		/* LIBUC */
#include	<bufprintf.h>		/* LIBUC */
#include	<localmisc.h>		/* LIBU |HEXBUFLEN| + |NTABCOLS| */

#include	"termdisp.h"

#pragma		GCC dependency		"mod/libutil.ccm"

import libutil ;			/* |memclear(3u)| + |lenstr(3u)| */

/* local defines */

#define	TD		termdisp
#define	TD_FL		termdisp_fl
#define	TD_POS		termdisp_pos
#define	TD_WIN		termdisp_win
#define	TD_MAGIC	TERMDISP_MAGIC
#define	TD_BUFLEN	(8 * 1024)

#define	TD_RESTORELEN	4
#define	TD_SCROLLBUFLEN	20

/* terminal capabilities */
#define	TD_TCSCROLL	(1 << 0)	/* supports scroll regions */
#define	TD_TCCOLOR	(1 << 1)	/* supports color */
#define	TD_TCEIGHT	(1 << 2)	/* 8-bit controls */
#define	TD_TCIL		(1 << 3)	/* insert line */

#define	TERMTYPE	termtype

#ifndef	TERMCONSEQLEN
#define	TERMCONSEQLEN	84
#endif

#undef	CBUFLEN
#define	CBUFLEN		((4*4)+3)

#ifndef	HEXBUFLEN
#define	HEXBUFLEN	100
#endif


/* imported namespaces */

using std::min ;			/* subroutine-template */
using std::max ;			/* subroutine-template */
using libuc::libmem ;			/* variable */
using std::nothrow ;			/* constant */


/* local typedefs */


/* external subroutines */

extern "C" {
    int	termdisp_vprintf	(TD *,int,cchar *,va_list) noex ;
    int	termdisp_vpprintf	(TD *,int,int,int,cchar *,va_list) noex ;
    int	termdisp_printf		(TD *,int,cchar *,...) noex ;
    int	termdisp_pprintf	(TD *,int,int,int,cchar *,...) noex ;
    int	termdisp_pwrite		(TD *,int,int,int,cchar *,int) noex ;
    int	termdisp_pwritegr	(TD *,int,int,int,int,cchar *,int) noex ;
    int	termdisp_suspend	(TD *,int,int) noex ;
} /* end extern */


/* external variables */


/* local structures */

enum erases {
	erase_disp,			/* display */
	erase_line,			/* line */
	erase_char,			/* character */
	erase_overlast
} ; /* end enum */

struct termtype {
	cchar		*term ;
	int		termcap ;	/* capabilities */
} ; /* end struct */


/* forward references */

template<typename ... Args>
local int termdisp_ctor(TD *op,Args ... args) noex {
    	TD		*hop = op ;
	cnullptr	np{} ;
	int		rs = SR_FAULT ;
	if (op && (args && ...)) ylikely {
	    rs = SR_NOMEM ;
	    memclear(hop) ; /* dangerous */
	    if ((op->tsp = new(nothrow) termstr) != np) ylikely {
	        if ((op->wlp = new(nothrow) vecitem) != np) ylikely {
		    rs = SR_OK ;
	        } /* end if (new-vecitem) */
		if (rs < 0) {
		    delete op->tsp ;
		    op->tsp = nullptr ;
		} /* end if (error) */
	    } /* end if (new-termstr) */
	} /* end if (non-null) */
	return rs ;
} /* end subroutine (termdisp_ctor) */

local int termdisp_dtor(TD *op) noex {
	int		rs = SR_FAULT ;
	if (op) ylikely {
	    rs = SR_OK ;
	    if (op->wlp) ylikely {
		delete op->wlp ;
		op->wlp = nullptr ;
	    }
	    if (op->tsp) ylikely {
		delete op->tsp;
		op->tsp = nullptr ;
	    }
	} /* end if (non-null) */
	return rs ;
} /* end subroutine (termdisp_dtor) */

template<typename ... Args>
local int termdisp_magic(TD *op,Args ... args) noex {
	int		rs = SR_FAULT ;
	if (op && (args && ...)) ylikely {
	    rs = (op->magval == TD_MAGIC) ? SR_OK : SR_NOTOPEN ;
	}
	return rs ;
} /* end subroutine (termdisp_magic) */

local int	termdisp_starter	(TD *,int,cchar *,int,int) noex ;
local int	termdisp_startwin	(TD *,int,int,int,int) noex ;
local int	termdisp_flushmove	(TD *,TD_WIN *,int,int) noex ;
local int	termdisp_iflush		(TD *) noex ;
local int	termdisp_procstr	(TD *,TD_WIN *,int,cchar *,int) noex ;
local int	termdisp_termstrbegin	(TD *) noex ;
local int	termdisp_termstrend	(TD *) noex ;
local int	termdisp_store		(TD *,cchar *,int) noex ;
local int	termdisp_erase		(TD *,int,int,int) noex ;

#if	CF_SAVERESTORE
local int	termdisp_isave(TD *) noex ;
local int	termdisp_irestore(TD *) noex ;
#endif /* CF_SAVERESTORE */

consteval int mkgrmask() noex {
    	int m = 0 ;
	m |= termdispgrm.bold ;
	m |= termdispgrm.under ;
	m |= termdispgrm.blink ;
	m |= termdispgrm.rev ;
	return m ;
} /* end subroutine (mkgrmask) */

#ifdef	COMMENT
local int	termmatch(const TERMTYPE *,cchar *) noex ;
#endif


/* local variables */

#ifdef	COMMENT
/* 	TYPE		CAPABILITY	*/
constexpr TERMTYPE	terms[] = {
	{ "vt100",	(TD_TCSCROLL) },
	{ "ansi",	(TD_TCSCROLL | TD_TCIL) },
	{ "xterm",	(TD_TCSCROLL | TD_TCIL | TD_TCCOLOR) },
	{ "screen",	(TD_TCSCROLL | TD_TCIL) },
	{ "vt220",	(TD_TCSCROLL | TD_TCIL | TD_TCEIGHT) },
	{ "vt240",	(TD_TCSCROLL | TD_TCIL | TD_TCEIGHT | TD_TCCOLOR) },
	{ "vt320",	(TD_TCSCROLL | TD_TCIL | TD_TCEIGHT) },
	{ "vt340",	(TD_TCSCROLL | TD_TCIL | TD_TCEIGHT | TD_TCCOLOR) },
	{ "vt420",	(TD_TCSCROLL | TD_TCIL | TD_TCEIGHT) },
	{ "vt440",	(TD_TCSCROLL | TD_TCIL | TD_TCEIGHT | TD_TCCOLOR) },
	{ "vt520",	(TD_TCSCROLL | TD_TCIL | TD_TCEIGHT) },
	{ "vt540",	(TD_TCSCROLL | TD_TCIL | TD_TCEIGHT | TD_TCCOLOR) },
	{ "vt101",	(TD_TCSCROLL | TD_TCIL) },
	{ "vt102",	(TD_TCSCROLL | TD_TCIL) },
	{ nullptr, 0 }
} ; /* end array */
#endif /* COMMENT */


/* exported variables */

const termdispgrms	termdispgrm ;
constexpr int		grmask		= mkgrmask() ;


/* exported subroutines */

int termdisp_start(TD *op,int tfd,cchar *termname,int r,int c) noex {
	int		rs ;
	if ((rs = termdisp_ctor(op,termname)) >= 0) ylikely {
	    rs = SR_BADF ;
	    if (tfd >= 0) ylikely {
		rs = SR_INVALID ;
	        if (termname[0] && (r >= 0) && (c >= 0)) {
	            if (ustat sb ; (rs = u_fstat(tfd,&sb)) >= 0) {
	                if (isatty(tfd)) {
		            rs = termdisp_starter(op,tfd,termname,r,c) ;
	                } else {
	                    rs = SR_NOTSOCK ;
	                }
	            } /* end if (stat) */
	        } /* end if (valid) */
	    } /* end if (ok-fd) */
	    if (rs < 0) {
		termdisp_dtor(op) ;
	    } /* end if (error) */
	} /* end if (termdisp_ctor) */
	return rs ;
} /* end subroutine (termdisp_start) */

int termdisp_finish(TD *op) noex {
	int		rs ;
	int		rs1 ;
	if ((rs = termdisp_magic(op)) >= 0) ylikely {
	    /* reset the scroll region if necessary */
	    {
	        rs1 = termdisp_suspend(op,-1,-1) ;
	        if (rs >= 0) rs = rs1 ;
	    }
	    /* do one final flush */
	    {
	        rs1 = termdisp_iflush(op) ;
	        if (rs >= 0) rs = rs1 ;
	    }
	    /* free the internal stuff */
	    if (op->wlp) ylikely {
	        rs1 = vecitem_finish(op->wlp) ;
	        if (rs >= 0) rs = rs1 ;
	    }
	    if (op->tsp) ylikely {
	        rs1 = termstr_finish(op->tsp) ;
	        if (rs >= 0) rs = rs1 ;
	    }
	    if (op->buf) ylikely {
	        rs1 = libmem.free(op->buf) ;
	        if (rs >= 0) rs = rs1 ;
	        op->buf = nullptr ;
	    } /* end if (memory-release) */
	    if (op->termname) ylikely {
		void *vp = voidp(op->termname) ;
	        rs1 = libmem.free(vp) ;
	        if (rs >= 0) rs = rs1 ;
	        op->termname = nullptr ;
	    } /* end if (memory-release) */
	    {
		rs1 = termdisp_dtor(op) ;
	        if (rs >= 0) rs = rs1 ;
	    }
	    op->magval = 0 ;
	} /* end if (magic) */
	return rs ;
} /* end subroutine (termdisp_finish) */

/* suspend the windowing object (like for escape or something like that) */
int termdisp_suspend(TD *op,int r,int c) noex {
	int		rs ;
	int		len = 0 ;
	if ((rs = termdisp_magic(op)) >= 0) ylikely {
	    termdisp_win	*wp ;
	    if ((rs = vecitem_get(op->wlp,0,&wp)) >= 0) ylikely {
	        if ((rs = termdisp_termstrbegin(op)) >= 0) ylikely {
	            if ((rs >= 0) && op->fl.smallscroll) {
	                op->fl.smallscroll = false ;
	                rs = termstr_ssr(op->tsp,0,op->rows) ;
	            }
	            if (rs >= 0) ylikely {
	                rs = termdisp_flushmove(op,wp,r,c) ;
		    }
	            if (rs >= 0) ylikely {
	                rs = termdisp_termstrend(op) ;
	                len = rs ;
	            }
	        } /* end if (transaction) */
	        if (rs >= 0) rs = termdisp_iflush(op) ;
	    } /* end if (getting window) */
	} /* end if (magic) */
	return (rs >= 0) ? len : rs ;
} /* end subroutine (termdisp_suspend) */

/* create a new subwindow */
int termdisp_subnew(TD *op,int srow,int scol,int rows,int cols) noex {
	int		rs ;
	if ((rs = termdisp_magic(op)) >= 0) ylikely {
	    bool	f = true ;
	    rs = SR_INVALID ;
	    f = f && (srow >= 0) && (scol >= 0) ;
	    f = f && (rows >= 0) && (cols >= 0) ;
	    if (f) {
	        rs = termdisp_startwin(op,srow,scol,rows,cols) ;
	    } /* end if (valid) */
	} /* end if (magic) */
	return rs ;
} /* end subroutine (termdisp_subnew) */

/* delete a subwindow */
int termdisp_subdel(TD *op,int wn) noex {
	int		rs ;
	if ((rs = termdisp_magic(op)) >= 0) ylikely {
	    rs = SR_INVALID ;	/* cannot delete win==0 */
	    if (wn != 0) {
	        if ((rs = termdisp_iflush(op)) >= 0) ylikely {
	            if (wn < 0) {
			vecitem	*wlp = op->wlp ;
			auto	vg = vecitem_get ;
	                termdisp_win	*wp ;
	                for (int i = 1 ; vg(wlp,i,&wp) >= 0 ; i += 1) {
	                    if (wp) {
	                        rs = vecitem_del(op->wlp,i) ;
	                        if (rs < 0) break ;
		            }
	                } /* end for */
	            } else {
	                rs = vecitem_del(op->wlp,wn) ;
	            }
	        } /* end if */
	    } /* end if (valid) */
	} /* end if (magic) */
	return rs ;
} /* end subroutine (termdisp_subdel) */

int termdisp_getlines(TD *op,int w) noex {
	int		rs ;
	int		lines = 0 ;
	if ((rs = termdisp_magic(op)) >= 0) ylikely {
	    termdisp_win	*wp ;
	    if ((rs = vecitem_get(op->wlp,w,&wp)) >= 0) ylikely {
	        lines = wp->rows ;
	    }
	} /* end if (magic) */
	return (rs >= 0) ? lines : rs ;
} /* end subroutine (termdisp_getlines) */

int termdisp_setlines(TD *op,int w,int nlines) noex {
	int		rs ;
	int		olines = 0 ;
	if ((rs = termdisp_magic(op)) >= 0) ylikely {
	    termdisp_win	*wp ;
	    if ((rs = vecitem_get(op->wlp,w,&wp)) >= 0) ylikely {
	        olines = wp->rows ;
	        wp->rows = nlines ;
	    }
	} /* end if (magic) */
	return (rs >= 0) ? olines : rs ;
} /* end subroutine (termdisp_setlines) */

/* execute a control action */
int termdisp_control(TD *op,int cmd,...) noex {
	int		rs ;
	if ((rs = termdisp_magic(op)) >= 0) ylikely {
	    switch (cmd) {
	    case termdispcmd_cursor:
	    case termdispcmd_eol:
	        break ;
	    default:
	        rs = SR_INVALID ;
	        break ;
	    } /* end switch */
	} /* end if (magic) */
	return rs ;
} /* end subroutine (termdisp_control) */

/* make a cursor screen move */
int termdisp_move(TD *op,int wn,int r,int c) noex {
	int		rs ;
	int		f = false ;
	if ((rs = termdisp_magic(op)) >= 0) ylikely {
	    if ((r >= 0) || (c >= 0)) {
	        termdisp_win	*wp ;
	        if ((rs = vecitem_get(op->wlp,wn,&wp)) >= 0) ylikely {
	            if ((r < wp->rows) && (c < wp->cols)) {
	                if (r >= 0) {
	                    f = true ;
	                    wp->move.row = r ;
	                }
	                if (c >= 0) {
	                    f = true ;
	                    wp->move.col = c ;
	                }
	                if (f) {
	                    wp->move.timecount = op->timecounter++ ;
		        }
	            } /* end if (ok) */
	        } /* end if (get-window-pointer) */
	    } /* end if (need something) */
	} /* end if (magic) */
	return (rs >= 0) ? f : rs ;
} /* end subroutine (termdisp_move) */

/* scroll a window ( (n>0)==UP, (n<0)==DOWN ) */
int termdisp_scroll(TD *op,int wn,int n) noex {
	int		rs ;
	if ((rs = termdisp_magic(op)) >= 0) ylikely {
	    if (n != 0) {
	        termdisp_win	*wp ;
	        if ((rs = vecitem_get(op->wlp,wn,&wp)) >= 0) ylikely {
	            int		na = abs(n) ;
	            char	index_chars[8] ;
		    /* prepare a move for after the scroll */
	            op->cur.row = -1 ; /* unknown cursor location */
	            op->cur.col = -1 ; /* unknown cursor location */
	            if (na > wp->rows) {
	                na = wp->rows ;
		    }
	            if ((rs = termdisp_termstrbegin(op)) >= 0) ylikely {
	                termstr	*tsp = op->tsp ;
	                int	index_line ;
	                if ((rs = termstr_ssr(tsp,wp->srow,wp->rows)) >= 0) {
			    /* move to the boundary of the scroll region */
	                    if (n >= 0) {
				cchar	*ts_ind = "\033D" ; /* index */
	                        index_line = (wp->srow + wp->rows - 1) ;
	                        strcpy(index_chars,ts_ind) ;	/* UP */
	                    } else {
				cchar	*ts_ri = "\033M" ; /* reverse-index */
	                        index_line = wp->srow ;
	                        strcpy(index_chars,ts_ri) ;	/* DOWN */
	                    }
	                } /* end if */
	                if (rs >= 0) ylikely {
	                    if ((rs = termstr_curh(tsp,index_line,-1)) >= 0) {
	                        for (int i = 0 ; (i < na) ; i += 1) {
	                            rs = termstr_write(tsp,index_chars,-1) ;
				    if (rs < 0) break ;
	                        } /* end for */
	                        if (rs >= 0) {
				    cint	r = op->rows ;
	                            if ((rs = termstr_ssr(tsp,0,r)) >= 0) {
	                                rs = termdisp_termstrend(op) ;
	                            }
	                        }
	                    } /* end if (termstr_curh) */
	                } /* end if (ok) */
	            } /* end if (transaction) */
	        } /* end if (vecitem_get) */
	    } /* end if (not-zero) */
	} /* end if (magic) */
	return rs ;
} /* end subroutine (termdisp_scroll) */

/* position print-formatted */
int termdisp_pprintf(TD *op,int wn,int r,int c,cchar *fmt,...) noex {
	va_list		ap ;
	int		rs ;
	int		len = 0 ;
	if ((rs = termdisp_magic(op,fmt)) >= 0) ylikely {
	    va_begin(ap,fmt) ;
	    rs = termdisp_vpprintf(op,wn,r,c,fmt,ap) ;
	    len = rs ;
	    va_end(ap) ;
	} /* end if (magic) */
	return (rs >= 0) ? len : rs ;
} /* end subroutine (termdisp_pprintf) */

/* regular print-formatted */
int termdisp_printf(TD *op,int wn,cchar *fmt,...) noex {
	va_list		ap ;
	int		rs ;
	int		len = 0 ;
	if ((rs = termdisp_magic(op,fmt)) >= 0) ylikely {
	    va_begin(ap,fmt) ;
	    rs = termdisp_vpprintf(op,wn,-1,-1,fmt,ap) ;
	    len = rs ;
	    va_end(ap) ;
	} /* end if (magic) */
	return (rs >= 0) ? len : rs ;
} /* end subroutine (termdisp_printf) */

int termdisp_vprintf(TD *op,int wn,cchar *fmt,va_list ap) noex {
	return termdisp_vpprintf(op,wn,-1,-1,fmt,ap) ;
} /* end subroutine (termdisp_vprintf) */

/* vprintf-like thing */
int termdisp_vpprintf(TD *op,int wn,int r,int c,cchar *fmt,va_list ap) noex {
	int		rs ;
	int		rs1 ;
	int		len = 0 ;
	if ((rs = termdisp_magic(op,fmt)) >= 0) ylikely {
	    if (char *lbuf ; (rs = lm_ml(&lbuf)) >= 0) ylikely {
		cint	llen = rs ;
	        if ((rs = bufvprintf(lbuf,llen,fmt,ap)) > 0) ylikely {
	            rs = termdisp_pwrite(op,wn,r,c,lbuf,rs) ;
		    len = rs ;
	        }
		rs1 = lm_free(lbuf) ;
		if (rs >= 0) rs = rs1 ;
	    } /* end if (m-a-f) */
	} /* end if (magic) */
	return (rs >= 0) ? len : rs ;
} /* end subroutine (termdisp_vpprintf) */

int termdisp_write(TD *op,int wn,cchar *wbuf,int wlen) noex {
	return termdisp_pwrite(op,wn,-1,-1,wbuf,wlen) ;
} /* end subroutine (termdisp_write) */

/* position-write */
int termdisp_pwrite(TD *op,int wn,int r,int c,cchar *wbuf,int wlen) noex {
	return termdisp_pwritegr(op,wn,r,c,0,wbuf,wlen) ;
} /* end subroutine (termdisp_pwrite) */

/* position-write w/ graphic-rendition */
int termdisp_pwritegr(TD *op,int wn,int r,int c,int gr,cc *wbuf,int wlen) noex {
	int		rs ;
	int		len = 0 ;
	if ((rs = termdisp_magic(op,wbuf)) >= 0) ylikely {
	    rs = SR_INVALID ;
	    if (wlen < 0) wlen = lenstr(wbuf) ;
	    if (wn >= 0) {
	        termdisp_win	*wp ;
	        if ((rs = vecitem_get(op->wlp,wn,&wp)) >= 0) ylikely {
	            cint	nrow = (r >= 0) ? r : wp->move.row ;
	            cint	ncol = (c >= 0) ? c : wp->move.col ;
	            if ((nrow < wp->rows) && (ncol < wp->cols)) {
	                if ((rs = termdisp_termstrbegin(op)) >= 0) {
			    cauto td_fm = termdisp_flushmove ;
	                    if ((rs = td_fm(op,wp,nrow,ncol)) >= 0) {
	                        rs = termdisp_procstr(op,wp,gr,wbuf,wlen) ;
	                        if (rs >= 0) {
	                            rs = termdisp_termstrend(op) ;
	                            len = rs ;
				} /* end if (ok) */
	                    }
	                } /* end if (transaction) */
	            } /* end if (ok) */
	        } /* end if (get-window-pointer) */
	    } /* end if (valid) */
	} /* end if (magic) */
	return (rs >= 0) ? len : rs ;
} /* end subroutine (termdisp_pwritegr) */

/* clear a window */
int termdisp_clear(TD *op,int w) noex {
	int		rs ;
	int		len = 0 ;
	if ((rs = termdisp_magic(op)) >= 0) ylikely {
	    termdisp_win	*wp ;
	    if ((rs = vecitem_get(op->wlp,w,&wp)) >= 0) ylikely {
	        if ((rs = termdisp_termstrbegin(op)) >= 0) ylikely {
	            if ((rs = termdisp_flushmove(op,wp,-1,-1)) >= 0) ylikely {
	                if (w == 0) {
	                    rs = termstr_ed(op->tsp,2) ;
	                } else {
	                    for (int i = 0 ; i < wp->rows ; i += 1) {
	                        if ((rs = termstr_el(op->tsp,2)) >= 0) {
	                            rs = termstr_char(op->tsp,'\n') ;
			        }
			        if (rs < 0) break ;
	                    } /* end for */
	                } /* end if */
	            } /* end if (flushmove) */
	            if (rs >= 0) {
	                rs = termdisp_termstrend(op) ;
	                len = rs ;
	            }
	        } /* end if (transaction) */
	    } /* end if (get-window-pointer) */
	} /* end if (magic) */
	return (rs >= 0) ? len : rs ;
} /* end subroutine (termdisp_clear) */

/* erase-window; type: 0=forward, 1=back, 2=whole */
int termdisp_ew(TD *op,int w,int r,int type) noex {
	int		rs ;
	int		len = 0 ;
	if ((rs = termdisp_magic(op)) >= 0) ylikely {
	    termdisp_win	*wp ;
	    if ((rs = vecitem_get(op->wlp,w,&wp)) >= 0) ylikely {
	        if ((rs = termdisp_termstrbegin(op)) >= 0) {
	            if ((rs = termdisp_flushmove(op,wp,r,-1)) >= 0) {
			rs = termstr_ed(op->tsp,type) ;
		    } /* end if */
	            if (rs >= 0) ylikely {
	                rs = termdisp_termstrend(op) ;
	                len = rs ;
	            }
	        } /* end if (transaction) */
	    } /* end if (get-window-pointer) */
	} /* end if (magic) */
	return (rs >= 0) ? len : rs ;
} /* end subroutine (termdisp_ew) */

/* erase-line */
int termdisp_el(TD *op,int w,int type) noex {
	cint		item = erase_line ;
	return termdisp_erase(op,w,item,type) ;
} /* end subroutine (termdisp_el) */

/* erase-character */
int termdisp_ec(TD *op,int w,int n) noex {
	cint		item = erase_char ;
	return termdisp_erase(op,w,item,n) ;
} /* end subroutine (termdisp_ec) */

/* flush buffer */
int termdisp_flush(TD *op) noex {
	int		rs ;
	if ((rs = termdisp_magic(op)) >= 0) ylikely {
	    rs = termdisp_iflush(op) ;
	} /* end if (magic) */
	return rs ;
} /* end subroutine (termdisp_flush) */

/* terminal window size change */
int termdisp_setsize(TD *op,int rows,int cols) noex {
	int		rs ;
	if ((rs = termdisp_magic(op)) >= 0) ylikely {
	    rs = SR_INVALID ;
	    if ((rows >= 0) && (cols >= 0)) {
	        termdisp_win	*wp ;
	        if ((rs = vecitem_get(op->wlp,0,&wp)) >= 0) ylikely {
	            wp->rows = rows ;
	            wp->cols = cols ;
	        } /* end if (get-window-pointer) */
	    } /* end if (valid) */
	} /* end if (magic) */
	return rs ;
} /* end subroutine (termdisp_setsize) */

/* check */
int termdisp_check(TD *op) noex {
	int		rs ;
	if ((rs = termdisp_magic(op)) >= 0) ylikely {
	    if ((rs = termdisp_flushmove(op,0,-1,-1)) >= 0) ylikely {
	        rs = termdisp_iflush(op) ;
	    }
	} /* end if (magic) */
	return rs ;
} /* end subroutine (termdisp_check) */

int termdisp_getpos(TD *op,int wn,TD_POS *pp) noex {
	int		rs ;
	if ((rs = termdisp_magic(op,pp)) >= 0) ylikely {
	    termdisp_win	*wp ;
	    memclear(pp) ;
	    if ((rs = vecitem_get(op->wlp,wn,&wp)) >= 0) ylikely {
	        pp->timecount = wp->cur.timecount ;
	        pp->row = wp->cur.row ;
	        pp->col = wp->cur.col ;
	    } /* end if (get-window-pointer) */
	} /* end if (magic) */
	return rs ;
} /* end subroutine (termdisp_position) */


/* private subroutines */

local int termdisp_starter(TD *op,int tfd,cchar *termname,int r,int c) noex {
        TERMIOS         termconf{} ;
        int             rs ;
        op->tfd = tfd ;
        op->termcap = 0 ;
        op->buflen = TD_BUFLEN ;
        op->curlen = 0 ;
        op->rows = r ;
        op->cols = c ;
        op->fl.statusdisplay = false ;
        op->fl.meol = false ;
        op->fl.linebuf = false ;
        if ((rs = uc_tcattrget(tfd,&termconf)) >= 0) ylikely {
            cchar       *cp ;
            op->fl.nlcr = !!(termconf.c_oflag & ONLCR) ;
            if ((rs = libmem.strw(termname,-1,&cp)) >= 0) ylikely {
                char    *bp ;
                op->termname = cp ;
                if ((rs = libmem.mall(op->buflen,&bp)) >= 0) ylikely {
                    cchar       *tt = termname ;
                    op->buf = bp ;
                    if ((rs = termstr_start(op->tsp,tt)) >= 0) ylikely {
                        vecitem *wlp = op->wlp ;
                        cint    vo = 0 ;
                        if ((rs = vecitem_start(wlp,5,vo)) >= 0) ylikely {
                            if ((rs = termdisp_startwin(op,0,0,r,c)) >= 0) {
                                op->cur.row = -1 ;
                                op->cur.col = -1 ;
                                op->magval = TD_MAGIC ;
                            }
                            if (rs < 0) {
                                vecitem_finish(wlp) ;
                            }
                        } /* end if (vecitem_start) */
                        if (rs < 0) {
                            termstr_finish(op->tsp) ;
                        }
                    } /* end if (termstr_start) */
                    if (rs < 0) {
                        libmem.free(op->buf) ;
                        op->buf = nullptr ;
                    }
                } /* end if (m-a) */
                if (rs < 0) {
		    void *vp = voidp(op->termname) ;
                    libmem.free(vp) ;
                    op->termname = nullptr ;
                }
            } /* end if (m-a) */
        } /* end if (tcgetattr) */
        return rs ;
} /* end subroutine (termdisp_starter) */

/* create and initialize a new sub-window */
local int termdisp_startwin(TD *op,int srow,int scol,int rows,int cols) noex {
	int		rs = SR_OK ;
	bool		f = false ;
	f = f || (srow >= op->rows) ;
	f = f || ((srow + rows) > op->rows) ;
	f = f || (scol >= op->cols) ;
	f = f || ((scol + cols) > op->cols) ;
	if (f) {
	    rs = SR_DOM ;
	} else {
	    cint	esz = szof(termdisp_win) ;
	    termdisp_win	win{} ;
	    win.srow = min(srow,(op->rows-1)) ;
	    win.scol = min(scol,(op->cols-1)) ;
	    win.rows = min(rows,op->rows) ;
	    win.cols = min(cols,op->cols) ;
	    win.move.row = -1 ;
	    win.move.col = -1 ;
	    rs = vecitem_add(op->wlp,&win,esz) ;
	} /* end if (ok) */
	return rs ;
} /* end subroutine (termdisp_startwin) */

/* flush buffer */
local int termdisp_iflush(TD *op) noex {
	termdisp_win		*wp ;
	termdisp_win		*mwp = nullptr ;
	uint		mtimecount = 0 ;
	int		rs = SR_OK ;
	for (int i = 0 ; vecitem_get(op->wlp,i,&wp) >= 0 ; i += 1) {
	    if (wp) {
	        if ((wp->move.row >= 0) || (wp->move.col >= 0)) {
	            if (wp->move.timecount > mtimecount) {
	                mtimecount = wp->move.timecount ;
	                mwp = wp ;
	            }
		}
	    } /* end if (non-null) */
	} /* end for */
	if (mwp != nullptr) {
	    rs = termdisp_flushmove(op,mwp,-1,-1) ;
	}
	if ((rs >= 0) && (op->curlen > 0)) {
	    rs = u_write(op->tfd,op->buf,op->curlen) ;
	    op->curlen = 0 ;
	}
	return rs ;
} /* end subroutine (termdisp_iflush) */

#if	CF_SAVERESTORE

/* set the save-cursor stuff in the buffer */
local int termdisp_isave(TD *op) noex {
	int		rs ;
	int		len = 0 ;
	int		rv = 0 ;
#ifdef	COMMENT
	rs = termstr_csr(op->tsp,true) ;
	if (rs >= 0) {
	    rs = termdisp_istore(op) ;
	}
	rv = (rs >= 0) ? len : rs ;
#else
	cp = strwcpy(op->buf,TERMSTR_RESTORE,-1) ;
	op->curlen = (cp - op->buf) ;
	rv = op->curlen ;
#endif /* COMMENT */
	return rv ;
} /* end subroutine (termdisp_isave) */

/* set the restore-cursor stuff in the buffer */
local int termdisp_irestore(TD *op) noex {
	cchar		*cp = strwcpy(op->buf,TERMSTR_RESTORE,-1) ;
	op->curlen = (cp - op->buf) ;
	return op->curlen ;
} /* end subroutine (termdisp_irestore) */

#endif /* CF_SAVERESTORE */

/* flush out any accumulated moves */
local int termdisp_flushmove(TD *op,TD_WIN *wp,int r,int c) noex {
	int		rs = SR_OK ;
	int		nrow = (r >= 0) ? r : wp->move.row ;
	int		ncol = (c >= 0) ? c : wp->move.col ;
	if ((nrow < wp->rows) && (ncol < wp->cols)) {
	    int		wrow = (nrow >= 0) ? nrow : wp->cur.row ;
	    int		wcol = (ncol >= 0) ? ncol : wp->cur.col ;
	    if ((wrow < wp->rows) && (wcol < wp->cols)) {
	        int	arow = wrow + wp->srow ;
	        int	acol = wcol + wp->scol ;
	        bool	f_chosen = false ;
		/* relocate to absolute coordinates */
		/* search for optimizations */
	        if ((op->cur.row >= 0) && (op->cur.col >= 0)) {
	            if ((arow == op->cur.row) && (acol == op->cur.col)) {
	                f_chosen = true ;
	            }
	            if ((! f_chosen) && 
	                (arow == op->cur.row) && (acol == 0)) {
	                f_chosen = true ;
	                rs = termstr_char(op->tsp,'\r') ;
	            }
	            if ((! f_chosen) && 
	                (arow == op->cur.row) && 
	                (op->cur.col > 0) && (acol == (op->cur.col-1))) {
	                f_chosen = true ;
	                rs = termstr_char(op->tsp,'\b') ;
	            }
	            if ((! f_chosen) && 
	                (acol == op->cur.col) && (arow == (op->cur.row+1))) {
	                f_chosen = true ;
#ifdef	COMMENT
	                rs = termstr_char(op->tsp,CH_IND) ;
#else
	                rs = termstr_write(op->tsp,"\033D",2) ;
#endif

	            }
	        } /* end if (optimization) */
	        if (! f_chosen) {
	            f_chosen = true ;
	            rs = termstr_curh(op->tsp,arow,acol) ;
	        } /* end if */
	        if (rs >= 0) {
	            wp->cur.row = wrow ;
	            wp->cur.col = wcol ;
	            op->cur.row = arow ;
	            op->cur.col = acol ;
	            wp->move.row = -1 ;
	            wp->move.col = -1 ;
	        }
	    } /* end if (needed) */
	} /* end if (needed) */
	return rs ;
} /* end subroutine (termdisp_flushmove) */

/* process the special C-language string-escape characters */
local int termdisp_procstr(TD *op,TD_WIN *wp,int gr,cchar *sbuf,int slen) noex {
	cint		clen = CBUFLEN ;
	int		rs = SR_OK ;
	int		sl ;
	int		cl ;
	int		ch ;
	int		irows, icols ;
	int		len = 0 ;
	cchar		*sp ;
	char		cbuf[CBUFLEN+1] ;
	bool		f_cr = false ;
	bool		f_inrow = true ;
	bool		f_incol = true ;
	bool		f_gr = false ;
	sp = sbuf ;
	sl = slen ;
	if (sl < 0) sl = lenstr(sp) ;
	if ((sl > 0) && (gr & grmask)) {
	    cint	a1 = (gr & termdispgrm.bold) ? ANSIGR_BOLD : -1 ;
	    cint	a2 = (gr & termdispgrm.under) ? ANSIGR_UNDER : -1 ;
	    cint	a3 = (gr & termdispgrm.blink) ? ANSIGR_BLINK : -1 ;
	    cint	a4 = (gr & termdispgrm.rev) ? ANSIGR_REV : -1 ;
	    f_gr = true ;
	    if ((rs = termconseq(cbuf,clen,'m',a1,a2,a3,a4)) >= 0) {
	        cl = rs ;
	        rs = termstr_write(op->tsp,cbuf,cl) ;
	        len += rs ;
	    }
	} /* end if */
	while ((rs >= 0) && (sl > 0) && *sp) {
	    f_cr = false ;
	    irows = 0 ;
	    icols = 0 ;
	    f_inrow = (wp->cur.row < wp->rows) ;
	    f_incol = (wp->cur.col < wp->cols) ;
	    ch = (*sp & 0xff) ;
	    switch (ch) {
	    case '\v':
	        if (f_inrow && f_incol) {
	            rs = termstr_el(op->tsp,-1) ;
	            len += rs ;
	        }
	        break ;
	    case '\f':
	        if (f_inrow && f_incol) {
	            rs = termstr_ed(op->tsp,-1) ;
	            len += rs ;
	        }
	        break ;
	    case '\t':
	        if (f_inrow && f_incol) {
	            icols = tabcols(NTABCOLS,op->cur.col) ;
	            rs = termstr_char(op->tsp,'\t') ;
	            len += rs ;
	        }
	        break ;
	    case CH_CSI:
	    case CH_ESC:
	    case CH_BEL:
	    case CH_SO:
	    case CH_SI:
	    case CH_SS2:
	    case CH_SS3:
	        if (f_inrow && f_incol) {
	            rs = termstr_char(op->tsp,ch) ;
	            len += rs ;
	        }
	        break ;
	    case CH_DEL:
	        break ;
	    case '\r':
	        if (f_inrow) {
	            f_cr = true ;
	            rs = termstr_char(op->tsp,ch) ;
	            len += rs ;
	        }
	        break ;
	    case '\n':
	        if (f_inrow) {
	            irows = 1 ;
	            f_cr = op->fl.nlcr ;
	            if (! f_cr) {
	                rs = termstr_char(op->tsp,'\r') ;
	                len += rs ;
	            }
	            if ((rs >= 0) && (wp->cur.row < (wp->rows-1))) {
	                rs = termstr_char(op->tsp,ch) ;
	                len += rs ;
	            }
	        }
	        break ;
	    case CH_IND:
	        if (f_inrow) {
	            irows = 1 ;
	            if (wp->cur.row < (wp->rows-1)) {
	                rs = termstr_char(op->tsp,ch) ;
	                len += rs ;
	            }
	        }
	        break ;
	    case '\b':
	        if (wp->cur.col > 0) {
	            f_incol = (op->cur.col < op->cols) ;
	            if (f_incol) {
	                op->cur.col -= 1 ;
	                rs = termstr_char(op->tsp,ch) ;
	                len += rs ;
	            }
	            wp->cur.col -= 1 ;
	        }
	        break ;
	    default:
	        if (f_inrow && ((ch & 0x7f) >= 0x20)) {
	            icols = 1 ;
	            if (f_incol) {
	                rs = termstr_char(op->tsp,ch) ;
	                len += rs ;
	            }
	        }
	        break ;
	    } /* end switch */
	    if (rs >= 0) {
	        sp += 1 ;
	        sl -= 1 ;
	        if (irows > 0) {
	            int	n, brows = irows ;
	            if ((wp->cur.row + irows) > (wp->rows - 1)) {
	                n = (wp->cur.row + irows) - (wp->rows - 1) ;
	                brows -= n ;
	            }
	            wp->cur.row += irows ;
	            op->cur.row = min((op->cur.row + brows),(op->rows-1)) ;
	        }
	        if (f_cr) {
	            f_cr = false ;
	            wp->cur.col = 0 ;
	            op->cur.col = 0 ;
	        } else if (icols > 0) {
	            int	n, bcols = icols ;
	            if ((wp->cur.col + icols) > (wp->cols - 1)) {
	                n = (wp->cur.col + icols) - (wp->cols - 1) ;
	                bcols -= n ;
	            }
	            wp->cur.col += icols ;
	            op->cur.col = min((op->cur.col + bcols),(op->cols-1)) ;
	        }
	    } /* end if */
	} /* end while */
	if ((rs >= 0) && f_gr) {
	    int	a1 = ANSIGR_OFFALL ;
	    rs = termconseq(cbuf,clen,'m',a1,-1,-1,-1) ;
	    cl = rs ;
	    if (rs >= 0) {
	        rs = termstr_write(op->tsp,cbuf,cl) ;
	        len += rs ;
	    }
	} /* end if */
	return (rs >= 0) ? len : rs ;
} /* end subroutine (termdisp_procstr) */

local int termdisp_termstrbegin(TD *op) noex {
	int		rs ;
	rs = termstr_clean(op->tsp) ; /* clear any buffered actions */
	return rs ;
} /* end subroutine (termdisp_termstrbegin) */

local int termdisp_termstrend(TD *op) noex {
	int		rs ;
	int		len = 0 ;
	if (cchar *bp ; (rs = termstr_get(op->tsp,&bp)) >= 0) ylikely {
	    cint	bl = rs ;
	    rs = termdisp_store(op,bp,bl) ;
	    len = rs ;
	} /* end if */
	return (rs >= 0) ? len : rs ;
} /* end subroutine (termdisp_termstrend) */

local int termdisp_store(TD *op,cchar *bp,int µbl) noex {
	int		rlen = (op->buflen - op->curlen) ;
	int		rs = SR_OK ;
	int		len = 0 ; /* return-value */
	if (int bl ; (bl = getlenstr(bp,µbl)) >= 0) {
	    if (bl > rlen) {
	        rs = u_write(op->tfd,op->buf,op->curlen) ;
	        op->curlen = 0 ;
	    }
	    if ((rs >= 0) && (bl > 0)) {
	        if (bl > op->buflen) {
	            rs = u_write(op->tfd,bp,bl) ;
	            len += rs ;
	        } else {
	            rlen = (op->buflen - op->curlen) ;
	            rs = snwcpy((op->buf + op->curlen),rlen,bp,bl) ;
	            if (rs >= 0) op->curlen += rs ;
	            len += rs ;
	        }
	    } /* end if */
	} /* end if (getlenstr) */
	return (rs >= 0) ? len : rs ;
} /* end subroutine (termdisp_store) */

local int termdisp_erase(TD *op,int w,int item,int type) noex {
	int		rs ;
	int		len = 0 ;
	if ((rs = termdisp_magic(op)) >= 0) ylikely {
	    termdisp_win	*wp ;
	    if ((rs = vecitem_get(op->wlp,w,&wp)) >= 0) ylikely {
	        if ((rs = termdisp_termstrbegin(op)) >= 0) ylikely {
	            if ((rs = termdisp_flushmove(op,wp,-1,-1)) >= 0) ylikely {
			switch (item) {
			case erase_disp:
	                    rs = termstr_ed(op->tsp,type) ;
			    break ;
			case erase_line:
	                    rs = termstr_el(op->tsp,type) ;
			    break ;
			case erase_char:
	                    rs = termstr_ec(op->tsp,type) ;
			    break ;
			} /* end switch */
	                if (rs >= 0) {
	                    rs = termdisp_termstrend(op) ;
	                    len = rs ;
	                }
		    } /* end if */
	        } /* end if (transaction) */
	    } /* end if (get-window-pointer) */
	} /* end if (magic) */
	return (rs >= 0) ? len : rs ;
} /* end subroutine (termdisp_erase) */

#ifdef	COMMENT

/* match a terminal name with our list */
local int termmatch(const TERMTYPE *tlist,cchar *term) noex {
	int	i ; /* used-afterwards */
	for (i = 0 ; tlist[i].term != nullptr ; i += 1) {
	    if (strcmp(term,tlist[i].term) == 0) break ;
	} /* end for */
	return (tlist[i].term != nullptr) ? i : -1 ;
} /* end subroutine (termmatch) */

#endif /* COMMENT */


