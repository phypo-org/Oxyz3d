#include <config.h>

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
#include <FL/Fl_Menu_Button.H>
#include <FL/Fl_Output.H>


#include "Shape/DataBase.h"
#include "Shape/ObjectFacet.h"
#include "Shape/ObjectLine.h"

#include "Win3d.h"
#include "Canvas3d.h"
#include "Dialogs.h"

#include "Application.h"
#include "MyFlWidget.h"
#include "MyImage.h"

#include "Selection.h"

#include "Shape/SavRead.h"
#include "Shape/ExportObj.h"

#include <memory>

#include <fstream>


#include <FL/Fl_Native_File_Chooser.H>


namespace M3d {

  

#define StrMenu_SaveAll         "Save all"
#define StrMenu_Open            "Open"
#define StrMenu_ExportAllObj       "Export all (.obj)"
#define StrMenu_ImportObj          "Import (.obj)"



#define StrMenu_Create3dView    "New 3d  view"
#define StrMenu_ObjectTree      "Objects view"
#define StrMenu_WinHisto        "Histo   view"
#define StrMenu_ConsolPython    "Console python"
#define StrMenu_ConsolSystem    "Console system"
#define StrMenu_ConsolLua       "Console lua"


#define StrMenu_UnselectAll     "Unselect all"
#define StrMenu_DeleteSelect    "Delete selection"
#define StrMenu_AddSelectCopyToInput "Add selection to input (copy)"

#define StrMenu_CreateShape          "New Shape "
#define StrMenu_CreateShapePolyline  StrMenu_CreateShape  "Polyline"
#define StrMenu_CreateShapeFacet     StrMenu_CreateShape "Facet"

	
#define StrMenu_CallDialoDiagSub     "New Subdivide"

#define StrMenu_Revol     "New Revol"
#define StrMenu_RevolX    StrMenu_Revol "X"
#define StrMenu_RevolY    StrMenu_Revol "Y"
#define StrMenu_RevolZ    StrMenu_Revol "Z"
#define StrMenu_RevolFreeAxe StrMenu_Revol "free axe (disabled)"


	

#define StrMenu_DialogPerspectivSettings  "Perspective settings"


#define StrMenu_TransformMoveX    "Move X"
#define StrMenu_TransformMoveY    "Move Y"
#define StrMenu_TransformMoveZ    "Move Z"

#define StrMenu_TransformRotX     "Rotate X"
#define StrMenu_TransformRotY     "Rotate Y"
#define StrMenu_TransformRotZ     "Rotate Z"

	
#define StrMenu_CreateCube      "Cube"
#define StrMenu_CreateTetra     "Tetraede"
#define StrMenu_CreatePyramid   "Pyramide"
#define StrMenu_CreateOcto      "Octoedre"
#define StrMenu_CreateDodec     "Dodecaedre"
#define StrMenu_CreateIcosahe   "Icosaedre"

#define StrMenu_Demo1            "Demo 1"
#define StrMenu_Demo2            "Demo 2"

	
  using namespace std;

  //-------------------------------------------
  static void	UndoCB(Fl_Widget*w, void*pData)
  {
		//    MyButton* lToggle = reinterpret_cast<MyButton*>( pData);
		//   Win3d* lWin3d = reinterpret_cast<Win3d*>( lToggle->cUserData1);
	
    union ConvVoid
    {
      void *             cPtr;
      GLuint             cVal;
      PP3d::SelectType   cSelType; 
    };
	
	}
  //-------------------------------------------
  static void	RedoCB(Fl_Widget*w, void*pData)
  {
		//  MyButton* lToggle = reinterpret_cast<MyButton*>( pData);
		//  Win3d* lWin3d = reinterpret_cast<Win3d*>( lToggle->cUserData1);
	
    union ConvVoid
    {
      void *             cPtr;
      GLuint             cVal;
      PP3d::SelectType   cSelType; 
    };
	
	}
	//-------------------------------------------
  static void	BasculeSelModeCB(Fl_Widget*w, void*pData)
  {
    MyButton* lToggle = reinterpret_cast<MyButton*>( pData);
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
    Win3d* lWin3d           = reinterpret_cast<Win3d*>( lToggle->cUserData1);

    cout << "Changement perpective button" << endl;
    lWin3d->getKamera().chgModeKamera();
		
    lWin3d->canvasRedraw();
  }		
  //****************************************

