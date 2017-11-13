#ifndef __XML_LEXER_HH__
#define __XML_LEXER_HH__

#include<string>
#include<vector>
#include<utility>
#include<stdexcept>

#include"xml_token_type.hh"

namespace gsw{

class EOF_exception : public std::domain_error{
public:
  EOF_exception():
    std::domain_error( "No end of file at end of token stream." ){
  }
};

class xml_lexer{
private:
  std::vector<std::pair<XML_TOKEN, std::string> > mTokens;

public:
  /*! Lex the text provided
   */
  void lex( const std::string& text );

  /*! Add final token to token stream
   *
   * Adds an EOF-like token to indicate the end of the token stream
   */
  void finalize();

  /*! Get iterator to first stored token
   */
  decltype( mTokens )::iterator begin();

  /*!
   */
  decltype( mTokens )::iterator end();
};

}

#endif

