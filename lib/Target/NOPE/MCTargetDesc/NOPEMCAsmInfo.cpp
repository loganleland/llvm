//===-- NOPEMCAsmInfo.cpp - NOPE asm properties -----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declarations of the NOPEMCAsmInfo properties.
//
//===----------------------------------------------------------------------===//

#include "NOPEMCAsmInfo.h"
using namespace llvm;

void NOPEMCAsmInfo::anchor() { }

NOPEMCAsmInfo::NOPEMCAsmInfo(const Triple &TT) {
  PointerSize = CalleeSaveStackSlotSize = 2;

  CommentString = ";";

  AlignmentIsInBytes = false;
  UsesELFSectionDirectiveForBSS = true;
}
