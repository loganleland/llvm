//===-- NOPERegisterInfo.cpp - NOPE Register Information --------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the NOPE implementation of the TargetRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#include "NOPERegisterInfo.h"
#include "NOPE.h"
#include "NOPEMachineFunctionInfo.h"
#include "NOPETargetMachine.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"

using namespace llvm;

#define DEBUG_TYPE "nope-reg-info"

#define GET_REGINFO_TARGET_DESC
#include "NOPEGenRegisterInfo.inc"

// FIXME: Provide proper call frame setup / destroy opcodes.
NOPERegisterInfo::NOPERegisterInfo()
  : NOPEGenRegisterInfo(NOPE::PC) {}

const MCPhysReg*
NOPERegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  const NOPEFrameLowering *TFI = getFrameLowering(*MF);
  const Function* F = MF->getFunction();
  static const MCPhysReg CalleeSavedRegs[] = {
    NOPE::FP, NOPE::R5, NOPE::R6, NOPE::R7,
    NOPE::R8, NOPE::R9, NOPE::R10, NOPE::R11,
    0
  };
  static const MCPhysReg CalleeSavedRegsFP[] = {
    NOPE::R5, NOPE::R6, NOPE::R7,
    NOPE::R8, NOPE::R9, NOPE::R10, NOPE::R11,
    0
  };
  static const MCPhysReg CalleeSavedRegsIntr[] = {
    NOPE::FP,  NOPE::R5,  NOPE::R6,  NOPE::R7,
    NOPE::R8,  NOPE::R9,  NOPE::R10, NOPE::R11,
    NOPE::R12, NOPE::R13, NOPE::R14, NOPE::R15,
    0
  };
  static const MCPhysReg CalleeSavedRegsIntrFP[] = {
    NOPE::R5,  NOPE::R6,  NOPE::R7,
    NOPE::R8,  NOPE::R9,  NOPE::R10, NOPE::R11,
    NOPE::R12, NOPE::R13, NOPE::R14, NOPE::R15,
    0
  };

  if (TFI->hasFP(*MF))
    return (F->getCallingConv() == CallingConv::NOPE_INTR ?
            CalleeSavedRegsIntrFP : CalleeSavedRegsFP);
  else
    return (F->getCallingConv() == CallingConv::NOPE_INTR ?
            CalleeSavedRegsIntr : CalleeSavedRegs);

}

BitVector NOPERegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());
  const NOPEFrameLowering *TFI = getFrameLowering(MF);

  // Mark 4 special registers with subregisters as reserved.
  Reserved.set(NOPE::PCB);
  Reserved.set(NOPE::SPB);
  Reserved.set(NOPE::SRB);
  Reserved.set(NOPE::CGB);
  Reserved.set(NOPE::PC);
  Reserved.set(NOPE::SP);
  Reserved.set(NOPE::SR);
  Reserved.set(NOPE::CG);

  // Mark frame pointer as reserved if needed.
  if (TFI->hasFP(MF)) {
    Reserved.set(NOPE::FPB);
    Reserved.set(NOPE::FP);
  }

  return Reserved;
}

const TargetRegisterClass *
NOPERegisterInfo::getPointerRegClass(const MachineFunction &MF, unsigned Kind)
                                                                         const {
  return &NOPE::GR16RegClass;
}

void
NOPERegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                        int SPAdj, unsigned FIOperandNum,
                                        RegScavenger *RS) const {
  assert(SPAdj == 0 && "Unexpected");

  MachineInstr &MI = *II;
  MachineBasicBlock &MBB = *MI.getParent();
  MachineFunction &MF = *MBB.getParent();
  const NOPEFrameLowering *TFI = getFrameLowering(MF);
  DebugLoc dl = MI.getDebugLoc();
  int FrameIndex = MI.getOperand(FIOperandNum).getIndex();

  unsigned BasePtr = (TFI->hasFP(MF) ? NOPE::FP : NOPE::SP);
  int Offset = MF.getFrameInfo()->getObjectOffset(FrameIndex);

  // Skip the saved PC
  Offset += 2;

  if (!TFI->hasFP(MF))
    Offset += MF.getFrameInfo()->getStackSize();
  else
    Offset += 2; // Skip the saved FP

  // Fold imm into offset
  Offset += MI.getOperand(FIOperandNum + 1).getImm();

  if (MI.getOpcode() == NOPE::ADD16ri) {
    // This is actually "load effective address" of the stack slot
    // instruction. We have only two-address instructions, thus we need to
    // expand it into mov + add
    const TargetInstrInfo &TII = *MF.getSubtarget().getInstrInfo();

    MI.setDesc(TII.get(NOPE::MOV16rr));
    MI.getOperand(FIOperandNum).ChangeToRegister(BasePtr, false);

    if (Offset == 0)
      return;

    // We need to materialize the offset via add instruction.
    unsigned DstReg = MI.getOperand(0).getReg();
    if (Offset < 0)
      BuildMI(MBB, std::next(II), dl, TII.get(NOPE::SUB16ri), DstReg)
        .addReg(DstReg).addImm(-Offset);
    else
      BuildMI(MBB, std::next(II), dl, TII.get(NOPE::ADD16ri), DstReg)
        .addReg(DstReg).addImm(Offset);

    return;
  }

  MI.getOperand(FIOperandNum).ChangeToRegister(BasePtr, false);
  MI.getOperand(FIOperandNum + 1).ChangeToImmediate(Offset);
}

unsigned NOPERegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  const NOPEFrameLowering *TFI = getFrameLowering(MF);
  return TFI->hasFP(MF) ? NOPE::FP : NOPE::SP;
}
