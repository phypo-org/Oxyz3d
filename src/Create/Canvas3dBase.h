#ifndef H__PP3dCanvasBase__H
#define H__PP3dCanvasBase__H




#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Menu_Button.H>

#include "Shape/ViewProps.h"

#include "MyFlWidget.h"

#include "Shape/Kamera.h"


namespace M3d {
    
  enum class ModeUser {
    MODE_BASE,
    MODE_SELECT,
    MODE_SELECT_RECT,
    MODE_MOVE_CAMERA,
    MODE_TRANSFORM,
    MODE_DRAG
  };
  
  enum class ModeGrid {
    NO_GRID=0,
    GRID_2D=1,
    GRID_3D=2
  };
  
  class Win3dBase;
  //***************************************
  class Canvas3dBase : public Fl_Gl_Window{

  protected:
    Win3dBase & cMyWin3d ;
    
    PP3d::Kamera    cKamera;
    double          cScale;
    ModeUser        cUserMode    = ModeUser::MODE_BASE;
  
    
    bool      cAxisFlag;
    bool      cFlagLightColor; 
    ModeGrid  cGridMode;


    bool    cFlagViewNormal    = false;
    bool    cFlagViewGeo       = true;
    bool    cFlagCursor3d      = false;
    bool    cFlagSelectGeo     = false;

    
    PP3d::ViewProps cViewGen;
    
    Fl_Menu_Button*           cPopup=nullptr;

    //------------------------------
    int       cMouseInitPosX=-1;
    int       cMouseInitPosY=-1;
    int       cMouseLastPosX=-1;
    int       cMouseLastPosY=-1;

    int       cMouseLastPosZ=-1;


    
  public:
   Canvas3dBase( Win3dBase & pW3d, int pX, int pY, int pW, int  pH, const char *l ); 
    virtual ~Canvas3dBase();
    
    void setViewGeo( bool cVal) { cFlagViewGeo = cVal; }
    bool getViewGeo()           { return cFlagViewGeo; }
    
    void setSelectGeo( bool cVal) { cFlagSelectGeo = cVal; }
    bool getSelectGeo()           { return cFlagSelectGeo && cFlagViewGeo; }

    PP3d::Kamera&   getKamera()   { return cKamera;   }
		
    
    virtual void drawGrid();
    virtual void drawUtils();
    virtual void beginDraw();
    virtual void endDraw();
    virtual void draw();
    
    Win3dBase& getMyWin() { return cMyWin3d; }
    
    void     changeUserMode( ModeUser iMode ) { cUserMode = iMode; }
    ModeUser getUserMode()   const             { return cUserMode;}
  
    void setVisualMode( PP3d::ViewMode pMode )    { cViewGen.cViewMode = pMode; }
    void userChangeKameraView(int	pEvent);

  };
  //***************************************

}


#endif
