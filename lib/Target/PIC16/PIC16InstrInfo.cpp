//===-- PIC16InstrInfo.cpp - PIC16 Instruction Information --------------===//
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

#include "PIC16InstrInfo.h"
#include "PIC16.h"
#include "PIC16MachineFunctionInfo.h"
#include "PIC16TargetMachine.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define GET_INSTRINFO_CTOR_DTOR
#include "PIC16GenInstrInfo.inc"

// Pin the vtable to this file.
void PIC16InstrInfo::anchor() {}

PIC16InstrInfo::PIC16InstrInfo(PIC16Subtarget &STI)
  : PIC16GenInstrInfo(PIC16::ADJCALLSTACKDOWN, PIC16::ADJCALLSTACKUP),
    RI() {}

void PIC16InstrInfo::storeRegToStackSlot(MachineBasicBlock &MBB,
                                          MachineBasicBlock::iterator MI,
                                    unsigned SrcReg, bool isKill, int FrameIdx,
                                          const TargetRegisterClass *RC,
                                          const TargetRegisterInfo *TRI) const {
  DebugLoc DL;
  if (MI != MBB.end()) DL = MI->getDebugLoc();
  MachineFunction &MF = *MBB.getParent();
  MachineFrameInfo &MFI = *MF.getFrameInfo();

  MachineMemOperand *MMO = MF.getMachineMemOperand(
      MachinePointerInfo::getFixedStack(MF, FrameIdx),
      MachineMemOperand::MOStore, MFI.getObjectSize(FrameIdx),
      MFI.getObjectAlignment(FrameIdx));

  if (RC == &PIC16::GR8RegClass){
    BuildMI(MBB, MI, DL, get(PIC16::MOVLW))
      .addFrameIndex(FrameIdx).addImm(0);
    BuildMI(MBB, MI, DL, get(PIC16::MOVWF))
      .addReg(SrcReg, getKillRegState(isKill)).addMemOperand(MMO);
  }
  else
    llvm_unreachable("Cannot store this register to stack slot!");
}

void PIC16InstrInfo::loadRegFromStackSlot(MachineBasicBlock &MBB,
                                           MachineBasicBlock::iterator MI,
                                           unsigned DestReg, int FrameIdx,
                                           const TargetRegisterClass *RC,
                                           const TargetRegisterInfo *TRI) const{
  DebugLoc DL;
//  if (MI != MBB.end()) DL = MI->getDebugLoc();
//  MachineFunction &MF = *MBB.getParent();
//  MachineFrameInfo &MFI = *MF.getFrameInfo();

//  MachineMemOperand *MMO = MF.getMachineMemOperand(
//      MachinePointerInfo::getFixedStack(MF, FrameIdx),
//      MachineMemOperand::MOLoad, MFI.getObjectSize(FrameIdx),
//      MFI.getObjectAlignment(FrameIdx));

//  if (RC == &PIC16::GR8RegClass)
//    BuildMI(MBB, MI, DL, get(PIC16::MOV8rm))
//      .addReg(DestReg, getDefRegState(true)).addFrameIndex(FrameIdx)
//      .addImm(0).addMemOperand(MMO);
//  else
//    llvm_unreachable("Cannot store this register to stack slot!");
}

void PIC16InstrInfo::copyPhysReg(MachineBasicBlock &MBB,
                                  MachineBasicBlock::iterator I,
                                  const DebugLoc &DL, unsigned DestReg,
                                  unsigned SrcReg, bool KillSrc) const {
//  unsigned Opc;
//  if (PIC16::GR8RegClass.contains(DestReg, SrcReg))
//    Opc = PIC16::MOV8rr;
//  else
//    llvm_unreachable("Impossible reg-to-reg copy");
//
//  BuildMI(MBB, I, DL, get(Opc), DestReg)
//    .addReg(SrcReg, getKillRegState(KillSrc));
}


/// GetInstSize - Return the number of bytes of code the specified
/// instruction may be.  This returns the maximum number of bytes.
///
unsigned PIC16InstrInfo::GetInstSizeInBytes(const MachineInstr &MI) const {
  const MCInstrDesc &Desc = MI.getDesc();

  switch (Desc.TSFlags & PIC16II::SizeMask) {
  default:
    switch (Desc.getOpcode()) {
    default: llvm_unreachable("Unknown instruction size!");
    case TargetOpcode::CFI_INSTRUCTION:
    case TargetOpcode::EH_LABEL:
    case TargetOpcode::IMPLICIT_DEF:
    case TargetOpcode::KILL:
    case TargetOpcode::DBG_VALUE:
      return 0;
    case TargetOpcode::INLINEASM: {
      const MachineFunction *MF = MI.getParent()->getParent();
      const TargetInstrInfo &TII = *MF->getSubtarget().getInstrInfo();
      return TII.getInlineAsmLength(MI.getOperand(0).getSymbolName(),
                                    *MF->getTarget().getMCAsmInfo());
    }
    }

  case PIC16II::Size2Bytes:
    return 2;
  case PIC16II::Size4Bytes:
    return 4;
  case PIC16II::Size6Bytes:
    return 6;
  }
}
