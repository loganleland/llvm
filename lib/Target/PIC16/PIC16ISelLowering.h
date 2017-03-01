//===-- PIC16ISelLowering.h - PIC16 DAG Lowering Interface ----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the interfaces that PIC16 uses to lower LLVM code into a
// selection DAG.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_PIC16_PIC16ISELLOWERING_H
#define LLVM_LIB_TARGET_PIC16_PIC16ISELLOWERING_H

#include "PIC16.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/Target/TargetLowering.h"

namespace llvm {
  namespace PIC16ISD {
    enum NodeType : unsigned {
      FIRST_NUMBER = ISD::BUILTIN_OP_END,

      /// Return with a flag operand. Operand 0 is the chain operand.
      RET_FLAG,

      /// Same as RET_FLAG, but used for returning from ISRs.
      RETI_FLAG,

      /// Y = R{R,L}A X, rotate right (left) arithmetically
      RRA, RLA,

      /// Y = RRC X, rotate right via carry
      RRC,

      /// CALL - These operations represent an abstract call
      /// instruction, which includes a bunch of information.
      CALL,

      /// Wrapper - A wrapper node for TargetConstantPool, TargetExternalSymbol,
      /// and TargetGlobalAddress.
      Wrapper,

      /// CMP - Compare instruction.
      CMP,

      /// SetCC - Operand 0 is condition code, and operand 1 is the flag
      /// operand produced by a CMP instruction.
      SETCC,

      /// PIC16 conditional branches. Operand 0 is the chain operand, operand 1
      /// is the block to branch if condition is true, operand 2 is the
      /// condition code, and operand 3 is the flag operand produced by a CMP
      /// instruction.
      BR_CC,

      /// SELECT_CC - Operand 0 and operand 1 are selection variable, operand 3
      /// is condition code and operand 4 is flag operand.
      SELECT_CC,

      /// SHL, SRA, SRL - Non-constant shifts.
      SHL, SRA, SRL
    };
  }

  class PIC16Subtarget;
  class PIC16TargetLowering : public TargetLowering {
  public:
    explicit PIC16TargetLowering(const TargetMachine &TM,
                                  const PIC16Subtarget &STI);

    MVT getScalarShiftAmountTy(const DataLayout &, EVT) const override {
      return MVT::i8;
    }

    /// LowerOperation - Provide custom lowering hooks for some operations.
    SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const override;

    /// getTargetNodeName - This method returns the name of a target specific
    /// DAG node.
    const char *getTargetNodeName(unsigned Opcode) const override;

    SDValue LowerShifts(SDValue Op, SelectionDAG &DAG) const;
    SDValue LowerGlobalAddress(SDValue Op, SelectionDAG &DAG) const;
    SDValue LowerBlockAddress(SDValue Op, SelectionDAG &DAG) const;
    SDValue LowerExternalSymbol(SDValue Op, SelectionDAG &DAG) const;
    SDValue LowerBR_CC(SDValue Op, SelectionDAG &DAG) const;
    SDValue LowerSETCC(SDValue Op, SelectionDAG &DAG) const;
    SDValue LowerSELECT_CC(SDValue Op, SelectionDAG &DAG) const;
    SDValue LowerSIGN_EXTEND(SDValue Op, SelectionDAG &DAG) const;
    SDValue LowerRETURNADDR(SDValue Op, SelectionDAG &DAG) const;
    SDValue LowerFRAMEADDR(SDValue Op, SelectionDAG &DAG) const;
    SDValue LowerVASTART(SDValue Op, SelectionDAG &DAG) const;
    SDValue LowerJumpTable(SDValue Op, SelectionDAG &DAG) const;
    SDValue getReturnAddressFrameIndex(SelectionDAG &DAG) const;

    TargetLowering::ConstraintType
    getConstraintType(StringRef Constraint) const override;
    std::pair<unsigned, const TargetRegisterClass *>
    getRegForInlineAsmConstraint(const TargetRegisterInfo *TRI,
                                 StringRef Constraint, MVT VT) const override;

  private:
    SDValue LowerCCCCallTo(SDValue Chain, SDValue Callee,
                           CallingConv::ID CallConv, bool isVarArg,
                           bool isTailCall,
                           const SmallVectorImpl<ISD::OutputArg> &Outs,
                           const SmallVectorImpl<SDValue> &OutVals,
                           const SmallVectorImpl<ISD::InputArg> &Ins,
                           const SDLoc &dl, SelectionDAG &DAG,
                           SmallVectorImpl<SDValue> &InVals) const;

    SDValue LowerCCCArguments(SDValue Chain, CallingConv::ID CallConv,
                              bool isVarArg,
                              const SmallVectorImpl<ISD::InputArg> &Ins,
                              const SDLoc &dl, SelectionDAG &DAG,
                              SmallVectorImpl<SDValue> &InVals) const;

    SDValue LowerCallResult(SDValue Chain, SDValue InFlag,
                            CallingConv::ID CallConv, bool isVarArg,
                            const SmallVectorImpl<ISD::InputArg> &Ins,
                            const SDLoc &dl, SelectionDAG &DAG,
                            SmallVectorImpl<SDValue> &InVals) const;

    SDValue
    LowerFormalArguments(SDValue Chain, CallingConv::ID CallConv, bool isVarArg,
                         const SmallVectorImpl<ISD::InputArg> &Ins,
                         const SDLoc &dl, SelectionDAG &DAG,
                         SmallVectorImpl<SDValue> &InVals) const override;
    SDValue
      LowerCall(TargetLowering::CallLoweringInfo &CLI,
                SmallVectorImpl<SDValue> &InVals) const override;

    SDValue LowerReturn(SDValue Chain, CallingConv::ID CallConv, bool isVarArg,
                        const SmallVectorImpl<ISD::OutputArg> &Outs,
                        const SmallVectorImpl<SDValue> &OutVals,
                        const SDLoc &dl, SelectionDAG &DAG) const override;

    bool getPostIndexedAddressParts(SDNode *N, SDNode *Op,
                                    SDValue &Base,
                                    SDValue &Offset,
                                    ISD::MemIndexedMode &AM,
                                    SelectionDAG &DAG) const override;
  };
} // namespace llvm

#endif
