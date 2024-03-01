#ifndef H__PPCONFIG__H
#define H__PPCONFIG__H


#include <cstdint>
#include <string>
#include <iostream>
#include <memory>
#include <map>


#include "PPErrLog.h"


#include "PPNonCopyable.h"
#include "PPFile.h"

namespace PPu {



  //**************************************
  // Pour les fichiers .ini
  
  class PPConfig : public PPNonCopyable
    {
    protected:
      
      const std::string kSep = "#|@";
      
      std::string	cFilePathParam    = "--config:";
      std::string       cName="config.ini";
      
      std::map< std::string, std::string>   cData;
  
    public:

      PPConfig() {;}
      virtual ~PPConfig() {;}
      
      bool readFile( const std::string & iFile );
      bool readFile( std::istream &pIn );
      bool readFile( int iArgc, char *iArgv[] );
      bool readFile()
      {
        return readFile( cName );
      }

      bool writeToFile() { return writeToFile( cName ); }
      bool writeToFile( const std::string& iFileName  );
      bool writeToStream( std::ostream & oOut  ) const;

      void addVar( const std::string& iEntity, const std::string& iVar, const std::string& iVal) ;

      auto & getData() { return cData; }

      void setParamToken( std::string& iVal) { cFilePathParam=iVal;}

      bool readParam( int iArgc, char *iArgv[] );
      void setFilename( const std::string & iName) { cName = iName; }
      const std::string & getFilename() const { return cName; }

 

    protected:
      bool internalReadString( const char* iSection, const char* iVariable, std::string & ioValue );
      bool internalWriteString( const char* iSection, const char* iVariable, std::string & ioValue );

    public:    
      bool get ( const char* iSection, const char* iVariable, std::string &oValue );
      
      bool get ( const char* iSection, const char* iVariable, int & oValue );
      bool get ( const char* iSection, const char* iVariable, long & oValue );
      bool get ( const char* iSection, const char* iVariable, long long & oValue );
     
      bool get ( const char* iSection, const char* iVariable, float & oValue );
      bool get ( const char* iSection, const char* iVariable, double & oValue );
      bool get ( const char* iSection, const char* iVariable, long double & oValue );
      bool get ( const char* iSection, const char* iVariable, bool & oValue );

      bool set ( const char* iSection, const char* iVariable, const std::string & iValue );
      bool set ( const char* iSection, const char* iVariable, const char* iValue );

      template <class TYPE>
      bool set( const char* iSection, const char* iVariable, const TYPE iValue )
      {
	return  set( iSection, iVariable, std::to_string( iValue ) );
      }

      void print( std::ostream& iOs ) const ;
      //
      friend inline std::ostream & operator << (std::ostream& iOs, const PPConfig&iConf ) { iConf.print( iOs); return iOs; }
      
  };
  //**************************************

  
} //end namespace
      
#endif 
