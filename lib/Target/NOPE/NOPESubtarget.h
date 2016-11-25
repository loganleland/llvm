//===-- NOPESubtarget.h - Define Subtarget for the NOPE ----*- C++ -*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the NOPE specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_NOPE_NOPESUBTARGET_H
#define LLVM_LIB_TARGET_NOPE_NOPESUBTARGET_H

#include "NOPEFrameLowering.h"
#include "NOPEISelLowering.h"
#include "NOPEInstrInfo.h"
#include "NOPERegisterInfo.h"
#include "llvm/CodeGen/SelectionDAGTargetInfo.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/Target/TargetSubtargetInfo.h"
#include <string>

#define GET_SUBTARGETINFO_HEADER
#include "NOPEGenSubtargetInfo.inc"

namespace llvm {
class StringRef;

class NOPESubtarget : public NOPEGenSubtargetInfo {
  virtual void anchor();
  bool ExtendedInsts;
  NOPEFrameLowering FrameLowering;
  NOPEInstrInfo InstrInfo;
  NOPETargetLowering TLInfo;
  SelectionDAGTargetInfo TSInfo;

public:
  /// This constructor initializes the data members to match that
  /// of the specified triple.
  ///
  NOPESubtarget(const Triple &TT, const std::string &CPU,
                  const std::string &FS, const TargetMachine &TM);

  NOPESubtarget &initializeSubtargetDependencies(StringRef CPU, StringRef FS);

  /// ParseSubtargetFeatures - Parses features string setting specified
  /// subtarget options.  Definition of function is auto generated by tblgen.
  void ParseSubtargetFeatures(StringRef CPU, StringRef FS);

  const TargetFrameLowering *getFrameLowering() const override {
    return &FrameLowering;
  }
  const NOPEInstrInfo *getInstrInfo() const override { return &InstrInfo; }
  const TargetRegisterInfo *getRegisterInfo() const override {
    return &InstrInfo.getRegisterInfo();
  }
  const NOPETargetLowering *getTargetLowering() const override {
    return &TLInfo;
  }
  const SelectionDAGTargetInfo *getSelectionDAGInfo() const override {
    return &TSInfo;
  }
};
} // End llvm namespace

#endif  // LLVM_TARGET_NOPE_SUBTARGET_H
