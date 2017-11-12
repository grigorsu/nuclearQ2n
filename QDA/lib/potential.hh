
#ifndef __Potential__
#define __Potential__


#include <complex>

#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>

#include <stdlib.h>
#include <stdio.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_multiroots.h>


struct MPotential {
     public:
        const int           size;  // the size of RR and UU
        double              RMin;
        double              RBar;  // Barier
        double              VMin;
        double              VBar;
        std::vector<double> RR;
        std::vector<double> UU;
        double x_range_left;
        double x_range_rght;

     private:
        
        gsl_interp_accel * acc;
        gsl_spline *    spline; 

     public:
        double eval(double xi);
        double x_min(double xA, double xB, double accuracy = 100, int iter = 10);
        double x_max(double xA, double xB, double accuracy = 100, int iter = 5);
        double min(double a, double b) { return a < b ? a : b; }
        double max(double a, double b) { return a > b ? a : b; }


        MPotential(std::vector<double> &_RR, const std::vector<double> &_UU ,  double _RMin=-1, double _RBar=-1, double _VMin=-1, double _VBar=-1 )
        : size(_RR.size())
        , acc(gsl_interp_accel_alloc ())
        , spline(gsl_spline_alloc (gsl_interp_cspline, size))
        {
            // DEBUG("tt")

            if(_RR.size() != _UU.size() || _RR.size() <=2 || _UU.size() <=2)
                std::cout <<"ERROR: "<< __FILE__ << " : " << __LINE__  
                << "_RR or _UU have incorrect size: _RR.size = " << _RR.size() << " _UU.size = " << _UU.size();
            // DEBUG("tt")
            x_range_left =  _RR[0];
            // DEBUG("tt")
            x_range_rght =  _RR[_RR.size()-1];
            // DEBUG("tt")
            double   x[_RR.size()];
            // DEBUG("tt")
            double   y[_UU.size()];
            // DEBUG("tt")
            for(size_t i=0; i<size; ++i) {RR.push_back(_RR[i]); x[i] = RR[i];} 
            // DEBUG("tt")
            for(size_t i=0; i<size; ++i) {UU.push_back(_UU[i]); y[i] = UU[i];} 
            // DEBUG("tt")

            // for(size_t i=0; i<size; ++i) {cout << " " << x[i];} cout << endl;  
            // for(size_t i=0; i<size; ++i) {cout << " " << y[i];} cout << endl;  
            gsl_spline_init(spline,x,y,size);
            if(_RMin == -1){
                size_t tmp_imin = 1;          for(size_t i=1;          i<size-1; ++i) { if(y[i+1] >= y[i] ) { tmp_imin = i-1  ; break; } }
                size_t tmp_imax = tmp_imin+1; for(size_t i=tmp_imin+1; i<size-1; ++i) { if(y[tmp_imax] <  y[i] ) tmp_imax = i; }

                RMin = x_min(x[tmp_imin], x[tmp_imax]);
                RBar = x_max(       RMin, x[size-1] );
                // RMin = x_min(x[0]  ,x[0]+3);
                // RBar = x_max(x[0]+2,x[0]+3);
                VMin = eval(RMin);
                VBar = eval(RBar);

            } else {
                RMin =  _RMin;
                RBar =  _RBar;
                VMin =  _VMin;
                VBar =  _VBar;
            }



        }

        ~MPotential() {
            gsl_spline_free (spline);
            gsl_interp_accel_free (acc);
        }

        double u_integrate(double Ein, double mu, double xA, double xB, double sum = 0, unsigned int iter = 10000 ) {
            double delta = (xB - xA) / iter;
            std::complex<double> tmp = 2*mu*(eval(xA)-Ein);
            double new_sum = sum + sqrt( tmp ).real() *delta;
            // ERROR(new_sum $ tmp.real() $ tmp.imag())
            double new_xA = xA + delta;
            if(iter == 1) return new_sum;
            return u_integrate(Ein, mu, new_xA, xB, new_sum, iter-1);
        }
};



//-----------------
//
// std::vector <MPotential *>  v_potential_from_file(std::string filename="./conf.potential.txt") {
std::vector <MPotential *>  v_potential_from_file(std::string filename /*="./conf.potential.txt"*/) {
  gConf  conf(filename);
  size_t Lmax = atoi(conf.get("Lmax").c_str());
  std::vector <double> RR (gFuns.vs2vd(conf.get_all("RR"  ) ));
  // std::vector <double> U0 (gFuns.vs2vd(conf.get_all("U0"  ) ));

  std::vector <double> UU (gFuns.vs2vd(conf.get_all("U0"  ) ));
  std::vector < MPotential * > v_potential;
  for(size_t i = 0; i <= Lmax; ++i) {
    std::vector <std::string> tmp_vs_conf = conf.get_all("L=" + gFile.to_str(i));
    std::vector <double> UU;
    for(size_t ii = 9; ii < tmp_vs_conf.size(); ++ii) {
      UU.push_back(atof(tmp_vs_conf[ii].c_str()));
    }  
    double  RMin = atof( tmp_vs_conf[1].c_str() );
    double  RBar = atof( tmp_vs_conf[3].c_str() );
    double  VMin = atof( tmp_vs_conf[5].c_str() );
    double  VBar = atof( tmp_vs_conf[7].c_str() );
    // MPotential * ppotential = new MPotential(RR, UU,  double  RMin, double  RBar, double  VMin, double  VBar);
    MPotential * ppotential = new MPotential(RR, UU,  RMin, RBar, VMin, VBar);
    v_potential.push_back(  ppotential );
  }
  return v_potential;
}


