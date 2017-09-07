// printf "\n\n\n\n\n\n\n"; g++ set_params.php.cc -o set_params.php.exe  -i conf.INP.txt -o conf.INP.txt
// printf "\n\n\n\n\n\n\n"; g++ set_params.php.cc -o set_params.php.exe  -A1 40 -Z1 20 -A2 40 -Z2 18 
//g++ -static -static-libgcc -static-libstdc++ set_params.php.cc -o w_set_params.php.exe
//g++ -static -static-libgcc -static-libstdc++ set_params.php.cc -o set_params.php.exe



#include <iostream>

#define ERROR(x) std::cout <<"ERROR: "<< __FILE__ << " : " << __LINE__  << " : " << x << std::endl;
#define DEBUG(x) std::cout <<"DEBUG: "<< __FILE__ << " : " << __LINE__  << " : " << x << std::endl;
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

// ------------------------------------------------------------------
struct MElement{
  const gConf & conf_table;
  int A,Z;
  float m;
  std::string Element;
  float b,b2,E2,d;
  bool exist;

  int                    indx_element;
  const vector <string>     &     v_A;
  const vector <string>     &     v_Z;
  const vector <string>     &     v_m;
  const vector <string>     &     v_N;
  const vector <string>     &     v_b;
  const vector <string>     &    v_b2;
  const vector <string>     &    v_E2;
  // const vector <string>     &     v_d;
  // const vector <string>     &  v_beta;
  std::vector <std::string> A_alternative;
  std::vector <std::string> Z_alternative;

  void print() {
    std::cout << Element << "[A = "<<gFile.format_to_str(A,3,"left")<<",Z = "<<gFile.format_to_str(Z,3,"left")<<"]  m: " << gFile.format_to_str(m,15,"left") 
              << "  beta: " << gFile.format_to_str(b,10,"left")
              << "  d: " << gFile.format_to_str(d,10,"left")
              << " beta[2+] : " << gFile.format_to_str(b2,10,"left") << " E[2+] : " << E2 << "  exist: " << exist << std::endl;
  };

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
  // ,      v_d( conf_table.get_all("d"      ))
  // ,   v_beta( conf_table.get_all("beta"   ))
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

