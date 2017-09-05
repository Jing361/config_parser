#include<cctype>
#include<string>

#include"xml_lexer.hh"

using namespace std;

void xml_lexer::lex( const string& text ){
  TOKEN_CLASS cls = TOKEN_CLASS::NONE;

  for( auto it = text.begin(); it != text.end(); ++it ){
    string token;

    if( isspace( *it ) ){
      continue;
    } else if( *it == '<' ) {
      cls = TOKEN_CLASS::OPEN_BRACKET;
      if( *(it + 1) == '\\' ){
        token = "<\\";
        ++it;
      } else {
        token = "<";
      }
    } else if( *it == '/' ) {
      if( *(it + 1) == '>' ){
        cls = TOKEN_CLASS::CLOSE_BRACKET;
        token = "/>";
        ++it;
      } else {
        //error out
      }
    } else if( *it == '>' ) {
      cls = TOKEN_CLASS::END_BRACKET;
      token = ">";
    } else if( *it == '=' || *it == '\"' ) {
      if( *it == '\"' ){
        cls = TOKEN_CLASS::STRING;
        ++it;
        while( *it != '\"' ){
          token += *it;
          ++it;
        }
      } else {
        token = string( {*it} );
        cls = TOKEN_CLASS::SYMBOL;
      }
    } else if( isalnum( *it ) ){
      cls = TOKEN_CLASS::WORD;
      do{
        token+= *it;
        ++it;
      }while( isalnum( *it ) );
    } else {
      //error out
    }

    mTokens.emplace_back( cls, token );
  }
}

decltype( xml_lexer::mTokens )::iterator xml_lexer::begin(){
  return mTokens.begin();
}

decltype( xml_lexer::mTokens )::iterator xml_lexer::end(){
  return mTokens.begin();
}

