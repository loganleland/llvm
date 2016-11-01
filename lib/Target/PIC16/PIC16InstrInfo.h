//===-- PIC16InstrInfo.h - PIC16 Instruction Information --------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the PIC16 implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_PIC16_PIC16INSTRINFO_H
#define LLVM_LIB_TARGET_PIC16_PIC16INSTRINFO_H

#include "PIC16RegisterInfo.h"
#include "llvm/Target/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "PIC16GenInstrInfo.inc"

namespace llvm {

class PIC16InstrInfo : public PIC16GenInstrInfo {
  const PIC16RegisterInfo RegisterInfo;

public:
  PIC16InstrInfo();

  virtual const PIC16RegisterInfo &getRegisterInfo() const {
    return RegisterInfo;
  }
}

} // namespace llvm

#endif // LLVM_LIB_TARGET_PIC16_PIC16INSTRINFO_H
