//===-- NOPETargetMachine.cpp - Define TargetMachine for NOPE ---------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Top-level implementation for the NOPE target.
//
//===----------------------------------------------------------------------===//

#include "NOPETargetMachine.h"
#include "NOPE.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;

extern "C" void LLVMInitializeNOPETarget() {
  // Register the target.
  RegisterTargetMachine<NOPETargetMachine> X(TheNOPETarget);
}

static Reloc::Model getEffectiveRelocModel(Optional<Reloc::Model> RM) {
  if (!RM.hasValue())
    return Reloc::Static;
  return *RM;
}

NOPETargetMachine::NOPETargetMachine(const Target &T, const Triple &TT,
                                         StringRef CPU, StringRef FS,
                                         const TargetOptions &Options,
                                         Optional<Reloc::Model> RM,
                                         CodeModel::Model CM,
                                         CodeGenOpt::Level OL)
    : LLVMTargetMachine(T, "e-m:e-p:16:16-i32:16:32-a:16-n8:16", TT, CPU, FS,
                        Options, getEffectiveRelocModel(RM), CM, OL),
      TLOF(make_unique<TargetLoweringObjectFileELF>()),
      // FIXME: Check DataLayout string.
      Subtarget(TT, CPU, FS, *this) {
  initAsmInfo();
}

NOPETargetMachine::~NOPETargetMachine() {}

namespace {
/// NOPE Code Generator Pass Configuration Options.
class NOPEPassConfig : public TargetPassConfig {
public:
  NOPEPassConfig(NOPETargetMachine *TM, PassManagerBase &PM)
    : TargetPassConfig(TM, PM) {}

  NOPETargetMachine &getNOPETargetMachine() const {
    return getTM<NOPETargetMachine>();
  }

  bool addInstSelector() override;
  void addPreEmitPass() override;
};
} // namespace

TargetPassConfig *NOPETargetMachine::createPassConfig(PassManagerBase &PM) {
  return new NOPEPassConfig(this, PM);
}

bool NOPEPassConfig::addInstSelector() {
  // Install an instruction selector.
  addPass(createNOPEISelDag(getNOPETargetMachine(), getOptLevel()));
  return false;
}

void NOPEPassConfig::addPreEmitPass() {
  // Must run branch selection immediately preceding the asm printer.
  addPass(createNOPEBranchSelectionPass(), false);
}
