#include<string>
#include<iostream>
#include<vector>

#define CATCH_CONFIG_MAIN
#include<catch.hpp>

#include"config_parse.hh"
#include"xml_parse.hh"

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
  xp.add_structure( cpuname, "", "timer" );
  xp.add_structure( cpuname, "", "adc" );
  xp.add_structure( rname, "", "value" );

  xp.parse_xml( "data/config.xml" );

  for( auto it : xp.get_structure().mItems ){
    cout << it.first << '\t';
    for( auto jt : it.second.mProps ){
      cout << jt.first << '\t' << jt.second << endl;
    }
  }
}

