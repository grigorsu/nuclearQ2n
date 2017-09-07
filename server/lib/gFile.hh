
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <string>
#include <map>
#include <vector>
#include <sstream>

#include <dirent.h>

#ifndef _g_File_
#define _g_File_


struct gFile {
  // by default flag = 1 - means include path (default)
  void ls(std::vector<std::string> &files, std::string path, std::string mask,  int flag );
  bool if_file_exist(const std::string & filename )  ;

  void read_file(const std::string & filename, std::vector <std::string> &lines, size_t max_line_size);

  std::string replace(const std::string & in, const std::string & from, const std::string & to);
  
  // option = "w" that means to write new file or "a" that means to append existed file; by default option = "w"
  void write_file(const std::string & s, const std::string filename, std::string  option ) ;
  //by default option = "all" 
  void split( const std::string& text, const std::string& delims, std::vector <std::string> &v, const std::string option);
  // returns the string from union vector of string with the delimeter
  std::string vs2s(std::vector<std::string> &vs, std::string delim);
  // returns the string from union vector of double with the delimeter
  std::string vd2s(std::vector< double > &vd, std::string delim, size_t n, std::string option);
  // returns the string from vector of string to vector of double
  std::vector <double> vs2vd(const std::vector<std::string> &vs);
  // returns vector of string from string
  std::vector<std::string> s2vs(std::string input, std::string delim);
  // Ex: vs_push_back(vs, "23,34,54", ","); // adds "23", "34", "54" to vs ;   by default delim=", "
  void vs_push_back(std::vector<std::string> &vs, const std::string &s, std::string delim);

  template <typename T> std::string to_str(T temp);

  std::string format_to_str(     double x, size_t n, std::string option);
  std::string format_to_str(std::string x, size_t n, std::string option);

  template <typename T> int find_in_v(T x, const std::vector <T> & v ) const;

} gFile;// gFile;


//------------------------------------------------------------------
// 
    std::string gFile::replace(const std::string & in, const std::string & from, const std::string & to){
      if(from.size() == 0 ) return in;
      std::string out = "";
      std::string tmp = "";
      for(int i = 0, ii = -1; i < in.size(); ++i) {
        // change ii
        if     ( ii <  0 &&  from[0] == in[i] )  {
          ii  = 0;
          tmp = from[0]; 
        } else if( ii >= 0 && ii < from.size()-1 )  {
          ii ++ ;
          tmp = tmp + in[i];
          if(from[ii] == in[i]) {
          } else {
            out = out + tmp;
            tmp = "";
            ii = -1;
          }
        } else {
          out = out + in[i];
        }
        if( tmp == from ) {
          out = out + to;
          tmp = "";
          ii = -1;
        }
      }
      return out;
    };

