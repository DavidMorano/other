#!/usr/extra/bin/ksh
# MACTMPMOUNTS

: ${EXTRA:=/usr/extra}
: ${USRLOCAL:=/usr/local}

PATH=${PATH}:${EXTRA}/bin
PATH=${PATH}:${USRLOCAL}/bin
export PATH

: ${HOME:=$( userhome )}
export HOME

VARDIR=${HOME}/var
if [[ ! -d "${VARDIR}" ]] ; then
  mkdir ${VARDIR}
fi

if [[ -d "${VARDIR}" ]] ; then
  : ${SYSNAME:=$( sysname )}
  if [[ "${SYSNAME}" == [Dd]arwin ]] ; then
    integer n=3 i
    for (( i = 0 ; i < n ; i += 1 )) ; do
      case ${i} in
      0)
        DN=public
        CN="DARWIN_USER_DIR"
        ;;
      1)
        DN=tmp
        CN="DARWIN_USER_TEMP_DIR"
        ;;
      2)
        DN=cache
        CN="DARWIN_USER_CACHE_DIR"
        ;;
      esac
      VARDIRNAME=${VARDIR}/${DN}
      if [[ -n "${CN}" ]] ; then
        VAL=$( getconf ${CN} )
        if [[ -n "${VAL}" ]] ; then
          if [[ -L ${VARDIRNAME} ]] ; then
            A=$( readlink ${VARDIRNAME} )
            if [[ "${A}" != "${VAL}" ]] ; then
              rm -f ${VARDIRNAME}
              ln -s "${VAL}" ${VARDIRNAME}
            fi
	  elif [[ ! -d ${VARDIRNAME} ]] ; then
            rm -f ${VARDIRNAME}
            ln -s "${VAL}" ${VARDIRNAME}
          fi
        fi
      fi
    done
  fi
fi

