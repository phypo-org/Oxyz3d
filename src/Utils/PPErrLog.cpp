#include "PPErrLog.h"

#include <FL/fl_ask.H>

namespace PPu {

  using namespace std;

  int PPErrLog::sVerbose=0;

  //***************************************************
  PPErrLog::PPErrLog()
  {
    ;
  }
  //-----------------------------------------------
  PPErrLog::~PPErrLog()
  {
    ;
  }  
  //-----------------------------------------------
  void
  PPErrLog::init( std::ostream *iOut, std::ostream *iErr, std::ostream *iConsoleOut )
  {
    cOut = iOut;
    cErr = iErr;
    cConsoleOut = iConsoleOut;
  }
  //-----------------------------------------------
  std::ostream &
  PPErrLog::initMsg( LogTypeMsg iTypeMsg, bool iAlert )
  {
    cTypeMsg = iTypeMsg;
    cAlert   = iAlert;
    
    return cOs;
  }
  //-----------------------------------------------
  void
  PPErrLog::trace()
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
  PPErrLog::GetLogTypeMsg  ( LogTypeMsg iTyp)
  {
    return LogTypeMsgStr[iTyp];
  }
  //-----------------------------------------------  
  const std::string  &
  PPErrLog::GetAlertTypeMsg( LogTypeMsg iTyp)
  {
    return AlertTypeMsgStr[iTyp];
  }  
  //***************************************************
 

} // namespace 
