#ifndef __XML_PARSE__
#define __XML_PARSE__

#include<map>
#include<set>
#include<string>
#include<exception>

struct item{
  std::map<std::string, item> mItems;
  std::map<std::string, std::string> mProps;
};

struct type{
  std::set<std::string> mItems;
  std::set<std::string> mProps;
};

class undefined_type : public std::exception{
private:
  std::string mMsg;

public:
  undefined_type( const std::string& name ):
    mMsg( std::string( "Undefined type found: " ) + name ){
  }

  const char* what() const noexcept override{
    return mMsg.c_str();
  }
};

class xml_parse{
private:
  item mItem;
  std::map<std::string, type> mTypes;

  void findObjs( std::string& text, item& it );

public:
  void add_structure( const std::string& name, const std::string& itm,
                                               const std::string& prp );

  item get_structure(){
    return mItem;
  }

  void parse_xml( const std::string& fileName );
};

#endif
