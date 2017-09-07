
// set PATH=D:\ProgramFiles\MinGW3264\mingw64\bin;%PATH%
// g++ not_spherical.cc -static -static-libgcc -static-libstdc++ -I"d:/ProgramFiles/MinGW3264/mingw64/include/" -L"d:/ProgramFiles/MinGW3264/mingw64/lib/" -lgsl -lgslcblas -lm -o w_not_spherical.exe


// printf "\n\n\n\n\n"; g++ not_spherical.cc -lgmpxx -lgmp -lmpfr -lgsl -lgslcblas -std=c++11 -o not_spherical.exe 
// ./not_spherical.exe -i ../conf.

//  h='../conf'; f='not_spherical'; p=' -lgsl -lgslcblas -std=c++11';  n='printf \n\n\n\n\n\n\n\n\n'; 
// $n; g++ $f.cc $p -o $f.exe && ./$f.exe -i $h/conf.prob.txt -o $h/conf.$f.txt -p $h/conf.potential.txt -e $h/conf.prob.energies.txt


// LD_LIBRARY_PATH='/usr/local/lib'
// export LD_LIBRARY_PATH;


#define ERROR(x) std::cout <<"ERROR: "<< __FILE__ << " : " << __LINE__  << " : " << x << std::endl;
#define DEBUG(x) std::cout <<"DEBUG: "<< __FILE__ << " : " << __LINE__  << " : " << x << std::endl;
#define  REP(i,size) for(unsigned int i=0; i < size; ++i) 


#include "../lib/gFile.hh"
#include "../lib/gConf.hh"
#include "../lib/gFuns.hh"
#include "../lib/potential.hh"

struct DeformProb{

  gConf     conf_param;
  gConf    conf_energies;
  gConf   conf_potential;

  // MNParameters                       param;
  std::vector <MPotential * >  v_potential;

  double Lmax, Eb, Rb, r0, beta1, beta2, A1, A2, koeff, mu; 
  double Z1, Z2, e, pi, R01, R02; 

  std::vector < double >    dataFF24;            // sigma [ Energy ] - Eb 
  std::vector < double >         vEd;   // vector of Energy 
  std::vector < double >        vEsp;  // short version vector of Energy 

  gsl_interp_accel * acc; 
  gsl_spline *    spline; 

  double FD1, FD2;

 
  DeformProb(const gConf & conf_args)
  :   conf_param( conf_args.get("-i") )
  ,      conf_energies( conf_args.get("-e") )
  , conf_potential( conf_args.get("-p") )
  ,    v_potential(v_potential_from_file(conf_args.get("-p")))
  ,          Lmax ( atof(conf_param.get("Lmax").c_str())     )
  //            0       1      2       3        4      5        6        7     8       9        10        ...
  //  L=0     xMin:  9.97271  xBar:  11.4825  UMin:  121.076  UBar:    127.5  UU:    144.348   133.665    ... 
  ,            Eb (  atof(conf_param.get("UBar" ).c_str())   ) 
  ,            Rb (  atof(conf_param.get("xBar" ).c_str())   )
  ,            r0 (  atof(conf_param.get("r0"       ).c_str())   )
  ,         beta1 (  atof(conf_param.get("beta1"    ).c_str())   )
  ,         beta2 (  atof(conf_param.get("beta2"    ).c_str())   )
  ,            A1 (  atof(conf_param.get("A1"       ).c_str())   )
  ,            A2 (  atof(conf_param.get("A2"       ).c_str())   )
  ,         koeff (  atof(conf_param.get("k"        ).c_str())   )
  ,            mu (   A1 * A2 / ( A1 + A2 ) *koeff      )
  ,            Z1 (  atof(conf_param.get("Z1"      ).c_str())     )
  ,            Z2 (  atof(conf_param.get("Z2"      ).c_str())     )
  ,             e (  atof(conf_param.get("e"       ).c_str())     )
  ,            pi (   3.141592654    )
  ,           R01 (  r0 * std::pow(A1,1./3.)     )
  ,           R02 (  r0 * std::pow(A2,1./3.)     )
  ,           vEsp(  gFile.vs2vd(conf_param.get_all("Esp")) )
  ,       dataFF24(  gFile.vs2vd(conf_param.get_all("sigma_sp")) )
  ,            vEd(  gFile.vs2vd(conf_param.get_all("Ed")) )
  // ,   vEsp(build_energies(conf_param, "expand"))
  // ,            vEd(build_energies(conf_param, ""))
  , acc(gsl_interp_accel_alloc ()) 
  , spline(gsl_spline_alloc (gsl_interp_cspline, vEsp.size()))  
  , FD1(1./std::sqrt(20.*pi)*(Z1*Z2 *e*e*R01)/Rb/Rb*(-5.+(3.*R01*(1.+2./7.*std::sqrt(5./pi)*beta1))/Rb))
  , FD2(1./std::sqrt(20.*pi)*(Z1*Z2 *e*e*R02)/Rb/Rb*(-5.+(3.*R02*(1.+2./7.*std::sqrt(5./pi)*beta2))/Rb))

