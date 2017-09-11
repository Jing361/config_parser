#ifndef __XML_PARSE_HH__
#define __XML_PARSE_HH__

#include<map>
#include<set>
#include<string>
#include<vector>
#include<exception>

#include<xml_token_type.hh>

struct item{
  std::multimap<std::string, item> mSubItems;
  std::map<std::string, std::string> mAttributes;
};

using type = std::set<std::string>;

class undefined_type : public std::out_of_range{
public:
  undefined_type( const std::string& name ):
    std::out_of_range( std::string( "Undefined type found: " ) + name ){
  }
};

class xml_parse{
private:
  item mItem;
  std::map<std::string, type> mTypeDict;
  std::vector<std::pair<XML_TOKEN, std::string> > mTokens;
  decltype( mTokens )::iterator mCurTok;

  void handle_tag( item& itm );

  void parse_elements( const type& typ, item& itm );
  void parse_attributes( const type& typ, item& itm );

public:
  template<typename inputIter>
  void read( inputIter first, inputIter last ){
    mTokens.insert( mTokens.end(), first, last );
    mCurTok = mTokens.begin();
  }

  void add_structure( const std::string& name, const std::string& sub );

  item get_structure(){
    return mItem;
  }

  void parse_xml();

  template<typename inputIter>
  void parse_xml( inputIter first, inputIter last ){
    read( first, last );

    parse_xml();
  }
};

#endif

