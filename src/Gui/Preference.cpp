#include "Preference.h"

#include "Shape/DebugVars.h"

#include "Utils/PPErrLog.h"
#include "Utils/PPDbgUtils.h"

#include "Shape/DebugVars.h"
#include "Create/Creation.h"

namespace M3d {

  
  //*********************************
  Preference::Preference()
    :cDbgDrw( PP3dDbg.cDbgDrw  )
    ,cDbgBaz( PP3dDbg.cDbgBaz  )
    ,cDbgSel( PP3dDbg.cDbgSel  )
    ,cDbgFil( PPuDbg.cDbgFile )
    ,cDbgIni( PPuDbg.cDbgConf )
  {
  }
  //---------------------------------------
  //#define INIT_ALL_DBG() 
  
#define INIT_DBG( VAR )   iConfig.get( "Debug",  "Dbg"   #VAR, cDbg ## VAR  )
#define INIT_SEL( VAR )   iConfig.get( "Select", "Sel"   #VAR, cSelect ## VAR  )
#define INIT_HISTO( VAR ) iConfig.get( "Histo",  "Histo" #VAR, cHisto ## VAR  )
#define INIT_FILE( VAR )  iConfig.get( "File",   "File"  #VAR, cFile ## VAR  )

  //---------------------------------------
  void Preference::resetToDefault()
  {
    cLastSave              = sSaveDefault;
    
    cFileAutoSave          = true;
    cFileAutoSaveFrequency = 60;
    cFileDefaultDir        = "Base";
    
    cMouseWheel            = 15;

    cDbgEvt = cDbgAct = cDbgTree = cDbgDrw = cDbgBaz = cDbgSel = cDbgFil = cDbgIni = 0;
    
  }
  //---------------------------------------
  void Preference::resetToFile()
  {
    resetToDefault();
    initFromIni( Creation::Instance().getConfig() );    
  }
  //---------------------------------------
  //---------------------------------------
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
    
    int lUndoLevel ;    
    if( iConfig.get( "Select",  "UndoLevel", lUndoLevel ))
      {
	PP3d::UndoHistory::Instance().setMaxHisto(lUndoLevel);
      }

      
    INIT_FILE( AutoSave  );
    INIT_FILE( AutoSaveFrequency  );
    INIT_FILE( DefaultDir  );
   
  }
  //---------------------------------------
  void Preference::initFromArgs( PPu::PPArgs & iArgs )
  {
    int lVerbose = 0;
    if(  iArgs.exist( "-v" )) 
       PPu::PPErrLog::sVerbose  =1;
    
    if( iArgs.get( "-verbose", lVerbose ))
      PPu::PPErrLog::sVerbose = lVerbose;

    VERBOSELN( "Preference::initFromArgs verbose:" <<  PPu::PPErrLog::sVerbose );
  }
  //---------------------------------------
  //---------------------------------------
  //---------------------------------------
#undef INIT_DBG
#define INIT_DBG( VAR )  iConfig.set( "Debug", "Dbg" #VAR, cDbg ## VAR  )

#undef INIT_SEL  
#define INIT_SEL( VAR )  iConfig.set( "Select", "Sel" #VAR, cSelect ## VAR  )

#undef INIT_HISTO
#define INIT_HISTO( VAR )  iConfig.set( "Histo", "Histo" #VAR, cHisto ## VAR  )

#undef INIT_FILE  
#define INIT_FILE( VAR )  iConfig.set( "File", "File" #VAR,  cFile ## VAR  )

  //---------------------------------------
  bool Preference::saveInCurrentIni()
  {
    return saveInIni( Creation::Instance().getConfig() );
  }
  //---------------------------------------

  bool Preference::saveInIni( PPu::PPConfig & iConfig )
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
    
    //    INIT_SEL( UndoLevel );
    iConfig.set( "Select",  "UndoLevel", PP3d::UndoHistory::Instance().getMaxHisto() );
 
    INIT_FILE( AutoSave  );
    INIT_FILE( AutoSaveFrequency  );
    INIT_FILE( DefaultDir  );
    return  Creation::Instance().getConfig().writeToFile(); 
  }
  //---------------------------------------
  //---------------------------------------
  //---------------------------------------

  
#undef INIT_DBG
#define INIT_DBG( VAR )  iArgs.get( "Dbg" #VAR, cDbg ## VAR )

#undef INIT_SEL  
#define INIT_SEL( VAR )  iArgs.get( "Sel" #VAR, cSelect  ## VAR )
  
#undef INIT_HISTO  
#define INIT_HISTO( VAR )  iArgs.get( "Histo" #VAR, cHisto  ## VAR )
  
#undef INIT_FILE  
#define INIT_FILE( VAR )  iArgs.get( "File" #VAR, cFile  ## VAR )

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
    
    //    INIT_SEL( UndoLevel );    
    int lUndoLevel ;    
    if( iArgs.get( "SelectUndoLevel", lUndoLevel ))
      {
	PP3d::UndoHistory::Instance().setMaxHisto((size_t)lUndoLevel);
      }

    INIT_FILE( AutoSave  );
    INIT_FILE( AutoSaveFrequency  );
    INIT_FILE( DefaultDir  );
  }
  //*********************************
 
  
} // end namespace