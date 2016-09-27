//===-- PIC16.h - Top-level interface for PIC16 representation ----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in
// the LLVM PIC16 back-end.
//
//===----------------------------------------------------------------------===//

#ifndef TARGET_PIC16_H
#define TARGET_PIC16_H

#include "MCTargetDesc/PIC16MCTargetDesc.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class TargetMachine;
class PIC16TargetMachine;
FunctionPass *createTOYISelDag(TOYTargetMachine &TM,
                               CodeGenOpt::Level OptLevel);
} // end namespace llvm;
#endif
