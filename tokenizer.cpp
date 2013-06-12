#include "tokenizer.h"

tokenizer::tokenizer( std::istream & _infile ) : 
line_num(0),
infile(_infile),
line_good(true),
word_good(true)
{
  load_line();
}

tokenizer::operator bool() const { return line_good; }

void tokenizer::load_line() {
  line_good = std::getline(infile, line);
  if( line_good ) {
    line_num++;
    char_offset = 0;
  }
  linestream.clear();
  linestream.str(line);
}

void tokenizer::load_word() {
  word = "";  // outer loop for dealing with empty lines
  while( 0 == word.length() && line_good ) {
    if( !word_good )
      load_line();

    char_offset = linestream.tellg();
    while( linestream.peek() == ' ' || linestream.peek() == '\t') {
      char_offset++;
      linestream.get();
    }

    linestream >> word;
    word_good = linestream.good();
  }
}

std::string tokenizer::read_word() {
  return word;
}

tokenizer& tokenizer::get_string( std::string &str ) {
  load_word();
  str = read_word();
  return *this;
}

tokenizer& tokenizer::operator>>( std::string &str ) {
  return get_string(str);
}
tokenizer& tokenizer::operator>>( int &i ) {
  std::string s;
  get_string(s);
  std::istringstream iss(s);
  iss >> i;
  return *this;
}
tokenizer& tokenizer::operator>>( char &c ) {
  std::string s;
  get_string(s);
  std::istringstream iss(s);
  iss >> c;
  return *this;
}

size_t tokenizer::get_line_num() const { return line_num; }
size_t tokenizer::get_char_offset() const { return char_offset + 1; }
