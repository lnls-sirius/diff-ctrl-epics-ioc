#!../../bin/linux-x86_64/diffCtrl

< envPaths

epicsEnvSet("TOP", "../..")

< diff_ctrl.config

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/diffCtrl.dbd"
diffCtrl_registerRecordDeviceDriver pdbbase

cd "iocBoot/${IOC}"

## Load records

dbLoadRecords("$(TOP)/db/diff_ctrl.db", "CTRL_NEG=$(CTRL_NEG), CTRL_POS=$(CTRL_POS), P=$(P), R=$(R), PREC=$(PREC), EGU=$(EGU), LOW_LIM=$(LOW_LIM), HIGH_LIM=$(HIGH_LIM), NEG_LIM=$(NEG_LIM), POS_LIM=$(POS_LIM), CONV_FUNC_NEG_CMD=$(CONV_FUNC_NEG_CMD), CONV_FUNC_POS_CMD=$(CONV_FUNC_POS_CMD), CONV_FUNC_NEG_RB=$(CONV_FUNC_NEG_RB), CONV_FUNC_POS_RB=$(CONV_FUNC_POS_RB), CONV_INPB=${CONV_INPB}, CONV_INPC=${CONV_INPC}, CONV_INPD=${CONV_INPD}, CONV_INPE=${CONV_INPE}, CONV_INPF=${CONV_INPF}, CONV_INPG=${CONV_INPG}, CONV_INPH=${CONV_INPH}, CONV_INPI=${CONV_INPI}, CONV_INPJ=${CONV_INPJ}, CONV_INPK=${CONV_INPK}, CONV_INPL=${CONV_INPL}")

< save_restore.cmd

iocInit

# save things every 5 seconds
create_monitor_set("auto_settings_diff_ctrl.req", 5,"P=$(P), R=$(R)")
# The following line is necessary because of the save file name used in save_restore.cmd
set_savefile_name("auto_settings_diff_ctrl.req", "auto_settings_$(P)$(R).sav")
