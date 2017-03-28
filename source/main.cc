#include<string>
#include<iostream>

#include"config_parse.hh"

using namespace std;

int main(){
  config_parse cp;
  cp.add_element( "CPU", "ports" );
  cp.add_element( "CPU", "timers" );
  cp.add_element( "CPU", "adcs" );

  cp.parse_config( "data/config.ini" );

  cout << cp.get_element<string>( "CPU", "ports" ) << endl;

  return 0;
}

