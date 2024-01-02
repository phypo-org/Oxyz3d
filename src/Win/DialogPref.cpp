#include "Dialogs.h"

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Float_Input.H>
#include <FL/Fl_Hor_Value_Slider.H>
#include <FL/Fl_Hor_Nice_Slider.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_File_Input.H>

#include <sstream>
#include <stdlib.h>
#include <fstream>

#include <stdlib.h>
#include <math.h>

#include "Shape/Object.h"
#include "Shape/Maker.h"
#include "Shape/SubDiv.h"
#include "Shape/ExportObj.h"

#include "Utils/PPSingletonCrtp.h"

#include "MySlider.h"
#include "Application.h"
#include "Preference.h"
#include "Shape/DebugVars.h"



#define DiagPref M3d::DialogPref::Instance()

namespace M3d {

  //************************
  class DialogPref  : public virtual PPSingletonCrtp<DialogPref>{

    Fl_Double_Window* cMyWindow = nullptr;
  
    DialogPref() {;}
  
    friend class PPSingletonCrtp;

    Fl_Light_Button* cSelectPassOverLighting;
    MyIntInput     * cSelectPickingSize;
    MyIntInput     * cBSplineMaille;

    MyIntInput     * cHistoMaxDept;


    Fl_Light_Button * cFileAutoSave;
    MyIntInput      * cFileAutoSaveFrequency;
    Fl_File_Input   * cFileDefaultDir;
 



    MyIntInput* cDbgEvt ;
    MyIntInput* cDbgAct ;
    MyIntInput* cDbgDrw;
    MyIntInput* cDbgBaz;
    MyIntInput* cDbgSel;
#ifdef USING_LUA    
    MyIntInput* cDbgLua;
#endif    
    MyIntInput* cDbgTree;
    MyIntInput* cDbgFil;
    MyIntInput* cDbgIni;

    
    //----------------------------------------
    static void CancelCB( Fl_Widget*, void* pUserData )
    {
      Fl::delete_widget( DiagPref.cMyWindow);
      DiagPref.cMyWindow = nullptr;
    }
     //----------------------------------------
    static void DefaultCB( Fl_Widget*, void*pUserData )
    {
      Preference::Instance().resetToDefault();
    }
     //----------------------------------------
    static void ReloadCB( Fl_Widget*, void*pUserData )
    {
      Preference::Instance().resetToFile();
    }
    
    //----------------------------------------
#undef  GET_INT_VALUE
#define GET_INT_VALUE(A) MyPref.A = atoi( DiagPref.A->value())
    
#undef  GET_BOOL_VALUE
#define GET_BOOL_VALUE(A)  MyPref.A = DiagPref.A->value() == 1;
#define GET_FILE_VALUE(A)  MyPref.A = DiagPref.A->value();
  
 static void OkCB( Fl_Widget*, void*pUserData )
    {  
      Application::Instance().redrawAll(PP3d::Compute::FacetAll);

      std::cout << " DiagPref.cSelectPassOverLighting=" << (int) DiagPref.cSelectPassOverLighting->value()
		<< std::endl;
      GET_BOOL_VALUE( cSelectPassOverLighting);
      // MyPref.cSelectPassOverLighting = DiagPref.cSelectPassOverLighting->value() == 1;
      
      GET_INT_VALUE( cSelectPickingSize );
      GET_INT_VALUE( cBSplineMaille );
      size_t lMaxHisto = atoi( DiagPref.cHistoMaxDept->value());
      PP3d::UndoHistory::Instance().setMaxHisto( lMaxHisto );
	
      GET_BOOL_VALUE( cFileAutoSave);
      GET_INT_VALUE(  cFileAutoSaveFrequency );
      GET_FILE_VALUE( cFileDefaultDir );

      
      //Debug
      GET_INT_VALUE(cDbgEvt);
      GET_INT_VALUE(cDbgAct);
      GET_INT_VALUE(cDbgDrw);
      GET_INT_VALUE(cDbgBaz);
      GET_INT_VALUE(cDbgSel);
#ifdef USING_LUA    
      GET_INT_VALUE(cDbgLua);
#endif
      GET_INT_VALUE(cDbgTree);
      GET_INT_VALUE(cDbgFil);
      GET_INT_VALUE(cDbgIni);

	     
      Fl::delete_widget( DiagPref.cMyWindow);
      DiagPref.cMyWindow = nullptr;
    }
    //----------------------------------------
   
