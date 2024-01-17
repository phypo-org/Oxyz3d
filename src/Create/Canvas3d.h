#ifndef H__PP3dCanvas__H
#define H__PP3dCanvas__H




#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Menu_Button.H>


#include "MyFlWidget.h"

#include "Shape/Kamera.h"


#include "Shape/DataBase.h"
#include "Shape/ObjectFacet.h"
#include "Shape/ObjectLine.h"
#include "Shape/ViewProps.h"
#include "Shape/Point3d.h"
#include "Shape/Entity.h"


#include "Modif/VisitorModif.h"
#include "Modif/Modif.h"

#include "Magnet.h"


class Fl_Menu_Button;
struct Fl_Menu_Item;


namespace M3d {
 
  enum class GlobalMode{
    INPUT,
    MAGNET,
    SCULT
  };
  
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

  class Win3d;

  //***************************************
  class Canvas3d : public Fl_Gl_Window{

  protected:
	
    Win3d&          cMyWin3d;
    PP3d::Kamera    cKamera;
    double          cScale;
    ModeUser        cUserMode    = ModeUser::MODE_BASE;
    GlobalMode      cGlobalMode  = GlobalMode::INPUT;

    
    bool      cAxisFlag;
    bool      cFlagLightColor; 
    ModeGrid  cGridMode;
		
 
    bool    cFlagViewNormal    = false;
    bool    cFlagViewGeo       = true;
    bool    cFlagCursor3d      = false;
    bool    cFlagSelectGeo     = false;

		
    PP3d::ViewProps cViewGen;
    PP3d::ViewProps cViewPropsTransform;
    PP3d::ViewProps cViewInputCursor;
    PP3d::ViewProps cViewInputPoly;
    PP3d::ViewProps cViewInputObject;
    PP3d::ViewProps cViewInputObjectMagnet;


    Fl_Menu_Button*           cPopup=nullptr;
    PP3d::VisitorModifPoints* cVisitModifSelect = nullptr;
    

  public:
    void setViewGeo( bool cVal) { cFlagViewGeo = cVal; }
    bool getViewGeo()           { return cFlagViewGeo; }
    
    void setSelectGeo( bool cVal) { cFlagSelectGeo = cVal; }
    bool getSelectGeo()           { return cFlagSelectGeo && cFlagViewGeo; }

    Magnet  & getMagnet();
  
  protected:
 
    // Pour le dragging 
    PP3d::PointPtrSet cDragPoints;      // les points concernées par la selection
    PP3d::VectPoint3d cDragSavPoints;   // sauvegarde de leurs coordonnées originales
    PP3d::Point3d     cDragCenter;      // Le barycentre des points
    
    bool initDragSelect();
    void dragSelect( PP3d::Mat4 &pMat );
    void validDragSelect( PP3d::Mat4 &pMat);
     void validDragSelect();
   void cancelDragSelect();

    
    // dragging


    // Selection
    PP3d::SelectMode cSelectMode= PP3d::SelectMode::Undefine;
    // Selection
		
  public:
    Canvas3d( Win3d& pW3d, int pX, int pY, int pW, int pH ,const char *l=nullptr);
    virtual ~Canvas3d(  );

    void drawGrid();
    void draw();
    void drawForSelect();
    int  handle(	int	pEvent	); 
    int  handleMagnet(	int	pEvent	); 
    int  handleInput(	int	pEvent	); 
    int  handleMenu(	int	pEvent	); 
    int  handleCamera(	int	pEvent	); 
    int  handleSelect(	int	pEvent	); 
    int  handleTransform( int	pEvent	);
    
    void stopMagnetise();

    PP3d::Kamera&   getKamera()   { return cKamera;   }
		

    void     changeUserMode( ModeUser iMode ) { cUserMode = iMode; }
    ModeUser getUserMode()   const             { return cUserMode;}
    
    void       setGlobalMode( GlobalMode iAct ) { cGlobalMode = iAct; }
    GlobalMode getGlobalMode()  const           { return cGlobalMode;}


    
    
    
    void setVisualMode( PP3d::ViewMode pMode )    { cViewGen.cViewMode = pMode; }
    //------------------------------
    int       cMouseInitPosX=-1;
    int       cMouseInitPosY=-1;
    int       cMouseLastPosX=-1;
    int       cMouseLastPosY=-1;

