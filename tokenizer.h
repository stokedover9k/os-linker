#ifndef __TOKENIZER_H__
#define __TOKENIZER_H__

#include <iostream>
#include <sstream>
#include <string>

struct tokenizer
{
  tokenizer( std::istream & _infile );

  explicit operator bool() const;

  tokenizer& get_string( std::string &str );
  tokenizer& operator>>( std::string &str );
  tokenizer& operator>>( int &i );
  tokenizer& operator>>( char &c );

  size_t get_line_num() const;
  size_t get_char_offset() const;

private:
  void load_line();
  void load_word();
  std::string read_word();

  size_t line_num, char_offset;
  std::string line, word;
  std::istream &infile;
  std::istringstream linestream;

  bool line_good, word_good;
};

#endif //__TOKENIZER_H__
