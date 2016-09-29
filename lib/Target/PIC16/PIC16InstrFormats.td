//===- PIC16InstrFormats.td - PIC16 Instruction Formats-------*- tblgen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source 
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//  Describe PIC16 instructions format
//
//  All the possible PIC16 fields are:
//
//  opcode  - operation code.
//  f       - 7-bit register file address.
//  d       - 1-bit direction specifier
//  k       - 8/11 bit literals
//  b       - 3 bits bit num specifier
//
//===----------------------------------------------------------------------===//

// A generic 14-bit PIC16 instruction.
class PIC16Inst<dag outs, dag ins, string asmstr, list<dag> pattern>
  : Instruction
{
  let Namespace = "PIC16";

  dag OutOperandList = outs;
  dag InOperandList = ins;
  let AsmString = asmstr;
  let Pattern = pattern;

  field bits<14> Inst;

  let DecoderNamespace = "PIC16";
}

//Instruction types

// Byte Oriented Operations
// Format: [op|d|f]
class PIC16ByteInst<dag outs, dag ins, string asmstr, list<dag> pattern>
  : PIC16Inst<outs, ins, asmstr, pattern>
{
  bits<6> opcode; // Length of opcode
  bit     d;      // Direction bit
  bits<7> f;      // Length of file register address

  // Assign format to Instruction
  let Inst{13-8} = opcode;
  let Inst{7}    = d;
  let Inst{6-0)  = f;
}

// Bit Oriented Operations
// Format: [op|b|f]
class PIC16BitInst<dag outs, dag ins, string asmstr, list<dag> pattern>
  : PIC16Inst<outs, ins, asmstr, pattern>
{
  bits<4> opcode; // Length of opcode
  bits<3> b;      // Length of selection of bit
  bits<7> f;      // Length of file register address

  // Assign format to Instruction
  let Inst{13-10} = opcode;
  let Inst{9-7}   = b;
  let Inst{6-0}   = f;
}




// File Register Instructions
class FRInst<dag outs, dag ins, string asmstr, list<dag> pattern>
  : PIC16Inst<outs, ins, asmstr, pattern>{

  bits<7> f; // File register
  bit     d; // Result direction (0 for W, 1 for file)

  let op = 0b00;

  let Inst{6-0} = f;
}

// Bit-oriented instructions

class BOInst<dag outs, dag ins, string asmstr, list<dag> pattern>
  : PIC16Inst<outs, ins, asmtr, pattern>{

  bits<7> f; // File register
  bits<3> b; // Bit selector (7-0)

  let op = 0b01;

  let Inst{9-7} = b;
  let Inst{6-0} = f;
}

// GOTO and CALL
class GCInst<dag outs, dag ins, string asmstr, list<dag> pattern>
  : PIC16Inst<outs, ins, asmstr, pattern>{

  bits<11> k; // Data Address

  let op = 0b10;

  let Inst{10-0} = k;
}

// Literal instructions
class LitInst<dag outs, dag ins, string asmstr, list<dag> pattern>
  : PIC16Inst<outs, ins, asmstr, pattern>{

  bits<8> k; // Literal

  let op = 0b11;

  let Inst{7-0} = k;
}


