set(LLVM_TARGET_DEFINITIONS NOPE.td)

tablegen(LLVM NOPEGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM NOPEGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM NOPEGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM NOPEGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM NOPEGenCallingConv.inc -gen-callingconv)
tablegen(LLVM NOPEGenSubtargetInfo.inc -gen-subtarget)
add_public_tablegen_target(NOPECommonTableGen)

add_llvm_target(NOPECodeGen
  NOPEBranchSelector.cpp
  NOPEISelDAGToDAG.cpp
  NOPEISelLowering.cpp
  NOPEInstrInfo.cpp
  NOPEFrameLowering.cpp
  NOPEMachineFunctionInfo.cpp
  NOPERegisterInfo.cpp
  NOPESubtarget.cpp
  NOPETargetMachine.cpp
  NOPEAsmPrinter.cpp
  NOPEMCInstLower.cpp
  )

add_subdirectory(InstPrinter)
add_subdirectory(TargetInfo)
add_subdirectory(MCTargetDesc)
