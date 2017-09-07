
#ifndef __Nuclear___Parameters__
#define __Nuclear___Parameters__



#include <gsl/gsl_sf_psi.h>
#include <gsl/gsl_sf_erf.h>
#include "digamma.hh"

struct MNParameters {

    std::complex  <double>  pi;
    std::complex <double>    I;
    gConf conf;
    std::complex  <double>  A1,A2,koeff;

    std::complex  <double>  mu, gamma, lambda, epsilon, delta;

    std::complex  <double>  T; 
    std::complex  <double>  tlim;
    std::complex  <double>  q0,p0;  
    std::complex  <double>  Lp, omega;
    std::complex  <double>  rrbarer;
    std::complex  <double>  rrS;


    std::complex <double>  aa ;
    std::complex <double>  bb ;
    std::complex <double>  s1 ;
    std::complex <double>  s2 ;
    std::complex <double>  s3 ;
    std::complex <double>  br1;
    std::complex <double>  br2;
    std::complex <double>  br3;
    std::complex <double>  lp ;
    std::complex <double>  mw2;
    std::complex <double>  ia;

    MNParameters(std::string filename_conf) 
    :     pi      (  3.141592654                )
    ,     I       (  0.,1.)
    ,     conf(filename_conf)
    ,       A1( conf.getf("A1"        ) )
    ,       A2( conf.getf("A2"        ) )
    ,    koeff( conf.getf("k"         ) )
    ,     mu (  A1 * A2 / ( A1 + A2 ) *koeff)
    ,   gamma ( conf.getf("gamma"     ) )
    ,  lambda ( conf.getf("lambda"    ) )
    , epsilon ( conf.getf("epsilon"   ) )
    ,   delta ( - epsilon * epsilon * mu        )
    ,       T ( conf.getf("T"         ) )
    ,    tlim ( conf.getf("tlim"      ) )
    ,      q0 ( conf.getf("q0"        ) )
    ,      p0 ( conf.getf("p0"        ) )
    ,      Lp ( conf.getf("Lp"        ) )
    ,   omega ( conf.getf("omega"     ) )
    , rrbarer ( conf.getf("rrbarer"   ) )
    ,   rrS   ( conf.getf("rrS"       ) )
    {
      rebuild_for_new_lambda_and_epsilon( lambda, epsilon);
    };

   void rebuild_for_new_lambda_and_epsilon(std::complex<double> _lambda, std::complex<double> _epsilon){
        lambda  =  _lambda  ;
        epsilon =  _epsilon  ;
        delta   = - epsilon * epsilon * mu        ;

        // aa = gamma; 
        // aa =         pow(  1./54. * (  2.* pow(gamma,3) -  9.*gamma*(2.*gamma*lambda*epsilon - pow(epsilon,2)) - 27.*gamma*pow(epsilon,2)) , 2) 
        //                 +pow( 1./9. * (3.*(2.*gamma*lambda*epsilon - pow(epsilon,2)) - pow(gamma,2) ) , 3 ) ; 

        // bb =  pow(    aa , 0.5 ) ; 

        // return ;
        aa = pow(  sqrt( pow(  1./54. * (  2.* pow(gamma,3) -  9.*gamma*(2.*gamma*lambda*epsilon - pow(epsilon,2)) - 27.*gamma*pow(epsilon,2)) , 2) 
                        +pow( 1./9. * (3.*(2.*gamma*lambda*epsilon - pow(epsilon,2)) - pow(gamma,2) ) , 3 )   )  
                 - 1./54. *(2.*pow(gamma,3) - 9.*gamma*(2.*gamma*lambda*epsilon - pow(epsilon,2)) - 27.*gamma*pow(epsilon,2)) 
                , 1./3.); 


        bb = pow( -sqrt( pow(  1./54. * (  2.*pow(gamma,3) -  9.*gamma*(2.*gamma*lambda*epsilon - pow(epsilon,2) ) - 27.*gamma*pow(epsilon,2) ) ,2) 
                        +pow(  1./9. * (3.*(2.*gamma*lambda*epsilon - pow(epsilon,2)) - pow(gamma,2) ) ,3 )  ) 
                  - 1./54. *(2.*pow(gamma,3) - 9.*gamma*(2.*gamma*lambda*epsilon - pow(epsilon,2) ) - 27.*gamma*pow(epsilon,2) ) 
             ,1./3.); 
                  
        s1 = aa + bb - gamma/3.; 
        s2 = -   (1./2.) * I  *  sqrt(3.)*(aa - bb) - (aa + bb)/2. - gamma/3.; 
        s3 =     (1./2.) * I  *  sqrt(3.)*(aa - bb) - (aa + bb)/2. - gamma/3.; 

        br1 = 1./((s1 - s2)*(s1 - s3)); 
        br2 = 1./((s2 - s1)*(s2 - s3)); 
        br3 = 1./((s3 - s1)*(s3 - s2)); 
        int ifault;

        lp = -((2.*gamma*lambda*epsilon*(pow(s1,3) + gamma*pow(s1,2) + pow(s2,3) + gamma*pow(s2,2) ) + (gamma + s1)*(s1 + s2)*(gamma + s2)*(s1*s2 - pow(epsilon,2) )) 
           /  ((s1 - s2)*(s2 - s1)*( pow(gamma + s1 + s2 ,2) - pow(epsilon,2) ))); 
        mw2 = (mu*pow(epsilon,2) *(gamma + s1)*(gamma + s2))/(pow(epsilon,2)  - pow(gamma + s1 + s2 ,2) ); 
        // ia  = (gamma*s1*gsl_sf_psi( (s1/(2.*pi*T)).real() ) - gamma*s1 * gsl_sf_psi( (gamma/(2.*pi*T) + 1.).real() ) + pi*s1*T + pi*gamma*T)/(4*gamma*pow(s1,3)*T - 4.*pow(gamma,3)*s1*T);
        // ia  = (gamma*s1* double(digamma( (s1/(2.*pi*T)).real(), &ifault )) - gamma*s1 * double(digamma( (gamma/(2.*pi*T) + 1.).real(), &ifault )) + pi*s1*T + pi*gamma*T)/(4*gamma*pow(s1,3)*T - 4.*pow(gamma,3)*s1*T);
        ia  = (   gamma*s1* digamma( (s1/(2.*pi*T)).real(), &ifault ) 
                - gamma*s1 *digamma( (gamma/(2.*pi*T) + 1.).real(), &ifault ) + pi*s1*T + pi*gamma*T)
              / (4.*gamma*pow(s1,3)*T - 4.*pow(gamma,3)*s1*T);
              // / //(4*gamma*pow(s1,3)*T - 4.*pow(gamma,3)*s1*T);
        // ia  = 0;
   };

