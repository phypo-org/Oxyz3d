// #include "config.h"


#include "Gui/Win3dBase.h"


#include "Callback.h"
#include "SelFunct.h"

#include "Gui/MyFloatInput.h"


namespace M3d {

#define StrMenu_File             "&File/"

#define StrMenu_NewBase          "New"
#define StrMenu_OpenBase         "Open ..."
#define StrMenu_SaveBase         "Save"
#define StrMenu_SaveBaseAs       "Save as ..."
#define StrMenu_SaveBaseSelect   "Save Selected ..."
#define StrMenu_MergeBase        "Merge ..."

#define StrMenu_Export           "Export/"
#define StrMenu_ExportD3dObj     "-> d3d file (.obj) ..."
#define StrMenu_ExportSelectD3dObj "Selection -> d3d file (.obj) ..."
#define StrMenu_ExportStl        "-> stl file (.stl) ..."
#define StrMenu_ExportSelectStl "Selection -> stl file (.stl) ..."
#define StrMenu_Import          "Import/"
#define StrMenu_ImportD3dObj     "d3d file (.obj) ..." // blanc au debut voulu pour <> export

#define StrMenu_Preferences     "Preferences ..."


#define StrMenu_Create3dView    "New 3d view"
#define StrMenu_ObjectTree      "Objects Tree"
#define StrMenu_ConsolPython    "Console python"
#define StrMenu_ConsolSystem    "Console system"
#ifdef USING_LUA     
#define StrMenu_ConsolLua       "Console lua"
#endif
  
#define StrMenu_Edit            "&Edit/"
#define StrMenu_Undo            "Undo  Ctrl+Z"
#define StrMenu_Redo            "Redo  Shift+Ctrl+Z"

#define StrMenu_Select          "&Select/"
#define StrMenu_SelectMore      "Select more"
#define StrMenu_SelectLess      "Select Less"
#define StrMenu_SelectEdgeLoop01  "Select edge loop01"
#define StrMenu_SelectEdgeLoop001  "Select edge loop001"
#define StrMenu_SelectEdgeLoop0001  "Select edge loop0001"

#define StrMenu_SelectAll       "Select all"
#define StrMenu_UnselectAll     "Unselect all"
#define StrMenu_InvertSelect    "Invert selection"

#define StrMenu_DeleteSelect    "Delete object selection"
 
#define StrMenu_AddSelectCopyToInput "Add selection to input (copy)"
  
#define StrMenu_SelectSimilarNormal  "Similar normal ..." // A FAIRE


  //#define StrMenu_DefDefaultPlane "Define plane as default"
  //#define StrMenu_DefDefaultTrans "Define translation as default"
  
#define StrMenu_DefDefaultAxis "Define default axis"
#define StrMenu_CreateAxis     "Create axis ..."  
#define StrMenu_SetAxisPoints   "With two selected points of object"
#define StrMenu_SetAxisLine     "With line of object"
#define StrMenu_SetAxisNormal   "With selected facet normal"
#define StrMenu_SetAxisInput    "With the last two input points"


#define StrMenu_DefInputPlane  "Define input plane"
#define StrMenu_InputPlaneX    " : X plane"
#define StrMenu_InputPlaneY    " : Y plane"
#define StrMenu_InputPlaneZ    " : Z plane"
#define StrMenu_InputPlaneFree " : Free plane " // (3 points or 2 lines needed)"
#define StrMenu_PlaneHeight    " : Plane height ..."

  
#define StrMenu_RoundInput    " Round input value to  ..."



  
  
  //#define StrMenu_SetPlanePoints "X With three points"
  //  //2#define StrMenu_SetPlaneLines  "X With two connect Lines"
  //#define StrMenu_SetPlaneFacet  "X With one facet"
  //#define StrMenu_SetPlaneInput  "X With last three input points"

  //#define StrMenu_SetTransPoints "X With two selected points of object"
  //#define StrMenu_SetTransLine   "X With selected line of object"
  //#define StrMenu_SetTransInput  "X With the last two input points"

#define StrMenu_PutOnGround    "Put selection on ground"
#define StrMenu_PutEachOnGround    "Put each element of selection on ground"
#define StrMenu_PutUnderGround "Put selection under ground"
#define StrMenu_PutEachUnderGround "Put each element of selection under ground"
#define StrMenu_Recenter       "Recenter selection"


#define StrMenu_DialogPerspectivSettings  "Perspective settings ..."

#define StrMenu_ViewReset   "Reset View"
#define StrMenu_ViewAlongX  "View along X"
#define StrMenu_ViewAlongY  "View along Y"
#define StrMenu_ViewAlongZ  "View along Z"
#define StrMenu_ViewAlong_X "View along -X"
#define StrMenu_ViewAlong_Y "View along -Y"
#define StrMenu_ViewAlong_Z "View along -Z"
#define StrMenu_ViewRecenterOnSelection "Recenter view on selection - (bug)"

#define StrMenu_Demo1            "Demo 1"
#define StrMenu_Demo2            "Demo 2"
  
