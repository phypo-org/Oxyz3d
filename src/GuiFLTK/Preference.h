#ifndef H__PREF__H
#define H__PREF__H

#include "Utils/PPSingletonCrtp.h"
#include "Utils/PPConfig.h"
#include "Utils/PPArgs.h"
#include "Utils/PPFile.h"

#include "Shape/DebugVars.h"
#include "Shape/ColorRGBA.h"
#include "Shape/ViewProps.h"

#include <string>

#include <FL/Fl_Color_Chooser.H>

#include <iostream>

#define sOxyz3dExt ".oxyz"

namespace M3d {
  //*********************************

  class Preference : public virtual PPSingletonCrtp<Preference>{
      
  public:

    static  std::vector<const char*> sSchemesStr;

    //Interface
    float cMouseWheel=15;

    std::string cHomePath     = ".";
    std::string cCurrentPath  = ".";
    std::string cConfigPath   = ".";
    const std::string sBasesName  = "bases";
                
    //Save
    const std::string sFileSaveDefault = "default";
    
    // AutoSave
    bool cFileAutoSave = true;
    int  cFileAutoSaveFrequency= 60;
    int  cNbFileCycling= 3;
    
    const std::string sFileAutoSaveDir="autosave";
    std::string cFileLastAutoSave ;    
     // AutoSave

    void initDefaultPaths();    
    void initDefaultPaths( std::string & iHome );
    void initCurrentPath( const std::string & iFileBase);
    std::string  getFileName( const char * iExt );
    std::string  getCurrentPath();

    
      
    //Dbg
    int cDbgEvt = 0;
    int cDbgAct = 0;
#ifdef USING_LUA    
    int cDbgLua = 0;
#endif    
    int cDbgTree = 0;
      
    int & cDbgDrw;
    int & cDbgBaz;
    int & cDbgSel;

    
    int & cDbgFil;
    int & cDbgIni;

    //Select
    bool cSelectPassOverLighting = true ; // inlight entity when passing on
    int  cSelectPickingSize=10;

    // Spline 
    int cBSplineMaille=8;


    //View
    //    Fl_Color cCanvasBG = FL_BLACK;

    //    Draw3d
    int   cDraw3d_LineWidth = 4;
    PP3d::ColorRGBA cDraw3d_ColorLine;
    PP3d::ColorRGBA cDraw3d_ColorLineSelect;
    
    PP3d::ColorRGBA cDraw3d_ColorFacet;
    PP3d::ColorRGBA cDraw3d_ColorFacetSelect;
    PP3d::ColorRGBA cDraw3d_ColorLineHighlight;
    
    PP3d::ColorRGBA cDraw3d_InputCursor_ColorPoint;
    PP3d::ColorRGBA cDraw3d_InputCursor_ColorLine;

    PP3d::ColorRGBA cDraw3d_InputPoly_ColorPoint;
    PP3d::ColorRGBA cDraw3d_InputPoly_ColorLine;
    
    PP3d::ColorRGBA cDraw3d_InputObject_ColorPoint;
    PP3d::ColorRGBA cDraw3d_InputObject_ColorLine;
    PP3d::ColorRGBA cDraw3d_InputObject_ColorFacet;
    
    PP3d::ColorRGBA cDraw3d_ObjectMagnet_ColorPoint;
    PP3d::ColorRGBA cDraw3d_ObjectMagnet_ColorLine;
    PP3d::ColorRGBA cDraw3d_ObjectMagnet_ColorFacet;
    
    PP3d::ViewMode cDraw3d_ObjectMagnet_ViewMode  = PP3d::ViewMode::SKELETON;
    

    friend class PPSingletonCrtp;
      
    Preference();
    
    void initFromIni    ( PPu::PPConfig & iConfig );    
    void initFromArgs   (  PPu::PPArgs & iArgs );    
    bool saveInIni      ( PPu::PPConfig & iConfig );    
    bool saveInCurrentIni ( );    
    void initFromArg    ( PPu::PPArgs   & iArgs );
    void resetToDefault ();
    void resetToFile    ();
  };
  //*********************************

} //endspace

#define MyPref M3d::Preference::Instance()

#define DBG_EVT( A )    if( MyPref.cDbgEvt > 0 ) std::cout << "DbgSel0> " << A << std::endl;
#define DBG_EVT1( A )   if( MyPref.cDbgEvt > 1 ) std::cout << "DbgSel1> " << A << std::endl;
#define DBG_EVT2( A )   if( MyPref.cDbgEvt > 2 ) std::cout << "DbgSel0> " << A << std::endl;
#define DBG_EVT_NL( A ) if( MyPref.cDbgEvt > 0 ) std::cout << "DbgSel> " << A ;
  
#define DBG_ACT( A )    if( MyPref.cDbgAct > 0 ) std::cout << "DbgAct0> " << A << std::endl;
#define DBG_ACT1( A )   if( MyPref.cDbgAct > 1 ) std::cout << "DbgAct1> "<< A << std::endl;
#define DBG_ACT2( A )   if( MyPref.cDbgAct > 2 ) std::cout << "DbgAct2> "<< A << std::endl;
  
#define DBG_LUA( A )    if( MyPref.cDbgLua > 0 ) std::cout << "DbgLua1> "<< A << std::endl;
#define DBG_LUA1( A )   if( MyPref.cDbgLua > 1 ) std::cout << "DbgLua2> "<< A << std::endl;
#define DBG_LUA2( A )   if( MyPref.cDbgLua > 2 ) std::cout << "DbgLua3> "<< A << std::endl;

#define DBG_TREE( A )    if( MyPref.cDbgTree > 0 ) std::cout << "DbgTree1> "<< A << std::endl;
#define DBG_TREE1( A )   if( MyPref.cDbgTree > 1 ) std::cout << "DbgTree2> "<< A << std::endl;
#define DBG_TREE2( A )   if( MyPref.cDbgTree > 2 ) std::cout << "DbgTree3> "<< A << std::endl;




#define ICON_PATH "usr/share/icons/"
  



#endif
