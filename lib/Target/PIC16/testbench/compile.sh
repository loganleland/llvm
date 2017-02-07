clang -S -emit-llvm -target pic16 submi.c
llc -mcpu=generic -march=pic16 submi.ll
/home/logan/llvm/lib/Target/PIC16/postLLC.sh submi.s