//----------------------------------------------------------------------------
//
double MPotential::eval(double xi) {
    if(RR.size()<=2)          
      std::cout <<"ERROR: "<< __FILE__ << " : " << __LINE__  << "RR has incorrect size = " << RR.size() << std::endl;
    if(xi< x_range_left)  {
      // std::cout <<"ERROR: "<< __FILE__ << " : " << __LINE__  << " xi = " << xi << " is less than " << x_range_left << std::endl;
        xi = x_range_left;
    }   
    if(xi> x_range_rght)  {
      // std::cout <<"ERROR: "<< __FILE__ << " : " << __LINE__  << "  xi = " << xi << " is more than " << x_range_rght << std::endl;
        xi = x_range_rght;
    }
    // return 0;
    return gsl_spline_eval (spline, xi, acc);
}

//--------------
//
double MPotential::x_min(double xA, double xB, double accuracy, int iter) {
    // DEBUG(xA $ xB)
    if(xA < x_range_left) xA = x_range_left;
    if(xB > x_range_rght) xB = x_range_rght;
    if(xB < xA) 
      std::cout <<"ERROR: "<< __FILE__ << " : " << __LINE__  << "xB = " << xB << " < xA = " << xA << std::endl;
    std::vector <double> y(1,eval(xA));
    std::vector <double> x(1,     xA );
    double delta = (xB-xA)/accuracy;
    for(double i=0; i < accuracy; ++i) {
        y.push_back(eval(xA+i*delta));    
        x.push_back(    (xA+i*delta));    
    }
    unsigned int imin = gFuns.indx_min_in_v(y);
    // REP(i,y.size()) if(x[i] >= xA && x[i] <= xB){ cout <<"   " << x[i] << ", " << y[i] ;   }    cout << endl;
    // printf("\n iter = %d, x_min = %.12f y_min = %.12f xB = %.5f xA = %.5f delta = %.5f \n", iter, x[imin], eval( x[imin]),  xB, xA, delta);
    if(iter==0) return x[imin];
    return x_min(max(xA, x[imin] - iter * delta), min(xB,x[imin] + iter*delta), accuracy, iter-1 ) ; 
}
//--------------
//
double MPotential::x_max(double xA, double xB, double accuracy, int iter) {
    std::vector <double> y(1,eval(xA));
    std::vector <double> x(1,     xA );
    double delta = (xB-xA)/accuracy;
    if(delta==0) return xB;
    for(double i=0; i < accuracy; ++i) {
        y.push_back(eval(xA+i*delta));    
        x.push_back(    (xA+i*delta));    
    }
    unsigned int imax = gFuns.indx_max_in_v(y);
    // REP(i,y.size()) if(x[i] >= xA && x[i] <= xB){ cout <<"   " << x[i] << ", " << y[i] ;   }    cout << endl;
    // printf("\n iter = %d, x_max = %.12f y_max = %.12f xB = %.5f xA = %.5f delta = %.5f \n", iter, x[imax], eval( x[imax]),  xB, xA, delta);
    if(iter==0) return x[imax];
    return x_max(max(xA, x[imax] - iter * delta), min(xB,x[imax] + iter*delta), accuracy, iter-1 ) ; 
}
//--------------
//
//--------------
//
//--------------
//
//--------------


