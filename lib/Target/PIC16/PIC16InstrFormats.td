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

