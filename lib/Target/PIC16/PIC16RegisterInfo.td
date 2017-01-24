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

def PCB   : PIC16Reg<0,  "r0">;
def SPB   : PIC16Reg<1,  "r1">;
def SRB   : PIC16Reg<2,  "r2">;
def CGB   : PIC16Reg<3,  "r3">;
def FPB   : PIC16Reg<4,  "r4">;
def R5B   : PIC16Reg<5,  "r5">;
def R6B   : PIC16Reg<6,  "r6">;
def R7B   : PIC16Reg<7,  "r7">;
def R8B   : PIC16Reg<8,  "r8">;
def R9B   : PIC16Reg<9,  "r9">;
def R10B  : PIC16Reg<10, "r10">;
def R11B  : PIC16Reg<11, "r11">;
def GPR1B  : PIC16Reg<12, "GPR1">;
def GPR2B  : PIC16Reg<13, "GPR2">;
def GPR3B  : PIC16Reg<14, "GPR3">;
def GPR4B  : PIC16Reg<15, "GPR4">;
def FSRB  : PIC16Reg<16, "FSR">;
def INDFB : PIC16Reg<17, "INDF">;

def subreg_8bit : SubRegIndex<8> { let Namespace = "PIC16"; }

let SubRegIndices = [subreg_8bit] in {
def PC   : PIC16RegWithSubregs<0,  "PC",  [PCB]>;
def SP   : PIC16RegWithSubregs<1,  "SP",  [SPB]>;
def SR   : PIC16RegWithSubregs<2,  "SR",  [SRB]>;
def CG   : PIC16RegWithSubregs<3,  "CG",  [CGB]>;
def FP   : PIC16RegWithSubregs<4,  "FP",  [FPB]>;
def R5   : PIC16RegWithSubregs<5,  "r5",  [R5B]>;
def R6   : PIC16RegWithSubregs<6,  "r6",  [R6B]>;
def R7   : PIC16RegWithSubregs<7,  "r7",  [R7B]>;
def R8   : PIC16RegWithSubregs<8,  "r8",  [R8B]>;
def R9   : PIC16RegWithSubregs<9,  "r9",  [R9B]>;
def R10  : PIC16RegWithSubregs<10, "r10", [R10B]>;
def R11  : PIC16RegWithSubregs<11, "r11", [R11B]>;
def GPR1  : PIC16RegWithSubregs<12, "GPR1", [GPR1B]>;
def GPR2  : PIC16RegWithSubregs<13, "GPR2", [GPR2B]>;
def GPR3  : PIC16RegWithSubregs<14, "GPR3", [GPR3B]>;
def GPR4  : PIC16RegWithSubregs<15, "GPR4", [GPR3B]>;
def FSR  : PIC16RegWithSubregs<16, "FSR", [FSRB]>;
def INDF : PIC16RegWithSubregs<17, "INDF", [INDFB]>;
}

def GR8 : RegisterClass<"PIC16", [i8], 8,
   // Volatile registers
  (add GPR1B, GPR2B, GPR3B, GPR4B, R11B, R10B, R9B, R8B, R7B, R6B, R5B,
   // Frame pointer, sometimes allocable
   FPB,FSRB,INDFB,
   // Volatile, but not allocable
   PCB, SPB, SRB, CGB)>;

def GR16 : RegisterClass<"PIC16", [i16], 16,
   // Volatile registers
  (add GPR1, GPR2, GPR3, GPR4, R11, R10, R9, R8, R7, R6, R5,
   // Frame pointer, sometimes allocable
   FP,FSR,INDF,
   // Volatile, but not allocable
   PC, SP, SR, CG)>;
 
def WRegClass : RegisterClass<"PIC16", [i8], 8,
    (add WReg)>;
