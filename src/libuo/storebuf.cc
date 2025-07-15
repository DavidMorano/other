/* storebuf SUPPORT */
/* charset=ISO8859-1 */
/* lang=C++20 */

/* storage buffer manipulation subroutines */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-03-24, David A­D­ Morano
	This object module was originally written.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

  	Name:
	storebuf

	Description:
	This subroutine can be used to construct strings or messages
	in a buffer WITHOUT using the 'sprintf(3c)' subroutine.

	This module is useful when the user supplies a buffer of a
	specified length and does not want to track the creation
	and destruction of an associated object. There is NO object
	(besides the user supplied buffer -- which can be considered
	THE object) to create and then destroy when using this
	module.

	The user must carefully track the buffer usage so that
	subsequent calls can be supplied with the correct index
	value of the next available (unused) byte in the buffer.

	Example usage:

	rs = 0 ;
	i = 0 ;
	if (rs >= 0) {
	    rs = storebuf_strw(rbuf,rlen,i,sp,sl) ;
	    i += rs ;
	}

	if (rs >= 0) {
	    rs = storebuf_buf(rbuf,rlen,i,bp,bl) ;
	    i += rs ;
	}

	if (rs >= 0) {
	    rs = storebuf_deci(rbuf,rlen,i,value) ;
	    i += rs ;
	}

	if (rs >= 0) {
	    rs = storebuf_strw(rbuf,rlen,i,sp,sl) ;
	    i += rs ;
	}

*******************************************************************************/

#include	<envstandards.h>	/* MUST be first to configure */
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<cstring>		/* |memcpy(3c)| */
#include	<usystem.h>
#include	<ctbin.h>
#include	<ctoct.h>
#include	<ctdec.h>
#include	<cthex.h>
#include	<strwcpy.h>
#include	<localmisc.h>		/* |DIGBUFLEN| */

#include	"storebuf.h"


/* local defines */


/* external subroutines */


/* external variables */


/* local structures */


/* forward references */

template<typename T>
static inline 
int storebuf_xxxx(char *rp,int rl,int i,int (*ctxxx)(char *,int,T),T v) noex {
	cint		dlen = DIGBUFLEN ;
	int		rs = SR_FAULT ;
	int		len = 0 ;
	if (rp) ylikely {
	    rs = SR_INVALID ;
	    if (i >= 0) ylikely {
	        char	*bp = (rp + i) ;
		rs = SR_OK ;
	        *bp = '\0' ;
	        if ((rl < 0) || ((rl-i) >= dlen)) ylikely {
	            if ((rs = ctxxx(bp,(rl-i),v)) >= 0) ylikely {
		        len = rs ;
	            }
	        } else {
	            char	dbuf[dlen+1] ;
	            if ((rs = ctxxx(dbuf,dlen,v)) >= 0) ylikely {
	                len = rs ;
	                if ((rl < 0) || ((rl - i) >= len)) {
	                    strwcpy(bp,dbuf,len) ;
	                } else {
		            rs = SR_OVERFLOW ;
		        }
	            }
	        } /* end if */
	    } /* end if (valid) */
	} /* end if (non-null) */
	return (rs >= 0) ? len : rs ;
}
/* end subroutine-template (storebuf_xxxx) */

template<typename T>
static inline int storebuf_binx(char *bp,int bl,int i,T v) noex {
	return storebuf_xxxx(bp,bl,i,ctbin,v) ;
}
/* end subroutine-template (storebuf_binx) */

template<typename T>
static inline int storebuf_octx(char *bp,int bl,int i,T v) noex {
	return storebuf_xxxx(bp,bl,i,ctoct,v) ;
}
/* end subroutine-template (storebuf_octx) */

template<typename T>
static inline 
int storebuf_decx(char *bp,int bl,int i,T v) noex {
	return storebuf_xxxx(bp,bl,i,ctdec,v) ;
}
/* end subroutine-template (storebuf_decx) */

template<typename T>
static inline int storebuf_hexx(char *bp,int bl,int i,T v) noex {
	return storebuf_xxxx(bp,bl,i,cthex,v) ;
}
/* end subroutine-template (storebuf_hexx) */


/* local variables */


/* exported variables */


/* exported subroutines */

