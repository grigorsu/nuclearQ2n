
// #include "gConf.hh"
#include "gFile.hh"
#include <vector>
#include <map>

#ifndef _g_Conf_
#define _g_Conf_


struct gConf {

 public:
  gConf();
  gConf(int a); //empty class
  gConf(const std::string & name);

  gConf(int argc, char** argv);


  float                            getf(const std::string name, int i) const;
  const std::string              & get(const std::string name, int i) const;
  const std::vector<std::string> & get_all(const std::string name)    const;
  std::vector< double >            get_vd( const std::string name)    const;
  std::string                      get_string(const std::string name) const;
  int                              length(const std::string name)     const;
  bool                             is_present(std::string name)       const;
  bool                             is_true(std::string name)          const;

  void                             erase(const std::string & name);  

  void                             add(const std::string & line);  
  void                             add(const std::string & line, const std::vector<std::string> &value);  
  void                             add(const std::string & line, const std::vector<double>  &value);  
  void                             addonefield(const std::string & line, const std::string & line2);  
  void                             read_from_file(const std::string & name);  
  void                             update();
  void                             read_table(const std::string &filename, const std::string  delims);
  void                             save(std::string file_name);  
  void                             write_table(const std::string &filename);

  void                             change(const std::string & key, const std::string & new_value, int index);  

  void                             print()    const;  
  
  // bool                             log_out(std::string message, std::string key);
  // std::string                      log_in(std::string key);


 private:
  std::map <std::string, std::vector <std::string> > _values;
  std::vector <std::string>        _keys;
  std::vector <std::string>        _s_values;  //for beauty printing
  int                              _ii_keys(const std::string &name) const; 
  void                             _error_if_is_not_present(const std::string, int i) const;
  std::string                      _file_name; // filename
 public:
  std::string                      source() const  {return _file_name;};  
};



// // ------------------------------------------------------------------
  void                             gConf::update(){
    _values.clear();
    _keys.clear();
    _s_values.clear();
    read_from_file(_file_name);
  }
// // ------------------------------------------------------------------
//   bool                             gConf::log_out(std::string message, std::string key="MESSAGE") {
//     if(this->is_present("-log")) {
//       gConf conf_control(this->get("-log"));
//       conf_control.change("MESSAGE",message);
//       conf_control.save();
//       return 1;
//     };
//     return 0;
//   }

// // ------------------------------------------------------------------
//   std::string                       gConf::log_in(std::string key) {
//     if(this->is_present("-log")) {
//       gConf conf_control(this->get("-log"));
//       conf_control.get(key);
//       conf_control.save();
//       return 1;
//     };
//     return 0;
//   }

// ------------------------------------------------------------------

void                             gConf::erase(const std::string & name){
  // check, before use !
  if(is_present(name)) {
    int ii = _ii_keys(name);
    // DEBUG(name << " " << ii)
    if (ii >=0 ) {
      _keys.erase(    _keys.begin()+ii);
      _s_values.erase(_s_values.begin()+ii);
      _values.erase(name);
    }
  }
};  

//------------------------------------------------------------------
//
  std::vector<double> gConf::get_vd( const std::string name)    const{
    std::vector <double> v;
    const std::vector <std::string> & s = get_all( name );
    for(size_t i = 0; i < s.size(); ++i) {
      v.push_back(atof(s[i].c_str()));
    }
    return v;
  };
