#include<string>
#include<iostream>
#include<vector>
#include<array>
#include<algorithm>

#define CATCH_CONFIG_MAIN
#include<catch.hpp>

#include"config_parse.hh"
#include"xml_parse.hh"
#include"xml_lexer.hh"

using namespace std;

TEST_CASE( "basics", "start" ){
  config_parse cp;
  cp.add_element( "CPU", "ports" );
  cp.add_element( "CPU", "timers" );
  cp.add_element( "CPU", "adcs" );
  cp.add_element( "aoeu", "a" );
  cp.add_element( "aoeu", "b" );
  cp.add_element( "aoeu", "stuff" );
  cp.add_element( "htns", "value" );
  cp.add_element( "htns", "empty" );

  cp.parse_config( "data/config.ini" );

  REQUIRE( cp.get_element<string>( "CPU", "ports" ) == string( "1" ) );
  REQUIRE( cp.get_element<int>( "CPU", "ports" ) == 1 );
  REQUIRE( cp.get_element<bool>( "CPU", "ports" ) );

  REQUIRE( cp.get_element<string>( "CPU", "timers" ) == string( "1" ) );
  REQUIRE( cp.get_element<int>( "CPU", "timers" ) == 1 );
  REQUIRE( cp.get_element<bool>( "CPU", "timers" ) );

  REQUIRE( cp.get_element<string>( "CPU", "adcs" ) == string( "1" ) );
  REQUIRE( cp.get_element<int>( "CPU", "adcs" ) == 1 );
  REQUIRE( cp.get_element<bool>( "CPU", "adcs" ) );


  REQUIRE( cp.get_element<string>( "aoeu", "a" ) == string( "1" ) );
  REQUIRE( cp.get_element<int>( "aoeu", "a" ) == 1 );
  REQUIRE( cp.get_element<bool>( "aoeu", "a" ) );

  REQUIRE( cp.get_element<string>( "aoeu", "b" ) == string( "2" ) );
  REQUIRE( cp.get_element<int>( "aoeu", "b" ) == 2 );
  REQUIRE( cp.get_element<bool>( "aoeu", "b" ) );

  REQUIRE( cp.get_element<string>( "aoeu", "stuff" ) == string( "things" ) );
  REQUIRE( cp.get_element<bool>( "aoeu", "b" ) );

  REQUIRE( cp.get_element<string>( "htns", "value" ) == string( "text" ) );
  REQUIRE( cp.get_element<bool>( "aoeu", "b" ) );

  REQUIRE( cp.get_element<string>( "htns", "empty" ) == string() );
  REQUIRE( !cp.get_element<bool>( "htns", "empty" ) );

  bool test = false;
  try{
    cp.get_element<bool>( "nope", "don't_care" );
  } catch( element_not_found ){
    test = true;
  }
  REQUIRE( test );

  test = false;
  try{
    !cp.get_element<bool>( "htns", "nope" );
  } catch( element_not_found ){
    test = true;
  }
  REQUIRE( test );
}

TEST_CASE( "XML", "xml" ){
  xml_parse xp;
  string cpuname( "CPU" );
  string rname( "RES" );

  xp.add_structure( cpuname, rname, "port" );
  xp.add_structure( cpuname, "timer" );
  xp.add_structure( cpuname, "adc" );
  xp.add_structure( rname, "value" );

  //xp.parse_xml( "data/config.xml" );

  for( auto it : xp.get_structure().mItems ){
    cout << it.first << '\t';
    for( auto jt : it.second.mProps ){
      cout << jt.first << '\t' << jt.second << endl;
    }
  }
}

typedef enum{
  TAG_TOKEN,
  OPEN_BRACKET_TOKEN,
  CLOSED_BRACKET_TOKEN,
  SINGLE_LINE_TOKEN,
  QUOTE_TOKEN,
  EQUALS_TOKEN,
  STRING_TOKEN,
  END_TAG_OPEN_TOKEN,
  MAX_TOKEN,
} TOKEN_t;

TEST_CASE( "TEMP", "testing" ){
  vector<pair<TOKEN_t, string> > tokens;
  array<string, 6> validTokens{ "<", ">", "</", "<\\", "=", "\"" };
  string text( "<AAA name = charles>\n\t<\\BBB things = stuff>\n\t<\\CCC stuff=\"things\">\n</AAA>\n\n<\\DDD text= letters>\n\n<CPU name = \"CPU1\">\n\t<RES name=\"R1\">\n\t\t<\\value val = 1K>\n\t</RES>\n\t<\\port name = \"A\">\n\t<\\timer name =1>\n\t<\\adc name=1>\n</CPU>\n\n<CPU name=\"CPU2\">\n\t<\\port name = \"C\">\n</CPU>\n\n" );

  auto textIter = text.begin();
  while( textIter != text.end() ){
    TOKEN_t token;
    string data;

    if( *textIter == '<' ){
      data += *textIter;
      if( *( textIter + 1 ) == '/' ){
        token = END_TAG_OPEN_TOKEN;
        data += *textIter;
      } else if( *( textIter + 1 ) == '\\' ){
        token = SINGLE_LINE_TOKEN;
        data += *textIter;
      } else {
        token = OPEN_BRACKET_TOKEN;
      }
    } else if( *textIter == '>' ){
      token = CLOSED_BRACKET_TOKEN;
    } else if( *textIter == '=' ){
      token = EQUALS_TOKEN;
    } else if( *textIter == '\"' ){
      token = QUOTE_TOKEN;
    } else if( isalpha( *textIter ) ){
      
    }

    tokens.emplace_back( token, string( textIter, textIter + 1 ) );

    ++textIter;
  }
}

TEST_CASE( "", "" ){
  xml_lexer lex;

  lex.lex( " << <=<\\\"asdf\"/>/>/>" );
  auto it = lex.begin();
  REQUIRE( ( it++ )->first == TOKEN_CLASS::OPEN_BRACKET );
  REQUIRE( ( it++ )->first == TOKEN_CLASS::OPEN_BRACKET );
  REQUIRE( ( it++ )->first == TOKEN_CLASS::OPEN_BRACKET );
  REQUIRE( ( it++ )->first == TOKEN_CLASS::SYMBOL );
  REQUIRE( ( it++ )->first == TOKEN_CLASS::OPEN_BRACKET );
  REQUIRE( ( it )->first == TOKEN_CLASS::STRING );
  REQUIRE( ( it++ )->second == "asdf" );
  REQUIRE( ( it++ )->first == TOKEN_CLASS::CLOSE_BRACKET );
  REQUIRE( ( it++ )->first == TOKEN_CLASS::CLOSE_BRACKET );
  REQUIRE( ( it++ )->first == TOKEN_CLASS::CLOSE_BRACKET );
}

