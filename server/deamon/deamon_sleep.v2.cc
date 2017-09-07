// printf "\n\n\n\n\n"; g++ deamon_sleep.v2.cc -o deamon_sleep.v2.exe  
// printf "\n\n\n\n\n"; g++  -static -static-libgcc -static-libstdc++  deamon_sleep.v2.cc -o deamon_sleep.v2.exe  
// ./deamon_sleep.v2.exe -h  /vagrant/www/www5/ -b /vagrant/www/www5/






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

using namespace std;
#include <vector>

#include <unistd.h>

// void run_command(std::string about , std::string s_command, gConf & conf) {
      // std::cout $ " ------------ " << about << " started  ------------ " $$
      // conf.change("01STATUS", "run");
      // conf.save();
      // system(s_command.c_str());
      // conf.change("01STATUS", "ready");
      // conf.save();
      // std::cout $ " ------------ " << about << " finished ------------ " $$
// }

void experiment_copy(std::string home_dir, std::string experiment, std::string option);
void remove_file_if_exist(std::string filename);
void write_fort(gConf &_conf, std::string filename);


int main(int argc, char * argv[]) {

gConf   conf_args(argc, argv);

if(conf_args.is_present("-help")) {
  std::cout  << "try: ./deamon_sleep.exe -h  'path to home dir'  -b 'path to bin dir'" << std::endl;
  std::cout  << " 'path to home dir' should have: " << std::endl;
  std::cout  << "   - conf.server.status.txt          (with fields )" << std::endl;
  std::cout  << "   - conf.server.status.default.txt  " << std::endl;
  std::cout  << "   - /conf/conf.INP.txt  " << std::endl;
  std::cout  << "   - /conf/conf.INP.txt  " << std::endl;
  std::cout  << "   - /fort/  " << std::endl;
  return 0;
}
// 
// current folder: $home/p/nuclear/bin  change it to $home/p/nuclear/bin
  // std::string home_dir = ".//p/nuclear/bin";
  
  std::string home_dir = conf_args.get("-h") ;
  std::string bin_dir =  conf_args.get("-b") ;

  std::string confname = home_dir + "/conf.server.status.txt";
  std::string conf_inp            = home_dir + "/conf/conf.INP.txt";
  std::string conf_makeplot       = home_dir + "/conf/conf.INP.txt";
  // std::string conf_makeplot       = home_dir + "/conf/conf.makeplot.txt";
  std::string conf_potential      = home_dir + "/conf/conf.potential.txt";
  std::string conf_prob           = home_dir + "/conf/conf.INP.txt";
  // std::string conf_prob           = home_dir + "/conf/conf.prob.txt";
  std::string conf_prob_energies  = home_dir + "/conf/conf.prob.energies.txt";
  std::string conf_not_spherical  = home_dir + "/conf/conf.not_spherical.txt";

  std::string f_plot_potential    = home_dir + "/conf/R/potential.R";
  std::string f_plot_potential_2  = home_dir + "/conf/R/potential2.R";
  std::string f_plot_prob         = home_dir + "/conf/R/prob.R";
  std::string f_heu_dat           = home_dir + "/conf/HEU.DAT";
  // std::string f_inp_dat           = home_dir + "/conf/INP.DAT";


  system( ( "cp "+home_dir+"conf.server.status.default.txt   " + confname ).c_str() );



  while(1) {
    usleep( 500000 );
    gConf conf(confname);
    // conf.read_from_file(confname); 

    std::string command = conf.get("01STATUS");
    std::string _m="";

// DEBUG(command); conf.print();
    if(command == "stop") {
      cout $ "------------- deamon stopped. --------- "  $$
      conf.change("01STATUS", "ready   0");  conf.save();
      break;
    } else if(command == "ready") {

      // break;
    } else if(command == "pwd") {
      cout $ "-------------        pwd      --------- " $$
      system("pwd");
      conf.change("01STATUS", "ready   100");  conf.save();
    } else if(command == "ls") {
      cout $ "-------------        ls       --------- " $$
      system("pwd");
      conf.change("01STATUS", "ready   100");  conf.save();
    // 
      // } else if(command == "potential_run") {
      //   cout $ "---------------------------------- calculate potential :  " $$

      //   _m = "cd " + home_dir + "/fort && ./fort.exe  && mv HEU.DAT ../conf/HEU.DAT && cp INP.DAT ../conf/INP.DAT";
      //   std::cout << ">> " << _m <<std::endl;  system(_m.c_str());

      //   _m = bin_dir + "/makeplot/makeplot.exe "
      //                + " -h " + home_dir + "/conf/HEU.DAT" 
      //                + " -i " + home_dir + "/conf/conf.INP.2.txt"
      //                + " -o " + home_dir + "/conf/conf.makeplot.txt"
      //                + " -r " + f_plot_potential;
      //   std::cout << ">> " << _m <<std::endl;  system(_m.c_str());

      //   // _m = "cd " + home_dir + "/conf/R/ && R CMD BATCH --no-save --no-restore " + f_plot_potential ;
      //   // std::cout << ">> " << _m <<std::endl;  system(_m.c_str());

      //   _m = bin_dir + "/potential/potential.exe "
      //   +              " -i  " + home_dir + "/conf/conf.makeplot.txt" 
      //   +              " -o  " + home_dir + "/conf/conf.potential.txt" 
      //   +              " -o2 " + home_dir + "/conf/conf.makeplot.txt" 
      //   +              " -r " + f_plot_potential_2 ;
      //   std::cout << ">> " << _m <<std::endl;  system(_m.c_str());

      //   // _m = ("cd " + home_dir + "/conf/R/ && R CMD BATCH --no-save --no-restore " + f_plot_potential_2);
      //   // std::cout << ">> " << _m <<std::endl;  system(_m.c_str());

      //   conf.change("03IS_POTENTIAL_LAST", "1");  
      //   conf.change("01STATUS", "ready   100");  
      //   conf.save();
      //   cout $ " calculate potential finished --------------------------\n\n" $$
      // } else if(command == "probability_run") {
      //   cout $ "---------------------------------- calculate probability :  " $$
      //   _m = bin_dir + "/prob/prob.exe "
      //   +              " -i " + home_dir + "/conf/conf.makeplot.txt"
      //   +              " -p " + home_dir + "/conf/conf.potential.txt"
      //   +              " -o " + home_dir + "/conf/conf.prob.txt"
      //   +              " -option omega";    // adds omega

      //   std::cout << ">> " << _m <<std::endl;  system(_m.c_str());
      //   _m = bin_dir + "/prob/prob.exe "
      //   +              " -i        " + home_dir + "/conf/conf.prob.txt"
      //   +              " -p        " + home_dir + "/conf/conf.potential.txt"
      //   +              " -o        " + home_dir + "/conf/conf.prob.txt"
      //   +              " -o2       " + home_dir + "/conf/prob.energies.txt"
      //   +              " -r        " + f_plot_prob
      //   +              " -control  " + confname // conf_server
      //   +              " -option energies > " + home_dir + "/conf/prob.energies.log";
      //   std::cout << ">> " << _m <<std::endl;  system(_m.c_str());

      //   // _m = "cd " + home_dir + "/conf/R/ && R CMD BATCH --no-save --no-restore " + f_plot_prob;
      //   // std::cout << ">> " << _m <<std::endl;  system(_m.c_str());

      //   conf.change("01STATUS", "ready   100");  
      //   conf.change("04IS_PROBABILITY_LAST", "1");  
      //   conf.change("07REFRESH", "1");  
      //   conf.save();
      //   cout $ " calculate probability finished ------------------------\n\n" $$
      // } else if(command == "not_spherical_run") {
      //   cout $ "---------------------------------- calculate probability for not spherical case :  " $$
      //   _m = bin_dir + "/not_spherical/not_spherical.exe "
      //   +              " -i " + home_dir + "/conf/conf.prob.txt"
      //   +              " -p " + home_dir + "/conf/conf.potential.txt"
      //   +              " -control " + confname
      //   +              " -e " + home_dir + "/conf/conf.prob.energies.txt"
      //   +              " -o " + home_dir + "/conf/conf.not_spherical.txt" + " > " + home_dir + "/conf/not_spherical.log";
      //   std::cout << ">> " << _m <<std::endl;  system(_m.c_str());

      //   conf.change("05IS_NOT_SHPERICAL_LAST", "1");  
      //   conf.change("07REFRESH", "1");  
      //   conf.change("01STATUS", "ready   100");  
      //   conf.save();
      //   cout $ " calculate probability finished ------------------------\n\n" $$
    } else if(command == "all_run" || command == "all_run_log") {

      conf.change("01STATUS", "all_run  potential_evaluation:_start");  conf.save();

        gConf conf_inp_tmp(home_dir + "/conf/conf.INP.2.txt");
        write_fort(conf_inp_tmp, bin_dir + "/fort/INP.DAT");
        _m = "cd " + bin_dir + "/fort && ./fort.exe  && mv HEU.DAT " + home_dir + "/conf/HEU.DAT && mv INP.DAT " + home_dir + "/conf/INP.DAT";
        std::cout << ">> " << _m <<std::endl;  system(_m.c_str());

      conf.change("01STATUS", "all_run  potential_evaluation:_middle");       conf.save();
        _m = bin_dir + "/makeplot/makeplot.exe "
                     + " -h " + home_dir + "/conf/HEU.DAT" 
                     + " -i " + home_dir + "/conf/conf.INP.2.txt"
                     + " -o " + home_dir + "/conf/conf.makeplot.txt"
                     + " -r " + f_plot_potential;
        std::cout << ">> " << _m <<std::endl;  system(_m.c_str());

      conf.change("01STATUS", "all_run  potential_evaluation:_final");        conf.save();
        _m = bin_dir + "/potential/potential.exe "
        +              " -i  " + home_dir + "/conf/conf.makeplot.txt" 
        +              " -o  " + home_dir + "/conf/conf.potential.txt" 
        +              " -o2 " + home_dir + "/conf/conf.makeplot.txt" 
        +              " -r " + f_plot_potential_2 ;
        std::cout << ">> " << _m <<std::endl;  system(_m.c_str());

      conf.change("01STATUS", "all_run  probabilities_calculation:_omega");   conf.save();
        _m = bin_dir + "/prob/prob.exe "
        +              " -i " + home_dir + "/conf/conf.makeplot.txt"
        +              " -p " + home_dir + "/conf/conf.potential.txt"
        +              " -o " + home_dir + "/conf/conf.prob.txt"
        +              " -option omega";    // adds omega
        std::cout << ">> " << _m <<std::endl;  system(_m.c_str());


      conf.change("01STATUS", "all_run  probabilities_calculation:_sigma");   conf.save();
        std::string s_log = (command == "all_run_log") ? " > " + home_dir + "/conf/prob.energies.log" : "";
        _m = bin_dir + "/prob/prob.exe "
        +              " -i        " + home_dir + "/conf/conf.prob.txt"
        +              " -p        " + home_dir + "/conf/conf.potential.txt"
        +              " -o        " + home_dir + "/conf/conf.prob.txt"
        +              " -o2       " + home_dir + "/conf/prob.energies.txt"
        +              " -r        " + f_plot_prob
        +              " -control  " + confname // conf_server
        +              " -option energies " + s_log ;
        std::cout << ">> " << _m <<std::endl;  system(_m.c_str());

      conf.change("01STATUS", "all_run  deformed_sigma");  conf.save();
        s_log = (command == "all_run_log") ? " > " + home_dir + "/conf/not_spherical.log" : "";
        _m = bin_dir + "/not_spherical/not_spherical.exe "
        +              " -i " + home_dir + "/conf/conf.prob.txt"
        +              " -p " + home_dir + "/conf/conf.potential.txt"
        +              " -control " + confname
        +              " -r " + home_dir + "/conf/R/prob.deformed.R"
        +              " -e " + home_dir + "/conf/conf.prob.energies.txt"
        +              " -o " + home_dir + "/conf/conf.not_spherical.txt" + s_log;
        std::cout << ">> " << _m <<std::endl;  system(_m.c_str());

        // _m = "cd " + home_dir + "/conf/R/ && R CMD BATCH --no-save --no-restore  prob.deformed.R" ;
        // std::cout << ">> " << _m <<std::endl;  system(_m.c_str());

      conf.change("01STATUS", "ready   100");  
      conf.save();

      std::cout << ">> finished. " << std::endl;


    } else if(command == "get.elements") {
      cout $ "---------------------------------- get.elements :  " $$

      _m = bin_dir + "/set_params/set_params.exe "
      +              " -t   " + bin_dir + "/set_params/isotopes.table.2.txt"
      +              " -i   " + conf_inp
      +              " -log " + confname
      +              " -o   " + conf_inp ;
      std::cout << ">> " << _m <<std::endl;  system(_m.c_str());
      conf.update();
      conf.change("01STATUS", "ready   100");  
      conf.save();
      cout $ " get.elements finished ------------------------\n\n" $$
    //
      // } else if(command == "experiment.load.default") {
      //   // cout $ "---------------------------------- experiment.load.default :  " $$
      //   experiment_copy(home_dir, "0", "load");
      //   conf.change("01STATUS", "ready   100");  conf.save();
      //   cout $ " experiment.load.default finished ------------------------\n\n" $$
      // } else if(command == "experiment.save.current") {
      //   // cout $ "---------------------------------- experiment.save.current :  " $$
      //   experiment_copy(home_dir, "error", "save");
      //   conf.change("01STATUS", "ready   100");  conf.save();
      //   cout $ " experiment.save.current finished ------------------------\n\n" $$
      // } else if(command == "experiment.load") {
      //   // cout $ "---------------------------------- experiment.load :  " $$
      //   std::string experiment = conf.get("06EXPRIMENT", 0);
      //   experiment_copy(home_dir, experiment, "load");
      //   conf.change("01STATUS", "ready   100");  conf.save();
      //   cout $ " experiment.load finished ------------------------\n\n" $$
      // } else if(command == "experiment.delete") {
      //   // cout $ "---------------------------------- experiment.delete :  " $$
      //   std::string experiment = conf.get("06EXPRIMENT", 0);
      //   experiment_copy(home_dir, experiment, "delete");
      //   conf.change("01STATUS", "ready   100");  conf.save();
      //   cout $ " experiment delete finished ------------------------\n\n" $$
      // } else if(command == "#####") {
      // } else if(command == "#####") {
      // } else if(command == "#####") {
    } else {

    }


  }


  return 0;
}


