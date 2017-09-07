#ifndef __p_r_o_j_e_c_t_f_u_n_c_t_i_o_n_s____
#define __p_r_o_j_e_c_t_f_u_n_c_t_i_o_n_s____


#include <iostream>
#include "gFile.hh"
#include "gConf.hh"

namespace nuclear {
  // returns diffusion by z
  float diffussion(int z);

  struct MElement{
    const gConf & conf_table;
    int A,Z;
    float m;
    std::string Element;
    float b,b2,E2,d;
    bool exist;

    int                    indx_element;
    const std::vector <std::string>     &     v_A;
    const std::vector <std::string>     &     v_Z;
    const std::vector <std::string>     &     v_m;
    const std::vector <std::string>     &     v_N;
    const std::vector <std::string>     &     v_b;
    const std::vector <std::string>     &    v_b2;
    const std::vector <std::string>     &    v_E2;
    std::vector <std::string> A_alternative;
    std::vector <std::string> Z_alternative;

    void print();

    MElement(int _A,int _Z, gConf _conf_table /*, float _m, std::string Element,float _b,float _b2,float _E2*/)
    : A(_A), Z(_Z), conf_table(_conf_table)
    , m(NAN), Element(""),b(NAN),b2(NAN),E2(NAN),d(diffussion(_Z))
    ,   v_A( conf_table.get_all("A"        ))
    ,   v_Z( conf_table.get_all("Z"        ))
    ,   v_m( conf_table.get_all("m"        ))
    ,   v_N( conf_table.get_all("Element"  ))
    ,   v_b( conf_table.get_all("BETA_GS"  ))
    ,  v_b2( conf_table.get_all("BETA_2+"  ))
    ,  v_E2( conf_table.get_all("E_2+(MeV)"))
    {
      
      int indx_element = -1;
      
      for(int ii = 0 /*title*/; ii < int(v_A.size()); ++ii) {
        if(atoi(v_A[ii].c_str()) == A && atoi(v_Z[ii].c_str()) == Z) {
          indx_element = ii;
          break;
        }
      }
      exist = (indx_element >= 0 ) ;
      if(exist) {
        m  = atof(  v_m[indx_element].c_str() );
        b  = atof(  v_b[indx_element].c_str() );
        b2 = atof( v_b2[indx_element].c_str() );
        E2 = atof( v_E2[indx_element].c_str() );
        Element = v_N[indx_element];
      } else {
        REP(ii,v_A.size()) {
          if (atoi(v_A[ii].c_str()) == A) Z_alternative.push_back(v_Z[ii]);
          if (atoi(v_Z[ii].c_str()) == Z) A_alternative.push_back(v_A[ii]);
        }
      }
    };
  };

  gConf make_default_conf();

  gConf set_params(gConf &_conf_args);

  void write_fort(gConf &_conf, std::string filename);
  
  void read_potential(std::vector<std::string> &R, std::vector<std::string> &En, gConf &conf );


  


}

//---------------------------------------------------------------------------


float nuclear::diffussion(int z) {
      if(    z == 2        ) return 0.48;
      if(    z == 3        ) return 0.50;
      if(    z == 4        ) return 0.52;
      if(    z == 5        ) return 0.53;
      if(    z == 6        ) return 0.53;
      if(    z == 7        ) return 0.53;
      if(    z == 8        ) return 0.53;
      if(    z == 9        ) return 0.53;
      if(z >=10 && z <= 89 ) return 0.55;
      if(    z >= 90       ) return 0.56;
      return 0.55;
};

void nuclear::MElement::print() {
      std::cout << Element 
                << "[A = "<<gFile.format_to_str(A,3,"left")
                <<",Z = "<<gFile.format_to_str(Z,3,"left")
                <<"]  m: " << gFile.format_to_str(m,15,"left") 
                << "  beta: " << gFile.format_to_str(b,10,"left")
                << "  d: " << gFile.format_to_str(d,10,"left")
                << " beta[2+] : " << gFile.format_to_str(b2,10,"left") 
                << " E[2+] : " << E2 
                << "  exist: " << exist << std::endl;
};

