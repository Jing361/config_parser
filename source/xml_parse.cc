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
/* @todo add structure checking */
// after the xml tree has been parsed, it should be verified to be consistent
//   with provided specifications
}

void xml_parse::handle_tag( item& itm ){
  if( mCurTok->first != XML_TOKEN::OPEN_BRACKET ){
    while( mCurTok->first != XML_TOKEN::CLOSE_BRACKET ){
      itm.data += mCurTok->second + " ";
      ++mCurTok;
    }
    return;
  }
  // skip <
  ++mCurTok;

  // read tag data
  string tag = ( mCurTok++ )->second;
  item& new_itm = itm.sub_items.insert( {tag, item()} )->second;

  parse_attributes( new_itm );

  // skip >
  if( ( mCurTok++ )->first != XML_TOKEN::ONE_LINE_CLOSE ){
    while( mCurTok->first != XML_TOKEN::CLOSE_BRACKET ){
      handle_tag( new_itm );
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

void xml_parse::parse_attributes( item& itm ){
  while( mCurTok->first != XML_TOKEN::END_BRACKET &&
         mCurTok->first != XML_TOKEN::ONE_LINE_CLOSE ){
    string attr = ( mCurTok++ )->second;

    /*! @todo add exception safety */
    if( ( ( mCurTok++ )->second == "=" ) &&
        ( ( mCurTok->first == XML_TOKEN::WORD ) ||
          ( mCurTok->first == XML_TOKEN::STRING ) ) ){
      itm.attributes.insert( {attr, mCurTok->second} );
      ++mCurTok;
    }
  }
}

