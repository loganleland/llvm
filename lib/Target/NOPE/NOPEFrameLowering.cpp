//===-- NOPEFrameLowering.cpp - NOPE Frame Information ----------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the NOPE implementation of TargetFrameLowering class.
//
//===----------------------------------------------------------------------===//

#include "NOPEFrameLowering.h"
#include "NOPEInstrInfo.h"
#include "NOPEMachineFunctionInfo.h"
#include "NOPESubtarget.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Function.h"
#include "llvm/Target/TargetOptions.h"

using namespace llvm;

bool NOPEFrameLowering::hasFP(const MachineFunction &MF) const {
  const MachineFrameInfo *MFI = MF.getFrameInfo();

  return (MF.getTarget().Options.DisableFramePointerElim(MF) ||
          MF.getFrameInfo()->hasVarSizedObjects() ||
          MFI->isFrameAddressTaken());
}

bool NOPEFrameLowering::hasReservedCallFrame(const MachineFunction &MF) const {
  return !MF.getFrameInfo()->hasVarSizedObjects();
}

void NOPEFrameLowering::emitPrologue(MachineFunction &MF,
                                       MachineBasicBlock &MBB) const {
  assert(&MF.front() == &MBB && "Shrink-wrapping not yet supported");
  MachineFrameInfo *MFI = MF.getFrameInfo();
  NOPEMachineFunctionInfo *NOPEFI = MF.getInfo<NOPEMachineFunctionInfo>();
  const NOPEInstrInfo &TII =
      *static_cast<const NOPEInstrInfo *>(MF.getSubtarget().getInstrInfo());

  MachineBasicBlock::iterator MBBI = MBB.begin();
  DebugLoc DL = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();

  // Get the number of bytes to allocate from the FrameInfo.
  uint64_t StackSize = MFI->getStackSize();

  uint64_t NumBytes = 0;
  if (hasFP(MF)) {
    // Calculate required stack adjustment
    uint64_t FrameSize = StackSize - 2;
    NumBytes = FrameSize - NOPEFI->getCalleeSavedFrameSize();

    // Get the offset of the stack slot for the EBP register... which is
    // guaranteed to be the last slot by processFunctionBeforeFrameFinalized.
    // Update the frame offset adjustment.
    MFI->setOffsetAdjustment(-NumBytes);

    BuildMI(MBB, MBBI, DL, TII.get(NOPE::INCF_F))
      .addReg(NOPE::SP);

    BuildMI(MBB, MBBI, DL, TII.get(NOPE::INCF_F), NOPE::SP);

    BuildMI(MBB, MBBI, DL, TII.get(NOPE::MOVF_W), NOPE::SP);

    BuildMI(MBB, MBBI, DL, TII.get(NOPE::MOVWF))
      .addReg(NOPE::FSR);

   BuildMI(MBB, MBBI, DL, TII.get(NOPE::MOVF_W))
      .addReg(NOPE::FP);

   BuildMI(MBB, MBBI, DL, TII.get(NOPE::MOVWF))
      .addReg(NOPE::INDF);

   BuildMI(MBB, MBBI, DL, TII.get(NOPE::DECF_F), NOPE::SP);
   BuildMI(MBB, MBBI, DL, TII.get(NOPE::DECF_F), NOPE::FSR);

   BuildMI(MBB, MBBI, DL, TII.get(NOPE::MOVF_W), NOPE::SP);

   BuildMI(MBB, MBBI, DL, TII.get(NOPE::MOVWF), NOPE::FP);

    // Mark the FramePtr as live-in in every block except the entry.
    for (MachineFunction::iterator I = std::next(MF.begin()), E = MF.end();
         I != E; ++I)
      I->addLiveIn(NOPE::FP);

  } else
    NumBytes = StackSize - NOPEFI->getCalleeSavedFrameSize();

  // Skip the callee-saved push instructions.
  while (MBBI != MBB.end() && (MBBI->getOpcode() == NOPE::PUSH16r))
    ++MBBI;

  if (MBBI != MBB.end())
    DL = MBBI->getDebugLoc();

  if (NumBytes) { // adjust stack pointer: SP -= numbytes
    // If there is an SUB16ri of SP immediately before this instruction, merge
    // the two.
    //NumBytes -= mergeSPUpdates(MBB, MBBI, true);
    // If there is an ADD16ri or SUB16ri of SP immediately after this
    // instruction, merge the two instructions.
    // mergeSPUpdatesDown(MBB, MBBI, &NumBytes);

    if (NumBytes) {
      MachineInstr *MI =
        BuildMI(MBB, MBBI, DL, TII.get(NOPE::ADDLW), NOPE::SP)
        .addReg(NOPE::SP).addImm(NumBytes-1);
      // The SRW implicit def is dead.
      MI->getOperand(3).setIsDead();
    }
  }
}

