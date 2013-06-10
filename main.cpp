#include <iostream>
#include <fstream>

#include "main.h"
#include "parser.h"

//============ main ===================//
int main( int argc, char *argv[] ) {   //
//=====================================//
  using namespace std;
  cout << "starting..." << endl;

  ifstream infile;
  if( argc > 1 ) {
    infile.open(argv[1]);
    if( !infile ) {
      cerr << "Error: failed to open input file: " << argv[1] 
           << ".\nTerminating...";
      return 1;
    }
  }
  else {
    cerr << "Error: no input file given.\nTerminating..." << endl;
  }

  parser Parser = parser(infile);

  while( Parser.input_stream() ) {
    string s = Parser.get_string();
    cout << s << endl;
  }

  cout << "complete..." << endl;
  return 0;
}
