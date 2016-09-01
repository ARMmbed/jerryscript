# JerryScript with mbed OS 5

TL;DR? jump straight to [quickstart](#quick-start)

## Introduction

This directory contains the necessary code to build JerryScript for devices
capable of running mbed OS 5. It has been tested with the following boards
so far:

- [Nordic Semiconductor NRF52 Development Kit](https://developer.mbed.org/platforms/Nordic-nRF52-DK/)
- [NXP Freedom K64F](https://developer.mbed.org/platforms/FRDM-K64F/)

## Features

### Peripheral Drivers

Peripheral Drivers are intended as a 1-to-1 mapping to mbed C++ APIs, with a few
differences (due to differences between JavaScript and C++ like lack of operator
overloading).

- [DigitalOut](https://docs.mbed.com/docs/mbed-os-api-reference/en/5.1/APIs/io/DigitalOut/)
- [InterruptIn](https://docs.mbed.com/docs/mbed-os-api-reference/en/5.1/APIs/io/InterruptIn/)
- [I2C](https://docs.mbed.com/docs/mbed-os-api-reference/en/5.1/APIs/interfaces/digital/I2C/)
- [Ticker](https://docs.mbed.com/docs/mbed-os-api-reference/en/5.1/APIs/tasks/Ticker/) (exposed through setInterval and setTimeout)

### Bluetooth Low Energy

Tested with the Nordic NRF52 DK and the ST Nucleo IDB04A1 shield, though should
support other devices supporting the mbed BLE API. The API is loosely based on
the [bleno](https://github.com/sandeepmistry/bleno) library, and only currently
supports BLE peripherals.

### mbed Client

Wrapper on top of [simple-mbed-client](https://github.com/ARMmbed/)

### Networking

Supports ethernet devices supported by the lwIP stack in mbed, as well as the
ESP8266.

## Dependencies

### mbed CLI

mbed CLI is used as the build tool for mbed OS 5. You can find out how to install
it in the [official documentation](https://docs.mbed.com/docs/mbed-os-handbook/en/5.1/dev_tools/cli/#installing-mbed-cli).

### arm-none-eabi-gcc

arm-none-eabi-gcc is the only currently tested compiler for jerryscript on mbed,
and instructions for building can be found as part of the mbed-cli installation
instructions above.

### make

make is used to automate the process of fetching dependencies, and making sure that
mbed-cli is called with the correct arguments. 

### (optional) jshint

jshint is used to statically check your JavaScript code, as part of the build process.
This ensures that pins you are using in your code are available on your chosen target
platform.

## Quick Start

Once you have all of your dependencies installed, you can build the project as follows:

```bash
git clone https://github.com/ARMmbed/jerryscript
cd jerryscript
make getlibs
# NRF52 Development Kit:
make BOARD=NRF52_DK
# FRDM K64F
make BOARD=K64F
```

The produced file (in .build/*[BOARD]*/GCC_ARM) can then be uploaded to your board, and will
run when you press reset.

If you make a modification to main.js, you can simply rerun make, and it will remember your
previous choice of board:

```bash
make
```
