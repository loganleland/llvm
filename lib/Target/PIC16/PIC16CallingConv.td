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
// PIC16 Return Value Calling Convention
//===----------------------------------------------------------------------===//
def RetCC_PIC16 : CallingConv<[
  // i8 are returned in the bottom 6 GPR registers in the first bank
  CCIfType<[i8], CCAssignToReg<[GPR106, GPR107, GPR108, GPR109, GPR110, GPR111]>>
]>;

//===----------------------------------------------------------------------===//
// PIC16 Argument Calling Conventions
//===----------------------------------------------------------------------===//
def CC_PIC16 : CallingConv<[

  //The first four integer arguments are passed in 4 registers
  //in the bottom section of the GPR's in the first bank
  CCIfType<[i8], CCAssignToReg<GPR102, GPR103, GPR104, GPR105>>,

  //Integer Values get stored in stack slots (bottom GPR in first bank),
  //that are 1 byte in size and 4-byte aligned
  CCIfType<[i8], CCAssignToStack<1,4>>
]>;