gConf nuclear::make_default_conf() {
  gConf conf;
   
  conf.add(  "Z1                 nan"                            );
  conf.add(  "Z2                 nan"                            );
  conf.add(  "e                  1.2"                            );
  conf.add(  "A1                 nan"                            );
  conf.add(  "A2                 nan"                            );
  conf.add(  "k                  0.024"                          );
  
  conf.add(  "Element1           nan"                            );
  conf.add(  "Element2           nan"                            );
  conf.add(  "reaction           nan"                            );
  conf.add(  "beta1              nan"                            );
  conf.add(  "beta2              nan"                            );
  conf.add(  "m1                 nan"                            );
  conf.add(  "m2                 nan"                            );
  conf.add(  "message            nan"                            );
  
  conf.add(  "Q                  nan"                            );
  conf.add(  "E2                 nan"                            );
  conf.add(  "A1_new             nan"                            );
  conf.add(  "A2_new             nan"                            );
  conf.add(  "m1_new             nan"                            );
  conf.add(  "m2_new             nan"                            );
  conf.add(  "beta1_new          nan"                            );
  conf.add(  "beta2_new          nan"                            );
  conf.add(  "d1_new             nan"                            );
  conf.add(  "d2_new             nan"                            );
  conf.add(  "Element1_new       nan"                            );
  conf.add(  "Element2_new       nan"                            );
  conf.add(  "reaction_new       nan"                            );
  conf.add(  "is_Q2_channel      nan"                            );
  
  conf.add(  "d1                 nan"                            );
  conf.add(  "d2                 nan"                            );
  
  conf.add(  "r0                 1.150"                          );
  conf.add(  "ro0                0.17"                           );
  
  conf.add(  "gamma              nan"                            );
  conf.add(  "lambda             nan"                            );
  conf.add(  "epsilon            nan"                            );


  conf.add(  "q0                 nan"                            );
  conf.add(  "p0                 nan"                            );
  conf.add(  "omega              nan"                            );
  // need for potential.hh rrbarer and rrS
  conf.add(  "rrbarer            0.5"                            );
  conf.add(  "rrS                1.1"                            );

  // need for prob.cc < CHECK!!  
  conf.add(  "T                  0.7"                            );
  conf.add(  "tlim               6.0"                            );
  conf.add(  "Lp                 2.0015"                         );
  // conf.add(  "T                  0.7"                            );
  // conf.add(  "tlim               6.0"                            );


    // potential for L=0:
  conf.add(  "R                  nan"                            );
  conf.add(  "En                 nan"                            );
    // potential properties:
  conf.add(  "Lmax               nan"                            );
  conf.add(  "xMin               nan"                            );
  conf.add(  "xBar               nan"                            );
  conf.add(  "UMin               nan"                            );
  conf.add(  "UBar               nan"                            );
  
  conf.add(  "sigma_sp           nan"                            );
  conf.add(  "sigma_d            nan"                            );
  conf.add(  "Esp                nan"                            );
  conf.add(  "Ed                 nan"                            );
  conf.add(  "pelss              nan"                            );
  
  conf.add(  "val_D_3            -0.10"                          );
  conf.add(  "val_E_1            0.00001"                        );
  conf.add(  "val_E_2            30.0"                           );
  conf.add(  "val_E_3            0.001"                          );
  conf.add(  "val_E_4            0.00001"                        );
  conf.add(  "val_F_1            34"                             );
  conf.add(  "val_G_1            1"                              );
  conf.add(  "val_G_2            1"                              );
  conf.add(  "val_H_1            0.000"                          );
  conf.add(  "val_H_2            3.140"                          );
  conf.add(  "val_H_3            0.0000"                         );
  conf.add(  "val_H_4            0.0000"                         );
  conf.add(  "val_H_5            0.001"                          );
  conf.add(  "val_I_1            0.00"                           );
  conf.add(  "val_I_2            0.00"                           );
  conf.add(  "val_I_3            0."                             );
  conf.add(  "val_J_1            0.440"                          );
  conf.add(  "val_J_2            3.00"                           );
  conf.add(  "val_J_3            0.450"                          );
  conf.add(  "val_K_1            300.00"                         );
  conf.add(  "val_K_2            0.09"                           );
  conf.add(  "val_K_3            0.42"                           );
  conf.add(  "val_K_4            -2.59"                          );
  conf.add(  "val_K_5            0.54"                           );
  conf.add(  "val_L_1            0.55"                           );
  conf.add(  "val_L_2            0.99"                           );
  conf.add(  "val_L_3            0.56"                           );
  conf.add(  "val_L_4            1.00"                           );
  conf.add(  "val_M_1            0.59"                           );
  conf.add(  "val_M_2            1.02"                           );
  conf.add(  "val_M_3            0.60"                           );
  conf.add(  "val_M_4            1.11"                           );
  conf.add(  "FORT.DAT            nan"                );
  return conf;
}

