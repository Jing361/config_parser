#include<iostream>

#include"xml_parse.hh"

using namespace std;

void xml_parse::add_structure( const std::string& name, const std::string& sub ){
  mTypeDict[name].insert( sub );
}

void xml_parse::parse_xml(){
  bool isEof = false;

  while( !isEof ){
    switch( mCurTok->first ){
    case XML_TOKEN::OPEN_BRACKET:
      handle_tag();
    break;

    case XML_TOKEN::FINAL:
      isEof = true;
    break;

    default:
      cout << "parse wat" << endl;
    break;
    }
  }
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
    parse_attributes( type_data, itm );
  case XML_TOKEN::END_BRACKET:
    ++mCurTok;
  break;

  default:
    cout << "handle wat" << endl;
  break;
  }
}

void xml_parse::parse_attributes( const type& typ, item& itm ){
  while( mCurTok->first != XML_TOKEN::END_BRACKET ||
         mCurTok->first != XML_TOKEN::ONE_LINE_CLOSE ){
    string attr = ( mCurTok++ )->second;

    /*! @todo add exception safety */
    if( ( typ.count( attr ) ) &&
        ( ( mCurTok++ )->second == "=" ) &&
        ( ( mCurTok->first == XML_TOKEN::WORD ) ||
          ( mCurTok->first == XML_TOKEN::STRING ) ) ){
      itm.mAttributes.insert( {attr, mCurTok->second} );
      ++mCurTok;
    }
  }
}

