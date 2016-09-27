//===-- PIC16TargetInfo.cpp - PIC16 Target Implementation -------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "PIC16.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;

Target llvm::ThePIC16Target;
extern "C" void LLVMInitializePIC16TargetInfo() {
  RegisterTarget<Triple::pic16> X(ThePIC16Target, "pic16", "PIC16");
}