void write_fort(gConf &_conf, std::string filename) {
  std::string content;
  char buffer [40];
  sprintf( buffer,            "  %s\n",     _conf.get( "reaction"   ).c_str()   ); content += buffer; //  "22Ne+238U"
  sprintf( buffer,          " %03d.00",(int)_conf.getf("A1"         )           ); content += buffer; //   048.00
  sprintf( buffer,       "    %03d.00",(int)_conf.getf("A2"         )           ); content += buffer; //   122.00
  sprintf( buffer,      "     %03d.00",(int)_conf.getf("Z1"         )           ); content += buffer; //   022.00
  sprintf( buffer,     "    %03d.00\n",(int)_conf.getf("Z2"         )           ); content += buffer; //   050.00
  sprintf( buffer,         "%.2f00111",     _conf.getf("d1"         )           ); content += buffer; //   0.5500111
  sprintf( buffer,    "    %.2f0111\n",     _conf.getf("d2"         )           ); content += buffer; //   0.550111
  sprintf( buffer,              "%.3f",     _conf.getf("r0"         )           ); content += buffer; //   1.150
  sprintf( buffer,         "     %.2f",     _conf.getf("ro0"        )           ); content += buffer; //   0.17
  sprintf( buffer,    "        %.2f\n",     _conf.getf("val_D_3"    )           ); content += buffer; //   -0.10
  sprintf( buffer,              "%.5f",     _conf.getf("val_E_1"    )           ); content += buffer; //   0.00001
  sprintf( buffer,          "    %.1f",     _conf.getf("val_E_2"    )           ); content += buffer; //   30.0
  sprintf( buffer,        "      %.3f",     _conf.getf("val_E_3"    )           ); content += buffer; //   0.001
  sprintf( buffer,        "    %.5f\n",     _conf.getf("val_E_4"    )           ); content += buffer; //   0.00001
  sprintf( buffer,             " %d\n",(int)_conf.getf("val_F_1"    )           ); content += buffer; //   34
  sprintf( buffer,              "  %d",(int)_conf.getf("val_G_1"    )           ); content += buffer; //   1
  sprintf( buffer,         "     %d\n",(int)_conf.getf("val_G_2"    )           ); content += buffer; //   1
  sprintf( buffer,              "%.3f",     _conf.getf("val_H_1"    )           ); content += buffer; //   0.000
  sprintf( buffer,       "       %.3f",     _conf.getf("val_H_2"    )           ); content += buffer; //   3.140
  sprintf( buffer,        "      %.4f",     _conf.getf("val_H_3"    )           ); content += buffer; //   0.0000
  sprintf( buffer,          "    %.4f",     _conf.getf("val_H_4"    )           ); content += buffer; //   0.0000
  sprintf( buffer,       "     %.3f\n",     _conf.getf("val_H_5"    )           ); content += buffer; //   0.001
  sprintf( buffer,              "%.2f",     _conf.getf("val_I_1"    )           ); content += buffer; //   0.00
  sprintf( buffer,      "        %.2f",     _conf.getf("val_I_2"    )           ); content += buffer; //   0.00
  sprintf( buffer, "            %d.\n",(int)_conf.getf("val_I_3"    )           ); content += buffer; //   0.
  sprintf( buffer,              "%.3f",     _conf.getf("val_J_1"    )           ); content += buffer; //   0.440
  sprintf( buffer,       "       %.2f",     _conf.getf("val_J_2"    )           ); content += buffer; //   3.00
  sprintf( buffer,    "        %.3f\n",     _conf.getf("val_J_3"    )           ); content += buffer; //   0.450
  sprintf( buffer,              "%.2f",     _conf.getf("val_K_1"    )           ); content += buffer; //   300.00
  sprintf( buffer,         "     %.2f",     _conf.getf("val_K_2"    )           ); content += buffer; //   0.09
  sprintf( buffer,        "      %.2f",     _conf.getf("val_K_3"    )           ); content += buffer; //   0.42
  sprintf( buffer,       "       %.2f",     _conf.getf("val_K_4"    )           ); content += buffer; //   -2.59
  sprintf( buffer,       "     %.2f\n",     _conf.getf("val_K_5"    )           ); content += buffer; //   0.54
  sprintf( buffer,        "      %.2f",     _conf.getf("val_L_1"    )           ); content += buffer; //   0.55
  sprintf( buffer,        "      %.2f",     _conf.getf("val_L_2"    )           ); content += buffer; //   0.99
  sprintf( buffer,        "      %.2f",     _conf.getf("val_L_3"    )           ); content += buffer; //   0.56
  sprintf( buffer,       "     %.2f\n",     _conf.getf("val_L_4"    )           ); content += buffer; //   1.00
  sprintf( buffer,        "      %.2f",     _conf.getf("val_M_1"    )           ); content += buffer; //   0.59
  sprintf( buffer,        "      %.2f",     _conf.getf("val_M_2"    )           ); content += buffer; //   1.02
  sprintf( buffer,        "      %.2f",     _conf.getf("val_M_3"    )           ); content += buffer; //   0.60
  sprintf( buffer,       "     %.2f\n",     _conf.getf("val_M_4"    )           ); content += buffer; //   1.11
  sprintf( buffer,                "%s",     _conf.get( "val_N_1"    ).c_str()   ); content += buffer; //  "HEU.DAT"
  gFile.write_file(content, filename );
}

