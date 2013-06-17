#include "parser.h"

parser::parser( std::istream &_infile ) :
  infile(_infile),
  line_num(0),
  word_length(0)
{

}

void parser::load_line() {
  std::string line;
  if( std::getline(infile, line) ) {
    linestream.clear();
    linestream << line;
    ++line_num;
    char_offset = 1;
  }
}

void parser::load_word() {
  std::string word;
  char_offset += word_length;
  while( infile ) {
    for( 
      char c = linestream.peek();
      c == '\t' || c == ' ';
      c = linestream.peek() )
    {
      ++char_offset;
      linestream.ignore(1);
    }

    if( !(linestream >> word) ) {
      load_line();
      continue;
    }

    wordstream.clear();
    wordstream << ' ' << word;
    word_length = word.length();
    return;
  }
}

parser& parser::get_string(std::string &str) {
  load_word();
  wordstream >> str;
  return *this;
}

parser::operator bool() const {
  return infile.good() && !wordstream.fail();
}
bool parser::eof() const {
  return infile.eof();
}

parser& parser::operator>>( std::string &str ) {
  return get_string(str);
}

parser& parser::operator>>( int &i ) {
  load_word();
  wordstream >> i;
  return *this;
}

parser& parser::operator>>( char &c ) {
  load_word();
  wordstream >> c;
  return *this;
}

size_t parser::linenum() const { return line_num; }
size_t parser::lineoffset() const { return char_offset; }

parser& parser::get_defcount(int& i) {
  int defcount;
  if( !(*this >> defcount) ) {
    if( !eof() )
      throw parse_error(NUM_EXPECTED, linenum(), lineoffset());
  }
  else {
    if( defcount < 0 || defcount > MAX_DEF_LIST_LENGTH )
      throw parse_error(TO_MANY_DEF_IN_MODULE, linenum(), lineoffset());
  }

  i = defcount;
  return *this;
}

parser& parser::get_usecount(int& i) {
  int usecount;
  if( !(*this >> usecount) )
    throw parse_error(NUM_EXPECTED, linenum(), lineoffset());
  if( usecount < 0 || usecount > MAX_USE_LIST_LENGTH )
      throw parse_error(TO_MANY_USE_IN_MODULE, linenum(), lineoffset());

  i = usecount;
  return *this;
}

parser& parser::get_codecount(int& i) {
  int codecount;
  if( !(*this >> codecount) )
    throw parse_error(NUM_EXPECTED, linenum(), lineoffset());

  i = codecount;
  return *this;
}

parser& parser::get_address(int& i) {
  int addr;
  if( !(*this >> addr) || addr < 0 )
    throw parse_error(NUM_EXPECTED, linenum(), lineoffset());

  i = addr;
  return *this;
}

parser& parser::get_symbol(std::string& str) {
  std::string sym;
  if( !(*this >> sym) )
    throw parse_error( SYM_EXPECTED, linenum(), lineoffset() );
  else if( VALID != valid_sym(sym) )
    throw parse_error( valid_sym(sym), linenum(), lineoffset() );

  str = sym;
  return *this;
}

parser& parser::get_instruction_type(char &c) {
  std::string instr;
  if( !(*this >> instr) || instr.length() != 1 )
    // for the lack of better error code...
    throw parse_error(ADDR_EXPECTED, linenum(), lineoffset());
  if( VALID != valid_instr_type(instr[0]) )
    throw parse_error(ADDR_EXPECTED, linenum(), lineoffset());
  c = instr[0];
  return *this;
}

parser& parser::get_instruction(int &i) {
  int instr;
  if( !(*this >> instr) )
    throw parse_error(NUM_EXPECTED, linenum(), lineoffset());
  i = instr;
  return *this;
}

void parser::reset() {
  infile.clear();
  infile.seekg(0, infile.beg);
  linestream.clear();
  wordstream.clear();
  line_num = 0;
  word_length = 0;
}

//---------------------------------------------------------

parse_error_code valid_sym( std::string const & sym ) {
  if( sym.length() > MAX_SYM_LENGTH ) return SYM_TOLONG;
  if( sym.length() < 1 ) return SYM_EXPECTED;
  if( !isalpha(sym[0]) ) return SYM_EXPECTED;
  for( int i = 1; i < sym.length(); ++i )
    if( !isalnum(sym[i]) ) return SYM_EXPECTED;
  return VALID;
}

parse_error_code valid_instr_type( char c ) {
  switch( c ) {
    case 'I':  case 'i':
    case 'A':  case 'a':
    case 'R':  case 'r':
    case 'E':  case 'e':  return VALID;
    default:              return SYM_EXPECTED;
  }
}
