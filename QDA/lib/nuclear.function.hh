
#ifndef __n_u_c_l_e_a_r____
#define __n_u_c_l_e_a_r____



namespace nuclear {


  double                  FF( const MNParameters &p ) ;
  double                  GG( const MNParameters &p ) ;

  #include <stdlib.h>
  #include <stdio.h>
  #include <gsl/gsl_vector.h>
  #include <gsl/gsl_multiroots.h>

 // private:
  struct find_root_params { MNParameters *pp; /*= &p*/ };
  int                       FFGG (const gsl_vector * x, void * find_root_params, gsl_vector * f) ;

 // public: 

  int ffgg_find_root_print_state (size_t iter, gsl_multiroot_fsolver * s) ;
  std::vector <double> ffgg_roots(MNParameters &p, std::string option="" /*"print"*/) ;

  double PELSS(double Ein, MNParameters &p, MPotential &potential, double * _omega);
  
  void print(MNParameters &p, std::string option = "" /*"details"*/);


} ;



//-----------------
//

double nuclear::FF(const MNParameters &p) {

    return (p.lp).real();
}
//-----------------
//
double nuclear::GG(const MNParameters &p) {

    return (sqrt( -p.mw2/p.mu )).real();
}
//-----------------
//   
int nuclear::FFGG (const gsl_vector * x, void * find_root_params, gsl_vector * f) {
    MNParameters & p = *(((struct find_root_params *) find_root_params)->pp   );
    const double x0 = gsl_vector_get (x, 0);
    const double x1 = gsl_vector_get (x, 1);
    p.rebuild_for_new_lambda_and_epsilon(x0,x1);
    const double y0 = - FF(p) + p.Lp.real();
    const double y1 = - GG(p) + p.omega.real();
    gsl_vector_set (f, 0, y0);
    gsl_vector_set (f, 1, y1);
    return GSL_SUCCESS;
}
//-----------------
//   
int nuclear::ffgg_find_root_print_state (size_t iter, gsl_multiroot_fsolver * s) {
    printf ("iter = %3d x = % .8f % .8f   f(x) = % .3e % .3e\n",
            int(iter),
            gsl_vector_get (s->x, 0),
            gsl_vector_get (s->x, 1),
            gsl_vector_get (s->f, 0),
            gsl_vector_get (s->f, 1));
}
// -----------------
// 
std::vector <double> nuclear::ffgg_roots(MNParameters &p, std::string option/*=""*/ /*"print"*/) {
    const gsl_multiroot_fsolver_type *T;

    gsl_multiroot_fsolver *s;
    int status;
    size_t i, iter = 0;
    const size_t n = 2;
    struct find_root_params ffgg_param = {&p};

    gsl_multiroot_function f = {&FFGG, n, &ffgg_param};
    double x_init[2] = {0.1,  p.omega.real() };

    gsl_vector *x = gsl_vector_alloc (n);

    gsl_vector_set (x, 0, x_init[0]);
    gsl_vector_set (x, 1, x_init[1]);
    T = gsl_multiroot_fsolver_hybrids;
    s = gsl_multiroot_fsolver_alloc (T, 2);
    gsl_multiroot_fsolver_set (s, &f, x);
    if(option=="print") ffgg_find_root_print_state (iter, s);

    do {
        iter++;
        status = gsl_multiroot_fsolver_iterate (s);
        if(option=="print") ffgg_find_root_print_state (iter, s);
        if (status) /* check if solver is stuck */
        break;
        status = gsl_multiroot_test_residual (s->f, 1e-15);
    } while (status == GSL_CONTINUE && iter < 1000);

    std::vector <double> out;
    out.push_back(gsl_vector_get (s->x, 0));
    out.push_back(gsl_vector_get (s->x, 1));

    if(option=="print") printf ("status = %s\n", gsl_strerror (status));
    gsl_multiroot_fsolver_free (s);
    gsl_vector_free (x);

    return out;
}
//-----------------
//   


