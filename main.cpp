#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <map>

#include "main.h"
#include "parser.h"
#include "parse-error.h"

std::map<int, std::string> addr_to_sym;
std::unordered_map<std::string, int> sym_to_addr;
std::unordered_map<std::string, int> sym_to_module;
std::unordered_map<int, int> module_to_addr;

void add_sym(std::string const & sym, int addr, int module_num) {
  int start = module_to_addr[module_num];
  std::pair<std::string, int> p(sym, module_num);

  auto result = sym_to_module.insert(p);
  if( !result.second ) {
    throw std::runtime_error("symbol already defined");
  }

  std::pair<std::string, int> p2(sym, start + addr);
  std::pair<int, std::string> p3(start + addr, sym);
  sym_to_addr.insert(p2);
  addr_to_sym.insert(p3);
}

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

  for(
   int module_num = 1, module_addr = 0;
   Parser.get_defcount(defcount);
   ++module_num )
  {
    for( int i = 0; i < defcount; ++i ) {
      string symbol;  int relative;

      Parser.get_symbol(symbol);
      Parser.get_address(relative);

      module_to_addr[module_num] = module_addr;
      add_sym(symbol, relative, module_num);
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
      Parser.get_instruction_type(instr_type);
      Parser.get_instruction(instr);
      cout << "code: " << instr_type << ' ' << instr << endl;
    }

    module_addr += codecount;
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

  //========== second pass =============//
  cout << "---- second pass ------- " << endl;
  p.reset();
  
  cout << "complete..." << endl;
  return 0;
}
