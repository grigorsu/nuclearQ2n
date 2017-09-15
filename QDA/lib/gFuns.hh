 // #include "gFuns.hh"
#include <vector>
#include <map>

#ifndef _g_FunS_
#define _g_FunS_


struct gFuns {
  // find index with minimum value
  size_t indx_min_in_v(const std::vector <double> &v) const;
  // find index with maximum value
  size_t indx_max_in_v(const std::vector <double> &v) const;
  // convert string like "2.345 4.234" to vector of double: 
  std::vector <double> vs2vd( const std::vector<std::string> &s); 

} gFuns;


//------------------------------------------------------------------
//
std::vector <double> gFuns::vs2vd( const std::vector<std::string> &s){
  std::vector <double> v;
  for(size_t i = 0; i < s.size(); ++i) {
    v.push_back(atof(s[i].c_str()));
  }
  return v;
} ; 
//------------------------------------------------------------------
//
size_t gFuns::indx_min_in_v(const std::vector <double> &v) const {
    if(v.size() == 0) 
      std::cout <<"ERROR: "<< __FILE__ << " : " << __LINE__  << ": can't find minimum in vetor v with zero size  " << std::endl;
    size_t imin = 0;
    for(unsigned i=0; i < v.size(); ++i) { if(v[imin] > v[i]) imin = i;};
    return imin;
}
//------------------------------------------------------------------
//
size_t gFuns::indx_max_in_v(const std::vector <double> &v) const {
    if(v.size() == 0) 
      std::cout <<"ERROR: "<< __FILE__ << " : " << __LINE__  << ": can't find maximum in vetor v with zero size  " << std::endl;
    size_t imax = 0;
    for(unsigned i=0; i < v.size(); ++i) { if(v[imax] < v[i]) imax = i;};
    return imax;
}
//------------------------------------------------------------------
//  
 


#endif