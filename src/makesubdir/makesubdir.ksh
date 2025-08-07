#!/usr/extra/bin/ksh
# MAKESUBDIR


PROG_WHICH=/bin/which
PROG_DOMAINNAME=/bin/domainname
PROG_FGREP=/bin/fgrep
PROG_CUT=/bin/cut
PROG_UNAME=/bin/uname
PROG_BASENAME=/bin/basename


PN=${0##*/}
: ${PWD:=$( pwd )}
: ${MAKE:=make}

TARGETS=
DIRS=

RF_DEBUG=false
RF_QUIET=false
RF_TARGETS=false
RF_DIRS=false
RF_OK=true

S=targets
OS=
for A in "${@}" ; do
  case $A in
  '-D' )
    RF_DEBUG=true
    ;;
  '-d' )
    OS=${S}
    S=dirs
    ;;
  '-q' )
    RF_QUIET=1
    ;;
  '-'* )
    print -u2 -- "${PN}: unknown option \"${A}\""
    RF_OK=false
    ;;
  * )
    case $S in
    targets )
      RF_TARGETS=1
      TARGETS="${TARGETS} ${A}"
      ;;
    dirs )
      RF_DIRS=1
      DIRS="${DIRS} ${A}"
      S=${OS}
      ;;
    esac
    ;;
  esac
done

TMPFILE=${TMPDIR:=/tmp}/mb${RANDOM}${$}

cleanup() {
  rm -f $TMPFILE
}

trap 'cleanup ; exit 1' 1 2 3 15 16 17

function linkfile {
  typeset F1=${1}
  typeset F2=${2}
  if [[ -r ${F1} ]] ; then
    if [[ -r ${F2} ]] ; then
      if [[ $F1 -ef $F2 ]] ; then :
      else
        ln -f $F1 $F2
      fi
    else
      ln -f $F1 $F2
    fi
  fi
}

function linkheader {
  typeset R S HF CDIR=${PWD} OF=${1}
  if [[ -r "${OF}" ]] ; then
    R=${OF%.*}
    S=${OF##*.}
    if [[ $S == 'o' ]] || [[ $S == 'a' ]] ; then
      HF=${R}.h
      if [[ -r ${HF} ]] ; then
	linkfile ${HF} ${CDIR}/${HF}
      fi
    fi
  fi
}

function makesub {
  typeset CDIR=${PWD} T=${1} 
  typeset S DS
  if [[ -n "${T}" ]] ; then
    typeset D=${T%.*}
    if [[ -d "${D}" ]] ; then
      ${MAKE} -C $D --no-print-directory $T
      for S in o a so h hh ; do
        DS=${D}/${D}.${S}
        if [[ -r ${DS} ]] ; then
	  linkfile ${DS} ${D}.${S}
        fi
      done
    fi
  fi
}

for T in ${TARGETS} ; do
  integer F=0 FS=0
  typeset S
  typeset D=${T%.*}
  if [[ -d "${D}" ]] ; then
    TF=${D}.o
    if [[ -r ${D}/Makefile ]] ; then
      if [[ -r "${TF}" ]] ; then
        if [[ -r ${D}/${TF} ]] ; then
	  if [[ ${TF} -ef ${D}/${TF} ]] ; then
            for S in o a so c cc ccm h hh ; do
	      typeset DSS=${D}/${D}*.${S}
	      for DS in ${DSS} ; do
	        if [[ -r "${DS}" ]] ; then
	          FS=1
	          if [[ ${DS} -nt ${TF} ]] ; then
	            F=1
	          fi
	        fi
	        if [[ $F -ne 0 ]] ; then break ; fi
	      done
	      if [[ $F -ne 0 ]] ; then break ; fi
	    done
            if [[ $F -ne 0 ]] || [[ ${FS} -eq 0 ]] ; then
	      makesub ${TF}
            fi
	  else
	    makesub ${TF}
	  fi
	else
	  makesub $TF
	fi
      else
	makesub $TF
      fi
    fi
  fi
done

cleanup


