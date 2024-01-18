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
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Hor_Value_Slider.H>
#include <FL/Fl_Hor_Nice_Slider.H>


#include <sstream>
#include <stdlib.h>

#include <stdlib.h>
#include <math.h>

#include "Gui/MySlider.h"

#include "Creation.h"

#include "Shape/Object.h"
#include "Shape/Maker.h"

#include "Utils/PPSingletonCrtp.h"

#include <functional>

class DialogInputDouble;
 
using DialogInputDoubleFuncExec = std::function<bool ( double , bool  )>;


//Tout mettre dans le Dialogue

namespace M3d {
  
#define MyDiag  M3d::DialogInputDouble::Instance()

  //************************
  class DialogInputDouble  : public virtual PPSingletonCrtp<DialogInputDouble>{

    Canvas3d *cMyCanvas;
    Fl_Double_Window* cMyWindow = nullptr;
    bool cMustRun = false;

    std::unique_ptr<MySlider> cSliderPas;

  public:
    double  cVal=0;
    double  cInitVal=0;
    
    bool    cRetVal = false;
    //----------------------------------------
    void stopMe(){
      cMustRun = false;
    }
    //----------------------------------------
    static void DeleteMe()
    {
      Fl::delete_widget( MyDiag.cMyWindow);
      MyDiag.cMyWindow = nullptr;
      MyDiag.cMustRun = false;
   }    
    //----------------------------------------
    static void CancelCB( Fl_Widget*, void* pUserData )
    {
      MyDiag.cRetVal = false;
      MyDiag.cMustRun = false;
   }
    //----------------------------------------
    static void OkCB( Fl_Widget*, void*pUserData )
    {
      MyDiag.cVal = MyDiag.cSliderPas->value();
      MyDiag.cRetVal = true;   
      MyDiag.cMustRun = false;
     }
    //----------------------------------------
    bool isAlreadyRunning() { return cMyWindow != nullptr; }
    //----------------------------------------
    DialogInputDouble() {;}
     //----------------------------------------

    
     //----------------------------------------
    void init (  const char* iLabel, double iVal, DialogInputDoubleFuncExec iFunc=nullptr )   
    {
      cInitVal = iVal;
      
      int lX = 20;
      int lY = 30;
      int lW = 400;
      int lH = 20;
      int lYStep = 40;
   
      cMyWindow = new Fl_Double_Window(lW+lX*3, 100, iLabel);
      cMyWindow->set_modal();
      cMyWindow->callback((Fl_Callback*)CancelCB, this);
      cMyWindow->position( 100, 100) ;

      cSliderPas =  std::unique_ptr<MySlider>(new MySlider(lX+5, lY, lW, lH, "value", SliderCB, this, -100, 100, true, false ));
      
      cSliderPas->value( iVal );
    
      lY += lYStep;
	 

      { Fl_Button* o = new Fl_Button(lX, lY, 75, 25, "Cancel");
	o->callback((Fl_Callback*)CancelCB);
      } // Fl_Button* o
      
      { Fl_Button* o = new Fl_Button(lX+90, lY, 75, 25, "OK");
	o->callback((Fl_Callback*)OkCB);
      } // Fl_Button* o
	 
      lY += lYStep;
      lY += lYStep;

      cMyWindow->size(  lW+lX*3, lY );
      cMyWindow->end();
		
      cMyWindow->show( 0, nullptr);
      
      cMustRun = true;
      //===================================
      while( Fl::wait() && cMustRun )
        {          
          //    std::cout << cMustRun << " " << cVal << std::flush ;
          if( iFunc) {
            //  std::cout << " f" << std::flush ;
            cVal = MyDiag.cSliderPas->value();           
            iFunc( cVal-cInitVal, cMustRun );
            cInitVal = cVal;
          } 
        }
      //===================================
      std::cout << "Finish1 " << std::endl;
      
      if( iFunc) iFunc(  cVal-cInitVal, cMustRun );
    }
    //----------------------------------------
    static  void SliderCB( Fl_Widget*, void*pUserData )
    {
      //      std::cout << "DialogInputDouble::SliderCB " << std::endl;
    }  
  };
  //************************
}

  //************************
extern bool CallDialogInputDouble( const char* iLabel, double & ioVal, DialogInputDoubleFuncExec iFunc )
  {
    if( MyDiag.isAlreadyRunning() == false )
      {
	MyDiag.init( iLabel, ioVal, iFunc);
	MyDiag.DeleteMe();
          
        std::cout << "Finish2 " << std::endl;

	ioVal = MyDiag.cVal;
 	//	std::cout << "CallDialogInputDouble return ioVal:" << ioVal
	//		  << " Ret:" <<   MyDiag.cRetVal << std::endl;
	
	return MyDiag.cRetVal;
      }
    return false;
  }
  //************************



