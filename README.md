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
$ ./runDiffCtrl.sh -n "CTRL_NEG" -p "CTRL_POS"
```

where `CTRL_NEG` is the prefix used for the negative slit, or scraper, side's controller
and `CTRL_POS` is the prefix used for the positive slit, or scraper, side's controller.
The other options that you can specify (after `./runDiffCtrl.sh`) are:

- `-P PREFIX1`: the value of the EPICS `$(P)` macro used to prefix the PV names
- `-R PREFIX2`: the value of the EPICS `$(R)` macro used to prefix the PV names
- `-g EGU`: the value of the engineering units displayed
- `-l LOW_LIM`: the low limit for the negative edge position
- `-h HIGH_LIM`: the high limit for the positive edge position

In some situations it is desired to run the process using procServ,
which enables the IOC to be controlled by the system. In order to
run the IOC with procServ, instead of the previous command, run:

```sh
$ procServ -n "SLIT" -f -i ^C^D 20000 ./runDiffCtrl.sh -n "Test:Controller1:" -p "Test:Controller2" -P "Test:" -R "Slit:"
```

where the options are as previously described.
