#include "ErrLog.h"

#include <FL/fl_ask.H>

namespace PPu {

  using namespace std;
  

  //***************************************************
  ErrLog::ErrLog()
  {
    ;
  }
  //-----------------------------------------------
  ErrLog::~ErrLog()
  {
    ;
  }  
  //-----------------------------------------------
  void
  ErrLog::init( std::ostream *iOut, std::ostream *iErr, std::ostream *iConsoleOut )
  {
    cOut = iOut;
    cErr = iErr;
    cConsoleOut = iConsoleOut;
  }
  //-----------------------------------------------
  std::ostream &
  ErrLog::initMsg( LogTypeMsg iTypeMsg, bool iAlert )
  {
    cTypeMsg = iTypeMsg;
    cAlert   = iAlert;
    
    return cOs;
  }
  //-----------------------------------------------
  void
  ErrLog::trace()
  {
    if( cTypeMsg == LogMsgInfo)
      {
        if( cOut != nullptr )
          *cOut << GetLogTypeMsg(cTypeMsg ) << cOs.str() << endl;
      }
    else
      {
        if( cErr != nullptr )
          *cErr << GetLogTypeMsg(cTypeMsg) << cOs.str() << endl;
      }
    
    if( cConsoleOut != nullptr )
      {
        *cConsoleOut << GetLogTypeMsg(cTypeMsg) << cOs.str() << endl;      
      }
    
    if( cAlert )
      {         
        fl_message_title( GetAlertTypeMsg( cTypeMsg ).c_str());
        switch( cTypeMsg )
          {
          case LogMsgInfo:
            fl_message( "%s", cOs.str().c_str() );
            break;
          case LogMsgWarning:
          case LogMsgError:            
          case LogMsgErrno:
          case LogMsgCritic:
          case LogMsgFatal:
            fl_alert( "%s",  cOs.str().c_str());
            break;
          }
      }
    cOs.str("");
    cOs.clear();
  }
  //-----------------------------------------------
  const std::string &
  ErrLog::GetLogTypeMsg  ( LogTypeMsg iTyp)
  {
    return LogTypeMsgStr[iTyp];
  }
  //-----------------------------------------------  
  const std::string  &
  ErrLog::GetAlertTypeMsg( LogTypeMsg iTyp)
  {
    return AlertTypeMsgStr[iTyp];
  }  
  //***************************************************
 

} // namespace 
