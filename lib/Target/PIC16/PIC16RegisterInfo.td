//===-- PIC16RegisterInfo.td - PIC16 Register defs -------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source 
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//  Declarations that describe the PIC16 register file
//===----------------------------------------------------------------------===//

class PIC16Reg<bits<8> num, string n> : Register<n> {
  field bits<8> Num = num;
  let Namespace = "PIC16";
}

class PIC16RegWithSubregs<bits<8> num, string n, list<Register> subregs> 
  : RegisterWithSubRegs<n, subregs> {
  field bits<8> Num = num;
  let Namespace = "PIC16";
}

//===----------------------------------------------------------------------===//
//  Registers
//===----------------------------------------------------------------------===//

def WReg  : PIC16Reg<200, "W">;

def PC   : PIC16Reg<0,  "r0">;
def SP   : PIC16Reg<1,  "r1">;
def SR   : PIC16Reg<2,  "r2">;
def CG   : PIC16Reg<3,  "r3">;
def FP   : PIC16Reg<4,  "r4">;
def R5   : PIC16Reg<5,  "r5">;
def R6   : PIC16Reg<6,  "r6">;
def R7   : PIC16Reg<7,  "r7">;
def R8   : PIC16Reg<8,  "r8">;
def R9   : PIC16Reg<9,  "r9">;
def R10  : PIC16Reg<10, "r10">;
def R11  : PIC16Reg<11, "r11">;
def GPR1  : PIC16Reg<12, "GPR1">;
def GPR2  : PIC16Reg<13, "GPR2">;
def GPR3  : PIC16Reg<14, "GPR3">;
def GPR4  : PIC16Reg<15, "GPR4">;
def FSR  : PIC16Reg<16, "FSR">;
def INDF : PIC16Reg<17, "INDF">;

def GR8 : RegisterClass<"PIC16", [i8], 8,
   // Volatile registers
  (add GPR1, GPR2, GPR3, GPR4, R11, R10, R9, R8, R7, R6, R5,
   // Frame pointer, sometimes allocable
   FP,FSR,INDF,
   // Volatile, but not allocable
   PC, SP, SR, CG)>;
 
def WRegClass : RegisterClass<"PIC16", [i8], 8,
    (add WReg)>;
