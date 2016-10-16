//===-- PIC16Subtarget.cpp - PIC16 Subtarget Information ----------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the PIC16 specific subclass of TargetSubtargetInfo.
// Currently there is only one subtarget, (PIC16F877). This will allow
// more features/other chips in the same family to be added later more
// easily.
//
//===----------------------------------------------------------------------===//

#include "PIC16Subtarget.h"
#include "PIC16.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define DEBUG_TYPE "pic16-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "PIC16GenSubtargetInfo.inc"

void PIC16Subtarget::anchor() { }

PIC16Subtarget &
PIC16Subtarget::initializeSubtargetDependencies(StringRef CPU, StringRef FS) {
  ParseSubtargetFeatures("generic", FS);
  return *this;
}

PIC16Subtarget::PIC16Subtarget(const Triple &TT, const std::string &CPU,
                                 const std::string &FS, const TargetMachine &TM)
    : PIC16GenSubtargetInfo(TT, CPU, FS), FrameLowering(),
      InstrInfo(initializeSubtargetDependencies(CPU, FS)), TLInfo(TM, *this) {}
