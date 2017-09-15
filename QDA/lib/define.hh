#define ERROR(x) std::cout <<"ERROR: "<< __FILE__ << " : " << __LINE__  << " : " << x << std::endl;
#define DEBUG(x) std::cout <<"DEBUG: "<< __FILE__ << " : " << __LINE__  << " : " << x << std::endl;
#define $ << "  " << 
#define $$ << std::endl    ;
#define $$$(x)  std::cout << #x << ": " << x << std::endl;  
#define  REP(i,size) for(unsigned int i=0; i < size; ++i) 
#define RREP(i,size1, size2) for(unsigned int i=size1; i < size2; ++i) 