  public:
    //----------------------------------------
    bool isAlreadyRunning() { return DiagPref.cMyWindow != nullptr; }

    
#define INTPUT_INT(A,B) { A = new MyIntInput( lX, lY, 50, lH, B, MyPref.A ); lY += A->h() + lMarge; ; A->value( std::to_string(MyPref.A).c_str()); A->align(Fl_Align(  FL_ALIGN_LEFT) ); }

#define INTPUT_FILE(A,B) { A = new MyFileInput( lX, lY, 50, lH, B, MyPref.A ); lY += A->h() + lMarge; ; A->value( MyPref.A.c_str()); A->align(Fl_Align(  FL_ALIGN_LEFT) ); }
    
#define INTPUT_VAR_INT(A,V,B) { A = new MyIntInput( lX, lY, 50, lH, B, V ); lY += A->h() + lMarge; ; A->value( std::to_string(V).c_str()); A->align(Fl_Align(  FL_ALIGN_LEFT) ); }

#define INTPUT_BOOL(A,B) { A =  new Fl_Light_Button( lX0, lY, 250, lH, B ); lY += A->h() + lMarge; A->value( MyPref.A ); }
  //----------------------------------------
    void init()
    {
      int lMarge = 10;
      int lWgroup = 600;
      int lHgroup = 400;  
      int lH = 30;
      int lW = 200;
	
      int lX0 = lMarge;
      int lX  = lMarge; 
      int lY  = lMarge;
      
      cMyWindow = new Fl_Double_Window(lWgroup + lMarge*4+100, lHgroup+lMarge*4+ 100, "Preferences");
      cMyWindow->set_modal();
      cMyWindow->callback((Fl_Callback*)CancelCB, nullptr);

      {
	Fl_Tabs* lTabs = new Fl_Tabs(lMarge, lMarge, lWgroup+lMarge+50, lHgroup+lMarge+50);
	
	//===================================
	
	{ Fl_Group* lGr = new Fl_Group( lMarge, lMarge, lWgroup, lHgroup, "File");
	  lX = lMarge+300;;
	  lY = lMarge;
	  INTPUT_BOOL( cFileAutoSave,          "Auto save");
	  INTPUT_INT(  cFileAutoSaveFrequency, "Auto save frequency");
	  INTPUT_FILE( cFileDefaultDir,        "Default base path" );
	  
	  lGr->end();
	  Fl_Group::current()->resizable(lGr);
	}
	//===================================
	{ Fl_Group* lGr = new Fl_Group( lMarge, lMarge, lWgroup, lHgroup, "Selection");
	  lX  = lMarge+300;
	  lX0 = lMarge;
	  lY = lMarge;


	  INTPUT_BOOL( cSelectPassOverLighting, "Inlight entity when pass over");
	  //	  cSelectPassOverLighting->value( MyPref.cSelectPassOverLighting);
      
	  INTPUT_INT( cSelectPickingSize, "Precision of picking");
 
	  INTPUT_VAR_INT( cHistoMaxDept, PP3d::UndoHistory::Instance().getMaxHisto(), "Undo level");
	  
	  lGr->end();
	  lGr->hide();
	  Fl_Group::current()->resizable(lGr);
	}
	//===================================
	{ Fl_Group* lGr = new Fl_Group( lMarge, lMarge, lWgroup, lHgroup, "Spline");
	  lX  = lMarge+300;
	  lX0 = lMarge;
	  lY = lMarge;


      
	  INTPUT_INT( cBSplineMaille, "Maillage des BSpline");
 
	  lGr->end();
	  lGr->hide();
	  Fl_Group::current()->resizable(lGr);
	}
	//===================================
	{ Fl_Group* lGr = new Fl_Group( lMarge, lMarge, lWgroup, lHgroup, "Misc");
	  lX  = lMarge+300;
	  lX0 = lMarge;
	  lY = lMarge;

	  
	  lGr->end();
	  lGr->hide();
	  Fl_Group::current()->resizable(lGr);
	}
	//===================================
	
	{ Fl_Group* lGr = new Fl_Group( lMarge, lMarge, lWgroup, lHgroup, "View");
	  lX = lMarge;
	  lY = lMarge;
	  lGr->end();
	  lGr->hide();
	  Fl_Group::current()->resizable(lGr);
	}
	//===================================	
	{ Fl_Group* lGr = new Fl_Group( lMarge, lMarge, lWgroup, lHgroup, "Debug");
	  lX = lMarge+300;
	  lY = lMarge;
	
	  //	  cDbgEvt = new MyIntInput( lX, lY, 200, lH, "Debug Event", MyPref.cDbgEvt );
	  //	  lY += cDbgEvt->h() + lMarge;


	  INTPUT_INT( cDbgEvt, "Debug Event (windows events)    " );	  
	  INTPUT_INT( cDbgAct, "Debug Action                    " );
	  INTPUT_INT( cDbgDrw, "Debug Draw                      " );
	  INTPUT_INT( cDbgBaz, "Debug Database                  " );
	  INTPUT_INT( cDbgSel, "Debug Selection                 " );	  
	  INTPUT_INT( cDbgTree,"Debug Tree                      " );
	  INTPUT_INT( cDbgFil, "Debug File (read write ...)     " );
	  INTPUT_INT( cDbgIni, "Debug configuration (config.ini)" );
#ifdef USING_LUA    
	  INTPUT_INT( cDbgLua, "Debug Lua                       " );
#endif	  
	  	  
	  /*

	  
	  cDbgBaz = new MyIntInput( lX, lY, 200, lH, "Debug Base", MyPref.cDbgBaz );
	  Y += lSaisie->h() + lMarge;
	  
	  cDbgSel = new MyIntInput( lX, lY, 200, lH, "Debug Select", MyPref.cDbgSel);
	  lY += lSaisie->h() + lMarge;
	  */
	  lGr->end();
	  lGr->hide();
	  Fl_Group::current()->resizable(lGr);
	}
	//===================================
	lTabs->end();
	Fl_Group::current()->resizable(lTabs);
      }
      lY = lHgroup+lMarge*8;      
      Create<Fl_Button>( lMarge, lY, 75, 25, "OK", (Fl_Callback*)OkCB);     
      
      Create<Fl_Button>( lMarge+200, lY, 75, 25, "Cancel", (Fl_Callback*)CancelCB );
      
      Create<Fl_Button>( lMarge+400, lY, 75, 25, "Default", (Fl_Callback*)DefaultCB);     
      
      Create<Fl_Button>( lMarge+600, lY, 100, 25, "Reload file", (Fl_Callback*)ReloadCB );
      
      
      cMyWindow->end();		
      cMyWindow->show( 0, nullptr);
      
      while( Fl::wait() && cMyWindow != nullptr );
   }
  };
  //************************
} // namespace


//---------------------------------
extern void CallDialogPref()
{
  std::cout << "CallDialogPref" <<std::endl;
  if( DiagPref.isAlreadyRunning() == false )
    {
      DiagPref.init();
    }
}
