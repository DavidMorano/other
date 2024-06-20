# PATHTO (shell function)
# prints path to argument (executable command) via searching PATH
# 
#

function pathto {

  typeset USAGE PDIR A C PNAME CMDS OS S OLDIFS EX
  typeset F_DEBUG F_SINGLE
  typeset PATHS


USAGE="pathto [-s] <command-name>"


PDIR=${0%/*}
A=${0##*/}
PNAME=${A%.*}


F_DEBUG=0
F_SINGLE=0


CMDS=


S=cmds
OS=""
for A in "${@}" ; do

  case $A in

  '-D' )
    F_DEBUG=1
    ;;

  '-s' )
    F_SINGLE=1
    ;;

  '-'* )
    echo "${PNAME}: unknown option \"${A}\"" >&2
    return 1
    ;;

  * )
    case $S in

    cmds )
      CMDS="${CMDS} ${A}"
      ;;

    esac
    ;;

  esac

done


if [[ -z "${CMDS}" ]] ; then

  echo "${PNAME}: argument required" >&2
  echo "${PNAME}: USAGE> ${USAGE}" >&2
  return 1

fi

if [[ -z "${PATH}" ]] ; then
  return 0
fi

PATHS=
OLDIFS=${IFS}
IFS=:
for C in $PATH ; do
  if [[ -n "${C}" ]] ; then
    PATHS="${PATHS} ${C}"
  else
    PATHS="${PATHS} ."
  fi
done
IFS=${OLDIFS}


CWD=${PWD:-$( pwd )}

integer count

EX=1

if [[ $F_DEBUG -ne 0 ]] ; then
  print -u2 CMDS="${CMDS}"
fi

for CMD in $CMDS ; do

  if [[ $F_DEBUG -ne 0 ]] ; then
    print -u2 CMD="${CMD}"
  fi

  (( count = 0 ))
  for I in $PATHS ; do
  
    if [[ $F_DEBUG -ne 0 ]] ; then
      print -u2 PATH="${I}"
    fi

    if [[ -z "${I}" ]] ; then
  
      if [[ -x "${CMD}" ]] ; then
  
        print "${CWD}/${CMD}"
        (( count += 1 ))
        EX=0
  
      fi
  
    else
  
      if [[ -x "${I}/${CMD}" ]] ; then
  
        print "${I}/${CMD}"
        (( count += 1 ))
        EX=0
  
      fi
  
    fi
  
    if [[ $F_SINGLE -ne 0 ]] && (( count > 0 )) ; then
      break 1
    fi
  
  done

done

return $EX
}
# end function (pathto)


function fpathto {

  typeset USAGE PDIR A C PNAME CMDS OS S OLDIFS EX
  typeset F_DEBUG F_SINGLE
  typeset PATHS


USAGE="fpathto [-s] <command-name>"


PDIR=${0%/*}
A=${0##*/}
PNAME=${A%.*}


F_DEBUG=0
F_SINGLE=0


CMDS=


S=cmds
OS=""
for A in "${@}" ; do

  case $A in

  '-D' )
    F_DEBUG=1
    ;;

  '-s' )
    F_SINGLE=1
    ;;

  '-'* )
    echo "${PNAME}: unknown option \"${A}\"" >&2
    return 1
    ;;

  * )
    case $S in

    cmds )
      CMDS="${CMDS} ${A}"
      ;;

    esac
    ;;

  esac

done


if [[ -z "${CMDS}" ]] ; then

  echo "${PNAME}: argument required" >&2
  echo "${PNAME}: USAGE> ${USAGE}" >&2
  return 1

fi

if [[ -z "${PATH}" ]] ; then
  return 0
fi

PATHS=
OLDIFS=${IFS}
IFS=:
for C in $FPATH ; do
  if [[ -n "${C}" ]] ; then
    PATHS="${PATHS} ${C}"
  else
    PATHS="${PATHS} ."
  fi
done
IFS=${OLDIFS}


CWD=${PWD:-$( pwd )}

integer count

EX=1

if [[ $F_DEBUG -ne 0 ]] ; then
  print -u2 CMDS="${CMDS}"
fi

for CMD in $CMDS ; do

  if [[ $F_DEBUG -ne 0 ]] ; then
    print -u2 CMD="${CMD}"
  fi

  (( count = 0 ))
  for I in $PATHS ; do
  
    if [[ $F_DEBUG -ne 0 ]] ; then
      print -u2 PATH="${I}"
    fi

    if [[ -z "${I}" ]] ; then
  
      if [[ -r "${CMD}" ]] ; then
  
        print "${CWD}/${CMD}"
        (( count += 1 ))
        EX=0
  
      fi
  
    else
  
      if [[ -r "${I}/${CMD}" ]] ; then
  
        print "${I}/${CMD}"
        (( count += 1 ))
        EX=0
  
      fi
  
    fi
  
    if [[ $F_SINGLE -ne 0 ]] && (( count > 0 )) ; then
      break 1
    fi
  
  done

done

return $EX
}
# end function (fpathto)

function pt {
   pathto "${@}"
}


