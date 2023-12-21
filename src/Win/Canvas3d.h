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




class Fl_Menu_Button;
struct Fl_Menu_Item;


namespace M3d {
 
  enum class ModeUser { MODE_BASE, MODE_SELECT, MODE_SELECT_RECT, MODE_MOVE_CAMERA, MODE_TRANSFORM, MODE_DRAG_INPUT_PT };
  enum class ModeGrid { NO_GRID=0, GRID_2D=1, GRID_3D=2 };

  class Win3d;

  //***************************************
  class Canvas3d : public Fl_Gl_Window{

  protected:
	
    Win3d&         cMyWin3d;
    PP3d::Kamera    cKamera;
    double          cScale;
    ModeUser        cMode;

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
    

    Fl_Menu_Button*           cPopup=nullptr;
    PP3d::VisitorModifPoints* cVisitModifSelect = nullptr;



  public:
    void setViewGeo( bool cVal) { cFlagViewGeo = cVal; }
    bool getViewGeo()           { return cFlagViewGeo; }
    
    void setSelectGeo( bool cVal) { cFlagSelectGeo = cVal; }
    bool getSelectGeo()           { return cFlagSelectGeo && cFlagViewGeo; }


  protected:
 
    // Pour le dragging 
    PP3d::PointPtrSet cDragPoints;      // les points concernées par la selection
    PP3d::VectPoint3d cDragSavPoints;   // sauvegarde de leurs coordonnées originales
    PP3d::Point3d     cDragCenter;      // Le barycentre des points
    
    bool initDragSelect();
    void dragSelect( PP3d::Mat4 &pMat );
    void validDragSelect( PP3d::Mat4 &pMat);
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
    int  handle(	int	pEventh	); 

    PP3d::Kamera&   getKamera()   { return cKamera;   }
		


    void changeUserMode( ModeUser pMode ) { cMode = pMode; }
    void setVisualMode( GLuint pMode ) { cViewGen.cViewMode = pMode; }
    //------------------------------
    int       cMouseInitPosX=-1;
    int       cMouseInitPosY=-1;
    int       cMouseLastPosX=-1;
    int       cMouseLastPosY=-1;
	 
    void userPrepareAction( int	pEvent );
    void userCancelAction(	int	pEvent );						 
    void userTerminateAction(	int	pEvent );						 
    void userChangeKameraView(int	pEvent);
    void userInputPoint( bool iFinalize);
    void userInputPoint( PP3d::Entity * iEntity );
    void userTransformSelection(int	pEvent, bool cFlagFinalize=false);
    bool userSelectionRectangle(int	pEvent,  bool cFlagFinalize=false);
    bool userSelectionPointColor(int	pEvent, bool cFlagMove);
    bool userDragInputPt(int  pEvent, bool iFinalize );
    bool setCursor3dPosition( int pX, int pY );
		
    bool transform2Dto3D(  int pX, int pY, PP3d::Point3d & iResult );

    bool selectEntity( PP3d::EntityId iId, bool pFlagMove   );
    bool pickingColor( int pX, int pY, bool pFlagMove, int iSizeX, int iSizeY, bool pFlagRect  );

    void drawSelectRect();

    
    //=========== MENUS =================
    void makeMenu( Fl_Menu_Button& pMenu);
    void makeMenuSelect( Fl_Menu_Button& pMenu);
    void makeMenuPrimitiv( Fl_Menu_Button& pMenu);

    static void MyMenuCallbackSpline(Fl_Widget* w, void* pUserData );
    static void MyMenuCallbackExtrude(Fl_Widget* w, void* pUserData );
    static void MyMenuCallbackFlatten(Fl_Widget* w, void* pUserData );
    static void MyMenuCallbackFlip(Fl_Widget* w, void* pUserData );
    static void MyMenuCallbackPutOn(Fl_Widget* w, void* pUserData );
    static void MyMenuCallbackAlign(Fl_Widget* w, void* pUserData );
    static void MyMenuCallbackSelect(Fl_Widget* w, void* pUserData );
    static void MyMenuCallbackShape(Fl_Widget* w, void* pUserData );
    static void MyMenuCallbackPrimitiv(Fl_Widget* w, void* pUserData );
    static void MyMenuCallbackRevol(Fl_Widget* w, void* pUserData );
    static void MyMenuCallbackSpiralInput(Fl_Widget* w, void* pUserData);
    static void MyMenuCallbackSpiralSelObj(Fl_Widget* w, void* pUserData );
    static void MyMenuCallbackConnectPoint(Fl_Widget* w, void* pUserData );
    static void MyMenuCallbackSubdivide(Fl_Widget* w, void* pUserData );
    static void MyMenuCallbackInset(Fl_Widget* w, void* pUserData );
    static void MyMenuCallbackSubdivide1(Fl_Widget* w, void* pUserData );
    static void MyMenuCallbackSubdivide2(Fl_Widget* w, void* pUserData );
    static void MyMenuCallbackSubdiveCatmullClark(Fl_Widget* w, void* pUserData );
    static void MyMenuCallbackCutLine(Fl_Widget* w, void* pUserData );

    static void MyMenuCallbackModifyShape(Fl_Widget* w, void* pUserData );
    static void MyMenuCallbackExtrudePath(Fl_Widget* w, void* pUserData);
    static void MyMenuCallbackExtrudePathNorm(Fl_Widget* w, void* pUserData);

    //=========== MENUS =================

    void traceMode() const;
		
    friend class Win3d;
  };
  //***************************************	
}




#endif
