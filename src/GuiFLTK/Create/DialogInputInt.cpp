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

#include "GuiFLTK/MySlider.h"

#include "Creation.h"

#include "Shape/Object.h"
#include "Shape/Maker.h"

#include "Utils/PPSingletonCrtp.h"

//Tout mettre dans le Dialogue

namespace M3d {

  
#define MyDiag  M3d::DialogInputInt::Instance()

  //************************
  class DialogInputInt  : public virtual PPSingletonCrtp<DialogInputInt>{

    Fl_Double_Window*         cMyWindow = nullptr;

    std::unique_ptr<MySlider> cSliderPas;
	
  public:
    long     cVal=0;
    bool     cRetVal=false;

    //----------------------------------------
    static void CancelCB( Fl_Widget*, void* pUserData )
    {
      Fl::delete_widget( MyDiag.cMyWindow );
      MyDiag.cMyWindow = nullptr;
      MyDiag.cRetVal = false;
    }
    //----------------------------------------
    static void OkCB( Fl_Widget*, void*pUserData )
    {
      MyDiag.cVal = (long)MyDiag.cSliderPas->value();
      
      MyDiag.cRetVal = true;
      
      Fl::delete_widget( MyDiag.cMyWindow);
      MyDiag.cMyWindow = nullptr;
    }
    //----------------------------------------
    bool isAlreadyRunning() { return cMyWindow != nullptr; }
    //----------------------------------------
    DialogInputInt()
    {
    }
    //----------------------------------------
    void init( const char* iLabel, long iVal  )
    {
      int lX = 20;
      int lY= 30;
      int lW = 400;
      int lH = 20;
      int lYStep = 40;

      cMyWindow = new Fl_Double_Window(lW+lX*3, 100, iLabel);
      cMyWindow->set_modal();
      cMyWindow->callback((Fl_Callback*)CancelCB, this);
      cMyWindow->position( 100, 100) ;
   
      cSliderPas =  std::unique_ptr<MySlider>(new MySlider(lX+5, lY, lW, lH, "value", SliderCB, nullptr, 2, 100 ));
      cSliderPas->value( iVal );
    
      lY += lYStep;
      lY += lYStep;
	 

      { Fl_Button* o = new Fl_Button(10, lY, 75, 25, "Cancel");
	o->callback((Fl_Callback*)CancelCB);
      } // Fl_Button* o
      { Fl_Button* o = new Fl_Button(125, lY, 75, 25, "OK");
	o->callback((Fl_Callback*)OkCB);
      } // Fl_Button* o
	 
      lY += lYStep;

      cMyWindow->size(  lW+lX*3, lY );
      cMyWindow->end();
		
      cMyWindow->show( 0, nullptr);
       while( MyDiag.cMyWindow != nullptr );
   }
    //----------------------------------------
    static  void SliderCB( Fl_Widget* iW, void*)
    {
    //   std::cout << "DialogInputInt::SliderCB "<< std::endl;
    }
  };
  //************************
}
  //************************
extern bool CallDialogInputInt( const char* iLabel, int & ioVal)
  {
    if( MyDiag.isAlreadyRunning() == false )
      {
	MyDiag.init( iLabel, ioVal);
	
	ioVal = MyDiag.cVal;
	return MyDiag.cRetVal;
      }
    return false;
  }
  //************************



