#ifndef H__PPARGS__H
#define H__PPARGS__H

#include <string>
#include <vector>


namespace PPu {

  //****************************
  class PPArgs {
    
    std::vector<std::string> cVectParam;

  public:
    PPArgs( int iArgc, char * iArgv[], char iSplitEgal='=' );
    
    int exist( const char* lVar);
    std::string getValue(const char* lVar);
    
    bool get ( const char* iVar, std::string &oValue );            
    bool get ( const char* iVar, int & oValue );
    bool get ( const char* iVar, long & oValue );
    bool get ( const char* iVar, long long & oValue );    
    bool get ( const char* iVar, float & oValue );
    bool get ( const char* iVar, double & oValue );
    bool get ( const char* iVar, long double & oValue );
    bool get ( const char* iVar, bool & oValue );
  };
  //****************************

} // namespace
#endif