  {
 
    // // sigma expanded
    // for(unsigned i = 0; i < vEsp.size(); ++i ) {
    //   double Ecm =  vEsp[i];
    //   std::string sE = gFile.to_str( int(Ecm) );
    //   dataFF24.push_back(atof(conf_energies.get("sigma_" + sE , 1).c_str() ));
    // }

    // interpolation V_E_expanded of sigma
    size_t size = dataFF24.size();
    double   x[size];
    double   y[size];
    for(size_t i=0; i<size; ++i) { x[i] = vEsp[i] - Eb; } 
    for(size_t i=0; i<size; ++i) { y[i] = dataFF24[i]; } 
    gsl_spline_init(spline,x,y,size);


  };

  //----------------------------------------------------------------------------
  //
  double eval(double xi) {
      if(dataFF24.size()<=2)           ERROR("dataFF24 has incorrect size = " << dataFF24.size())
      if(xi< vEsp[0] - Eb)  {
          // ERROR(" xi = " << xi << " is less than " << vEsp[0] - Eb );
          // xi = vEsp[0] - Eb;
          xi = NAN;
      }   
      if(xi> vEsp.back() - Eb)  {
          // ERROR(" xi = " << xi << " is more than " << vEsp.back() - Eb );
          // xi = vEsp.back() - Eb;
          xi = NAN;
      }
      return gsl_spline_eval (spline, xi, acc);
      // return 0;
  };

  double EbD(double theta1, double theta2) {
    return Eb + FD1 * beta1 / 2. * (3. * std::cos(theta1) *  std::cos(theta1) - 1.) + 
                FD2 * beta2 / 2. * (3. * std::cos(theta2)  * std::cos(theta2) - 1.);
  };
};


int main(int argc, char** argv) {

  gConf        conf_args(argc, argv);

  DeformProb dfprob(conf_args);
  // DEBUG(dfprob.eval(122.));

  std::string tmp_messsage_previous = "None";

  std::vector < double > v_SS24;
  for(size_t i = 0; i < dfprob.vEd.size(); ++i) {
    double ss24 = 0;
    for(double m = 0; m <= 0.5 * dfprob.pi; m = m + 0.01) {
      for(double k = 0; k <= 0.5 * dfprob.pi; k = k + 0.01) {
        // ss24 =  ss24 + 0.001 * std::sin(k) * 0.001 * std::sin(m)*(dfprob.vEd[i] );
        ss24 = ss24 + 0.01 * std::sin(k) * 0.01 * std::sin(m)*dfprob.eval(dfprob.vEd[i] - dfprob.EbD(m,k));
        // if()
        // DEBUG("k: " << k << " m: " <<m <<" FF24: "  << dfprob.eval(dfprob.vEd[i] - dfprob.EbD(m,k)) <<" Ed: " << dfprob.vEd[i] <<" EbD[m,k]: " << dfprob.EbD(m,k))
      }
    }
    v_SS24.push_back(ss24);
    std::cout << " SS24: "  << i << "  Ein =  " << dfprob.vEd[i] << ": " << v_SS24[i] << std::endl;

    // control
    if(conf_args.is_present("-control")) {
      gConf conf_control(conf_args.get("-control"));
      if  (conf_control.get("COMMAND") == "stop") return 0;
      std::string tmp_messsage = gFile.to_str(i) + "/" + gFile.to_str( dfprob.vEd.size()) + "_E<sub>in</sub>_=_" + gFile.to_str(dfprob.vEd[i]) + "_sigma:_" + gFile.to_str(v_SS24[i]);
      conf_control.change("10MESSAGE","please,_wait");
      conf_control.change("10MESSAGE01",tmp_messsage_previous);
      conf_control.change("10MESSAGE02",tmp_messsage);
      conf_control.save();
      tmp_messsage_previous = tmp_messsage;
    } 



  }

  std::string message0 = "ss24  ";
  std::string message1 = "vEd(ss24)  ";
  for(size_t i=0; i < v_SS24.size(); ++i) {
    message0 = message0 + "  " +  gFile.to_str(v_SS24[i]);
  }
  for(size_t i=0; i < dfprob.vEd.size(); ++i) {
    message1 = message1 + "  " +  gFile.to_str(  dfprob.vEd[i] );
  }

  std::cout << message0  << std::endl;
  std::cout << message1  << std::endl;

  dfprob.conf_param.add(message0);
  dfprob.conf_param.add(message1);
  dfprob.conf_param.save(conf_args.get("-o"));

  gFile.write_file(message0 + "\n" + message1 + "\n" , conf_args.get("-o"));


  if(conf_args.is_present("-r")) {
    // plot v_sigma[v_Energy]
    std::string command = "";
    command = command  + "x<-c(" + gFile.vd2s(dfprob.vEd, ", ") + "); \n" 
    +                    "y<-c(" + gFile.vd2s(v_SS24, ", ") + "); \n"
    +                       "png('probability.deformed.png', height = 300, width = 600) \n"
    +                       "plot(x,y,log='y',type='o',col='red',main=expression(paste(sigma,'(E)')),xlab='r',ylab=expression(sigma)) \n"
    +                       "dev.off() \n";
    gFile.write_file(command, conf_args.get("-r"),  "w");
  }






  return 0;
}




