//===-- NOPERegisterInfo.td - NOPE Register defs -------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source 
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//  Declarations that describe the NOPE register file
//===----------------------------------------------------------------------===//

class NOPEReg<bits<8> num, string n> : Register<n> {
  field bits<8> Num = num;
  let Namespace = "NOPE";
}

class NOPERegWithSubregs<bits<8> num, string n, list<Register> subregs> 
  : RegisterWithSubRegs<n, subregs> {
  field bits<8> Num = num;
  let Namespace = "NOPE";
}

//===----------------------------------------------------------------------===//
//  Registers
//===----------------------------------------------------------------------===//

def PCB   : NOPEReg<0,  "r0">;
def SPB   : NOPEReg<1,  "r1">;
def SRB   : NOPEReg<2,  "r2">;
def CGB   : NOPEReg<3,  "r3">;
def FPB   : NOPEReg<4,  "r4">;
def R5B   : NOPEReg<5,  "r5">;
def R6B   : NOPEReg<6,  "r6">;
def R7B   : NOPEReg<7,  "r7">;
def R8B   : NOPEReg<8,  "r8">;
def R9B   : NOPEReg<9,  "r9">;
def R10B  : NOPEReg<10, "r10">;
def R11B  : NOPEReg<11, "r11">;
def GPR1B  : NOPEReg<12, "GPR1">;
def GPR2B  : NOPEReg<13, "GPR2">;
def GPR3B  : NOPEReg<14, "GPR3">;
def GPR4B  : NOPEReg<15, "GPR4">;
def FSRB  : NOPEReg<16, "FSR">;
def INDFB : NOPEReg<17, "INDF">;

def subreg_8bit : SubRegIndex<8> { let Namespace = "NOPE"; }

let SubRegIndices = [subreg_8bit] in {
def PC   : NOPERegWithSubregs<0,  "PC",  [PCB]>;
def SP   : NOPERegWithSubregs<1,  "SP",  [SPB]>;
def SR   : NOPERegWithSubregs<2,  "SR",  [SRB]>;
def CG   : NOPERegWithSubregs<3,  "CG",  [CGB]>;
def FP   : NOPERegWithSubregs<4,  "FP",  [FPB]>;
def R5   : NOPERegWithSubregs<5,  "r5",  [R5B]>;
def R6   : NOPERegWithSubregs<6,  "r6",  [R6B]>;
def R7   : NOPERegWithSubregs<7,  "r7",  [R7B]>;
def R8   : NOPERegWithSubregs<8,  "r8",  [R8B]>;
def R9   : NOPERegWithSubregs<9,  "r9",  [R9B]>;
def R10  : NOPERegWithSubregs<10, "r10", [R10B]>;
def R11  : NOPERegWithSubregs<11, "r11", [R11B]>;
def GPR1  : NOPERegWithSubregs<12, "GPR1", [GPR1B]>;
def GPR2  : NOPERegWithSubregs<13, "GPR2", [GPR2B]>;
def GPR3  : NOPERegWithSubregs<14, "GPR3", [GPR3B]>;
def GPR4  : NOPERegWithSubregs<15, "GPR4", [GPR3B]>;
def FSR  : NOPERegWithSubregs<16, "FSR", [FSRB]>;
def INDF : NOPERegWithSubregs<17, "INDF", [INDFB]>;
}

def GR8 : RegisterClass<"NOPE", [i8], 8,
   // Volatile registers
  (add GPR1B, GPR2B, GPR3B, GPR4B, R11B, R10B, R9B, R8B, R7B, R6B, R5B,
   // Frame pointer, sometimes allocable
   FPB,FSRB,INDFB,
   // Volatile, but not allocable
   PCB, SPB, SRB, CGB)>;

def GR16 : RegisterClass<"NOPE", [i16], 16,
   // Volatile registers
  (add GPR1, GPR2, GPR3, GPR4, R11, R10, R9, R8, R7, R6, R5,
   // Frame pointer, sometimes allocable
   FP,FSR,INDF,
   // Volatile, but not allocable
   PC, SP, SR, CG)>;
