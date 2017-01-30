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

def PC    : PIC16Reg<0,  "PC">;
def SP    : PIC16Reg<1,  "SP">;
def SR    : PIC16Reg<2,  "SR">;
def CG    : PIC16Reg<3,  "CG">;
def FP    : PIC16Reg<4,  "FP">;
def GPR0  : PIC16Reg<5,  "GPR0">;
def GPR1  : PIC16Reg<6,  "GPR1">;
def GPR2  : PIC16Reg<7,  "GPR2">;
def GPR3  : PIC16Reg<8,  "GPR3">;
def GPR4  : PIC16Reg<9,  "GPR4">;
def GPR5  : PIC16Reg<10, "GPR5">;
def GPR6  : PIC16Reg<11, "GPR6">;
def GPR7  : PIC16Reg<12, "GPR7">;
def GPR8  : PIC16Reg<13, "GPR8">;
def GPR9  : PIC16Reg<14, "GPR9">;
def GPR10 : PIC16Reg<15, "GPR10">;
def FSR   : PIC16Reg<16, "FSR">;
def INDF  : PIC16Reg<17, "INDF">;

def GR8 : RegisterClass<"PIC16", [i8], 8,
   // Volatile registers
  (add GPR0, GPR1, GPR2, GPR3, GPR4, GPR5, GPR6,
       GPR7, GPR8, GPR9, 
   // Frame pointer, sometimes allocable
   FP,FSR,INDF,
   // Volatile, but not allocable
   PC, SP, SR, CG)>;
