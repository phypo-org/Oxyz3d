#include "MySlider.h"

#include <sstream>
#include <stdlib.h>

//******************************
MySlider::MySlider( int pX, int pY, int pW, int pH, const char* pLabel,  Fl_Callback pCB, void* pUserData, double lMin, double lMax )
  :cCallback(pCB)
  ,cUserData(pUserData)
{ 
  cInput = new Fl_Float_Input(pX, pY, pW/4, pH, pLabel);
  cInput->align(Fl_Align(FL_ALIGN_TOP_LEFT));
  
  cInput->callback((Fl_Callback*)sInputCB, this);
  
		cSlider = new Fl_Hor_Nice_Slider( pX+pW/4, pY, (pW/4)*3, pH, "");
		cSlider->align(Fl_Align(FL_ALIGN_TOP_LEFT));
		cSlider->bounds(lMin, lMax );
	
    cSlider->callback((Fl_Callback*)sSliderCB, this);
}
//-------------------------------------------------
void MySlider::sSliderCB( Fl_Widget*pWidget, void*pUserData )
{
  MySlider* lSlider= reinterpret_cast<MySlider*>(pUserData);
  std::stringstream lOs;
  lOs <<  lSlider->value();
  lSlider->cInput->value(  lOs.str().c_str() );
  (*lSlider->cCallback)(pWidget, lSlider->cUserData);  
}
//-------------------------------------------------
void MySlider::sInputCB( Fl_Widget*pWidget, void*pUserData )
{
  MySlider* lSlider= reinterpret_cast<MySlider*>(pUserData);
  lSlider->cSlider->value(  atof(lSlider->cInput->value()) );
  (*lSlider->cCallback)(pWidget, lSlider->cUserData);
}
//-------------------------------------------------
  void MySlider::value( float pVal )
  {
    cSlider->value( pVal );
    std::stringstream lOs;
    lOs <<  cSlider->value();
    cInput->value(  lOs.str().c_str() );
  }
//-------------------------------------------------
  float MySlider::value()
  {
    return  cSlider->value();
  }

//******************************
