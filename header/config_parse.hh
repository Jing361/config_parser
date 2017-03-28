#ifndef __CONFIG_PARSE_HH__
#define __CONFIG_PARSE_HH__

#include<string>
#include<map>
class config_parse{
private:
  typedef std::map<std::string, std::string> element;

  std::map<std::string, element> mElements;

public:
  void add_element( const std::string& arg, const std::string& eleName );

  template<typename T>
  T get_element( const std::string& tag, const std::string& name ){
    return T( mElements[tag][name] );
  }

  void parse_config( const std::string& fileName );
};

#endif

