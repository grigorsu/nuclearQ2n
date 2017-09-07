# include <cmath>
# include <cstdlib>
# include <iomanip>
# include <iostream>

// # include "asa103.hpp"



double digamma ( double x, int *ifault );
void psi_values ( int *n_data, double *x, double *fx );
void timestamp ( );



# include <cmath>
# include <cstdlib>
# include <ctime>
# include <iostream>
# include <iomanip>

using namespace std;

// # include "asa103.hpp"

//****************************************************************************80

double digamma ( double x, int *ifault )

//****************************************************************************80
//
//  Purpose:
//
//    DIGAMMA calculates DIGAMMA ( X ) = d ( LOG ( GAMMA ( X ) ) ) / dX
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    20 March 2016
//
//  Author:
//
//    Original FORTRAN77 version by Jose Bernardo.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Jose Bernardo,
//    Algorithm AS 103:
//    Psi ( Digamma ) Function,
//    Applied Statistics,
//    Volume 25, Number 3, 1976, pages 315-317.
//
//  Parameters:
//
//    Input, double X, the argument of the digamma function.
//    0 < X.
//
//    Output, int *IFAULT, error flag.
//    0, no error.
//    1, X <= 0.
//
//    Output, double DIGAMMA, the value of the digamma function at X.
//
{
  static double c = 8.5;
  static double euler_mascheroni = 0.57721566490153286060;
  double r;
  double value;
  double x2;
//
//  Check the input.
//
  if ( x <= 0.0 )
  {
    value = 0.0;
    *ifault = 1;
    return value;
  }
//
//  Initialize.
//
  *ifault = 0;
//
//  Use approximation for small argument.
//
  if ( x <= 0.000001 )
  {
    value = - euler_mascheroni - 1.0 / x + 1.6449340668482264365 * x;
    return value;
  }
//
//  Reduce to DIGAMA(X + N).
//
  value = 0.0;
  x2 = x;
  while ( x2 < c )
  {
    value = value - 1.0 / x2;
    x2 = x2 + 1.0;
  }
//
//  Use Stirling's (actually de Moivre's) expansion.
//
  r = 1.0 / x2;
  value = value + log ( x2 ) - 0.5 * r;

  r = r * r;

  value = value 
    - r * ( 1.0 / 12.0 
    - r * ( 1.0 / 120.0 
    - r * ( 1.0 / 252.0 
    - r * ( 1.0 / 240.0
    - r * ( 1.0 / 132.0 ) ) ) ) );

  return value;
}
//****************************************************************************80

void psi_values ( int *n_data, double *x, double *fx )

//****************************************************************************80
//
//  Purpose:
//
//    PSI_VALUES returns some values of the Psi or Digamma function.
//
//  Discussion:
//
//    In Mathematica, the function can be evaluated by:
//
//      PolyGamma[x]
//
//    or
//
//      Polygamma[0,x]
//
//    PSI(X) = d ln ( Gamma ( X ) ) / d X = Gamma'(X) / Gamma(X)
//
//    PSI(1) = - Euler's constant.
//
//    PSI(X+1) = PSI(X) + 1 / X.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    17 August 2004
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Milton Abramowitz, Irene Stegun,
//    Handbook of Mathematical Functions,
//    National Bureau of Standards, 1964,
//    ISBN: 0-486-61272-4,
//    LC: QA47.A34.
//
//    Stephen Wolfram,
//    The Mathematica Book,
//    Fourth Edition,
//    Cambridge University Press, 1999,
//    ISBN: 0-521-64314-7,
//    LC: QA76.95.W65.
//
//  Parameters:
//
//    Input/output, int *N_DATA.  The user sets N_DATA to 0 before the
//    first call.  On each call, the routine increments N_DATA by 1, and
//    returns the corresponding data; when there is no more data, the
//    output value of N_DATA will be 0 again.
//
//    Output, double *X, the argument of the function.
//
//    Output, double *FX, the value of the function.
//
{
# define N_MAX 11

  double fx_vec[N_MAX] = { 
     -0.5772156649015329E+00,  
     -0.4237549404110768E+00,  
     -0.2890398965921883E+00,  
     -0.1691908888667997E+00,  
     -0.6138454458511615E-01,  
      0.3648997397857652E-01,  
      0.1260474527734763E+00,  
      0.2085478748734940E+00,  
      0.2849914332938615E+00,  
      0.3561841611640597E+00,  
      0.4227843350984671E+00 };

  double x_vec[N_MAX] = { 
     1.0E+00,  
     1.1E+00,  
     1.2E+00,  
     1.3E+00,  
     1.4E+00,  
     1.5E+00,  
     1.6E+00,  
     1.7E+00,  
     1.8E+00,  
     1.9E+00,  
     2.0E+00 };

  if ( *n_data < 0 )
  {
    *n_data = 0;
  }

  *n_data = *n_data + 1;

  if ( N_MAX < *n_data )
  {
    *n_data = 0;
    *x = 0.0;
    *fx = 0.0;
  }
  else
  {
    *x = x_vec[*n_data-1];
    *fx = fx_vec[*n_data-1];
  }

  return;
# undef N_MAX
}
//****************************************************************************80

