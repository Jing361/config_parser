#include<fstream>

#include"config_parse.hh"

using namespace std;

int main(){
  configparse cp;
  fstream file( "data/config.ini" );

  cp.add_argument( "CPU", 3 );
  cp.add_element( "CPU", "ports" );
  cp.add_element( "CPU", "timers" );
  cp.add_element( "CPU", "adcs" );

  file = cp.parse_config( file );

  return 0;
}

