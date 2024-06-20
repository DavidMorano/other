# USERNAME + HROUPNAME (shell function)

function username {
  : ${USERNAME:=${LOGNAME}}
  : ${USERNAME:=${USER}}
  : ${USERNAME:=${HOME##*/}}
  : ${USERNAME:=${MAIL##*/}}
  : ${USERNAME:=$( command username )}
  print -- ${USERNAME}
}

function groupname {
  : ${GROUPNAME:=$( command groupname )}
  print -- ${GROUPNAME}
}


