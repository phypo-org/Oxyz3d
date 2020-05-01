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

    MyIntInput* cDbgEvt ;
    MyIntInput* cDbgAct ;
    MyIntInput* cDbgDrw;
    MyIntInput* cDbgBaz;
    MyIntInput* cDbgSel;
    MyIntInput* cDbgLua;


    
    //----------------------------------------
    static void CancelCB( Fl_Widget*, void* pUserData )
    {
      Fl::delete_widget( DiagPref.cMyWindow);
      DiagPref.cMyWindow = nullptr;
    }
    //----------------------------------------
#undef  INT_VALUE
#define INT_VALUE(A) MyPref.A = atoi( DiagPref.A->value())
    
#undef  BOOL_VALUE
#define BOOL_VALUE(A)  MyPref.A = DiagPref.A->value() == 1;
    
    static void OkCB( Fl_Widget*, void*pUserData )
    {  
      Application::Instance().redrawAllCanvas3d();

      std::cout << " DiagPref.cSelectPassOverLighting=" << (int) DiagPref.cSelectPassOverLighting->value()
		<< std::endl;
      MyPref.cSelectPassOverLighting = DiagPref.cSelectPassOverLighting->value() == 1;

      
      //Debug
      INT_VALUE(cDbgEvt);
      INT_VALUE(cDbgAct);
      INT_VALUE(cDbgDrw);
      INT_VALUE(cDbgBaz);
      INT_VALUE(cDbgSel);
      INT_VALUE(cDbgLua);

	

     
      Fl::delete_widget( DiagPref.cMyWindow);
      DiagPref.cMyWindow = nullptr;
    }
    //----------------------------------------
   
  public:
    //----------------------------------------
    bool isAlreadyRunning() { return DiagPref.cMyWindow != nullptr; }
  
#define INTPUT_INT(A,B) { A = new MyIntInput( lX, lY, 200, lH, B, MyPref.A ); lY += A->h() + lMarge; ; A->value( std::to_string(MyPref.A).c_str()); }

#define INTPUT_BOOL(A,B) { A =  new Fl_Light_Button( lX, lY, 200, lH, B ); lY += A->h() + lMarge; A->value( MyPref.A ); }
  //----------------------------------------
    void init()
    {
      int lMarge = 10;
      int lWgroup = 600;
      int lHgroup = 400;
      int lH = 30;
      int lW = 200;
	
      int lX = lMarge;
      int lY = lMarge;
      
      cMyWindow = new Fl_Double_Window(lWgroup + lMarge*4+100, lHgroup+lMarge*4+ 100, "Preferences");
      cMyWindow->callback((Fl_Callback*)CancelCB, nullptr);

      {
	Fl_Tabs* lTabs = new Fl_Tabs(lMarge, lMarge, lWgroup+lMarge+50, lHgroup+lMarge+50);
	
	//===================================
	
	{ Fl_Group* lGr = new Fl_Group( lMarge, lMarge, lWgroup, lHgroup, "File");
	  lX = lMarge;
	  lY = lMarge;
	  lGr->end();
	  Fl_Group::current()->resizable(lGr);
	}
	//===================================
	{ Fl_Group* lGr = new Fl_Group( lMarge, lMarge, lWgroup, lHgroup, "Selection");
	  lX = lMarge;
	  lY = lMarge;
	  
	  INTPUT_BOOL( cSelectPassOverLighting, "Inlight entity when pass over");
	  cSelectPassOverLighting->value( MyPref.cSelectPassOverLighting);

	  
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
	  lX = lMarge+100;
	  lY = lMarge;
	
	  cDbgEvt = new MyIntInput( lX, lY, 200, lH, "Debug Event", MyPref.cDbgEvt );
	  lY += cDbgEvt->h() + lMarge;
	  
	  INTPUT_INT( cDbgAct, "Debug Action" );
	  INTPUT_INT( cDbgDrw, "Debug Draw" );
	  INTPUT_INT( cDbgBaz, "Debug Base" );
	  INTPUT_INT( cDbgSel, "Debug Sel" );	  
	  INTPUT_INT( cDbgLua, "Debug Lua" );
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
      
     
      
      cMyWindow->end();		
      cMyWindow->show( 0, nullptr);
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
