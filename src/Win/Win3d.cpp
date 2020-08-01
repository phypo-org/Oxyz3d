#include "config.h"

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
#include "Shape/Selection.h"
#include "Shape/SavRead.h"
#include "Shape/ExportObj.h"

#include "Utils/PPFile.h"
#include "Shape/PP3dUtils.h"

#include "Win3d.h"
#include "Canvas3d.h"
#include "Dialogs.h"

#include "Application.h"
#include "MyFlWidget.h"
#include "MyImage.h"
#include "Preference.h"



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


#define StrMenu_DefDefaultAxe   "Define default axe as default"
#define StrMenu_DefDefaultPlane "Define default plane as default"
#define StrMenu_DefDefaultTrans "Define default translation as default"

#define StrMenu_SetAxePoints   "With selection"
  //#define StrMenu_SetAxeLine     "With line"
#define StrMenu_SetAxeNormal   "With facet normal"
#define StrMenu_SetAxeInput    "With last two input points"

#define StrMenu_SetPlanePoints "With three points"
#define StrMenu_SetPlaneLines  "With two connect Lines"
#define StrMenu_SetPlaneFacet  "With one facet"
#define StrMenu_SetPlaneInput  "With last three input points"

#define StrMenu_SetTransPoints "With two points"
#define StrMenu_SetTransLine   "With line"
#define StrMenu_SetTransInput  "With last two input points"

#define StrMenu_PutOnGround    "Put selection on ground"
#define StrMenu_PutUnderGround "Put selection under ground"
#define StrMenu_Recenter       "Recenter selection"


#define StrMenu_DialogPerspectivSettings  "Perspective settings ..."

