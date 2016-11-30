#!/bin/bash

# "$1" is the file that is generated from target backend. It must
# be passed as the only command line argument such as
# ./postLLC.sh test.s

# This creates a backup of the input file, $1, and removes
# the ".text" line created from assembly generation
sed -i.bak '/.text/d' "$1"
rm "$1".bak


### FIX ME ###
# This will find the end of the main function and add
# "here: goto here" to account for the return of main
#sed '/^main:(\s|.)*return/a\
#  here: goto here
#  ' "$1"

# This adds the "end" directive at the end of the assembly file
echo end >> "$1"

