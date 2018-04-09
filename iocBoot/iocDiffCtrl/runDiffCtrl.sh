#!/bin/sh

# Source environment
. ./checkEnv.sh

# Parse command-line options
. ./parseCMDOpts.sh "$@"

# Check last command return status
if [ $? -ne 0 ]; then
        echo "Could not parse command-line options" >&2
        exit 1
fi

if [ -z "$EPICS_CA_MAX_ARRAY_BYTES" ]; then
    EPICS_CA_MAX_ARRAY_BYTES="50000000"
fi

if [ -z "$EGU" ]; then
    EGU="mm"
fi

if [ -z "$CTRL_LEFT" ]; then
    echo "Left controller prefix not set. Please use the -l option or set \$CTRL_LEFT environment variable" >&2
    exit 3
fi

if [ -z "$CTRL_RIGHT" ]; then
    echo "Right controller prefix not set. Please use the -r option or set \$CTRL_RIGHT environment variable" >&2
    exit 3
fi

cd "$IOC_BOOT_DIR"

EPICS_CA_MAX_ARRAY_BYTES="$EPICS_CA_MAX_ARRAY_BYTES" EGU="$EGU" \
CTRL_LEFT="$CTRL_LEFT" CTRL_RIGHT="$CTRL_RIGHT" P="$P" R="$R" \
"$IOC_BIN" stDiffCtrl.cmd
