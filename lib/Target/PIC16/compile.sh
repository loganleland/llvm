#!/bin/bash

# The variable $1 should be the only command line argument which is the input
# file for the compiler. It should be in the format of C. The output of
# the compiler will be the input file with the extension changed to .asm

# Timestamp
DATE=$(date +%F_%H_%M_%S)

# Output from llc destination as variable
dest=${1%.c}.s

# Clang targeting pic16
~/pic16beta/bin/clang -S -emit-llvm -target pic16 $1 -o ${1%.c}.ll

# llc targeting pic16. Input is the output from clang
~/pic16beta/bin/llc -mcpu=generic -march=pic16 ${1%.c}.ll -o ${1%.c}.s
#mv ./output.log ./$DATE.log
rm ${1%.c}.ll

# Code that is in postLLC.sh
# Post processing. Input is the output from llc
sed -i '1s/^/\npremain:\n  call main\n  quicksand: goto quicksand\n\n/' "$dest"
sed -i '1s/^/\ninit:\n  movlw 0x33\n  movwf SP\n  movwf FP\n/' "$dest"
sed -i '1s/^/nop\n/' "$dest"
sed -i '1s/^/\n\norg 0x000\n/' "$dest"
sed -i '1s/^/GPR0 equ 0x20\n/' "$dest"
sed -i '1s/^/GPR1 equ 0x21\n/' "$dest"
sed -i '1s/^/GPR2 equ 0x22\n/' "$dest"
sed -i '1s/^/GPR3 equ 0x23\n/' "$dest"
sed -i '1s/^/GPR4 equ 0x24\n/' "$dest"
sed -i '1s/^/GPR5 equ 0x25\n/' "$dest"
sed -i '1s/^/GPR6 equ 0x26\n/' "$dest"
sed -i '1s/^/GPR7 equ 0x27\n/' "$dest"
sed -i '1s/^/GPR8 equ 0x28\n/' "$dest"
sed -i '1s/^/GPR9 equ 0x29\n/' "$dest"
sed -i '1s/^/GPR10 equ 0x30\n/' "$dest"

sed -i '1s/^/TRISB equ 0x06\n/' "$dest"
sed -i '1s/^/PORTB equ 0x06\n/' "$dest"
sed -i '1s/^/FP   equ 0x31\n/' "$dest"
sed -i '1s/^/SP   equ 0x32\n/' "$dest"
sed -i '1s/^/W    equ 0x0\n/' "$dest"
sed -i '1s/^/F    equ 0x1\n/' "$dest"
sed -i '1s/^/Z    equ 0x2\n/' "$dest"
sed -i '1s/^/FSR  equ 0x04\n/' "$dest"
sed -i '1s/^/INDF equ 0x00\n/' "$dest"
sed -i '1s/^/STATUS equ 0x03\n/' "$dest"
sed -i '1s/^/__CONFIG 0x3F39\n\n\n/' "$dest"
sed -i '1s/^/list p=16f877\n/' "$dest"

sed -i.bak '/.text/d' "$dest"
rm "$dest".bak

sed -i.bak '/.globl/d' "$dest"
rm "$dest".bak

echo end >> "$dest"

mv "$dest" "${dest/.s/.asm}"

# Survey
xdg-open https://goo.gl/forms/AYxzRiEtKxIy9aBy2
