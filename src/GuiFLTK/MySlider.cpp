#include "MySlider.h"

#include <sstream>
#include <stdlib.h>

//******************************
MySliderFloat::MySliderFloat( int pX, int pY, int pW, int pH, const char* pLabel,  Fl_Callback pCB, void* pUserData, double pMin, double pMax, bool pHaveSlider )
  :cSlider(nullptr)
  ,cCallback(pCB)
  ,cUserData(pUserData)
  ,cHaveSlider( pHaveSlider )
{ 
  cInput = new Fl_Float_Input(pX, pY, pW/4, pH, pLabel);
  cInput->align(Fl_Align(FL_ALIGN_TOP_LEFT));
  
  if( cHaveSlider )
    {
      cInput->callback((Fl_Callback*)sInputCB, this);
   
        cSlider = new Fl_Hor_Nice_Slider( pX+pW/4, pY, (pW/4)*3, pH, "");
 
  
      cSlider->align(Fl_Align(FL_ALIGN_TOP_LEFT));
            
      cSlider->bounds(pMin, pMax );
      
      cSlider->callback((Fl_Callback*)sSliderCB, this);
    }
  value( pMin );
}
//-------------------------------------------------
void MySliderFloat::sSliderCB( Fl_Widget*pWidget, void*pUserData )
{
  MySlider* lSlider= reinterpret_cast<MySlider*>(pUserData);
  std::stringstream lOs;
  lOs <<  lSlider->value();
  lSlider->cInput->value(  lOs.str().c_str() );
  (*lSlider->cCallback)(pWidget, lSlider->cUserData);  
}
//-------------------------------------------------
void MySliderFloat::sInputCB( Fl_Widget*pWidget, void*pUserData )
{
  MySlider* lSlider= reinterpret_cast<MySlider*>(pUserData);
  if( lSlider->cSlider != nullptr )
    lSlider->cSlider->value(  atof(lSlider->cInput->value()) );
  
  (*lSlider->cCallback)(pWidget, lSlider->cUserData);
}
//-------------------------------------------------
  void MySliderFloat::value( float pVal )
  {
    if( cSlider )
      {
        cSlider->value( pVal );
        std::stringstream lOs;
        lOs <<  cSlider->value();
        cInput->value(  lOs.str().c_str() );
      }
    else
      {
        std::stringstream lOs;
        lOs <<  pVal;
        cInput->value(  lOs.str().c_str() );
      }
  }
//-------------------------------------------------
  float MySliderFloat::value()
  {
    if( cSlider != nullptr)
      return  (float)cSlider->value();
    else
      return  (float) atof(cInput->value());
  }

//******************************
//******************************
//******************************

MySliderInt::MySliderInt( int pX, int pY, int pW, int pH, const char* pLabel,  Fl_Callback pCB, void* pUserData, long pMin, long pMax, bool pHaveSlider )
  :cSlider(nullptr)
  ,cCallback(pCB)
  ,cUserData(pUserData)  
  ,cHaveSlider( pHaveSlider )
{ 
  cInput = new Fl_Int_Input(pX, pY, pW/4, pH, pLabel);
  cInput->align(Fl_Align(FL_ALIGN_TOP_LEFT));
  
  if( cHaveSlider )
    {
      cInput->callback((Fl_Callback*)sInputCB, this);

        cSlider = new Fl_Hor_Nice_Slider( pX+pW/4, pY, (pW/4)*3, pH, "");
 
  
      cSlider->align(Fl_Align(FL_ALIGN_TOP_LEFT));
            
      cSlider->bounds(pMin, pMax );
      
      cSlider->callback((Fl_Callback*)sSliderCB, this);
    }
  value( pMin );
}
//-------------------------------------------------
void MySliderInt::sSliderCB( Fl_Widget*pWidget, void*pUserData )
{
  MySliderInt* lSlider= reinterpret_cast<MySliderInt*>(pUserData);
  std::stringstream lOs;
  lOs <<  lSlider->value();
  lSlider->cInput->value(  lOs.str().c_str() );
  (*lSlider->cCallback)(pWidget, lSlider->cUserData);  
}
//-------------------------------------------------
void MySliderInt::sInputCB( Fl_Widget*pWidget, void*pUserData )
{
  MySliderInt* lSlider= reinterpret_cast<MySliderInt*>(pUserData);
  if( lSlider->cSlider != nullptr )
    lSlider->cSlider->value(  atof(lSlider->cInput->value()) );
  
  (*lSlider->cCallback)(pWidget, lSlider->cUserData);
}
//-------------------------------------------------
  void MySliderInt::value( long pVal )
  {
    if( cSlider )
      {
        cSlider->value( pVal );
        std::stringstream lOs;
        lOs <<  cSlider->value();
        cInput->value(  lOs.str().c_str() );
      }
    else
      {
        std::stringstream lOs;
        lOs <<  pVal;
        cInput->value(  lOs.str().c_str() );
      }
  }
//-------------------------------------------------
  long MySliderInt::value()
  {
    if( cSlider )
      return  (long)cSlider->value();
    else
      return  (long) atol(cInput->value());
  }
