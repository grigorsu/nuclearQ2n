
// set PATH=D:\ProgramFiles\MinGW3264\mingw64\bin;%PATH%
// g++ wprob.cc -static -static-libgcc -static-libstdc++ -I"d:/ProgramFiles/MinGW3264/mingw64/include/" -L"d:/ProgramFiles/MinGW3264/mingw64/lib/" -lgsl -lgslcblas -lm -o w_prob.exe
// printf "\n\n\n\n\n"; g++ prob.cc -lgsl -lgslcblas -std=c++11 -o prob.exe  \
//  && time ./prob.exe -i ../conf.test0/conf.INP.txt  -o ./conf.prob.txt -o2 ./prob.energies.txt -p ../conf.test0/conf.potential.txt -r ./prob.R -option Ein

// -option Ein
// -option omega
// -option section
// -option energies

// LD_LIBRARY_PATH='/usr/local/lib'
// export LD_LIBRARY_PATH;

#define ERROR(x) std::cout <<"ERROR: "<< __FILE__ << " : " << __LINE__  << " : " << x << std::endl;
#define DEBUG(x) std::cout <<"DEBUG: "<< __FILE__ << " : " << __LINE__  << " : " << x << std::endl;
#define  REP(i,size) for(unsigned int i=0; i < size; ++i) 


#include "../lib/gFile.hh"
#include "../lib/gConf.hh"
#include "../lib/gFuns.hh"
#include "../lib/gMath.hh"
#include "../lib/potential.hh"

// #include "../lib/nuclear.parameters.hh"   // 
#include "../lib/win.nuclear.parameters.hh"   // 
#include "../lib/nuclear.function.hh"   // 
// #include "../lib/win.nuclear.function.hh"   // 


