//===-- PIC16RegisterInfo.h - PIC16 Register Information Impl -*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the PIC16 implementation of the MRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_PIC16_PIC16REGISTERINFO_H
#define LLVM_LIB_TARGET_PIC16_PIC16REGISTERINFO_H

#include "llvm/Target/TargetRegisterInfo.h"

#define GET_REGINFO_HEADER
#include "PIC16GenRegisterInfo.inc"

namespace llvm {

struct PIC16RegisterInfo : public PIC16GenRegisterInfo {
public:
  PIC16RegisterInfo();

  /// Code Generation virtual methods...
  const MCPhysReg *getCalleeSavedRegs(const MachineFunction *MF) const override;

  BitVector getReservedRegs(const MachineFunction &MF) const override;
  const TargetRegisterClass*
  getPointerRegClass(const MachineFunction &MF,
                     unsigned Kind = 0) const override;

  void eliminateFrameIndex(MachineBasicBlock::iterator II,
                           int SPAdj, unsigned FIOperandNum,
                           RegScavenger *RS = nullptr) const override;

  // Debug information queries.
  unsigned getFrameRegister(const MachineFunction &MF) const override;
};

} // end namespace llvm

#endif
