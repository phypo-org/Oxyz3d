#include "Preference.h"

#include "Shape/DebugVars.h"

#include "Utils/PPErrLog.h"
#include "Utils/PPDbgUtils.h"

#include "Shape/DebugVars.h"
#include "Create/Creation.h"

#include <algorithm>

#include <FL/Fl.H>

namespace M3d {
  std::vector<const char*> Preference::sSchemesStr= {
    "none",
    "plastic",
    "gtk+",
    "gleam"
  };
  
  //*********************************
  Preference::Preference()
    :cDbgDrw( PP3dDbg.cDbgDrw  )
    ,cDbgBaz( PP3dDbg.cDbgBaz  )
    ,cDbgSel( PP3dDbg.cDbgSel  )
    ,cDbgFil( PPuDbg.cDbgFile )
    ,cDbgIni( PPuDbg.cDbgConf )
  {
    resetToDefault();    
  }
  //---------------------------------------
  //#define INIT_ALL_DBG() 
  
#define INIT_DBG( VAR )   iConfig.get( "Debug",  "Dbg"   #VAR, cDbg ## VAR  )
#define INIT_SEL( VAR )   iConfig.get( "Select", "Sel"   #VAR, cSelect ## VAR  )
#define INIT_HISTO( VAR ) iConfig.get( "Histo",  "Histo" #VAR, cHisto ## VAR  )
#define INIT_FILE( VAR )  iConfig.get( "File",   "File"  #VAR, cFile ## VAR  )

#define INIT_VIEW( VAR )  iConfig.get( "View",   "View"  #VAR, cView ## VAR  )

#define INIT_DRAW3D( VAR )  iConfig.get( "Draw3d",   "Draw3d"  #VAR, cDraw3d_ ## VAR  )
  
#define INIT_COLOR_DRAW3D( VAR )  { std::string tmp; PP3d::ColorRGBA Col; \
  if( iConfig.get( "Draw3d",   "Draw3d"  #VAR, tmp )) { \
      if( Col.fromStringFloats(  tmp ) == false)        \
        std::cerr <<  #VAR << " read failed" << std::endl; \
      else   VAR = Col; } }


    //--------------------------------------