// void experiment_copy(std::string home_dir, std::string experiment, std::string option) {

//   std::vector<std::string> files;
//   gFile.ls( files, home_dir + "/experiments/","",0);

//   std::string path_from = "/##################/#####/";
//   std::string path_to = "/##################/#####/";

//       // experiment_copy(home_dir, "0", "load");
//       // experiment_copy(home_dir, "error", "save");
//       // experiment_copy(home_dir, experiment, "load");
//       // experiment_copy(home_dir, experiment, "delete");

//   if (option == "save") {
//     for(int i = 0; i < 20; ++i) {
//       if(i == 20 ) {
//         cout $ "too many experiments, please, delete some " $$;
//         return;
//       }
//       if(  gFile.find_in_v(gFile.to_str(i),files) >= 0) {
//         continue; 
//       } else {
//         experiment = gFile.to_str(i);
//         break;
//       }
//     }

//     path_from = home_dir + "/conf/";
//     path_to   = home_dir + "/experiments/" + experiment + "/";
    
//     // DEBUG("pwd")
//     // DEBUG((" cp " + home_dir + "/conf.server.status.txt " + home_dir + "/experiments/" + experiment + "/conf.server.status.txt ").c_str());
//     // DEBUG((" cp " + home_dir + "/conf/HEU.DAT           " + home_dir + "/experiments/" + experiment + "/HEU.DAT                ").c_str());
//     // DEBUG((" cp " + home_dir + "/conf/INP.DAT           " + home_dir + "/experiments/" + experiment + "/INP.DAT                ").c_str());
//     // DEBUG(("mkdir " + path_to).c_str());
//     // DEBUG(("mkdir " + path_to + "/R").c_str());

