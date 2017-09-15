// printf "\n\n\n\n\n\n\n"; g++ settingcc -o settings.exe  -i conf.INP.txt -o conf.INP.txt
// printf "\n\n\n\n\n\n\n"; g++ settingcc -o settings.exe  -A1 40 -Z1 20 -A2 40 -Z2 18 
// g++ -static -static-libgcc -static-libstdc++ settingcc -o settings.exe
// g++ -static -static-libgcc -static-libstdc++ settingcc -o w_settings.exe

// ../bin/settings.exe -print console -A1 42 -Z1 22 -A2 40 -Z2 18 -o ./musor/conf.inp.txt -q yes -f ./musor/INP.DAT
// cd musor ; ../../fort.exe ; cd -


#include "../lib/define.hh"
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <vector>
// namespace nuclear:
#include "../lib/project_functions.hh" 

void help(){
      std::cout << "./settings.exe [-print console|table|conf] -A1 number -A2 number -Z1 number -Z2 number [-o conf.0.txt] [-t isotopes.txt]  " 
      << std::endl;
      std::cout << " -print console    <- print to console the details for the corresponding to A1,A2,Z1,Z2 Elements " << std::endl;
      std::cout << " -print conf       <- print all output configurations  " << std::endl;
      std::cout << " -o conf.0.txt     <- print output configurations to the file conf.0.txt " << std::endl;
      std::cout << " -t isotopes.txt   <- file with the table of isotopes,   by default it's ../conf/isotopes.table.txt " << std::endl;
      std::cout << " -f INP.DAT        <- write the file for the potential, built by fort.exe " << std::endl;
      std::cout << " -h                <- print help " << std::endl;
      std::cout << " -q yes            <- if there is option yes for Q 2n channel " << std::endl;
}


int main(int argc, char** argv) {

  gConf conf_args(argc, argv);
  if(conf_args.is_present("-h") ) { help(); return 0; }

  // -print console|table|conf] -A1 a1 -Z1 z1 -A2 a2 -Z2 z2 -o conf.inp.txt
  gConf conf = nuclear::set_params(conf_args);
 

  // build potential with fort:
  if(conf_args.is_present("-f") ) { 
    nuclear::write_fort(conf, conf_args.get("-f"));
  }



  return 0;
}

