# diff-ctrl-epics-ioc

### Overall

Repository containing the EPICS IOC for the Slit and Scraper applications.
This is a Soft IOC responsible for managing two motion controllers,
corresponding to the negative and positive sides of the slit or scraper, through two instances of the
[controller's IOC](https://github.com/lnls-dig/galil-dmc30017-epics-ioc).

The negative and positive edges are named so to indicate how are their positions
with respect to the beam center. Given that the beam is at position zero,
negative values with greater magnitude increase the distance of the negative edge
from the center, while greater positive position values for the positive edge increase the
distance of it from the beam center.

### Building

In order to build the IOC, from the top level directory, run:

```sh
$ make clean uninstall install
```
### Running

In order to run the IOC, from the top level directory, run:

```sh
$ cd iocBoot/iocDiffCtrl &&
$ ./runDiffCtrl.sh -n "CTRL_NEG" -p "CTRL_POS" -d "DEVICE" -P "PREFIX1" -R "PREFIX2"
```

where `CTRL_NEG` is the PV prefix used for the negative side controller of a slit or scraper,
`CTRL_POS` is the PV prefix used for the positive side controller of a slit or scraper,
`DEVICE` is the movement conversion type, e.g., slit, `PREFIX1` is the first PV prefix,
and `PREFIX2` is the second PV prefix.
The complete list of options that you can specify (after `./runDiffCtrl.sh`) is:

- `-P PREFIX1`: the value of the EPICS `$(P)` macro used to prefix the PV names
- `-R PREFIX2`: the value of the EPICS `$(R)` macro used to prefix the PV names
- `-n CTRL_NEG`: PV prefix used for the controller of the negative Slit/Scraper blade
- `-p CTRL_POS`: PV prefix used for the controller of the positive Slit/Scraper blade
- `-d DEVICE`: Configure the device type used for coordinate conversion: `slit`, `scraper-h`, or `scraper-v`.
- `-g EGU`: the value of the engineering units displayed
- `-l LOW_LIM`: the low limit for the negative edge position
- `-h HIGH_LIM`: the high limit for the positive edge position
- `-a NEG_LIM`: the position limit for the negative edge as it advances towards the center
- `-r POS_LIM`: the position limit for the positive edge as it advances towards the center

In some situations it is desired to run the process using procServ,
which enables the IOC to be controlled by the system. In order to
run the IOC with procServ, instead of the previous command, run:

```sh
$ cd iocBoot/iocDiffCtrl &&
$ ./runProcServ.sh -t "TELNET_PORT" -n "CTRL_NEG" -p "CTRL_POS" -d "DEVICE" -P "PREFIX1" -R "PREFIX2"
```

where `TELNET\_PORT` is the telnet port used to access the IOC shell,
and the other options are as previously described.
