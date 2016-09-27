//===-- SparcTargetMachine.h - Define TargetMachine for PIC16 ---*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the PIC16 specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_PIC16_PIC16TARGETMACHINE_H
#define LLVM_LIB_TARGET_PIC16_PIC16TARGETMACHINE_H

#include "PIC16InstrInfo.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {

class Module;

class PIC16TargetMachine : public LLVMTargetMachine {
  const DataLayout DataLayout;       // Calculates type size & alignment
  PIC16InstrInfo InstrInfo;
  PIC16Subtarget Subtarget;
  TargetFrameInfo FrameInfo;

protected:
  virtual const TargetAsmInfo *createTargetAsmInfo() const;

public:
  PIC16TargetMachine(const Module &M, const std::string &FS);

  virtual const PIC16InstrInfo *getInstrInfo() const {return &InstrInfo; }
  virtual const TargetFrameInfo *getFrameInfo() const {return &FrameInfo; }
  virtual const TargetSubtarget *getSubtargetImpl() const{return &Subtarget; }
  virtual const TargetRegisterInfo *getRegisterInfo() const {
    return &InstrInfo.getRegisterInfo();
  }
  virtual const DataLayout *getDataLayout() const { return &DataLayout; }

  // Pass Pipeline Configuration
  virtual bool addInstSelector(PassManagerBase &PM, bool Fast);
  virtual bool addPreEmitPass(PassManagerBase &PM, bool Fast);
};

} // end namespace llvm

#endif