//------------------------------------------------------------------
//
void gConf::_error_if_is_not_present(const std::string name, int i = 0) const{
  std::map < std::string, std::vector <std::string> >::const_iterator it = _values.find(name);
  if(it == _values.end()) {
    if(_file_name == "_@_##_argc_argv_##_@_" ) {
      std::cout << " unknown argument: "<< name << " " << i <<" try --help for details"  << std::endl;
      exit(0);
    }
    std::cout <<"ERROR: "<< __FILE__ << " : " << __LINE__  << ": In " << _file_name << " there is no such name: " << name  << std::endl;
  } 
  if (i >= (int)it->second.size()) 
    std::cout <<"ERROR: "<< __FILE__ << " : " << __LINE__  << ": In " << _file_name << " the name " << name << " has no the index i = " << i << std::endl;
}
//------------------------------------------------------------------
//  
gConf::gConf() {};
//------------------------------------------------------------------
//  read from arguments
gConf::gConf(int a) {
  // empty class created
  if(a==0) std::cout << "";
};//------------------------------------------------------------------
//  read from arguments
gConf::gConf(int argc, char** argv) {
  _file_name="_@_##_argc_argv_##_@_";
  std::string key = "";
  for(size_t i = 0; i < argc; ++i) {
    std::string tmp = std::string(argv[i]);
    if (tmp.size() == 6 and tmp.substr(0,6) == "--help" ) add("-h ..");
    if (tmp.size() == 2 and tmp.substr(0,2) == "-h"     ) add("-h ..");
    else if (tmp[0] == '-' ) key = tmp;
    else if (key != "" )  add(key + " " + tmp );
  }
};
//------------------------------------------------------------------
//  
gConf::gConf(const std::string & name){
  _file_name = name;
  if(!gFile.if_file_exist(name)) {
    std::cout <<"WARNING(): "<< __FILE__ << " : " << __LINE__  << " there is no such file: " << name 
    << ", empty file is created. " << std::endl;
    system(("touch " + name).c_str());
  }
  read_from_file(name);
};   
//------------------------------------------------------------------
//  
float    gConf::getf(const std::string name, int i=0) const{
  _error_if_is_not_present(name, i);
  return atof((_values.find(name) -> second[i]).c_str());
};   
//------------------------------------------------------------------
//  
const std::string              & gConf::get(const std::string name, int i=0) const{
  _error_if_is_not_present(name, i);
  return _values.find(name) -> second[i];
};   
//------------------------------------------------------------------
//  
std::string              gConf::get_string(const std::string name) const{
  _error_if_is_not_present(name);
  for(size_t ii = 0; ii < _s_values.size(); ++ii) {
      if(_s_values[ii] == name) return _s_values[ii];
  }
  std::string error_result = "ERROR, can't find key - " + std::string(name) + "\n";
  std::cout << error_result;
  return error_result;
};   
//------------------------------------------------------------------
//  
const std::vector<std::string> & gConf::get_all(const std::string name) const{
  _error_if_is_not_present(name);
  return _values.find(name) -> second;
};   
//------------------------------------------------------------------
//  
int                              gConf::length(const std::string name) const{
  _error_if_is_not_present(name);
  return _values.find(name) -> second.size();
};   
//------------------------------------------------------------------
//  
bool                             gConf::is_present(std::string name) const{

  return (_values.find(name) != _values.end());
};   
//------------------------------------------------------------------
//  
bool                             gConf::is_true(std::string name) const{

  return get(name) != "0";
};   
//------------------------------------------------------------------
//  
int                              gConf::_ii_keys(const std::string &name) const{
  for(int i = int(_keys.size())-1; i >=0 ; --i) {
    if(name == _keys[i]) return i;
  }
  return -1;
}; 
//------------------------------------------------------------------
//  
void                             gConf::add(const std::string & line){
  // DEBUG(line)

  if( line.size() == 0 || (line.size() > 0 && line[0] == '/' ))  {
    _keys.push_back("");
    _s_values.push_back(line);
    return;
  } 

  std::vector <std::string> words;
  gFile.split(line, " \t", words, "");

  if(words.size() == 0) {
    return;
  } 
  if(words.size() == 1) {
    std::cout <<"ERROR: "<< __FILE__ << " : " << __LINE__  << " incorrect line: " << line << std::endl;
    return;
  }
  if(_values.find(words[0]) == _values.end()){
    _values[words[0]] = std::vector < std::string >();
    _keys.push_back(words[0]);
    _s_values.push_back("");
  } 

  for(size_t i = 1; i < words.size(); ++i) {
    _values[words[0]].push_back(words[i]);
  }
  
  int i = _ii_keys(words[0]);
  if( i >= 0 ) {
  // DEBUG(i)
    for(size_t ii = words[0].size(); ii < line.size(); ++ii) {
      if(line[ii] == '\0' || line[ii] == '\r' || line[ii] == '\n') break;
      _s_values[i] = _s_values[i] + line[ii];
    }
  }
};  
//------------------------------------------------------------------
//  
void                           gConf::add(const std::string & line, const std::vector<std::string> &value){
  std::string line_full = line;
  for(size_t i = 0 ; i < value.size(); ++i) { 
    line_full = line_full + "  " + (value[i]);
  }  
  add(line_full);
};     
//------------------------------------------------------------------
//  
void                           gConf::add(const std::string & line, const std::vector<double> &value){
  std::string line_full = line;
  for(size_t i = 0 ; i < value.size(); ++i) { 
    line_full = line_full + "  " +  gFile.to_str(value[i]);
  }  
  add(line_full);
};     
//------------------------------------------------------------------
//  
//   void                             gConf::addonefield(const std::string & line, const std::string & line2){
//   };     
//------------------------------------------------------------------
//  
void                             gConf::print() const{
  for(unsigned i = 0; i < _keys.size(); ++i) {
    std::cout << _keys[i] << _s_values[i] << "\n";
  }
  // for( std::map < std::string, std::vector<std::string> > :: const_iterator it = _values.begin(); it != _values.end(); ++it) {
  //   std::cout << it->first << " <<< ";
  //   for(size_t i = 0; i < (it->second).size(); ++i ) {
  //     std::cout << (it->second)[i] << " ... ";
  //   }
  //   std::cout << std::endl;
  // }
};     
//------------------------------------------------------------------
//  
void                             gConf::save(std::string file_name=""){
  if (file_name == "") file_name = _file_name;
  std::string message ="";

  for(unsigned i = 0; i < _keys.size(); ++i) {
    message = message + _keys[i] + _s_values[i] + "\n";
  }

  // for( std::map < std::string, std::vector<std::string> > :: const_iterator it = _values.begin(); it != _values.end(); ++it) {
  //   message = message + (it->first) + "    ";
  //   for(size_t i = 0; i < (it->second).size(); ++i ) {
  //     message = message + ((it->second)[i]) + "    ";
  //   }
  //   message = message + "\n";
  // }  
  gFile.write_file(message, file_name, "w" );
};  

