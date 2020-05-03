//#include "config.h"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Hor_Slider.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/math.h>

#include <FL/Fl_Sys_Menu_Bar.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/Fl_Choice.H>

#include <FL/Fl_File_Icon.H>
#include <FL/Fl_Button.H>

#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/fl_ask.H>


#include "Shape/DataBase.h"
#include "Shape/ObjectFacet.h"
#include "Shape/ObjectLine.h"
#include "Shape/UndoHistory.h"
#include "Shape/SortVisitor.h"

#include "Utils/PPFile.h"

#include "Win3d.h"
#include "Canvas3d.h"
#include "Dialogs.h"

#include "Application.h"
#include "MyFlWidget.h"
#include "MyImage.h"
#include "Preference.h"


#include "Shape/Selection.h"

#include "Shape/SavRead.h"
#include "Shape/ExportObj.h"

#include <memory>

#include <fstream>
#include <stdlib.h>


namespace M3d {

  
#define StrMenu_NewBase          "New"
#define StrMenu_OpenBase         "Open ..."
#define StrMenu_SaveBase         "Save"
#define StrMenu_SaveBaseAs       "Save as ..."
#define StrMenu_SaveBaseSelect   "Save Selected ..."
#define StrMenu_MergeBase        "Merge ..."
  
#define StrMenu_ExportD3dObj     "Export d3d file (.obj) ..."
#define StrMenu_ExportSelectD3dObj "Export selection d3d file (.obj) ..."
#define StrMenu_ImportD3dObj     "Import d3d file (.obj) ..."

#define StrMenu_Preferences     "Preferences ..."


#define StrMenu_Create3dView    "New 3d view"
#define StrMenu_ObjectTree      "Objects Tree"
#define StrMenu_ConsolPython    "Console python"
#define StrMenu_ConsolSystem    "Console system"
#define StrMenu_ConsolLua       "Console lua"

#define StrMenu_UnselectAll     "Unselect all"
#define StrMenu_DeleteSelect    "Delete object selection"
#define StrMenu_AddSelectCopyToInput "Add selection to input (copy)"

#define StrMenu_CreateShape          "New Shape "
#define StrMenu_CreateShapePolyline  StrMenu_CreateShape  "Polyline"
#define StrMenu_CreateShapeFacet     StrMenu_CreateShape "Facet"

	
#define StrMenu_CallDialoDiagSub     "New Subdivide ..."

#define StrMenu_Revol     "New Revol "
#define StrMenu_RevolX    StrMenu_Revol "X ..."
#define StrMenu_RevolY    StrMenu_Revol "Y ..."
#define StrMenu_RevolZ    StrMenu_Revol "Z ..."


	

#define StrMenu_DialogPerspectivSettings  "Perspective settings ..."


#define StrMenu_TransformMoveX    "Move X"
#define StrMenu_TransformMoveY    "Move Y"
#define StrMenu_TransformMoveZ    "Move Z"

#define StrMenu_TransformRotX     "Rotate X"
#define StrMenu_TransformRotY     "Rotate Y"
#define StrMenu_TransformRotZ     "Rotate Z"

	
#define StrMenu_CreateCube      "Cube ..."
#define StrMenu_CreateTetra     "Tetraede ... "
#define StrMenu_CreatePyramid   "Pyramide ... "
#define StrMenu_CreateOcto      "Octoedre ... "
#define StrMenu_CreateDodec     "Dodecaedre ..."
#define StrMenu_CreateIcosahe   "Icosaedre ..."

#define StrMenu_Demo1            "Demo 1"
#define StrMenu_Demo2            "Demo 2"
	
  using namespace std;
  
  //-------------------------------------------
  static bool SaveBase( PP3d::DataBase * ioDatabase, const std::string & iName, void* iUseSelect )
  {
    // FAIRE FICHIER .backup
    std::ofstream lOut;						
    lOut.open( iName );
    if( lOut.good() )
      {
	bool lRet;
	PP3d::MySav lSav( lOut );				

	if( iUseSelect )
	  {
	    PP3d::SortEntityVisitor  lVisit;
	    TheSelection.execVisitorObjects( lVisit );	    	    
	    lRet = lSav.save( *ioDatabase, &lVisit.cSetAllEntity);
	  }
	else
	  {
	    lRet = lSav.save( *ioDatabase, nullptr);
	  }
	
	lOut.close();
	return lRet;
      }
    return false;
  }
  //-------------------------------------------
  static bool OpenBase( PP3d::DataBase * ioDatabase, const std::string & iName, bool iKeepId )
  {
    std::ifstream lIn;						
    lIn.open( iName );
    
    if( lIn.good() )
      {									
	PP3d::MyRead lRead( lIn );
	bool lRet = lRead.read( *ioDatabase, iKeepId );
	lIn.close();
	return lRet;
      }
    return false;
  }
  //-------------------------------------------
  //-------------------------------------------
  //-------------------------------------------
  static bool ExportD3dObj( PP3d::DataBase * ioDatabase, const std::string & iName, void* iUseSelect )
  {
    std::ofstream lOut;						
    lOut.open( iName );
    if( lOut.good() )
      {
	bool lRet;
	PP3d::MyExportObj lExpObj( lOut );				
	
	if( iUseSelect )
	  {
	    PP3d::SortEntityVisitor  lVisit;
	    TheSelection.execVisitorObjects( lVisit );	    
	    lRet = lExpObj.save( *ioDatabase, &lVisit.cSetAllEntity);
	  }
	else
	  {
	   lRet = lExpObj.save( *ioDatabase, nullptr);
	  }
	lOut.close();
	
	return lRet;
      }
    return false;
  }
  //-------------------------------------------
  static bool ImportD3dObj( PP3d::DataBase * ioDatabase, const std::string & iName )
  {
    std::ifstream lIn;						
    lIn.open( iName );
    
    if( lIn.good() )
      {									
	PP3d::MyImportObj lRead( lIn );
	bool lRet = lRead.read( *ioDatabase );
	lIn.close();
	return lRet;
      }
    return false;
  }
  //------------------------------------------------------
  //------------------------------------------------------
  //------------------------------------------------------
  