    int cMouseLastPosZ=-1;

    bool userActionIsRun() const{ return cMouseLastPosX != -1; } // mettre un bool a la place

	 
    void userActionPrepare( );
    void userActionCancel();						 
    void userActionTerminate( );
    
    void userChangeKameraView(int	pEvent);
    PP3d::Point3d  userInputPoint( int x, int y, bool iFinalize );
    PP3d::Point3d  userInputPoint( bool iFinalize);
    void userInputPoint( PP3d::Entity * iEntity );
    void userTransformSelectionInput( int pEvent );
    void userTransformSelection(int	pEvent, bool cFlagFinalize=false);
    void userTransformSelectionInternal( float lDx, float lDy,  bool cFlagFinalize=false);
    bool userSelectionRectangle(int	pEvent,  bool cFlagFinalize=false);
    bool userSelectionPointColor(int	pEvent, bool cFlagMove);
    bool userDragInputPt(int  pEvent, bool iFinalize );
    bool setCursor3dPosition( int pX, int pY );
		
    bool transform2Dto3D(  int pX, int pY, PP3d::Point3d & iResult );

    bool selectEntity( PP3d::EntityId iId, bool pFlagMove   );
    bool pickingColor( int pX, int pY, bool pFlagMove, int iSizeX, int iSizeY, bool pFlagRect  );

    void drawSelectRect();
    void drawSelectCircle(  PP3d::Point3d iPos, int iSize, bool iUseAlternColor = false );

    
    //=========== MENUS =================
    void makeMenuSelect( Fl_Menu_Button& pMenu);
    void makeMenuPrimitiv( Fl_Menu_Button& pMenu);
    void makeMenuMagnet( Fl_Menu_Button& pMenu);

    static void MyMenuCallbackSubdivide(Fl_Widget* w, void* pUserData );
    static void MyMenuCallbackSubdivide1(Fl_Widget* w, void* pUserData );
    static void MyMenuCallbackSubdivide2(Fl_Widget* w, void* pUserData );
    static void MyMenuCallbackSubdiveCatmullClark(Fl_Widget* w, void* pUserData );
  
    static void MyMenuCallbackModifyShape(Fl_Widget* w, void* pUserData );
 

    //=========== MENUS =================

    void traceMode() const;
		
    friend class Win3d;
  };
  //***************************************





  const char * const ANNULE_ACTION="e";
  
  const char * const CHG_AXIS="a";
  
  const char * const CENTER_ON_SELECTION="A";
 
  const char * const CHG_GRID="g";
  
  const char * const CHG_ORTHO_PERS="p";
  
  
  const char * const RESET_VIEW_TO_X="x";
  const char * const RESET_VIEW_TO_Y="y";
  const char * const RESET_VIEW_TO_Z="z";
  const char * const RESET_VIEW_TO_X2="X";
  const char * const RESET_VIEW_TO_Y2="Y";
  const char * const RESET_VIEW_TO_Z2="Z";
  

  const char * const RESET_VIEW_SCALE_ORIGIN="O";
  
  const char * const RESET_VIEW="O";
  const char * const RESET_VIEW_SCALE_0="0";
  const char * const RESET_VIEW_SCALE_1="1";
  const char * const RESET_VIEW_SCALE_2="2";
  const char * const RESET_VIEW_SCALE_3="3";
  const char * const RESET_VIEW_SCALE_4="4";
  const char * const RESET_VIEW_SCALE_5="5";
  const char * const RESET_VIEW_SCALE_6="6";
  
  const char * const KEY_UNDO="/7a";

  
  const char * const STR_CURSOR_3D="c";
  const char * const STR_EXIT="q";

  const char * const MOVE_Z_N="-";
  const char * const MOVE_Z_P="+";
	
  const char * const UNSELECT_ALL=" ";
  const char * const BASCULE_DRAW_SELECT_COLOR="!";
  const char * const BASCULE_TEST_SELECT_COLOR=":";
  
}




#endif
