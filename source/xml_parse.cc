#include<fstream>
#include<regex>

#include"xml_parse.hh"

using namespace std;

void xml_parse::findObjs( string& text, item& it ){
  regex rexp( "<\\s*(\\w+)(\\s+(\\w+)\\s*=\\s*(\"?)(\\w+)\\4\\s*)+>((?:\\s*([^\\\\\\1]*)\\s*)+)<\\s*\\\\\\1\\s*>|<\\/(\\w+)\\s+(\\w+)\\s*=\\s*(\\w+)\\s*>" );
  smatch matches;

  while( regex_search( text, matches, rexp ) ){
    if( string( matches[0] )[1] == '/' ){
      bool found = false;

      try{
        found = mTypes.at( matches[8] ).mProps.count( matches[9] ) == 1;
      } catch( out_of_range& ) {
        throw undefined_type( matches[8] );
      }

      if( found ){
        it.mProps[matches[8]] = matches[10];
      }
    } else {
      string data( matches[6] );
      findObjs( data, it.mItems[matches[5]] );
    }

    text = matches.suffix().str();
  }
}

void xml_parse::parse_xml( const std::string& fileName ){
  fstream file( fileName );
  string text;

  file.seekg( 0, ios::end );
  text.reserve( file.tellg() );
  file.seekg( 0, ios::beg );

  text.assign( istreambuf_iterator<char>( file ),
               istreambuf_iterator<char>() );

  findObjs( text, mItem );
}

