

#include <iostream>

#define ERROR(x) std::cout <<"ERROR: "<< __FILE__ << " : " << __LINE__  << " : " << x << std::endl;
#define DEBUG(x) std::cout <<"DEBUG: "<< __FILE__ << " : " << __LINE__  << " : " << x << std::endl;
// #define $ << "  " << 
// #define $$ << std::endl    ;
// #define $$$(x)  std::cout << #x << ": " << x << std::endl;  
#define  REP(i,size) for(unsigned int i=0; i < size; ++i) 
// #define RREP(i,size1, size2) for(unsigned int i=size1; i < size2; ++i) 

#include "../lib/gFile.hh"
#include "../lib/gConf.hh"
#include "../lib/gFuns.hh"
#include "../lib/potential.hh"


int main(int argc, char** argv) {


  gConf conf_args(argc, argv);

  //potential.exe -i ./conf.i.txt -o ./conf.potential.txt -r ./plot.R

  std::vector < MPotential * > v_ppotential; // vector of pointers to MPotential for each L
  gConf conf(conf_args.get("-i"));
  double        A1(atof( conf.get("A1"      ).c_str() ))     ;
  double        A2(atof( conf.get("A2"      ).c_str() ))     ;
  double     koeff(atof( conf.get("k" ).c_str() ))     ;
  double       mu (  A1 * A2 / ( A1 + A2 ) *koeff)                ;
  double     Lmax = 0;
  std::vector<double> RR (gFuns.vs2vd(conf.get_all("R"  ) ));
  std::vector<double> U0 (gFuns.vs2vd(conf.get_all("En" ) ));
  size_t L_limit = 100;
  bool is_Lmax = 0;

  REP(L,200) {  // create v_ppotential
    std::vector <double> UU;
    REP(i, U0.size() )  UU.push_back( U0[i] + L*(L+1)/2/mu/RR[i]/RR[i] );
    // DEBUG(gFile.vd2s(RR))
    // DEBUG(gFile.vd2s(UU))
    MPotential * ppotential = new MPotential(RR, UU);
    v_ppotential.push_back(  ppotential );

    if(v_ppotential[L]->UBar - v_ppotential[L]->UMin  <= 0.5 && !is_Lmax) {
      Lmax = L;
      is_Lmax = 1;
    }
    if( L == L_limit) break;
  }


  { // create plot
    std::string sub_command_1 = "";
    std::string sub_command_2 = "";
    REP(L,L_limit+1) {
      std::string si = gFile.to_str(L);
      sub_command_1 = sub_command_1 +  "y"+ si + "<-c(" + gFile.vd2s(v_ppotential[L]->UU, ",") + "); \n";  
      if(L==0) continue;
      sub_command_2 = sub_command_2 +  "lines(x,y"+ si + ",col='green'); \n";  
    }

    std::string command = "";
    command = command  + "x<-c(" + gFile.vd2s(RR, ", ") + "); \n" 
    + sub_command_1
    +                       "png('potential2.png', height = 300, width = 600) \n"
    +                       "plot(x,y"+gFile.to_str(L_limit)+",col='green',main='Potential for different L',xlab='r',ylab='E') \n"
    + sub_command_2
    +                       "lines(x,y"+gFile.to_str(Lmax)+",type='o',col='red'); \n";  
    +                       "dev.off() \n";
    gFile.write_file(command, conf_args.get("-r"),  "w");

    // system("R CMD BATCH --no-save --no-restore potential2.R");
  }


  { //change conf
    conf.change("Lmax", gFile.format_to_str(Lmax,10) );
    conf.change("xMin", gFile.format_to_str(v_ppotential[0]->xMin,10) );
    conf.change("xBar", gFile.format_to_str(v_ppotential[0]->xBar,10) );
    conf.change("UMin", gFile.format_to_str(v_ppotential[0]->UMin,10) );
    conf.change("UBar", gFile.format_to_str(v_ppotential[0]->UBar,10) );
    std::vector <double> Esp(build_energies(*(v_ppotential[0]),  conf, "spherical"));
    std::vector <double> Ed( build_energies(*(v_ppotential[0]),  conf, "deformed"));
    conf.erase("Esp");
    conf.erase("Ed");
    conf.add(std::string("Esp ") + gFile.vd2s(Esp,  "  ", 7));
    conf.add(std::string("Ed  ") + gFile.vd2s( Ed,  "  ", 7));
    // std::cout << gFile.vd2s( Esp , "  "  ,7)  <<std::endl;
    // std::cout << gFile.vd2s( Ed , "  "   ,7)  <<std::endl;
    conf.save( conf_args.get("-o2") );
  }
  
  { // write conf.potential.txt
    // DEBUG("Lmax: " << Lmax)
    // std::cout << " Lmax: " << Lmax << std::endl; 
    std::string message = "";
      REP(L,Lmax+1) {
        std::string si = gFile.format_to_str(L,4,"left");
        message += "L=" + si + " " 
        + "xMin: "+ gFile.format_to_str(v_ppotential[L]->xMin,7)+"  "
        + "xBar: "+ gFile.format_to_str(v_ppotential[L]->xBar,7)+"  "
        + "UMin: "+ gFile.format_to_str(v_ppotential[L]->UMin,7)+"  "
        + "UBar: "+ gFile.format_to_str(v_ppotential[L]->UBar,7)+"  "
        + "UU:   "+ gFile.vd2s( v_ppotential[L]->UU , "  "   ,7) + "\n";
      }
        message += "RR                                                                           " +  gFile.vd2s( RR , "  ", 7) + "\n";
        message += "U0                                                                           " +  gFile.vd2s( U0 , "  ", 7) + "\n";
        // message += "Esp                                                                          " +  gFile.vd2s( U0 , "  ", 7) + "\n";
        // message += "U0                                                                           " +  gFile.vd2s( U0 , "  ", 7) + "\n";
        // message += "Esp                                                                          " +  gFile.vd2s( Esp, "  ", 7) + "\n";
        // message += "Edef                                                                         " +  gFile.vd2s( Ed , "  ", 7) + "\n";
        message += "Lmax  " +  gFile.to_str(Lmax) + "\n";
    gFile.write_file(message, conf_args.get("-o"),  "w");
  }  




  return 0;
}





