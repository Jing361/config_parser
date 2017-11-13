#include<iostream>

#include"xml_parser.hh"

using namespace std;
using namespace gsw;

void xml_parser::validate_structure( const std::string& str, const item& itm ){
  for( auto i : itm.sub_items ){
    if( mTypeDict.at( str ).count( i.first ) == 0 ){
      // differentiate between str fail and i.first fail
      // should it be called undefined attribute? or sub_item?
      throw undefined_attribute( i.first );
    }

    validate_structure( i.first, i.second );
  }

  for( auto i : itm.attributes ){
    if( mTypeDict.at( str ).count( i.first ) == 0 ){
      throw undefined_attribute( i.first );
    }
  }
}

void xml_parser::add_structure( const std::string& name, const std::string& sub ){
  mTypeDict[name].insert( sub );
}

void xml_parser::parse_xml(){
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
  for( auto i : mItem.sub_items ){
    validate_structure( i.first, i.second );
  }
}

void xml_parser::handle_tag( item& itm ){
  if( mCurTok->first != XML_TOKEN::OPEN_BRACKET ){
    while( mCurTok->first != XML_TOKEN::CLOSE_BRACKET ){
      itm.data += mCurTok->second + " ";
      ++mCurTok;
    }
    // remove trailing space
    itm.data.pop_back();
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

void xml_parser::parse_attributes( item& itm ){
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

