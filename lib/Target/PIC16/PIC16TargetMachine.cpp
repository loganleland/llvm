//===-- PIC16TargetMachine.cpp - Define TargetMachine for PIC16 ---------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Top-level implementation for the PIC16 target.
//
//===----------------------------------------------------------------------===//

#include "PIC16TargetMachine.h"
#include "PIC16.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;

extern "C" void LLVMInitializePIC16Target() {
  // Register the target.
  RegisterTargetMachine<PIC16TargetMachine> X(ThePIC16Target);
}

static Reloc::Model getEffectiveRelocModel(Optional<Reloc::Model> RM) {
  if (!RM.hasValue())
    return Reloc::Static;
  return *RM;
}

PIC16TargetMachine::PIC16TargetMachine(const Target &T, const Triple &TT,
                                         StringRef CPU, StringRef FS,
                                         const TargetOptions &Options,
                                         Optional<Reloc::Model> RM,
                                         CodeModel::Model CM,
                                         CodeGenOpt::Level OL)
    : LLVMTargetMachine(T, "e-m:e-p:8:8-i8:8:8-a:8-n8", TT, CPU, FS,
                        Options, getEffectiveRelocModel(RM), CM, OL),
      TLOF(make_unique<TargetLoweringObjectFileELF>()),
      Subtarget(TT, CPU, FS, *this) {
  initAsmInfo();
}
      // FIXME: Check DataLayout string.
      // e -> Little endian
      // m:e -> ELF mangling (PIC16 uses ELF)
      // p:16:16 -> Assembler takes care of addr. space. But 13b ptr, 8b align
      // i32:16:32 -> i32 has 16b alignment but prefers 32b
      // a:16 -> Aggregate types aligned to 16b
      // n8 -> Native integers are 8b

PIC16TargetMachine::~PIC16TargetMachine() {}

namespace {
/// PIC16 Code Generator Pass Configuration Options.
class PIC16PassConfig : public TargetPassConfig {
public:
  PIC16PassConfig(PIC16TargetMachine *TM, PassManagerBase &PM)
    : TargetPassConfig(TM, PM) {}

  PIC16TargetMachine &getPIC16TargetMachine() const {
    return getTM<PIC16TargetMachine>();
  }

  bool addInstSelector() override;
};
} // namespace

TargetPassConfig *PIC16TargetMachine::createPassConfig(PassManagerBase &PM) {
  return new PIC16PassConfig(this, PM);
}

bool PIC16PassConfig::addInstSelector() {
  // Install an instruction selector.
  addPass(createPIC16ISelDag(getPIC16TargetMachine(), getOptLevel()));
  return false;
}
