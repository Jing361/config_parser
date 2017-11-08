#include<fstream>
#include<regex>

#include"config_parse.hh"

using namespace std;

element_not_found::element_not_found( const string& name, const string& value ):
  mMsg( string( "Element " ) + name + " not found.  Expected assignment: " + value ){
}

const char* element_not_found::what() const noexcept{
  return mMsg.c_str();
}

void config_parse::add_element( const string& arg, const string& eleName ){
  mElements[arg][eleName] = string();
}

void config_parse::parse_config( const string& fileName ){
  regex rexp1( "\\[(\\w+)\\]((?:\\s*\\w+\\s*=\\s*\\w+\\s*)+)" );
  regex rexp2( "\\s*(\\w+)\\s*=\\s*(\\w+)\\s*" );
  fstream file( fileName );
  string text;
  smatch matches1;
  smatch matches2;

  file.seekg( 0, ios::end );
  text.reserve( file.tellg() );
  file.seekg( 0, ios::beg );

  text.assign( istreambuf_iterator<char>( file ),
               istreambuf_iterator<char>() );

  while( regex_search( text, matches1, rexp1 ) ){
    for( unsigned int i = 1; i < matches1.size(); ){
      string tag = matches1[i++];
      string values = matches1[i++];

      while( regex_search( values, matches2, rexp2 ) ){
        unsigned int j = 1;
        while( j < matches2.size() ){
          try{
            mElements.at( tag ).at( matches2[j] ) = matches2[j + 1];
          } catch( out_of_range& ){
            throw element_not_found( matches2[j], matches2[j + 1] );
          }

          j += 3;
        }

        values = matches2.suffix().str();
      }
    }

    text = matches1.suffix().str();
  }
}

template<>
bool config_parse::get_element<bool>( const std::string& tag, const std::string& name ){
  std::string text;

  try{
    text = mElements.at( tag ).at( name );
  } catch( std::out_of_range& ){
    throw element_not_found( tag, name );
  }

  return ( text != "" ) && ( text != "false" );
}

