
#include "MyFloatInput.h"

#include "Win3dBase.h"
#include "Canvas3dBase.h"

#include "Creation.h"

#include <FL/Fl_Float_Input.H>
#include <FL/Fl_Output.H>



namespace M3d {
  //****************************************

Win3dBase::Win3dBase(const char*pName, int pW, int pH )
    :Fl_Window( pW, pH, pName )
    ,cMenubar(0,0,10000, 30)
  {      
    initWin(  pW, pH);
 //    cX += lW;
  }
  //-------------------------------------------
  void  Win3dBase::initWin( int pW, int pH)
  {    
    static int slWinId=1;
    cWinId=slWinId++;

    
     cX = 15;
     cY = cMenubar.h();
    int lH = (int)(((float)cMenubar.h())*0.6f);
    int lW = 50;

 
    cSzOutputInfo = 20;

    cCurrentUnity =  new MyChoiceButton( cX, cY+lH, lW*1.5, lH, "u", ChangeUnity, this );
    for( int i=0 ; i< PP3d::Kamera::GetNbMeasurement(); i++)
      {
	cCurrentUnity->add( PP3d::Kamera::GetMeasurement(i));
      }
    cCurrentUnity->value(PP3d::Kamera::GetDefaultMeasurement());
    cCurrentUnity->tooltip("Current unity");

    cX += cCurrentUnity->w() + 2;
    //    cCurrentScale = new MyFloatInput( cX, cY+lH, lW, lH, "Scale" );
    //    cX += cCurrentScale->w() + 2;
    cCurrentInput1 = new MyFloatInput( cX, cY+lH, lW, lH, "i" );
    
    cX = 15;
   									
    cXinput = new MyFloatInput( cX, cY, lW*1.5, lH, "x" );
    cX += cXinput->w() + 16;
    cYinput = new MyFloatInput( cX, cY, lW*1.5, lH, "y" );
    cX += cYinput->w() + 16;
    cZinput = new MyFloatInput( cX, cY, lW*1.5,  lH, "z" );
    cX += cZinput->w() + 16;
 

    lW =  Creation::sIconSize+4;
    lH =  Creation::sIconSize+4;

    //   cCanvas3d = createCanvas( 0, cXCanvas, this->w()-10, (this->h()-cX Canvas)-cSzOutputInfo , "1" );
    
    //   this->resizable( cCanvas3d  );
  }
  //-------------------------------------------
  void Win3dBase::setCursorPosition( PP3d::Point3d& pPos)
  {
    cXinput->setFloatValue( pPos.cX );
    cYinput->setFloatValue( pPos.cY );
    cZinput->setFloatValue( pPos.cZ );
  }
  //-------------------------------------------
  void Win3dBase::setCurrentVal( const char* iLabel, double iVal)
  {
    cCurrentInput1->copy_label( iLabel );

    cCurrentInput1->setFloatValue( iVal );
  }
  //-------------------------------------------
  double  Win3dBase::getCurrentVal()
  {
    return strtod(cCurrentInput1->value(), nullptr);
  }
  //-------------------------------------------
  void Win3dBase::setInfo( const std::string & iStr )
  {
    cInfoOutput->value(iStr.c_str());
  }
  //****************************************

} // namespace
