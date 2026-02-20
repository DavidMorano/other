
local int proclink(PI *pip,cchar *name,ustat *sbp,FILEINFO *ckp) noex {
	int		rs = SR_OK ;
	int		f_linked = false ; /* return-value */
	if (sbp->st_dev == pip->tardev) {
	    int		w = 0 ;
	    cmode	dm = 0775 ;
	    char	tarfname[MAXPATHLEN + 1] ;
	    if ((rs = mkpath2(tarfname,pip->tardname,name)) >= 0) {
	        bool	f_dolink = true ;
	        if (ustat tsb ; (rs = uc_lstat(tarfname,&tsb)) >= 0) {
	            if (S_ISDIR(sbp->st_mode)) {
	                if (S_ISDIR(tsb.st_mode)) {
	                    f_dolink = false ;
	                } else {
	                    w = 3 ;
	                    rs = uc_unlink(tarfname) ;
	                }
		    } else {
	                bool	f = true ;
	                f = f && (tsb.st_dev == sbp->st_dev) ;
	                f = f && (tsb.st_ino == sbp->st_ino) ;
	                if (f) {
	                    f_linked = true ;
	                    f_dolink = false ;
	                } else {
	                    if (S_ISDIR(tsb.st_mode)) {
	                        w = 1 ;
	                        rs = removes(tarfname) ;
	                    } else {
	                        w = 2 ;
	                        rs = uc_unlink(tarfname) ;
	                    }
	                }
	            } /* end if */
	        } else if (isNotStat(rs)) {
	            rs = SR_OK ;
	        }

	        if ((rs >= 0) && f_dolink) {
	            if (S_ISDIR(sbp->st_mode)) {
	                w = 7 ;
	                rs = uc_mkdir(tarfname,dm) ;
	                if ((rs == SR_NOTDIR) || (rs == SR_NOENT)) {
	                    w = 8 ;
	                    if ((rs = mkpdirs(tarfname,dm)) >= 0) {
	                        w = 9 ;
	                        rs = uc_mkdir(tarfname,dm) ;
	                    }
	                }
	            } else {
	                f_linked = true ;
	                w = 4 ;
	                rs = uc_link(name,tarfname) ;
	                if ((rs == SR_NOTDIR) || (rs == SR_NOENT)) {
	                    w = 5 ;
	                    if ((rs = mkpdirs(tarfname,dm)) >= 0) {
	                        w = 6 ;
	                        rs = uc_link(name,tarfname) ;
	                    }
	                }
		    } /* end if */
	        } /* end if (dolink) */
	        if ((rs == SR_EXIST) && (! pip->fl.quiet)) {
	    	    bfile	*efp = (bfile *) pip->efp ;
	            cchar	*pn = pip->progname ;
	            bprintf(efp,"%s: exists w=%u\n",pn,w) ;
	        }
	    } /* end if (mkpath) */
	} else {
	    pip->c_linkerr += 1 ;
	    rs = SR_XDEV ;
	}
	return (rs >= 0) ? f_linked : rs ;
}
/* end subroutine (proclink) */

