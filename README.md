# dash
This program was a two part project for SD Mines' Operating Systems class, taught by Dr. Karlsson.
Overall, the program acts like a terminal which gives information about the OS.
The first part gives a few new commands while the second part mimics the standard bash terminal (also contains first part commands).

### Side Note
Most Linux systems already have a command called `dash` (Debian Almquist shell). So, as an unfortunate side effect, this dash program must either be ran as `./dash` or be renamed.

## dash1
dash1 comes with __dash.cpp__ and __Makefile__. To run:

```bash
$ make
$ ./dash
```
### Usage

```
dash> systat
```
`systat` dumps information from __/proc/version__, __/proc/uptime__, __/proc/meminfo__, and part of __/proc/cpuinfo__.


```
dash> pid <command_name>
```
`pid` will find the PID associated with the given command name. Nothing is given if it does not find the command.


```
dash> cmdnm <pid>
```
`cmdnm` will find the command name associated with the given PID number. 'Invalid PID' error given if the PID is not found.

## TODO
- Commit dash2
- Add a non-looping feature to dash1 when command line paramters are given
- Redo comments?
- Usage statement in program!