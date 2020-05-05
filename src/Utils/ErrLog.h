#ifndef H__ERRLOG__H
#define H__ERRLOG__H


#include "PPSingletonCrtp.h"

#include <string>
#include <iostream>
#include <string.h>

namespace PPu {
  
  struct  PPDebugPPu : public virtual PPSingletonCrtp<PPDebugPPu> {
    int  cDbgFil    = 0;
    int  cDbgIni    = 0;

    friend class PPSingletonCrtp;
  }; 
} // end namespace
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
    

// Trace dans le log (pour le moment sur cerr : TODO )
#define WARN_LOG( A )    std::cerr << "Warning:" << A << std::endl;
#define ERR_LOG( A )     std::cerr << "Error:" << A << std::endl;
#define ERRNO_LOG( A )   std::cerr << "Errno:" << errno << ":" << ::strerror(errno) <<":"<<  A << std::endl;
#define CRITIC_LOG( A )  std::cerr << "Critic:" << A << std::endl;
#define FATAL_LOG( A )   std::cerr << "Fatal:" << A << std::endl;


#define TRACE(A)        std::cout << " --- "                  << A << std::endl;
#define TRACE_LOG(A)    TRACE(A)
#define TRACE_WARN(A)   std::cerr << " +++ Warning : "        << A << std::endl;
#define TRACE_ERR(A)    std::cerr << " *** Error : "          << A << std::endl;
#define TRACE_CRITIC(A) std::cerr << " *** Critical error : " << A << std::endl;
#define TRACE_FATAL(A)  std::cerr  << " *** Fatal error : " << A << std::endl;

#endif
