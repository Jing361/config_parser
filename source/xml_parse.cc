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
      handle_tag( mItem );
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

  if( mCurTok->first != XML_TOKEN::OPEN_BRACKET ){
    while( mCurTok->first != XML_TOKEN::CLOSE_BRACKET ){
      itm.data += mCurTok->second;
    }
    return;
  }
  // skip <
  ++mCurTok;

  // read tag data
  tag = ( mCurTok++ )->second;
  auto type_data = mTypeDict.at( tag );

  parse_attributes( type_data, itm );

  // skip >
  if( ( mCurTok++ )->first != XML_TOKEN::ONE_LINE_CLOSE ){
    while( mCurTok->first != XML_TOKEN::CLOSE_BRACKET ){
      handle_tag( itm );
    }

    // skip </
    ++mCurTok;

    // check tag is correct, and is closed correctly
    string close_tag = ( mCurTok++ )->second;
    if( close_tag == tag &&
        ( mCurTok++ )->first == XML_TOKEN::END_BRACKET ){
    } else {
      throw parsing_error( string( "Closing tag:\t" ) + close_tag + "\nDoes not close current tag:\t" + tag );
    }
  }
}

void xml_parse::parse_attributes( const type& typ, item& itm ){
  while( mCurTok->first != XML_TOKEN::END_BRACKET &&
         mCurTok->first != XML_TOKEN::ONE_LINE_CLOSE ){
    string attr = ( mCurTok++ )->second;

    if( !typ.count( attr ) ){
      throw undefined_attribute( attr );
    }

    /*! @todo add exception safety */
    if( ( typ.count( attr ) ) &&
        ( ( mCurTok++ )->second == "=" ) &&
        ( ( mCurTok->first == XML_TOKEN::WORD ) ||
          ( mCurTok->first == XML_TOKEN::STRING ) ) ){
      itm.attributes.insert( {attr, mCurTok->second} );
      ++mCurTok;
    }
  }
}

