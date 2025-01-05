#ifndef  H__Win3DBASE__H
#define  H__Win3DBASE__H



#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Sys_Menu_Bar.H>
#include <FL/Fl_Menu_Button.H>


#include <FL/Fl_Native_File_Chooser.H>
#include <FL/Fl_Input.H>


#include <string>
#include <memory>

#include "Canvas3dBase.h"

#include "MyFloatInput.h"

class Fl_Output;


namespace M3d {

  //****************************************
  class Win3dBase : public Fl_Window{
  protected:
    int                    cWinId=0;
    int                    cX=0;
    int                    cY=0;
      
    Fl_Menu_Bar            cMenubar;
      
    MyFloatInput*           cXinput;
    MyFloatInput*           cYinput;
    MyFloatInput*           cZinput;
      
    MyChoiceButton*         cCurrentUnity; //METTRE une drop down !
    MyFloatInput*           cCurrentScale;
    MyFloatInput*           cCurrentInput1;
    
    long                    cCurrentMeasurementUnit=0;
    int                     cSzOutputInfo=20;

  public:
      
    Fl_Output      *        cInfoOutput;
      
    Fl_Native_File_Chooser  *cFileChooser = nullptr;
     

  public:
    
    Win3dBase( const char*pName, int pW, int pH );
    //    virtual ~Win3dBase() { delete cCanvas3d; }
    
    void initWin(  int pW, int pH );
    PP3d::Kamera& getKamera() { return getCanvas().getKamera();}
 
    friend class Canvas3d;
    int getId()  { return cWinId; }
    virtual Canvas3dBase & getCanvas() = 0;
 
    void setMeasurement( double iScale, long lPosScale )
    {
      //      std::cout << "setMeasurement : " << iScale << " " <<  lPosScale << std::endl;
      
      cCurrentMeasurementUnit    = lPosScale;
      //      cCurrentScale->setFloatValue( iScale );
      cCurrentUnity->value( cCurrentMeasurementUnit );
    }
    void setMeasurement( int lPosScale )
    {
      double lScale =  pow( 10, lPosScale-12);
      setMeasurement( lScale, lPosScale );
    }

    double getCurrentVal();
    void setCurrentVal( const char* iLabel, double iVal);
    void setInfo( const std::string & iStr );
    void canvasRedraw() { getCanvas().redraw(); }
    void setCursorPosition( PP3d::Point3d& pPos);

    //    virtual Canvas3dBase * createCanvas(  int pX, int pY, int pW, int pH, const char* iName)=0;
      
  };
 

  //****************************************
#define INFO( A )	  { ostringstream lInfoStream ; lInfoStream << A; cInfoOutput->value(lInfoStream.str().c_str() );}
#define SINFO( WIN3D, A ) { ostringstream lInfoStream ; lInfoStream << A; WIN3D->cInfoOutput->value(lInfoStream.str().c_str() );}
#define CINFO( A )  	  { ostringstream lInfoStream ; lInfoStream << A; cMyWin3d.cInfoOutput->value(lInfoStream.str().c_str() );}

#define AINFO( A ) { ostringstream lInfoStream ; lInfoStream << A;  TheCreat.info( lInfoStream.str() ); }



  
  extern void ChangeUnity(Fl_Widget*w, void*pData);
}

#endif
