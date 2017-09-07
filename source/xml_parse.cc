#include<iostream>
#include<fstream>
#include<regex>

#include"xml_parse.hh"

using namespace std;

typedef enum{
  TOTAL_RESULT_IDX,
  PROP_TAG_IDX,
  TYPE_IDX,
  TRAIT_IDX,
  QUOTE_IDX,
  TRAIT_VALUE_IDX,
  CONTENT_IDX,
} DATA_INDEX;

void xml_parse::add_structure( const string& name, const string& itm, const string& prp ){
  type& t = mTypes[name];

  if( itm != "" ){
    t.mItems.insert( itm );
  }
  if( prp != "" ){
    t.mProps.insert( prp );
  }
}

void xml_parse::parse_xml(){
  bool isEof = false;

  while( !isEof ){
    switch( mCurTok->first ){
    case XML_TOKEN::OPEN_BRACKET:
      handle_open_bracket();
    break;

    case XML_TOKEN::ONE_LINE_BRACKET:
      handle_tag();
    break;

    default:
      cout << "parse wat" << endl;
    break;
    }
  }
}

void xml_parse::handle_open_bracket(){
  item itm;
}

void xml_parse::handle_tag(){
  item itm;

  // skip < token
  ++mCurTok;
  // read tag name
  auto type_data = mTypes.at( mCurTok->second );
  ++mCurTok;

  switch( mCurTok->first ){
  // for a word or single line, parse properties, then jump end bracket
  //  if just end bracket, skip.
  case XML_TOKEN::WORD:
  case XML_TOKEN::ONE_LINE_BRACKET:
    parse_property( type_data, itm );
  case XML_TOKEN::END_BRACKET:
    ++mCurTok;
  break;

  default:
    cout << "handle wat" << endl;
  break;
  }
}

void xml_parse::parse_property( const type& typ, item& itm ){
  while( mCurTok->first != XML_TOKEN::END_BRACKET ){
    string prop = ( mCurTok++ )->second;

    /*! @todo add exception safety */
    if( ( typ.mProps.count( prop ) ) &&
        ( ( mCurTok++ )->second == "=" ) &&
        ( ( mCurTok->first == XML_TOKEN::WORD ) ||
          ( mCurTok->first == XML_TOKEN::STRING ) ) ){
      itm.mProps[prop] = mCurTok->second;
      ++mCurTok;
    }
  }
}

