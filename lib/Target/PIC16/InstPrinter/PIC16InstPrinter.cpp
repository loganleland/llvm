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

#include "PIC16InstPrinter.h"
#include "PIC16.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FormattedStream.h"
using namespace llvm;

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
  if (Op.isImm())
    O << Op.getImm();
  else {
    assert(Op.isExpr() && "unknown pcrel immediate operand");
    Op.getExpr()->print(O, &MAI);
  }
}

void PIC16InstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                     raw_ostream &O, const char *Modifier) {
  assert((Modifier == nullptr || Modifier[0] == 0) && "No modifiers supported");
  const MCOperand &Op = MI->getOperand(OpNo);
  if (Op.isReg()) {
    O << getRegisterName(Op.getReg());
  } else if (Op.isImm()) {
    O << Op.getImm();
  } else {
    assert(Op.isExpr() && "unknown operand kind in printOperand");
    Op.getExpr()->print(O, &MAI);
  }
}

void PIC16InstPrinter::printSrcMemOperand(const MCInst *MI, unsigned OpNo,
                                           raw_ostream &O,
                                           const char *Modifier) {
  const MCOperand &Base = MI->getOperand(OpNo);
  const MCOperand &Disp = MI->getOperand(OpNo+1);

  if (Disp.isExpr())
    Disp.getExpr()->print(O, &MAI);
  else {
    assert(Disp.isImm() && "Expected immediate in displacement field");
    O << Disp.getImm();
  }

  // Print register base field
  if (Base.getReg())
    O << getRegisterName(Base.getReg());
}

void PIC16InstPrinter::printCCOperand(const MCInst *MI, unsigned OpNo,
                                       raw_ostream &O) {
  unsigned CC = MI->getOperand(OpNo).getImm();

  switch (CC) {
  default:
   llvm_unreachable("Unsupported CC code");
  case PIC16CC::COND_DECFSZ:
   O << "decfsz";
   break;
  case PIC16CC::COND_INCFSZ:
   O << "incfsz";
   break;
  case PIC16CC::COND_IORWF:
   O << "iorwf";
   break;
  case PIC16CC::COND_XORWF:
   O << "xorwf";
   break;
  case PIC16CC::COND_BTFSC:
   O << "btfsc";
   break;
  case PIC16CC::COND_BTFSS:
   O << "btfss";
   break;
  case PIC16CC::COND_IORLW:
   O << "iorlw";
   break;
  case PIC16CC::COND_XORLW:
   O << "xorlw";
   break;
  }
}
