#!/usr/extra/bin/ksh
# TERMDEV

integer rv=1
if logged && haveprogram utmpline ; then
  LINE=$( utmpline )
  if [[ -n "${LINE}" ]] ; then
    TERM="/dev/${LINE}"
    if [[ -r ${TERM} ]] ; then
      rv=0
      print -- ${TERM}
    fi
  fi
fi
return ${rv}

