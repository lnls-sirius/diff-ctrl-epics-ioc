#!/bin/sh

set -e
set +u

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

if [ -z "$CTRL_NEG" ]; then
    echo "Negative controller prefix not set. Please use the -n option or set \$CTRL_NEG environment variable" >&2
    exit 3
fi

if [ -z "$CTRL_POS" ]; then
    echo "Positive controller prefix not set. Please use the -p option or set \$CTRL_POS environment variable" >&2
    exit 3
fi

cd "$IOC_BOOT_DIR"

EPICS_CA_MAX_ARRAY_BYTES="$EPICS_CA_MAX_ARRAY_BYTES" EGU="$EGU" \
CTRL_NEG="$CTRL_NEG" CTRL_POS="$CTRL_POS" P="$P" R="$R" \
"$IOC_BIN" stDiffCtrl.cmd
