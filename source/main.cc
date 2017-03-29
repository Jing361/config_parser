#include<string>
#include<iostream>

#define CATCH_CONFIG_MAIN
#include<catch.hpp>

#include"config_parse.hh"

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

  cp.parse_config( "data/config.ini" );

  REQUIRE( cp.get_element<string>( "CPU", "ports" ) == string( "1" ) );
  REQUIRE( cp.get_element<int>( "CPU", "ports" ) == 1 );
}

