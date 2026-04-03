#!/usr/extra/bin/ksh
# WHATIS

# change the following ("RF_DEBUG") to "true" to enable debugging
RF_DEBUG=false

PN=${0##*/}
KEY=
FGREP_OPTS=

if [[ ${PN} == "whatis" ]] ; then
  FGREP_OPTS="-w"
fi

function debprint {
  if ${RF_DEBUG} && [[ -n "${*}" ]] ; then
    print -u 2 -- "${PN}: ${*}" | cl
  fi
}

function findone {
  typeset WI
  KEY=$1
  if [[ -n "${KEY}" ]] ; then
    debprint key=${KEY}
    pathenum MANPATH | while read D ; do
      if [[ -d "${D}" ]] ; then
        debprint mandir=${D}
        WI=${D}/whatis
        if [[ -r ${WI} ]] ; then
          fgrep ${FGREP_OPTS} ${KEY} ${WI}
        fi
      fi
    done
  fi
}


if haveprogram pathenum ; then
  for K in "${@}" ; do
    findone $K
  done
fi


