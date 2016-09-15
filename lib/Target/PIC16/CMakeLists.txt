set(LLVM_TARGET_DEFINITIONS PIC16.td)

tablegen(LLVM PIC16GenRegisterInfo.inc -gen-register-info)
tablegen(LLVM PIC16GenInstrInfo.inc -gen-instr-info)
tablegen(LLVM PIC16GenDisassemblerTables.inc -gen-disassembler)
tablegen(LLVM PIC16GenMCCodeEmitter.inc -gen-emitter)
tablegen(LLVM PIC16GenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM PIC16GenDAGISel.inc -gen-dag-isel)
tablegen(LLVM PIC16GenFastISel.inc -gen-fast-isel)
tablegen(LLVM PIC16GenCallingConv.inc -gen-callingconv)
tablegen(LLVM PIC16GenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM PIC16GenAsmMatcher.inc -gen-asm-matcher)
tablegen(LLVM PIC16GenMCPseudoLowering.inc -gen-pseudo-lowering)
add_public_tablegen_target(PIC16CommonTableGen)

add_llvm_target(PIC16CodeGen
  PIC16FrameLowering.cpp
  PIC16HardFloat.cpp
  PIC16HardFloatInfo.cpp
  PIC16InstrInfo.cpp
  PIC16ISelDAGToDAG.cpp
  PIC16ISelLowering.cpp
  PIC16RegisterInfo.cpp
  PIC16AnalyzeImmediate.cpp
  PIC16AsmPrinter.cpp
  PIC16CCState.cpp
  PIC16ConstantIslandPass.cpp
  PIC16DelaySlotFiller.cpp
  PIC16FastISel.cpp
  PIC16HazardSchedule.cpp
  PIC16InstrInfo.cpp
  PIC16ISelDAGToDAG.cpp
  PIC16ISelLowering.cpp
  PIC16FrameLowering.cpp
  PIC16LongBranch.cpp
  PIC16MCInstLower.cpp
  PIC16MachineFunction.cpp
  PIC16ModuleISelDAGToDAG.cpp
  PIC16OptimizePICCall.cpp
  PIC16Os16.cpp
  PIC16RegisterInfo.cpp
  PIC16SEFrameLowering.cpp
  PIC16SEInstrInfo.cpp
  PIC16SEISelDAGToDAG.cpp
  PIC16SEISelLowering.cpp
  PIC16SERegisterInfo.cpp
  PIC16Subtarget.cpp
  PIC16TargetMachine.cpp
  PIC16TargetObjectFile.cpp
  )

add_subdirectory(InstPrinter)
add_subdirectory(Disassembler)
add_subdirectory(TargetInfo)
add_subdirectory(MCTargetDesc)
add_subdirectory(AsmParser)

