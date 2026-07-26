#!/usr/extra/bin/ksh
# CL

PN=${0##*/}
COLS="1-80"
case ${PN} in
cl40)
  COLS="41-120"
  ;;
cl60)
  COLS="61-140"
  ;;
cl80)
  COLS="81-160"
  ;;
esac

if [[ ${#} -gt 0 ]] ; then
  for F in ${@} ; do
    expand $F
  done
else
  expand 
fi | cut -c ${COLS}

