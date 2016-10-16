//===- PIC16RegisterInfo.td - PIC16 Register defs ------------*- tblgen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source 
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//  Declarations that describe the PIC16 register file
//===----------------------------------------------------------------------===//
include "llvm/Target/Target.td"

class PIC16Reg<bits<16> Enc, string n> : Register<n> {
  let HWEncoding = Enc;
  let Namespace = "PIC16";
}

//Working accumulator register, non-addressable, should it even be here?
//def W      : PIC16Reg<"W">;

// PIC16 Control Registers.
//These first five are common across 4 banks.
def INDR    : PIC16Reg<0,  "INDR">;
def PCL     : PIC16Reg<3,  "PCL">;
def STATUS  : PIC16Reg<3,  "STATUS">;
def FSR     : PIC16Reg<4,  "FSR">;
def PCLATH  : PIC16Reg<10, "PCLATH">;
def INTCON  : PIC16Reg<11, "INTCON">;

def TMR0    : PIC16Reg<1,  "TMR0">;
def PORTA   : PIC16Reg<5,  "PORTA">;
def PORTB   : PIC16Reg<6,  "PORTB">;
def PORTC   : PIC16Reg<7,  "PORTC">;
def PORTD   : PIC16Reg<8,  "PORTD">;
def PORTE   : PIC16Reg<9,  "PORTE">;
def PIR1    : PIC16Reg<12,  "PIR1">;
def PIR2    : PIC16Reg<13,  "PIR2">;
def TMR1H   : PIC16Reg<14,  "TMR1H">;
def TMR1L   : PIC16Reg<15,  "TMR1L">;
def T1CON   : PIC16Reg<16,  "T1CON">;
def TMR2    : PIC16Reg<17,  "TMR2">;
def T2CON   : PIC16Reg<18,  "T2CON">;
def SSPBUF  : PIC16Reg<19,  "SSPBUF">;
def SSPCON  : PIC16Reg<20,  "SSPCON">;
def CCPR1L  : PIC16Reg<21,  "CCPR1L">;
def CCPR1H  : PIC16Reg<22,  "CCPR1H">;
def CCP1CON : PIC16Reg<23, "CCP1CON">;
def RCSTA   : PIC16Reg<24,  "RCSTA">;
def TXREG   : PIC16Reg<25,  "TXREG">;
def RCREG   : PIC16Reg<26,  "RCREG">;
def CCPR2L  : PIC16Reg<27,  "CCPR2L">;
def CCPR2H  : PIC16Reg<28,  "CCPR2H">;
def CCP2CON : PIC16Reg<29, "CCP2CON">;
def ADRESH  : PIC16Reg<30,  "ADRESH">;
def ADCON0  : PIC16Reg<31,  "ADCON0">;
//def         : PIC16Reg<"      ">;


//GPRnn refers to bank 1 GPR from 0x20 to 0x69
foreach Index = 32-111 in {
  def GPR#Index : PIC16Reg<#Index, "GPR"#Index>;
}

//16 registers at end, 4-bit subfield of address
//Shared GPR
foreach Index = 112-127 in {
  def SGPR#Index : PIC16Reg<#Index, "SGPR"#Index>;
}

//TODO: These registers are common on banks 0, 2

//TODO: These registers are common on banks 1, 3




// PIC16 Register classes.
// Question: include/Target/TargetRegisterInfo.h specifies fn: 
//   getPointerRegClass(...). Stale branch didn't implement; necessary??
// It would be in PIC16RegisterInfo.h/cpp.

// Control Registers
//NEXT

// These file registers are the same across all banks

def GPR : RegisterClass<"PIC16", [i8], 8,
          (add (sequence "GPR%u", 32, 111))>;

def SGPR : RegisterClass<"PIC16", [i8], 8,
           (add (sequence "SGPR%u", 32, 111))>;

//TODO: Banked registers; cost between same bank is 0. Same wires.
// Some registers are shared 0-3, some [0, 2], [1, 3], and some unique.
