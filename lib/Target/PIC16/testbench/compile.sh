clang -S -emit-llvm -target pic16 xormi.c
llc -mcpu=generic -march=pic16 xormi.ll
/home/logan/llvm/lib/Target/PIC16/postLLC.sh xormi.s
