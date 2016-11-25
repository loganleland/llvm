//==-- NOPE.h - Top-level interface for NOPE representation --*- C++ -*-==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in
// the LLVM NOPE backend.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_NOPE_NOPE_H
#define LLVM_LIB_TARGET_NOPE_NOPE_H

#include "MCTargetDesc/NOPEMCTargetDesc.h"
#include "llvm/Target/TargetMachine.h"

namespace NOPECC {
    // NOPE specific condition code.
    enum CondCodes {
        COND_E = 0,  // aka COND_Z
        COND_NE = 1,  // aka COND_NZ
        COND_HS = 2,  // aka COND_C
        COND_LO = 3,  // aka COND_NC
        COND_GE = 4,
        COND_L = 5,

        COND_INVALID = -1
    };
}

namespace llvm {
    class NOPETargetMachine;

    class FunctionPass;

    class formatted_raw_ostream;

    FunctionPass *createNOPEISelDag(NOPETargetMachine &TM,
                                      CodeGenOpt::Level OptLevel);

    FunctionPass *createNOPEBranchSelectionPass();

} // end namespace llvm;

#endif