  static void SaveBaseCB( Fl_File_Chooser *cFc,	// I - File chooser
			  void            *cData )	// I - Data
  {
    printf(" SaveAsCB filename = \"%s\"\n",  cFc->value() ?  cFc->value() : "(null)");        
    
    if( cFc->value() )
      {
	std::string lFilename = cFc->value();

	if( SaveBase( Application::Instance().getDatabase(), lFilename, cData) )
	  {
	    MyPref.cLastSave = lFilename;
	  }
	else
	  {
	    fl_alert( "Saving database in <%s> failed", lFilename.c_str());
	  }
      }
  } 
  //-------------------------------------------
  static void OpenBaseCB( Fl_File_Chooser *cFc,	// I - File chooser
			  void            *cData)	// I - Data
  {    
    printf(" ReadCB filename = \"%s\"\n",  cFc->value() ?  cFc->value() : "(null)");        
    
    if( cFc->value()  )
      {
	// Une Nlle base
	std::unique_ptr<PP3d::DataBase> luBase( new PP3d::DataBase() );

	std::string lFilename = cFc->value();
	if( OpenBase( luBase.get(), lFilename, true ) ) // on prend les id de la base lu
	  {
	    PushHistory(); // on sauve l'ancienne base dans l'historique
	    
	    luBase->resetIdFromMax(); // on prend en compte les id de la base lu 
	    Application::Instance().setDatabase( luBase ); // on prend la nlle base
	    
	    MyPref.cLastSave = lFilename;
	    Application::Instance().redrawAll();
	  }
	else
	  {
	    fl_alert( "Open file <%s> failed", lFilename.c_str() );
	  }
      }
  } 
  //-------------------------------------------
  // on ajoute a la base courante la nlle base
  
  static void MergeBaseCB( Fl_File_Chooser *cFc,	// I - File chooser
			    void            *cData)	// I - Data
  {    
    printf(" ReadCB filename = \"%s\"\n",  cFc->value() ?  cFc->value() : "(null)");        
    
    if( cFc->value()  )
      {
	PushHistory(); 

	std::string lFilename = cFc->value();
	if( OpenBase( Application::Instance().getDatabase(), lFilename, false)) // On change les ID !!!!!!!!!!!!!!!!!!!!!
	  {
	    MyPref.cLastSave = lFilename;
	    Application::Instance().redrawAll();
	  }
	else
	  {
	    fl_alert( "Open file <%s> failed", lFilename.c_str() );
	    // A FAIRE : Restore de la base
	  }
      }
  }
  //------------------------------------------------------
  static void ExportD3dObjCB(Fl_File_Chooser *cFc,	// I - File chooser
			     void            *cData)	// I - Data
  {    
    printf(" ExportD3dObjCB filename = \"%s\"\n",  cFc->value() ?  cFc->value() : "(null)");        
    
    if( cFc->value()  )
      {
	std::string lFilename = cFc->value();

	
	if( ExportD3dObj( Application::Instance().getDatabase(), lFilename, cData ))
	  {
	  }
	else
	  {
	    fl_alert( "Export for D3D format  in <%s> failed", lFilename.c_str());
	  }
      }
  } 
  //-------------------------------------------
  // on ajoute a la base courante la nlle base
  
