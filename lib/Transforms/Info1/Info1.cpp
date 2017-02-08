//===- Info1.cpp - "Info1" ------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the "Info1" pass for data analytics
//
//===----------------------------------------------------------------------===//

#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

#define DEBUG_TYPE "info1"

// Global statistic variable
STATISTIC(Info1Counter, "Counts number of functions");

// Anonymous namespace
namespace {
// Code goes here to override the runOnFunction()


}

// Must have ID
//char Info1::ID = 0;
// Registering pass with PassManager
//static RegisterPass<Info1> X("info1", "Info1 analytic pass");
