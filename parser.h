#ifndef __PARSER_H__
#define __PARSER_H__

#include <istream>

struct parser {

  parser(std::istream &);

  std::istream const& input_stream() const;

  int get_int();
  char get_char();
  std::string get_string();

  int get_defcount();
  int get_usecount();
  int get_codecount();

  std::string get_symbol();
  int get_relative_addr();
  char get_instr_type();
  int get_instr();

private:
  std::istream &in;
};

#endif//__PARSER_H__
