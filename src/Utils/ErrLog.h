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
  class ErrLog : public  PPSingletonCrtp<ErrLog>{
  public:
    std::ostringstream  cOs;
    LogTypeMsg          cTypeMsg = LogMsgInfo;
    bool                cAlert   = true;
  
    std::ostream *cOut        = nullptr;
    std::ostream *cErr        = nullptr;
    std::ostream *cConsoleOut = nullptr;

  public:
    ErrLog();    
    virtual ~ErrLog();
    void init( std::ostream *iOut, std::ostream *iErr, std::ostream *cConsoleOut ); 
    std::ostream & initMsg( LogTypeMsg iTypeMsg, bool iAlert );
    void trace( );

    
    static const std::string & GetLogTypeMsg  ( LogTypeMsg iTyp);
    static const std::string & GetAlertTypeMsg( LogTypeMsg iTyp);

  };
} // namespace 

//***************************************************

#define PPuDbg PPu::PPDebugPPu::Instance()

#define DBG_FIL( A )  if( PPuDbg.cDbgFil > 0 ) std::cout << "DbgFil0> " << A << std::endl;
#define DBG_FIL1( A ) if( PPuDbg.cDbgFil > 1 ) std::cout << "DbgFil1> " << A << std::endl;
#define DBG_FIL2( A ) if( PPuDbg.cDbgFil > 2 ) std::cout << "DgbFil2> " << A << std::endl;

#define DBG_INI( A )  if( PPuDbg.cDbgIni > 0 ) std::cout << "DbgIni0> " << A << std::endl;
#define DBG_INI1( A ) if( PPuDbg.cDbgIni > 1 ) std::cout << "DbgIni1> " << A << std::endl;
#define DBG_INI2( A ) if( PPuDbg.cDbgIni > 2 ) std::cout << "DgbIni2> " << A << std::endl;


#define VERBOSE(A)   if( sVerboseCSam )     std::cout << "<" << A << "> ";
#define VERBOSELN(A) if( sVerboseCSam )     std::cout << "<" << A << ">" << std::endl;

#define VERBOSE2(A)   if( sVerboseCSam >1) std::cout << "<<" << A << ">> ";
#define VERBOSELN2(A) if( sVerboseCSam >1) std::cout << "<<" << A << ">>" << std::endl;

#define VERBOSE3(A)   if( sVerboseCSam  >2) std::cout << "<<<" << A << ">>> ";
#define VERBOSELN3(A) if( sVerboseCSam  >2) std::cout << "<<<" << A << ">>>" << std::endl;
    

#define STRERR_OUT(A, B) ErrLog::Instance().ErrLog::Instance().cOs

// Trace dans le log (pour le moment sur cerr : TODO )
#define INFO_LOG( A )    PPu::ErrLog::Instance().initMsg(PPu::LogMsgInfo,    false ) << A; PPu::ErrLog::Instance().trace();
#define WARN_LOG( A )    PPu::ErrLog::Instance().initMsg(PPu::LogMsgWarning, false ) << A; PPu::ErrLog::Instance().trace();
#define ERR_LOG( A )     PPu::ErrLog::Instance().initMsg(PPu::LogMsgError,   false ) << A; PPu::ErrLog::Instance().trace();
#define ERRNO_LOG( A )   PPu::ErrLog::Instance().initMsg(PPu::LogMsgErrno,   false ) << " Errno : "<< errno <<" : "<< strerror(errno) << A; PPu::ErrLog::Instance().trace();
#define CRITIC_LOG( A )  PPu::ErrLog::Instance().initMsg(PPu::LogMsgCritic,  false ) << A; PPu::ErrLog::Instance().trace();
#define FATAL_LOG( A )   PPu::ErrLog::Instance().initMsg(PPu::LogMsgFatal,   false ) << A; PPu::ErrLog::Instance().trace();


#define INFO_DIAG( A )    PPu::ErrLog::Instance().initMsg(PPu::LogMsgInfo,    true ) << A; PPu::ErrLog::Instance().trace();
#define WARN_DIAG( A )    PPu::ErrLog::Instance().initMsg(PPu::LogMsgWarning, true ) << A; PPu::ErrLog::Instance().trace();
#define ERR_DIAG( A )     PPu::ErrLog::Instance().initMsg(PPu::LogMsgError,   true ) << A; PPu::ErrLog::Instance().trace();
#define ERRNO_DIAG( A )   PPu::ErrLog::Instance().initMsg(PPu::LogMsgErrno,   true ) << " Errno : "<< errno <<" : "<< strerror(errno) << A; PPu::ErrLog::Instance().trace();
#define CRITIC_DIAG( A )  PPu::ErrLog::Instance().initMsg(PPu::LogMsgCriti,   true ) << A; PPu::ErrLog::Instance().trace();
#define FATAL_DIAG( A )   PPu::ErrLog::Instance().initMsg(PPu::LogMsgFatal,   true ) << A; PPu::ErrLog::Instance().trace();



#endif
