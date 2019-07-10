#!/bin/sh

set -e 

usage () {
    echo "Usage:" >&2
    echo "  $1 -t PROCSERV_TELNET_PORT -P P_VAL -R R_VAL -n CTRL_NEG -p CTRL_POS -d DEVICE -g EGU -l LOW_LIM -h HIGH_LIM -a NEG_LIM -r POS_LIM" >&2
    echo >&2
    echo " Options:" >&2
    echo "  -t                  Configure procServ telnet port" >&2
    echo "  -P                  Configure value of \$(P) macro" >&2
    echo "  -R                  Configure value of \$(R) macro" >&2
    echo "  -n                  Configure negative controller prefix (required)" >&2
    echo "  -p                  Configure positive controller prefix (required)" >&2
    echo "  -d                  Configure the device type used for coordinate conversion: SLIT or SCRAPER." >&2
    echo "  -g                  Configure the engineering units displayed" >&2
    echo "  -l                  Configure the low limit for the negative edge position" >&2
    echo "  -h                  Configure the high limit for the positive edge position" >&2
    echo "  -a                  Configure the negative controller advancing movement position limit" >&2
    echo "  -r                  Configure the positive controller advancing movement position limit" >&2
}

while getopts ":t:P:R:n:p:d:g:l:h:a:r:" opt; do
  case $opt in
    t) DEVICE_TELNET_PORT="$OPTARG" ;;
    P) P="$OPTARG" ;;
    R) R="$OPTARG" ;;
    n) CTRL_NEG="$OPTARG" ;;
    p) CTRL_POS="$OPTARG" ;;
    d) case "$(echo "$OPTARG" | tr "[:upper:]" "[:lower:]")" in
           "slit") DEVICE="SLIT" ;;
           "scraper-h") DEVICE="SCRAPER_H" ;;
           "scraper-v") DEVICE="SCRAPER_V" ;;
           *) DEVICE="$OPTARG" ;;
        esac
        ;;
    g) EGU="$OPTARG" ;;
    l) LOW_LIM="$OPTARG" ;;
    h) HIGH_LIM="$OPTARG" ;;
    a) NEG_LIM="$OPTARG" ;;
    r) POS_LIM="$OPTARG" ;;
    \?)
      echo "Invalid option: -$OPTARG" >&2
      usage $0
      exit 1
      ;;
    :)
      echo "Option -$OPTARG requires an argument." >&2
      usage $0
      exit 1
      ;;
  esac
done

# if getopts did not process all input
if [ "$OPTIND" -le "$#" ]; then
    echo "Invalid argument at index '$OPTIND' does not have a corresponding option." >&2
    usage $0
    exit 1
fi
