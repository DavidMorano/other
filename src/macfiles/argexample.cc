/* ARGMGR */

/**** example:

****/

int proginfo::argproc() noex {
	int	rs ;
	int	rs1 ;
	if (argmgr am(argc,argv) ; (ra = am.start) >= 0) {
	    while ((rs = am.arg) > 0) {
		if (cc *sp ; (rs = am.argopt(&sp)) > 1) {
		    if (int wi ; (wi = matostr(argopts,2,sp,rs)) >= 0) {
			rs = argoptstr(&am,wi) ;
		    } else {
			rs = argoptchr(&am,sp,rs) ;
		    } /* end if (matostr) */
		} else if (rs == 1) {
		    rs = argoptchr(&am,sp,rs) ;
		} else if ((rs >= 0) && ((rs = am.argoptlong(&sp)) > 0)) {
		    rs = argoptlong(&am,sp,rs) ;
		}
		if ((rs < 0) || am.argoptdone) break ;
	    } /* end while */
	    if (rs >= 0) {
	        for (cauto &a : am) {
		    cerr << "arg=" << a << eol ;
	        } /* end for */
	    } /* end if (ok) */
	    rs1 = am.finish ;
	    if (rs >= 0) rs = rs1 ;
	} /* end if (argmgr) */
	return rs ;
} /* end method (proginfo::argproc) */

int proginfo::argoptstr(argmgr *amp,int wi) noex {
    	int	rs = SR_OK ;
	(void) amp ;
	switch (wi) {
	case argopt_a:
	    fl.a = true ;
	    break ;
	} /* end switch */
	return rs ;
} /* end method (proginfo::argkeustr) */

int progifo::argoptlong(argmgr *amp,cchar *sp,int sl) noex {
	int		rs = SR_OK ;
	cint		sl = rs ;
	(void) amp ;
	if (int wi ; (wi = matostr(argoptlongs,2,sp,rs)) >= 0) {
	    rs = 0 ;
	} else {
	    rs = SR_INVALID ;
	} /* end if (matostr) */
	return rs ;
} /* end method (proginfo::argoptlong) */

int proginfo::argoptchr(argmr *amp,cchar *sp,int sl) noex {
    	int		rs = SR_OK ;
	(void) amp ;
	while (sl-- && *sp) {
	    cint key = mkchar(*sp++) ;
	    switch (key) {
	    case 'a':
		if (amp->fl.plus) fl.a = true ;
		break ;
	    } /* end switch */
	} /* end while */
	return rs ;
} /* end method (proginfo::argoptchr) */


