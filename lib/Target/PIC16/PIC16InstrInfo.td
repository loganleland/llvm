//===- PIC16InstrInfo.td - PIC16 Instruction defs -------------*- tblgen-*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source 
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file describes the PIC16 instructions in TableGen format.
//
//===----------------------------------------------------------------------===//


//===----------------------------------------------------------------------===//
// Instruction format superclass
//===----------------------------------------------------------------------===//

include "PIC16InstrFormats.td"

//===----------------------------------------------------------------------===//

// NOP - subject to change
let hasSideEffects = 0 in
def NOP : PIC16PseudoInst<(outs), (ins), "nop", []>;


// File Register Instructions



// Bit-oriented Instructions


// GOTO and CALL


// Literal Instructions