  using namespace std;

  
  //-------------------------------------------
#define BEGINCALL						        \
  static bool slFlagDialog=false;                                       \
                                                                        \
  PP3d::Mat4 lMatTran;                                                  \
  lMatTran.identity();                                                  \
			                                                \
  Fl_Menu_* mw = (Fl_Menu_*)w;                                          \
  const Fl_Menu_Item* m = mw->mvalue();                                 \
  if (!m)                                                               \
    {                                                                   \
      printf("NULL\n");                                                 \
      return ;                                                          \
    }                                                                   \
  printf("%s\n", m->label());                                           \
                                                                        \
  M3d::Win3d*    lWin3d = reinterpret_cast<M3d::Win3d*>(pUserData);     \
  M3d::Canvas3d* lCanvas =lWin3d->cuCanvas3d.get();                     \
                                                                        \
  PP3d::Kamera & lKamera= lWin3d->getKamera();                          \
  Fl::focus(lCanvas);                                                 


#define LAMBDA [](Fl_Widget *w, void *pUserData) { BEGINCALL
#define ADBMAL }

  //****************************************

  Win3d::Win3d(const char*pName, int pW, int pH, PP3d::DataBase & pDatabase )
    :Win3dBase( "Oxyd3d : 3d view", pW, pH )
    ,cDatabase( pDatabase )
  {    
    //================
     cY = cMenubar.h();
    int lH = (int)(((float)cMenubar.h())*0.6f);
    int lW = 50;

 	
    lW =  Creation::sIconSize+4;
    lH =  Creation::sIconSize+4;

    int cXCanvas = cY+lH+2;
	
    // cCanvas3d = createCanvas( 0, cXCanvas, this->w()-10, (this->h()-cXCanvas)-cSzOutputInfo , "1" );
 
    cuCanvas3d = std::unique_ptr<Canvas3d>(new Canvas3d( *this, 10, 70, this->w()-10, this->h()-90, "1" ));
    //sw.mode(FL_RGB);
	
    this->resizable( cuCanvas3d.get()  );


    //========================		
    Fl_Image* lPixUndo = MyImage::LoadImage("Icons/undo.png", Creation::sIconSize);
    
    cButUndo = new MyButton( cX, cY, lW, lH, nullptr,
			     UndoCB, this, nullptr);
    cButUndo->image( lPixUndo );
    cButUndo->tooltip("Undo");
    cX += lW;
	
    Fl_Image* lPixPush = MyImage::LoadImage("Icons/store.png", Creation::sIconSize);
    
    cButStore = new MyButton( cX, cY, lW, lH, nullptr,
                              PushHistoryCB, this, nullptr);
    cButStore->image( lPixPush );
    cButStore->tooltip("Push history");
    cX += lW;
	
    Fl_Image* lPixRedo = MyImage::LoadImage("Icons/redo.png", Creation::sIconSize);

    
    cButRedo = new MyButton( cX, cY, lW, lH, nullptr,
			     RedoCB, this, nullptr);
    cButRedo->image( lPixRedo );
    cButUndo->tooltip("Redo");
    cX += lW;
	
    //      cCurrentUndo = new MyIntInput( cX, cY, lW, lH, "Pos " ); 
    //     cCurrentUndoMax = new MyIntInput( cX, cY+lH, lW, lH, "Size" );


    //========================		
    //========================		
    //========================		


    //   cX += lW;
    cX += lW;


 
   //========================		
    Fl_Image* lPixSel = MyImage::LoadImage("Icons/SelectTransformOnOff.png", Creation::sIconSize);

    MyToggleButton*
      lButSelTran = new MyToggleButton( cX, cY, lW, lH, nullptr,
                                        BasculeSelModeGeoCB, this, (void*)4 );
    lButSelTran->value(false);
    lButSelTran->image( lPixSel );
    lButSelTran->tooltip("Enable selection for transformation : axe, plane ...");
    cX += lW;
   
 
   //========================		
    lPixSel = MyImage::LoadImage("Icons/magnet.png", Creation::sIconSize);

    MyToggleButton*
      lButModeMagnet = new MyToggleButton( cX, cY, lW, lH, nullptr,
                                        []( Fl_Widget*w, void*pData){
                                            MyToggleButton* lToggle = reinterpret_cast<MyToggleButton*>( pData);
                                            Win3d* lWin3d = reinterpret_cast<Win3d*>( lToggle->cUserData1);

                                            if( lToggle->value() )
                                              {
                                                lWin3d->cuCanvas3d->setGlobalMode( GlobalMode::MAGNET );
                                                CallDialogMagnet( TheCreat.getMagnet() );
                                              }
                                            else
                                              {
                                                lWin3d->cuCanvas3d->setGlobalMode( GlobalMode::INPUT );
                                                CloseDialogMagnet();
                                              }                                           
                                          }, this );
    lButModeMagnet->value(false);
    lButModeMagnet->image( lPixSel );
    lButModeMagnet->tooltip("Enable/Disable magnt mode ...");
    cX += lW;

 
 
    //========================		
    lPixSel = MyImage::LoadImage("Icons/all.png", Creation::sIconSize);

    MyToggleButton* lButSelAll = nullptr ;
    new MyToggleButton( cX, cY, lW, lH, nullptr,
    				      BasculeSelModeCB, this, (void*)0 );
    //    lButSelAll->value(false);
    //    lButSelAll->image( lPixSel );
    //    lButSelAll->tooltip("Enable selection for all type (execpt transformation)");
 
    //========================		
    lPixSel = MyImage::LoadImage("Icons/vertex.png", Creation::sIconSize);

    MyToggleButton*
      lButSelPt = new MyToggleButton( cX, cY, lW, lH, nullptr,
                                      BasculeSelModeCB, this, (void*)0 );
    lButSelPt->value(false);
    lButSelPt->image( lPixSel );
    lButSelPt->tooltip("Enable selection for points of objects");
    cX += lW;
    //========================		
    lPixSel = MyImage::LoadImage("Icons/edge.png", Creation::sIconSize);

    MyToggleButton*
      lButSelLine = new MyToggleButton( cX, cY, lW, lH, nullptr,
                                        BasculeSelModeCB, this,   (void*)1);
    lButSelLine->value(false );
    lButSelLine->image( lPixSel );
    lButSelLine->tooltip("Enable selection for lines of objects");
    cX += lW;
    //========================		
    lPixSel = MyImage::LoadImage("Icons/face.png", Creation::sIconSize);

    MyToggleButton*
      lButFacet = new MyToggleButton( cX, cY, lW, lH, nullptr,
                                      BasculeSelModeCB, this,  (void*)2);
    lButFacet->value(false );
    lButFacet->image( lPixSel );
    lButFacet->tooltip("Enable selection for facets of objects");
    cX += lW;
    //========================		
    lPixSel = MyImage::LoadImage("Icons/body.png", Creation::sIconSize);

    MyToggleButton*
      lButSelBody = new MyToggleButton( cX, cY, lW, lH, nullptr,
                                        BasculeSelModeCB, this,  (void*)3);
    lButSelBody->value(true );
    lButSelBody->image( lPixSel );
    lButSelBody->tooltip("Enable selection for objects");
    cX += lW;

    //========================		
    lPixSel = MyImage::LoadImage("Icons/group.png", Creation::sIconSize);

    MyToggleButton*
      lButSelGroup = new MyToggleButton( cX, cY, lW, lH, nullptr,
                                        BasculeSelModeCB, this, (void*)4 );
    lButSelGroup->value(false);
    lButSelGroup->image( lPixSel );
    lButSelGroup->tooltip("Enable selection for transformation : axe, plane ...");
    cX += lW;
    

    //    lButSelAll->setUserData(  this, (void*)PP3d::SelectType::All,    lButSelAll,  lButSelPt,   lButSelLine, lButFacet,   lButSelBody, lButSelTran  );
    lButSelPt->setUserData(   this, (void*)PP3d::SelectType::Point,  lButSelPt,   lButSelLine, lButFacet,   lButSelBody,  lButSelGroup, lButSelTran, lButSelAll  );
    lButSelLine->setUserData( this, (void*)PP3d::SelectType::Line,   lButSelLine, lButSelPt,   lButFacet,   lButSelBody,  lButSelGroup, lButSelTran, lButSelAll  );
    lButFacet->setUserData(   this, (void*)PP3d::SelectType::Facet,  lButFacet,   lButSelPt,   lButSelLine, lButSelBody,  lButSelGroup, lButSelTran, lButSelAll  );
    lButSelBody->setUserData( this, (void*)PP3d::SelectType::Object, lButSelBody, lButSelPt,   lButSelLine, lButFacet,    lButSelGroup, lButSelTran,   lButSelAll  );
    lButSelGroup->setUserData( this, (void*)PP3d::SelectType::Group, lButSelGroup, lButSelPt,  lButSelLine, lButFacet,    lButSelBody,  lButSelTran,   lButSelAll  );
    //    lButSelTran->setUserData( this, (void*)PP3d::SelectType::Null,   lButSelTran, lButSelBody, lButSelPt,   lButSelLine, lButFacet,   lButSelAll   );

    //=================================================================
	
    cX += lW;

    //========================		
    lPixSel = MyImage::LoadImage("Icons/skelet.png", Creation::sIconSize);


    MyToggleButton*
      lBut1 = new MyToggleButton( cX, cY, lW, lH, nullptr,
				  BasculeViewModeCB, this,   (void*)1);
    lBut1->value( cuCanvas3d->cFlagViewGeo );
    lBut1->image( lPixSel );
    lBut1->tooltip("Wire");
    cX += lW;
    //========================		

    lBut1->setUserData( this, lBut1);

    //========================
    MyToggleButton*			lBut = nullptr;

		
    //========================

    Fl_Image* lPixDif = MyImage::LoadImage("Icons/color.png", Creation::sIconSize);
		
    lBut = new MyToggleButton( cX, cY, lW, lH, nullptr,
			       BasculeBoolCB, this, &cuCanvas3d->cFlagLightColor  );
    lBut->value( cuCanvas3d->cFlagLightColor);
    lBut->image( lPixDif );
    lBut->tooltip("Colored draw");
    cX += lW;
		
    //========================

    Fl_Image* lPixViewNormal = MyImage::LoadImage("Icons/ViewNormal.png", Creation::sIconSize);
		
    lBut = new MyToggleButton( cX, cY, lW, lH, nullptr,
			       BasculeBoolCB, this, &cuCanvas3d->cFlagViewNormal );
    lBut->value( cuCanvas3d->cFlagViewNormal);
    lBut->image( lPixViewNormal );
    lBut->tooltip("View facet normal");
    cX += lW;


    //========================

    Fl_Image* lPixViewGeo = MyImage::LoadImage("Icons/ViewTransformOnOff.png", Creation::sIconSize);
		
    lBut = new MyToggleButton( cX, cY, lW, lH, nullptr,
			       CB_ViewTransfrom, this );
    lBut->value( cuCanvas3d->getViewGeo());
    lBut->image( lPixViewGeo );
    lBut->tooltip("On/off for transformations draw");
    cX += lW;


		
    //=================================================================
    cX += lW;
    //========================
    lBut = nullptr;
    
		
    Fl_Image* lPixPersp = MyImage::LoadImage("Icons/perspective.png", Creation::sIconSize);
		
    lBut = new MyToggleButton( cX, cY, lW, lH, nullptr,
			       BasculePerspective, this );
    lBut->value( cuCanvas3d->getKamera().isPerspectiveOn() );
    lBut->image( lPixPersp );
    lBut->tooltip("Perpective draw");
    cX += lW;
    //========================
		
    Fl_Image* lPix2 = MyImage::LoadImage("Icons/grid.png", Creation::sIconSize);
		
    lBut = new MyToggleButton( cX, cY, lW, lH, nullptr,
			       BasculeGridCB, this, &cuCanvas3d->cGridMode );
    lBut->value( cuCanvas3d->cGridMode ==  ModeGrid::GRID_2D );
    lBut->image( lPix2 );
    lBut->tooltip("On/off for grid");
    cX += lW;
    //========================
    Fl_Image* lPix1 = MyImage::LoadImage("Icons/axe.png", Creation::sIconSize);

    lBut = new MyToggleButton( cX, cY, lW, lH, nullptr,
			       BasculeBoolCB, this, &cuCanvas3d->cAxisFlag );
    lBut->value( cuCanvas3d->cAxisFlag );
    lBut->image( lPix1 );
    lBut->tooltip("On/off for XYZ axis");
    cX += lW;



		
    cInfoOutput = new Fl_Output( 1, this->h() - cSzOutputInfo , this->w()-2, cSzOutputInfo, "Fl_InfoOutput");
    cInfoOutput->align(FL_ALIGN_BOTTOM);
    cInfoOutput->value("Welcome ...");

			
    //=================================================================
		

    
    //================================
    ////////////////    cMenubar.add(StrMenu_File, "",MyMenuCallback, this);
    
    cMenubar.add(StrMenu_File    StrMenu_NewBase,    "^n",  LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 if( fl_choice("Clear current work", "Yes", "No", 0)  == 0)
                   {
                     PushHistory(); // on sauve l'ancienne base dans l'historique
                     // Une Nlle base
                     std::unique_ptr<PP3d::DataBase> luBase( new PP3d::DataBase() );
                     luBase->resetIdFromMax(); // on prend en compte les id de la base lu 
                     TheCreat.setDatabase( luBase ); // on prend la nlle base
                     TheCreat.redrawAll(PP3d::Compute::FacetAll);
                   }
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL, this);


    
    cMenubar.add(StrMenu_File    StrMenu_OpenBase,    "^o", LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 Fl_File_Chooser* lFc = new Fl_File_Chooser(".", "*.oxyz",
                                                            Fl_File_Chooser::SINGLE,
                                                            "Open Oxyz3d database");	      
                 lFc->callback( OpenBaseCB );
                 lFc->show();
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL, this);
    
    cMenubar.add(StrMenu_File    StrMenu_MergeBase,  "",  LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 Fl_File_Chooser* lFc = new Fl_File_Chooser(".", "*.oxyz",
                                                            Fl_File_Chooser::SINGLE,
                                                            "Import Oxyz3d database");	      
                 lFc->callback( MergeBaseCB );
                 lFc->show();
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL,this, FL_MENU_DIVIDER);

    
    cMenubar.add(StrMenu_File    StrMenu_SaveBase,    "^s",  LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 if( MyPref.cLastSave.size() )
                   {
                     if( SaveBase( TheCreat.getDatabase(), MyPref.cLastSave, 0 ) == false )
                       {
                         fl_alert( "Saving database in <%s> failed",  MyPref.cLastSave.c_str());
                       }
                   }
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL, this);
    
    cMenubar.add(StrMenu_File    StrMenu_SaveBaseAs,  "",  LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 Fl_File_Chooser* lFc = new Fl_File_Chooser(".", "*.oxyz",
                                                            Fl_File_Chooser::CREATE,
                                                            "Save base as");	   
                 lFc->callback( SaveBaseCB, 0 );			 
                 lFc->show();
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL, this);
    
    cMenubar.add(StrMenu_File    StrMenu_SaveBaseSelect,  "",  LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 Fl_File_Chooser* lFc = new Fl_File_Chooser(".", "*.oxyz",
                                                            Fl_File_Chooser::CREATE,
                                                            "Save base as");
                 lFc->callback( SaveBaseCB, (void*)1 );			 
                 lFc->show();
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL, this, FL_MENU_DIVIDER);
    
    cMenubar.add(StrMenu_File StrMenu_Export StrMenu_ExportD3dObj,     "^e",  LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 Fl_File_Chooser* lFc = new Fl_File_Chooser(".", "*.obj",
                                                            Fl_File_Chooser::CREATE,
                                                            "Export in d3d (.obj)");
                 
                 lFc->callback( ExportD3dObjCB, 0 );
                 lFc->show();
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL, this);
    
    cMenubar.add(StrMenu_File StrMenu_Export StrMenu_ExportSelectD3dObj,"",  LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 Fl_File_Chooser* lFc = new Fl_File_Chooser(".", "*.obj",
                                                            Fl_File_Chooser::CREATE,
                                                            "Export selection in d3d (.obj)");
                 
                 lFc->callback( ExportD3dObjCB, (void*)1 );
                 lFc->show();	      
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL, this, FL_MENU_DIVIDER);
    
    cMenubar.add(StrMenu_File StrMenu_Export StrMenu_ExportStl,     "",  LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 Fl_File_Chooser* lFc = new Fl_File_Chooser(".", "*.stl",
                                                            Fl_File_Chooser::CREATE,
                                                            "Export in stl (*.stl)");
                 
                 lFc->callback( ExportStlCB, (void*)0 );
                 lFc->show();
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL, this);
    
    cMenubar.add(StrMenu_File StrMenu_Export StrMenu_ExportSelectStl,"",  LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 Fl_File_Chooser* lFc = new Fl_File_Chooser(".", "*.stl",
                                                            Fl_File_Chooser::CREATE,
                                                            "Export selection in stl (*.stl)");
                 
                 lFc->callback( ExportStlCB, (void*)1 );
                 lFc->show();
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL, this);

    
    cMenubar.add(StrMenu_File StrMenu_Import StrMenu_ImportD3dObj,  "",  LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 Fl_File_Chooser* lFc = new Fl_File_Chooser(".", "*.obj",
                                                            Fl_File_Chooser::SINGLE,
                                                            "Import d3d file (.obj)");	      
                 lFc->callback( ImportD3dObjCB );					
                 lFc->show();
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL, this);
    
    cMenubar.add(StrMenu_File    StrMenu_Preferences,  "^p",  LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 CallDialogPref();
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL, this, FL_MENU_DIVIDER);

    
    cMenubar.add(StrMenu_File "&Quit",                     "^q", LAMBDA
                 exit(0);
                 ADBMAL,   this);
									       
    //================================ 
    cMenubar.add( StrMenu_Edit   StrMenu_DeleteSelect, "",  LAMBDA
                  //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                  TheSelect.deleteAllFromDatabase( *TheCreat.getDatabase());
                  PushHistory();			    
                  TheCreat.redrawAllCanvas3d(PP3d::Compute::FacetAll);
                  TheCreat.redrawObjectTree();
                  //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                  ADBMAL, this);
    
    cMenubar.add( StrMenu_Edit   StrMenu_Undo,         "", UndoCB, this, FL_MENU_DIVIDER);
    cMenubar.add( StrMenu_Edit   StrMenu_Redo,         "", RedoCB, this, FL_MENU_DIVIDER);
    
    //================================
    
    cMenubar.add( StrMenu_Select StrMenu_SelectAll, "",  LAMBDA
                  //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                  TheSelect.selectAll( *TheCreat.getDatabase() );
		  
                  TheCreat.redrawAllCanvas3d(PP3d::Compute::FacetAll);
                  TheCreat.redrawObjectTree();
                  //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                  ADBMAL, this);
    
    cMenubar.add( StrMenu_Select StrMenu_UnselectAll, "",  LAMBDA
                  //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                  TheSelect.removeAll();		    
                  TheCreat.redrawAllCanvas3d(PP3d::Compute::FacetAll);
                  TheCreat.redrawObjectTree();
                  //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                  ADBMAL, this);
    
    cMenubar.add( StrMenu_Select StrMenu_SelectMore,   "",  LAMBDA
                  //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                  SelFunct::SelectMore( TheSelect, TheBase );
		  
                  TheCreat.redrawAllCanvas3d(PP3d::Compute::FacetAll);
                  TheCreat.redrawObjectTree();
                  //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                  ADBMAL, this );
    
    cMenubar.add( StrMenu_Select StrMenu_SelectLess,   "",  LAMBDA
                  //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                  fl_alert( "Less not implemented");

                  //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                  ADBMAL, this);
    
    cMenubar.add( StrMenu_Select StrMenu_SelectEdgeLoop01,   "",  LAMBDA
                  //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                  SelFunct::SelectEdgeLoop( TheSelect, TheBase, 0.1 );
		    
                  TheCreat.redrawAllCanvas3d(PP3d::Compute::FacetAll);
                  TheCreat.redrawObjectTree();
                  //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                  ADBMAL, this);
    
    cMenubar.add( StrMenu_Select StrMenu_SelectEdgeLoop001,   "",  LAMBDA
                  //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                  SelFunct::SelectEdgeLoop( TheSelect, TheBase, 0.01 );
		    
                  TheCreat.redrawAllCanvas3d(PP3d::Compute::FacetAll);
                  TheCreat.redrawObjectTree();
                  //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                  ADBMAL, this);
    
    cMenubar.add( StrMenu_Select StrMenu_SelectEdgeLoop0001,   "",  LAMBDA
                  //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                  SelFunct::SelectEdgeLoop( TheSelect, TheBase, 0.001 );
		  
                  TheCreat.redrawAllCanvas3d(PP3d::Compute::FacetAll);
                  TheCreat.redrawObjectTree();
                  //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                  ADBMAL, this);
    cMenubar.add( StrMenu_Select StrMenu_InvertSelect, "",  LAMBDA
                  //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                  TheSelect.invertSelection( *TheCreat.getDatabase() );
                  
                  TheCreat.redrawAllCanvas3d(PP3d::Compute::FacetAll);
                  TheCreat.redrawObjectTree();		  
                  //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                  ADBMAL, this);
    
    cMenubar.add( StrMenu_Select StrMenu_AddSelectCopyToInput, "",  LAMBDA
                  //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                  cout << "Select menu :" << StrMenu_AddSelectCopyToInput << endl;
                  TheSelect.addSelectionToInput( *TheCreat.getDatabase(), false);
                  PushHistory();
                  TheCreat.redrawAllCanvas3d(PP3d::Compute::FacetAll);
                  TheCreat.redrawObjectTree();
                  //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                  ADBMAL, this);
    //================================
   
    
    cMenubar.add("&Utils/" StrMenu_DefDefaultAxis  , "",  LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 if( TheCreat.setCurrentAxis( TheSelect.getSelectionVect()[0] ))
                   {
                     
                     TheSelect.removeAll();			
                   }
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL, this);
    //    cMenubar.add("&Utils/X StrMenu_DefDefaultPlane", "", MyMenuCallback, this);
    //    cMenubar.add("&Utils/X StrMenu_DefDefaultTrans", "", MyMenuCallback, this, FL_MENU_DIVIDER);

    cMenubar.add("&Utils/" StrMenu_CreateAxis "/" StrMenu_SetAxisPoints  , "",  LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 PP3d::SortEntityVisitorPoint lVisit;		    
                 TheSelect.execVisitorOnEntity(lVisit);
                 size_t lSz = lVisit.cVectPoints.size();
                 if( lSz >= 2 )
                   {			
                     if( TheCreat.addAxis( lVisit.cVectPoints[lSz-2], lVisit.cVectPoints[lSz-1]) ==false)
                       {
                         fl_alert( "Creation of axe failed, perhaps same coordinates ?" );
                       }
                   }
                 else
                   {
                     fl_alert( "We must have at least two points to make an axis");
                   }		     
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL, this);
    
    cMenubar.add("&Utils/" StrMenu_CreateAxis "/" StrMenu_SetAxisLine    , "",  LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 PP3d::SortEntityVisitorLine lVisit;		    
                 TheSelect.execVisitorOnEntity(lVisit);
                 size_t lSz = lVisit.cVectLines.size();
                 if( lSz >= 1 )
                   {			
                     if( TheCreat.addAxis( lVisit.cVectLines[0]->first(), lVisit.cVectLines[0]->second()) ==false)
                       {
                         fl_alert( "Creation of axe failed, perhaps same coordinates ?" );
                       }
                   }
                 else {
                   fl_alert( "We must have at least a line to make an axis");
                 }		     
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL, this);
    
    
    cMenubar.add("&Utils/" StrMenu_CreateAxis "/" StrMenu_SetAxisNormal  , "", LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 PP3d::SortEntityVisitorPointFacet lVisit;		    
                 TheSelect.execVisitorOnEntity(lVisit);
                 
                 PP3d::Point3d lNorm;
                 PP3d::Point3d lCenter;
               
                 if( PP3d::GetVectorFromFacets( lVisit, lNorm, lCenter ))
                   {
                     lNorm += lCenter;		    
                     
                     if( TheCreat.addAxis( lCenter, lNorm ) == false)
                       {
                         fl_alert( "Creation of axe failed, perhaps same coordinates ?" );
                       }			
                   }
                 else {
                   fl_alert( "We must have at least on facet selected to make an axe with the normal");
                 }               
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL, this);
    
    cMenubar.add("&Utils/" StrMenu_CreateAxis "/" StrMenu_SetAxisInput  , "", LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 PP3d::SortEntityVisitorPoint lVisit;
                 if( TheInput.execVisitorOnCurrentLine( lVisit ) && lVisit.cVectPoints.size() >= 2 )
                   {
                     PP3d::Point3d lCenter = lVisit.cVectPoints[0]->get();
                     PP3d::Point3d lNorm   = lVisit.cVectPoints[1]->get();
                     if( TheCreat.addAxis( lCenter, lNorm ) == false)
                       {
                         fl_alert( "Creation of axe failed, perhaps same coordinates ?" );
                       }					      
                   }
                 else
                   {
                     fl_alert( "We must have two distinct points");
                   }
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL, this);
  
  

	       
    cMenubar.add("&Utils/" StrMenu_PutOnGround ,    "",  LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 PP3d::VisitorMinMax lVisitMinMax;
                 TheSelect.execVisitorOnlyOnObjects ( lVisitMinMax );
		    
                 PP3d::SortEntityVisitorPoint lVisitPoint;
                 TheSelect.execVisitorOnlyOnObjects( lVisitPoint );
                 for( PP3d::PointPtr lPoint : lVisitPoint.cVectPoints )
                   {
                     lPoint->get().y() -=  lVisitMinMax.getMin().y();
                   }				
                 PushHistory();
                 TheCreat.redrawAllCanvas3d(PP3d::Compute::Nothing);
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL, this );
  
    cMenubar.add("&Utils/" StrMenu_PutUnderGround , "", LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 PP3d::VisitorMinMax lVisitMinMax;
                 TheSelect.execVisitorOnlyOnObjects( lVisitMinMax );
		    
                 PP3d::SortEntityVisitorPoint lVisitPoint;
                 TheSelect.execVisitorOnlyOnObjects( lVisitPoint );
                 for( PP3d::PointPtr lPoint : lVisitPoint.cVectPoints )
                   {
                     lPoint->get().y() -=  lVisitMinMax.getMax().y();
                   }				
                 PushHistory();
                 TheCreat.redrawAllCanvas3d(PP3d::Compute::Nothing);
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL, this );
  
    cMenubar.add("&Utils/" StrMenu_PutEachOnGround ,    "",  LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 const std::vector<PP3d::EntityPtr> & lVectSel =  TheSelect.getSelectionVect(); 

                 for( PP3d::EntityPtr lEntity : lVectSel )
                   {
                     PP3d::VisitorMinMax lVisitMinMax;
                     lEntity->execVisitor( lVisitMinMax );
		    
                     PP3d::SortEntityVisitorPoint lVisitPoint;
                     lEntity->execVisitor( lVisitPoint );
                        
                     for( PP3d::PointPtr lPoint : lVisitPoint.cVectPoints )
                       {
                         lPoint->get().y() -=  lVisitMinMax.getMin().y();
                       }				
                     TheCreat.redrawAllCanvas3d(PP3d::Compute::Nothing);
                   }
                 PushHistory();
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL, this );
    cMenubar.add("&Utils/" StrMenu_PutEachUnderGround , "",  LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 const std::vector<PP3d::EntityPtr> & lVectSel =  TheSelect.getSelectionVect(); 

                 for( PP3d::EntityPtr lEntity : lVectSel )
                   {
                     PP3d::VisitorMinMax lVisitMinMax;
                     lEntity->execVisitor( lVisitMinMax );
		    
                     PP3d::SortEntityVisitorPoint lVisitPoint;
                     lEntity->execVisitor( lVisitPoint );
                        
                     for( PP3d::PointPtr lPoint : lVisitPoint.cVectPoints )
                       {
                         lPoint->get().y() -=  lVisitMinMax.getMax().y();
                       }				
                     TheCreat.redrawAllCanvas3d(PP3d::Compute::Nothing);
                   }
                 PushHistory();
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL, this );
  
    cMenubar.add("&Utils/" StrMenu_Recenter , "",  LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 PP3d::VisitorMinMax lVisitMinMax;
                 TheSelect.execVisitorOnlyOnObjects( lVisitMinMax );
		    
                 PP3d::SortEntityVisitorPoint lVisitPoint;
                 TheSelect.execVisitorOnlyOnObjects( lVisitPoint );
		    
                 PP3d::Point3d lCenter = lVisitMinMax.center();
		    
                 for( PP3d::PointPtr lPoint : lVisitPoint.cVectPoints )
                   {
                     lPoint->get() -=  lCenter;
                   }				
                 TheCreat.redrawAllCanvas3d(PP3d::Compute::Nothing);
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL, this,   FL_MENU_DIVIDER);
  
    cMenubar.add("&Utils/" StrMenu_DefInputPlane "/" StrMenu_InputPlaneX, "",  LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 TheCreat.setInputPlane( InputPlaneType::X );
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL, this );
  
    cMenubar.add("&Utils/" StrMenu_DefInputPlane "/" StrMenu_InputPlaneY, "",  LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 TheCreat.setInputPlane( InputPlaneType::X );
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL, this );
  
    cMenubar.add("&Utils/" StrMenu_DefInputPlane "/" StrMenu_InputPlaneZ, "",  LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 TheCreat.setInputPlane( InputPlaneType::Z );
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL, this );
  
    cMenubar.add("&Utils/" StrMenu_DefInputPlane "/" StrMenu_InputPlaneFree, "",  LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 TheCreat.setInputPlane( InputPlaneType::Free );
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL, this );
  
    cMenubar.add("&Utils/" StrMenu_DefInputPlane "/" StrMenu_PlaneHeight, "",  LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 double lVal = TheCreat.getInputPlaneHeight();
                 if( CallDialogInputDouble( "Plane height", lVal ) )
                   {
			
                     std::cout << "getInputPlaneHeight Val:" <<lVal << std::endl;
			
                     TheCreat.setInputPlaneHeight( lVal );
                   }
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL, this,   FL_MENU_DIVIDER );
  
    cMenubar.add("&Utils/" StrMenu_RoundInput, "",  LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 double lVal = TheCreat.getInputPlaneHeight();
                 if( CallDialogInputDouble( "Rounding input", lVal ) )
                   {		       			
                     TheCreat.setRoundingInput( lVal );
                   }
                 PushHistory();
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL, this );
    //================================
		
    cMenubar.add("&View/" StrMenu_DialogPerspectivSettings, "^p",  LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 // Mettre un RAII
                 //		if( cFlagDialogPerspectiv )
                 //			return;
					 
                 //		cFlagDialogPerspectiv = true;
                 CallDialogPerspectiv( lCanvas,  lCanvas->getKamera() );
                 //			cFlagDialogPerspectiv = false;
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL, this, FL_MENU_DIVIDER);
    
    cMenubar.add("&View/" StrMenu_ViewReset,  "o",  LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 lKamera.reset(); 
                 lKamera.raz45(); 
                 lKamera.scaleTo(1.0);
                 TheCreat.redrawAllCanvas3d(PP3d::Compute::Nothing);
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL, this, FL_MENU_DIVIDER);
    
    cMenubar.add("&View/" StrMenu_ViewAlongX, "x",  LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 lKamera.razX(); 
                 TheCreat.redrawAllCanvas3d(PP3d::Compute::Nothing);
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL, this);
    
    cMenubar.add("&View/" StrMenu_ViewAlongY, "y",  LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 lKamera.razY(); 
                 TheCreat.redrawAllCanvas3d(PP3d::Compute::Nothing);
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL, this);
    
    cMenubar.add("&View/" StrMenu_ViewAlongZ, "z",  LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 lKamera.razZ(); 
                 TheCreat.redrawAllCanvas3d(PP3d::Compute::Nothing);
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL, this);
    
    cMenubar.add("&View/" StrMenu_ViewAlong_X, "",  LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 lKamera.razXInv(); 
                 TheCreat.redrawAllCanvas3d(PP3d::Compute::Nothing);
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL, this);
    
    cMenubar.add("&View/" StrMenu_ViewAlong_Y, "",  LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 lKamera.razYInv(); 
                 TheCreat.redrawAllCanvas3d(PP3d::Compute::Nothing);
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL, this);
    
    cMenubar.add("&View/" StrMenu_ViewAlong_Z, "", LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 lKamera.razZInv(); 
                 TheCreat.redrawAllCanvas3d(PP3d::Compute::Nothing);
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL, this);
                 
    
    cMenubar.add("&View/" StrMenu_ViewRecenterOnSelection, "",  LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 
                 lWin3d->recenterOnSelection(lKamera);
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL, this);
      
    //================================
    cMenubar.add("&Win/" StrMenu_Create3dView, "^v", LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 Creation::Instance().createNewWin3d( 1000, 800  );
                 //		    lOsLuaCode <<"WinNewCanvas3d( 1000, 800 )"<< std::endl;
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL,  this);
    
    cMenubar.add("&Win/" StrMenu_ObjectTree, "^t",  LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 TheCreat.createObjectTree( );
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL, this);
    
    //		cMenubar.add("&Win/" StrMenu_ConsolPython, nullptr, MyMenuCallback, this);
    cMenubar.add("&Win/" StrMenu_ConsolSystem, nullptr, LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 CallConsoleSystem( );
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL , this);

    
#ifdef USING_LUA    
    cMenubar.add("&Win/" StrMenu_ConsolLua, "^l",  LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 std::ostringstream lOsLuaCode;
                 std::ostringstream lOsLuaOut;

                 CallConsoleLua( );
                 
                 if( lOsLuaCode.str().size() > 0 )
                   {
                     // Ily a du lua a executer
                     if( TheCreat.execLuaHisto(lOsLuaCode, lOsLuaOut) != nullptr)
                       
                       {
                         // ERREUR
                       }
                   }
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL, this);
#endif
    //================================

    cMenubar.add("&Debug/" StrMenu_Demo1, nullptr,  LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 TheCreat.getDatabase()->demo1();
                 TheCreat.redrawAllCanvas3d(PP3d::Compute::FacetAll);
                 TheCreat.redrawObjectTree();
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL  , this);
    
    cMenubar.add("&Debug/" StrMenu_Demo2, nullptr,  LAMBDA
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 TheCreat.getDatabase()->demo2();
                 TheCreat.redrawAllCanvas3d(PP3d::Compute::FacetAll);
                 TheCreat.redrawObjectTree();
                 //::::::::::::::::::::::::::::::::::::::::::::::::::::::
                 ADBMAL, this);
    //================================

		
    cuCanvas3d->cPopup = new	Fl_Menu_Button( 0, 0, 1, 1, "Popup");

    
    end();
    show(0,nullptr);
	
    cuCanvas3d->show();
    resizable(*this);
  }
  //-------------------------------------------
  void Win3d::recenterOnSelection(PP3d::Kamera & iKamera)
  {
    PP3d::VisitorMinMax lVisitMinMax;
    TheSelect.execVisitorOnEntity ( lVisitMinMax );
    iKamera.zoomTo( lVisitMinMax ); 
    TheCreat.redrawAllCanvas3d(PP3d::Compute::Nothing);
  }  
  //-------------------------------------------

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

  //****************************************
}
 
