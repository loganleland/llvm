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

  PointerSize = 1;
  CalleeSaveStackSlotSize = 1;
  MaxInstLength = 1;
  SeparatorString = "\n";
  CommentString = ";";
  GlobalDirective = "    ; .globl  ";
  HasFunctionAlignment = false;
  HasDotTypeDotSizeDirective = false;
  HasSingleParameterDotFile = false;
  HasIdentDirective = false;
  HiddenVisibilityAttr = MCSA_Invalid;
  HiddenDeclarationVisibilityAttr = MCSA_Invalid;
  ProtectedVisibilityAttr = MCSA_Invalid;
  SupportsDebugInformation = false;
  UseIntegratedAssembler = false;
  AlignmentIsInBytes = false;
  UsesELFSectionDirectiveForBSS = true;

  // From MCAsmInfoELF.h
  UsesNonexecutableStackSection = false;
}
