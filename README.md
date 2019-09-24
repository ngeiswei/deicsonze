# Deicsonze

Emulator of the YM2414, the FM synthesizer of the Yamaha DX11 and TX81Z.

## Features

- [X] Nearly perfect emulation
- [X] Support SysEx
- [X] Over 2000 presets
- [X] VST, DSSI, (LV2 comming soon)
- [ ] MIDI controls assigned to parameters
- [ ] GUI

## Requirements

- Boost (version 1.54 minimum) http://www.boost.org/
- DSSI SDK http://dssi.sourceforge.net/ (optional)
- VST SDK http://www.steinberg.net/en/company/developers.html (optional)

## Installation

1. Create build directory and run cmake
```bash
$ mkdir build
$ cd build
$ cmake ..
```
2. Compile
```bash
$ make
```
3. Install
```bash
$ sudo make install
```

For VST support, download the VST SDK
(https://www.steinberg.net/en/company/developers.html) and unzip it
directly under the project root directory.

Under GNU/Linux 64-bit, you may also need to comment out a bit of code
in

```
pluginterfaces/vst2.x/aeffect.h
```

like all definitions of VSTCALLBACK except
```
#define VSTCALLBACK
```

## Parameters Description

TODO

## TODO

- [ ] Fix release bug in Poly mode
- [ ] Fix noise period parameter
- [ ] Expose parameters to VST host
- [ ] Define presets

## Help

The last features (portamento and pitch envelope) have been
implemented only based on the documentation, not compared with the
real hardward. If you own a DX11 or TX81Z I need you! Please record
your sound with different values of portamento and pitch envelope and
send it to me so I can improve the fidelity of the emulation.

## Author

Nil Geisweiller