  float diffussion(int z) {
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

};
// ------------------------------------------------------------------
// bool logger(gConf &_conf_args, std::string message, std::string key="MESSAGE") {
//   if(_conf_args.is_present("-log")) {
//     gConf conf_control(_conf_args.get("-log"));
//     conf_control.change(key,message);
//     conf_control.save();
//     return 1;
//   };
//   return 0;
// }
// ------------------------------------------------------------------



int main(int argc, char** argv) {

  gConf conf_args(argc, argv);
  string filename = (conf_args.is_present("-t")) ? conf_args.get("-t") : "./isotopes.table.2.txt";
  gConf conf_table;  conf_table.read_table(filename);
  
  gConf conf;
  // if(conf_args.is_present("-i")) conf.read_from_file(conf_args.get("-i"));
  conf.add("A1   " + conf_args.get("-A1"));
  conf.add("Z1   " + conf_args.get("-Z1"));
  conf.add("A2   " + conf_args.get("-A2"));
  conf.add("Z2   " + conf_args.get("-Z2"));
  conf.add("message         nan ");
  conf.add("message01       nan ");
  conf.add("is_Q2_channel   nan ");
  conf.add("Q               nan ");


  std::map <std::string, MElement> element;

  MElement XX = MElement(atoi(conf.get("A1").c_str()), atoi(conf.get("Z1").c_str()), conf_table);
  MElement YY = MElement(atoi(conf.get("A2").c_str()), atoi(conf.get("Z2").c_str()), conf_table);
  MElement X1 = MElement(XX.A + 2, XX.Z, conf_table);
  MElement Y1 = MElement(YY.A - 2, YY.Z, conf_table);
  MElement X2 = MElement(XX.A - 2, XX.Z, conf_table);
  MElement Y2 = MElement(YY.A + 2, YY.Z, conf_table);

  MElement * new_X = NULL;
  MElement * new_Y = NULL;
  
  float Q  = NAN;
  float E2 = NAN;

  if(XX.exist  && YY.exist) {
    if(conf_args.is_present("-print") && conf_args.get("-print") == "console" ) {
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
      if(conf_args.is_present("-print") && conf_args.get("-print") == "console" ) {
        std::cout << "Q: " << tmpQ  $ "MeV E[2+]: "  $ tmpE2 << "MeV " << std::endl;
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
      if(conf_args.is_present("-print") && conf_args.get("-print") == "console" ) {
        std::cout << "Q: " <<  tmpQ  $ "MeV  E[2+]: " $ tmpE2 << "MeV " << std::endl;
        X2.print();
        Y2.print();
      }
    }


    if(XX.Z > 90 || YY.Z > 90) conf.add("gamma   24.0");
    else                       conf.add("gamma   12.0");

    



    conf.add( "m1   "           +     gFile.format_to_str(XX.m          ,10,"right") ) ;
    conf.add( "m2   "           +     gFile.format_to_str(YY.m          ,10,"right") ) ;
    conf.add( "Element1   "     +     gFile.format_to_str(XX.Element    ,10,"right") ) ;
    conf.add( "Element2   "     +     gFile.format_to_str(YY.Element    ,10,"right") ) ;
    conf.add( "beta1   "        +     gFile.format_to_str(XX.b          ,10,"right") ) ;
    conf.add( "beta2   "        +     gFile.format_to_str(YY.b          ,10,"right") ) ;
    conf.add( "d1   "           +     gFile.format_to_str(XX.d          ,10,"right") ) ;
    conf.add( "d2   "           +     gFile.format_to_str(YY.d          ,10,"right") ) ;
    std::string reaction = gFile.format_to_str(XX.A,3,"right") + XX.Element + "+" + YY.Element + gFile.format_to_str(YY.A,3,"left");
    conf.add( "reaction   " + reaction    ) ;
    if(new_X != NULL) {
      if(conf_args.is_present("-print") && conf_args.get("-print") == "console" ) {
        std::cout <<" Q2n channel found: Q: " << Q << " E[2+]: " << E2 << std::endl;
        new_X->print();
        new_Y->print();
      }
      conf.change( "Q",                gFile.format_to_str( Q            ,10,"right")    ) ;
      conf.add( "E2   "           +    gFile.format_to_str( E2           ,10,"right")    ) ;
      conf.add( "A1_new   "       +    gFile.format_to_str(new_X->A      ,10,"right")    ) ;
      conf.add( "A2_new   "       +    gFile.format_to_str(new_Y->A      ,10,"right")    ) ;
      conf.add( "m1_new   "       +    gFile.format_to_str(new_X->m      ,10,"right")    ) ;
      conf.add( "m2_new   "       +    gFile.format_to_str(new_Y->m      ,10,"right")    ) ;
      conf.add( "beta1_new   "    +    gFile.format_to_str(new_X->b      ,10,"right")    ) ;
      conf.add( "beta2_new   "    +    gFile.format_to_str(new_Y->b      ,10,"right")    ) ;
      conf.add( "d1_new   "       +    gFile.format_to_str(new_X->d      ,10,"right")    ) ;
      conf.add( "d2_new   "       +    gFile.format_to_str(new_Y->d      ,10,"right")    ) ;
      conf.add( "Element1_new   " +    gFile.format_to_str(new_X->Element,10,"right")    ) ;
      conf.add( "Element2_new   " +    gFile.format_to_str(new_Y->Element,10,"right")    ) ;
      std::string reaction_new = gFile.format_to_str(new_X->A,3,"right") + new_X->Element + "+" + new_Y->Element + gFile.format_to_str(new_Y->A,3,"left");
      conf.add( "reaction_new   " + reaction_new    ) ;
    }    
    // if(conf_args.is_present("-o")) conf.save(conf_args.get("-o"));
  } else {
    // std::string message = "nan";
    conf.change("message",    "undefined_isotopes,_please_try_another_one");
    // logger(conf_args, "undefined_isotopes,_please_try_another_one", "10MESSAGE");
    // logger(conf_args, "01", "09STEP");


    std::string message01 = "" ;
    if(!XX.exist)  message01 = message01 + "<br>for_A_=_"+gFile.to_str(XX.A)+"_select_Z_from_{_" +gFile.vs2s(XX.Z_alternative,"_")+ "_}_" 
                          + "<br>for_Z_=_"+gFile.to_str(XX.Z)+"_select_A_from_{_" +gFile.vs2s(XX.A_alternative,"_")+ "_}_";
    if(!YY.exist)  message01 = message01 + "<br>for_A_=_"+gFile.to_str(YY.A)+"_select_Z_from_{_" +gFile.vs2s(YY.Z_alternative,"_")+ "_}_" 
                          + "<br>for_Z_=_"+gFile.to_str(YY.Z)+"_select_A_from_{_" +gFile.vs2s(YY.A_alternative,"_")+ "_}_"; 
    // ERROR("undefined isotopes, please try another one")
    // ERROR(gFile.replace(gFile.replace(message01,"<br>","\n"),"_"," "))
    conf.change("message01" , message01);
    // logger(conf_args, message01 , "10MESSAGE01");
    // logger(conf_args, "undefined_isotopes,_please_try_another_one", "10MESSAGE02");
  }

  if(conf_args.is_present("-print") && conf_args.get("-print") == "conf"  ) conf.print();  

  return 0;
}