void NOPEFrameLowering::emitEpilogue(MachineFunction &MF,
                                       MachineBasicBlock &MBB) const {
  const MachineFrameInfo *MFI = MF.getFrameInfo();
  NOPEMachineFunctionInfo *NOPEFI = MF.getInfo<NOPEMachineFunctionInfo>();
  const NOPEInstrInfo &TII =
      *static_cast<const NOPEInstrInfo *>(MF.getSubtarget().getInstrInfo());

  MachineBasicBlock::iterator MBBI = MBB.getLastNonDebugInstr();
  unsigned RetOpcode = MBBI->getOpcode();
  DebugLoc DL = MBBI->getDebugLoc();

  switch (RetOpcode) {
  case NOPE::RET: break;
  default:
    llvm_unreachable("Can only insert epilog into returning blocks");
  }

  // Get the number of bytes to allocate from the FrameInfo
  uint64_t StackSize = MFI->getStackSize();
  unsigned CSSize = NOPEFI->getCalleeSavedFrameSize();
  uint64_t NumBytes = 0;

  if (hasFP(MF)) {
    // Calculate required stack adjustment
    uint64_t FrameSize = StackSize - 2;
    NumBytes = FrameSize - CSSize;

    // pop FP.
    BuildMI(MBB, MBBI, DL, TII.get(NOPE::INCF_F), NOPE::SP);
  } else
    NumBytes = StackSize - CSSize;

  // Skip the callee-saved pop instructions.
  while (MBBI != MBB.begin()) {
    MachineBasicBlock::iterator PI = std::prev(MBBI);
    unsigned Opc = PI->getOpcode();
    if (Opc != NOPE::POP16r && !PI->isTerminator())
      break;
    --MBBI;
  }

  DL = MBBI->getDebugLoc();

  // If there is an ADD16ri or SUB16ri of SP immediately before this
  // instruction, merge the two instructions.
  //if (NumBytes || MFI->hasVarSizedObjects())
  //  mergeSPUpdatesUp(MBB, MBBI, StackPtr, &NumBytes);

  if (MFI->hasVarSizedObjects()) {
    BuildMI(MBB, MBBI, DL,
            TII.get(NOPE::MOV16rr), NOPE::SP).addReg(NOPE::FP);
    if (CSSize) {
      MachineInstr *MI =
        BuildMI(MBB, MBBI, DL,
                TII.get(NOPE::SUB16ri), NOPE::SP)
        .addReg(NOPE::SP).addImm(CSSize);
      // The SRW implicit def is dead.
      MI->getOperand(3).setIsDead();
    }
  } else {
    // adjust stack pointer back: SP += numbytes
    if (NumBytes) {
      MachineInstr *MI =
        BuildMI(MBB, MBBI, DL, TII.get(NOPE::ADD16ri), NOPE::SP)
        .addReg(NOPE::SP).addImm(NumBytes);
      // The SRW implicit def is dead.
      MI->getOperand(3).setIsDead();
    }
  }
}

// FIXME: Can we eleminate these in favour of generic code?
bool
NOPEFrameLowering::spillCalleeSavedRegisters(MachineBasicBlock &MBB,
                                           MachineBasicBlock::iterator MI,
                                        const std::vector<CalleeSavedInfo> &CSI,
                                        const TargetRegisterInfo *TRI) const {
  if (CSI.empty())
    return false;

  DebugLoc DL;
  if (MI != MBB.end()) DL = MI->getDebugLoc();

  MachineFunction &MF = *MBB.getParent();
  const TargetInstrInfo &TII = *MF.getSubtarget().getInstrInfo();
  NOPEMachineFunctionInfo *MFI = MF.getInfo<NOPEMachineFunctionInfo>();
  MFI->setCalleeSavedFrameSize(CSI.size() * 2);

  for (unsigned i = CSI.size(); i != 0; --i) {
    unsigned Reg = CSI[i-1].getReg();
    // Add the callee-saved register as live-in. It's killed at the spill.
    MBB.addLiveIn(Reg);
    BuildMI(MBB, MI, DL, TII.get(NOPE::PUSH16r))
      .addReg(Reg, RegState::Kill);
  }
  return true;
}

