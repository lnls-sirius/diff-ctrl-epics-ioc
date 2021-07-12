#!/bin/sh
set -xe
set +u

# Parse command-line options
. ./parseCMDOpts.sh "$@"

UNIX_SOCKET=""
if [ -z "${DEVICE_TELNET_PORT}" ]; then
    UNIX_SOCKET="true"
fi

if [ -z "${DIFF_INSTANCE}" ]; then
   DIFF_INSTANCE="1"
fi

set -u

# Run run*.sh scripts with procServ
if [ "${UNIX_SOCKET}" ]; then
    /usr/local/bin/procServ --logfile - -f -n diffCtrl_${DIFF_INSTANCE} -i ^C^D unix:./procserv.sock ./runDiffCtrl.sh "$@"
else
    /usr/local/bin/procServ -f -n diffCtrl_${DIFF_INSTANCE} -i ^C^D ${DEVICE_TELNET_PORT} ./runDiffCtrl.sh "$@"
fi
