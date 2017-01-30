//===-- PIC16InstPrinter.cpp - Convert PIC16 MCInst to assembly syntax --===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This class prints an PIC16 MCInst to a .s file.
//
//===----------------------------------------------------------------------===//
#include <iostream>
#include <sstream>
#include "PIC16InstPrinter.h"
#include "PIC16.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FormattedStream.h"
using namespace llvm;
using namespace std;

#define DEBUG_TYPE "asm-printer"


// Include the auto-generated portion of the assembly writer.
#include "PIC16GenAsmWriter.inc"

void PIC16InstPrinter::printInst(const MCInst *MI, raw_ostream &O,
                                  StringRef Annot, const MCSubtargetInfo &STI) {
  printInstruction(MI, O);
  printAnnotation(O, Annot);
}

void PIC16InstPrinter::printPCRelImmOperand(const MCInst *MI, unsigned OpNo,
                                             raw_ostream &O) {
  const MCOperand &Op = MI->getOperand(OpNo);
  stringstream OPbase16;
  if (Op.isImm()) {
    OPbase16 << hex << Op.getImm();
    O << OPbase16.str(); 
    OPbase16.str("");
  }
  else {
    assert(Op.isExpr() && "unknown pcrel immediate operand");
    Op.getExpr()->print(O, &MAI);
  }
}

void PIC16InstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                     raw_ostream &O, const char *Modifier) {
  assert((Modifier == nullptr || Modifier[0] == 0) && "No modifiers supported");
  const MCOperand &Op = MI->getOperand(OpNo);
  stringstream OPbase16;
  if (Op.isReg()) {
    O << getRegisterName(Op.getReg());
  } else if (Op.isImm()) {
      //Imm is in base 10 but our assembler prefers base16
      //d'' doesn't work due to calculating the stack/frame
      // pointer offset
      OPbase16 << hex << Op.getImm();
      O << OPbase16.str(); 
      OPbase16.str("");
  } else {
      assert(Op.isExpr() && "unknown operand kind in printOperand");
      Op.getExpr()->print(O, &MAI);
  }
}

void PIC16InstPrinter::printSrcMemOperand(const MCInst *MI, unsigned OpNo,
                                           raw_ostream &O,
                                           const char *Modifier) {
  const MCOperand &Disp = MI->getOperand(OpNo+1);
  uint64_t alpha = Disp.getImm()-Disp.getImm()-Disp.getImm();
  stringstream OPbase16;
  // Print displacement first

  if (Disp.isExpr())
    Disp.getExpr()->print(O, &MAI);
  else {
    assert(Disp.isImm() && "Expected immediate in displacement field");
    OPbase16 << hex << alpha;
    O << OPbase16.str(); 
    OPbase16.str("");
  }
}

void PIC16InstPrinter::printCCOperand(const MCInst *MI, unsigned OpNo,
                                       raw_ostream &O) {
  unsigned CC = MI->getOperand(OpNo).getImm();

  switch (CC) {
  default:
   llvm_unreachable("Unsupported CC code");
  case PIC16CC::COND_E:
   O << "eq";
   break;
  case PIC16CC::COND_NE:
   O << "ne";
   break;
  case PIC16CC::COND_HS:
   O << "hs";
   break;
  case PIC16CC::COND_LO:
   O << "lo";
   break;
  case PIC16CC::COND_GE:
   O << "ge";
   break;
  case PIC16CC::COND_L:
   O << 'l';
   break;
  }
}
