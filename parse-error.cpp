#include "parse-error.h"

//============ parse_error functions ===============//

std::string parse_error::parse_error_message(
  parse_error_code _errcode,
  int _linenum,
  int _lineoffset)
{
  static const char* errstr[] = {
    "NUM_EXPECTED",
    "SYM_EXPECTED",
    "ADDR_EXPECTED",
    "SYM_TOLONG",
    "TO_MANY_DEF_IN_MODULE",
    "TO_MANY_USE_IN_MODULE",
    "TO_MANY_SYMBOLS",
    "TO_MANY_INSTR"
  };

  std::stringstream ss(std::ios_base::out);
  ss << "Parse Error" 
     << " line_" << _linenum
     << " offset " << _lineoffset
     << ": " << errstr[_errcode];
  return ss.str();
}

parse_error::parse_error(
  parse_error_code _errcode,
  int _linenum,
  int _lineoffset)
  : 
  runtime_error( parse_error_message(_errcode, _linenum, _lineoffset) ),
  errcode(_errcode),
  linenum(_linenum),
  lineoffset(_lineoffset)
{ }
