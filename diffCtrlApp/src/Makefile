TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS BELOW HERE

#=============================
# Build the IOC application
PROD_IOC = diffCtrl

# diffCtrl.dbd will be created and installed
DBD += diffCtrl.dbd

# diffCtrl.dbd will include these files:
diffCtrl_DBD += base.dbd

# diffCtrl_registerRecordDeviceDriver.cpp derives from diffCtrl.dbd
diffCtrl_SRCS += diffCtrl_registerRecordDeviceDriver.cpp

# Build the main IOC entry point where needed
diffCtrl_SRCS_DEFAULT += diffCtrlMain.cpp
diffCtrl_SRCS_vxWorks += -nil-

# Finally link IOC to the EPICS Base libraries
diffCtrl_LIBS += $(EPICS_BASE_IOC_LIBS)

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD EXTRA GNUMAKE RULES BELOW HERE

diffCtrl_DBD += calcSupport.dbd
diffCtrl_DBD += asSupport.dbd

diffCtrl_LIBS += calc
diffCtrl_LIBS += autosave
