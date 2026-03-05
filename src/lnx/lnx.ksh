#!/usr/extra/bin/ksh
# LNX

PN=${0##*/}

function errprint {
  print -u 2 -- "${PN}: ${*}"
}

if havefunction linkfile ; then
  typeset FILES=
  typeset LNAME=
  typeset TDIR=
  typeset OS=files
  typeset S=files
  typeset A
  typeset RF_ERR=false
  for A in "${@}" ; do
    case ${A} in
    -d)
      OS=${S}
      S=tdir
      ;;
    -'*')
      RF_ERR=true
      errprint "invalid option given"
      ;;
    *)
      case ${S} in
      files)
        if [[ -n "${LNAME}" ]] ; then
          FILES="${FILES} ${LNAME}"
        fi
        LNAME=${A}
        S=${OS}
        ;;
      tdir)
        TDIR=${A}
        S=${OS}
        ;;
      esac
      ;;
    esac
    if ${RF_ERR} ; then
      break
    fi
  done
  if [[ ${RF_ERR} != true ]] ; then
    if [[ -n "${TDIR}" ]] ; then
      if [[ -d "${TDIR}" ]] ; then
        if [[ -r "${LNAME}" ]] ; then
          FILES="${FILES} ${LNAME}"
        fi
      else
        RF_ERR=true
        errprint "invalid target directory"
      fi
    else
      if [[ -r "${LNAME}" ]] ; then
        TDIR=${LNAME}
      else
        RF_ERR=true
        errprint "no target directory"
      fi
    fi
    if [[ ${RF_ERR} != true ]] ; then
      if [[ -d "${TDIR}" ]] ; then
        for F in ${FILES} ; do
          if [[ -r "${F}" ]] ; then
            linkfile $F ${TDIR}
          else
            RF_ERR=true
            errprint "no-file >${F}<"
          fi
          if ${RF_ERR} ; then
            break
          fi
        done
      fi
    fi 
  fi
fi

