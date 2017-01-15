//==-- PIC16.h - Top-level interface for PIC16 representation --*- C++ -*-==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in
// the LLVM PIC16 backend.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_PIC16_PIC16_H
#define LLVM_LIB_TARGET_PIC16_PIC16_H

#include "MCTargetDesc/PIC16MCTargetDesc.h"
#include "llvm/Target/TargetMachine.h"

namespace PIC16CC {
    // PIC16 specific condition code.
    enum CondCodes {
        COND_E = 0,  // aka COND_Z
        COND_NE = 1,  // aka COND_NZ
        COND_HS = 2,  // aka COND_C
        COND_LO = 3,  // aka COND_NC
        COND_GE = 4,
        COND_L = 5,

        COND_INVALID = 6
    };
}

namespace llvm {
    class PIC16TargetMachine;

    class FunctionPass;

    class formatted_raw_ostream;

    FunctionPass *createPIC16ISelDag(PIC16TargetMachine &TM,
                                      CodeGenOpt::Level OptLevel);

    FunctionPass *createPIC16BranchSelectionPass();

} // end namespace llvm;

#endif
