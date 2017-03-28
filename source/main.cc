#include<fstream>

//#include"config_parse.hh"

#include<regex>
#include<string>
#include<iostream>

using namespace std;

int main(){
/*  configparse cp;
  fstream file( "data/config.ini" );

  cp.add_element( "CPU", "ports" );
  cp.add_element( "CPU", "timers" );
  cp.add_element( "CPU", "adcs" );

  file = cp.parse_config( file );*/
  string text( "[aoeu]\n\ta = 1\n\tb = 2\n\tstuff = things\n[htns]\n\tval = text\n" );
  regex rexp1( "\\[(\\w+)\\]((?:\\s*\\w+\\s*=\\s*\\w+\\s*)+)" );
  regex rexp2( "\\s*(\\w+)\\s*=\\s*(\\w+)\\s*" );
  smatch matches1;
  smatch matches2;

  while( regex_search( text, matches1, rexp1 ) ){
    for( unsigned int i = 1; i < matches1.size(); ){
      string match = matches1[i++];
      string values = matches1[i++];

      cout << "TAG" << endl;
      cout << match << endl;
      cout << "VALS" << endl;
      cout << values << endl;

      while( regex_search( values, matches2, rexp2 ) ){
        for( unsigned int j = 0; j < matches2.size(); ++j ){
          cout << j << endl;
          cout << '\t' << matches2[j] << endl;
        }
        values = matches2.suffix().str();
      }
    }
    text = matches1.suffix().str();
  }

  return 0;
}