gConf nuclear::set_params(gConf &_conf_args) {
    

    std::string file_isotopes_table = (_conf_args.is_present("-t")) ? _conf_args.get("-t") : "../conf/isotopes.table.txt";
    // std::string file_conf_input     = (_conf_args.is_present("-i")) ? _conf_args.get("-i") : "../conf/conf.INP.default.txt";

    gConf conf_isotopes_table;    conf_isotopes_table.read_table(file_isotopes_table);
    // For DEBUG ONLY:
    // if(_conf_args.is_present("-print") && _conf_args.get("-print") == "table" ) conf_isotopes_table.print();
    
    gConf conf(make_default_conf());

   

    conf.change("A1" , _conf_args.get("-A1"));
    conf.change("Z1" , _conf_args.get("-Z1"));
    conf.change("A2" , _conf_args.get("-A2"));
    conf.change("Z2" , _conf_args.get("-Z2"));


    std::map <std::string, MElement> element;

    MElement XX = MElement(atoi(conf.get("A1").c_str()), atoi(conf.get("Z1").c_str()), conf_isotopes_table);
    MElement YY = MElement(atoi(conf.get("A2").c_str()), atoi(conf.get("Z2").c_str()), conf_isotopes_table);
    MElement X1 = MElement(XX.A + 2, XX.Z, conf_isotopes_table);
    MElement Y1 = MElement(YY.A - 2, YY.Z, conf_isotopes_table);
    MElement X2 = MElement(XX.A - 2, XX.Z, conf_isotopes_table);
    MElement Y2 = MElement(YY.A + 2, YY.Z, conf_isotopes_table);

    MElement * new_X = NULL;
    MElement * new_Y = NULL;
    
    float Q  = NAN;
    float E2 = NAN;

    if(XX.exist  && YY.exist) {
      if(_conf_args.is_present("-print") && _conf_args.get("-print") == "console" ) {
        std::cout << std::endl;
        XX.print();
        YY.print();
      }
      if( X1.exist  && Y1.exist ) {
        float tmpQ  = (XX.m + YY.m - X1.m - Y1.m)/1000.  /*   1000 is here because we transfer KeV to MeV */      ;
        float tmpE2 = X1.E2 /*who recieves neutrons,  it's in MeV  */ ;
        if(tmpQ > tmpE2) { 
          new_X = &X1; new_Y = &Y1; 
          new_X->b = new_X->b2;
          Q = tmpQ; E2 = tmpE2;
        }
        if(_conf_args.is_present("-print") && _conf_args.get("-print") == "console" ) {
          std::cout << std::endl << "Q: " << tmpQ  $ "MeV E[2+]: "  $ tmpE2 << "MeV " << std::endl;
          X1.print();
          Y1.print();
        } 
      };
      if( X2.exist  && Y2.exist ) {
        float tmpQ  = (XX.m + YY.m - X2.m - Y2.m)/1000.  /*  1000 is here because we transfer KeV to MeV  */      ;
        float tmpE2 = Y2.E2 /*who recieves neutrons, it's in MeV   */ ;
        if(tmpQ > tmpE2) { 
          new_X = &X2; new_Y = &Y2; 
          new_Y->b = new_Y->b2;
          Q = tmpQ; E2 = tmpE2;
        } 
        if(_conf_args.is_present("-print") && _conf_args.get("-print") == "console" ) {
          std::cout << std::endl << "Q: " <<  tmpQ  $ "MeV  E[2+]: " $ tmpE2 << "MeV " << std::endl;
          X2.print();
          Y2.print();
        }
      }


      if(XX.Z > 90 || YY.Z > 90) conf.change("gamma",   "24.0");
      else                       conf.change("gamma",   "12.0");
      conf.change( "m1"              ,     gFile.format_to_str(XX.m          ,10,"right") ) ;
      conf.change( "m2"              ,     gFile.format_to_str(YY.m          ,10,"right") ) ;
      conf.change( "Element1"        ,     gFile.format_to_str(XX.Element    ,10,"right") ) ;
      conf.change( "Element2"        ,     gFile.format_to_str(YY.Element    ,10,"right") ) ;
      conf.change( "FORT.DAT"        ,     gFile.format_to_str(XX.Element + YY.Element + ".DAT"    ,20,"right") ) ;
      conf.change( "beta1"           ,     gFile.format_to_str(XX.b          ,10,"right") ) ;
      conf.change( "beta2"           ,     gFile.format_to_str(YY.b          ,10,"right") ) ;
      conf.change( "d1"              ,     gFile.format_to_str(XX.d          ,10,"right") ) ;
      conf.change( "d2"              ,     gFile.format_to_str(YY.d          ,10,"right") ) ;
      std::string reaction = gFile.format_to_str(XX.A,3,"right") + XX.Element + "+" + YY.Element + gFile.format_to_str(YY.A,3,"left");
      conf.change( "reaction",  reaction    ) ;
      if(new_X != NULL) {
        if(_conf_args.is_present("-print") && _conf_args.get("-print") == "console" ) {
          std::cout << std::endl <<" Q2n channel found: Q: " << Q << " E[2+]: " << E2 << std::endl;
          new_X->print();
          new_Y->print();
        }
        conf.change( "Q",                gFile.format_to_str( Q            ,10,"right")    ) ;
        conf.change( "E2"              ,    gFile.format_to_str( E2           ,10,"right")    ) ;
        conf.change( "A1_new"          ,    gFile.format_to_str(new_X->A      ,10,"right")    ) ;
        conf.change( "A2_new"          ,    gFile.format_to_str(new_Y->A      ,10,"right")    ) ;
        conf.change( "m1_new"          ,    gFile.format_to_str(new_X->m      ,10,"right")    ) ;
        conf.change( "m2_new"          ,    gFile.format_to_str(new_Y->m      ,10,"right")    ) ;
        conf.change( "beta1_new"       ,    gFile.format_to_str(new_X->b      ,10,"right")    ) ;
        conf.change( "beta2_new"       ,    gFile.format_to_str(new_Y->b      ,10,"right")    ) ;
        conf.change( "d1_new"          ,    gFile.format_to_str(new_X->d      ,10,"right")    ) ;
        conf.change( "d2_new"          ,    gFile.format_to_str(new_Y->d      ,10,"right")    ) ;
        conf.change( "Element1_new"    ,    gFile.format_to_str(new_X->Element,10,"right")    ) ;
        conf.change( "Element2_new"    ,    gFile.format_to_str(new_Y->Element,10,"right")    ) ;
        conf.change( "FORT.DAT"        ,     gFile.format_to_str(new_X->Element + new_Y->Element + ".DAT"    ,20,"right") ) ;
        std::string reaction_new = gFile.format_to_str(new_X->A,3,"right") + new_X->Element + "+" + new_Y->Element + gFile.format_to_str(new_Y->A,3,"left");
        conf.change( "reaction_new"    , reaction_new    ) ;
      }    
    } else {
      // std::string message = "nan";
      conf.change("message",    "undefined isotopes, please try another one");

      std::string message01 = "" ;
      if(!XX.exist)  message01 = message01 + "for A = "+gFile.to_str(XX.A)+" select Z from { " +gFile.vs2s(XX.Z_alternative," ")+ " } \n" 
                                           + "for Z = "+gFile.to_str(XX.Z)+" select A from { " +gFile.vs2s(XX.A_alternative," ")+ " } \n";
      if(!YY.exist)  message01 = message01 + "for A = "+gFile.to_str(YY.A)+" select Z from { " +gFile.vs2s(YY.Z_alternative," ")+ " } \n" 
                                           + "for Z = "+gFile.to_str(YY.Z)+" select A from { " +gFile.vs2s(YY.A_alternative," ")+ " } \n"; 
      if(_conf_args.is_present("-print") && _conf_args.get("-print") == "console") 
        std::cout << gFile.vs2s(conf.get_all("message"), " ") << std::endl  << message01 << std::endl;      
    }

     if(_conf_args.is_present("-q") && ( conf.get("Q") != "nan") && conf.getf("Q") > 0  ) { 
      conf.change("is_Q2_channel", "yes");
    }
    if(_conf_args.is_present("-o")) conf.save(_conf_args.get("-o"));
    if(_conf_args.is_present("-print") && _conf_args.get("-print") == "conf"  ) conf.print();  
    return conf;
};