   // Copy Constructor
   MNParameters(const MNParameters &p) 
    :       pi(   3.141592654  )
    ,        I(    p.I         )
    ,     conf(    p.conf.source())
    ,       A1(    p.A1        )    
    ,       A2(    p.A2        )    
    ,    koeff(    p.koeff     )       
    ,       mu(    p.mu        )    
    ,    gamma(    p.gamma     )       
    ,   lambda(    p.lambda    )        
    ,  epsilon(    p.epsilon   )         
    ,    delta(    p.delta     )       
    ,        T(    p.T         )   
    ,     tlim(    p.tlim      )      
    ,       q0(    p.q0        )    
    ,       p0(    p.p0        )    
    ,       Lp(    p.Lp        )    
    ,    omega(    p.omega     )       
    ,  rrbarer(    p.rrbarer   )         
    ,      rrS(    p.rrS       )
    ,       aa(    p.aa        )
    ,       bb(    p.bb        )
    ,       s1(    p.s1        )
    ,       s2(    p.s2        )
    ,       s3(    p.s3        )
    ,      br1(    p.br1       )
    ,      br2(    p.br2       )
    ,      br3(    p.br3       )
    ,       lp(    p.lp        )
    ,      mw2(    p.mw2       )
    ,       ia(    p.ia         )
    {
    };

   
    void print() {
        std::cout << "pi: "       << pi << std::endl;
        std::cout << " I: "       <<  I << std::endl;
        std::cout << "A1:       " << A1         << std::endl;     
        std::cout << "A2:       " << A2         << std::endl;     
        std::cout << "koeff:    " << koeff      << std::endl;        
        std::cout << "mu:       " << mu         << std::endl;     
        std::cout << "gamma:    " << gamma      << std::endl;        
        std::cout << "lambda:   " << lambda     << std::endl;         
        std::cout << "epsilon:  " << epsilon    << std::endl;          
        std::cout << "delta:    " << delta      << std::endl;        
        std::cout << "T:        " << T          << std::endl;    
        std::cout << "tlim:     " << tlim       << std::endl;       
        std::cout << "q0:       " << q0         << std::endl;     
        std::cout << "p0:       " << p0         << std::endl;     
        std::cout << "Lp:       " << Lp         << std::endl;     
        std::cout << "omega:    " << omega      << std::endl;        
        std::cout << "rrbarer:  " << rrbarer    << std::endl;          
        std::cout << "rrS:      " << rrS        << std::endl;      
        std::cout << " aa  :    " << aa         << std::endl; 
        std::cout << " bb  :    " << bb         << std::endl; 
        std::cout << " s1  :    " << s1         << std::endl; 
        std::cout << " s2  :    " << s2         << std::endl; 
        std::cout << " s3  :    " << s3         << std::endl; 
        std::cout << " br1 :    " << br1        << std::endl; 
        std::cout << " br2 :    " << br2        << std::endl; 
        std::cout << " br3 :    " << br3        << std::endl; 
        std::cout << " lp  :    " << lp         << std::endl; 
        std::cout << " mw2 :    " << mw2        << std::endl; 
        std::cout << " ia  :    " << ia         << std::endl; 

    }

};




# endif