#!/usr/extra/bin/ksh
# UTMPNAME

integer rv=1
if haveprogram utmp && haveprogram sid ; then
  SID=$( sid )
  utmp | while read ID NAME LINE DATE PID HOST J ; do
    if [[ ${PID} -eq ${SID} ]] ; then
      rv=0
      print -- ${NAME}
      break
    fi
  done
fi
return ${rv}

