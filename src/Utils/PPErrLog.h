#ifndef H__ERRLOG__H
#define H__ERRLOG__H


#include "PPSingletonCrtp.h"

#include <string>
#include <iostream>
#include <sstream>
#include <cstring>

namespace PPu {
  
  struct  PPDebugPPu : public virtual PPSingletonCrtp<PPDebugPPu> {
    int  cDbgFil    = 0;
    int  cDbgIni    = 0;
    
    friend class PPSingletonCrtp;
  }; 


  enum LogTypeMsg{
    LogMsgInfo,
    LogMsgWarning,
    LogMsgError,
    LogMsgErrno,
    LogMsgCritic,
    LogMsgFatal
  };
  
  const std::string LogTypeMsgStr[] = {
    " --- Info     : ",
    " +++ Warning  : ",   
    " *** Error    : ",        
    " *** Error    : ",        
    " *** Critical : ",
    " *** Fatal    : ",
  };

  const std::string AlertTypeMsgStr[] = {
    "  Info     ",
    "  Warning  ",   
    "  Error    ",        
    "  Error    ",        
    "  Critical ",
    "  Fatal    ",
  };

 
  //***************************************************
  class PPErrLog : public  PPSingletonCrtp<PPErrLog>{
  public:
    static int          sVerbose;
    
    std::ostringstream  cOs;
    LogTypeMsg          cTypeMsg = LogMsgInfo;
    bool                cAlert   = true;
  
    std::ostream *cOut        = nullptr;
    std::ostream *cErr        = nullptr;
    std::ostream *cConsoleOut = nullptr;

  public:
    PPErrLog();    
    virtual ~PPErrLog();
    void init( std::ostream *iOut, std::ostream *iErr, std::ostream *cConsoleOut ); 
    std::ostream & initMsg( LogTypeMsg iTypeMsg, bool iAlert );
    void trace( );

    
    static const std::string & GetLogTypeMsg  ( LogTypeMsg iTyp);
    static const std::string & GetAlertTypeMsg( LogTypeMsg iTyp);

    static void SetVerbose( int iVerbose ) { sVerbose = iVerbose;}
    static int  Verbose()                  { return sVerbose;}
  };
  
} // namespace 

//***************************************************


#define VERBOSE(A)   if( PPu::PPErrLog::sVerbose )     std::cout << "<" << A << "> ";
#define VERBOSELN(A) if( PPu::PPErrLog::sVerbose )     std::cout << "<" << A << ">" << std::endl;

#define VERBOSE2(A)   if( PPu::PPErrLog::sVerbose >=2) std::cout << "<<" << A << ">> ";
#define VERBOSELN2(A) if( PPu::PPErrLog::sVerbose >=2) std::cout << "<<" << A << ">>" << std::endl;

#define VERBOSE3(A)   if( PPu::PPErrLog::sVerbose  >=3) std::cout << "<<<" << A << ">>> ";
#define VERBOSELN3(A) if( PPu::PPErrLog::sVerbose  >=3) std::cout << "<<<" << A << ">>>" << std::endl;
    
#define VERBOSE4(A)   if( PPu::PPErrLog::sVerbose  >=4) std::cout << "<<<" << A << ">>> ";
#define VERBOSELN4(A) if( PPu::PPErrLog::sVerbose  >=4) std::cout << "<<<" << A << ">>>" << std::endl;

#define STRERR_OUT(A, B) PPErrLog::Instance().PPErrLog::Instance().cOs

// Trace dans le log (pour le moment sur cerr : TODO )
#define INFO_LOG( A )    { PPu::PPErrLog::Instance().initMsg(PPu::LogMsgInfo,    false ) << A; PPu::PPErrLog::Instance().trace();}
#define WARN_LOG( A )    { PPu::PPErrLog::Instance().initMsg(PPu::LogMsgWarning, false ) << A; PPu::PPErrLog::Instance().trace();}
#define ERR_LOG( A )     { PPu::PPErrLog::Instance().initMsg(PPu::LogTypeMsg::LogMsgError,   false ) << A; PPu::PPErrLog::Instance().trace();}
#define ERRNO_LOG( A )   { PPu::PPErrLog::Instance().initMsg(PPu::LogMsgErrno,   false ) << " Errno : "<< errno <<" : "<< strerror(errno) << A; PPu::PPErrLog::Instance().trace();}
#define CRITIC_LOG( A )  { PPu::PPErrLog::Instance().initMsg(PPu::LogMsgCritic,  false ) << A; PPu::PPErrLog::Instance().trace();}
#define FATAL_LOG( A )   { PPu::PPErrLog::Instance().initMsg(PPu::LogMsgFatal,   false ) << A; PPu::PPErrLog::Instance().trace();}


#define INFO_DIAG( A )    { PPu::PPErrLog::Instance().initMsg(PPu::LogMsgInfo,    true ) << A; PPu::PPErrLog::Instance().trace();}
#define WARN_DIAG( A )    { PPu::PPErrLog::Instance().initMsg(PPu::LogMsgWarning, true ) << A; PPu::PPErrLog::Instance().trace();}
#define ERR_DIAG( A )     { PPu::PPErrLog::Instance().initMsg(PPu::LogMsgError,   true ) << A; PPu::PPErrLog::Instance().trace();}
#define ERRNO_DIAG( A )   { PPu::PPErrLog::Instance().initMsg(PPu::LogMsgErrno,   true ) << " Errno : "<< errno <<" : "<< strerror(errno) << A; PPu::PPErrLog::Instance().trace();}
#define CRITIC_DIAG( A )  { PPu::PPErrLog::Instance().initMsg(PPu::LogMsgCriti,   true ) << A; PPu::PPErrLog::Instance().trace();}
#define FATAL_DIAG( A )   { PPu::PPErrLog::Instance().initMsg(PPu::LogMsgFatal,   true ) << A; PPu::PPErrLog::Instance().trace();}



#endif
