#!/usr/extra/bin/ksh
# FU

typeset -a FILES
integer i=0

function unique {
  typeset	FF=${1}
  integer	fr=1
  integer	j=0
  if [[ -f "${FF}" ]] ; then
    fr=0
    for (( j = 0 ; j < ${#FILES[@]} ; j += 1 )) ; do
      if [[ $FF -ef ${FILES[j]} ]] ; then
	fr=1
        break
      fi
    done
  fi
  return ${fr}
}

while read F ; do 
  if [[ -r "${F}" ]] ; then 
    if unique $F ; then
      FILES[i++]=${F}
    fi
  fi 
done

integer j=0
for (( j = 0 ; j < ${#FILES[@]} ; j += 1 )) ; do
  print -- ${FILES[j]}
done


