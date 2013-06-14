#ifndef __PARSER_H__
#define __PARSER_H__

#include <iostream>
#include <sstream>
#include <string>

struct parser
{
  parser( std::istream & infile );

  parser& operator>>( std::string &str );
  parser& operator>>( int &i );
  parser& operator>>( char &c );

  explicit operator bool() const;
  bool eof() const;

  size_t get_line_num() const;
  size_t get_char_offset() const;

private:
  std::istream &infile;
  std::stringstream linestream, wordstream;
  size_t line_num, char_offset, word_length;

  void load_line();
  void load_word();

  parser& get_string(std::string &);

  /*
  parser( std::istream & _infile );

  explicit operator bool() const;
  bool eof() const;
  size_t get_line_num() const;
  size_t get_char_offset() const;

  parser& get_string( std::string &str );
  parser& operator>>( std::string &str );
  parser& operator>>( int &i );
  parser& operator>>( char &c );

private:
  void load_line();
  void load_word();
  std::string read_word();

  size_t line_num, char_offset;
  std::string line, word;
  std::istream &infile;
  std::istringstream linestream, wordstream;

  bool line_good, word_good;
  */
};

#endif //__PARSER_H__
