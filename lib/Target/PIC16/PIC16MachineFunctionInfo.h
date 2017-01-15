//=== PIC16MachineFunctionInfo.h - PIC16 machine function info -*- C++ -*-==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares PIC16-specific per-machine-function information.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_PIC16_PIC16MACHINEFUNCTIONINFO_H
#define LLVM_LIB_TARGET_PIC16_PIC16MACHINEFUNCTIONINFO_H

#include "llvm/CodeGen/MachineFunction.h"

namespace llvm {

/// PIC16MachineFunctionInfo - This class is derived from MachineFunction and
/// contains private PIC16 target-specific information for each MachineFunction.
class PIC16MachineFunctionInfo : public MachineFunctionInfo {
  virtual void anchor();

  /// CalleeSavedFrameSize - Size of the callee-saved register portion of the
  /// stack frame in bytes.
  unsigned CalleeSavedFrameSize;

  /// ReturnAddrIndex - FrameIndex for return slot.
  int ReturnAddrIndex;

  /// VarArgsFrameIndex - FrameIndex for start of varargs area.
  int VarArgsFrameIndex;

public:
  PIC16MachineFunctionInfo() : CalleeSavedFrameSize(0) {}

  explicit PIC16MachineFunctionInfo(MachineFunction &MF)
    : CalleeSavedFrameSize(0), ReturnAddrIndex(0) {}

  unsigned getCalleeSavedFrameSize() const { return CalleeSavedFrameSize; }
  void setCalleeSavedFrameSize(unsigned bytes) { CalleeSavedFrameSize = bytes; }

  int getRAIndex() const { return ReturnAddrIndex; }
  void setRAIndex(int Index) { ReturnAddrIndex = Index; }

  int getVarArgsFrameIndex() const { return VarArgsFrameIndex;}
  void setVarArgsFrameIndex(int Index) { VarArgsFrameIndex = Index; }
};

} // End llvm namespace

#endif
