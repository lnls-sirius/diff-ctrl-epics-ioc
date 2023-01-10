TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS BELOW HERE

#=============================
# Build an IOC support library
LIBRARY_IOC += diffCtrlSupport

# Link locally-provided code into the support library,
# rather than directly into the IOC application.
# This is required for Windows DLL builds.
diffCtrlSupport_SRCS += convFnc.c
diffCtrlSupport_SRCS += myUtil.c

INC += myUtil.h

diffCtrlSupport_LIBS += $(EPICS_BASE_IOC_LIBS)

# Build the IOC application
PROD_IOC = diffCtrl

# diffCtrl.dbd will be created and installed
DBD += diffCtrl.dbd

# diffCtrl.dbd will include these files:
diffCtrl_DBD += base.dbd

# Include dbd files from all support applications:
diffCtrl_DBD += convFnc.dbd

# diffCtrl_registerRecordDeviceDriver.cpp derives from diffCtrl.dbd
diffCtrl_SRCS += diffCtrl_registerRecordDeviceDriver.cpp

# Build the main IOC entry point where needed
diffCtrl_SRCS_DEFAULT += diffCtrlMain.cpp
diffCtrl_SRCS_vxWorks += -nil-

# To build SNL programs, SNCSEQ must be defined
# in the <top>/configure/RELEASE file
ifneq ($(SNCSEQ),)
    # Build sncDiffCtrl into diffCtrlSupport
    sncDiffCtrl_SNCFLAGS += +r
    diffCtrl_DBD += sncDiffCtrl.dbd
    diffCtrlSupport_SRCS += sncDiffCtrl.stt
    diffCtrlSupport_LIBS += seq pv
    diffCtrl_LIBS += seq pv

    # Build sncDiffCtrlProgram as a standalone program
    PROD_HOST += sncDiffCtrlProgram
    sncDiffCtrlProgram_SNCFLAGS += +m
    sncDiffCtrlProgram_SRCS += sncDiffCtrlProgram.st
    sncDiffCtrlProgram_LIBS += seq pv
    sncDiffCtrlProgram_LIBS += $(EPICS_BASE_HOST_LIBS)
endif

# Link in the code from our support library
diffCtrl_LIBS += diffCtrlSupport

# Finally link IOC to the EPICS Base libraries
diffCtrl_LIBS += $(EPICS_BASE_IOC_LIBS)

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD EXTRA GNUMAKE RULES BELOW HERE

diffCtrl_DBD += calcSupport.dbd
diffCtrl_DBD += asSupport.dbd

diffCtrl_LIBS += calc
diffCtrl_LIBS += autosave
