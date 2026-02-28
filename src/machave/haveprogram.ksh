#!/usr/extra/bin/ksh
# HAVEPROGRAM

EX=1
DN=/dev/null
TP=${1}
if [[ -n "${TP}" ]] && whence -p -q "${TP}" ; then
  EX=0
fi
return ${EX}


