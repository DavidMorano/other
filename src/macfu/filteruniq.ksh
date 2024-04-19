#!/usr/extra/bin/ksh
# FILTERUNIQ 

typeset -a FPA
integer i=0

while read FP ; do
  FPA[i]="${FP}"
  (( i += 1 ))
done

#print -- ${FPA[@]}

typeset integer n=${#FPA[@]}
i=0
integer j=0
RF_DIFFER=false
for (( i = 0 ; i < n ; i += 1 )) ; do
  RF_DIFFER=true
  for (( j = 0 ; j < i ; j += 1 )) ; do
    if [[ ${FPA[i]} -ef ${FPA[j]} ]] ; then
      RF_DIFFER=false
      break
    fi
  done
  if ${RF_DIFFER} ; then
    print -- "${FPA[i]}"
  fi
done


