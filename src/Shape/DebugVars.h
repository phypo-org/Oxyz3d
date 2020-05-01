#ifndef  H__DEBUG_VARS__H
#define  H__DEBUG_VARS__H

#include <ostream>

#include "Utils/PPSingletonCrtp.h"


namespace PP3d {

  struct  DebugVars : public virtual PPSingletonCrtp<DebugVars> {
    int  cDbgDrw    = 0;
    int  cDbgBaz    = 0;
    int  cDbgSel  = 0;

    friend class PPSingletonCrtp;
  }; 
} // end namespace


#define MyDbg PP3d::DebugVars::Instance()

#define DBG_DRW( A )  if( MyDbg.cDbgDrw > 0 ) std::cout << "DbgDrw0> " << A << std::endl;
#define DBG_DRW1( A ) if( MyDbg.cDbgDrw > 1 ) std::cout << "DbgDrw1> " << A << std::endl;
#define DBG_DRW2( A ) if( MyDbg.cDbgDrw > 2 ) std::cout << "DgbDrw2> " << A << std::endl;
  
#define DBG_BAZ( A )  if( MyDbg.cDbgBaz > 0 ) std::cout << "DbgBaz0> " << A << std::endl;
#define DBG_BAZ1( A ) if( MyDbg.cDbgBaz > 1 ) std::cout << "DgbBaz1> " << A << std::endl;
#define DBG_BAZ2( A ) if( MyDbg.cDbgBaz > 2 ) std::cout << "DgbBaz2> " << A << std::endl;

#define DBG_SEL( A )  if( MyDbg.cDbgSel > 0 ) std::cout << "DbgSel0> " << A << std::endl;
#define DBG_SEL1( A ) if( MyDbg.cDbgSel > 1 ) std::cout << "DbgSel1> " << A << std::endl;
#define DBG_SEL2( A ) if( MyDbg.cDbgSel > 2 ) std::cout << "DbgSel2> " << A << std::endl;
#define DBG_SEL_NL( A )  if( MyDbg.cDbgSel > 0 ) { std::cout << "DbgSel> " << A ; }

#define DBG_SEL_LN( A ) DBG_SEL(A)
#define DBG_SEL_NN( A )  if( MyDbg.cDbgSel > 0 ) std::cout << A ;


#endif