    // initiaslise les paths avec home path defini
    void Preference::initDefaultPaths()
    {
  
      cCurrentPath  = PPu::PPFile::JoinPathNames( cHomePath, sBasesName );    
      cCurrentPath  = PPu::PPFile::JoinPathNames( cCurrentPath, sFileSaveDefault );
      
      cFileLastAutoSave  = cCurrentPath;
        
      std::cout << "%%% Preference::initDefaultPaths.cCurrentPath =" << cCurrentPath << std::endl;      
    }
    //--------------------------------------
    // redefini les paths avec le nouveau home path
    void Preference::initDefaultPaths( std::string & iHome )
    {
      cHomePath    = iHome;
      cCurrentPath = iHome;
      cConfigPath  = iHome;
      
       PPu::PPFile::DoMkSubdir( cHomePath, sBasesName );
       std::cout << "%%% Preference::initDefaultPaths2 " << cHomePath << " | " <<  sBasesName  << std::endl;

      initDefaultPaths();
      std::cout << "%%% Preference::initDefaultPaths2.cHome ="        << cHomePath << std::endl;      
      std::cout << "%%% Preference::initDefaultPaths2.cCurrentPath =" << cCurrentPath << std::endl;      
      std::cout << "%%% Preference::initDefaultPaths2.cConfigPath ="  << cConfigPath << std::endl;      
   }
    //--------------------------------------                   
    // definis les paths avec 
    void Preference::initCurrentPath( const std::string & iFileBase)
    {
      cCurrentPath= PPu::PPFile::WithoutExtension( iFileBase );
     
      cFileLastAutoSave  = cCurrentPath;
      cFileLastAutoSave += cFileAutoSave;

      std::cout << "%%% initCurrentPath cCurrentPath=" <<  cCurrentPath<< std::endl;
      std::cout << "%%% initCurrentPath cFileLastAutoSave=" << cFileLastAutoSave << std::endl;

     }
    //--------------------------------------
    // Renvoie le path du fichier avec la position courante 
    std::string  Preference::getFileName( const char * iExt )
    {
      std::string lTmp = cCurrentPath;
      lTmp += iExt;

      std::cout << "%%% getFileName " << iExt  << " => " << lTmp <<  std::endl;
      return lTmp;
    }
    //--------------------------------------
    // Renvoie le path du fichier avec la position courante 
    std::string  Preference::getCurrentPath()
    {
      std::cout << "%%% getCurrentPath=" << cCurrentPath <<  std::endl;
      return cCurrentPath;
    }
    //--------------------------------------



  
 //---------------------------------------
  void Preference::resetToDefault()
  {
    //cLastSave              = cSaveDefault;
    initDefaultPaths();

    cFileAutoSave          = true;
    cFileAutoSaveFrequency = 60;
    
    cMouseWheel            = 15;

    // Draw3d
    cDraw3d_LineWidth = 4;
    
    cDraw3d_ColorLine.set( 0.,1.0,0. );
    cDraw3d_ColorLineSelect.set(0.,0.8,0.);

    cDraw3d_ColorFacet.set(0.,0.7,0.);
    cDraw3d_ColorFacetSelect.set(0.,1.0,0.);
    cDraw3d_ColorLineHighlight.set(0.1,1.0,0.1);

    
    cDraw3d_InputCursor_ColorPoint.set( 1, 0.9, 0.1, 0.8);
    cDraw3d_InputCursor_ColorLine.set( 0.9, 0.5, 0.3, 0.8);

 
    cDraw3d_InputPoly_ColorPoint.set( 0.7, 0.4, 0.1, 0.7);
    cDraw3d_InputPoly_ColorLine.set( 0.6, 0.5, 0.2, 0.7 );    
    
    cDraw3d_InputObject_ColorPoint.set( 0.4, 1, 0.4, 0.5);
    cDraw3d_InputObject_ColorLine.set( 0.4, 0.8, 0.3, 0.4);
    cDraw3d_InputObject_ColorFacet.set( 0.2, 0.4, 0.2, 0.3 );

    
    cDraw3d_ObjectMagnet_ColorPoint.set ( 0.5, 0.5, 1, 0.1);
    cDraw3d_ObjectMagnet_ColorLine.set  ( 0.4, 0.4, 1, 0.1);
    cDraw3d_ObjectMagnet_ColorFacet.set ( 0.3, 0.3, 1, 0.05 );
    cDraw3d_ObjectMagnet_ViewMode = PP3d::ViewMode::SKELETON;

    
    //Debug
    cDbgEvt = cDbgAct = cDbgTree = cDbgDrw = cDbgBaz = cDbgSel = cDbgFil = cDbgIni = 0;
    
  }
  //---------------------------------------
  void Preference::resetToFile()
  {
    resetToDefault();
    initFromIni( Creation::Instance().getConfig() );    
  }
  //------------------------------------------------
  //------------------------------------------------
  //------------------------------------------------
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
    //    INIT_FILE( DefaultDir  );

    std::string lTmp;
    if( iConfig.get( "View", "Scheme", lTmp ) )
      {
        if( std::find( sSchemesStr.begin(), sSchemesStr.end(), lTmp) != std::end(sSchemesStr))
       Fl::scheme(lTmp.c_str() );		// change scheme
      }



    INIT_DRAW3D( LineWidth );
    INIT_COLOR_DRAW3D( cDraw3d_ColorLine );
    INIT_COLOR_DRAW3D( cDraw3d_ColorLineSelect );    

