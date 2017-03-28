#ifndef __CONFIG_PARSE_HH__
#define __CONFIG_PARSE_HH__

#include<string>
#include<map>
#include<fstream>
#include<regex>

class config_parse{
private:
  typedef std::map<std::string, std::string> element;

  std::map<std::string, element> mElements;

public:
  void add_element( const std::string& arg, const std::string& eleName ){
    mElements[arg] = element();
    mElements[arg][eleName] = std::string();
  }

  template<typename T>
  T get_element( const std::string& tag, const std::string& name ){
    return T( mElements[tag][name] );
  }

  void parse_config( const std::string& fileName ){
    std::regex rexp1( "\\[(\\w+)\\]((?:\\s*\\w+\\s*=\\s*\\w+\\s*)+)" );
    std::regex rexp2( "\\s*(\\w+)\\s*=\\s*(\\w+)\\s*" );
    std::fstream file( fileName );
    std::string text;
    std::smatch matches1;
    std::smatch matches2;

    file.seekg( 0, std::ios::end );
    text.reserve( file.tellg() );
    file.seekg( 0, std::ios::beg );

    text.assign( std::istreambuf_iterator<char>( file ),
                 std::istreambuf_iterator<char>() );

    while( regex_search( text, matches1, rexp1 ) ){
      for( unsigned int i = 1; i < matches1.size(); ){
        std::string tag = matches1[i++];
        std::string values = matches1[i++];

        while( std::regex_search( values, matches2, rexp2 ) ){
          unsigned int j = 1;
          while( j < matches2.size() ){
            mElements[tag][matches2[j]] = matches2[j + 1];
            j += 3;
          }

          values = matches2.suffix().str();
        }
      }

      text = matches1.suffix().str();
    }
  }
};

#endif

