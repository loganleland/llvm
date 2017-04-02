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
Download and Usage
==================

- Note: This is a very crude way of usage. This will change in the future.

To Download:
============
1) This was built on Ubuntu 16.04.02. It should work for most Linux
   distributions.

2) Navigate to the release tab and download 'pic16beta.zip'

3) Unzip the build and place it in your home directory, that is, the file path
   that looks like '~/'
   - The current way we are handling the usage of the compiler is to enforce
     the build is placed in that specific location.

To Use:
=======
1) Have a C source file ready to go.

2) At the command line type:
   ~/pic16beta/compile.sh /path/to/input/file

   - This will execute the necessary steps needed to translate and lower the
     C code to acceptable PIC16 ASM code.

3) If your input file was named 'input.c', you should now have a file named
   'input.asm' in the same directory that you C file was in.
   - You should now be able to set up a PIC16 project in the MPLAB
     IDE (version 1.8), load the ASM file into the editor, and build/run it for
     a PIC16F877 microcontroller.

4) A survey will open up immediately following the execution of the compile
   command. You should fill this out after you have viewed and tested the
   ASM file to see if it met your expectations.

=========================
How the Execution Happens
=========================

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