//     // system((" cp " + home_dir + "/conf.server.status.txt " + home_dir + "/experiments/" + experiment + "/conf.server.status.txt ").c_str());
//     // system((" cp " + home_dir + "/conf/HEU.DAT           " + home_dir + "/experiments/" + experiment + "/HEU.DAT                ").c_str());
//     // system((" cp " + home_dir + "/conf/INP.DAT           " + home_dir + "/experiments/" + experiment + "/INP.DAT                ").c_str());
//     system(("mkdir " + path_to).c_str());
//     system(("mkdir " + path_to + "R").c_str());
//     system((" cp " + path_from + "/conf.server.status.txt " + home_dir + "/conf.server.status.txt " ).c_str());
//     // system(("cp -R " + path_from + "/* " + path_to).c_str());
//     // system(("mkdir " + path_to + "/R").c_str());
//   } else if (option == "load") {

//     if(gFile.find_in_v(experiment,files) == -1) { 
//       cout $ " undefined experiment \n" $$
//       return;
//     }
//     path_from = home_dir + "/experiments/" + experiment;
//     path_to   = home_dir + "/conf/";
//     // system(("cp -R " + path_from + "/* " + path_to).c_str());
//     system((" cp " + path_from + "/conf.server.status.txt " + home_dir + "/conf.server.status.txt " ).c_str());
//     // system((" cp " + home_dir + "/experiments/" + experiment + "/HEU.DAT                " + home_dir + "/fort/HEU.DAT           " ).c_str());
//     // system((" cp " + home_dir + "/experiments/" + experiment + "/INP.DAT                " + home_dir + "/fort/INP.DAT           " ).c_str());
//   } else if (option == "delete") {

