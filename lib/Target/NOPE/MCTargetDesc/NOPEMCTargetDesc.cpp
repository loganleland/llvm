//===-- NOPEMCTargetDesc.cpp - NOPE Target Descriptions ---------------===//
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

#include "NOPEMCTargetDesc.h"
#include "InstPrinter/NOPEInstPrinter.h"
#include "NOPEMCAsmInfo.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define GET_INSTRINFO_MC_DESC
#include "NOPEGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "NOPEGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "NOPEGenRegisterInfo.inc"

static MCInstrInfo *createNOPEMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitNOPEMCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createNOPEMCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitNOPEMCRegisterInfo(X, NOPE::PC);
  return X;
}

static MCSubtargetInfo *
createNOPEMCSubtargetInfo(const Triple &TT, StringRef CPU, StringRef FS) {
  return createNOPEMCSubtargetInfoImpl(TT, CPU, FS);
}

static MCInstPrinter *createNOPEMCInstPrinter(const Triple &T,
                                                unsigned SyntaxVariant,
                                                const MCAsmInfo &MAI,
                                                const MCInstrInfo &MII,
                                                const MCRegisterInfo &MRI) {
  if (SyntaxVariant == 0)
    return new NOPEInstPrinter(MAI, MII, MRI);
  return nullptr;
}

extern "C" void LLVMInitializeNOPETargetMC() {
  // Register the MC asm info.
  RegisterMCAsmInfo<NOPEMCAsmInfo> X(TheNOPETarget);

  // Register the MC instruction info.
  TargetRegistry::RegisterMCInstrInfo(TheNOPETarget, createNOPEMCInstrInfo);

  // Register the MC register info.
  TargetRegistry::RegisterMCRegInfo(TheNOPETarget,
                                    createNOPEMCRegisterInfo);

  // Register the MC subtarget info.
  TargetRegistry::RegisterMCSubtargetInfo(TheNOPETarget,
                                          createNOPEMCSubtargetInfo);

  // Register the MCInstPrinter.
  TargetRegistry::RegisterMCInstPrinter(TheNOPETarget,
                                        createNOPEMCInstPrinter);
}
