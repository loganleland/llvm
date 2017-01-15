//===-- PIC16MCTargetDesc.h - PIC16 Target Descriptions -------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides PIC16 specific target descriptions.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_PIC16_MCTARGETDESC_PIC16MCTARGETDESC_H
#define LLVM_LIB_TARGET_PIC16_MCTARGETDESC_PIC16MCTARGETDESC_H

#include "llvm/Support/DataTypes.h"

namespace llvm {
class Target;

extern Target ThePIC16Target;

} // End llvm namespace

// Defines symbolic names for PIC16 registers.
// This defines a mapping from register name to register number.
#define GET_REGINFO_ENUM
#include "PIC16GenRegisterInfo.inc"

// Defines symbolic names for the PIC16 instructions.
#define GET_INSTRINFO_ENUM
#include "PIC16GenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "PIC16GenSubtargetInfo.inc"

#endif
