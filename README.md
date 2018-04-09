# diff-ctrl-epics-ioc

### Overall

Repository containing the EPICS IOC for the Slit and Scraper applications.
This is a Soft IOC responsible for managing two motion controllers,
corresponding to the left and right sides of the slit or scraper, through two instances of the
[controller's IOC](https://github.com/lnls-dig/galil-dmc30017-epics-ioc).

### Building

In order to build the IOC, from the top level directory, run:

```sh
$ make clean uninstall install
```
### Running

In order to run the IOC, from the top level directory, run:

```sh
$ cd iocBoot/iocDiffCtrl &&
$ ./runDiffCtrl.sh -l "CTRL_LEFT" -r "CTRL_RIGHT"
```

where `CTRL_LEFT` is the prefix used for the left slit, or scraper, side's controller
and `CTRL_RIGHT` is the prefix used for the right slit, or scraper, side's controller.
The options that you can specify (after `./runDiffCtrl.sh`) are:

- `-P PREFIX1`: the value of the EPICS `$(P)` macro used to prefix the PV names
- `-R PREFIX2`: the value of the EPICS `$(R)` macro used to prefix the PV names
- `-g EGU`: the value of the engineering units displayed

In some situations it is desired to run the process using procServ,
which enables the IOC to be controlled by the system. In order to
run the IOC with procServ, instead of the previous command, run:

```sh
$ procServ -n "SLIT" -f -i ^C^D 20000 ./runDiffCtrl.sh -l "Test:Controller1:" -r "Test:Controller2" -P "Test:" -R "Slit:"
```

where the options are as previously described.
