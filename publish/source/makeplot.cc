// printf "\n\n\n\n\n\n\n"; g++ makeplot.cc -o makeplot.exe 
    // cout $ "try: \n  makeplot.exe -h HEU.DAT -i ./conf.input.txt -o ./conf.output.txt -r ./potential.R" $$;

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include "../lib/define.hh"
#include "../lib/gFile.hh"
#include "../lib/gConf.hh"
#include "../lib/project_functions.hh"

using namespace std;
#include <vector>




int main(int argc, char** argv) {

  gConf conf_args(argc, argv);

  if(!( conf_args.is_present("-i") && conf_args.is_present("-o") && conf_args.is_present("-r")) )  {
    std::cout <<  "try: \n  makeplot.exe -H FORT.DAT -i ./conf.input.txt -o ./conf.output.txt -r ./potential.R" <<  std::endl;
    return 0;
  }

  gConf conf(conf_args.get("-i"));
  // conf.change("FORT.DAT" ,  conf_args.get("-H") );
  // read potential and put it into R and En
  std::vector<std::string> R;
  std::vector<std::string> En;
  nuclear::read_potential(R,En,conf);
  conf.change("R",  gFile.vs2s(R,   "  "));
  conf.change("En", gFile.vs2s(En,  "  "));


  // write to output conf
  if(conf_args.is_present("-o") ) 
    conf.save( conf_args.get("-o") );

  // write to R
  // to run script from shell, use command, like: R CMD BATCH --no-save --no-restore my_script.R
  if(conf_args.is_present("-r") ) {
    std::string command = "";
    command = command  + "x<-c(" + gFile.vs2s(R, ", ") + "); \n" 
    +                    "y<-c(" + gFile.vs2s(En, ", ") + "); \n"
    +                       "png('potential.png', height = 300, width = 600) \n"
    +                       "plot(x,y,type='o',col='red',main='Potential',xlab='r',ylab='E') \n"
    +                       "dev.off() \n";
    gFile.write_file(command, conf_args.get("-r"),  "w");
  }


  return 0;
}