  static void ImportD3dObjCB( Fl_File_Chooser *cFc,	// I - File chooser
			      void            *cData)	// I - Data
  {    
    printf(" ImportD3dObjCB = \"%s\"\n",  cFc->value() ?  cFc->value() : "(null)");        
    
    if( cFc->value()  )
      {
	PushHistory(); 

	std::string lFilename = cFc->value();
	if( ImportD3dObj(  Application::Instance().getDatabase(), lFilename ) ) // On change les ID !!!!!!!!!!!!!!!!!!!!!
	  {
	    Application::Instance().redrawAll();
	  }
	else
	  {
	    fl_alert( "Import  for D3D format file <%s> failed", lFilename.c_str() );
	    // A FAIRE : Restore de la base
	  }
      }
  }
  //-------------------------------------------------------------
  //-------------------------------------------------------------
  //-------------------------------------------------------------
  static void  UndoCB(Fl_Widget*w, void*pData)
  {
    MyButton* lButton = reinterpret_cast<MyButton*>( pData);
    Win3d*    lWin3d = reinterpret_cast<Win3d*>( lButton->cUserData1);
	
    std::unique_ptr<PP3d::DataBase> luBase( new PP3d::DataBase() );
    if( PP3d::UndoHistory::Instance().readPrev( *luBase ) )
      {
	luBase->resetIdFromMax();
	//	std::cout << "UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU" << std::endl;

 	Application::Instance().setDatabase( luBase );
      }
    Application::Instance().redrawAllCanvas3d();
    Application::Instance().redrawObjectTree();
  }
  //-------------------------------------------
  static void RedoCB(Fl_Widget*w, void*pData)
  {
    MyButton* lButton = reinterpret_cast<MyButton*>( pData);
    Win3d* lWin3d = reinterpret_cast<Win3d*>( lButton->cUserData1);
    
    std::unique_ptr<PP3d::DataBase> luBase( new PP3d::DataBase() );
    if( PP3d::UndoHistory::Instance().readNext( *luBase ) )
      {
	luBase->resetIdFromMax();
	//	std::cout << "RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR" << std::endl;

	Application::Instance().setDatabase( luBase );
      }
 	
    Application::Instance().redrawAllCanvas3d();
    Application::Instance().redrawObjectTree();
  }
  //-------------------------------------------
  //-------------------------------------------
  //-------------------------------------------
  static void	BasculeSelModeCB(Fl_Widget*w, void*pData)
  {
    MyToggleButton* lToggle = reinterpret_cast<MyToggleButton*>( pData);
    Win3d* lWin3d = reinterpret_cast<Win3d*>( lToggle->cUserData1);
	
    union ConvVoid
    {
      void *             cPtr;
      GLuint             cVal;
      PP3d::SelectType   cSelType; 
    };
	
    ConvVoid lVal;
    lVal.cPtr=  lToggle->cUserData2;
	
    PP3d::Selection::Instance().changeSelectType( lVal.cSelType );
    std::cout << "  BasculeSelModeCB " << PP3d::Selection::GetStrSelectType(lVal.cSelType) << std::endl;

		
    MyToggleButton* 	lBut0 =  reinterpret_cast<MyToggleButton*>( lToggle->cUserData3 );
    MyToggleButton* 	lBut1 =  reinterpret_cast<MyToggleButton*>( lToggle->cUserData4 );
    MyToggleButton* 	lBut2 =  reinterpret_cast<MyToggleButton*>( lToggle->cUserData5 );
    MyToggleButton* 	lBut3 =  reinterpret_cast<MyToggleButton*>( lToggle->cUserData6 );
    MyToggleButton* 	lBut4 =  reinterpret_cast<MyToggleButton*>( lToggle->cUserData7 );
	
    lBut0->value( true );
    lBut1->value( false );
    lBut2->value( false );
    lBut3->value( false );
    lBut4->value( false );

    std::cout << "BasculeSelModeCB " << lVal.cVal << std::endl;

	
    lWin3d->canvasRedraw();
  }
  //-------------------------------------------
  static void	BasculeViewModeCB(Fl_Widget*w, void*pData)
  {
    MyToggleButton* lToggle = reinterpret_cast<MyToggleButton*>( pData);
    Win3d* lWin3d = reinterpret_cast<Win3d*>( lToggle->cUserData1);
	

    std::cout << " value:" << (int)lToggle->value();
    if( lToggle->value() )
      lToggle->value( true );
    else
      lToggle->value( false );
    std::cout << " ---> value:" << (int)lToggle->value() << std::endl;

		
    lWin3d->cuCanvas3d->setVisualMode(  (int)lToggle->value() );
		

    lWin3d->canvasRedraw();
  }
  //-------------------------------------------
  static void	BasculeBoolCB(Fl_Widget*w, void*pData)
  {
    MyToggleButton* lToggle = reinterpret_cast<MyToggleButton*>( pData);
    Win3d* lWin3d = reinterpret_cast<Win3d*>( lToggle->cUserData1);
    bool* lBool = reinterpret_cast<bool*>( lToggle->cUserData2);
    *lBool = ! *lBool;
		
    lWin3d->canvasRedraw();
  }
  //-------------------------------------------
	
  static void	BasculeGridCB(Fl_Widget*w, void*pData)
  {
    MyToggleButton* lToggle = reinterpret_cast<MyToggleButton*>( pData);
    Win3d* lWin3d        = reinterpret_cast<Win3d*>( lToggle->cUserData1);
		
    ModeGrid *lGridMode = reinterpret_cast<ModeGrid*>( lToggle->cUserData2);
	 
    if( *lGridMode == ModeGrid::NO_GRID )
      {
	*lGridMode = ModeGrid::GRID_2D;
      }
    else 
      {
	*lGridMode = ModeGrid::NO_GRID;
      }
		
    lWin3d->canvasRedraw();
  }
  //-------------------------------------------
	
  static void	BasculePerspective(Fl_Widget*w, void*pData)
  {
    MyToggleButton* lToggle = reinterpret_cast<MyToggleButton*>( pData);
    Win3d* lWin3d        = reinterpret_cast<Win3d*>( lToggle->cUserData1);

    cout << "Changement perpective button" << endl;
    lWin3d->getKamera().chgModeKamera();
		
    lWin3d->canvasRedraw();
  }		
  //****************************************

