#ifndef __CONFIG_PARSE_HH__
#define __CONFIG_PARSE_HH__

#include<string>
#include<map>
#include<fstream>

class config_parse{
private:
  typedef std::map<std::string, std::string> element;

  std::map<std::string, element> mElements;

public:
  void add_element( const std::string& arg, const std::string& eleName ){
    mElements[arg] = element();
    mElements[arg][eleName] = string();
  }

  fstream&& parse_config( fstraem&& file ){
    return file;
  }
};

#endif

