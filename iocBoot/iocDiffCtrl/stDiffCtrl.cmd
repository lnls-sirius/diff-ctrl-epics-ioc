#!../../bin/linux-x86_64/diffCtrl

< envPaths
< diff_ctrl.config

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/diffCtrl.dbd"
diffCtrl_registerRecordDeviceDriver pdbbase

cd "${TOP}/iocBoot/${IOC}"

## Load records

dbLoadRecords("$(TOP)/db/diffCtrl.db", "CTRL_LEFT=$(CTRL_LEFT), CTRL_RIGHT=$(CTRL_RIGHT), P=$(P), R=$(R), PREC=$(PREC), EGU=$(EGU)")

< save_restore.cmd

iocInit

# save things every 5 seconds
create_monitor_set("auto_settings_diff_ctrl.req", 5,"P=$(P), R=$(R)")
# The following line is necessary because of the save file name used in save_restore.cmd
set_savefile_name("auto_settings_diff_ctrl.req", "auto_settings_$(P)$(R).sav")