int storebuf_chrs(char *rbuf,int rlen,int idx,int ch,int n) noex {
	int		rs = SR_FAULT ;
	if (rbuf) ylikely {
	    rs = SR_INVALID ;
	    if ((idx >= 0) && (n >= 0)) ylikely {
	        char	*bp = (rbuf + idx) ;
		rs = SR_OK ;
	        if ((rlen < 0) || ((rlen - idx) >= n)) ylikely {
		    for (int i = 0 ; i < n ; i += 1) {
	                *bp++ = char(ch) ;
		    }
	        } else {
	            rs = SR_OVERFLOW ;
	        }
	        *bp = '\0' ;
	    } /* end if (valid) */
	} /* end if (non-null) */
	return (rs >= 0) ? 1 : rs ;
}
/* end subroutine (storebuf_chrs) */

int storebuf_chr(char *rbuf,int rlen,int i,int ch) noex {
	int		rs = SR_FAULT ;
	if (rbuf) ylikely {
	    rs = SR_INVALID ;
	    if (i >= 0) ylikely {
	        char	*bp = (rbuf + i) ;
		rs = SR_OK ;
	        if ((rlen < 0) || ((rlen - i) >= 1)) ylikely {
	            *bp++ = char(ch) ;
	        } else {
	            rs = SR_OVERFLOW ;
	        }
	        *bp = '\0' ;
	    } /* end if (valid) */
	} /* end if (non-null) */
	return (rs >= 0) ? 1 : rs ;
}
/* end subroutine (storebuf_chr) */

int storebuf_buf(char *rbuf,int rlen,int i,cchar *sp,int sl) noex {
	int		rs = SR_FAULT ;
	int		len = 0 ;
	if (rbuf && sp) ylikely {
	    rs = SR_INVALID ;
	    if (i >= 0) ylikely {
	        char	*bp = (rbuf + i) ;
		rs = SR_OK ;
	        if (rlen < 0) {
	            if (sl < 0) {
	                while (*sp) {
	                    *bp++ = *sp++ ;
	 	        }
	            } else {
	                memcpy(bp,sp,sl) ;
	                bp += sl ;
	            } /* end if */
	        } else {
	            if (sl < 0) {
	                while ((bp < (rbuf + rlen)) && *sp) {
	                    *bp++ = *sp++ ;
		        }
		        if ((bp == (rbuf + rlen)) && (*sp != '\0')) {
		            rs = SR_OVERFLOW ;
		        }
	            } else {
		        if ((rlen - i) >= sl) {
	                    memcpy(bp,sp,sl) ;
	                    bp += sl ;
		        } else {
		            rs = SR_OVERFLOW ;
			}
	            } /* end if */
	        } /* end if */
	        *bp = '\0' ;
		len = intconv(bp - (rbuf + i)) ;
	    } /* end if (valid) */
	} /* end if (non-null) */
	return (rs >= 0) ? len : rs ;
}
/* end subroutine (storebuf_buf) */

int storebuf_strw(char *rbuf,int rlen,int i,cchar *sp,int sl) noex {
	int		rs = SR_FAULT ;
	int		len = 0 ;
	if (rbuf && sp) {
 	    if (i >= 0) {	
	        char	*bp = (rbuf + i) ;
	        rs = SR_OK ;
	        if (rlen < 0) {
	            if (sl < 0) {
	                while (*sp) {
	                    *bp++ = *sp++ ;
		        }
	            } else {
	                while (sl && *sp) {
	                    *bp++ = *sp++ ;
	                    sl -= 1 ;
	                }
	            } /* end if */
	        } else {
	            if (sl < 0) {
	                while ((bp < (rbuf + rlen)) && *sp) {
	                    *bp++ = *sp++ ;
		        }
		        if ((bp == (rbuf + rlen)) && (*sp != '\0')) {
		            rs = SR_OVERFLOW ;
		        }
	            } else {
	                while ((bp < (rbuf + rlen)) && sl && *sp) {
	                    *bp++ = *sp++ ;
	                    sl -= 1 ;
	                }
		        if ((bp == (rbuf + rlen)) && sl) {
		            rs = SR_OVERFLOW ;
		        }
	            } /* end if */
	        } /* end if */
	        *bp = '\0' ;
		len = intconv(bp - (rbuf + i)) ;
	    } /* end if (valid) */
	} /* end if (non-null) */
	return (rs >= 0) ? len : rs ;
}
/* end subroutine (storebuf_strw) */