  Win3d::Win3d(const char*pName, int pW, int pH, PP3d::DataBase & pDatabase )
    :Fl_Window( pW, pH)
    ,cDatabase( pDatabase )
    ,cMenubar(0,0, 700,30)
  {
    static int slWinId=1;
    cWinId=slWinId++;
    
    cuCanvas3d = 	std::make_unique<Canvas3d>(*this, 0, 100, this->w()-10, (this->h()-100)-16 , cDatabase, "1" );
		
    // cuCanvas3d = 	std::unique_ptr<Canvas3d>(new Canvas3d(10, 100, this->w()-10, this->h()-100, cDatabase, "1" ));
    //sw.mode(FL_RGB);
    this->resizable( cuCanvas3d.get() );
	
    //================
    int lX = 15;
    int lY = cMenubar.h();
    int lH = cMenubar.h()*0.6;
    int lW = 70;
											
    cXinput = new MyFloatInput( lX, lY, lW, lH, "X" );
    lX += cXinput->w() + 2;
    cYinput = new MyFloatInput( lX, lY, lW, lH, "Y" );
    lX += cYinput->w() + 2;
    cZinput = new MyFloatInput( lX, lY, lW,  lH, "Z" );
    lX += cZinput->w() + 2;


    lW =  Application::sIconSize+4;
    lH =  Application::sIconSize+4;
		
    lX += lW;

    //========================		
    Fl_Image* lPixSel = MyImage::LoadImage("Icons/undo.png", Application::sIconSize);


    MyButton*
      lButUndo = new MyButton( lX, lY, lW, lH, nullptr,  UndoCB, this);
    lButUndo->value(false );
    lButUndo->image( lPixSel );
    lX += lW;
    //========================		
    lPixSel = MyImage::LoadImage("Icons/redo.png", Application::sIconSize);

    MyButton*
      lButRedo = new MyButton( lX, lY, lW, lH, nullptr,RedoCB, this);
    lButRedo->value(false );
    lButRedo->image( lPixSel );
    lX += lW;
    //========================		
			
		lButUndo->setUserData( this, lButUndo,  lButRedo  );
		lButRedo->setUserData( this, lButUndo,  lButRedo  );

    //========================		
    //========================		
    //========================		


    lX += lW*2;
    //========================		
    lPixSel = MyImage::LoadImage("Icons/all.png", Application::sIconSize);

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
    MyToggleButton*			lBut = nullptr;

		//========================		
    lPixSel = MyImage::LoadImage("Icons/skelet.png", Application::sIconSize);


    MyToggleButton*
      lBut1 = new MyToggleButton( lX, lY, lW, lH, nullptr,
				  BasculeViewModeCB, this,   (void*)1);
    lBut1->value(false );
    lBut1->image( lPixSel );
    lX += lW;
   
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
		

    // Fl_Menu_Bar cMenubar(0,0,window.w(),30); 

    cMenubar.add("&File/"         StrMenu_Open,     "", MyMenuCallback, this);
    cMenubar.add("&File/"         StrMenu_SaveAll,  "", MyMenuCallback, this, FL_MENU_DIVIDER);
    cMenubar.add("&File/"         StrMenu_ExportAllObj,     "", MyMenuCallback, this);
    cMenubar.add("&File/"         StrMenu_ImportObj,  "", MyMenuCallback, this, FL_MENU_DIVIDER);
    cMenubar.add("&File/&Quit",                     "", QuitCallback,   this);
    cMenubar.add("&Edit/&Copy",  "", MyMenuCallback, this);
    cMenubar.add("&Edit/&Paste", "", MyMenuCallback, this, FL_MENU_DIVIDER);

    cMenubar.add("&Selection/"  StrMenu_UnselectAll, "", MyMenuCallback, this);
    cMenubar.add("&Selection/"  StrMenu_DeleteSelect, "", MyMenuCallback, this);
    cMenubar.add("&Selection/"  StrMenu_AddSelectCopyToInput, "", MyMenuCallback, this);

			
    cMenubar.add("&Create/" StrMenu_CreateCube , "^c", MyMenuCallback, this);
    cMenubar.add("&Create/" StrMenu_CreateTetra, "^t", MyMenuCallback, this );
    cMenubar.add("&Create/" StrMenu_CreatePyramid, "^t", MyMenuCallback, this);
    cMenubar.add("&Create/" StrMenu_CreateOcto, "^t", MyMenuCallback, this);
    cMenubar.add("&Create/" StrMenu_CreateDodec, "^t", MyMenuCallback, this);
    cMenubar.add("&Create/" StrMenu_CreateIcosahe, "^t", MyMenuCallback, this, FL_MENU_DIVIDER);
		
    cMenubar.add("&Create/" StrMenu_CreateShapeFacet, "^t", MyMenuCallback, this);
    cMenubar.add("&Create/" StrMenu_CreateShapePolyline, "^t",MyMenuCallback, this, FL_MENU_DIVIDER);

    cMenubar.add("&Create/" StrMenu_RevolX, "", MyMenuCallback, this);
    cMenubar.add("&Create/" StrMenu_RevolY, "", MyMenuCallback, this);
    cMenubar.add("&Create/" StrMenu_RevolZ, "", MyMenuCallback, this
		 );
    cMenubar.add("&Create/" StrMenu_RevolFreeAxe,  "", MyMenuCallback, this, FL_MENU_DIVIDER);
		 
    cMenubar.add("&Create/" StrMenu_CallDialoDiagSub, "^t",MyMenuCallback, this, FL_MENU_DIVIDER);

    cMenubar.add("&Transform/" StrMenu_TransformMoveX, "", MyMenuCallback, this);
    cMenubar.add("&Transform/" StrMenu_TransformMoveY, "", MyMenuCallback, this);
    cMenubar.add("&Transform/" StrMenu_TransformMoveZ, "", MyMenuCallback, this);
    cMenubar.add("&Transform/" StrMenu_TransformRotX, "", MyMenuCallback, this);
    cMenubar.add("&Transform/" StrMenu_TransformRotY, "", MyMenuCallback, this);
    cMenubar.add("&Transform/" StrMenu_TransformRotZ, "", MyMenuCallback, this);
	
		
    cMenubar.add("&View/" StrMenu_DialogPerspectivSettings, "^t", MyMenuCallback, this);

    cMenubar.add("&Win/" StrMenu_Create3dView, "^t", MyMenuCallback, this);
    cMenubar.add("&Win/" StrMenu_ObjectTree, "^t", MyMenuCallback, this);
    cMenubar.add("&Win/" StrMenu_WinHisto, "^t", MyMenuCallback, this);
    //		cMenubar.add("&Win/" StrMenu_ConsolPython, nullptr, MyMenuCallback, this);
    cMenubar.add("&Win/" StrMenu_ConsolSystem, nullptr, MyMenuCallback, this);
    cMenubar.add("&Win/" StrMenu_ConsolLua, nullptr, MyMenuCallback, this);

    cMenubar.add("&Debug/" StrMenu_Demo1, nullptr, MyMenuCallback, this);
    cMenubar.add("&Debug/" StrMenu_Demo2, nullptr, MyMenuCallback, this);

		
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


    if( strcmp( m->label(),StrMenu_SaveAll	) == 0)
      {
	Fl_Native_File_Chooser fnfc;
	fnfc.title("Pick a file for write");
	fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
	fnfc.filter("3D\t*.oxyz\n"
		    "3D Files\t*.{oxyz}");
	fnfc.directory(".");           // default directory to use
	// Show native chooser
	switch ( fnfc.show() ) {
	case -1: printf("ERROR: %s\n", fnfc.errmsg());    break;  // ERROR
	case  1: printf("CANCEL\n");                      break;  // CANCEL
	default:
	  {
	    std::cout << "SAVE TO: " << fnfc.filename() << std::endl;

	    std::ofstream lOut;						
	    lOut.open( fnfc.filename());
	    if( lOut.good() )
	      {
				
		PP3d::MySav lSav( lOut );
				
		lSav.save( lCanvas->getDataBase());

		lOut.close();
	      }
	  }
	  break;						
	}				 
      }
    else
      if( strcmp( m->label(),StrMenu_ExportAllObj	) == 0)
	{
	  Fl_Native_File_Chooser fnfc;
	  fnfc.title("Pick a file for export (.obj)");
	  fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
	  fnfc.filter("3D obj\t*.obj\n"
		      "3D obj Files\t*.{obj}");
	  fnfc.directory(".");           // default directory to use
	  // Show native chooser
	  switch ( fnfc.show() ) {
	  case -1: printf("ERROR: %s\n", fnfc.errmsg());    break;  // ERROR
	  case  1: printf("CANCEL\n");                      break;  // CANCEL
	  default:
	    {
	      std::cout << "EXPORT TO: " << fnfc.filename() << std::endl;

	      std::ofstream lOut;						
	      lOut.open( fnfc.filename());
	      if( lOut.good() )
		{
				
		  PP3d::MyExportObj lExpObj( lOut );
				
		  lExpObj.save( lCanvas->getDataBase());

		  lOut.close();
		}
	    }
	    break;						
	  }				 
	}
      else
	if( strcmp( m->label(), StrMenu_Open )== 0 )
	  {				
	    Fl_Native_File_Chooser fnfc;
	    fnfc.title("Pick a file for read");
	    fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
	    fnfc.filter("3D\t*.oxyz\n"
			"3D Files\t*.{oxyz}");
	    fnfc.directory(".");           // default directory to use
	    // Show native chooser
	    switch ( fnfc.show() ) {
	    case -1: printf(">>>>>>>>>>>>>>ERROR: %s\n", fnfc.errmsg());    break;  // ERROR
	    case  1: printf(">>>>>>>>>>>>>>CANCEL\n");                      break;  // CANCEL

	    default:
	      {
			
		std::cout << " READ FROM : " << fnfc.filename() << std::endl;
						
		std::ifstream lFileIn;						
		lFileIn.open( fnfc.filename());
						
		if( lFileIn.good() )
		  {								
		    PP3d::MyRead lRead( lFileIn );
		    lRead.read( lCanvas->getDataBase() );
		    lFileIn.close();								
		  }
	      }
	      break;						
	    }
			
			Application::Instance().validate( History::SaveMode::Reset );
			
	    return;
	  }	
	else
	  if( strcmp( m->label(), StrMenu_ImportObj )== 0 )
	    {				
	      Fl_Native_File_Chooser fnfc;
	      fnfc.title("Pick a file for read");
	      fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
	      fnfc.filter("3D\t*.obj\n"
										"3D Files\t*.{obj}");
	      fnfc.directory(".");           // default directory to use
	      // Show native chooser
	      switch ( fnfc.show() ) {
	      case -1: printf(">>>>>>>>>>>>>>ERROR: %s\n", fnfc.errmsg());    break;  // ERROR
	      case  1: printf(">>>>>>>>>>>>>>CANCEL\n");                      break;  // CANCEL
								
	      default:
					{
									
						std::cout << "IMPORT FROM : " << fnfc.filename() << std::endl;
									
						std::ifstream lFileIn;						
						lFileIn.open( fnfc.filename());
									
						if( lFileIn.good() )
							{								
								PP3d::MyImportObj lRead( lFileIn );
								lRead.read( lCanvas->getDataBase() );
								lFileIn.close();								
							}
					}
					break;						
	      }				 
				Application::Instance().validate( History::SaveMode::Full);			

	      return;
	    }
		
	  else if( strcmp( m->label(),StrMenu_UnselectAll	) == 0)
	    {
	      PP3d::Selection::Instance().removeAll();
				Application::Instance().validate( History::SaveMode::Mini);			
	    }
	  else if( strcmp( m->label(), StrMenu_DeleteSelect	) == 0)
	    {
	      cout << "Select menu :" << StrMenu_DeleteSelect << endl;
	      PP3d::Selection::Instance().deleteAllFromDatabase(lCanvas->getDataBase());
				Application::Instance().validate( History::SaveMode::Full);			

	    }
	  else if( strcmp( m->label(), StrMenu_AddSelectCopyToInput	) == 0)
	    {
	      cout << "Select menu :" << StrMenu_AddSelectCopyToInput << endl;
	      PP3d::Selection::Instance().addSelectionToInput(lCanvas->getDataBase(), false);				
				Application::Instance().validate( History::SaveMode::Mini);			
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
		  if(  lCanvas->getDataBase().getNbCurrentPoints() >= 3 )
		    {
		      lShape = lCanvas->getDataBase().convertCurrentLineToFacet();
		    }
		  else {
		    SINFO ( lWin3d, "Error : Almost 3 points is requiered to create facet" );
		  }
		}
	      else if( strcmp( m->label(), StrMenu_CreateShapePolyline ) == 0)
		{
		  if(  lCanvas->getDataBase().getNbCurrentPoints() >= 2 )
		    {
		      lShape = lCanvas->getDataBase().convertCurrentLineToPolylines();
		    }
		  else {				
		    if(  lCanvas->getDataBase().getNbCurrentPoints() < 2 )
		      return;
							
		    if( strcmp( m->label(), StrMenu_CreateShapeFacet ) == 0)
		      {						
			lShape = lCanvas->getDataBase().convertCurrentLineToFacet();
		      }
						
		    SINFO ( lWin3d, "Error : Almost 2 points is requiered to create facet" );
		  }
		}
			
	      if( lShape != nullptr )
		{
			Application::Instance().validate( History::SaveMode::Diff);			
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
	      else if( strcmp( m->label(), StrMenu_RevolZ)  == 0)
		{
		  CallDialogRevol( slFlagDialog, lCanvas, TypeRevol::RevolFreeAxe  );
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
	  else if( strcmp( m->label(), StrMenu_Create3dView ) == 0)
	    {
				lOsLuaCode << "WinNewCanvas3d( 1000, 800 )"<< std::endl;
	
				//	      Application::Instance().createNewWin3d( 1000, 800 );
	    }
	  else if( strcmp( m->label(), StrMenu_ObjectTree ) == 0)
	    {
	      Application::Instance().createObjectTree( );
	    }
	  else if( strcmp( m->label(), StrMenu_WinHisto ) == 0)
	    {
	      Application::Instance().createWinHisto( );
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
	      lCanvas->getDataBase().demo1();
				Application::Instance().validate( History::SaveMode::Diff);			

	    }
	  else if( strcmp( m->label(), 	StrMenu_Demo2 ) == 0)
	    {
	      lCanvas->getDataBase().demo2();
				Application::Instance().validate( History::SaveMode::Diff);			
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
  //****************************************
};
