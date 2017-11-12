
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>

#include "../lib/define.hh"
#include "../lib/project_functions.hh"

#include "../lib/gFile.hh"
#include "../lib/gConf.hh"
#include "../lib/gFuns.hh"
#include "../lib/gMath.hh"

#include "../lib/potential.hh"
#include "../lib/nuclear.parameters.hh"   // 
#include "../lib/nuclear.function.hh"   // 


using namespace std;
#include <vector>



int main(int argc, char** argv) {

  gConf conf_args(argc, argv);

  
  conf_args.add("-i     ./conf.inp.txt "); 
  conf_args.add("-o     ./conf.out.txt"); 
  conf_args.add("-e     ./prob.energies.txt"); 
  conf_args.add("-r     ./plot.R"); 
  conf_args.add("-op    ./conf.potential.txt"); 
  conf_args.add("-re    ./prob.R"); 
  conf_args.add("-rd    ./probd.R"); 
  conf_args.add("-rp    ./potential.R"); 


  gConf  conf_param( conf_args.get("-i"));
  gConf  conf_poten(0); 
  

  gFile.write_file("", conf_args.get("-o"), "w"  );

  // -------- part 1: read potential from FORT.EXE, save to conf_param and create its plot
  {
    // "adds vectors U0 and RR to conf_param
    nuclear::read_potential(conf_param); 
    if(conf_args.is_present("-rp") ) {
        nuclear::write_potential_plot_R(conf_args.get("-rp"), conf_param);
    }
  }
  
  // -------- part 2: read potential from FORT.EXE, save to conf_param and create its plot

  double     Lmax = 0;
  {
    std::vector < MPotential * > v_ppotential; // vector of pointers to MPotential for each L
    double        A1(atof( conf_param.get("A1"      ).c_str() ))     ;
    double        A2(atof( conf_param.get("A2"      ).c_str() ))     ;
    double     koeff(atof( conf_param.get("k" ).c_str() ))     ;
    double       mu (  A1 * A2 / ( A1 + A2 ) *koeff)                ;
    vector <double>  RR ( conf_param.get_vd("R"  ) );
    vector <double>  U0 ( conf_param.get_vd("U0" ) );
    size_t L_limit = 100;
    bool is_Lmax = 0;
    
    // calculate Lmax
    REP(L,200) {  // create v_ppotential
      std::vector <double> UU;
      REP(i, U0.size() )  UU.push_back( U0[i] + L*(L+1)/2/mu/RR[i]/RR[i] );
      MPotential * ppotential = new MPotential(RR, UU);
      v_ppotential.push_back(  ppotential );

      if(v_ppotential[L]->VBar - v_ppotential[L]->VMin  <= 0.5 && !is_Lmax) {
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
    }


    { //change conf
      conf_param.change("Lmax", gFile.format_to_str(Lmax,10) );
      conf_param.change("RMin", gFile.format_to_str(v_ppotential[0]->RMin,10) );
      conf_param.change("RBar", gFile.format_to_str(v_ppotential[0]->RBar,10) );
      conf_param.change("VMin", gFile.format_to_str(v_ppotential[0]->VMin,10) );
      conf_param.change("VBar", gFile.format_to_str(v_ppotential[0]->VBar,10) );
      std::vector <double> Esp(build_energies(*(v_ppotential[0]),  conf_param, "spherical"));
      std::vector <double> Ed( build_energies(*(v_ppotential[0]),  conf_param, "deformed"));
      conf_param.erase("Esp");
      conf_param.erase("Ed");
      conf_param.add(std::string("Esp ") + gFile.vd2s(Esp,  "  ", 7));
      conf_param.add(std::string("Ed  ") + gFile.vd2s( Ed,  "  ", 7));
    }

  
    if(conf_args.is_present("-debug")) conf_param.print();

    { // write conf.potential.txt
        REP(L,Lmax+1) {
          conf_poten.add( "L=" + gFile.format_to_str(L,4,"left") + " " 
              + "RMin: "+ gFile.format_to_str(v_ppotential[L]->RMin,7)+"  "
              + "RBar: "+ gFile.format_to_str(v_ppotential[L]->RBar,7)+"  "
              + "VMin: "+ gFile.format_to_str(v_ppotential[L]->VMin,7)+"  "
              + "VBar: "+ gFile.format_to_str(v_ppotential[L]->VBar,7)+"  "
              + "UU:   "+ gFile.vd2s( v_ppotential[L]->UU , "  "   ,7) 
          );
        }
        std::string message = "";
        conf_poten.add("RR    " +  gFile.vd2s( RR , "  ", 7) );
        conf_poten.add("U0    " +  gFile.vd2s( U0 , "  ", 7) );
        conf_poten.add("Lmax  " +  gFile.to_str(Lmax)        );
      // gFile.write_file(message, conf_args.get("-op"),  "w");
        conf_poten.save(conf_args.get("-op"));  
    }  
  }


  // -------- part 3:
  {
    // Lmax
    double Lmax = atof(conf_poten.get("Lmax").c_str());
    std::vector <MPotential * >  v_potential(v_potential_from_file(conf_args.get("-op")));

    // omega for Ein
    {
      MNParameters param(conf_args.get("-i"));
      REP(i,10) {
        double omega = param.omega.real();
        double Ecm = v_potential[0]->VBar + 15. + i*3;
        double tmpPELSS = nuclear::PELSS(Ecm, param, *(v_potential[0]),  &omega); 
        conf_param.change("omega", gFile.to_str(omega));  
        if(conf_args.is_present("-debug")) {
          std::cout << "PELSS:  "  << tmpPELSS << "  omega:  "  << omega << std::endl ;
          std::cout << "param.omega: " << param.omega <<std::endl ;
        }
        if(tmpPELSS > 0.9999) {
          break;
        }
      }
    }



    // Section for Ein
    {
      std::cout << std::endl;
      MNParameters param(conf_args.get("-i"));
      std::vector <double> v_Energ (gFuns.vs2vd(conf_param.get_all("Esp"  ) ));

      std::vector < double > v_sigma; 
      std::vector <double>   v_pelss0;
      std::ofstream prob_file0(conf_args.get("-e"), std::ios_base::out);

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
          if(conf_args.is_present("-debug")) 
            std::cout << j <<"/"<< v_Energ.size() <<" Ecm: " << Ecm <<" L: " << L << " Lmax: " << Lmax <<" PELSS: "<< gFile.format_to_str(v_pelss[L],12,"left") <<" sum: " << sum <<std::endl ;
          else
            std::cout  << round( float(j)  /  float(v_Energ.size())  * 100.)  << "\% left" << "\r";
          
          prob_file0 << gFile.format_to_str(v_pelss[L],12,"right") << std::flush;

          if(L > 0 && v_pelss[L] < 0.0000001 * v_pelss[0] ) break;

        }

        prob_file0 << std::endl <<     "sigma_" << gFile.format_to_str(Ecm,8,"left")  << " = " << sum  << std::endl;
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
      gFile.write_file(command, conf_args.get("-re"),  "w");

      conf_param.erase("sigma_sp");
      conf_param.erase("pelss");
      conf_param.add(std::string("sigma_sp   ") + gFile.vd2s(v_sigma,  "  "));
      conf_param.add(std::string("pelss  ") + gFile.vd2s(v_pelss0,  "  "));
      std::cout << std::endl;
    }



    // Deformed 
    DeformProb dfprob(conf_param);

    std::string tmp_messsage_previous = "None";

    
    std::cout << std::endl;
    //----------
    std::vector < double > v_SS24;
    for(size_t i = 0; i < dfprob.vEd.size(); ++i) {
      double ss24 = 0;
      for(double m = 0; m <= 0.5 * dfprob.pi; m = m + 0.01) {
        for(double k = 0; k <= 0.5 * dfprob.pi; k = k + 0.01) {
          ss24 = ss24 + 0.01 * std::sin(k) * 0.01 * std::sin(m)*dfprob.eval(dfprob.vEd[i] - dfprob.EbD(m,k));
        }
      }
      v_SS24.push_back(ss24);

      if(conf_args.is_present("-debug")) 
        std::cout << " SS24: "  << i << "  Ein =  " << dfprob.vEd[i] << ": " << v_SS24[i] << std::endl;
    }


    std::string message0 = "sigma_cap mb  ";
    // std::string message1 = "Ed  "; Ed -> Ecm
    std::string message1 = "Ecm  MeV";
    for(size_t i=0; i < v_SS24.size(); ++i) {
      message0 = message0 + "  " +  gFile.to_str(v_SS24[i]);
    }
    for(size_t i=0; i < dfprob.vEd.size(); ++i) {
      message1 = message1 + "  " +  gFile.to_str(  dfprob.vEd[i] );
    }

   
    std::cout << "Lmax = " << conf_param.get("Lmax") << std::endl;
    std::cout << "RMin = " << conf_param.get("RMin") << std::endl;
    std::cout << "RBar = " << conf_param.get("RBar") << std::endl;
    std::cout << "VMin = " << conf_param.get("VMin") << std::endl;
    std::cout << "VBar = " << conf_param.get("VBar") << std::endl;

    std::cout << message0  << std::endl;
    std::cout << message1  << std::endl;

    conf_param.erase("sigma_cap");
    conf_param.add(message0);
    conf_param.save(conf_args.get("-o"));


    if(conf_args.is_present("-r")) {
      // plot v_sigma[v_Energy]
      std::string command = "";
      command = command  + "x<-c(" + gFile.vd2s(dfprob.vEd, ", ") + "); \n" 
      +                    "y<-c(" + gFile.vd2s(v_SS24, ", ") + "); \n"
      +                       "png('probability.deformed.png', height = 300, width = 600) \n"
      +                       "plot(x,y,log='y',type='o',col='red',main=expression(paste(sigma,'(E)')),xlab='r',ylab=expression(sigma)) \n"
      +                       "dev.off() \n";
      gFile.write_file(command, conf_args.get("-rd"),  "w");
    }

  }

  

  return 0;
}



