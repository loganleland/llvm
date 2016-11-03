//===-- PIC16MCAsmInfo.cpp - PIC16 asm properties -----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declarations of the PIC16MCAsmInfo properties.
//
//===----------------------------------------------------------------------===//

#include "PIC16MCAsmInfo.h"
using namespace llvm;

void PIC16MCAsmInfo::anchor() { }

PIC16MCAsmInfo::PIC16MCAsmInfo(const Triple &TT) {
  PointerSize = CalleeSaveStackSlotSize = 2;

  CommentString = ";";

  AlignmentIsInBytes = false;
  UsesELFSectionDirectiveForBSS = true;
}
