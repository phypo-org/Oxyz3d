#ifndef H__MySlider__H
#define H__MySlider__H

#include <FL/Fl.H>
#include <FL/Fl_Hor_Nice_Slider.H>
#include <FL/Fl_Float_Input.H>


//******************************
class MySlider{
  
 protected:

  Fl_Hor_Nice_Slider* cSlider;
  Fl_Float_Input    * cInput;
  
  Fl_Callback*        cCallback;
  void*               cUserData;
  
 public:
  MySlider( int pX, int pY, int pW, int pH, const char* pLabel,  Fl_Callback* pCB, void* pUserData, double pMin, double pMax );

  void  value( float pVal );
  float value();

 protected:
  static void sSliderCB( Fl_Widget*, void*pUserData );
  static void sInputCB(  Fl_Widget*, void*pUserData );
};
	      
//******************************

#endif
