// set PATH=D:\ProgramFiles\MinGW3264\mingw64\bin;%PATH%
// g++ makeplot.cc  -static -static-libgcc -static-libstdc++ -o w_makeplot.exe
// printf "\n\n\n\n\n\n\n"; g++ makeplot.cc -o makeplot.exe 
// h='../conf'; f='makeplot';  printf "\n\n\n"; g++ $f.cc -o $f.exe && ./$f.exe -h $h/HEU.DAT -i $h/conf.INP.txt -o $h/conf.$f.txt -r $h/$f.R
// R CMD BATCH --no-save --no-restore potential.R

#include <iostream>

#define ERROR(x) std::cout <<"ERROR: "<< __FILE__ << " : " << __LINE__  << " : " << x << std::endl;
#define DEBUG(x) std::cout <<"DEBUG: "<< __FILE__ << " : " << __LINE__  << " : " << x << std::endl;
// #define DEBUG(x) std::cout <<"DEBUG: "<< __FILE__ << " : " << __LINE__  << " : " << x << std::endl;
// #define ERROR(x) std::cerr << __FILE__ << " : " << __LINE__  << " : " << x << std::endl; std::cout.flush(); std::exit(1);
#define $ << "  " << 
#define $$ << std::endl    ;
#define $$$(x)  std::cout << #x << ": " << x << std::endl;  
#define  REP(i,size) for(unsigned int i=0; i < size; ++i) 
#define RREP(i,size1, size2) for(unsigned int i=size1; i < size2; ++i) 

#include "../lib/gFile.hh"
#include "../lib/gConf.hh"
#include <stdio.h>
#include <stdlib.h>
#include <cmath>

using namespace std;
#include <vector>

// #include <stdlib>

void read_potential(std::vector<std::string> &R, std::vector<std::string> &En, gConf &conf ) {
  std::string filename0 = conf.get("HEU.DAT");
  std::vector <double> _R, En1, En2, _En;

  // read from HEU.DAT
  std::vector<std::string> v;
  std::vector <std::string> lines;
  gFile.read_file(filename0,lines);
  REP(i,lines.size()) {
    if( lines[i].find("A") != std::string::npos) continue;
    if( lines[i].find("R") != std::string::npos) continue;
    if( lines[i].find("E") != std::string::npos) continue;
    if( lines[i].size() < 20 ) continue;
    std::vector <std::string> subline;
    gFile.split(lines[i], " ", subline, "");
    if(subline.size() == 5 ) {
      _R.push_back( atof(subline[0].c_str()));
      En1.push_back(atof(subline[1].c_str()));
      En2.push_back(atof(subline[2].c_str()));
      _En.push_back(atof(subline[3].c_str()));
    }
  }
  float z1       = atof(conf.get("Z1").c_str());
  float z2       = atof(conf.get("Z2").c_str());
  float e        = atof(conf.get("e").c_str()); // 1.2;
  float rMaximum = 16.;

  std::vector <float> R_addition();

      float tt1_1 = _R[ _R.size()-1]; 
      float tt1_2 = _R[ _R.size()-2];
      float tt3_1 = En2[_R.size()-1];
      float tt3_2 = En2[_R.size()-2];
  if(_R.size() > 2) {
    for(float r_last = _R.back()+0.2, rr = _R.back()+0.2; rr < r_last + 16.0 ; rr = rr + 0.2) {
      _R.push_back(rr);
      En2.push_back( tt3_2 * pow( tt3_1/tt3_2 , (rr - tt1_2)/(tt1_1 - tt1_2)  ) );
      En1.push_back(z1*z2*e*e/ rr) ;
      _En.push_back(En1.back() + En2.back() );
    }
  } 
  REP(i,_R.size()) {
    R.push_back( gFile.format_to_str( _R[i],9));
    En.push_back(gFile.format_to_str(float( int(_En[i]*10000) ) / 10000. ,9));
  }
}


int main(int argc, char** argv) {

  gConf conf_args(argc, argv);

  if(!( conf_args.is_present("-i") && conf_args.is_present("-o") && conf_args.is_present("-r")) )  {
    cout $ "try: \n  makeplot.exe -h HEU.DAT -i ./conf.input.txt -o ./conf.output.txt -r ./potential.R" $$;
    return 0;
  }


  gConf conf(conf_args.get("-i"));
  conf.erase("HEU.DAT");
  conf.add("HEU.DAT  " +  conf_args.get("-h") );

  std::vector<std::string> R;
  std::vector<std::string> En;
  
  conf.erase("R");
  conf.erase("En");
  // conf.change("R",  gFile.vs2s(R,   "  "));
  // conf.change("En", gFile.vs2s(En,  "  "));
  read_potential(R,En,conf);
  conf.add(std::string("R   ") + gFile.vs2s(R,  "  "));
  conf.add(std::string("En  ") + gFile.vs2s(En,  "  "));

  conf.save( conf_args.get("-o") );

  std::string command = "";
  command = command  + "x<-c(" + gFile.vs2s(R, ", ") + "); \n" 
  +                    "y<-c(" + gFile.vs2s(En, ", ") + "); \n"
  +                       "png('potential.png', height = 300, width = 600) \n"
  +                       "plot(x,y,type='o',col='red',main='Potential',xlab='r',ylab='E') \n"
  +                       "dev.off() \n";

  gFile.write_file(command, conf_args.get("-r"),  "w");

  // R CMD BATCH --no-save --no-restore my_script.R
	// cout <<command;

  return 0;
}


