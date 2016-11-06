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
    COND_DECFSZ=0,
    COND_INCFSZ=1,
    COND_IORWF=2,
    COND_XORWF=3,
    COND_BTFSC=4,
    COND_BTFSS=5,
    COND_IORLW=6,
    COND_XORLW=7,
    //just so it builds
    COND_E=8,  // aka COND_Z
    COND_NE=9,  // aka COND_NZ
    COND_HS=10,  // aka COND_C
    COND_LO=11,  // aka COND_NC
    COND_GE=12,
    COND_L=13,
    COND_INVALID=-1
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
