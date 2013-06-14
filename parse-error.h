#ifndef __PARSE_ERROR_H__
#define __PARSE_ERROR_H__

#include <stdexcept>
#include <sstream>

enum parse_error_code {
  NUM_EXPECTED,
  SYM_EXPECTED,
  ADDR_EXPECTED,
  SYM_TOLONG,
  TO_MANY_DEF_IN_MODULE,
  TO_MANY_USE_IN_MODULE,
  TO_MANY_SYMBOLS,
  TO_MANY_INSTR
};

struct parse_error : public std::runtime_error
{
  parse_error(parse_error_code, int linenum, int lineoffset);
  static std::string parse_error_message(parse_error_code, int linenum, int lineoffset);

private:
  parse_error_code errcode;
  int linenum, lineoffset;
};

#endif //__PARSE_ERROR_H__