int storebuf_bini(char *rbuf,int rlen,int i,int v) noex {
	uint		uv = uint(v) ;
	return storebuf_binx(rbuf,rlen,i,uv) ;
}

int storebuf_binl(char *rbuf,int rlen,int i,long v) noex {
	ulong		uv = ulong(v) ;
	return storebuf_binx(rbuf,rlen,i,uv) ;
}

int storebuf_binll(char *rbuf,int rlen,int i,longlong v) noex {
	ulonglong	uv = ulonglong(v) ;
	return storebuf_binx(rbuf,rlen,i,uv) ;
}

int storebuf_binui(char *rbuf,int rlen,int i,uint uv) noex {
	return storebuf_binx(rbuf,rlen,i,uv) ;
}

int storebuf_binul(char *rbuf,int rlen,int i,ulong uv) noex {
	return storebuf_binx(rbuf,rlen,i,uv) ;
}

int storebuf_binull(char *rbuf,int rlen,int i,ulonglong uv) noex {
	return storebuf_binx(rbuf,rlen,i,uv) ;
}

int storebuf_octi(char *rbuf,int rlen,int i,int v) noex {
	uint		uv = uint(v) ;
	return storebuf_octx(rbuf,rlen,i,uv) ;
}

int storebuf_octl(char *rbuf,int rlen,int i,long v) noex {
	ulong		uv = ulong(v) ;
	return storebuf_octx(rbuf,rlen,i,uv) ;
}

int storebuf_octll(char *rbuf,int rlen,int i,longlong v) noex {
	ulonglong	uv = ulonglong(v) ;
	return storebuf_octx(rbuf,rlen,i,uv) ;
}

int storebuf_octui(char *rbuf,int rlen,int i,uint uv) noex {
	return storebuf_octx(rbuf,rlen,i,uv) ;
}

int storebuf_octul(char *rbuf,int rlen,int i,ulong uv) noex {
	return storebuf_octx(rbuf,rlen,i,uv) ;
}

int storebuf_octull(char *rbuf,int rlen,int i,ulonglong uv) noex {
	return storebuf_octx(rbuf,rlen,i,uv) ;
}

int storebuf_deci(char *rbuf,int rlen,int i,int v) noex {
	uint		uv = uint(v) ;
	return storebuf_decx(rbuf,rlen,i,uv) ;
}

int storebuf_decl(char *rbuf,int rlen,int i,long v) noex {
	ulong		uv = ulong(v) ;
	return storebuf_decx(rbuf,rlen,i,uv) ;
}

int storebuf_decll(char *rbuf,int rlen,int i,longlong v) noex {
	ulonglong	uv = ulonglong(v) ;
	return storebuf_decx(rbuf,rlen,i,uv) ;
}

int storebuf_decui(char *rbuf,int rlen,int i,uint uv) noex {
	return storebuf_decx(rbuf,rlen,i,uv) ;
}

int storebuf_decul(char *rbuf,int rlen,int i,ulong uv) noex {
	return storebuf_decx(rbuf,rlen,i,uv) ;
}

int storebuf_decull(char *rbuf,int rlen,int i,ulonglong uv) noex {
	return storebuf_decx(rbuf,rlen,i,uv) ;
}

int storebuf_hexi(char *rbuf,int rlen,int i,int v) noex {
	uint		uv = uint(v) ;
	return storebuf_hexx(rbuf,rlen,i,uv) ;
}

int storebuf_hexl(char *rbuf,int rlen,int i,long v) noex {
	ulong		uv = ulong(v) ;
	return storebuf_hexx(rbuf,rlen,i,uv) ;
}

int storebuf_hexll(char *rbuf,int rlen,int i,longlong v) noex {
	ulonglong	uv = ulonglong(v) ;
	return storebuf_hexx(rbuf,rlen,i,uv) ;
}

int storebuf_hexui(char *rbuf,int rlen,int i,uint uv) noex {
	return storebuf_hexx(rbuf,rlen,i,uv) ;
}

int storebuf_hexul(char *rbuf,int rlen,int i,ulong uv) noex {
	return storebuf_hexx(rbuf,rlen,i,uv) ;
}

int storebuf_hexull(char *rbuf,int rlen,int i,ulonglong uv) noex {
	return storebuf_hexx(rbuf,rlen,i,uv) ;
}


