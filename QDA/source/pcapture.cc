
// printf "\n\n\n\n\n"; g++ pcapture.cc -lgmpxx -lgmp -lmpfr -lgsl -lgslcblas -std=c++11 -o pcapture.exe 
// ./pcapture.exe -i ./conf/conf.2.txt  -o ./conf/conf.3.txt \
//                -e ./conf/prob.energies.txt -p ./conf/conf.potential.txt -r ./R/prob.R

#define ERROR(x) std::cout <<"ERROR: "<< __FILE__ << " : " << __LINE__  << " : " << x << std::endl;
#define DEBUG(x) std::cout <<"DEBUG: "<< __FILE__ << " : " << __LINE__  << " : " << x << std::endl;
#define REP(i,size) for(unsigned int i=0; i < size; ++i) 



#include "../lib/gFile.hh"
#include "../lib/gConf.hh"
#include "../lib/gFuns.hh"
#include "../lib/gMath.hh"
#include "../lib/potential.hh"

#include "../lib/nuclear.parameters.hh"   // 
#include "../lib/nuclear.function.hh"   // 

struct DeformProb{

  // gConf      conf_energies;
  // gConf     conf_potential;

  // MNParameters                       param;
  // std::vector <MPotential * >  v_potential;

  double Lmax, Eb, Rb, r0, beta1, beta2, A1, A2, koeff, mu; 
  double Z1, Z2, e, pi, R01, R02; 

  std::vector < double >    dataFF24;            // sigma [ Energy ] - Eb 
  std::vector < double >         vEd;   // vector of Energy 
  std::vector < double >        vEsp;  // short version vector of Energy 

  gsl_interp_accel * acc; 
  gsl_spline *    spline; 

  double FD1, FD2;

 
  DeformProb( gConf & conf_param)
  // :  conf_energies( conf_args.get("-e") )
  // , conf_potential( conf_args.get("-p") )
  // ,    v_potential(v_potential_from_file(conf_args.get("-p")))
  :          Lmax ( atof(conf_param.get("Lmax").c_str())     )
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

  gConf   conf_args(argc, argv);
  // conf.potential.txt - here load:  Lmax, v < MPotential * >  for each L = 0..L_max;
  gConf   conf_poten( conf_args.get("-p"));  
  gConf   conf_param( conf_args.get("-i"));   

  // Lmax
  double Lmax = atof(conf_poten.get("Lmax").c_str());
  // vector < MPotential * > for each L = 0 .. L_max:
  std::vector <MPotential * >  v_potential(v_potential_from_file(conf_args.get("-p")));


  // omega for Ein
  {
    MNParameters param(conf_args.get("-i"));
    REP(i,10) {
      double omega = param.omega.real();
      double Ecm = v_potential[0]->UBar + 15. + i*3;
      double tmpPELSS = nuclear::PELSS(Ecm, param, *(v_potential[0]),  &omega); 
      conf_param.change("omega", gFile.to_str(omega));  
      std::cout << "PELSS:  "  << tmpPELSS << "  omega:  "  << omega << std::endl ;
      std::cout << "param.omega: " << param.omega <<std::endl ;
      if(tmpPELSS > 0.9999) {
        break;
      }
    }
    conf_param.save(conf_args.get("-o"));
  }


  // Section for Ein
  {
    MNParameters param(conf_args.get("-i"));
    // std::vector < double > v_Energ(v_potential[0].build_energies());
    std::vector <double> v_Energ (gFuns.vs2vd(conf_param.get_all("Esp"  ) ));

    // std::vector < double > v_Energ(build_energies(conf_param, "expand")); 
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
        std::cout << j <<"/"<< v_Energ.size() <<" Ecm: " << Ecm <<" L: " << L << " Lmax: " << Lmax <<" PELSS: "<< gFile.format_to_str(v_pelss[L],12,"left") <<" sum: " << sum <<std::endl ;
       
        prob_file0 << gFile.format_to_str(v_pelss[L],12,"right") << std::flush;

        if(L > 0 && v_pelss[L] < 0.0000001 * v_pelss[0] ) break;

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
    // conf_param.save(conf_args.get("-o"));
  }


  // Deformed 
  DeformProb dfprob(conf_param);
  // DEBUG(dfprob.eval(122.));

  std::string tmp_messsage_previous = "None";

  
  //----------
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
  }


  std::string message0 = "sigma_d  ";
  std::string message1 = "Ed  ";
  for(size_t i=0; i < v_SS24.size(); ++i) {
    message0 = message0 + "  " +  gFile.to_str(v_SS24[i]);
  }
  for(size_t i=0; i < dfprob.vEd.size(); ++i) {
    message1 = message1 + "  " +  gFile.to_str(  dfprob.vEd[i] );
  }

  std::cout << message0  << std::endl;
  std::cout << message1  << std::endl;

  conf_param.erase("sigma_d");
  conf_param.add(message0);
  conf_param.save(conf_args.get("-o"));

  // gFile.write_file(message0 + "\n" + message1 + "\n" , conf_args.get("-o"));


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


}