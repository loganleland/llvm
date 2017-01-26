//===-- PIC16RegisterInfo.cpp - PIC16 Register Information --------------===//
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
#include "PIC16TargetMachine.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"

using namespace llvm;

#define DEBUG_TYPE "pic16-reg-info"

#define GET_REGINFO_TARGET_DESC
#include "PIC16GenRegisterInfo.inc"

// FIXME: Provide proper call frame setup / destroy opcodes.
PIC16RegisterInfo::PIC16RegisterInfo()
  : PIC16GenRegisterInfo(PIC16::PC) {}

const MCPhysReg*
PIC16RegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  const PIC16FrameLowering *TFI = getFrameLowering(*MF);
  const Function* F = MF->getFunction();
  static const MCPhysReg CalleeSavedRegs[] = {
    PIC16::FP, PIC16::GPR0, PIC16::GPR1, PIC16::GPR2,
    PIC16::GPR3, PIC16::GPR4, PIC16::GPR5, PIC16::GPR6,
    0
  };
  static const MCPhysReg CalleeSavedRegsFP[] = {
    PIC16::GPR0, PIC16::GPR1, PIC16::GPR2,
    PIC16::GPR3, PIC16::GPR4, PIC16::GPR5, PIC16::GPR6,
    0
  };
  static const MCPhysReg CalleeSavedRegsIntr[] = {
    PIC16::FP,  PIC16::GPR0,  PIC16::GPR1,  PIC16::GPR2,
    PIC16::GPR3,  PIC16::GPR4,  PIC16::GPR5, PIC16::GPR6,
    PIC16::GPR7, PIC16::GPR8, PIC16::GPR9, PIC16::GPR10,
    0
  };
  static const MCPhysReg CalleeSavedRegsIntrFP[] = {
    PIC16::GPR0,  PIC16::GPR1,  PIC16::GPR2,
    PIC16::GPR3,  PIC16::GPR4,  PIC16::GPR5, PIC16::GPR6,
    PIC16::GPR7, PIC16::GPR8, PIC16::GPR9, PIC16::GPR10,
    0
  };

  if (TFI->hasFP(*MF))
    return (F->getCallingConv() == CallingConv::PIC16_INTR ?
            CalleeSavedRegsIntrFP : CalleeSavedRegsFP);
  else
    return (F->getCallingConv() == CallingConv::PIC16_INTR ?
            CalleeSavedRegsIntr : CalleeSavedRegs);

}

BitVector PIC16RegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());
  const PIC16FrameLowering *TFI = getFrameLowering(MF);

  // Mark 4 special registers with subregisters as reserved.
  Reserved.set(PIC16::PC);
  Reserved.set(PIC16::SP);
  Reserved.set(PIC16::SR);
  Reserved.set(PIC16::CG);

  // Mark frame pointer as reserved if needed.
  if (TFI->hasFP(MF)) {
    Reserved.set(PIC16::FP);
  }

  return Reserved;
}

const TargetRegisterClass *
PIC16RegisterInfo::getPointerRegClass(const MachineFunction &MF, unsigned Kind)
                                                                         const {
  return &PIC16::GR8RegClass;
}

void
PIC16RegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                        int SPAdj, unsigned FIOperandNum,
                                        RegScavenger *RS) const {
  assert(SPAdj == 0 && "Unexpected");

  MachineInstr &MI = *II;
  MachineBasicBlock &MBB = *MI.getParent();
  MachineFunction &MF = *MBB.getParent();
  const PIC16FrameLowering *TFI = getFrameLowering(MF);
  DebugLoc dl = MI.getDebugLoc();
  int FrameIndex = MI.getOperand(FIOperandNum).getIndex();

  unsigned BasePtr = (TFI->hasFP(MF) ? PIC16::FP : PIC16::SP);
  int Offset = MF.getFrameInfo()->getObjectOffset(FrameIndex);

  // Skip the saved PC
  Offset += 2;

  if (!TFI->hasFP(MF))
    Offset += MF.getFrameInfo()->getStackSize();
  else
    Offset += 2; // Skip the saved FP

  // Fold imm into offset
  Offset += MI.getOperand(FIOperandNum + 1).getImm();

  if (MI.getOpcode() == PIC16::ADD8ri) {
    // This is actually "load effective address" of the stack slot
    // instruction. We have only two-address instructions, thus we need to
    // expand it into mov + add
    const TargetInstrInfo &TII = *MF.getSubtarget().getInstrInfo();

    MI.setDesc(TII.get(PIC16::MOV8rr));
    MI.getOperand(FIOperandNum).ChangeToRegister(BasePtr, false);

    if (Offset == 0)
      return;

    // We need to materialize the offset via add instruction.
    unsigned DstReg = MI.getOperand(0).getReg();
    if (Offset < 0)
      BuildMI(MBB, std::next(II), dl, TII.get(PIC16::SUB8ri), DstReg)
        .addReg(DstReg).addImm(-Offset);
    else
      BuildMI(MBB, std::next(II), dl, TII.get(PIC16::ADD8ri), DstReg)
        .addReg(DstReg).addImm(Offset);

    return;
  }

  MI.getOperand(FIOperandNum).ChangeToRegister(BasePtr, false);
  MI.getOperand(FIOperandNum + 1).ChangeToImmediate(Offset);
}

unsigned PIC16RegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  const PIC16FrameLowering *TFI = getFrameLowering(MF);
  return TFI->hasFP(MF) ? PIC16::FP : PIC16::SP;
}