#define StrMenu_ViewReset  "Reset View"
#define StrMenu_ViewAlongX "View along X"
#define StrMenu_ViewAlongY "View along Y"
#define StrMenu_ViewAlongZ "View along Z"
#define StrMenu_ViewAlong_X "View along -X"
#define StrMenu_ViewAlong_Y "View along -Y"
#define StrMenu_ViewAlong_Z "View along -Z"
#define StrMenu_ViewRecenterOnSelection "Recenter view on selection - (bug)"

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
	    TheSelect.execVisitorOnlyOnObjects( lVisit );	    	    
	    lRet = lSav.save( *ioDatabase, &TheSelect, &lVisit.cSetAllEntity );
	  }
	else
	  {
	    lRet = lSav.save( *ioDatabase,  &TheSelect, nullptr );
	  }
	
	lOut.close();
	return lRet;
      }
    return false;
  }
  //-------------------------------------------
  static bool OpenBase( PP3d::DataBase * ioDatabase,const std::string & iName, bool iKeepId )
  {
    std::ifstream lIn;						
    lIn.open( iName );
    
    if( lIn.good() )
      {									
	PP3d::MyRead lRead( lIn );
	bool lRet = lRead.read( *ioDatabase, &TheSelect, iKeepId );
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
	    TheSelect.execVisitorOnlyOnObjects( lVisit );	    
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
	std::string lTmp = PPu::PPFile::WithoutExtension( PPu::PPFile::LastPath( iName ) );
	lRead.setName( lTmp );
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

	if( SaveBase( TheAppli.getDatabase(), lFilename, cData) )
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
	    TheAppli.setDatabase( luBase ); // on prend la nlle base
	    
	    MyPref.cLastSave = lFilename;
	    TheAppli.redrawAll();
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
	if( OpenBase( TheAppli.getDatabase(), lFilename, false)) // On change les ID !!!!!!!!!!!!!!!!!!!!!
	  {
	    MyPref.cLastSave = lFilename;
	    TheAppli.redrawAll();
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

	
	if( ExportD3dObj( TheAppli.getDatabase(), lFilename, cData ))
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
	if( ImportD3dObj(  TheAppli.getDatabase(), lFilename ) ) // On change les ID !!!!!!!!!!!!!!!!!!!!!
	  {
	    TheAppli.redrawAll();
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
    
    TheSelect.removeAll();
    
    std::unique_ptr<PP3d::DataBase> luBase( new PP3d::DataBase() );
    if( PP3d::UndoHistory::Instance().readPrev( *luBase, &TheSelect ) )
      {
	luBase->resetIdFromMax();
	//	std::cout << "UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU" << std::endl;

 	TheAppli.setDatabase( luBase );
      }
    else
      TheSelect.removeAll();

    TheAppli.redrawAllCanvas3d();
    TheAppli.redrawObjectTree();
  }
  //-------------------------------------------
  static void RedoCB(Fl_Widget*w, void*pData)
  {
    MyButton* lButton = reinterpret_cast<MyButton*>( pData);
    Win3d* lWin3d = reinterpret_cast<Win3d*>( lButton->cUserData1);
    
    TheSelect.removeAll();
    std::unique_ptr<PP3d::DataBase> luBase( new PP3d::DataBase() );
    if( PP3d::UndoHistory::Instance().readNext( *luBase, &TheSelect ) )
      {
	luBase->resetIdFromMax();
	//	std::cout << "RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR" << std::endl;

	TheAppli.setDatabase( luBase );
      }
    else
      TheSelect.removeAll();
      
 	
    TheAppli.redrawAll();
  }
  //------------------------------------------------------
  static void CB_ViewTransfrom(Fl_Widget*w, void*pData)
  {
    MyToggleButton* lToggle = reinterpret_cast<MyToggleButton*>( pData);
    Win3d* lWin3d = reinterpret_cast<Win3d*>( lToggle->cUserData1);
    
    lWin3d->cuCanvas3d->setViewTransform(  (bool)lToggle->value() );
    lWin3d->canvasRedraw();
  }
  //-------------------------------------------
  //-------------------------------------------
  //-------------------------------------------
  // Change selection mode
  
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
	
       
   TheSelect.changeSelectType( lVal.cSelType );
   if(lVal.cSelType == PP3d::SelectType::Null )
     {
       TheSelectTransform.changeSelectType( PP3d::SelectType::Object );       
     }
   else
     {
       TheSelectTransform.changeSelectType( PP3d::SelectType::Null );       
     }
      

    // std::cout << "  BasculeSelModeCB " << PP3d::Selection::GetStrSelectType(lVal.cSelType) << std::endl;
    

		
    MyToggleButton* 	lBut0 =  reinterpret_cast<MyToggleButton*>( lToggle->cUserData3 );
    MyToggleButton* 	lBut1 =  reinterpret_cast<MyToggleButton*>( lToggle->cUserData4 );
    MyToggleButton* 	lBut2 =  reinterpret_cast<MyToggleButton*>( lToggle->cUserData5 );
    MyToggleButton* 	lBut3 =  reinterpret_cast<MyToggleButton*>( lToggle->cUserData6 );
    MyToggleButton* 	lBut4 =  reinterpret_cast<MyToggleButton*>( lToggle->cUserData7 );
    //    MyToggleButton* 	lBut5 =  reinterpret_cast<MyToggleButton*>( lToggle->cUserData8 );

    lBut0->value( true );
    lBut1->value( false );
    lBut2->value( false );
    lBut3->value( false );
    lBut4->value( false );
    //    lBut5->value( false );

    std::cout << "BasculeSelModeCB " << lVal.cVal << std::endl;

    TheAppli.redrawAll();
  }
  //-------------------------------------------
  // Change view mode
  
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
    cButUndo->tooltip("Undo");
    lX += lW;
	
    Fl_Image* lPixRedo = MyImage::LoadImage("Icons/redo.png", Application::sIconSize);

    
    cButRedo = new MyButton( lX, lY, lW, lH, nullptr,
			     RedoCB, this, nullptr);
    cButRedo->image( lPixRedo );
    cButUndo->tooltip("Redo");
    lX += lW;
	
    //    cCurrentUndo = new MyIntInput( lX, lY, lW, lH, "Pos " ); 
    //    cCurrentUndoMax = new MyIntInput( lX, lY+lH, lW, lH, "Size" );


    //========================		
    //========================		
    //========================		


    lX += lW;


    //========================		
    Fl_Image* lPixSel = MyImage::LoadImage("Icons/SelectTransformOnOff.png", Application::sIconSize);

    MyToggleButton*
      lButSelTran = new MyToggleButton( lX, lY, lW, lH, nullptr,
				      BasculeSelModeCB, this, (void*)4 );
    lButSelTran->value(false);
    lButSelTran->image( lPixSel );
    lButSelTran->tooltip("Enable selection for transformation : axe, plane ...");
    lX += lW;

 
    //========================		
    //    lPixSel = MyImage::LoadImage("Icons/all.png", Application::sIconSize);

    MyToggleButton* lButSelAll = nullptr ;
    //new MyToggleButton( lX, lY, lW, lH, nullptr,
    //				      BasculeSelModeCB, this, (void*)0 );
    //    lButSelAll->value(false);
    //    lButSelAll->image( lPixSel );
    //    lButSelAll->tooltip("Enable selection for all type (execpt transformation)");
   lX += lW;

    //========================		
    lPixSel = MyImage::LoadImage("Icons/vertex.png", Application::sIconSize);

    MyToggleButton*
      lButSelPt = new MyToggleButton( lX, lY, lW, lH, nullptr,
				     BasculeSelModeCB, this, (void*)0 );
    lButSelPt->value(false);
    lButSelPt->image( lPixSel );
    lButSelPt->tooltip("Enable selection for points of objects");
    lX += lW;
    //========================		
    lPixSel = MyImage::LoadImage("Icons/edge.png", Application::sIconSize);

    MyToggleButton*
      lButSelLine = new MyToggleButton( lX, lY, lW, lH, nullptr,
				     BasculeSelModeCB, this,   (void*)1);
    lButSelLine->value(false );
    lButSelLine->image( lPixSel );
    lButSelLine->tooltip("Enable selection for lines of objects");
    lX += lW;
    //========================		
    lPixSel = MyImage::LoadImage("Icons/face.png", Application::sIconSize);

    MyToggleButton*
      lButFacet = new MyToggleButton( lX, lY, lW, lH, nullptr,
				     BasculeSelModeCB, this,  (void*)2);
    lButFacet->value(false );
    lButFacet->image( lPixSel );
    lButFacet->tooltip("Enable selection for facets of objects");
    lX += lW;
    //========================		
    lPixSel = MyImage::LoadImage("Icons/body.png", Application::sIconSize);

    MyToggleButton*
      lButSelBody = new MyToggleButton( lX, lY, lW, lH, nullptr,
				     BasculeSelModeCB, this,  (void*)3);
    lButSelBody->value(true );
    lButSelBody->image( lPixSel );
    lButSelBody->tooltip("Enable selection for objects");
    lX += lW;

    //    lButSelAll->setUserData(  this, (void*)PP3d::SelectType::All,    lButSelAll,  lButSelPt,   lButSelLine, lButFacet,   lButSelBody, lButSelTran  );
    lButSelPt->setUserData(   this, (void*)PP3d::SelectType::Point,  lButSelPt,   lButSelLine, lButFacet,   lButSelBody,  lButSelTran, lButSelAll  );
    lButSelLine->setUserData( this, (void*)PP3d::SelectType::Line,   lButSelLine, lButSelPt,   lButFacet,   lButSelBody,  lButSelTran, lButSelAll  );
    lButFacet->setUserData(   this, (void*)PP3d::SelectType::Facet,  lButFacet,   lButSelPt,   lButSelLine, lButSelBody,  lButSelTran, lButSelAll  );
    lButSelBody->setUserData( this, (void*)PP3d::SelectType::Object, lButSelBody, lButSelPt,   lButSelLine, lButFacet,  lButSelTran,   lButSelAll  );
    lButSelTran->setUserData( this, (void*)PP3d::SelectType::Null,   lButSelTran, lButSelBody, lButSelPt,   lButSelLine, lButFacet,   lButSelAll   );

    //=================================================================
    lX += lW*2;

    //========================		
    lPixSel = MyImage::LoadImage("Icons/skelet.png", Application::sIconSize);


    MyToggleButton*
      lBut1 = new MyToggleButton( lX, lY, lW, lH, nullptr,
				  BasculeViewModeCB, this,   (void*)1);
    lBut1->value( cuCanvas3d->cFlagViewTransform);
    lBut1->image( lPixSel );
    lBut1->tooltip("Wire");
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
    lBut->tooltip("Colored draw");
    lX += lW;
		
    //========================

    Fl_Image* lPixViewNormal = MyImage::LoadImage("Icons/ViewNormal.png", Application::sIconSize);
		
    lBut = new MyToggleButton( lX, lY, lW, lH, nullptr,
			       BasculeBoolCB, this, &cuCanvas3d->cFlagViewNormal );
    lBut->value( cuCanvas3d->cFlagViewNormal);
    lBut->image( lPixViewNormal );
    lBut->tooltip("Grayed draw");
    lX += lW;


    //========================

    Fl_Image* lPixViewTransform = MyImage::LoadImage("Icons/ViewTransformOnOff.png", Application::sIconSize);
		
    lBut = new MyToggleButton( lX, lY, lW, lH, nullptr,
			       CB_ViewTransfrom, this );
    lBut->value( cuCanvas3d->getViewTransform());
    lBut->image( lPixViewTransform );
    lBut->tooltip("On/off for transformations draw");
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
    lBut->tooltip("Perpective draw");
    lX += lW;
    //========================
		
    Fl_Image* lPix2 = MyImage::LoadImage("Icons/grid.png", Application::sIconSize);
		
    lBut = new MyToggleButton( lX, lY, lW, lH, nullptr,
			       BasculeGridCB, this, &cuCanvas3d->cGridMode );
    lBut->value( cuCanvas3d->cGridMode ==  ModeGrid::GRID_2D );
    lBut->image( lPix2 );
    lBut->tooltip("On/off for grid");
   lX += lW;
    //========================
    Fl_Image* lPix1 = MyImage::LoadImage("Icons/axe.png", Application::sIconSize);

    lBut = new MyToggleButton( lX, lY, lW, lH, nullptr,
			       BasculeBoolCB, this, &cuCanvas3d->cAxisFlag );
    lBut->value( cuCanvas3d->cAxisFlag );
    lBut->image( lPix1 );
    lBut->tooltip("On/off for XYZ axis");
    lX += lW;



		
    cInfoOutput = new Fl_Output( 1, this->h() -16 , this->w()-2, 16, "Fl_InfoOutput");
    cInfoOutput->align(FL_ALIGN_BOTTOM);
    cInfoOutput->value("Welcome ...");

			
    //=================================================================
		

    
    //================================
    cMenubar.add("&File/"    StrMenu_NewBase,    "^n", MyMenuCallback, this); 
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

    
    cMenubar.add("&Utils/" StrMenu_DefDefaultAxe  , "", MyMenuCallback, this);
    cMenubar.add("&Utils/" StrMenu_DefDefaultPlane, "", MyMenuCallback, this);
    cMenubar.add("&Utils/" StrMenu_DefDefaultTrans, "", MyMenuCallback, this, FL_MENU_DIVIDER);

    cMenubar.add("&Utils/Create axe/" StrMenu_SetAxePoints  , "", MyMenuCallback, this);
    //    (TheSelect.getSelectType() != PP3d::SelectType::Point
    //     && TheSelect.getNbSelected() < 2
    //   ? FL_MENU_INACTIVE :0));
		   
    //    cMenubar.add("&Utils/Define axe/" StrMenu_SetAxeLine    , "", MyMenuCallback, this);
    //		 (TheSelect.getSelectType() != PP3d::SelectType::Line
    //		  && TheSelect.getNbSelected() < 1
    //		  ? FL_MENU_INACTIVE :0));
  
  cMenubar.add("&Utils/Create axe/" StrMenu_SetAxeNormal  , "", MyMenuCallback, this);
  //	       (TheSelect.getSelectType() != PP3d::SelectType::Facet
  //	       && TheSelect.getNbSelected() < 1
  //	       ? FL_MENU_INACTIVE :0) );
  
  cMenubar.add("&Utils/Create axe/" StrMenu_SetAxeInput   , "", MyMenuCallback, this);
  //	       (TheAppli.getDatabase()->getNbCurrentPoints()  < 2
  //	       ? FL_MENU_INACTIVE :0) );
    
  cMenubar.add("&Utils/Create plane/" StrMenu_SetPlanePoints, "", MyMenuCallback, this);
  //	       (TheSelect.getSelectType() != PP3d::SelectType::Point
  //		&& TheSelect.getNbSelected() < 3
  //		? FL_MENU_INACTIVE :0));
  
  cMenubar.add("&Utils/Create plane/" StrMenu_SetPlaneLines , "", MyMenuCallback, this);
  //	       (TheSelect.getSelectType() != PP3d::SelectType::Line
  //		&& TheSelect.getNbSelected() < 1
  //		? FL_MENU_INACTIVE :0));
  
  cMenubar.add("&Utils/Create plane/" StrMenu_SetPlaneFacet, "", MyMenuCallback, this);
  //	       (TheSelect.getSelectType() != PP3d::SelectType::Facet
  //		&& TheSelect.getNbSelected() < 1
  //		? FL_MENU_INACTIVE :0));
	       
  cMenubar.add("&Utils/Create plane/" StrMenu_SetPlaneInput, "", MyMenuCallback, this);
  //	       (TheAppli.getDatabase()->getNbCurrentPoints()  < 2
  //	       ? FL_MENU_INACTIVE :3) );

  cMenubar.add("&Utils/Create translation/" StrMenu_SetTransPoints, "", MyMenuCallback, this);
  //	       (TheSelect.getSelectType() != PP3d::SelectType::Point
  //		&& TheSelect.getNbSelected() < 2
  //		? FL_MENU_INACTIVE :0));
  
  
  cMenubar.add("&Utils/Create translation/" StrMenu_SetTransLine  , "", MyMenuCallback, this);
  //	       (TheSelect.getSelectType() != PP3d::SelectType::Line
  //		&& TheSelect.getNbSelected() < 1
  //		? FL_MENU_INACTIVE :0));
  
  cMenubar.add("&Utils/Create translation/" StrMenu_SetTransInput  , "", MyMenuCallback, this);
	       //	       (TheAppli.getDatabase()->getNbCurrentPoints()  < 2) );
  cMenubar.add("&Utils/", "", MyMenuCallback, this, FL_MENU_DIVIDER);
	       
  cMenubar.add("&Utils/" StrMenu_PutOnGround ,    "", MyMenuCallback, this );
  cMenubar.add("&Utils/" StrMenu_PutUnderGround , "", MyMenuCallback, this );
  cMenubar.add("&Utils/" StrMenu_Recenter , "", MyMenuCallback, this );

    //================================
		
    cMenubar.add("&View/" StrMenu_DialogPerspectivSettings, "^p", MyMenuCallback, this, FL_MENU_DIVIDER);
    cMenubar.add("&View/" StrMenu_ViewReset,  "o", MyMenuCallback, this, FL_MENU_DIVIDER);
    cMenubar.add("&View/" StrMenu_ViewAlongX, "x", MyMenuCallback, this);
    cMenubar.add("&View/" StrMenu_ViewAlongY, "y", MyMenuCallback, this);
    cMenubar.add("&View/" StrMenu_ViewAlongZ, "z", MyMenuCallback, this);
    cMenubar.add("&View/" StrMenu_ViewAlong_X, "", MyMenuCallback, this);
    cMenubar.add("&View/" StrMenu_ViewAlong_Y, "", MyMenuCallback, this);
    cMenubar.add("&View/" StrMenu_ViewAlong_Z, "", MyMenuCallback, this);
    cMenubar.add("&View/" StrMenu_ViewAlong_Y, "", MyMenuCallback, this);
    cMenubar.add("&View/" StrMenu_ViewRecenterOnSelection, "", MyMenuCallback, this);
      
    //================================
    cMenubar.add("&Win/" StrMenu_Create3dView, "^v", MyMenuCallback, this);
    cMenubar.add("&Win/" StrMenu_ObjectTree, "^t", MyMenuCallback, this);
    //		cMenubar.add("&Win/" StrMenu_ConsolPython, nullptr, MyMenuCallback, this);
    cMenubar.add("&Win/" StrMenu_ConsolSystem, nullptr, MyMenuCallback, this);
    cMenubar.add("&Win/" StrMenu_ConsolLua, "^l", MyMenuCallback, this);
    //================================

    cMenubar.add("&Debug/" StrMenu_Demo1, nullptr, MyMenuCallback, this);
    cMenubar.add("&Debug/" StrMenu_Demo2, nullptr, MyMenuCallback, this);
    //================================

		
    cuCanvas3d->cPopup = new	Fl_Menu_Button( 0, 0, 1, 1);

    
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


    PP3d::Mat4 lMatTran;
    lMatTran.Identity();

										
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
    PP3d::Kamera & lKamera= lWin3d->getKamera();

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
	    TheAppli.setDatabase( luBase ); // on prend la nlle base
	    TheAppli.redrawAll();
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
	      if( SaveBase( TheAppli.getDatabase(), MyPref.cLastSave, 0 ) == false )
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
		Fl_File_Chooser* lFc = new Fl_File_Chooser(".", "*.obj",
							   Fl_File_Chooser::CREATE,
							   "Export for d3d (.obj)");
	    	    
		lFc->callback( ExportD3dObjCB, 0 );
		lFc->show();
	      }
	  else //========================================
	    if( strcmp( m->label(),StrMenu_ExportSelectD3dObj ) == 0)
	      {	    
		Fl_File_Chooser* lFc = new Fl_File_Chooser(".", "*.obj",
							   Fl_File_Chooser::CREATE,
							   "Export for d3d (.obj)");
	    	    
		lFc->callback( ExportD3dObjCB, (void*)1 );
		lFc->show();
	      }
	    else //========================================
	      if( strcmp( m->label(), StrMenu_ImportD3dObj )== 0 )
		{												
		  Fl_File_Chooser* lFc = new Fl_File_Chooser(".", "*.obj",
							     Fl_File_Chooser::SINGLE,
							     "Import d3d file (.obj)");	      
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
		    TheSelect.removeAll();
		    TheAppli.redrawAllCanvas3d();
		    TheAppli.redrawObjectTree();
		  }
		else if( strcmp( m->label(), StrMenu_DeleteSelect) == 0)
		  {
		    cout << "Select menu :" << StrMenu_DeleteSelect << endl;
		    TheSelect.deleteAllFromDatabase( *TheAppli.getDatabase());
		    PushHistory();			    
		    TheAppli.redrawAllCanvas3d();
		    TheAppli.redrawObjectTree();
		  }
		else if( strcmp( m->label(), StrMenu_AddSelectCopyToInput) == 0)
		  {
		    cout << "Select menu :" << StrMenu_AddSelectCopyToInput << endl;
		    TheSelect.addSelectionToInput( *TheAppli.getDatabase(), false);
		    PushHistory();
		    TheAppli.redrawAllCanvas3d();
		    TheAppli.redrawObjectTree();
		  }
    //=================== UTILS ====================    
   
		else if(  strcmp( m->label(), StrMenu_DefDefaultAxe	) == 0)
		  {
		    if( TheAppli.isSelectAxis() )
		      {
			TheAppli.setCurrentAxis( (PP3d::ObjectPtr)TheSelectTransform.getSelectionVect()[0]);
			TheSelectTransform.removeAll();			
		      }
		    else
		      fl_alert( "An axis must be selectionned !");
		  }
		else if(  strcmp( m->label(), StrMenu_SetAxePoints	) == 0)
		  {
		    PP3d::SortEntityVisitor lVisit;		    
		    TheSelect.execVisitorOnEntity(lVisit);
		    size_t lSz = lVisit.cVectPoints.size();
		    if( lSz >= 2 )
		      {			
			if( TheAppli.addAxis( lVisit.cVectPoints[lSz-2], lVisit.cVectPoints[lSz-1]) ==false)
			  {
			    fl_alert( "Creation of axe failed, perhaps same coordinates ?" );
			  }
		      }
		    else {
		      fl_alert( "We must have at least two points to make an axe");
		    }		     
		  }
		else if(  strcmp( m->label(), StrMenu_SetAxeNormal	) == 0)
		  {
		    PP3d::SortEntityVisitor lVisit;		    
		    TheSelect.execVisitorOnEntity(lVisit);
		    /*
		    size_t lSz = lVisit.cVectFacets.size();
		    if( lSz >= 1 )
		      {
			PP3d::Point3d lNorm = lVisit.cVectFacets[lSz-1]->getNormal();
			for( size_t i=1; i< lSz; i++ )
			  {
			    lNorm += lVisit.cVectFacets[i]->getNormal();
			  }
			if( lSz > 0)
			  lNorm /= lSz;

			
			PP3d::Point3d lCenter;			
			for(  const PP3d::PointPtr pPoint : lVisit.cVectPoints )
			  {
			    lCenter += pPoint->get();
			  }
			lCenter /= (double)lVisit.cVectPoints.size();
		    */
		    PP3d::Point3d lNorm;
		    PP3d::Point3d lCenter;

		    if( PP3d::GetVectorFromFacets( lVisit, lNorm, lCenter ))
		      {
			lNorm += lCenter;		    
		    
			if( TheAppli.addAxis( lCenter, lNorm ) == false)
			  {
			    fl_alert( "Creation of axe failed, perhaps same coordinates ?" );
			  }			
		      }
		    else {
		      fl_alert( "We must have at least on facet selected to make an axe with the normal");
		    }		     
	    
		  }
		else if(  strcmp( m->label(), StrMenu_SetAxeInput	) == 0)
		  {
		  }
		else if(  strcmp( m->label(), StrMenu_SetPlanePoints	) == 0)
		  {
		  }
		else if(  strcmp( m->label(), StrMenu_SetPlaneLines	) == 0)
		  {
		  }
		else if(  strcmp( m->label(), StrMenu_SetPlaneFacet	) == 0)
		  {
		  }
		else if(  strcmp( m->label(), StrMenu_SetPlaneInput	) == 0)
		  {
		  }
		else if(  strcmp( m->label(), StrMenu_SetTransPoints	) == 0)
		  {
		  }
		else if(  strcmp( m->label(), StrMenu_SetTransLine	) == 0)
		  {
		  }
		else if(  strcmp( m->label(), StrMenu_PutOnGround	) == 0)
		  {		    
 		    PP3d::VisitorMinMax lVisitMinMax;
		    TheSelect.execVisitorOnlyOnObjects ( lVisitMinMax );
		    
		    PP3d::SortEntityVisitorPoint lVisitPoint;
		    TheSelect.execVisitorOnlyOnObjects( lVisitPoint );
		    for( PP3d::PointPtr lPoint : lVisitPoint.cVectPoints )
		      {
			lPoint->get() -=  lVisitMinMax.getMin().y();
		      }				
		    TheAppli.redrawAllCanvas3d();
		  }
		else if(  strcmp( m->label(), StrMenu_PutUnderGround	) == 0)
		  {		    
 		     PP3d::VisitorMinMax lVisitMinMax;
		    TheSelect.execVisitorOnlyOnObjects( lVisitMinMax );
		    
		     PP3d::SortEntityVisitorPoint lVisitPoint;
		    TheSelect.execVisitorOnlyOnObjects( lVisitPoint );
		    for( PP3d::PointPtr lPoint : lVisitPoint.cVectPoints )
		      {
			lPoint->get() -=  lVisitMinMax.getMax().y();
		      }				
		    TheAppli.redrawAllCanvas3d();
		  }
		else if(  strcmp( m->label(), StrMenu_Recenter	) == 0)
		  {		    
 		     PP3d::VisitorMinMax lVisitMinMax;
		    TheSelect.execVisitorOnlyOnObjects( lVisitMinMax );
		    
		     PP3d::SortEntityVisitorPoint lVisitPoint;
		    TheSelect.execVisitorOnlyOnObjects( lVisitPoint );
		    
		    PP3d::Point3d lCenter = lVisitMinMax.center();
		    
		    for( PP3d::PointPtr lPoint : lVisitPoint.cVectPoints )
		      {
			lPoint->get() -=  lCenter;
		      }				
		    TheAppli.redrawAllCanvas3d();
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
		else if( strcmp( m->label(), StrMenu_ViewReset ) == 0)
		  {
		    lKamera.reset(); 
		    lKamera.raz45(); 
		    lKamera.scaleTo(1.0);
		    TheAppli.redrawAllCanvas3d();
   		  }
		else if( strcmp( m->label(), StrMenu_ViewAlongX ) == 0)
		  {
		    lKamera.razX(); 
 		    TheAppli.redrawAllCanvas3d();
   		  }
		else if( strcmp( m->label(), StrMenu_ViewAlongY ) == 0)
		  {
		    lKamera.razY(); 
		    TheAppli.redrawAllCanvas3d();
   		  }
		else if( strcmp( m->label(), StrMenu_ViewAlongZ ) == 0)
		  {
		    lKamera.razZ(); 
 		    TheAppli.redrawAllCanvas3d();
  		  }
		else if( strcmp( m->label(), StrMenu_ViewAlong_X ) == 0)
		  {
		    lKamera.razXInv(); 
 		    TheAppli.redrawAllCanvas3d();
   		  }
		else if( strcmp( m->label(), StrMenu_ViewAlong_Y ) == 0)
		  {
		    lKamera.razYInv(); 
		    TheAppli.redrawAllCanvas3d();
   		  }
		else if( strcmp( m->label(), StrMenu_ViewAlong_Z ) == 0)
		  {
		    lKamera.razZInv(); 
 		    TheAppli.redrawAllCanvas3d();
  		  }
    		else if( strcmp( m->label(), StrMenu_ViewRecenterOnSelection ) == 0)
		  {
		    PP3d::VisitorMinMax lVisitMinMax;
		    TheSelect.execVisitorOnEntity ( lVisitMinMax );
		    lKamera.zoomTo( lVisitMinMax ); 
 		    TheAppli.redrawAllCanvas3d();
		  }
    //================= WINDOWS ===================
		else if( strcmp( m->label(), StrMenu_ObjectTree ) == 0)
		  {
		    TheAppli.createObjectTree( );
		  }
		else if( strcmp( m->label(), StrMenu_Create3dView ) == 0)
		  {
		    lOsLuaCode << "WinNewCanvas3d( 1000, 800 )"<< std::endl;
	
		    //	      TheAppli.createNewWin3d( 1000, 800 );
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
		    TheAppli.getDatabase()->demo1();
		    TheAppli.redrawAllCanvas3d();
		    TheAppli.redrawObjectTree();
		  }
		else if( strcmp( m->label(), 	StrMenu_Demo2 ) == 0)
		  {
		    TheAppli.getDatabase()->demo2();
		    TheAppli.redrawAllCanvas3d();
		    TheAppli.redrawObjectTree();
		  }

				 
    if( lOsLuaCode.str().size() > 0 )
      {
	// Ily a du lua a executer
	if( TheAppli.execLuaHisto(lOsLuaCode, lOsLuaOut) != nullptr)
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
 
