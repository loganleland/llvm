//===-- NOPETargetInfo.cpp - NOPE Target Implementation ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "NOPE.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;

Target llvm::TheNOPETarget;

extern "C" void LLVMInitializeNOPETargetInfo() { 
  RegisterTarget<Triple::nope> 
    X(TheNOPETarget, "nope", "NOPE [experimental]");
}
