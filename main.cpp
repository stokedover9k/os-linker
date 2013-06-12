#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "main.h"
#include "parser.h"
#include "tokenizer.h"

bool white_space( int c ) {
  switch( c ) {
    case ' ':
    case '\t':  return true;
    default:    return false;
  }
}

//============ main ===================//
int main( int argc, char *argv[] ) {   //
//=====================================//
  using namespace std;
  cout << "starting..." << endl;

  //-------- set up input ------------
  ifstream infile;
  if( argc > 1 ) {
    infile.open(argv[1]);
    if( !infile ) {
      cerr << "Error: failed to open input file: " << argv[1] 
           << ".\nTerminating...";
      return 1;
    }
  }
  else {
    cerr << "Error: no input file given.\nTerminating..." << endl;
  }

  string str;
  tokenizer tk(infile);
  while( tk.get_string(str) ) {
    cout << "line " << tk.get_line_num();
    cout << " char " << tk.get_char_offset();
    cout << ": " << str << endl;
  }

  cout << "complete..." << endl;
  return 0;
}
