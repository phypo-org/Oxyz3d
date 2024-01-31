#ifndef H__PPExcept__H
#define H__PPExcept__H


#include <string>
#include <sstream>

namespace PPu {

  //*****************************
  class PPExcept {
 
  public: 
    std::string   cSender;
    std::string   cDetail;
    const char *  cFile;
    int           cLine;
    long long     cCode;
    
    PPExcept(const char * iSender, const char * iDetail, const char * iFile = nullptr, int iLine = -1, long iCode = -1L )
      :cSender(iSender), cDetail(iDetail), cFile(iFile), cLine(iLine), cCode(iCode)
    {
    }
    PPExcept(const std::string & iSender, const std::string & iDetail, const char* iFile =nullptr, int iLine = -1, long iCode = -1L )
      :cSender(iSender), cDetail(iDetail), cFile(iFile), cLine(iLine), cCode(iCode)
    {
    }

    PPExcept & operator=( const PPExcept & iExcept )
    {
      cSender = iExcept.cSender;
      cDetail = iExcept.cDetail;
      cFile   = iExcept.cFile;
      cLine   = iExcept.cLine;
      cCode   = iExcept.cCode;
      return *this;
    }

    std::string toString() const
    {
      std::ostringstream lOs;
      lOs << *this;
      return lOs.str();
    }
  
 
    friend std::ostream & operator <<(std::ostream & pOs, const PPExcept & iExcept ) 
    {
      pOs << "Exception throw by " << iExcept.cSender << " : " << iExcept.cDetail ;
      if( iExcept.cFile != nullptr )
	pOs << " at " << iExcept.cFile << ":" << iExcept.cLine ;
      if( iExcept.cCode > 0 )
	pOs << " - code : " << iExcept.cCode;
      return pOs;
    }
    //*****************************
  };
  
} // namespace

#define    THROW_EXCEPT     ( SENDER, DETAIL )       { std::ostringstream lOs ; lOs << DETAIL; throw PPu::PPExcept( SENDER, lOs.str(), __FILE__, __LINE__ ) }
#define    THROW_EXCEPT_CODE( SENDER, DETAIL, CODE ) { std::ostringstream lOs ; lOs << DETAIL; throw PPu::PPExcept( SENDER, lOs.str(), __FILE__, __LINE__, CODE ) } }



#endif
