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

class NOPEReg<bits<4> num, string n> : Register<n> {
  field bits<4> Num = num;
  let Namespace = "NOPE";
}

class NOPERegWithSubregs<bits<4> num, string n, list<Register> subregs> 
  : RegisterWithSubRegs<n, subregs> {
  field bits<4> Num = num;
  let Namespace = "NOPE";
}

//===----------------------------------------------------------------------===//
//  Registers
//===----------------------------------------------------------------------===//

def PCB  : NOPEReg<0,  "r0">;
def SPB  : NOPEReg<1,  "r1">;
def SRB  : NOPEReg<2,  "r2">;
def CGB  : NOPEReg<3,  "r3">;
def FPB  : NOPEReg<4,  "r4">;
def R5B  : NOPEReg<5,  "r5">;
def R6B  : NOPEReg<6,  "r6">;
def R7B  : NOPEReg<7,  "r7">;
def R8B  : NOPEReg<8,  "r8">;
def R9B  : NOPEReg<9,  "r9">;
def R10B : NOPEReg<10, "r10">;
def R11B : NOPEReg<11, "r11">;
def R12B : NOPEReg<12, "r12">;
def R13B : NOPEReg<13, "r13">;
def R14B : NOPEReg<14, "r14">;
def R15B : NOPEReg<15, "r15">;

def subreg_8bit : SubRegIndex<8> { let Namespace = "NOPE"; }

let SubRegIndices = [subreg_8bit] in {
def PC  : NOPERegWithSubregs<0,  "r0",  [PCB]>;
def SP  : NOPERegWithSubregs<1,  "r1",  [SPB]>;
def SR  : NOPERegWithSubregs<2,  "r2",  [SRB]>;
def CG  : NOPERegWithSubregs<3,  "r3",  [CGB]>;
def FP  : NOPERegWithSubregs<4,  "r4",  [FPB]>;
def R5  : NOPERegWithSubregs<5,  "r5",  [R5B]>;
def R6  : NOPERegWithSubregs<6,  "r6",  [R6B]>;
def R7  : NOPERegWithSubregs<7,  "r7",  [R7B]>;
def R8  : NOPERegWithSubregs<8,  "r8",  [R8B]>;
def R9  : NOPERegWithSubregs<9,  "r9",  [R9B]>;
def R10 : NOPERegWithSubregs<10, "r10", [R10B]>;
def R11 : NOPERegWithSubregs<11, "r11", [R11B]>;
def R12 : NOPERegWithSubregs<12, "r12", [R12B]>;
def R13 : NOPERegWithSubregs<13, "r13", [R13B]>;
def R14 : NOPERegWithSubregs<14, "r14", [R14B]>;
def R15 : NOPERegWithSubregs<15, "r15", [R15B]>;
}

def GR8 : RegisterClass<"NOPE", [i8], 8,
   // Volatile registers
  (add R12B, R13B, R14B, R15B, R11B, R10B, R9B, R8B, R7B, R6B, R5B,
   // Frame pointer, sometimes allocable
   FPB,
   // Volatile, but not allocable
   PCB, SPB, SRB, CGB)>;

def GR16 : RegisterClass<"NOPE", [i16], 16,
   // Volatile registers
  (add R12, R13, R14, R15, R11, R10, R9, R8, R7, R6, R5,
   // Frame pointer, sometimes allocable
   FP,
   // Volatile, but not allocable
   PC, SP, SR, CG)>;
