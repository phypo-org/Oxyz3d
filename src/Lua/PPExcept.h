#ifndef   __PP3D_EXCEPT__
#define   __PP3D_EXCEPT__

#include <string>
#include <sstream>


namespace PP3d {

  //*********************************
  class Except {

  public:                  
    std::string  cSender;
    std::string  cDetail;
    long long    cCode;
    const char*  cFile;
    int          cLine;
   
    std::string  cWhat;

    
    Except(const char* iSender, const char* iDetail, long iCode = 0, const char* iFile =nullptr, int iLine = -1)
      :cSender(iSender), cDetail(iDetail), cCode(iCode), cFile(iFile), cLine(iLine)
    {
    }
    Except(const std::string& iSender, const std::string& iDetail, long iCode = -1L, const char* iFile =nullptr, int iLine = -1)
      :cSender(iSender), cDetail(iDetail), cCode(iCode), cFile(iFile), cLine(iLine)
    {
    }

    std::string toString() const
    {
       std::ostringstream lOs;
       lOs << *this;
       return lOs.str();
    }

    virtual const char* what() 
    {
      cWhat = toString();
      return cWhat.c_str(); 
    }

    Except& operator=( const Except& iOther )
    {
      cSender = iOther.cSender;
      cDetail = iOther.cDetail;
      cCode   = iOther.cCode;
      cFile   = iOther.cFile;
      cLine   = iOther.cLine;
      // no need : cWhat = iOther.cWhat;

      return *this;
    }

    friend std::ostream& operator <<(std::ostream& pOs, const Except& iExcept )
    {
      pOs << "Exception throw by " << iExcept.cSender << " - " << iExcept.cDetail ;
      if( iExcept.cCode != 0 )
	pOs << " : " << iExcept.cCode;
      if( iExcept.cFile != nullptr )
	pOs << " at " << iExcept.cFile << ":" << iExcept.cLine ;
      return pOs;
    }
	
  };
   //*********************************
 
#define    THROW_CODE( SENDER, DETAIL, CODE ) throw PP3d::Except( SENDER, DETAIL, CODE, __FILE__, __LINE__ );
#define    THROW( SENDER, DETAIL ) THROW__CODE( SENDER, DETAIL, 0 )
  
#define    THROW_STREAM_CODE( SENDER, DETAIL, CODE ) { std::ostringstream lOs ; lOs << DETAIL; THROW_CODE( SENDER, lOs.str(), CODE ) }
#define    THROW_STREAM( SENDER, DETAIL ) THROW_STREAM_CODE( SENDER, DETAIL, 0 )


  

} // end namespace 

#endif
