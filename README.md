NES Zelda for TI-84 Plus CE
===========================

A game for your graphing calculator. This is a port, NOT an emulator, and does not rely on the original ROM. The project is currently in the development stage, and could be unstable enough to brick your calculator.

Building
--------

Compiling and packaging requires the [CE C/C++ Toolchain](https://github.com/CE-Programming/toolchain) and make.

To package graphics assets: `make gfx`

To compile the program binary: `make`

Use [TI Connect CE](https://education.ti.com/en/products/computer-software/ti-connect-ce-sw) to send the program, assets, and the accompanying [clibs.8xg](lib/clibs.8xg) to the calculator's archive, or import into [CEmu](https://github.com/CE-Programming/CEmu) for emulation.