//------------------------------------------------------------------
// 
bool gFile::if_file_exist(const std::string & filename )   {
  struct stat buffer;   
  return (stat (filename.c_str(), &buffer) == 0); 
};
//------------------------------------------------------------------
// 
template <typename T> int gFile::find_in_v(T x, const std::vector <T> & v ) const{
  int result = -1;
  for( size_t i = 0 ; i < v.size(); ++i ) {
    // DEBUG(x $ v[i])
    if(x == v[i]) {
      result = i;
      break;
    }
  }
  return result;
};
//------------------------------------------------------------------
// 
std::string gFile::format_to_str(double  x, size_t n, std::string option="right"){
  std::string sx = to_str(x);
  if(n < sx.size()) {
    std::cout <<"ERROR: "<< __FILE__ << " : " << __LINE__  << ": format_to_str(): size of string = "<< sx.size() << " is more than n = " << n << std::endl;
  } else if (option == "left"){
    for(size_t i = sx.size(); i <= n; ++i ) sx = sx + " ";
  } else if (option == "right"){
    for(size_t i = sx.size(); i <= n; ++i ) sx = " " + sx;
  } else if (option == "center"){
    for(size_t i = sx.size(); i <= n; ++i ) sx = (i % 2 == 0) ? " " + sx : sx + " ";
  }
  return sx;
};
//------------------------------------------------------------------
// 
std::string gFile::format_to_str(std::string  sx, size_t n, std::string option="right"){
  if(n < sx.size()) {
    std::cout <<"ERROR: "<< __FILE__ << " : " << __LINE__  << ": format_to_str(): size of string = "<< sx.size() << " is more than n = " << n << std::endl;
  } else if (option == "left"){
    for(size_t i = sx.size(); i <= n; ++i ) sx = sx + " ";
  } else if (option == "right"){
    for(size_t i = sx.size(); i <= n; ++i ) sx = " " + sx;
  } else if (option == "center"){
    for(size_t i = sx.size(); i <= n; ++i ) sx = (i % 2 == 0) ? " " + sx : sx + " ";
  }
  return sx;
};
//------------------------------------------------------------------
// 
void gFile::ls(std::vector<std::string> &files, std::string path, std::string mask="", int flag = 1) {
  DIR *dir;
  struct dirent * pdirent;
  if((dir = opendir(path.c_str())) != NULL ) {
    while(( pdirent = readdir(dir)) != NULL ) {
      std::string tmpname =  pdirent -> d_name;
      if (tmpname == "." ||  tmpname == "..") continue;
      if(mask.size() >0 && tmpname.find(mask) == std::string::npos) continue;
      if(flag == 1) tmpname = path + tmpname;
      files.push_back(tmpname);
    }  
    closedir(dir);
  } else {
    std::cout <<"ERROR: "<< __FILE__ << " : " << __LINE__  << ": ls(): couldn't read directory " << std::endl;
  }
}
//------------------------------------------------------------------
// 
void gFile::read_file(const std::string & filename, std::vector <std::string> &lines, size_t max_line_size = 10000) {
  FILE *f = fopen(filename.c_str(), "r");
  if(f == NULL) {
    std::cout <<"ERROR: "<< __FILE__ << " : " << __LINE__  << ": can't open file " << filename << std::endl;
  } else {
    bool eof =0;
    char c_previous = ' ';
    while(1) {
      int current_position = 0;
      char tmp[max_line_size]; 
      for(int i = 0; i < max_line_size; ++i) tmp[i] = '\0';
      char c;
      while(1) {
        c = fgetc(f);
        if(current_position >= max_line_size - 1) 
          std::cout <<"ERROR: "<< __FILE__ << " : " << __LINE__  << ": too long string ... " << std::endl;
        if(c == '\0' || c == '\r' || c == '\n') break;
        else if (c==EOF) {  eof = 1; break;} 
        else { tmp[current_position] = c; ++current_position; }
      }
      tmp[current_position] = '\0';
      if(current_position >= 0 && !(c_previous == '\r' && c == '\n'))
        lines.push_back(tmp);
      c_previous = c;
      if(eof) break;
    }
  // std::cout <<"DEBUG: "<< __FILE__ << " : " << __LINE__  << filename<< std::endl;
    fclose(f);
  }
}
//------------------------------------------------------------------
// 
void gFile::write_file(const std::string & s, const std::string filename, std::string  option = "w" ) {
  if(option == "w") {
    std::ofstream tmp(filename.c_str(), std::ios_base::out);
    if(tmp.is_open()) {
      tmp << s;
      tmp.close();
    } else {
      std::cout <<"ERROR: "<< __FILE__ << " : " << __LINE__  << ": write_file(): couldn't write file " << filename << std::endl;
    }
  }
}
//------------------------------------------------------------------
// 
void gFile::split( const std::string& s, const std::string& c, std::vector <std::string> &v, const std::string option = "all") {

  if((int)c.size()==0) std::cout <<"ERROR: "<< __FILE__ << " : " << __LINE__  << ": undefined delimiter ... " << std::endl;
  v.clear();
  if(s.size() == 0 ) return;

  // aaa and bbb are all coordinates for delimiters
  std::vector <int> aaa(1,-1); // left boundary
  std::vector <int> bbb;       // right boundary
  for(int indx = 0; indx < (int)s.size(); ++indx) {
    bool is_delimiter(c[0] == s[indx]);
    for(int ii = 0; ii < (int)c.size(); ++ii) {
      is_delimiter = is_delimiter || (c[ii] == s[indx]);
    }
    if( is_delimiter ) {
      aaa.push_back(indx); 
      bbb.push_back(indx);
    }
  }
  bbb.push_back(s.size());
  
  for(size_t i = 0; i < bbb.size(); ++i) {
    if  (bbb[i] == aaa[i] + 1 ) {
      if(option == "all") v.push_back("");
    } else if (bbb[i] > aaa[i] + 1) {
      v.push_back(s.substr(aaa[i] + 1, bbb[i] - aaa[i] - 1 ));
    } else {
        std::cout <<"ERROR: "<< __FILE__ << " : " << __LINE__  << ": aaa and bbb have wrong values: " ;
        for(size_t i = 0 ; i < aaa.size(); ++i) std::cout << " " << aaa[i] ;
        std::cout <<std::endl;
        for(size_t i = 0 ; i < bbb.size(); ++i) std::cout << " " << bbb[i] ;
    }
  }
}

//------------------------------------------------------------------
// 
std::string gFile::vs2s(std::vector<std::string> &vs, std::string delim=", ") {
  std::string out ="";
  for(size_t i = 0; i < vs.size(); ++i) {
    out = out + vs[i];
    if(i < vs.size()-1) 
      out = out + delim;
  }
  return out;
}
//------------------------------------------------------------------
// 
std::string gFile::vd2s(std::vector<double> &vd, std::string delim=", ", size_t n=0, std::string option = "right") {
  std::string out ="";
  for(size_t i = 0; i < vd.size(); ++i) {
    // out = out + to_str(vd[i]);
    out = (n>0) ? out + format_to_str(vd[i],n,option) : out + to_str(vd[i]);
    // cout << out;
    if(i < vd.size()-1) 
      out = out + delim;
  }
  return out;
}



//------------------------------------------------------------------
// 
#include <cstdlib>
std::vector <double> gFile::vs2vd(const std::vector<std::string> &vs) {
  std::vector <double> vd;  
  for(size_t i = 0; i < vs.size(); ++ i) {
    vd.push_back(atof(vs[i].c_str()));
  }
  return vd;
};
//------------------------------------------------------------------
// 
std::vector<std::string> gFile::s2vs(std::string input, std::string delim=","){
  std::vector<std::string> v;
  split(input,delim, v, "");
  return v;
};

//------------------------------------------------------------------
// 
void gFile::vs_push_back(std::vector<std::string> &vs, const std::string &s, std::string delim=", "){
  std::vector <std::string> tmp_vs; 
  split(s, delim, tmp_vs, "");
  for(size_t i=0; i < tmp_vs.size(); ++i ) {
    vs.push_back(tmp_vs[i]);
  }
}

//------------------------------------------------------------------
// 
template <typename T> 
std::string gFile::to_str(T temp){
  std::stringstream ss;
  ss<<temp;
  return ss.str();
}


#endif