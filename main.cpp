#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "main.h"
#include "parser.h"

std::ostream& print_position(parser const &p) {
  return std::cout << p.get_line_num() << ':' << p.get_line_num();
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



  parser p(infile);

  int defcount, usecount, codecount;

  while( p >> defcount ) {

    cout << "# def: " << defcount << endl;
    for( int i = 0; i < defcount; ++i ) {
      string symbol;  int relative;

      p >> symbol;
      cout << p.get_line_num() << ":" << p.get_char_offset();
      cout << " def: " << symbol << endl;
      p >> relative;
      cout << p.get_line_num() << ":" << p.get_char_offset();
      cout << " @ " << relative << endl;
    }

    p >> usecount;
    cout << "# use: " << usecount << endl;
    for( int i = 0; i < usecount; ++i ) {
      string symbol;
      p >> symbol;
      cout << "use: " << symbol << endl;
    }

    p >> codecount;
    cout << "# code: " << codecount << endl;
    for( int i = 0; i < codecount; ++i ) {
      char instr_type;
      int instr;
      p >> instr_type;
      p >> instr;
      cout << "code: " << instr_type << ' ' << instr << endl;
    }
  }

  if( !p.eof() ) {
    cout << "there's more" << endl;
  }

  cout << "complete..." << endl;
  return 0;
}
