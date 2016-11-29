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

  // Note that the MPASM assembler does not accept any `.section` headings
  // We took pains to get around or delete all of the headings
  // TODO: Implement `banksel` `pagesel` and such.
  // ALSO TODO: For readability is `equ` e.g. `NAME equ ADDR` possible?

  PointerSize = 1;
  CalleeSaveStackSlotSize = 1; // This will most likely change
                               // as functions are implemented.

  MaxInstLength = 1;
  SeparatorString = "\n";      // All instructions on separate lines
  CommentString = ";";
  GlobalDirective = "    ; .globl  ";

  HasFunctionAlignment = false;
  HasDotTypeDotSizeDirective = false;
  HasSingleParameterDotFile = false;
  HasIdentDirective = false;

  HiddenVisibilityAttr = MCSA_Invalid;
  HiddenDeclarationVisibilityAttr = MCSA_Invalid;
  ProtectedVisibilityAttr = MCSA_Invalid;
  
  SupportsDebugInformation = false;      // Debugging not yet implemented
                                         // Implemented in assembler itself?

  UseIntegratedAssembler = false;        // Using external assembler

  AlignmentIsInBytes = false;            // MSP430 old code. TODO: Prune
  UsesELFSectionDirectiveForBSS = true;

  // From MCAsmInfoELF.h
  UsesNonexecutableStackSection = false;
}