namespace potential{
  struct find_root_params { double energy;  MPotential * p_potential;};
  // define function for find root
  int f_eval_root (const gsl_vector * x, void * find_root_params, gsl_vector * f){
   double energy =   (  ((struct find_root_params *) find_root_params)->energy ); 
   MPotential & potential = *(((struct find_root_params *) find_root_params)->p_potential   );
   const double x0 = gsl_vector_get (x, 0);
   const double y0 = - potential.eval(x0) + energy;
   gsl_vector_set (f, 0, y0);
  return GSL_SUCCESS;
  };
  int find_root_print_state (size_t iter, gsl_multiroot_fsolver * s) {
   printf ("iter = %3d x = % .8f  f(x) = % .3e \n",
           int(iter),
           gsl_vector_get (s->x, 0),
           gsl_vector_get (s->f, 0));
  } ;
  double eval_root(MPotential &potential, double _en, double _x0, std::string option="" /*"print"*/) {
     const gsl_multiroot_fsolver_type *T;
     gsl_multiroot_fsolver *s;
     int status;
     size_t i, iter = 0;
     const size_t n = 1;
     struct find_root_params param = {_en, &potential};
     gsl_multiroot_function f = {&f_eval_root, n, &param};
     double x_init[1] = {_x0};
     gsl_vector *x = gsl_vector_alloc (n);
     gsl_vector_set (x, 0, x_init[0]);
     T = gsl_multiroot_fsolver_hybrids;
     s = gsl_multiroot_fsolver_alloc (T, 1);
     gsl_multiroot_fsolver_set (s, &f, x);
     if(option=="print") find_root_print_state (iter, s);
     do {
         iter++;
         status = gsl_multiroot_fsolver_iterate (s);
         if(option=="print") find_root_print_state (iter, s);
         if (status) /* check if solver is stuck */
         break;
         status = gsl_multiroot_test_residual (s->f, 1e-15);
     } while (status == GSL_CONTINUE && iter < 1000);
     double out;
     out = gsl_vector_get (s->x, 0);
     if(option=="print") printf ("status = %s\n", gsl_strerror (status));
     gsl_multiroot_fsolver_free (s);
     gsl_vector_free (x);
     return out;
  }

}



//-----------------
// build spectr of Energies: for option = "deformed":     { Efrom-10, Efrom-9,...,Efrom, Efrom + Estep, Efrom + 2 Estep ,..., Eto, Eto+1,...,Eto+10}
// build spectr of Energies: for option = "spherical" (default): { Efrom, Efrom + Estep, Efrom + 2 Estep ,..., Eto }
std::vector <double> build_energies( MPotential & potential, const gConf & conf_param, std::string option) {
  std::vector <double> v_Energ;
  double pi = 3.141592654;
  double A1       = atof(conf_param.get("A1"   ).c_str());
  double A2       = atof(conf_param.get("A2"   ).c_str());
  double beta1    = atof(conf_param.get("beta1").c_str());
  double beta2    = atof(conf_param.get("beta2").c_str());
  double Z1       = atof(conf_param.get("Z1"   ).c_str());
  double Z2       = atof(conf_param.get("Z2"   ).c_str());
  double e        = atof(conf_param.get("e"    ).c_str()); // 1.2;
  double r0       = atof(conf_param.get("r0"   ).c_str()); 
  double R01      = r0 * pow(A1,1./3.) ;
  double R02      = r0 * pow(A2,1./3.) ;
  double Eb       = atof(conf_param.get("VBar"     ).c_str());
  double Rb       = atof(conf_param.get("RBar"     ).c_str());
  double FD1      = 1./std::sqrt(20.*pi)*(Z1*Z2 *e*e*R01)/Rb/Rb*(-5.+(3.*R01*(1.+2./7.*std::sqrt(5./pi)*beta1))/Rb) ;
  double FD2      = 1./std::sqrt(20.*pi)*(Z1*Z2 *e*e*R02)/Rb/Rb*(-5.+(3.*R02*(1.+2./7.*std::sqrt(5./pi)*beta2))/Rb) ;
  double rrS       = atof(conf_param.get("rrS"     ).c_str());
  double rrbarer       = atof(conf_param.get("rrbarer"     ).c_str());

  double EbD_00 =  Eb + FD1 * beta1 + FD2 * beta2 ;

  double Estart = potential.eval(Rb + rrS) - (Eb - EbD_00);
  // double Estart = potential.eval(Rb + rrS + 0.0) - (Eb - EbD_00);
  double Eto    = Eb + 20.;

  if(option == "deformed") {
    for(double ee = Estart;  ee < Eto  ; ee = ee + 0.5 ) {
    // for(double ee = Estart;  ee <= Eto  ; ee = ee + 1. ) {
      v_Energ.push_back(ee);   
    }

  } else if (option == "spherical") {
    for(double ee = Estart - (Eb - EbD_00)-1.; ee <= Eto + (Eb - EbD_00 + 1.0); ee = ee + 0.5 ) {
    // for(double ee = Estart - (Eb - EbD_00); ee <= Eto + (Eb - EbD_00); ee = ee + 1. ) {
      v_Energ.push_back(ee);   
    }
  } 
    // DEBUG( Estart <<" " <<  Eto )
    // DEBUG( Estart << " " << Eb << " " << EbD_00 << " " <<  Eto << " " << Eb << " " <<EbD_00 )
    // DEBUG(  " Eb: "  << Eb <<    " FD1: "  << FD1 <<    " beta1: "  << beta1 <<    " FD2: "  << FD2 <<    " beta2: " << beta2  )
  return v_Energ;
};


    

# endif    