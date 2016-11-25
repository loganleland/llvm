//===-- NOPEMCAsmInfo.h - NOPE asm properties --------------*- C++ -*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source 
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of the NOPEMCAsmInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_NOPE_MCTARGETDESC_NOPEMCASMINFO_H
#define LLVM_LIB_TARGET_NOPE_MCTARGETDESC_NOPEMCASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {
class Triple;

class NOPEMCAsmInfo : public MCAsmInfoELF {
  void anchor() override;

public:
  explicit NOPEMCAsmInfo(const Triple &TT);
};

} // namespace llvm

#endif
