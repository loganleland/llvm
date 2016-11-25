//===-- NOPEMCTargetDesc.h - NOPE Target Descriptions -------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides NOPE specific target descriptions.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_NOPE_MCTARGETDESC_NOPEMCTARGETDESC_H
#define LLVM_LIB_TARGET_NOPE_MCTARGETDESC_NOPEMCTARGETDESC_H

#include "llvm/Support/DataTypes.h"

namespace llvm {
class Target;

extern Target TheNOPETarget;

} // End llvm namespace

// Defines symbolic names for NOPE registers.
// This defines a mapping from register name to register number.
#define GET_REGINFO_ENUM
#include "NOPEGenRegisterInfo.inc"

// Defines symbolic names for the NOPE instructions.
#define GET_INSTRINFO_ENUM
#include "NOPEGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "NOPEGenSubtargetInfo.inc"

#endif
