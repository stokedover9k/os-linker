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

size_t parser::get_line_num() const { return line_num; }
size_t parser::get_char_offset() const { return char_offset; }
