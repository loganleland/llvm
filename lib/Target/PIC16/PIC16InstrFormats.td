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

//===----------------------------------------------------------------------===//
// Instruction types
//===----------------------------------------------------------------------===//

// Byte Oriented Operations
// Format: [opcode|d|f]
class PIC16ByteInst<bits<6> opcode, dag outs, dag ins, string asmstr,
  list<dag> pattern> : PIC16Inst<outs, ins, asmstr, pattern>
{
  bit     d;      // Direction bit
  bits<7> f;      // Length of file register address

  // Assign format to Instruction
  let Inst{13-8} = opcode;
  let Inst{7}    = d;
  let Inst{6-0}  = f;
}

// Bit Oriented Operations
// Format: [opcode|b|f]
class PIC16BitInst<bits<4> opcode, dag outs, dag ins, string asmstr,
  list<dag> pattern> : PIC16Inst<outs, ins, asmstr, pattern>
{
  bits<3> b;      // Length of selection of bit
  bits<7> f;      // Length of file register address

  // Assign format to Instruction
  let Inst{13-10} = opcode;
  let Inst{9-7}   = b;
  let Inst{6-0}   = f;
}

// Control Operations
// Format: [opcode|k]
class PIC16ControlInst<bits<3> opcode, dag outs, dag ins, string asmstr,
  list<dag> pattern> : PIC16Inst<outs, ins, asmstr, pattern>
{
  bits<11> k;

  // Assign format to Instruction
  let Inst{13-11} = opcode;
  let Inst{10-0} = k;
}

// Literal Operations
// Format: [opcode|k]
class PIC16LiteralInst<bits<6> opcode, dag outs, dag ins, string asmstr,
  list<dag> pattern> : PIC16Inst<outs, ins, asmstr, pattern>
{
  bits<8> k;      // Length of literal

  // Assign format to Instruction
  let Inst{13-8} = opcode;
  let Inst{7-0}  = k;
}

// Pseudo Instruction format
class PIC16PseudoInst<dag outs, dag ins, string asmstr, list<dag> pattern>
  : PIC16Inst<outs, ins, asmstr, pattern>
{
  // Property set to true
  let isPseudo = 1;

}