//     if(gFile.find_in_v(experiment,files) == -1) { 
//       cout $ " undefined experiment \n" $$
//     } else if(experiment == "0"){
//       cout $ " experiment 0 is default and can't be deleted \n" $$
//     } else {
//       system(("rm -rf " + home_dir + "/experiments/" + experiment).c_str() );
//     } 
//     return;
//   }

//   system((" cp " + path_from + "/not_spherical.log               " + path_to + "/not_spherical.log               ").c_str());
//   system((" cp " + path_from + "/prob.energies.log               " + path_to + "/prob.energies.log               ").c_str());
//   system((" cp " + path_from + "/INP.DAT                         " + path_to + "/INP.DAT             ").c_str());
//   system((" cp " + path_from + "/HEU.DAT                         " + path_to + "/HEU.DAT             ").c_str());
//   system((" cp " + path_from + "/conf.prob.energies.txt          " + path_to + "/conf.prob.energies.txt          ").c_str());
//   system((" cp " + path_from + "/conf.prob.txt                   " + path_to + "/conf.prob.txt                   ").c_str());
//   system((" cp " + path_from + "/conf.potential.txt              " + path_to + "/conf.potential.txt              ").c_str());
//   system((" cp " + path_from + "/conf.not_spherical.txt          " + path_to + "/conf.not_spherical.txt          ").c_str());
//   system((" cp " + path_from + "/conf.INP.txt                    " + path_to + "/conf.INP.txt        ").c_str());
//   system((" cp " + path_from + "/conf.INP.default.txt            " + path_to + "/conf.INP.default.txt").c_str());
//   system((" cp " + path_from + "/R/potential.png                 " + path_to + "/R/potential.png                 ").c_str());
//   system((" cp " + path_from + "/R/potential2.png                " + path_to + "/R/potential2.png                ").c_str());
//   system((" cp " + path_from + "/R/probability.png               " + path_to + "/R/probability.png               ").c_str());
//   system((" cp " + path_from + "/R/potential.R                   " + path_to + "/R/potential.R                   ").c_str());
//   system((" cp " + path_from + "/R/potential2.R                  " + path_to + "/R/potential2.R                  ").c_str());
//   system((" cp " + path_from + "/R/probability.R                 " + path_to + "/R/probability.R                 ").c_str());
// };

// void remove_file_if_exist(std::string filename){
//   if(gFile.if_file_exist(filename)){
//     std::cout << " - removing file: >> " << filename << " <<" << std::endl;
//     system(("rm " + filename).c_str());
//   } else {
//     std::cout << " - file: >> " << filename << " does not exist" << std::endl;
//   };
// };






