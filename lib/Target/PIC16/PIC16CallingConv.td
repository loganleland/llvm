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
  // i8 are returned in registers GPR4, GPR3, GPR2, GPR1
  CCIfType<[i8], CCAssignToReg<[GPR4, GPR3, GPR2, GPR1]>>

]>;

//===----------------------------------------------------------------------===//
// PIC16 Argument Calling Conventions
//===----------------------------------------------------------------------===//
def CC_PIC16_AssignStack : CallingConv<[
  // Pass by value if the byval attribute is given
  CCIfByVal<CCPassByVal<2, 2>>,

  // Integer values get stored in stack slots that are 2 bytes in
  // size and 2-byte aligned.
  CCIfType<[i8], CCAssignToStack<2, 2>>
]>;
