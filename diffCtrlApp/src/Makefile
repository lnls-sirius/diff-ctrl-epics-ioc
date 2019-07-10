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
