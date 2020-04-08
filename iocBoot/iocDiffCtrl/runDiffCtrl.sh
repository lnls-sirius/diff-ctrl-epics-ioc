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

if [ -z "$CTRL_NEG" ]; then
    echo "Negative controller prefix not set. Please use the -n option or set \$CTRL_NEG environment variable" >&2
    exit 2
fi

if [ -z "$CTRL_POS" ]; then
    echo "Positive controller prefix not set. Please use the -p option or set \$CTRL_POS environment variable" >&2
    exit 3
fi

if [ -z "$EPICS_CA_MAX_ARRAY_BYTES" ]; then
    EPICS_CA_MAX_ARRAY_BYTES="50000000"
fi

if [ -z "$EGU" ]; then
    EGU="mm"
fi

if [ -z "$LOW_LIM" ]; then
    LOW_LIM="-1000000"
fi

if [ -z "$HIGH_LIM" ]; then
    HIGH_LIM="1000000"
fi

if [ -z "$NEG_LIM" ]; then
    NEG_LIM="1000000"
fi

if [ -z "$POS_LIM" ]; then
    POS_LIM="-1000000"
fi

case ${DEVICE} in
    SLIT)
        CONV_FUNC_NEG_CMD="slitConvFnc";             # negative edge command conv function
        CONV_FUNC_NEG_RB="slitConvFnc";              # negative edge readback conv function
        CONV_FUNC_POS_CMD="slitConvFnc";             # positive edge command conv function
        CONV_FUNC_POS_RB="slitConvFnc";              # positive edge readback conv function
        ;;

    SCRAPER_V)
        CONV_FUNC_NEG_CMD="convFuncVScrapBottowCmd"; # negative edge cmd conv function
        CONV_FUNC_NEG_RB="convFuncVScrapBottowRB";   # negative edge RB conv function
        CONV_FUNC_POS_CMD="convFuncVScrapTopCmd";    # positive edge cmd conv function
        CONV_FUNC_POS_RB="convFuncVScrapTopRB";      # positive edge RB conv function
        ;;

    SCRAPER_H)
        CONV_FUNC_NEG_CMD="convFuncHScrapInCmd";     # negative edge cmd conv function
        CONV_FUNC_NEG_RB="convFuncHScrapInRB";       # negative edge RB conv function
        CONV_FUNC_POS_CMD="convFuncHScrapOutCmd";    # positive edge cmd conv function
        CONV_FUNC_POS_RB="convFuncHScrapOutRB";      # positive edge RB conv function
        ;;

    *)
        echo "Invalid device type: ${DEVICE}. Please see available -d option choices." >&2
        exit 4
        ;;
esac

cd "$IOC_BOOT_DIR"

EPICS_CA_MAX_ARRAY_BYTES="$EPICS_CA_MAX_ARRAY_BYTES" \
	P="$P" R="$R" \
	EGU="$EGU" \
	LOW_LIM="$LOW_LIM" \
	HIGH_LIM="$HIGH_LIM" \
	NEG_LIM="$NEG_LIM" \
	POS_LIM="$POS_LIM" \
	CTRL_NEG="$CTRL_NEG" \
	CTRL_POS="$CTRL_POS" \
	CONV_FUNC_NEG_CMD="$CONV_FUNC_NEG_CMD" \
	CONV_FUNC_POS_CMD="$CONV_FUNC_POS_CMD" \
	CONV_FUNC_NEG_RB="$CONV_FUNC_NEG_RB" \
	CONV_FUNC_POS_RB="$CONV_FUNC_POS_RB" \
	CONV_INPB="$CONV_INPB" \
	CONV_INPC="$CONV_INPC" \
	CONV_INPD="$CONV_INPD" \
	CONV_INPE="$CONV_INPE" \
	CONV_INPF="$CONV_INPF" \
	CONV_INPG="$CONV_INPG" \
	CONV_INPH="$CONV_INPH" \
	CONV_INPI="$CONV_INPI" \
	CONV_INPJ="$CONV_INPJ" \
	CONV_INPK="$CONV_INPK" \
	CONV_INPL="$CONV_INPL" \
	"$IOC_BIN" stDiffCtrl.cmd
