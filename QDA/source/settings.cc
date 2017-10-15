
#include "../lib/define.hh"
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <vector>

// namespace nuclear:
#include "../lib/project_functions.hh" 


void help(){
      std::cout << "./settings.exe  -A1 number -A2 number -Z1 number -Z2 number [-t isotopes.txt]  " << std::endl;
      std::cout << " -t isotopes.txt   <- file with the table of isotopes,   by default it's ../conf/isotopes.table.txt " << std::endl;
      std::cout << " -h                <- print help " << std::endl;
}


int main(int argc, char** argv) {


  gConf conf_args(argc, argv);
  if(conf_args.is_present("-h") ) { help(); return 0; }

  gConf conf = nuclear::set_params(conf_args);
 

  // build potential with fort:
  nuclear::write_fort(conf, "./INP.DAT");



  return 0;
}