void timestamp ( )

//****************************************************************************80
//
//  Purpose:
//
//    TIMESTAMP prints the current YMDHMS date as a time stamp.
//
//  Example:
//
//    31 May 2001 09:45:54 AM
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    24 September 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    None
//
{
# define TIME_SIZE 40

  static char time_buffer[TIME_SIZE];
  const struct tm *tm;
  size_t len;
  time_t now;

  now = time ( NULL );
  tm = localtime ( &now );

  len = strftime ( time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm );

  cout << time_buffer << "\n";

  return;
# undef TIME_SIZE
}



using namespace std;

int main ( );
void digamma_test ( );

//****************************************************************************80

// int main ( )

// //****************************************************************************80
// //
// //  Purpose:
// //
// //    MAIN is the main program for ASA103_PRB.
// //
// //  Discussion:
// //
// //    ASA103_PRB tests the ASA103 library.
// //
// //  Licensing:
// //
// //    This code is distributed under the GNU LGPL license. 
// //
// //  Modified:
// //
// //    18 January 2008
// //
// //  Author:
// //
// //    John Burkardt
// //
// {
//   timestamp ( );
//   cout << "\n";
//   cout << "ASA103_PRB:\n";
//   cout << "  C++ version\n";
//   cout << "  Test the ASA103 library.\n";

//   digamma_test ( );
// //
// //  Terminate.
// //
//   cout << "\n";
//   cout << "ASA103_PRB:\n";
//   cout << "  Normal end of execution.\n";
//   cout << "\n";
//   timestamp ( );

//   return 0;
// }
//****************************************************************************80

void digamma_test ( )

//****************************************************************************80
//
//  Purpose:
//
//    DIGAMMA_TEST tests DIGAMA.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    18 January 2008
//
//  Author:
//
//    John Burkardt
//
{
  double fx;
  double fx2;
  int ifault;
  int n_data;
  double x;

  cout << "\n";
  cout << "DIGAMMA_TEST:\n";
  cout << "  DIGAMMA computes the Digamma or Psi function. \n";
  cout << "  Compare the result to tabulated values.\n";
  cout << "\n";
  cout << "          X       "
       << "FX                        FX2\n";
  cout << "                  "
       << "(Tabulated)               (DIGAMMA)               DIFF\n";
  cout << "\n";

  n_data = 0;

  for ( ; ; )
  {
    psi_values ( &n_data, &x, &fx );

    if ( n_data == 0 )
    {
      break;
    }

    fx2 = digamma ( x, &ifault );

    cout << "  " << setprecision(4) << setw(10) << x
         << "  " << setprecision(16) << setw(24) << fx
         << "  " << setprecision(16) << setw(24) << fx2
         << "  " << setprecision(4) << setw(10) << fabs ( fx - fx2 ) << "\n";
  }

  return;
}