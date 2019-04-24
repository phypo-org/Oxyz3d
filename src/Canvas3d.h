#ifndef H__PP3dCanvas__H
#define H__PP3dCanvas__H




#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Menu_Button.H>

#include "DataBase.h"

#include "MyFlWidget.h"

#include "Kamera.h"


#include "Shape/DataBase.h"
#include "Shape/ObjectFacet.h"
#include "Shape/ObjectLine.h"
#include "Shape/ViewProps.h"
#include "Shape/Point3d.h"
#include "Shape/Entity.h"

#include "Shape/Modif.h"




class Fl_Menu_Button;
struct Fl_Menu_Item;


namespace M3d {
 
	enum class ModeUser { MODE_BASE, MODE_SELECT, MODE_SELECT_RECT, MODE_MOVE_CAMERA, MODE_TRANSFORM };
		enum class ModeGrid { NO_GRID=0, GRID_2D=1, GRID_3D=2 };

class Win3d;

	//***************************************
	class Canvas3d : public Fl_Gl_Window{
	public:


	protected:
	
		Win3d&         cMyWin3d;
		PP3d::Kamera    cKamera;
		double          cScale;
		ModeUser        cMode;

		bool      cAxisFlag;
		bool      cFlagLightColor; 
		ModeGrid  cGridMode;
		

		bool      cDebug;

		PP3d::DataBase& cDataBase;
		//	DataBase cDataBaseLight;
		
		bool    cFlagCursor3d;
		
		PP3d::ViewProps cViewProps;
		
	
		Fl_Menu_Button*          cPopup=nullptr;
	        PP3d::VisitorModifPoints* cVisitModifSelect = nullptr;


		// Pour le dragging 
		PP3d::PointPtrSet  cDragPoints;      // les points concernées par la selection
		PP3d::VectPoint3d cDragSavPoints;    // sauvegarde de leurs coordonnées originales
		bool initDragSelect();
		void dragSelect( PP3d::Mat4 &pMat );
		void validDragSelect( PP3d::Mat4 &pMat);
		void cancelDragSelect();
		// dragging


		// Selection
		PP3d::SelectMode cSelectMode= PP3d::SelectMode::Undefine;
		// Selection
		
	public:
		Canvas3d( Win3d& pW3d, int pX, int pY, int pW, int pH , PP3d::DataBase& pDatabase, const char *l=nullptr);
		virtual ~Canvas3d(  );

		void draw();
		int  handle(	int	pEventh	); 

		PP3d::Kamera&   getKamera()   { return cKamera;   }
		PP3d::DataBase& getDataBase() { return cDataBase; }
		


		void changeUserMode( ModeUser pMode ) { cMode = pMode; }
		void setVisualMode( GLuint pMode ) { cViewProps.cViewMode = pMode; }
		//------------------------------
		int       cMouseLastPosX=-1;
		int       cMouseLastPosY=-1;
	 
		int       cRectBeginX=-1;
		int       cRectBeginY=-1;
		
		void userPrepareAction( int	pEvent );
		void userCancelAction(	int	pEvent );						 
		void userTerminateAction(	int	pEvent );						 
		void userChangeKameraView(int	pEvent);
		void userInputPoint(int	pEvent);
		void userTransformSelection(int	pEvent, bool cFlagFinalize=false);
		void userSelectionRectangle(int	pEvent,  bool cFlagFinalize=false);
		void userSelectionPoint(int	pEvent, bool cFlagMove);
		void setCursor3dPosition( int pX, int pY, int pZ=0 );
		
		PP3d::Point3d tranform2Dto3D(  int pX, int pY, int pZ=0);

		void processHits( GLuint pNbHits, GLuint*  pSelectBuf, bool cFlagMove);
		void picking( int pX, int pY, bool cFlagMove );


		//=========== MENUS =================
		void makeMenu( Fl_Menu_Button& pMenu);
		void makeMenuSelect( Fl_Menu_Button& pMenu);
		void makeMenuPrimitiv( Fl_Menu_Button& pMenu);
		
		static void MyMenuCallbackExtrude(Fl_Widget* w, void* pUserData );
		static void MyMenuCallbackSelect(Fl_Widget* w, void* pUserData );
		static void MyMenuCallbackPrimitiv(Fl_Widget* w, void* pUserData );
		//=========== MENUS =================

		void traceMode() const;
		
		friend class Win3d;
	};
	//***************************************	
};




#endif
