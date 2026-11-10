#!/usr/local/bin/ksh
# MAXTEMP

if haveprogram smc ; then
  smc -F "${@}"
fi

