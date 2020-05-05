#include "Preference.h"

#include "Shape/DebugVars.h"

#include "Utils/ErrLog.h"

#include "Shape/DebugVars.h"

namespace M3d {

  
  //*********************************
  Preference::Preference()
    :cDbgDrw( MyDbg.cDbgDrw  )
    ,cDbgBaz( MyDbg.cDbgBaz  )
    ,cDbgSel( MyDbg.cDbgSel  )
    ,cDbgFil( PPuDbg.cDbgFil )
    ,cDbgIni( PPuDbg.cDbgIni )
  {
  }
  //---------------------------------------
  //#define INIT_ALL_DBG() 
  
#define INIT_DBG( VAR )  iConfig.get( "Debug", "cDbg ## VAR", cDbg ## VAR  )
  
  void Preference::initFromIni( PPu::PPConfig & iConfig )
  {
    INIT_DBG( Evt );
    INIT_DBG( Act );
    INIT_DBG( Lua );
    INIT_DBG( Tree );
    INIT_DBG( Drw );
    INIT_DBG( Baz );
    INIT_DBG( Sel );
    INIT_DBG( Fil );
    INIT_DBG( Ini );		      
  }
#undef INIT_DBG
#define INIT_DBG( VAR )  iArgs.get( "cDbg ## VAR", cDbg ## VAR )
  //---------------------------------------
  void Preference::initFromArg( PPu::PPArgs & iArgs )
  {
    INIT_DBG( Evt );
    INIT_DBG( Act );
    INIT_DBG( Lua );
    INIT_DBG( Tree );
    INIT_DBG( Drw );
    INIT_DBG( Baz );
    INIT_DBG( Sel );
    INIT_DBG( Fil );
    INIT_DBG( Ini );		      
  }
  //*********************************
 
  
} // end namespace
