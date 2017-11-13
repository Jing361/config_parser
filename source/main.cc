#include<string>
#include<iostream>
#include<vector>
#include<array>
#include<algorithm>

#define CATCH_CONFIG_MAIN
#include<catch.hpp>

#include<config_parser.hh>
#include<xml_parser.hh>
#include<xml_lexer.hh>
#include<xml_token_type.hh>

using namespace std;
using namespace gsw;

TEST_CASE( "basics", "[config]" ){
  config_parser cp;
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
  } catch( element_not_found& ){
    test = true;
  }
  REQUIRE( test );

  test = false;
  try{
    !cp.get_element<bool>( "htns", "nope" );
  } catch( element_not_found& ){
    test = true;
  }
  REQUIRE( test );
}

TEST_CASE( "XML", "[xml]" ){
  xml_parser xp;
  xml_lexer xl;
  string cpuname( "CPU" );
  string rname( "RES" );
  string xml;
  fstream f_xml( "data/config.xml" );

  f_xml.seekg( 0, ios::end );
  xml.reserve( f_xml.tellg() );
  f_xml.seekg( 0, ios::beg );

  xml.assign( istreambuf_iterator<char>( f_xml ),
              istreambuf_iterator<char>() );

  xl.lex( xml );
  xl.finalize();

  xp.add_structure( cpuname, "name" );
  xp.add_structure( cpuname, "port" );
  xp.add_structure( cpuname, "timer" );
  xp.add_structure( cpuname, "adc" );
  xp.add_structure( cpuname, "RES" );
  xp.add_structure( rname, "value" );
  xp.add_structure( rname, "name" );
  xp.add_structure( "port",  "name" );
  xp.add_structure( "adc",   "name" );
  xp.add_structure( "timer", "name" );

  xp.parse_xml( xl.begin(), xl.end() );
  auto itm = xp.get_structure();

  REQUIRE( itm.sub_items.count( "CPU" ) == 2 );
  auto sub0_itm1 = itm.sub_items.lower_bound( "CPU" )->second;
  REQUIRE( sub0_itm1.attributes["name"] == "CPU1" );

  REQUIRE( sub0_itm1.sub_items.count( "RES" ) == 2 );
  auto sub2_itm1 = sub0_itm1.sub_items.lower_bound( "RES" )->second;
  REQUIRE( sub2_itm1.attributes["name"] == "R1" );
  REQUIRE( sub2_itm1.attributes["value"] == "1" );

  auto sub2_itm2 = sub0_itm1.sub_items.lower_bound( "port" )->second;
  REQUIRE( sub2_itm2.attributes["name"] == "A" );
  auto sub2_itm3 = sub0_itm1.sub_items.lower_bound( "adc" )->second;
  REQUIRE( sub2_itm3.attributes["name"] == "1" );
  auto sub2_itm4 = sub0_itm1.sub_items.lower_bound( "timer" )->second;
  REQUIRE( sub2_itm4.attributes["name"] == "1" );

  auto sub0_itm2 = (++( itm.sub_items.lower_bound( "CPU" ) ) )->second;
  REQUIRE( sub0_itm2.attributes["name"] == "CPU2" );
  auto sub0_itm3 = sub0_itm2.sub_items.lower_bound( "port" )->second;
  REQUIRE( sub0_itm3.attributes["name"] == "C" );
}

TEST_CASE( "Lexer detects tokens correctly", "[xml][lexer]" ){
  xml_lexer lex;
  string test( " <</= \"asdf\">/> <>" );
  string text;

  lex.lex( test );
  lex.finalize();

  auto it = lex.begin();
  text += it->second;
  REQUIRE( ( it++ )->first == XML_TOKEN::OPEN_BRACKET );

  text += it->second;
  REQUIRE( ( it++ )->first == XML_TOKEN::CLOSE_BRACKET );

  text += it->second;
  REQUIRE( ( it++ )->first == XML_TOKEN::SYMBOL );

  text += it->second;
  REQUIRE( ( it )->first == XML_TOKEN::STRING );

  REQUIRE( ( it++ )->second == "asdf" );
  text += it->second;
  REQUIRE( ( it++ )->first == XML_TOKEN::END_BRACKET );

  text += it->second;
  REQUIRE( ( it++ )->first == XML_TOKEN::ONE_LINE_CLOSE );

  text += it->second;
  REQUIRE( ( it++ )->first == XML_TOKEN::OPEN_BRACKET );

  text += it->second;
  REQUIRE( ( it++ )->first == XML_TOKEN::END_BRACKET );

  REQUIRE( text == "<</=asdf>/><>" );
}

TEST_CASE( "Attributes parsed correctly", "[xml][parser]" ){
  xml_parser xp;
  xml_lexer xl;

  string xml1( "<CPU name = \"foo\" color = green>\n</CPU>" );
  string xml2( "<CPU>\n<name>\"bar\"</name>\n<color>orange</color>\n</CPU>" );

  xp.add_structure( "CPU", "name" );
  xp.add_structure( "CPU", "color" );

  SECTION( "Attributes inline" ){
    xl.lex( xml1 );
    xl.finalize();

    xp.read( xl.begin(), xl.end() );
    xp.parse_xml();

    item itm = xp.get_structure().sub_items.lower_bound( "CPU" )->second;

    REQUIRE( itm.attributes["name"] == "foo" );
    REQUIRE( itm.attributes["color"] == "green" );
  }

  SECTION( "Attributes separate" ){
    xl.lex( xml2 );
    xl.finalize();

    xp.read( xl.begin(), xl.end() );
    xp.parse_xml();

    item itm = xp.get_structure().sub_items.lower_bound( "CPU" )->second;

    REQUIRE( itm.sub_items.lower_bound( "name"  )->second.data == "bar" );
    REQUIRE( itm.sub_items.lower_bound( "color" )->second.data == "orange" );
  }
}

