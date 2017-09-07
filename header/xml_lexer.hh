#ifndef __XML_LEXER_HH__
#define __XML_LEXER_HH__

#include<string>
#include<vector>
#include<utility>

#include"xml_token_type.hh"

class xml_lexer{
private:
  std::vector<std::pair<XML_TOKEN, std::string> > mTokens;

public:
  void lex( const std::string& text );

  decltype( mTokens )::iterator begin();
  decltype( mTokens )::iterator end();
};

#endif

