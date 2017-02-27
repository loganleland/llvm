// Author: Ryan Newlun
// File: instcount.cpp
// Purpose: This file counts the occurrence of instructions in an assembly
//          output file as defined by the PIC16F877 instruction set.
// Input: 1 command line argument: the input assembly file.
// Output: Outputs currently to standard out.
// Built with clang++ instcount.cpp -o instcount
// Execute with ./instcount input_file.asm

#include <iostream>
#include <string>
#include <fstream>
#include <map>

using namespace std;

int main(int argc, char* argv[]) {
  
  // Initialize map with all PIC16F877 instructions
  map<string, int> instcount;
  instcount["addwf"] = 0;
  instcount["andwf"] = 0;
  instcount["clrf"] = 0;
  instcount["clrw"] = 0;
  instcount["comf"] = 0;
  instcount["decf"] = 0;
  instcount["decfsz"] = 0;
  instcount["incf"] = 0;
  instcount["incfsz"] = 0;
  instcount["iorwf"] = 0;
  instcount["movf"] = 0;
  instcount["movwf"] = 0;
  instcount["nop"] = 0;
  instcount["rlf"] = 0;
  instcount["rrf"] = 0;
  instcount["subwf"] = 0;
  instcount["swapf"] = 0;
  instcount["xorwf"] = 0;
  instcount["bcf"] = 0;
  instcount["bsf"] = 0;
  instcount["btfsc"] = 0;
  instcount["btfss"] = 0;
  instcount["addlw"] = 0;
  instcount["andlw"] = 0;
  instcount["call"] = 0;
  instcount["clrwdt"] = 0;
  instcount["goto"] = 0;
  instcount["iorlw"] = 0;
  instcount["movlw"] = 0;
  instcount["retfie"] = 0;
  instcount["retlw"] = 0;
  instcount["return"] = 0;
  instcount["sleep"] = 0;
  instcount["sublw"] = 0;
  instcount["xorlw"] = 0;

  // Open input file for reading
  // FIXME: Currently has issues with separating out instructions with commas
  ifstream infile (argv[1]);
  string word;
  if (infile.is_open()){
    while (infile >> word){
      if (instcount.count(word)) {
        ++instcount[word];
      }
    }
    infile.close();
  }

  // Output results sorted alphabetically to standard out
  // FIXME: Output to a text file
  for (map<string, int>::iterator it=instcount.begin(); it!=instcount.end();
                                                        ++it) {
    cout << it->first << " " << it->second << "\n";
  }

  return 0;
}
