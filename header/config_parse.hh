#ifndef __CONFIG_PARSE_HH__
#define __CONFIG_PARSE_HH__

#include<string>
#include<map>
#include<exception>

class element_not_found : public std::exception{
private:
  std::string mMsg;

public:
  element_not_found( const std::string& name, const std::string& value );

  const char* what() const noexcept override;
};

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