  Win3d::Win3d(const char*pName, int pW, int pH, PP3d::DataBase & pDatabase )
    :Fl_Window( pW, pH, "Oxyd3d : 3d view" )
    ,cDatabase( pDatabase )
    ,cMenubar(0,0,10000, 30)
  {
    static int slWinId=1;
    cWinId=slWinId++;

    cuCanvas3d = 	std::make_unique<Canvas3d>(*this, 0, 100, this->w()-10, (this->h()-100)-16 , "1" );
		
    // cuCanvas3d = 	std::unique_ptr<Canvas3d>(new Canvas3d(10, 100, this->w()-10, this->h()-100, cDatabase, "1" ));
    //sw.mode(FL_RGB);
    this->resizable( cuCanvas3d.get() );
	
    //================
    int lX = 15;
    int lY = cMenubar.h();
    int lH = (int)(((float)cMenubar.h())*0.6f);
    int lW = 70;
											
    cXinput = new MyFloatInput( lX, lY, lW, lH, "X" );
    lX += cXinput->w() + 2;
    cCurrentInput1 = new MyFloatInput( lX, lY+lH, lW, lH, "Current" );
    cYinput = new MyFloatInput( lX, lY, lW, lH, "Y" );
    lX += cYinput->w() + 2;
    cZinput = new MyFloatInput( lX, lY, lW,  lH, "Z" );
    lX += cZinput->w() + 2;


    lW =  Application::sIconSize+4;
    lH =  Application::sIconSize+4;
		
    lX += lW;


    //========================		
    Fl_Image* lPixUndo = MyImage::LoadImage("Icons/undo.png", Application::sIconSize);

    cButUndo = new MyButton( lX, lY, lW, lH, nullptr,
			     UndoCB, this, nullptr);
    cButUndo->image( lPixUndo );
    lX += lW;
	
    Fl_Image* lPixRedo = MyImage::LoadImage("Icons/redo.png", Application::sIconSize);

    
    cButRedo = new MyButton( lX, lY, lW, lH, nullptr,
			     RedoCB, this, nullptr);
    cButRedo->image( lPixRedo );
    lX += lW;
	
    //    cCurrentUndo = new MyIntInput( lX, lY, lW, lH, "Pos " ); 
    //    cCurrentUndoMax = new MyIntInput( lX, lY+lH, lW, lH, "Size" );


    //========================		
    //========================		
    //========================		


    lX += lW*2;
    //========================		
    Fl_Image* lPixSel = MyImage::LoadImage("Icons/all.png", Application::sIconSize);

    MyToggleButton*
      lButSel00 = new MyToggleButton( lX, lY, lW, lH, nullptr,
				      BasculeSelModeCB, this, (void*)0 );
    lButSel00->value(false);
    lButSel00->image( lPixSel );
    lX += lW;

    //========================		
    lPixSel = MyImage::LoadImage("Icons/vertex.png", Application::sIconSize);

    MyToggleButton*
      lButSel0 = new MyToggleButton( lX, lY, lW, lH, nullptr,
				     BasculeSelModeCB, this, (void*)0 );
    lButSel0->value(false);
    lButSel0->image( lPixSel );
    lX += lW;
    //========================		
    lPixSel = MyImage::LoadImage("Icons/edge.png", Application::sIconSize);

    MyToggleButton*
      lButSel1 = new MyToggleButton( lX, lY, lW, lH, nullptr,
				     BasculeSelModeCB, this,   (void*)1);
    lButSel1->value(false );
    lButSel1->image( lPixSel );
    lX += lW;
    //========================		
    lPixSel = MyImage::LoadImage("Icons/face.png", Application::sIconSize);

    MyToggleButton*
      lButSel2 = new MyToggleButton( lX, lY, lW, lH, nullptr,
				     BasculeSelModeCB, this,  (void*)2);
    lButSel2->value(false );
    lButSel2->image( lPixSel );
    lX += lW;
    //========================		
    lPixSel = MyImage::LoadImage("Icons/body.png", Application::sIconSize);

    MyToggleButton*
      lButSel3 = new MyToggleButton( lX, lY, lW, lH, nullptr,
				     BasculeSelModeCB, this,  (void*)3);
    lButSel3->value(true );
    lButSel3->image( lPixSel );
    lX += lW;

    lButSel00->setUserData( this, (void*)PP3d::SelectType::All,   lButSel00, lButSel0, lButSel1, lButSel2,lButSel3  );
    lButSel0->setUserData( this, (void*)PP3d::SelectType::Point,  lButSel0, lButSel1, lButSel2, lButSel3, lButSel00 );
    lButSel1->setUserData( this, (void*)PP3d::SelectType::Line,   lButSel1, lButSel0, lButSel2, lButSel3, lButSel00 );
    lButSel2->setUserData( this, (void*)PP3d::SelectType::Facet,  lButSel2, lButSel0, lButSel1, lButSel3, lButSel00 );
    lButSel3->setUserData( this, (void*)PP3d::SelectType::Object,   lButSel3, lButSel0, lButSel1, lButSel2, lButSel00 );

    //=================================================================
    lX += lW*2;

    //========================		
    lPixSel = MyImage::LoadImage("Icons/skelet.png", Application::sIconSize);


    MyToggleButton*
      lBut1 = new MyToggleButton( lX, lY, lW, lH, nullptr,
				  BasculeViewModeCB, this,   (void*)1);
    lBut1->value(false );
    lBut1->image( lPixSel );
    lX += lW;
    //========================		
	

    lBut1->setUserData( this, lBut1);

    //========================
    MyToggleButton*			lBut = nullptr;

		
    //========================

    Fl_Image* lPixDif = MyImage::LoadImage("Icons/color.png", Application::sIconSize);
		
    lBut = new MyToggleButton( lX, lY, lW, lH, nullptr,
			       BasculeBoolCB, this, &cuCanvas3d->cFlagLightColor  );
    lBut->value( cuCanvas3d->cFlagLightColor);
    lBut->image( lPixDif );
    lX += lW;
		
    //========================

    Fl_Image* lPixBug = MyImage::LoadImage("Icons/debug.png", Application::sIconSize);
		
    lBut = new MyToggleButton( lX, lY, lW, lH, nullptr,
			       BasculeBoolCB, this, &cuCanvas3d->cDebug );
    lBut->value( cuCanvas3d->cDebug);
    lBut->image( lPixBug );
    lX += lW;


		
    //=================================================================
    lX += lW*2;
    //========================
    lBut = nullptr;


    
		
    Fl_Image* lPixPersp = MyImage::LoadImage("Icons/perspective.png", Application::sIconSize);
		
    lBut = new MyToggleButton( lX, lY, lW, lH, nullptr,
			       BasculePerspective, this );
    lBut->value( cuCanvas3d->getKamera().isPerspectiveOn() );
    lBut->image( lPixPersp );
    lX += lW;
    //========================
		
    Fl_Image* lPix2 = MyImage::LoadImage("Icons/grid.png", Application::sIconSize);
		
    lBut = new MyToggleButton( lX, lY, lW, lH, nullptr,
			       BasculeGridCB, this, &cuCanvas3d->cGridMode );
    lBut->value( cuCanvas3d->cGridMode ==  ModeGrid::GRID_2D );
    lBut->image( lPix2 );
    lX += lW;
    //========================
    Fl_Image* lPix1 = MyImage::LoadImage("Icons/axe.png", Application::sIconSize);

    lBut = new MyToggleButton( lX, lY, lW, lH, nullptr,
			       BasculeBoolCB, this, &cuCanvas3d->cAxisFlag );
    lBut->value( cuCanvas3d->cAxisFlag );
    lBut->image( lPix1 );
    lX += lW;



		
    cInfoOutput = new Fl_Output( 1, this->h() -16 , this->w()-2, 16, "Fl_InfoOutput");
    cInfoOutput->align(FL_ALIGN_BOTTOM);
    cInfoOutput->value("Welcome ...");

			
    //=================================================================
		

    
    //================================
    cMenubar.add("&File/"         StrMenu_NewBase,    "^n", MyMenuCallback, this); 
    cMenubar.add("&File/"         StrMenu_OpenBase,    "^o", MyMenuCallback, this);
    cMenubar.add("&File/"         StrMenu_MergeBase,  "", MyMenuCallback, this, FL_MENU_DIVIDER);
    
    cMenubar.add("&File/"         StrMenu_SaveBase,    "^s", MyMenuCallback, this);
    cMenubar.add("&File/"         StrMenu_SaveBaseAs,  "", MyMenuCallback, this);
    cMenubar.add("&File/"         StrMenu_SaveBaseSelect,  "", MyMenuCallback, this, FL_MENU_DIVIDER);
    
   
    cMenubar.add("&File/Export/"         StrMenu_ExportD3dObj,     "^e", MyMenuCallback, this);
    cMenubar.add("&File/Export/"         StrMenu_ExportSelectD3dObj,"", MyMenuCallback, this);
    cMenubar.add("&File/Import/"         StrMenu_ImportD3dObj,  "", MyMenuCallback, this, FL_MENU_DIVIDER);

    cMenubar.add("&File/"         StrMenu_Preferences,  "^p", MyMenuCallback, this, FL_MENU_DIVIDER);
    cMenubar.add("&File/&Quit",                     "^q", QuitCallback,   this);
									       
    //================================
    cMenubar.add("&Edit/&Copy",  "", MyMenuCallback, this);
    cMenubar.add("&Edit/&Paste", "", MyMenuCallback, this, FL_MENU_DIVIDER);

    cMenubar.add("&Selection/"  StrMenu_UnselectAll, "", MyMenuCallback, this);
    cMenubar.add("&Selection/"  StrMenu_DeleteSelect, "", MyMenuCallback, this);
    cMenubar.add("&Selection/"  StrMenu_AddSelectCopyToInput, "", MyMenuCallback, this);
    //================================

			
    cMenubar.add("&Create/" StrMenu_CreateCube , "^c", MyMenuCallback, this);
    cMenubar.add("&Create/" StrMenu_CreateTetra, "^t", MyMenuCallback, this );
    cMenubar.add("&Create/" StrMenu_CreatePyramid, "^p", MyMenuCallback, this);
    cMenubar.add("&Create/" StrMenu_CreateOcto, "^o", MyMenuCallback, this);
    cMenubar.add("&Create/" StrMenu_CreateDodec, "^d", MyMenuCallback, this);
    cMenubar.add("&Create/" StrMenu_CreateIcosahe, "^i", MyMenuCallback, this, FL_MENU_DIVIDER);
		
    cMenubar.add("&Create/" StrMenu_CreateShapeFacet, "^f", MyMenuCallback, this);
    cMenubar.add("&Create/" StrMenu_CreateShapePolyline, "^l",MyMenuCallback, this, FL_MENU_DIVIDER);

    cMenubar.add("&Create/" StrMenu_RevolX, "", MyMenuCallback, this);
    cMenubar.add("&Create/" StrMenu_RevolY, "", MyMenuCallback, this);
    cMenubar.add("&Create/" StrMenu_RevolZ, "", MyMenuCallback, this, FL_MENU_DIVIDER);

    cMenubar.add("&Create/" StrMenu_CallDialoDiagSub, "^b",MyMenuCallback, this, FL_MENU_DIVIDER);

    //================================
    cMenubar.add("&Transform/" StrMenu_TransformMoveX, "", MyMenuCallback, this);
    cMenubar.add("&Transform/" StrMenu_TransformMoveY, "", MyMenuCallback, this);
    cMenubar.add("&Transform/" StrMenu_TransformMoveZ, "", MyMenuCallback, this, FL_MENU_DIVIDER);
    
    cMenubar.add("&Transform/" StrMenu_TransformRotX, "", MyMenuCallback, this);
    cMenubar.add("&Transform/" StrMenu_TransformRotY, "", MyMenuCallback, this);
    cMenubar.add("&Transform/" StrMenu_TransformRotZ, "", MyMenuCallback, this);
	
    //================================
		
    cMenubar.add("&View/" StrMenu_DialogPerspectivSettings, "^p", MyMenuCallback, this);

    cMenubar.add("&Win/" StrMenu_Create3dView, "^v", MyMenuCallback, this);
    cMenubar.add("&Win/" StrMenu_ObjectTree, "^t", MyMenuCallback, this);
    //		cMenubar.add("&Win/" StrMenu_ConsolPython, nullptr, MyMenuCallback, this);
    cMenubar.add("&Win/" StrMenu_ConsolSystem, nullptr, MyMenuCallback, this);
    cMenubar.add("&Win/" StrMenu_ConsolLua, "^l", MyMenuCallback, this);
    //================================

    cMenubar.add("&Debug/" StrMenu_Demo1, nullptr, MyMenuCallback, this);
    cMenubar.add("&Debug/" StrMenu_Demo2, nullptr, MyMenuCallback, this);
    //================================

		
    /*
      menubar.menu(menutable);
      menubar.callback(test_cb);
      menus[0] = &menubar;
      Fl_Menu_Bar menubar2(0,31,window.w(),30); 
      menubar2.menu(menutable);
      menubar2.callback(test_cb);
      menus[1] = &menubar2;
    */
		
		
    cuCanvas3d->cPopup = new	Fl_Menu_Button( 0, 0, 1, 1);
		
    //		cuCanvas3d->cPopup->type(Fl_Menu_Button::POPUP3);
    //	cuCanvas3d->cPopup->add("This|is|a popup|menu");
		
    //================
	
    //    cFileChooser = new Fl_File_Chooser();
    
    end();
    show(0,nullptr);
	
    cuCanvas3d->show();
    resizable(*this);
  }