bool
NOPEFrameLowering::restoreCalleeSavedRegisters(MachineBasicBlock &MBB,
                                                 MachineBasicBlock::iterator MI,
                                        const std::vector<CalleeSavedInfo> &CSI,
                                        const TargetRegisterInfo *TRI) const {
  if (CSI.empty())
    return false;

  DebugLoc DL;
  if (MI != MBB.end()) DL = MI->getDebugLoc();

  MachineFunction &MF = *MBB.getParent();
  const TargetInstrInfo &TII = *MF.getSubtarget().getInstrInfo();

  for (unsigned i = 0, e = CSI.size(); i != e; ++i)
    BuildMI(MBB, MI, DL, TII.get(NOPE::POP16r), CSI[i].getReg());

  return true;
}

MachineBasicBlock::iterator NOPEFrameLowering::eliminateCallFramePseudoInstr(
    MachineFunction &MF, MachineBasicBlock &MBB,
    MachineBasicBlock::iterator I) const {
  const NOPEInstrInfo &TII =
      *static_cast<const NOPEInstrInfo *>(MF.getSubtarget().getInstrInfo());
  unsigned StackAlign = getStackAlignment();

  if (!hasReservedCallFrame(MF)) {
    // If the stack pointer can be changed after prologue, turn the
    // adjcallstackup instruction into a 'sub SP, <amt>' and the
    // adjcallstackdown instruction into 'add SP, <amt>'
    // TODO: consider using push / pop instead of sub + store / add
    MachineInstr &Old = *I;
    uint64_t Amount = Old.getOperand(0).getImm();
    if (Amount != 0) {
      // We need to keep the stack aligned properly.  To do this, we round the
      // amount of space needed for the outgoing arguments up to the next
      // alignment boundary.
      Amount = (Amount+StackAlign-1)/StackAlign*StackAlign;

      MachineInstr *New = nullptr;
      if (Old.getOpcode() == TII.getCallFrameSetupOpcode()) {
        New =
            BuildMI(MF, Old.getDebugLoc(), TII.get(NOPE::SUB16ri), NOPE::SP)
                .addReg(NOPE::SP)
                .addImm(Amount);
      } else {
        assert(Old.getOpcode() == TII.getCallFrameDestroyOpcode());
        // factor out the amount the callee already popped.
        uint64_t CalleeAmt = Old.getOperand(1).getImm();
        Amount -= CalleeAmt;
        if (Amount)
          New = BuildMI(MF, Old.getDebugLoc(), TII.get(NOPE::ADD16ri),
                        NOPE::SP)
                    .addReg(NOPE::SP)
                    .addImm(Amount);
      }

      if (New) {
        // The SRW implicit def is dead.
        New->getOperand(3).setIsDead();

        // Replace the pseudo instruction with a new instruction...
        MBB.insert(I, New);
      }
    }
  } else if (I->getOpcode() == TII.getCallFrameDestroyOpcode()) {
    // If we are performing frame pointer elimination and if the callee pops
    // something off the stack pointer, add it back.
    if (uint64_t CalleeAmt = I->getOperand(1).getImm()) {
      MachineInstr &Old = *I;
      MachineInstr *New =
          BuildMI(MF, Old.getDebugLoc(), TII.get(NOPE::SUB16ri), NOPE::SP)
              .addReg(NOPE::SP)
              .addImm(CalleeAmt);
      // The SRW implicit def is dead.
      New->getOperand(3).setIsDead();

      MBB.insert(I, New);
    }
  }

  return MBB.erase(I);
}

void
NOPEFrameLowering::processFunctionBeforeFrameFinalized(MachineFunction &MF,
                                                         RegScavenger *) const {
  // Create a frame entry for the FP register that must be saved.
  if (hasFP(MF)) {
    int FrameIdx = MF.getFrameInfo()->CreateFixedObject(2, -4, true);
    (void)FrameIdx;
    assert(FrameIdx == MF.getFrameInfo()->getObjectIndexBegin() &&
           "Slot for FP register must be last in order to be found!");
  }
}
