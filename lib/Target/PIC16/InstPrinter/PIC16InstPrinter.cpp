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
  int alpha;
  if(Op.isImm()){
    if(Op.getImm() < 0) {
      alpha=Op.getImm()-Op.getImm()-Op.getImm();
      OPbase16 << hex << alpha;
      O << '-' << "0x" << OPbase16.str(); 
    }
    else if(Op.getImm() > 0) {
      OPbase16 << hex << Op.getImm();
      O << "0x" << OPbase16.str(); 
    }
  }
  else {
    assert(Op.isExpr() && "unknown pcrel immediate operand");
    Op.getExpr()->print(O, &MAI);
  }
  OPbase16.str("");
}

void PIC16InstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                     raw_ostream &O, const char *Modifier) {
  assert((Modifier == nullptr || Modifier[0] == 0) && "No modifiers supported");

  const MCOperand &Op = MI->getOperand(OpNo);
  int alpha;
  stringstream OPbase16;

  if (Op.isReg()) {
    O << getRegisterName(Op.getReg());
  }
  else if (Op.isImm() and Op.getImm()<0) {
    alpha=Op.getImm()-Op.getImm()-Op.getImm();
    OPbase16 << hex << alpha;
    O << '-' << "0x" << OPbase16.str(); 
  }
  else if (Op.isImm() and Op.getImm()>=0) {
    OPbase16 << hex << Op.getImm();
    O << "0x" << OPbase16.str(); 
  }   

  else {
    assert(Op.isExpr() && "unknown operand kind in printOperand");
    Op.getExpr()->print(O, &MAI);
  }
  OPbase16.str("");
}

void PIC16InstPrinter::printSrcMemOperand(const MCInst *MI, unsigned OpNo,
                                           raw_ostream &O,
                                           const char *Modifier) {
  const MCOperand &Disp = MI->getOperand(OpNo+1);
  uint64_t alpha = Disp.getImm();
  stringstream OPbase16;
  // Print displacement first
  if (Disp.isExpr())
    Disp.getExpr()->print(O, &MAI);
  else {
    assert(Disp.isImm() && "Expected immediate in displacement field");
    alpha=alpha-alpha-alpha;
    OPbase16 << hex << alpha;
    O << "0x" << OPbase16.str(); 
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
