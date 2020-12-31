#include <math.h>

#include "MySlider.h"

#include "Application.h"

#include "Shape/Object.h"
#include "Shape/Maker.h"


//Tout mettre dans le Dialogue

namespace M3d {

  //************************

  class DialogKeepFloat {
    
    Fl_Double_Window* myWindow;
    Canvas3d *cMyCanvas;
  public:
    
    std::unique_ptr<MySlider> cSliderVal;
    
    
  public:
    DialogKeepFloat( Canvas3d* pCanvas );
    //    bool cContinue;
    static DialogKeepFloat* sTheDialogKeepFloat;
   
 
  protected:
    static void CancelCB( Fl_Widget*, void* lUserData );
    static void OkCB( Fl_Widget*, void* lUserData);
    
    static void SliderCB( Fl_Widget*, void*pUserData );
    void maj();
  };  
  //************************
  DialogKeepFloat* DialogKeepFloat::sTheDialogKeepFloat=nullptr;
  //----------------------------------------
  void DialogKeepFloat::maj()
  {   
    std::cout << " Slider " << cSliderVal->value() << std::endl;
    
    double  lSz = cSliderVal->value();
    
    if( lSz < 0.0000001 )
      lSz =  0.0000001;
    
    if( lSz > 1E6 )
      lSz= 1E6;
    	
  }

  //----------------------------------------
  DialogKeepFloat::DialogKeepFloat( Canvas3d* pCanvas)
    :cMyCanvas( pCanvas )
  {    
    int lX = 20;
    int lY= 30;
    int lW = 300;
    int lH = 20;
    int lYStep = 40;


    myWindow = new Fl_Double_Window(500, 511, "Value");
    myWindow->callback((Fl_Callback*)CancelCB, this);


    cSliderVal =  std::unique_ptr<MySlider>(new MySlider(lX+5, lY, lW, lH, "Val", SliderCB, this, 1, 360 ));


    lY += lYStep;
			

    { Fl_Button* o = new Fl_Button(210, lY, 75, 25, "Cancel");
      o->callback((Fl_Callback*)CancelCB, this );
    } // Fl_Button* o
    { Fl_Button* o = new Fl_Button(125, lY, 75, 25, "OK");
      o->callback((Fl_Callback*)OkCB, this );
    } // Fl_Button* o
	 

    myWindow->end();
		
    myWindow->show( 0, nullptr);
    maj();
    ///////    while (Fl::wait() && cContinue );
  }  
  //----------------------------------------

  void DialogKeepFloat::CancelCB( Fl_Widget*, void* pUserData ) {
 
    DialogKeepFloat* lDialog = reinterpret_cast<DialogKeepFloat*>(pUserData);
    Application::Instance().getDatabase()->cancelCurrentCreation();

    Application::Instance().redrawAllCanvas(PP3d::Compute::FacetAll);

    Fl::delete_widget( lDialog->myWindow );
  }
  //----------------------------------------
  void DialogKeepFloat::OkCB( Fl_Widget*, void*pUserData )
  {  
    DialogKeepFloat* lDialog = reinterpret_cast<DialogKeepFloat*>(pUserData);
    Application::Instance().redrawAllCanvas(PP3d::Compute::FacetAll);

    Fl::delete_widget(lDialog->myWindow);
  }
  //----------------------------------------
  void DialogKeepFloat::SliderCB( Fl_Widget*, void*pUserData )
  {    
    DialogKeepFloat * lDialog = reinterpret_cast<DialogKeepFloat*>(pUserData);
    std::cout << "DialogKeepFloat ::SliderCB " << lDialog->cSliderVal->value() << std::endl;
    lDialog->maj();
  }
} //fin namesapce
//************************
extern void CallDialogKeepFloatInit( M3d::Canvas3d* pCanvas )
{ 
  if( M3d::DialogKeepFloat::sTheDialogKeepFloat == nullptr )
    {  
      M3d::DialogKeepFloat::sTheDialogKeepFloat = new M3d::DialogKeepFloat( pCanvas);
    }
}
//************************
extern bool CallDialogKeepFloat( double & ioVal)
{
  if( M3d::DialogKeepFloat::sTheDialogKeepFloat != nullptr )
    {
      M3d::DialogKeepFloat::sTheDialogKeepFloat->cSliderVal->value((float) ioVal );
      return true;
    }
  return false;
}
