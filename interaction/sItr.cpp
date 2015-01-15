#include "interaction.h"
#include <cmath> // exp(); pow(); sqrt()
#include <gsl/gsl_sf_coupling.h>
#include <algorithm> // max() and min()
#include <boost/math/special_functions/binomial.hpp>
#include <boost/math/special_functions/factorials.hpp>
#include <iostream>

double interaction::vl(int l, double Q)  // l: relative angular momentum; this is for Coulomb interaction
{
   using namespace boost::math;
   using namespace std;
   double result;
   double  R = sqrt(Q);  //radius of the sphere
   
result = (double)(2/R) * binomial_coefficient<double>(4*Q-2*l, 2*Q-l) * binomial_coefficient<double>(4*Q+ 2*l +2, 2*Q +l +1) / binomial_coefficient<double>(4*Q+2, 2*Q+1) / binomial_coefficient<double>(4*Q+2, 2*Q+1);  // V_L formula   
    return result;
}

double interaction::cgcs(int m1, int m2, int m, double Q)
{
  using namespace boost::math;
  using namespace std;
  double result = 0;
  int l1, l2, L, M, M1, M2;  // L: two times of the relative angular momentum; M:two times of the total angular momentum = m1+m2
  M = (int) 2*(m1 + m2 - 2* Q);
  M1 = (int) 2*(m1 - Q);
  M2 = (int) 2*(m2 - Q);
  l1 = (int) 2*Q; l2 = (int) 2*Q; L = 2*m;

  result = pow(-1, m1+m2-2*Q) * sqrt(2*m+1) * gsl_sf_coupling_3j(l1, l2, L, M1, M2, -M);
  
  return result;  
}

double interaction::sItr(int m1, int m2, int m3, double Q)
{
  using namespace std;
  int m4 = m1 + m2 - m3;
  if (m4 < 0)
    return 0;

  double result = 0;
  int mz = (int)abs(m1+m2-2*Q);
  
  switch (type[1])
    {
    case 'C':
       for (int m = mz; m <= 2*Q; m++)
        result += cgcs(m3, m4, m, Q) * vl(m,Q) * cgcs(m1, m2, m, Q);
      break;
    case 'H':
      for (int m = 0; m <= 1; m++)
          result += cgcs(m3, m4, m, Q) * param[2 + m] * cgcs(m1, m2, m, Q);
      break;
    }
  return result;
} 
