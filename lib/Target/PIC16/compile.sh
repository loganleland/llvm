#!/bin/bash

# The variable $1 should be the only command line argument which is the input
# file for the compiler. It should be in the format of C. The output of
# the compiler will be the input file with the extension changed to .asm

# Create timestamp variable
DATE=$(date +%F_%H_%M_%S)
echo $DATE

# Run clang targeting pic16 on input C file
./bin/clang -S -emit-llvm -target pic16 $1

# Run llc targeting pic16 on LLVM IR file generated from clang
./bin/llc -mcpu=generic -march=pic16 --stats ${1%.c}.ll 2> output.log
mv ./output.log ./$DATE.log

# Run the post processing script to tidy up the assembly .s file and rename
# it to a .asm assembly file
./postLLC.sh ${1%.c}.s

# Open to survey
xdg-open https://goo.gl/forms/AYxzRiEtKxIy9aBy2
