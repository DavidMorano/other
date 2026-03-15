#!/usr/extra/bin/ksh
# TERMDEV

integer ex=1
if logged && haveprogram utmpline ; then
  LINE=$( utmpline )
  if [[ -n "${LINE}" ]] ; then
    TERM="/dev/${LINE}"
    if [[ -r ${TERM} ]] ; then
      ex=0
      print -- ${TERM}
    fi
  fi
fi
return ${ex}