  //-------------------------------------------
  void Win3d::MyMenuCallback(Fl_Widget* w, void* pUserData) {
		
    static bool slFlagDialog=false; // C'est moche !!!!
    std::ostringstream lOsLuaCode;
    std::ostringstream lOsLuaOut;
	
							
		
    Fl_Menu_* mw = (Fl_Menu_*)w;
    const Fl_Menu_Item* m = mw->mvalue();		
    if (!m)
      {
	printf("NULL\n");
	return ;
      }


    printf("%s\n", m->label());
    M3d::Win3d* lWin3d = reinterpret_cast<M3d::Win3d*>(pUserData);
    M3d::Canvas3d* lCanvas =lWin3d->cuCanvas3d.get();

		
    Fl::focus(lCanvas);

    //========================================
    if( strcmp( m->label(), StrMenu_NewBase )== 0 )
      {
	if( fl_choice("Clear current work", "Yes", "No", 0)  == 0)
	  {
	    PushHistory(); // on sauve l'ancienne base dans l'historique
	    // Une Nlle base
	    std::unique_ptr<PP3d::DataBase> luBase( new PP3d::DataBase() );
	    luBase->resetIdFromMax(); // on prend en compte les id de la base lu 
	    Application::Instance().setDatabase( luBase ); // on prend la nlle base
	    Application::Instance().redrawAll();
	  }
      }
    else    //========================================
    if( strcmp( m->label(), StrMenu_OpenBase )== 0 )
      {
	Fl_File_Chooser* lFc = new Fl_File_Chooser(".", "*.oxyz",
						   Fl_File_Chooser::SINGLE,
						   "Open Oxyz3d database");	      
	lFc->callback( OpenBaseCB );
	lFc->show();
      }
    else    //========================================
      if( strcmp( m->label(),StrMenu_SaveBase	) == 0)
	{
	  if( MyPref.cLastSave.size() )
	    {
	      if( SaveBase( Application::Instance().getDatabase(), MyPref.cLastSave, 0 ) == false )
		{
		  fl_alert( "Saving database in <%s> failed",  MyPref.cLastSave.c_str());
		}
	    }
	}
      else //========================================
	if( strcmp( m->label(), StrMenu_SaveBaseAs ) == 0)
	  {
	    Fl_File_Chooser* lFc = new Fl_File_Chooser(".", "*.oxyz",
						       Fl_File_Chooser::CREATE,
						       "Save base as");
	   
	    lFc->callback( SaveBaseCB, 0 );			 
	    lFc->show();
	  }
	else //========================================
	  if( strcmp( m->label(), StrMenu_SaveBaseSelect ) == 0)
	  {
	    Fl_File_Chooser* lFc = new Fl_File_Chooser(".", "*.oxyz",
						       Fl_File_Chooser::CREATE,
						       "Save base as");
	    lFc->callback( SaveBaseCB, (void*)1 );			 
	    lFc->show();
	  }
	else //========================================
	  if( strcmp( m->label(), StrMenu_MergeBase)== 0 )
	    {
	      Fl_File_Chooser* lFc = new Fl_File_Chooser(".", "*.oxyz",
							 Fl_File_Chooser::SINGLE,
							 "Import Oxyz3d database");	      
	      lFc->callback( MergeBaseCB );
	      lFc->show();
	    }
	  else //========================================
	    if( strcmp( m->label(),StrMenu_ExportD3dObj ) == 0)
	      {	    
		Fl_File_Chooser* lFc = new Fl_File_Chooser(".", "*.d3d",
							   Fl_File_Chooser::CREATE,
							   "Export for d3d");
	    	    
		lFc->callback( ExportD3dObjCB, 0 );
		lFc->show();
	      }
	  else //========================================
	    if( strcmp( m->label(),StrMenu_ExportSelectD3dObj ) == 0)
	      {	    
		Fl_File_Chooser* lFc = new Fl_File_Chooser(".", "*.d3d",
							   Fl_File_Chooser::CREATE,
							   "Export for d3d");
	    	    
		lFc->callback( ExportD3dObjCB, (void*)1 );
		lFc->show();
	      }
	    else //========================================
	      if( strcmp( m->label(), StrMenu_ImportD3dObj )== 0 )
		{												
		  Fl_File_Chooser* lFc = new Fl_File_Chooser(".", "*.d3d",
							     Fl_File_Chooser::SINGLE,
							     "Import d3d file");	      
		  lFc->callback( ImportD3dObjCB );					
		  lFc->show();
		}
	      else //========================================
		if( strcmp( m->label(), StrMenu_Preferences ) == 0)
		  {
		    CallDialogPref();
		  }		
		else if( strcmp( m->label(),StrMenu_UnselectAll	) == 0)
		  {
		    PP3d::Selection::Instance().removeAll();
		    Application::Instance().redrawAllCanvas3d();
		    Application::Instance().redrawObjectTree();
		  }
		else if( strcmp( m->label(), StrMenu_DeleteSelect	) == 0)
		  {
		    cout << "Select menu :" << StrMenu_DeleteSelect << endl;
		    PP3d::Selection::Instance().deleteAllFromDatabase( *Application::Instance().getDatabase());
		    Application::Instance().redrawAllCanvas3d();
		    Application::Instance().redrawObjectTree();
		  }
		else if( strcmp( m->label(), StrMenu_AddSelectCopyToInput	) == 0)
		  {
		    cout << "Select menu :" << StrMenu_AddSelectCopyToInput << endl;
		    PP3d::Selection::Instance().addSelectionToInput( *Application::Instance().getDatabase(), false);				
		    Application::Instance().redrawAllCanvas3d();
		    Application::Instance().redrawObjectTree();
		  }
		else if( strcmp( m->label(), StrMenu_CreateCube ) == 0)
		  {
		    CallDialogPrimitiv( slFlagDialog, lCanvas, PP3d::PrimitivFactory::Type::CUBE  );
		  }
		else if( strcmp( m->label(), StrMenu_CreateTetra ) == 0)
		  {
		    CallDialogPrimitiv( slFlagDialog, lCanvas, PP3d::PrimitivFactory::Type::TETRA  );
		  }
		else if( strcmp( m->label(), StrMenu_CreatePyramid ) == 0)
		  {
		    CallDialogPrimitiv( slFlagDialog, lCanvas, PP3d::PrimitivFactory::Type::PYRAMID  );
		  }
		else if( strcmp( m->label(), StrMenu_CreateOcto ) == 0)
		  {
		    CallDialogPrimitiv( slFlagDialog,lCanvas, PP3d::PrimitivFactory::Type::OCTO  );
		  }
		else if( strcmp( m->label(), StrMenu_CreateDodec ) == 0)
		  {
		    CallDialogPrimitiv( slFlagDialog, lCanvas, PP3d::PrimitivFactory::Type::DODEC  );
		  }
		else if( strcmp( m->label(), StrMenu_CreateIcosahe ) == 0)
		  {
		    CallDialogPrimitiv( slFlagDialog, lCanvas, PP3d::PrimitivFactory::Type::ICOSAHED  );
		  }
		else if( strcmp( m->label(), StrMenu_CallDialoDiagSub ) == 0)
		  {
		    CallDialogSubDiv( slFlagDialog, lCanvas );
		  }
		else if( strncmp( m->label(), StrMenu_CreateShape, strlen(StrMenu_CreateShape)  ) == 0)
		  {
		    PP3d::Object * lShape=nullptr;
				
		    if( strcmp( m->label(), StrMenu_CreateShapeFacet ) == 0)
		      {
			if(  Application::Instance().getDatabase()->getNbCurrentPoints() >= 3 )
			  {
			    lShape = Application::Instance().getDatabase()->convertCurrentLineToFacet();
			    PushHistory();
			  }
			else {
			  SINFO ( lWin3d, "Error : Almost 3 points is requiered to create facet" );
			}
		      }
		    else if( strcmp( m->label(), StrMenu_CreateShapePolyline ) == 0)
		      {
			if(   Application::Instance().getDatabase()->getNbCurrentPoints() >= 2 )
			  {
			    lShape = Application::Instance().getDatabase()->convertCurrentLineToPolylines();
			    PushHistory();
			  }
			else {				
			  if(  Application::Instance().getDatabase()->getNbCurrentPoints() < 2 )
			    return;
							
			  if( strcmp( m->label(), StrMenu_CreateShapeFacet ) == 0)
			    {						
			      lShape = Application::Instance().getDatabase()->convertCurrentLineToFacet();
			      PushHistory();
			    }
						
			  SINFO ( lWin3d, "Error : Almost 2 points is requiered to create facet" );
			}
		      }
			
		    if( lShape != nullptr )
		      {
			Application::Instance().redrawAllCanvas3d();
			Application::Instance().redrawObjectTree();
		      }
		  }
		else if( strncmp( m->label(), StrMenu_Revol, strlen(StrMenu_Revol)  ) == 0)
		  {
		    if( strcmp( m->label(), StrMenu_RevolX )  == 0)
		      {
			CallDialogRevol( slFlagDialog, lCanvas, TypeRevol::RevolX  );
		      }
		    else if( strcmp( m->label(), StrMenu_RevolY )  == 0)
		      {
			CallDialogRevol( slFlagDialog, lCanvas, TypeRevol::RevolY  );
		      }

		    else if( strcmp( m->label(), StrMenu_RevolZ)  == 0)
		      {
			CallDialogRevol( slFlagDialog, lCanvas, TypeRevol::RevolZ  );
		      }									
		  }
    //============== TRANSFORMATION ====================
		else if( strcmp( m->label(), StrMenu_TransformMoveX ) == 0)
		  {
		    lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
		    Application::Instance().setCurrentTransformType(Transform::MoveX);
		  }
		else if( strcmp( m->label(), StrMenu_TransformMoveY ) == 0)
		  {
		    lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
		    Application::Instance().setCurrentTransformType(Transform::MoveY);
		  }
		else if( strcmp( m->label(), StrMenu_TransformMoveZ ) == 0)
		  {
		    lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
		    Application::Instance().setCurrentTransformType(Transform::MoveZ);
		  }
		else if( strcmp( m->label(), StrMenu_TransformRotX ) == 0)
		  {
		    lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
		    Application::Instance().setCurrentTransformType(Transform::CenterRotX);
		  }
		else if( strcmp( m->label(), StrMenu_TransformRotY ) == 0)
		  {
		    lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
		    Application::Instance().setCurrentTransformType( Transform::CenterRotY );
		  }
		else if( strcmp( m->label(), StrMenu_TransformRotZ ) == 0)
		  {
		    lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
		    Application::Instance().setCurrentTransformType(Transform::CenterRotZ );
		  }


    //=================== VIEW ====================
		else if( strcmp( m->label(), StrMenu_DialogPerspectivSettings ) == 0)
		  {
		    // Mettre un RAII
		    //		if( cFlagDialogPerspectiv )
		    //			return;
					 
		    //		cFlagDialogPerspectiv = true;
		    CallDialogPerspectiv( lCanvas,  lCanvas->getKamera() );
		    //			cFlagDialogPerspectiv = false;
		  }
    //================= WINDOWS ===================
		else if( strcmp( m->label(), StrMenu_ObjectTree ) == 0)
		  {
		    Application::Instance().createObjectTree( );
		  }
		else if( strcmp( m->label(), StrMenu_Create3dView ) == 0)
		  {
		    lOsLuaCode << "WinNewCanvas3d( 1000, 800 )"<< std::endl;
	
		    //	      Application::Instance().createNewWin3d( 1000, 800 );
		  }

    //				else if( strcmp( m->label(), StrMenu_ConsolPython ) == 0)
    //					{
    //						CallConsolePython( );
    //					}
		else if( strcmp( m->label(), StrMenu_ConsolSystem ) == 0)
		  {
		    CallConsoleSystem( );
		  }
		else if( strcmp( m->label(), StrMenu_ConsolLua ) == 0)
		  {
		    CallConsoleLua( );
		  }
		else if( strcmp( m->label(), 	StrMenu_Demo1 ) == 0)
		  {
		    Application::Instance().getDatabase()->demo1();
		    Application::Instance().redrawAllCanvas3d();
		    Application::Instance().redrawObjectTree();
		  }
		else if( strcmp( m->label(), 	StrMenu_Demo2 ) == 0)
		  {
		    Application::Instance().getDatabase()->demo2();
		    Application::Instance().redrawAllCanvas3d();
		    Application::Instance().redrawObjectTree();
		  }

				 
    if( lOsLuaCode.str().size() > 0 )
      {
	// Ily a du lua a executer
	if( Application::Instance().execLuaHisto(lOsLuaCode, lOsLuaOut) != nullptr)
	  {
	    // ERREUR
	  }
      }
  }
  //-------------------------------------------
  void Win3d::QuitCallback(Fl_Widget*, void*) {exit(0);}

  //-------------------------------------------
  void Win3d::setCursorPosition( PP3d::Point3d& pPos)
  {
    cXinput->setFloatValue( pPos.cX );
    cYinput->setFloatValue( pPos.cY );
    cZinput->setFloatValue( pPos.cZ );
  }
  //-------------------------------------------
  void Win3d::setUndoRedoState()
  {
    if(PP3d::UndoHistory::Instance().isMin())      
      cButUndo->deactivate();
    else
      cButUndo->activate();

    if(PP3d::UndoHistory::Instance().isMax())      
      cButRedo->deactivate();
    else
      cButRedo->activate();

    //    cCurrentUndo->setIntValue( PP3d::UndoHistory::Instance().getCurrent() );
    //    cCurrentUndoMax->setIntValue( PP3d::UndoHistory::Instance().getSize() );
  }
  //-------------------------------------------
  void Win3d::setCurrentVal( const char* iLabel, double iVal)
  {
    cCurrentInput1->copy_label( iLabel );

    cCurrentInput1->setFloatValue( iVal );
  }
  //-------------------------------------------
  double  Win3d::getCurrentVal()
  {
    return strtod(cCurrentInput1->value(), nullptr);
  }
  //****************************************
}
