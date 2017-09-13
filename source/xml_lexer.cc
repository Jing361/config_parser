#include<cctype>
#include<string>
#include<stdexcept>

#include"xml_lexer.hh"

using namespace std;

class undefined_token : public runtime_error{
public:
  undefined_token( const string& str ):
    runtime_error( str ){
  }
};

void xml_lexer::lex( const string& text ){
  for( auto it = text.begin(); it != text.end(); ){
    string token;
    XML_TOKEN cls = XML_TOKEN::NONE;

    if( isspace( *it ) ){
      ++it;
      continue;
    } else if( *it == '<' ){
      if( *( it + 1 ) == '/' ){
        cls = XML_TOKEN::CLOSE_BRACKET;
        token = "</";
        ++it;
      } else {
        cls = XML_TOKEN::OPEN_BRACKET;
        token = "<";
      }
      ++it;
    } else if( *it == '/' ){
      if( *( it + 1 ) == '>' ){
        cls = XML_TOKEN::ONE_LINE_CLOSE;
        token = "/>";
        ++it;
      } else {
        throw undefined_token( "Found '/' with no '>'" );
      }
      ++it;
    } else if( *it == '>' ){
      cls = XML_TOKEN::END_BRACKET;
      token = ">";
      ++it;
    } else if( *it == '=' || *it == '\"' ){
      if( *it == '\"' ){
        cls = XML_TOKEN::STRING;
        ++it;
        while( *it != '\"' ){
          token += *it;
          ++it;
        }
      } else {
        token = string( {*it} );
        cls = XML_TOKEN::SYMBOL;
      }
      // eat symbol, or closing quote
      ++it;
    } else if( isalnum( *it ) ){
      cls = XML_TOKEN::WORD;
      do{
        token+= *it;
        ++it;
      }while( isalnum( *it ) );
    } else {
      //error out
      throw undefined_token( string( "Found undefined token:\t" ) + *it );
    }

    mTokens.emplace_back( cls, token );
  }
}

void xml_lexer::finalize(){
  mTokens.emplace_back( XML_TOKEN::FINAL, "" );
}

decltype( xml_lexer::mTokens )::iterator xml_lexer::begin(){
  return mTokens.begin();
}

decltype( xml_lexer::mTokens )::iterator xml_lexer::end(){
  if( mTokens.back().first != XML_TOKEN::FINAL ){
    throw EOF_exception();
  }
  return mTokens.end();
}