double nuclear::PELSS(double Ein, MNParameters &p, MPotential &potential, double * _omega=NULL ) {
        
    double xomega = p.rrbarer.real();
    double Eomega = potential.eval( potential.xBar + xomega );
    double Eful = Ein - potential.UBar;
      

      if (Ein <= potential.UMin) {
          double xE = potential.xMin;
          double xS = potential::eval_root(potential, Ein,potential.xBar + 1./*,"print"*/);
          double S = potential.u_integrate(Ein, double(p.mu.real()), xE, xS);
          p.omega = double(p.pi.real())*(potential.eval(potential.xBar)-Ein)/S;
        
      }else if (Ein <= Eomega) {
          double xE = potential::eval_root(potential, Ein,potential.xMin + 0.3/*,"print"*/);
          double xS = potential::eval_root(potential, Ein,potential.xBar + 0.3/*,"print"*/);
          double S = potential.u_integrate(Ein, double(p.mu.real()), xE, xS);
          p.omega = double(p.pi.real())*(potential.eval(potential.xBar)-Ein)/S;
      } else if (Ein > Eomega) {
          if (Eomega > potential.UMin) {
  	        double xE = potential::eval_root(potential, Eomega,potential.xMin + 0.3/*,"print"*/);
  	        double xS = potential::eval_root(potential, Eomega,potential.xBar + 0.3/*,"print"*/);
  	        double S = potential.u_integrate(Eomega, double(p.mu.real()), xE, xS);
  	        p.omega = double(p.pi.real())*(potential.eval(potential.xBar)-Eomega)/S;
        } else {
          double xE = potential.xMin;
	        double xS = potential::eval_root(potential, Eomega,potential.xBar + 1/*,"print"*/); ////////// ---???????????
	        double S = potential.u_integrate(Eomega, double(p.mu.real()), xE, xS);
	        p.omega = double(p.pi.real())*(potential.eval(potential.xBar)-Eomega)/S;
        }
    } 
    double PP = 0;
    if(Eful <= -40) {PP = 0; return PP; }
    if (Eful <= - (p.mu*p.omega*p.omega*p.rrS*p.rrS / 2.).real()) {
        std::vector <double> roots = ffgg_roots(p,"" /* or "print" */); //lambda,epsilon
        p.p0=0;
        p.q0=sqrt(-2.*Eful/double(p.mu.real())/p.omega/p.omega);
         
        // -----------------##############################################3
        PP = (1./2.)*  gsl_sf_erfc(  (   ( (p.br1*p.p0*(p.gamma + p.s1))/p.mu +   p.br1*p.q0*(2.*p.gamma*p.lambda*p.epsilon + p.s1*(p.gamma + p.s1) )    )
           /(4.*p.gamma*sqrt(  (p.lambda*p.T*p.epsilon*(p.br1*p.br1*p.ia*(p.gamma + p.s1)*(p.gamma + p.s1))) / (p.pi*p.mu) ) )  ).real()  );
        // -----------------##############################################3
        return(PP);
    } else if (Eful > - (p.mu*p.omega*p.omega*p.rrS*p.rrS / 2.).real()) {
        std::vector <double> roots = ffgg_roots(p,"" /* or "print" */); //lambda,epsilon
        p.p0= -sqrt(2.*double(p.mu.real())*(Eful+ double(p.mu.real())*p.omega*p.omega*p.rrS*p.rrS/2.));
        p.q0=p.rrS;
        // -----------------##############################################3
        PP = (1./2.)*  gsl_sf_erfc(  (   ( (p.br1*p.p0*(p.gamma + p.s1))/p.mu +   p.br1*p.q0*(2.*p.gamma*p.lambda*p.epsilon + p.s1*(p.gamma + p.s1) )    )
           /(4.*p.gamma*sqrt(  (p.lambda*p.T*p.epsilon*(p.br1*p.br1*p.ia*(p.gamma + p.s1)*(p.gamma + p.s1))) / (p.pi*p.mu) ) )  ).real()  );
        // -----------------##############################################3

        if(_omega != NULL) (*_omega) = p.omega.real();
        return(PP);
    }
    return -9999999999;
}

//-----------------
//   
void nuclear::print(MNParameters &p, std::string option /*"details"*/) {
    std::cout << " ------------- nuclear functions values:   --------------- " << std::endl;
    std::cout << " FF:  " <<  FF( p)  << std::endl  ;
    std::cout << " GG:  " <<  GG( p)  << std::endl  ;
    if(option=="details") {
      ffgg_roots(p, "print");
    } else {
      ffgg_roots(p);
    } 
      std::cout << "solution for equations FF(lambda, epsilon) = Lp and GG(lambda, epsilon) = omega: " << std::endl;
      std::cout << "      Lp: "       << p.Lp  << std::endl  ;
      std::cout << "   omega: "    << p.omega  << std::endl  ;
      std::cout << "  lambda: "   << p.lambda  << std::endl  ;
      std::cout << " epsilon: " << p.epsilon  << std::endl  ;
}




#endif
