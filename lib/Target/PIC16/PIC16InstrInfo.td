//===-- PIC16InstrInfo.td - PIC16 Instruction defs -------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source 
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file describes the PIC16 instructions in TableGen format.
//
//===----------------------------------------------------------------------===//

include "PIC16InstrFormats.td"

//===----------------------------------------------------------------------===//
// Type Constraints.
//===----------------------------------------------------------------------===//
class SDTCisI8<int OpNum> : SDTCisVT<OpNum, i8>;

//===----------------------------------------------------------------------===//
// Type Profiles.
//===----------------------------------------------------------------------===//
def SDT_PIC16Call         : SDTypeProfile<0, -1, [SDTCisVT<0, iPTR>]>;
def SDT_PIC16CallSeqStart : SDCallSeqStart<[SDTCisVT<0, i8>]>;
def SDT_PIC16CallSeqEnd   : SDCallSeqEnd<[SDTCisVT<0, i8>, SDTCisVT<1, i8>]>;
def SDT_PIC16Wrapper      : SDTypeProfile<1, 1, [SDTCisSameAs<0, 1>,
                                                  SDTCisPtrTy<0>]>;
def SDT_PIC16Cmp          : SDTypeProfile<0, 2, [SDTCisSameAs<0, 1>]>;
def SDT_PIC16BrCC         : SDTypeProfile<0, 2, [SDTCisVT<0, OtherVT>,
                                                  SDTCisVT<1, i8>]>;
def SDT_PIC16SelectCC     : SDTypeProfile<1, 3, [SDTCisSameAs<0, 1>,
                                                  SDTCisSameAs<1, 2>, 
                                                  SDTCisVT<3, i8>]>;
def SDT_PIC16Shift        : SDTypeProfile<1, 2, [SDTCisSameAs<0, 1>,
                                                  SDTCisI8<2>]>;