int main(int argc, char** argv) {

  gConf   conf_args(argc, argv);
  gConf   conf_poten( conf_args.get("-p"));  // Lmax, v < MPotential * >  for each L = 0..L_max;
  gConf   conf_param( conf_args.get("-i"));  // 
    
  // Lmax
  double Lmax = atof(conf_poten.get("Lmax").c_str());
  // vector < MPotential * > for each L = 0 .. L_max:
  std::vector <MPotential * >  v_potential(v_potential_from_file(conf_args.get("-p")));
  MNParameters param(conf_args.get("-i"));

  // // PELSS for Ein
  // if ( conf_args.get("-option") == "Ein" ) {
  //   double Ecm  = atof( conf_param.get("Ein").c_str()); 
  //   double pelss = double(nuclear::PELSS(Ecm, param , *(v_potential[0]) ) )  ;
  //   param.print();
  //   nuclear::print(param);
  //   std::cout << "PELSS:  "  << pelss << std::endl ;
  //   conf_param.change("PELSS",  gFile.to_str(pelss) );    
  //   conf_param.save(conf_args.get("-o"));
  //   return 0;    
  // }

// omega for Ein
  if ( conf_args.get("-option") == "omega" ) {
    REP(i,10) {
      double omega = param.omega.real();
      double Ecm = v_potential[0]->UBar + 15. + i*3;
      DEBUG(Ecm)
      double tmpPELSS;// = nuclear::PELSS(Ecm, param, *(v_potential[0]),  &omega); 
      conf_param.change("omega", gFile.to_str(omega));  
      std::cout << "PELSS:  "  << tmpPELSS << "  omega:  "  << omega << std::endl ;
      std::cout << "param.omega: " << param.omega <<std::endl ;
      if(tmpPELSS > 0.9999) {
        break;
      }
    }
    conf_param.save(conf_args.get("-o"));
    return 0;    
  }

  // // Section for Ein
  // if ( conf_args.get("-option") == "section" ) {
  //   double Ecm  = atof( conf_param.get("Ein").c_str()); 
  //   std::vector <double> v_pelss;
  //   double sum = 0;
  //   for(double L = 0; L < Lmax; L = L + 1.0) {
  //     v_pelss.push_back( double(nuclear::PELSS(Ecm, param , *(v_potential[L]) ) )  );
  //     double tmp = 10. * 3.141592654 / 2./ param.mu.real() / Ecm * (2*L+1) * v_pelss[L];
  //     sum = sum + gMath.zero_if_nan(tmp);
  //     std::cout <<" "<< "Ecm: " << Ecm <<"  L: "<< L <<" PELSS: " << v_pelss[L] <<" section: " <<" "<< sum <<std::endl ;
  //   }
  //   conf_param.change("v_PELSS" , gFile.vd2s(v_pelss, "  ") );    
  //   conf_param.change("SIGMA" ,  gFile.to_str(sum) );    
  //   conf_param.save(conf_args.get("-o"));
  //   return 0;    
  // }

  // Section for Ein
  if ( conf_args.get("-option") == "energies" ) {

    // std::vector < double > v_Energ(v_potential[0].build_energies());
    std::vector <double> v_Energ (gFuns.vs2vd(conf_param.get_all("Esp"  ) ));

    // std::vector < double > v_Energ(build_energies(conf_param, "expand")); 
    std::vector < double > v_sigma; 
    std::vector <double>   v_pelss0;
    std::ofstream prob_file0(conf_args.get("-o2"), std::ios_base::out);

    std::string tmp_messsage_previous = "None";


    REP(j,v_Energ.size())  { 
      double Ecm = v_Energ[j];
      double sum = 0;
      prob_file0 <<     "Ecm="   << gFile.format_to_str(Ecm,8,"left") << " PELLS: ";
      std::vector <double> v_pelss;
      if(Lmax == 0) Lmax = 1;
      for(double L = 0; L < Lmax; L = L + 1.0) {

        double tmp_pells = double(nuclear::PELSS(Ecm, param, *(v_potential[L]) ) );
        v_pelss.push_back(tmp_pells);
        if(L==0) v_pelss0.push_back(tmp_pells);
        double tmp = 10. * 3.141592654 / 2./ param.mu.real() / Ecm * (2*L+1) * v_pelss[L];
        sum = sum + gMath.zero_if_nan(tmp);
        std::cout << j <<"/"<< v_Energ.size() <<" Ecm: " << Ecm <<" L: " << L << " Lmax: " << Lmax <<" PELSS: "<< gFile.format_to_str(v_pelss[L],12,"left") <<" sum: " << sum <<std::endl ;
       
        prob_file0 << gFile.format_to_str(v_pelss[L],12,"right") << std::flush;

        if(L > 0 && v_pelss[L] < 0.0000001 * v_pelss[0] ) break;

      }

      // control
      if(conf_args.is_present("-control")) {
        gConf conf_control(conf_args.get("-control"));
        if  (conf_control.get("COMMAND") == "stop") return 0;
        std::string tmp_messsage = "_Ecm:_"  + gFile.to_str( Ecm  )  
        + "_Lmax:_" + gFile.to_str( Lmax ) + "_PELSS[L=0]:_" + gFile.to_str( v_pelss[0] )
        + "_sum:_"  + gFile.to_str( sum  );
        conf_control.change("10MESSAGE","please,_wait");
        conf_control.change("10MESSAGE01",tmp_messsage_previous);
        conf_control.change("10MESSAGE02",tmp_messsage);
        conf_control.save();
        tmp_messsage_previous = tmp_messsage;
      } 


      prob_file0 << std::endl <<     "sigma_" << gFile.format_to_str(Ecm,8,"left")  << " = " << sum  << std::endl;
      std::cout << " final Ecm: " << Ecm  << " sum: " << sum << std::endl ;
      v_sigma.push_back(sum);
    }  
    prob_file0.close();

    // plot v_sigma[v_Energy]
    std::string command = "";
    command = command  + "x<-c(" + gFile.vd2s(v_Energ, ", ") + "); \n" 
    +                    "y<-c(" + gFile.vd2s(v_sigma, ", ") + "); \n"
    +                       "png('probability.png', height = 300, width = 600) \n"
    +                       "plot(x,log(y),type='o',col='red',main='Probability',xlab='r',ylab='log(E)') \n"
    +                       "dev.off() \n";
    gFile.write_file(command, conf_args.get("-r"),  "w");

    conf_param.erase("sigma_sp");
    conf_param.erase("pelss");
    conf_param.add(std::string("sigma_sp   ") + gFile.vd2s(v_sigma,  "  "));
    conf_param.add(std::string("pelss  ") + gFile.vd2s(v_pelss0,  "  "));
    conf_param.save(conf_args.get("-o"));

  }




  return 0;
}





