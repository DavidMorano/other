#!/usr/extra/bin/ksh
# UTMPNAME
# UTMPID

PN=${0##*/}
integer ex=1
if haveprogram utmp && haveprogram sid ; then
  SID=$( sid )
  utmp | while read ID NAME LINE DATE PID HOST J ; do
    if [[ ${PID} -eq ${SID} ]] ; then
      ex=0
      case ${PN} in
      utmpname)
        print -- ${NAME}
	;;
      utmpid)
        print -- ${ID}
	;;
      esac
      break
    fi
  done
fi
return ${ex}

