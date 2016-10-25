//===-- PIC16RegisterInfo.cpp - PIC16 Register Information ----------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the PIC16 implementation of the TargetRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#include "PIC16RegisterInfo.h"
#include "PIC16.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Target/TargetInstrInfo.h"

using namespace llvm;

#define GET_REGINFO_TARGET_DESC
#include "PIC16GenRegisterInfo.inc"

--------

PIC16RegisterInfo::PIC16RegisterInfo() : PIC16GenRegisterInfo(SP::O7) {}

const MCPhysReg*
PIC16RegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  return CSR_SaveList;
}

const uint32_t *
PIC16RegisterInfo::getCallPreservedMask(const MachineFunction &MF,
                                        CallingConv::ID CC) const {
  return CSR_RegMask;
}

const uint32_t*
PIC16RegisterInfo::getRTCallPreservedMask(CallingConv::ID CC) const {
  return RTCSR_RegMask;
}

BitVector PIC16RegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());
  
  return Reserved;
}

