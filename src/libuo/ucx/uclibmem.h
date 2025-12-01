/* uclibmem HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* memory allocation facility (for library use) */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-11-01, David A­D­ Morano
	This subroutine was written for Rightcore Network Services.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	UCLIBMEM_INCLUDE
#define	UCLIBMEM_INCLUDE


#include	<envstandards.h>	/* ordered first to configure */
#include	<clanguage.h>
#include	<utypedefs.h>
#include	<utypealiases.h>
#include	<usysdefs.h>
#include	<usysrets.h>


EXTERNC_begin

extern int uc_libmemallocstrw(cchar *,int,cchar **) noex ;
extern int uc_libmemallocitem(cvoid *,int,voidpp) noex ;
extern int uc_libmemalloc(int,void *) noex ;
extern int uc_libmemvalloc(int,void *) noex ;
extern int uc_libmemcalloc(int,int,void *) noex ;
extern int uc_libmemrealloc(void *,int,void *) noex ;
extern int uc_libmemfree(void *) noex ;
extern int uc_libmemrsfree(int,void *) noex ;

EXTERNC_end

#ifdef	__cplusplus

namespace libuc {
    struct libmems {
	int strw	(cchar *,int,cchar **) noex ;
	int item	(cvoid *,int,voidpp) noex ;
	int mall	(int,void *) noex ;
	int vall	(int,void *) noex ;
	int call	(int,int,void *) noex ;
	int rall	(void *,int,void *) noex ;
	int free	(void *) noex ;
	int rsfree	(int,void *) noex ;
    } ; /* end struct (libmems) */
    extern libmems	libmem ;
} /* end namespace (libuc) */

inline int lm_strw	(cchar *sp,int sl,cchar **rpp) noex {
    	return libuc::libmem.strw(sp,sl,rpp) ;
}
inline int lm_item	(cvoid *sp,int sl,void **rpp) noex {
    	return libuc::libmem.item(sp,sl,rpp) ;
}
inline int lm_mall	(int sz,void *vp) noex {
    	return libuc::libmem.mall(sz,vp) ;
}
inline int lm_vall	(int sz,void *vp) noex {
    	return libuc::libmem.vall(sz,vp) ;
}
inline int lm_call	(int ne,int esz,void *vp) noex {
    	return libuc::libmem.call(ne,esz,vp) ;
}
inline int lm_rall	(void *cp,int nsz,void *vp) noex {
    	return libuc::libmem.rall(cp,nsz,vp) ;
}
inline int lm_free	(void *vp) noex {
    	return libuc::libmem.free(vp) ;
}
inline int lm_rsfree	(int rs,void *vp) noex {
    	return libuc::libmem.rsfree(rs,vp) ;
}

#endif /* __cplusplus */

EXTERNC_begin

extern int lm_ps(char **) noex ;	/* PS pagesize */
extern int lm_ma(char **) noex ;	/* MA max-args */
extern int lm_mn(char **) noex ;	/* MN max-name */
extern int lm_mp(char **) noex ;	/* MP max-path */
extern int lm_ml(char **) noex ;	/* ML max-line */
extern int lm_mm(char **) noex ;	/* ML max-message */
extern int lm_nn(char **) noex ;	/* NN node-name */
extern int lm_hn(char **) noex ;	/* HN host-name */
extern int lm_un(char **) noex ;	/* UN user-name */
extern int lm_gn(char **) noex ;	/* GN group-name */
extern int lm_pn(char **) noex ;	/* PN project-name */
extern int lm_pw(char **) noex ;	/* PW object */
extern int lm_sp(char **) noex ;	/* SP object */
extern int lm_ua(char **) noex ;	/* UA object */
extern int lm_gr(char **) noex ;	/* GR object */
extern int lm_pj(char **) noex ;	/* PJ object */
extern int lm_pr(char **) noex ;	/* PR object */
extern int lm_nw(char **) noex ;	/* NW object */
extern int lm_ho(char **) noex ;	/* HO object */
extern int lm_sv(char **) noex ;	/* SV object */
extern int lm_fs(char **) noex ;	/* FS type */
extern int lm_sn(char **) noex ;	/* SN symbol-name */
extern int lm_zn(char **) noex ;	/* ZN time-zone-name abbreviation */
extern int lm_zi(char **) noex ;	/* ZI zone-info-name (Zone-Info) */

EXTERNC_end


#endif /* UCLIBMEM_INCLUDE */


