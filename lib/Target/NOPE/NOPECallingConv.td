//==- NOPECallingConv.td - Calling Conventions for NOPE -*- tablegen -*-==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
// This describes the calling conventions for NOPE architecture.
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// NOPE Return Value Calling Convention
//===----------------------------------------------------------------------===//
def RetCC_NOPE : CallingConv<[
  // i8 are returned in registers GPR4B, GPR3B, GPR2B, GPR1B
  CCIfType<[i8], CCAssignToReg<[GPR4B, GPR3B, GPR2B, GPR1B]>>,

  // i16 are returned in registers R15, R14, R13, GPR1
  CCIfType<[i16], CCAssignToReg<[GPR4, GPR3, GPR2, GPR1]>>
]>;

//===----------------------------------------------------------------------===//
// NOPE Argument Calling Conventions
//===----------------------------------------------------------------------===//
def CC_NOPE_AssignStack : CallingConv<[
  // Pass by value if the byval attribute is given
  CCIfByVal<CCPassByVal<2, 2>>,

  // Promote i8 arguments to i16.
  CCIfType<[i8], CCPromoteToType<i16>>,

  // Integer values get stored in stack slots that are 2 bytes in
  // size and 2-byte aligned.
  CCIfType<[i16], CCAssignToStack<2, 2>>
]>;

