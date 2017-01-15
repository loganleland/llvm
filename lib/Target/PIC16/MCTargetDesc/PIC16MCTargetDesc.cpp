//===-- PIC16MCTargetDesc.cpp - PIC16 Target Descriptions ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides PIC16 specific target descriptions.
//
//===----------------------------------------------------------------------===//

#include "PIC16MCTargetDesc.h"
#include "InstPrinter/PIC16InstPrinter.h"
#include "PIC16MCAsmInfo.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define GET_INSTRINFO_MC_DESC
#include "PIC16GenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "PIC16GenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "PIC16GenRegisterInfo.inc"

static MCInstrInfo *createPIC16MCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitPIC16MCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createPIC16MCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitPIC16MCRegisterInfo(X, PIC16::PC);
  return X;
}

static MCSubtargetInfo *
createPIC16MCSubtargetInfo(const Triple &TT, StringRef CPU, StringRef FS) {
  return createPIC16MCSubtargetInfoImpl(TT, CPU, FS);
}

static MCInstPrinter *createPIC16MCInstPrinter(const Triple &T,
                                                unsigned SyntaxVariant,
                                                const MCAsmInfo &MAI,
                                                const MCInstrInfo &MII,
                                                const MCRegisterInfo &MRI) {
  if (SyntaxVariant == 0)
    return new PIC16InstPrinter(MAI, MII, MRI);
  return nullptr;
}

extern "C" void LLVMInitializePIC16TargetMC() {
  // Register the MC asm info.
  RegisterMCAsmInfo<PIC16MCAsmInfo> X(ThePIC16Target);

  // Register the MC instruction info.
  TargetRegistry::RegisterMCInstrInfo(ThePIC16Target, createPIC16MCInstrInfo);

  // Register the MC register info.
  TargetRegistry::RegisterMCRegInfo(ThePIC16Target,
                                    createPIC16MCRegisterInfo);

  // Register the MC subtarget info.
  TargetRegistry::RegisterMCSubtargetInfo(ThePIC16Target,
                                          createPIC16MCSubtargetInfo);

  // Register the MCInstPrinter.
  TargetRegistry::RegisterMCInstPrinter(ThePIC16Target,
                                        createPIC16MCInstPrinter);
}
