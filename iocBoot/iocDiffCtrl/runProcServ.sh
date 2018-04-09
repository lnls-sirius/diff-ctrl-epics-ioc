#!/bin/sh

# Use defaults if not set
if [ -z "${DIFF_DEVICE_TELNET_PORT}" ]; then
   DIFF_DEVICE_TELNET_PORT=20000
fi

# Run run*.sh scripts with procServ
/usr/local/bin/procServ -f -n diffCtrl${DIFF_INSTANCE} -i ^C^D ${DIFF_DEVICE_TELNET_PORT} ./runDiffCtrl.sh "$@"
