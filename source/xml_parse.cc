#include<fstream>
#include<regex>

#include"xml_parse.hh"

#include<iostream>

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

void xml_parse::findObjs( const string& inputText, item& it, const string& typeName ){
  regex rexp( "<(/?)\\s*(\\w+)\\s+(\\w+)\\s*=\\s*(\"?)(\\w+)\\4\\s*>(?:\\s*([\\s\\w<>=\\\\/\"]+)\\s*<\\\\\\2>)?" );
  smatch matches;
  string text = inputText;

  while( regex_search( text, matches, rexp ) ){
int i = 0;
for( auto it : matches ){
  cout << i++ << '\t' << it << endl;
}
    if( string( matches[PROP_TAG_IDX] ) == string( "/" ) && string( matches[CONTENT_IDX] ) == "" ){
cout << "prop" << endl;
      bool validType = false;

      if( typeName == "" ){
        validType = true;
      } else {
        string name( matches[TYPE_IDX] );

        try{
          validType = ( mTypes.at( typeName ).mProps.count( name ) == 1 );
        } catch( out_of_range ) {
          throw undefined_type( name );
        }
      }

      if( validType ){
        it.mProps[matches[TYPE_IDX]] = matches[TRAIT_VALUE_IDX];
      }
    } else if( string( matches[PROP_TAG_IDX] ) == string( "" ) && string( matches[CONTENT_IDX] ) != "" ){
cout << "obj" << endl;
      findObjs( matches[CONTENT_IDX], it.mItems[matches[TRAIT_VALUE_IDX]], matches[TYPE_IDX] );
    } else {
      //TODO throw syntax error
cout << "property tag\t";
cout << string( matches[PROP_TAG_IDX] ) << endl;
cout << "content tag\t";
cout << string( matches[CONTENT_IDX] ) << endl;
cout << "source text\t";
cout << text << endl;
      throw undefined_type( "syntax error!" );
    }

    text = matches.suffix().str();
  }
}

void xml_parse::parse_xml( const string& fileName ){
  fstream file( fileName );
  string text;

  file.seekg( 0, ios::end );
  text.reserve( file.tellg() );
  file.seekg( 0, ios::beg );

  text.assign( istreambuf_iterator<char>( file ),
               istreambuf_iterator<char>() );

  findObjs( text, mItem );
}

