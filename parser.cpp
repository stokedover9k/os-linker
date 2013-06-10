#include "parser.h"

parser::parser(std::istream &_in) : in(_in) { }

std::istream const& parser::input_stream() const { return in; }

int parser::get_int() {
  int x;  in >> x;  return x; }

char parser::get_char() {
  char c;  in >> c;  return c; }

std::string parser::get_string() {
  std::string s;  in >> s;  return s; }

int parser::get_defcount()  { return get_int(); }
int parser::get_usecount()  { return get_int(); }
int parser::get_codecount() { return get_int(); }

std::string parser::get_symbol()        { return get_string(); }
int         parser::get_relative_addr() { return get_int(); }
char        parser::get_instr_type()    { return get_char(); }
int         parser::get_instr()         { return get_int(); }
