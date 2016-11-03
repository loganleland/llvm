set(LLVM_TARGET_DEFINITIONS PIC16.td)

tablegen(LLVM PIC16GenRegisterInfo.inc -gen-register-info)
tablegen(LLVM PIC16GenInstrInfo.inc -gen-instr-info)
tablegen(LLVM PIC16GenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM PIC16GenDAGISel.inc -gen-dag-isel)
tablegen(LLVM PIC16GenCallingConv.inc -gen-callingconv)
tablegen(LLVM PIC16GenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM PIC16GenAsmMatcher.inc -gen-asm-matcher)
add_public_tablegen_target(PIC16CommonTableGen)

add_llvm_target(PIC16CodeGen
  PIC16BranchSelector.cpp
  PIC16FrameLowering.cpp
  PIC16ISelDAGToDAG.cpp
  PIC16ISelLowering.cpp
  PIC16InstrInfo.cpp
  PIC16MCInstLower.cpp
  PIC16MachineFunctionInfo.cpp
  PIC16RegisterInfo.cpp
  PIC16AsmPrinter.cpp
  PIC16RegisterInfo.cpp
  PIC16Subtarget.cpp
  PIC16TargetMachine.cpp
  )

add_subdirectory(InstPrinter)
add_subdirectory(TargetInfo)
