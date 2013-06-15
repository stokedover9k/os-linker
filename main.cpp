#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <map>
#include <vector>

#include "main.h"
#include "parser.h"
#include "parse-error.h"

std::map<int, std::string> addr_to_sym;
std::unordered_map<std::string, int> sym_to_addr;
std::unordered_map<std::string, int> sym_to_module;
std::map<int, int> module_to_addr;
std::map<int, int> module_sizes;
std::vector<std::string> use_list;

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

void print_sym_table() {
  for(
   auto i = addr_to_sym.begin();
   i != addr_to_sym.end(); i++)
  {
    std::cout << i->second << '=' << i->first << '\n'; 
  }
  std::flush(std::cout);
}

void print_absolute_addr( char instr_type, int instr, int module_num ) {
  int relative = instr % 1000;
  int opcode = instr / 1000;
  int absolute;
  char const *error = NULL;
  switch( instr_type ) {
    case 'I':  case 'i':    absolute = relative;  break;

    case 'A':  case 'a':    
    absolute = relative;  break;
    if( absolute > MACHINE_SIZE ) {
      absolute = 0;
      error = "Error: Absolute address exceeds machine size; zero used";
    }

    case 'R':  case 'r':
    if( relative > module_sizes[module_num]) {
      relative = 0;
      error = "Error: Relative address exceeds module size; zero used";
    }
    absolute = relative + module_to_addr[module_num];
    break;

    case 'E':  case 'e':
    if( relative > use_list.size() ) {
      absolute = relative;
      error = "Error: External address exceeds length of uselist; treated as immediate";
    }
    else
      absolute = sym_to_addr[use_list[relative]];
    break;
  }

  std::cout << (absolute + 1000 * opcode);
  if( error )
    std::cout << ' ' << error;
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

//------------ FIRST PASS ------------//
void first_pass( parser &Parser ) {   //
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
    module_to_addr[module_num] = module_addr;

    for( int i = 0; i < defcount; ++i ) {
      string symbol;
      int relative;

      Parser.get_symbol(symbol);
      Parser.get_address(relative);

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
    }

    module_sizes[module_num] = codecount;
    module_addr += codecount;
  }

  if( !Parser.eof() ) {
    syntax_err(NUM_EXPECTED, Parser);
  }

}

//------------ SECOND PASS ------------//
void second_pass( parser &Parser ) {   //
//-------------------------------------//
  using namespace std;
  int defcount, usecount, codecount, memory_addr = 0;

  for(
    auto module_ptr = module_to_addr.begin();
    module_ptr != module_to_addr.end();
    module_ptr++ )
  {
    cout << '+' << module_ptr->second << endl;

    Parser.get_defcount(defcount);
    for( int i = 0; i < defcount; ++i ) {
      string symbol;
      int relative;

      Parser.get_symbol(symbol);
      Parser.get_address(relative);
    }

    Parser.get_usecount( usecount );
    use_list.clear();
    for( int i = 0; i < usecount; ++i ) {
      string symbol;
      Parser.get_symbol(symbol);
      use_list.push_back(symbol);
    }

    Parser.get_codecount( codecount );
    for( int i = 0; i < codecount; ++i ) {
      char instr_type;
      int instr;

      Parser.get_instruction_type(instr_type);
      Parser.get_instruction(instr);

      cout << setfill('0') << setw(3) << i
           << " (" << instr_type << ' ' << instr << "): ";

      print_absolute_addr(instr_type, instr, module_ptr->first);
      cout << endl;
    }
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
      cout << "Not a valid inputfile <" << argv[1] << '>' << endl;
      return 1;
    }
  }
  else {
    cerr << "Error: no input file given.\nTerminating..." << endl;
  }



  parser p(infile);

  //========== first pass ==============//
  try {
    first_pass(p);
  }
  catch( parse_error& pe ) {
    cout << pe.what() << endl;
    exit(1);
  }

  cout << "Symbol Table\n";
  print_sym_table();
  cout << '\n';

  //========== second pass =============//
  cout << "Memory Map\n";
  p.reset();
  second_pass(p);
  
  cout << "complete..." << endl;
  return 0;
}
