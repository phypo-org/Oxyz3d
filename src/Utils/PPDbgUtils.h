#ifndef  H__PPDBG_UTILS__H
#define  H__PPDBG_UTILS__H

#include <iostream>

#include "Utils/PPSingletonCrtp.h"


namespace PPu {

  struct  PPDbgUtils : public virtual PPSingletonCrtp<PPDbgUtils> {
    int  cDbgFile    = 0;
    int  cDbgConf  = 0;


    friend class PPSingletonCrtp;
  };
  
} // end namespace

#define PPuDbg PPu::PPDbgUtils::Instance()


#define DBG_FIL(A ) if(PPu::PPDbgUtils::Instance().cDbgFile >  0 ) std::cout << "DbgFile>" << A << std::endl;
#define DBG_FIL2(A) if(PPu::PPDbgUtils::Instance().cDbgFile >= 1 ) std::cout << "DbgFile2>" << A << std::endl;
#define DBG_FIL3(A) if(PPu::PPDbgUtils::Instance().cDbgFile >= 2 ) std::cout << "DgbFile3>" << A << std::endl;

#define DBG_CONF(A) if(PPu::PPDbgUtils::Instance().cDbgConf >  0 ) std::cout << "DbgConf>"  << A << std::endl;

#endif


