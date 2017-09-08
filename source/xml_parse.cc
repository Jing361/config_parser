#include<iostream>

#include"xml_parse.hh"

using namespace std;

void xml_parse::add_structure( const std::string& name, const std::string& sub ){
  mTypeDict[name].insert( sub );
}

void xml_parse::parse_xml(){
  bool isEof = false;
  item itm;

  while( !isEof ){
    switch( mCurTok->first ){
    case XML_TOKEN::OPEN_BRACKET:
      handle_tag( itm );
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

void xml_parse::handle_tag( item& itm){
  string tag;

  // skip < token
  ++mCurTok;
  tag = mCurTok->second;
  // read tag name
  auto type_data = mTypeDict.at( tag );
  ++mCurTok;

  parse_attributes( type_data, itm );
  // skip >
  ++mCurTok;
  parse_elements( type_data, itm );

  while( mCurTok->first != XML_TOKEN::CLOSE_BRACKET ){
    handle_tag( itm );
  }
  ++mCurTok;
  if( ( mCurTok++ )->second == tag &&
      ( mCurTok++ )->first == XML_TOKEN::END_BRACKET ){
  } else {
    //error!
  }
}

void xml_parse::parse_elements( const type& typ, item& itm ){
  
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