//------------------------------------------------------------------
//  
void                             gConf::read_from_file(const std::string & name){
  std::vector <std::string> lines;
  gFile.read_file(name,lines);
  for(size_t i = 0; i < lines.size(); ++i) {
    // if((lines[i]).size() > 0 && (lines[i])[0] == '/' )    continue;
    add(lines[i]);
  }
};     
//------------------------------------------------------------------
//  
void                             gConf::read_table(const std::string &filename, const std::string  delims = " \t"){
  std::vector <std::string> lines;
  gFile.read_file(filename,lines);

  std::vector <std::string> columns;
  
  for(size_t i = 0; i < lines.size(); ++i) {
    std::vector <std::string> words;
    gFile.split(lines[i], delims, words, ""); // ""  - reset default value for option "all", that means skip empty values

    // title;
    if (i == 0) {
      for(size_t ii = 0; ii < words.size(); ++ii) {
        columns.push_back(words[ii] + "   ");
      }
      continue;
    };
    // else i > 0:

    if(words.size() != columns.size() ) {
      ERROR("in file " << filename << " in line " << i << " there are should be " << columns.size() << " columns ");
      continue;
    }

    for(size_t ii = 0; ii < words.size(); ++ii) {
      columns[ii] = columns[ii] + words[ii] + " ";
    }

  }
  for(size_t i = 0; i < columns.size(); ++i) {
    add(columns[i]);
  }
};     
//------------------------------------------------------------------
//  
void                             gConf::write_table(const std::string &filename){
 
  std::vector <std::string>                     keys;
  std::vector < std::vector < std::string > * > values;
  std::vector <size_t>                          column_width;
  size_t                                        num_of_rows = 0;

  for(size_t _ki = 0; _ki < _keys.size(); ++_ki) {
    if(_keys[_ki].size() > 0) {
      values.push_back(&_values[_keys[_ki]]);
      column_width.push_back(_keys[_ki].size());
      keys.push_back(_keys[_ki]);
    }
  }

  for(size_t ki = 0; ki < keys.size(); ++ki) { 
    // define num_of_rows
    if(values[ki]->size() > num_of_rows) num_of_rows = values[ki]->size();
    // define column_width:
    for(size_t ii = 0; ii  < values[ki]->size(); ++ii) {
      if (column_width[ki] < (*(values[ki]))[ii].size()) column_width[ki] = (*(values[ki]))[ii].size();
    }
  }

  // write to file:
  std::string line = "";
  // - title
  for(size_t ki = 0; ki < keys.size(); ++ki) {
      line = line + gFile.format_to_str( keys[ki], column_width[ki] + 2, "left");
  }
  line += "\n";
  // - values  
  for(size_t ii = 0; ii < num_of_rows; ++ii) {
    for(size_t ki = 0; ki < keys.size(); ++ki) {
      if( ii < values[ki]->size() ) {
        line = line + gFile.format_to_str( (*(values[ki]))[ii], column_width[ki] + 2, "left");
      } else {
        line = line + gFile.format_to_str( " ", column_width[ki] + 2, "left");
      }
    }
    line += "\n";


  }
  std::ofstream tmp(filename.c_str(), std::ios_base::out);
    if(tmp.is_open()) {
      tmp << line;
      tmp.close();
    } else {
    std::cout <<"ERROR: "<< __FILE__ << " : " << __LINE__  << ": write_table(): couldn't write file " << filename << std::endl;
  }

};     

//------------------------------------------------------------------
//  
void                             gConf::change(const std::string & key, const std::string & new_value, int index=-1){

  int i = _ii_keys(key);
  if(i >= 0) {

    _s_values[i] = ""; 
    const std::vector <std::string> & ref_values (_values[key]) ; 
    std::string new_value_line = "";

    if(index >=0 and index < int(ref_values.size()) ) {
      for(size_t i = 0; i < ref_values.size(); ++i) {
        if (int(i) == index) {
          new_value_line =  new_value_line + "  " + new_value;
          continue;
        }
        new_value_line =  new_value_line + "  " + ref_values[i];
      }
    } else {
      new_value_line = new_value;
    }
    (_values[key]).clear();
    add(key + "   " + new_value_line);

  }

  // DEBUG(line $ " " $ new_value);
}  
//------------------------------------------------------------------
//  
 


#endif