#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "main.h"
#include "parser.h"
#include "parse-error.h"

struct pos_stamp {
  parser const & p;
  pos_stamp( parser const & _p ) : p(_p) {}
  std::ostream& operator()() {
    return std::cout << p.linenum() << ':' << p.lineoffset(); }
};

void syntax_err(parse_error_code errcode, parser const & p) {
  throw parse_error(errcode, p.linenum(), p.lineoffset());
}

//------------ FIRST PATH ------------//
void first_path( parser &Parser ) {   //
//------------------------------------//

  using namespace std;
  pos_stamp Pos(Parser);
  int defcount, usecount, codecount;
  parse_error_code errorcode;

  while( Parser.get_defcount(defcount) ) {
    for( int i = 0; i < defcount; ++i ) {
      string symbol;  int relative;

      Parser.get_symbol(symbol);
      Parser.get_address(relative);
    }

    Parser.get_usecount( usecount );
    for( int i = 0; i < usecount; ++i ) {
      string symbol;
      Parser.get_symbol(symbol);
    }

    Parser.get_codecount( codecount );
    for( int i = 0; i < codecount; ++i ) {
      char instr_type;
      int instr;
      Parser >> instr_type;
      Parser.get_instruction(instr);
      cout << "code: " << instr_type << ' ' << instr << endl;
    }
  }

  if( !Parser.eof() ) {
    syntax_err(NUM_EXPECTED, Parser);
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



  parser p(infile);

  //========== first pass ==============//
  try {
    first_path(p);
  }
  catch( parse_error& pe ) {
    cout << pe.what() << endl;
    exit(1);
  }
  
  cout << "complete..." << endl;
  return 0;
}
