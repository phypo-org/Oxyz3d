#ifndef H__MySlider__H
#define H__MySlider__H

#include <FL/Fl.H>
#include <FL/Fl_Hor_Nice_Slider.H>
#include <FL/Fl_Float_Input.H>
#include <FL/Fl_Int_Input.H>


//******************************
class MySliderFloat{
  
 protected:

  Fl_Hor_Nice_Slider* cSlider=nullptr;
  Fl_Float_Input    * cInput=nullptr;
  
  Fl_Callback*        cCallback=nullptr;
  void*               cUserData=nullptr;
  bool                cHaveSlider=false;
  
 public:
  MySliderFloat( int pX, int pY, int pW, int pH, const char* pLabel,  Fl_Callback* pCB, void* pUserData, double pMin, double pMax,  bool pHaveSlider=true );

  void  value( float pVal );
  float value();

 protected:
  static void sSliderCB( Fl_Widget*, void*pUserData );
  static void sInputCB(  Fl_Widget*, void*pUserData );
};
using MySlider =  MySliderFloat;
//******************************
class MySliderInt{
  
 protected:

  Fl_Hor_Nice_Slider* cSlider=nullptr;
  Fl_Int_Input      * cInput=nullptr;

  
  Fl_Callback*        cCallback=nullptr;
  void*               cUserData=nullptr;
  bool                cHaveSlider=false;
  
 public:
  MySliderInt( int pX, int pY, int pW, int pH, const char* pLabel,  Fl_Callback* pCB, void* pUserData, long pMin, long pMax,  bool pHaveSlider=true );

  void  value( long pVal );
  long value();

 protected:
  static void sSliderCB( Fl_Widget*, void*pUserData );
  static void sInputCB(  Fl_Widget*, void*pUserData );
};
	      
//******************************

#endif
