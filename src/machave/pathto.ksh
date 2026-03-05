#!/usr/extra/bin/ksh
# PATHTO

EX=1
DN=/dev/null
TP=${1}
if [[ -n "${TP}" ]] && whence -a -p "${TP}" ; then
  EX=0
fi
return ${EX}


