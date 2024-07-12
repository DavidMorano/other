#!/usr/extra/bin/ksh
# MAKEINSTALL
PN=${0##*/}
ID=installdirs.txt
IN=installnames.txt
if [[ -r "${ID}" ]] && [[ -r "${IN}" ]] ; then
  PROG=${1}
  if [[ -x "${PROG}" ]] ; then
    for D in $( < ${ID} ) ; do
      if [[ -d "${D}" ]] ; then
        for N in $( < ${IN} ) ; do
          if [[ -n "${N}" ]] ; then
	    lnx ${PROG} ${D}/bin/${N}
          fi
        done
      fi
    done
  else
    print -u2 -- "${PN}: missing program"
  fi
else
  print -u2 -- "${PN}: missing configuration"
fi
