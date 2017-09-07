#ifndef __XML_PARSE__
#define __XML_PARSE__

#include<map>
#include<set>
#include<string>
#include<exception>

#include<xml_token_type.hh>

struct item{
  std::map<std::string, item> mItems;
  std::map<std::string, std::string> mProps;
};

struct type{
  std::set<std::string> mItems;
  std::set<std::string> mProps;
};

class undefined_type : public std::out_of_range{
public:
  undefined_type( const std::string& name ):
    std::out_of_range( std::string( "Undefined type found: " ) + name ){
  }
};

class xml_parse{
private:
  item mItem;
  std::map<std::string, type> mTypes;
  std::vector<std::pair<XML_TOKEN, std::string> > mTokens;
  decltype( mTokens )::iterator mCurTok;

  void parse_property( const type& typ, item& itm );

  void handle_open_bracket();
  void handle_tag();

public:
  template<typename inputIter>
  void read( inputIter first, inputIter last ){
    mTokens.insert( mTokens.end(), first, last );
    mCurTok = mTokens.begin();
  }

  void add_structure( const std::string& name, const std::string& itm,
                                               const std::string& prp );
  void add_structure( const std::string& name, const std::string& prp ){
    add_structure( name, "", prp );
  }

  item get_structure(){
    return mItem;
  }

  void parse_xml();
};

#endif

