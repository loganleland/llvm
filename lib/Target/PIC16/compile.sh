#!/bin/bash

# The variable $1 should be the only command line argument which is the input
# file for the compiler. It should be in the format of C. The output of
# the compiler will be the input file with the extension changed to .asm

# run clang targeting pic16 on input C file
./bin/clang -S -emit-llvm -target pic16 $1

# run llc targeting pic16 on LLVM IR file generated from clang
./bin/llc -mcpu=generic -march=pic16 ${1%.c}.ll

# run the post processing script to tidy up the assembly .s file and rename
# it to a .asm assembly file
./postLLC.sh ${1%.c}.s

# open to survey
sleep 3
xdg-open https://goo.gl/forms/AYxzRiEtKxIy9aBy2
