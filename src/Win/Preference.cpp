#include "Preference.h"

#include "Shape/DebugVars.h"

#include "Utils/ErrLog.h"

#include "Shape/DebugVars.h"
#include "Application.h"

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
  
#define INIT_DBG( VAR )  iConfig.get( "Debug", "Dbg ## VAR", cDbg ## VAR  )

#define INIT_SEL( VAR )  iConfig.get( "Select", "## VAR", cSelect ## VAR  )

  void Preference::resetToDefault()
  {
    cLastSave = sSaveDefault;
    cAutoSave = true;
    cMouseWheel = 15;

    cDbgEvt = cDbgAct = cDbgTree = cDbgDrw = cDbgBaz = cDbgSel = cDbgFil = cDbgIni = 0;
    
  }
  //---------------------------------------
  void Preference::resetToFile()
  {
    resetToDefault();
    initFromIni( Application::Instance().getConfig() );    
  }
  //---------------------------------------
  void Preference::initFromIni( PPu::PPConfig & iConfig )
  {
    INIT_DBG( Evt );
    INIT_DBG( Act );
#ifdef USING_LUA        
    INIT_DBG( Lua );
#endif    
    INIT_DBG( Tree );
    INIT_DBG( Drw );
    INIT_DBG( Baz );
    INIT_DBG( Sel );
    INIT_DBG( Fil );
    INIT_DBG( Ini );

    INIT_SEL( PickingSize );
  }
#undef INIT_DBG
#define INIT_DBG( VAR )  iArgs.get( "Dbg ## VAR", cDbg ## VAR )

#undef INIT_SEL  
#define INIT_SEL( VAR )  iArgs.get( "Select ## VAR", cSelect  ## VAR )
  
#undef INIT_HISTO  
#define INIT_HISTO( VAR )  iArgs.get( "Histo ## VAR", cHisto  ## VAR )
  //---------------------------------------
  void Preference::initFromArg( PPu::PPArgs & iArgs )
  {
    INIT_DBG( Evt );
    INIT_DBG( Act );
#ifdef USING_LUA        
    INIT_DBG( Lua );
#endif    
    INIT_DBG( Tree );
    INIT_DBG( Drw );
    INIT_DBG( Baz );
    INIT_DBG( Sel );
    INIT_DBG( Fil );
    INIT_DBG( Ini );
    
    INIT_SEL( PickingSize );

  }
  //*********************************
 
  
} // end namespace
