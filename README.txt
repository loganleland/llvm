Forked LLVM Repository to build PIC16 Backend
=============================================

- This repository contains LLVM source with the backend code for the PIC16F877
  microcontroller.

- Most of the files and code specific to the backend can be found in:
  ./lib/Target/PIC16/

- A few extraneous files must be modified to register the target backend for 
  command-line purposes.

- The code for passes on LLVM IR can be found in:
  ./lib/Transforms/

==================
Order of Execution
==================

1) Build LLVM

2) Write a C source file, e.g. hello.c

3) Run the following command using the newly built tools:
   
   clang -S -emit-llvm -target pic16 hello.c

   Expected output:
   hello.ll <--- LLVM IR in human readable format targeting the PIC16

4) Run llc on LLVM IR:
   
   llc hello.ll

   Expected output:
   hello.s <--- Assembly file that lowered LLVM IR to PIC16 instructions as
                defined by the backend

5) There exsits a bash script in the PIC16 source directory called postLLC.sh
   Run this script on the output of llc as so:
   
   postLLC.sh hello.s

   Expected output:
   hello.asm <--- Assembly file that can be accepted by MPLABX IDE v1.8

// FIXME: Add instructions for actually building LLVM with proper flags

Legacy LLVM Readme:
================================
Low Level Virtual Machine (LLVM)
================================

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