    INIT_COLOR_DRAW3D( cDraw3d_ColorFacet  );
    INIT_COLOR_DRAW3D( cDraw3d_ColorFacetSelect );
    INIT_COLOR_DRAW3D( cDraw3d_ColorLineHighlight );

    
    INIT_COLOR_DRAW3D( cDraw3d_InputCursor_ColorPoint);
    INIT_COLOR_DRAW3D( cDraw3d_InputCursor_ColorLine);

    INIT_COLOR_DRAW3D( cDraw3d_InputPoly_ColorPoint);
    INIT_COLOR_DRAW3D( cDraw3d_InputPoly_ColorLine);
    
    INIT_COLOR_DRAW3D( cDraw3d_InputObject_ColorPoint);
    INIT_COLOR_DRAW3D( cDraw3d_InputObject_ColorLine);
    INIT_COLOR_DRAW3D( cDraw3d_InputObject_ColorFacet);
    
    INIT_COLOR_DRAW3D( cDraw3d_ObjectMagnet_ColorPoint);
    INIT_COLOR_DRAW3D( cDraw3d_ObjectMagnet_ColorLine);
    INIT_COLOR_DRAW3D( cDraw3d_ObjectMagnet_ColorFacet);
    
    // PP3d::ViewMode cDraw3d_ObjectMagnet_ViewMode
    
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

#undef INIT_DRAW3D 
#define INIT_DRAW3D( VAR )  iConfig.set( "Draw3d", #VAR, VAR )
  
#undef INIT_COLOR_DRAW3D
#define INIT_COLOR_DRAW3D( VAR ) iConfig.set( "Draw3d", #VAR, VAR.toStringFloats() )


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
    //    INIT_FILE( DefaultDir  );

    const char* lTmp = Fl::scheme();
    if( lTmp != nullptr )
      {
        iConfig.set( "View", "Scheme", lTmp );
      }

    

    INIT_DRAW3D( cDraw3d_LineWidth );
    INIT_COLOR_DRAW3D( cDraw3d_ColorLine );
    INIT_COLOR_DRAW3D( cDraw3d_ColorLineSelect );    

    INIT_COLOR_DRAW3D( cDraw3d_ColorFacet  );
    INIT_COLOR_DRAW3D( cDraw3d_ColorFacetSelect );
    INIT_COLOR_DRAW3D( cDraw3d_ColorLineHighlight );

    
    INIT_COLOR_DRAW3D( cDraw3d_InputCursor_ColorPoint);
    INIT_COLOR_DRAW3D( cDraw3d_InputCursor_ColorLine);

    INIT_COLOR_DRAW3D( cDraw3d_InputPoly_ColorPoint);
    INIT_COLOR_DRAW3D( cDraw3d_InputPoly_ColorLine);
    
    INIT_COLOR_DRAW3D( cDraw3d_InputObject_ColorPoint);
    INIT_COLOR_DRAW3D( cDraw3d_InputObject_ColorLine);
    INIT_COLOR_DRAW3D( cDraw3d_InputObject_ColorFacet);
    
    INIT_COLOR_DRAW3D( cDraw3d_ObjectMagnet_ColorPoint);
    INIT_COLOR_DRAW3D( cDraw3d_ObjectMagnet_ColorLine);
    INIT_COLOR_DRAW3D( cDraw3d_ObjectMagnet_ColorFacet);


    
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

#undef INIT_DRAW3D 
#define INIT_DRAW3D( VAR )  iArgs.get( "Draw3d" #VAR,  VAR )
#undef INIT_COLOR_DRAW3D
#define INIT_COLOR_DRAW3D( VAR ) { std::string lTmp = iArgs.get( "Draw3d",  #VAR ); fromStringFloats

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
    //    INIT_FILE( DefaultDir  );


    
    std::string lTmp;
    if( iArgs.get( "Scheme", lTmp ) )
      {
        if( std::find( sSchemesStr.begin(), sSchemesStr.end(), lTmp) != std::end(sSchemesStr))
            Fl::scheme(lTmp.c_str() );		// change scheme
      }

    
  }
  //*********************************
 
  
} // end namespace
