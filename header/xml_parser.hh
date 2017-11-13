#ifndef __XML_PARSER_HH__
#define __XML_PARSER_HH__

#include<map>
#include<set>
#include<string>
#include<vector>
#include<stdexcept>

#include<xml_token_type.hh>

namespace gsw{

struct item{
  std::multimap<std::string, item> sub_items;
  std::map<std::string, std::string> attributes;
  std::string data;
};

using type = std::set<std::string>;

class undefined_type : public std::out_of_range{
public:
  undefined_type( const std::string& name ):
    std::out_of_range( std::string( "Undefined type found: " ) + name ){
  }
};

class undefined_attribute : public std::out_of_range{
public:
  /* @todo add type name to report string */
  undefined_attribute( const std::string& name ):
    std::out_of_range( std::string( "Undefined attribute found:\t" ) + name ){
  }
};

class parsing_error : public std::runtime_error{
public:
  parsing_error( const std::string& what ):
    runtime_error( what ){
  }
};

class xml_parser{
private:
  item mItem;
  std::map<std::string, type> mTypeDict;
  std::vector<std::pair<XML_TOKEN, std::string> > mTokens;
  decltype( mTokens )::iterator mCurTok;

  void handle_tag( item& itm );

  void parse_attributes( item& itm );

  void validate_structure( const std::string& str, const item& itm );

public:
  /*! Read tokens pointed to between provided iterators
   *
   * @param first Iterator pointing to first element to add to input token stream
   *
   * @param last Iterator past the end of the elements to add to stream
   */
  template<typename inputIter>
  void read( inputIter first, inputIter last ){
    mTokens.insert( mTokens.end(), first, last );
    mCurTok = mTokens.begin();
  }

  /*! Add sub as a structure expected to appear under name
   *
   * @param name
   *
   * @param sub
   */
  void add_structure( const std::string& name, const std::string& sub );

  /*! Get results from parsing the data
   */
  item get_structure(){
    return mItem;
  }

  /*! Parse provided tokens
   */
  void parse_xml();

  /*! Read tokens from given iterators, then parse the data
   *
   * @param first
   *
   * @param last
   */
  template<typename inputIter>
  void parse_xml( inputIter first, inputIter last ){
    read( first, last );

    parse_xml();
  }
};

}

#endif

