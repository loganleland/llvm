# LLVM
This directory and its subdirectories contain source code for LLVM,
a toolkit for the construction of highly optimized compilers,
optimizers, and runtime environments.

LLVM is open source software. You may freely distribute it under the terms of
the license agreement found in LICENSE.txt.

Please see the documentation provided in docs/ for further
assistance with LLVM, and in particular docs/GettingStarted.rst for getting
started with LLVM and docs/README.txt for an overview of LLVM's
documentation setup.

If you are writing a package for LLVM, see docs/Packaging.rst for our
suggestions.

# PIC16F877
A microcontroller created by MicroChip, more details can be found here: http://www.microchip.com/wwwproducts/en/PIC16F877A

# Quick Start

This repo can be built from source. To do this, follow the offical LLVM guide to building: http://llvm.org/docs/CMake.html

Alternatively this project's binaries can be downloaded:
This executable was built using Debian Jessie, although it should work for any x86 linux operating system.

```
wget https://github.com/loganleland/llvm/releases/download/rc1/pic16rc1.tar.gz
tar -zxvf pic16rc1.tar.gz
PATH=$PATH:$PWD/PIC16rc1
```

# Usage
```
compilePIC16 source.c
```

# Survey
While using this compiler it would be greatly beneficial to the developers of this backend if you fill out the following survey: https://goo.gl/forms/AYxzRiEtKxIy9aBy2

# Limitations
This backend is a preliminary prototype undergoing active development. Pull requests are very welcome.

# Currently Supported:
* Functions
* Variable scoping
* Control Structures
* Variable assignment
* Integer addition/subtraction
* Bitwise operations: &,|,^
* integer comparisons: &&, ||, ==, !=

# Compiler Overview
### Languages

* [ANSI C](http://c-faq.com/ansi/ansi1.html): Everyday C
* [LLVM IR](http://llvm.org/docs/LangRef.html): LLVM's intermediate representation
* [PIC16 ISA](http://ww1.microchip.com/downloads/en/DeviceDoc/39582C.pdf): PIC16F877's ISA

### Compiler Phases
#### `ANSI C`

* `ANSI C` to `LLVM IR` [clang](https://clang.llvm.org/)

#### `LLVM IR`

* `LLVM IR` to `Assembly_1` [llc](http://llvm.org/docs/CommandGuide/llc.html)

#### `Assembly_1`

* `Assembly_1` to `PIC16 ISD` [postLLC](https://github.com/loganleland/llvm/blob/Star_Wars_A_New_NOPE/lib/Target/PIC16/postLLC.sh): Transforms the output assembly into PIC16 (will be removed once assembler is implemented)

#### `PIC16 ISA`

* `PIC16 ISA` to `Byte-code` [mpasm](http://www.microchip.com/developmenttools/getting_started/gs_mplab2.aspx)
