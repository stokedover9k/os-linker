#ifndef __PARSER_H__
#define __PARSER_H__

#define MACHINE_SIZE (512)
#define MAX_SYM_LENGTH (32)
#define MAX_DEF_LIST_LENGTH (32)
#define MAX_USE_LIST_LENGTH (32)

#include <iostream>
#include <sstream>
#include <string>
#include <cctype>
#include <cstdlib>
#include "parse-error.h"

struct parser
{
  parser( std::istream & infile );

  parser& get_defcount(int& i);
  parser& get_usecount(int& i);
  parser& get_codecount(int& i);
  parser& get_address(int& i);
  parser& get_symbol(std::string& s);
  parser& get_instruction_type(char& c);
  parser& get_instruction(int& i);

  parser& operator>>( std::string &str );
  parser& operator>>( int &i );
  parser& operator>>( char &c );

  explicit operator bool() const;
  bool eof() const;

  size_t linenum() const;
  size_t lineoffset() const;

  void reset();

private:
  std::istream &infile;
  std::stringstream linestream, wordstream;
  size_t line_num, char_offset, word_length;

  void load_line();
  void load_word();

  parser& get_string(std::string &);
};

parse_error_code valid_sym( std::string const & sym );
parse_error_code valid_instr_type( char );

#endif //__PARSER_H__
