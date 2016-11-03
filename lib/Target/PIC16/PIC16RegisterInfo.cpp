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
#include "PIC16MachineFunctionInfo.h"
#include "PIC16Subtarget.h"
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

PIC16RegisterInfo::PIC16RegisterInfo() : PIC16GenRegisterInfo(PIC16::RA) {}

/*const MCPhysReg*
PIC16RegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  return //CALLEE SAVED REGISTERS - SEE CallingConv.td;
}

const uint32_t *
PIC16RegisterInfo::getCallPreservedMask(const MachineFunction &MF,
                                        CallingConv::ID CC) const {
  return //CALL PRESERVED MASK;
}

const uint32_t*
PIC16RegisterInfo::getRTCallPreservedMask(CallingConv::ID CC) const {
  return //RT CALL PRESERVED MASK;
}

BitVector PIC16RegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());
  
  return Reserved;
}

// There is one pointer register for files,
// and another pair of pointer registers for data.
const TargetRegisterClass*
PIC16RegisterInfo::getPointerRegClass(const MachineFunction &MF,
                                      unsigned Kind) const {
  const PIC16Subtarget &Subtarget = MF.getSubtarget<PIC16Subtarget>();
  return &PIC16::GPRRegClass;
}*/
