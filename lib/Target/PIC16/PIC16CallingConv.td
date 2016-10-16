//==- PIC16CallingConv.td - Calling Conventions for PIC16 -*- tablegen -*-==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
// This describes the calling conventions for PIC16 architecture.
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// PIC16 Return Value Calling Convention (currently only supports the first
// bank of memory
//===----------------------------------------------------------------------===//
def RetCC_PIC16 : CallingConv<[
  // i8 are returned in GPR registers in the first bank
  foreach Index = 32-111 in{
  CCIfType<[i8], CCAssignToReg<["GPR"#Index>]>>,
]>;

//===----------------------------------------------------------------------===//
// PIC16 Argument Calling Conventions
//===----------------------------------------------------------------------===//
//Implement once we are ready for functions