//===----------------------------------------------------------------------===//
// PIC16 Specific Node Definitions.
//===----------------------------------------------------------------------===//
def PIC16retflag  : SDNode<"PIC16ISD::RET_FLAG", SDTNone,
                       [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;
def PIC16retiflag : SDNode<"PIC16ISD::RETI_FLAG", SDTNone,
                       [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

def PIC16rra     : SDNode<"PIC16ISD::RRA", SDTIntUnaryOp, []>;
def PIC16rla     : SDNode<"PIC16ISD::RLA", SDTIntUnaryOp, []>;
def PIC16rrc     : SDNode<"PIC16ISD::RRC", SDTIntUnaryOp, []>;

def PIC16call    : SDNode<"PIC16ISD::CALL", SDT_PIC16Call,
                     [SDNPHasChain, SDNPOutGlue, SDNPOptInGlue, SDNPVariadic]>;
def PIC16callseq_start :
                 SDNode<"ISD::CALLSEQ_START", SDT_PIC16CallSeqStart,
                        [SDNPHasChain, SDNPOutGlue]>;
def PIC16callseq_end :
                 SDNode<"ISD::CALLSEQ_END",   SDT_PIC16CallSeqEnd,
                        [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue]>;
def PIC16Wrapper : SDNode<"PIC16ISD::Wrapper", SDT_PIC16Wrapper>;
def PIC16cmp     : SDNode<"PIC16ISD::CMP", SDT_PIC16Cmp, [SDNPOutGlue]>;
def PIC16brcc    : SDNode<"PIC16ISD::BR_CC", SDT_PIC16BrCC,
                            [SDNPHasChain, SDNPInGlue]>;
def PIC16selectcc: SDNode<"PIC16ISD::SELECT_CC", SDT_PIC16SelectCC,
                            [SDNPInGlue]>;
def PIC16shl     : SDNode<"PIC16ISD::SHL", SDT_PIC16Shift, []>;
def PIC16sra     : SDNode<"PIC16ISD::SRA", SDT_PIC16Shift, []>;
def PIC16srl     : SDNode<"PIC16ISD::SRL", SDT_PIC16Shift, []>;

//===----------------------------------------------------------------------===//
// PIC16 Operand Definitions.
//===----------------------------------------------------------------------===//

// Address operands
def memsrc : Operand<i8> {
  let PrintMethod = "printSrcMemOperand";
  let MIOperandInfo = (ops GR8, i8imm);
}

def memdst : Operand<i8> {
  let PrintMethod = "printSrcMemOperand";
  let MIOperandInfo = (ops GR8, i8imm);
}

// Short jump targets have OtherVT type and are printed as pcrel imm values.
def jmptarget : Operand<OtherVT> {
  let PrintMethod = "printPCRelImmOperand";
}

// Operand for printing out a condition code.
def cc : Operand<i8> {
  let PrintMethod = "printCCOperand";
}

//===----------------------------------------------------------------------===//
// PIC16 Complex Pattern Definitions.
//===----------------------------------------------------------------------===//

def addr : ComplexPattern<iPTR, 2, "SelectAddr", [], []>;

//===----------------------------------------------------------------------===//
// Pattern Fragments
def and_su : PatFrag<(ops node:$lhs, node:$rhs), (and node:$lhs, node:$rhs), [{
  return N->hasOneUse();
}]>;
//===----------------------------------------------------------------------===//
// Instruction list..

// ADJCALLSTACKDOWN/UP implicitly use/def SP because they may be expanded into
// a stack adjustment and the codegen must know that they may modify the stack
// pointer before prolog-epilog rewriting occurs.
// Pessimistically assume ADJCALLSTACKDOWN / ADJCALLSTACKUP will become
// sub / add which can clobber SR.
let Defs = [SP, SR], Uses = [SP] in {
def ADJCALLSTACKDOWN : Pseudo<(outs), (ins i8imm:$amt),
                              "#ADJCALLSTACKDOWN",
                              [(PIC16callseq_start timm:$amt)]>;
def ADJCALLSTACKUP   : Pseudo<(outs), (ins i8imm:$amt1, i8imm:$amt2),
                              "#ADJCALLSTACKUP",
                              [(PIC16callseq_end timm:$amt1, timm:$amt2)]>;
}

let usesCustomInserter = 1 in {
  def Select8  : Pseudo<(outs GR8:$dst), (ins GR8:$src, GR8:$src2, i8imm:$cc),
                        "# Select8 PSEUDO",
                        [(set GR8:$dst,
                          (PIC16selectcc GR8:$src, GR8:$src2, imm:$cc))]>;
  let Defs = [SR] in {
  def Shl8     : Pseudo<(outs GR8:$dst), (ins GR8:$src, GR8:$cnt),
                        "# Shl8 PSEUDO",
                        [(set GR8:$dst, (PIC16shl GR8:$src, GR8:$cnt))]>;
  def Sra8     : Pseudo<(outs GR8:$dst), (ins GR8:$src, GR8:$cnt),
                        "# Sra8 PSEUDO",
                        [(set GR8:$dst, (PIC16sra GR8:$src, GR8:$cnt))]>;
  def Srl8     : Pseudo<(outs GR8:$dst), (ins GR8:$src, GR8:$cnt),
                        "# Srl8 PSEUDO",
                        [(set GR8:$dst, (PIC16srl GR8:$src, GR8:$cnt))]>;

  }
}

let hasSideEffects = 0 in
def NOP : Pseudo<(outs), (ins), "nop", []>;

//===----------------------------------------------------------------------===//
//  Control Flow Instructions...
//

// FIXME: Provide proper encoding!
let isReturn = 1, isTerminator = 1, isBarrier = 1 in {
  def RET  : IForm16<0x0, DstReg, SrcPostInc, Size2Bytes,
                     (outs), (ins), "return",  [(PIC16retflag)]>;
  def RETI : II16r<0x0, (outs), (ins), "reti", [(PIC16retiflag)]>;
}

let isBranch = 1, isTerminator = 1 in {

// FIXME: expand opcode & cond field for branches!

// Direct branch
let isBarrier = 1 in {
  // Short branch
  def JMP : CJForm<0, 0, (outs), (ins jmptarget:$dst),
                   "jmp\t$dst",
                   [(br bb:$dst)]>;
  let isIndirectBranch = 1 in {
    // Long branches
    def Bi  : I8ri<0, (outs), (ins i8imm:$brdst),
                    "br\t$brdst",
                    [(brind tblockaddress:$brdst)]>;
    def Br  : I8rr<0, (outs), (ins GR8:$brdst),
                    "br\t$brdst",
                    [(brind GR8:$brdst)]>;
    def Bm  : I8rm<0, (outs), (ins memsrc:$brdst),
                    "br\t$brdst",
                    [(brind (load addr:$brdst))]>;
  }
}

// Conditional branches
let Uses = [SR] in
  def JCC : CJForm<0, 0,
                   (outs), (ins jmptarget:$dst, cc:$cc),
                   "j$cc\t$dst",
                   [(PIC16brcc bb:$dst, imm:$cc)]>;
} // isBranch, isTerminator

//===----------------------------------------------------------------------===//
//  Call Instructions...
//
let isCall = 1 in
  // All calls clobber the non-callee saved registers. SPW is marked as
  // a use to prevent stack-pointer assignments that appear immediately
  // before calls from potentially appearing dead. Uses for argument
  // registers are added manually.
  let Defs = [GPR1, GPR2, GPR3, GPR4, SR],
      Uses = [SP] in {
    def CALLi     : II16i<0x0,
                          (outs), (ins i8imm:$dst),
                          "call\t$dst", [(PIC16call imm:$dst)]>;
    def CALLr     : II16r<0x0,
                          (outs), (ins GR8:$dst),
                          "call\t$dst", [(PIC16call GR8:$dst)]>;
    def CALLm     : II16m<0x0,
                          (outs), (ins memsrc:$dst),
                          "call\t${dst:mem}", [(PIC16call (load addr:$dst))]>;
  }


//===----------------------------------------------------------------------===//
//  Miscellaneous Instructions...
//
let Defs = [SP], Uses = [SP], hasSideEffects=0 in {
let mayLoad = 1 in
def POP8r   : IForm16<0x0, DstReg, SrcPostInc, Size2Bytes,
                       (outs GR8:$reg), (ins), "pop.w\t$reg", []>;

let mayStore = 1 in
def PUSH8r  : II16r<0x0,
                     (outs), (ins GR8:$reg), "push.w\t$reg, W",[]>;
}



let hasSideEffects = 0 in {

def DECF_F  : I8rr<0x0,
                   (outs), (ins GR8:$src),
                   "decf\t{$src, F}",
                   []>;

def INCF_F  : I8rr<0x0,
                   (outs), (ins GR8:$src),
                   "incf\t{$src, F}",
                   []>;

def INCF_W  : I8rr<0x0,
                   (outs), (ins GR8:$src),
                   "incf\t{$src, W}",
                   []>;
}//===----------------------------------------------------------------------===//
// Move Instructions

// FIXME: Provide proper encoding!
let hasSideEffects = 0 in {

def MOVF_W  : I8rr<0x0,
                   (outs), (ins GR8:$src),
                   "movf\t{$src, W}",
                   []>;

def MOVF_F  : I8rr<0x0,
                   (outs), (ins GR8:$src),
                   "movf\t{$src, F}",
                   []>;

def MOVLW  : I8rr<0x0,
                   (outs), (ins GR8:$src),
                   "movlw $src",
                   []>;


def MOVWF  : I8rr<0x0,
                   (outs), (ins GR8:$src),
                   "movwf\t{$src}",
                   []>;

def MOV8rr  : I8rr<0x0,
                   (outs GR8:$dst), (ins GR8:$src),
                   "MOV8rr",
                   []>;
}

let isReMaterializable = 1, isAsCheapAsAMove = 1 in {
def MOV8ri  : I8ri<0x0,
                   (outs GR8:$dst), (ins i8imm:$src),
                   "movlw $src\n\tmovwf $dst",
                   [(set GR8:$dst, imm:$src)]>;
}

let canFoldAsLoad = 1, isReMaterializable = 1 in {
def MOV8rm  : I8rm<0x0,
                   (outs GR8:$dst), (ins memsrc:$src),
                   "movf FP,W\n\taddlw $src\n\tmovwf FSR\n\tmovf INDF,W\n\tmovwf $dst", 
                   [(set GR8:$dst, (load addr:$src))]>;
}

let mayLoad = 1, hasExtraDefRegAllocReq = 1, Constraints = "$base = $base_wb" in {
def MOV8rm_POST  : IForm8<0x0, DstReg, SrcPostInc, Size2Bytes,
                         (outs GR8:$dst, GR8:$base_wb), (ins GR8:$base),
                         "MOV8rm_POST\t{@$base+, $dst}", []>;
}

def LOCALVAR: I8mi<0x0,
                   (outs), (ins memdst:$dst, i8imm:$src),
                   "movf FP,W\n\taddlw $dst\n\tmovwf FSR\n\tmovlw $src\n\tmovwf INDF",
                   [(store (i8 imm:$src), addr:$dst)]>;

def MOV8mr  : I8mr<0x0,
                   (outs), (ins memdst:$dst, GR8:$src),
                   "movf FP,W\n\taddlw $dst\n\tmovwf FSR\n\tmovf $src, W\n\tmovwf INDF",
                   [(store GR8:$src, addr:$dst)]>;

def MOV8mm  : I8mm<0x0,
                   (outs), (ins memdst:$dst, memsrc:$src),
                   "movf FP,W\n\taddlw $src\n\tmovwf FSR\n\tmovf INDF, W\n\tmovwf GPR10\n\tmovf FP,W\n\taddlw $dst\n\tmovwf FSR\n\tmovf GPR10,W\n\tmovwf INDF",
                   [(store (i8 (load addr:$src)), addr:$dst)]>;

//===----------------------------------------------------------------------===//
// Arithmetic Instructions

def ADDLW  : I8rr<0x0,
                   (outs), (ins GR8:$src),
                   "addlw\t{$src}",
                   []>;

def SUBLW  : I8rr<0x0,
                   (outs), (ins GR8:$src),
                   "sublw\t{$src}",
                   []>;


let Constraints = "$src = $dst" in {
let Defs = [SR] in {
let isCommutable = 1 in { // X = ADD Y, Z  == X = ADD Z, Y


def ADDrr  : I8rr<0x0,
                   (outs GR8:$dst), (ins GR8:$src, GR8:$src2),
                   "movf $src,W\n\taddwf $src2,W\n\tmovwf $dst",
                   [(set GR8:$dst, (add GR8:$src, GR8:$src2)),
                    (implicit SR)]>;
}

def ADDrm  : I8rm<0x0,
                   (outs GR8:$dst), (ins GR8:$src, memsrc:$src2),
                   "movf FP,W\n\taddlw $src2\n\tmovwf FSR\n\tmovf INDF,W\n\taddwf $src,F",
                   [(set GR8:$dst, (add GR8:$src, (load addr:$src2))),
                    (implicit SR)]>;

let mayLoad = 1, hasExtraDefRegAllocReq = 1, 
Constraints = "$base = $base_wb, $src = $dst" in {
def ADDrm_POST : IForm8<0x0, DstReg, SrcPostInc, Size2Bytes,
                         (outs GR8:$dst, GR8:$base_wb),
                         (ins GR8:$src, GR8:$base),
                         "add8rm_POST.b\t{@$base+, $dst}", []>;
}


def ADDri  : I8ri<0x0,
                   (outs GR8:$dst), (ins GR8:$src, i8imm:$src2),
                   "movf $src,W\n\taddlw $src2\n\tmovwf $dst",
                   [(set GR8:$dst, (add GR8:$src, imm:$src2)),
                    (implicit SR)]>;

let Constraints = "" in {
def ADDmr  : I8mr<0x0,
                   (outs), (ins memdst:$dst, GR8:$src),
                   "movf FP,W\n\taddlw $dst\n\tmovwf FSR\n\tmovf INDF,W\n\taddwf $src,W\n\tmovwf INDF",
                   [(store (add (load addr:$dst), GR8:$src), addr:$dst),
                    (implicit SR)]>;

def ADDmi  : I8mi<0x0,
                   (outs), (ins memdst:$dst, i8imm:$src),
                   "movf FP,W\n\taddlw $dst\n\tmovwf FSR\n\tmovf INDF,W\n\taddlw $src\n\tmovwf INDF",
                   [(store (add (load addr:$dst), (i8 imm:$src)), addr:$dst),
                    (implicit SR)]>;

def ADDmm  : I8mm<0x0,
                   (outs), (ins memdst:$dst, memsrc:$src),
                   "movf FP, W\n\taddlw $src\n\tmovwf FSR\n\tmovf INDF,W\n\tmovwf GPR10\n\tmovf FP,W\n\taddlw $dst\n\tmovwf FSR\n\tmovf INDF,W\n\taddwf GPR10,W\n\tmovwf FSR",
                   [(store (add (load addr:$dst), 
                                (i8 (load addr:$src))), addr:$dst),
                    (implicit SR)]>;
}

let Uses = [SR] in {

let isCommutable = 1 in { // X = ADDC Y, Z  == X = ADDC Z, Y
def ADC8rr  : I8rr<0x0,
                   (outs GR8:$dst), (ins GR8:$src, GR8:$src2),
                   "ADDC8rr",
                   [(set GR8:$dst, (adde GR8:$src, GR8:$src2)),
                    (implicit SR)]>;
} // isCommutable

def ADC8ri  : I8ri<0x0,
                   (outs GR8:$dst), (ins GR8:$src, i8imm:$src2),
                   "addc.b\t{$src2, $dst}",
                   [(set GR8:$dst, (adde GR8:$src, imm:$src2)),
                    (implicit SR)]>;

def ADC8rm  : I8rm<0x0,
                   (outs GR8:$dst), (ins GR8:$src, memsrc:$src2),
                   "addc.b\t{$src2, $dst}",
                   [(set GR8:$dst, (adde GR8:$src, (load addr:$src2))),
                    (implicit SR)]>;

let Constraints = "" in {
def ADC8mr  : I8mr<0x0,
                   (outs), (ins memdst:$dst, GR8:$src),
                   "addc.b\t{$src, $dst}",
                   [(store (adde (load addr:$dst), GR8:$src), addr:$dst),
                    (implicit SR)]>;

def ADC8mi  : I8mi<0x0,
                   (outs), (ins memdst:$dst, i8imm:$src),
                   "addc.b\t{$src, $dst}",
                   [(store (adde (load addr:$dst), (i8 imm:$src)), addr:$dst),
                    (implicit SR)]>;

def ADC8mm  : I8mm<0x0,
                   (outs), (ins memdst:$dst, memsrc:$src),
                   "addc.b\t{$src, $dst}",
                   [(store (adde (load addr:$dst), 
                                 (i8 (load addr:$src))), addr:$dst),
                    (implicit SR)]>;
}

} // Uses = [SR]

let isCommutable = 1 in { // X = AND Y, Z  == X = AND Z, Y
def AND8rr  : I8rr<0x0,
                   (outs GR8:$dst), (ins GR8:$src, GR8:$src2),
                   "and.b\t{$src2, $dst}",
                   [(set GR8:$dst, (and GR8:$src, GR8:$src2)),
                    (implicit SR)]>;
}

def AND8ri  : I8ri<0x0,
                   (outs GR8:$dst), (ins GR8:$src, i8imm:$src2),
                   "and.b\t{$src2, $dst}",
                   [(set GR8:$dst, (and GR8:$src, imm:$src2)),
                    (implicit SR)]>;

def AND8rm  : I8rm<0x0,
                   (outs GR8:$dst), (ins GR8:$src, memsrc:$src2),
                   "and.b\t{$src2, $dst}",
                   [(set GR8:$dst, (and GR8:$src, (load addr:$src2))),
                    (implicit SR)]>;

let mayLoad = 1, hasExtraDefRegAllocReq = 1, 
Constraints = "$base = $base_wb, $src = $dst" in {
def AND8rm_POST : IForm8<0x0, DstReg, SrcPostInc, Size2Bytes,
                         (outs GR8:$dst, GR8:$base_wb),
                         (ins GR8:$src, GR8:$base),
                         "and.b\t{@$base+, $dst}", []>;
}

let Constraints = "" in {
def AND8mr  : I8mr<0x0,
                   (outs), (ins memdst:$dst, GR8:$src),
                   "and.b\t{$src, $dst}",
                   [(store (and (load addr:$dst), GR8:$src), addr:$dst),
                    (implicit SR)]>;

def AND8mi  : I8mi<0x0,
                   (outs), (ins memdst:$dst, i8imm:$src),
                   "and.b\t{$src, $dst}",
                   [(store (and (load addr:$dst), (i8 imm:$src)), addr:$dst),
                    (implicit SR)]>;

def AND8mm  : I8mm<0x0,
                   (outs), (ins memdst:$dst, memsrc:$src),
                   "and.b\t{$src, $dst}",
                   [(store (and (load addr:$dst), 
                                (i8 (load addr:$src))), addr:$dst),
                    (implicit SR)]>;
}

let isCommutable = 1 in { // X = OR Y, Z  == X = OR Z, Y
def OR8rr  : I8rr<0x0,
                  (outs GR8:$dst), (ins GR8:$src, GR8:$src2),
                  "bis.b\t{$src2, $dst}",
                  [(set GR8:$dst, (or GR8:$src, GR8:$src2))]>;
}

def OR8ri  : I8ri<0x0,
                  (outs GR8:$dst), (ins GR8:$src, i8imm:$src2),
                  "bis.b\t{$src2, $dst}",
                  [(set GR8:$dst, (or GR8:$src, imm:$src2))]>;

def OR8rm  : I8rm<0x0,
                  (outs GR8:$dst), (ins GR8:$src, memsrc:$src2),
                  "bis.b\t{$src2, $dst}",
                  [(set GR8:$dst, (or GR8:$src, (load addr:$src2)))]>;

let mayLoad = 1, hasExtraDefRegAllocReq = 1, 
Constraints = "$base = $base_wb, $src = $dst" in {
def OR8rm_POST : IForm8<0x0, DstReg, SrcPostInc, Size2Bytes,
                        (outs GR8:$dst, GR8:$base_wb),
                        (ins GR8:$src, GR8:$base),
                        "bis.b\t{@$base+, $dst}", []>;
}

let Constraints = "" in {
def OR8mr  : I8mr<0x0,
                  (outs), (ins memdst:$dst, GR8:$src),
                  "bis.b\t{$src, $dst}",
                  [(store (or (load addr:$dst), GR8:$src), addr:$dst)]>;

def OR8mi  : I8mi<0x0, 
                  (outs), (ins memdst:$dst, i8imm:$src),
                  "bis.b\t{$src, $dst}",
                  [(store (or (load addr:$dst), (i8 imm:$src)), addr:$dst)]>;

def OR8mm  : I8mm<0x0,
                  (outs), (ins memdst:$dst, memsrc:$src),
                  "bis.b\t{$src, $dst}",
                  [(store (or (i8 (load addr:$dst)),
                              (i8 (load addr:$src))), addr:$dst)]>;
}

// bic does not modify condition codes
def BIC8rr :  I8rr<0x0,
                   (outs GR8:$dst), (ins GR8:$src, GR8:$src2),
                   "bic.b\t{$src2, $dst}",
                   [(set GR8:$dst, (and GR8:$src, (not GR8:$src2)))]>;

def BIC8rm :  I8rm<0x0,
                   (outs GR8:$dst), (ins GR8:$src, memsrc:$src2),
                   "bic.b\t{$src2, $dst}",
                    [(set GR8:$dst, (and GR8:$src, (not (i8 (load addr:$src2)))))]>;

let Constraints = "" in {
def BIC8mr :  I8mr<0x0,
                   (outs), (ins memdst:$dst, GR8:$src),
                   "bic.b\t{$src, $dst}",
                   [(store (and (load addr:$dst), (not GR8:$src)), addr:$dst)]>;

def BIC8mm :  I8mm<0x0,
                   (outs), (ins memdst:$dst, memsrc:$src),
                   "bic.b\t{$src, $dst}",
                   [(store (and (load addr:$dst),
                                (not (i8 (load addr:$src)))), addr:$dst)]>;
}

let isCommutable = 1 in { // X = XOR Y, Z  == X = XOR Z, Y
def XOR8rr  : I8rr<0x0,
                   (outs GR8:$dst), (ins GR8:$src, GR8:$src2),
                   "xor.b\t{$src2, $dst}",
                   [(set GR8:$dst, (xor GR8:$src, GR8:$src2)),
                    (implicit SR)]>;
}

def XOR8ri  : I8ri<0x0,
                   (outs GR8:$dst), (ins GR8:$src, i8imm:$src2),
                   "xor.b\t{$src2, $dst}",
                   [(set GR8:$dst, (xor GR8:$src, imm:$src2)),
                    (implicit SR)]>;

def XOR8rm  : I8rm<0x0,
                   (outs GR8:$dst), (ins GR8:$src, memsrc:$src2),
                   "xor.b\t{$src2, $dst}",
                   [(set GR8:$dst, (xor GR8:$src, (load addr:$src2))),
                    (implicit SR)]>;

let mayLoad = 1, hasExtraDefRegAllocReq = 1, 
Constraints = "$base = $base_wb, $src = $dst" in {
def XOR8rm_POST : IForm8<0x0, DstReg, SrcPostInc, Size2Bytes,
                         (outs GR8:$dst, GR8:$base_wb),
                         (ins GR8:$src, GR8:$base),
                         "xor.b\t{@$base+, $dst}", []>;
}

let Constraints = "" in {
def XOR8mr  : I8mr<0x0,
                   (outs), (ins memdst:$dst, GR8:$src),
                   "xor.b\t{$src, $dst}",
                   [(store (xor (load addr:$dst), GR8:$src), addr:$dst),
                    (implicit SR)]>;

def XOR8mi  : I8mi<0x0,
                   (outs), (ins memdst:$dst, i8imm:$src),
                   "xor.b\t{$src, $dst}",
                   [(store (xor (load addr:$dst), (i8 imm:$src)), addr:$dst),
                    (implicit SR)]>;

def XOR8mm  : I8mm<0x0,
                   (outs), (ins memdst:$dst, memsrc:$src),
                   "xor.b\t{$src, $dst}",
                   [(store (xor (load addr:$dst), (i8 (load addr:$src))), addr:$dst),
                    (implicit SR)]>;
}


def SUB8rr  : I8rr<0x0,
                   (outs GR8:$dst), (ins GR8:$src, GR8:$src2),
                   "SUB8RR\t{$src2, $dst}",
                   [(set GR8:$dst, (sub GR8:$src, GR8:$src2)),
                    (implicit SR)]>;

def SUB8ri  : I8ri<0x0,
                   (outs GR8:$dst), (ins GR8:$src, i8imm:$src2),
                   "SUB8ri\t{$src2, $dst}",
                   [(set GR8:$dst, (sub GR8:$src, imm:$src2)),
                    (implicit SR)]>;

def SUB8rm  : I8rm<0x0,
                   (outs GR8:$dst), (ins GR8:$src, memsrc:$src2),
                   "SUB8RM\t{$src2, $dst}",
                   [(set GR8:$dst, (sub GR8:$src, (load addr:$src2))),
                    (implicit SR)]>;

let mayLoad = 1, hasExtraDefRegAllocReq = 1, 
Constraints = "$base = $base_wb, $src = $dst" in {
def SUB8rm_POST : IForm8<0x0, DstReg, SrcPostInc, Size2Bytes,
                         (outs GR8:$dst, GR8:$base_wb),
                         (ins GR8:$src, GR8:$base),
                         "SUB8RM_POST\t{@$base+, $dst}", []>;
}

let Constraints = "" in {
def SUB8mr  : I8mr<0x0,
                   (outs), (ins memdst:$dst, GR8:$src),
                   "SUB8MR\t{$src, $dst}",
                   [(store (sub (load addr:$dst), GR8:$src), addr:$dst),
                    (implicit SR)]>;

def SUB8mi  : I8mi<0x0,
                   (outs), (ins memdst:$dst, i8imm:$src),
                   "SUB8MI\t{$src, $dst}",
                   [(store (sub (load addr:$dst), (i8 imm:$src)), addr:$dst),
                    (implicit SR)]>;

def SUB8mm  : I8mm<0x0,
                   (outs), (ins memdst:$dst, memsrc:$src),
                   "SUB8MM\t{$src, $dst}",
                   [(store (sub (load addr:$dst), 
                                (i8 (load addr:$src))), addr:$dst),
                    (implicit SR)]>;
}

let Uses = [SR] in {
def SBC8rr  : I8rr<0x0,
                   (outs GR8:$dst), (ins GR8:$src, GR8:$src2),
                   "SBC8RR\t{$src2, $dst}",
                   [(set GR8:$dst, (sube GR8:$src, GR8:$src2)),
                    (implicit SR)]>;

def SBC8ri  : I8ri<0x0,
                   (outs GR8:$dst), (ins GR8:$src, i8imm:$src2),
                   "SBC8RI\t{$src2, $dst}",
                   [(set GR8:$dst, (sube GR8:$src, imm:$src2)),
                    (implicit SR)]>;

def SBC8rm  : I8rm<0x0,
                   (outs GR8:$dst), (ins GR8:$src, memsrc:$src2),
                   "subc.b\t{$src2, $dst}",
                   [(set GR8:$dst, (sube GR8:$src, (load addr:$src2))),
                    (implicit SR)]>;

let Constraints = "" in {
def SBC8mr  : I8mr<0x0,
                   (outs), (ins memdst:$dst, GR8:$src),
                   "subc.b\t{$src, $dst}",
                  [(store (sube (load addr:$dst), GR8:$src), addr:$dst),
                   (implicit SR)]>;

def SBC8mi  : I8mi<0x0,
                   (outs), (ins memdst:$dst, i8imm:$src),
                   "subc.b\t{$src, $dst}",
                   [(store (sube (load addr:$dst), (i8 imm:$src)), addr:$dst),
                    (implicit SR)]>;

def SBC8mm  : I8mm<0x0,
                   (outs), (ins memdst:$dst, memsrc:$src),
                   "subc.b\t{$src, $dst}",
                   [(store (sube (load addr:$dst),
                                 (i8 (load addr:$src))), addr:$dst),
                    (implicit SR)]>;
}

} // Uses = [SR]

// FIXME: memory variant!
def SAR8r1  : II8r<0x0,
                   (outs GR8:$dst), (ins GR8:$src),
                   "rra.b\t$dst",
                   [(set GR8:$dst, (PIC16rra GR8:$src)),
                    (implicit SR)]>;

def SHL8r1  : I8rr<0x0,
                   (outs GR8:$dst), (ins GR8:$src),
                   "rla.b\t$dst",
                   [(set GR8:$dst, (PIC16rla GR8:$src)),
                    (implicit SR)]>;

def SAR8r1c  : Pseudo<(outs GR8:$dst), (ins GR8:$src),
                      "clrc\n\t"
                      "rrc.b\t$dst",
                      [(set GR8:$dst, (PIC16rrc GR8:$src)),
                       (implicit SR)]>;

} // Defs = [SR]

} // Constraints = "$src = $dst"

// Integer comparisons
let Defs = [SR] in {
def CMP8rr  : I8rr<0x0,
                   (outs), (ins GR8:$src, GR8:$src2),
                   "cmp.b\t{$src2, $src}",
                   [(PIC16cmp GR8:$src, GR8:$src2), (implicit SR)]>;

def CMP8ri  : I8ri<0x0,
                   (outs), (ins GR8:$src, i8imm:$src2),
                   "cmp.b\t{$src2, $src}",
                   [(PIC16cmp GR8:$src, imm:$src2), (implicit SR)]>;

def CMP8mi  : I8mi<0x0,
                   (outs), (ins memsrc:$src, i8imm:$src2),
                   "cmp.b\t{$src2, $src}",
                   [(PIC16cmp (load addr:$src),
                               (i8 imm:$src2)), (implicit SR)]>;

def CMP8rm  : I8rm<0x0,
                   (outs), (ins GR8:$src, memsrc:$src2),
                   "cmp.b\t{$src2, $src}",
                   [(PIC16cmp GR8:$src, (load addr:$src2)), 
                    (implicit SR)]>;

def CMP8mr  : I8mr<0x0,
                   (outs), (ins memsrc:$src, GR8:$src2),
                   "cmp.b\t{$src2, $src}",
                   [(PIC16cmp (load addr:$src), GR8:$src2),
                    (implicit SR)]>;

// BIT TESTS, just sets condition codes
// Note that the C condition is set differently than when using CMP.
let isCommutable = 1 in {
def BIT8rr  : I8rr<0x0,
                   (outs), (ins GR8:$src, GR8:$src2),
                   "bit.b\t{$src2, $src}",
                   [(PIC16cmp (and_su GR8:$src, GR8:$src2), 0),
                    (implicit SR)]>;
}
def BIT8ri  : I8ri<0x0,
                   (outs), (ins GR8:$src, i8imm:$src2),
                   "bit.b\t{$src2, $src}",
                   [(PIC16cmp (and_su GR8:$src, imm:$src2), 0),
                    (implicit SR)]>;

def BIT8rm  : I8rm<0x0,
                   (outs), (ins GR8:$src, memdst:$src2),
                   "bit.b\t{$src2, $src}",
                   [(PIC16cmp (and_su GR8:$src,  (load addr:$src2)), 0),
                    (implicit SR)]>;

def BIT8mr  : I8mr<0x0,
                  (outs), (ins memsrc:$src, GR8:$src2),
                  "bit.b\t{$src2, $src}",
                  [(PIC16cmp (and_su (load addr:$src), GR8:$src2), 0),
                   (implicit SR)]>;

def BIT8mi  : I8mi<0x0,
                   (outs), (ins memsrc:$src, i8imm:$src2),
                   "bit.b\t{$src2, $src}",
                   [(PIC16cmp (and_su (load addr:$src), (i8 imm:$src2)), 0),
                    (implicit SR)]>;
def BIT8mm  : I8mm<0x0,
                   (outs), (ins memsrc:$src, memsrc:$src2),
                   "bit.b\t{$src2, $src}",
                   [(PIC16cmp (and_su (i8 (load addr:$src)),
                                       (load addr:$src2)),
                                 0),
                      (implicit SR)]>;
} // Defs = [SR]

//===----------------------------------------------------------------------===//
// calls
def : Pat<(PIC16call (i8 tglobaladdr:$dst)),
          (CALLi tglobaladdr:$dst)>;
def : Pat<(PIC16call (i8 texternalsym:$dst)),
          (CALLi texternalsym:$dst)>;

def : Pat<(addc GR8:$src, GR8:$src2),
          (ADDrr GR8:$src, GR8:$src2)>;
def : Pat<(addc GR8:$src, (load addr:$src2)),
          (ADDrm GR8:$src, addr:$src2)>;
def : Pat<(addc GR8:$src, imm:$src2),
          (ADDri GR8:$src, imm:$src2)>;
def : Pat<(store (addc (load addr:$dst), GR8:$src), addr:$dst),
          (ADDmr addr:$dst, GR8:$src)>;
def : Pat<(store (addc (load addr:$dst), (i8 (load addr:$src))), addr:$dst),
          (ADDmm addr:$dst, addr:$src)>;

def : Pat<(subc GR8:$src, GR8:$src2),
          (SUB8rr GR8:$src, GR8:$src2)>;
def : Pat<(subc GR8:$src, (load addr:$src2)),
          (SUB8rm GR8:$src, addr:$src2)>;
def : Pat<(subc GR8:$src, imm:$src2),
          (SUB8ri GR8:$src, imm:$src2)>;
def : Pat<(store (subc (load addr:$dst), GR8:$src), addr:$dst),
          (SUB8mr addr:$dst, GR8:$src)>;
def : Pat<(store (subc (load addr:$dst), (i8 (load addr:$src))), addr:$dst),
          (SUB8mm addr:$dst, addr:$src)>;
