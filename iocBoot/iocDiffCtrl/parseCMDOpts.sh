#!/bin/sh

while [ "$#" -gt 0 ]; do
    case "$1" in
        "-P") P="$2" ;;
        "-R") R="$2" ;;
        "-l"|"--left-ctrl-prefix") CTRL_LEFT="$2" ;;
        "-r"|"--right-ctrl-prefix") CTRL_RIGHT="$2" ;;
        "-g"|"--egu") EGU="$2" ;;
        *) echo "Usage:" >&2
            echo "  $0 -l CTRL_LEFT -r CTRL_RIGHT [-P P_VAL] [-R R_VAL]" >&2
            echo >&2
            echo " Options:" >&2
            echo "  -l or --left-ctrl-prefix       Configure left controller prefix (required)" >&2
            echo "  -r or --right-ctrl-prefix      Configure right controller prefix (required)" >&2
            echo "  -P                             Configure value of \$(P) macro" >&2
            echo "  -R                             Configure value of \$(R) macro" >&2
            echo "  -g or --egu                    Configure the engineering units displayed" >&2
            exit 2
    esac

    shift 2
done

