#!/bin/bash

# "$1" is the file that is generated from target backend. It must
# be passed as the only command line argument such as
# ./postLLC.sh test.s



sed -i '1s/^/\npremain:\n  call main\n  quicksand: goto quicksand\n\n/' "$1"
sed -i '1s/^/\ninit:\n  movlw 0x33\n  movwf SP\n  movwf FP\n/' "$1"
sed -i '1s/^/nop\n/' "$1"
sed -i '1s/^/\n\norg 0x000\n/' "$1"
sed -i '1s/^/GPR0 equ 0x20\n/' "$1"
sed -i '1s/^/GPR1 equ 0x21\n/' "$1"
sed -i '1s/^/GPR2 equ 0x22\n/' "$1"
sed -i '1s/^/GPR3 equ 0x23\n/' "$1"
sed -i '1s/^/GPR4 equ 0x24\n/' "$1"
sed -i '1s/^/GPR5 equ 0x25\n/' "$1"
sed -i '1s/^/GPR6 equ 0x26\n/' "$1"
sed -i '1s/^/GPR7 equ 0x27\n/' "$1"
sed -i '1s/^/GPR8 equ 0x28\n/' "$1"
sed -i '1s/^/GPR9 equ 0x29\n/' "$1"
sed -i '1s/^/GPR10 equ 0x30\n/' "$1"

sed -i '1s/^/TRISB equ 0x06\n/' "$1"
sed -i '1s/^/PORTB equ 0x06\n/' "$1"
sed -i '1s/^/FP   equ 0x31\n/' "$1"
sed -i '1s/^/SP   equ 0x32\n/' "$1"
sed -i '1s/^/W    equ 0x0\n/' "$1"
sed -i '1s/^/F    equ 0x1\n/' "$1"
sed -i '1s/^/Z    equ 0x2\n/' "$1"
sed -i '1s/^/C    equ 0x0\n/' "$1"
sed -i '1s/^/FSR  equ 0x04\n/' "$1"
sed -i '1s/^/INDF equ 0x00\n/' "$1"
sed -i '1s/^/STATUS equ 0x03\n/' "$1"
sed -i '1s/^/__CONFIG 0x3F39\n\n\n/' "$1"
sed -i '1s/^/list p=16f877\n/' "$1"


# This creates a backup of the input file, $1, and removes
# the ".text" line created from assembly generation
sed -i.bak '/.text/d' "$1"
rm "$1".bak

sed -i.bak '/.globl/d' "$1"
rm "$1".bak

# This adds the "end" directive at the end of the assembly file
echo end >> "$1"

# remove '.' from file to correct the labels generated
sed -i s/[.]//g "$1"

mv "$1" "${1/.s/.asm}"