void nuclear::write_fort(gConf &_conf, std::string filename) {
  std::string Q2n_tail = (_conf.get("is_Q2_channel") == "yes") ? "_new" : "" ;
  std::string content;

  char buffer [40];
  sprintf( buffer,            "  %s\n",     _conf.get( "reaction"+Q2n_tail).c_str()   ); content += buffer; //  "22Ne+238U"
  sprintf( buffer,          " %03d.00",(int)_conf.getf("A1"+Q2n_tail)           ); content += buffer; //   048.00
  sprintf( buffer,       "    %03d.00",(int)_conf.getf("A2"+Q2n_tail)           ); content += buffer; //   122.00
  sprintf( buffer,      "     %03d.00",(int)_conf.getf("Z1"         )           ); content += buffer; //   022.00
  sprintf( buffer,     "    %03d.00\n",(int)_conf.getf("Z2"         )           ); content += buffer; //   050.00
  sprintf( buffer,         "%.2f00111",     _conf.getf("d1"+Q2n_tail)           ); content += buffer; //   0.5500111
  sprintf( buffer,    "    %.2f0111\n",     _conf.getf("d2"+Q2n_tail)           ); content += buffer; //   0.550111
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
  sprintf( buffer,                "%s",     _conf.get( "FORT.DAT").c_str()      ); content += buffer; //  "HEU.DAT"
  gFile.write_file(content, filename );
};

void nuclear::read_potential(std::vector<std::string> &R, std::vector<std::string> &En, gConf &conf ){
  std::string filename0 = conf.get("FORT.DAT");
  std::vector <double> _R, En1, En2, _En;

  // read from FORT.DAT
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


#endif
