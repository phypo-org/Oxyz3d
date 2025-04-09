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


#include <sstream>
#include <stdlib.h>
#include <cmath>    
#include <stdlib.h>
#include <math.h>

#include "GuiFLTK/MySlider.h"

#include "Creation.h"

#include "Magnet.h"

#include "Utils.h"


//Tout mettre dans le Dialogue

using namespace std;

namespace M3d {
  using namespace PP3d;


#define Diag M3d::DialogMagnet::Instance()
 
  //************************
  class DialogMagnet  : public virtual PPSingletonCrtp<DialogMagnet>{
    
    Magnet * cMagnet = nullptr;

    Magnet & getMagnet() { return *cMagnet; }
    
    Fl_Double_Window                * cMyWindow    = nullptr  ;
    std::unique_ptr<MyChoiceButton>   cChoiceDirection ;
    std::unique_ptr<MyChoiceButton>   cChoiceAction ;
    std::unique_ptr<MyChoiceButton>   cChoiceAlgo   ;
    std::unique_ptr<MySlider>         cSliderSize   ;
 
    std::unique_ptr<MyCheckbutton>    cCheckX;
    std::unique_ptr<MyCheckbutton>    cCheckY;   
    std::unique_ptr<MyCheckbutton>    cCheckZ;

    
	
  public:
    DialogMagnet(  ) {;}
    bool isAlreadyRunning() { return Diag.cMyWindow != nullptr; }

    //----------------------------------------
    void close()
    {      
      //          Fl::delete_widget( cMyWindow);  // Normly if I am understand the documentation, it will destroy all the children
      //          cMyWindow = nullptr;
      std::cout << "HIDE" << std::endl;
      cMyWindow->hide();
    }
    //----------------------------------------
    void init( Magnet & iMagnet)
    {
      cMagnet = & iMagnet;
      
      std::cout << "*********************************** DialogMagnet init  **************************" << std::endl;
   

      int lX =80;
      int lY= 30;
      int lW = 300;
      int lH = 20;
      int lYStep = 40;
      int lXStep = 200;
      int lMemX=0;

      cMyWindow = new Fl_Double_Window(500, 300, "Magnet");
      cMyWindow->callback((Fl_Callback*)CloseCB, this);
  
      
      cChoiceAction= std::unique_ptr<MyChoiceButton>( new MyChoiceButton(lX, lY, lW, lH, "Action", ChoiceCB, this ))  ;
      cChoiceAction->callback((Fl_Callback*)ChoiceCB, this );
      cChoiceAction->add( "Attrack"); //0
      cChoiceAction->add( "Repel"); //1
      lY += lYStep; 

      //  cChoiceAction->value( (int)getMagnet().getAction());
      cChoiceDirection= std::unique_ptr<MyChoiceButton>( new MyChoiceButton(lX, lY, lW, lH, "Dir", ChoiceCB, this ))  ;
      cChoiceDirection->callback((Fl_Callback*)ChoiceCB, this );
      cChoiceDirection->add( "Free"); //0
      cChoiceDirection->add( "Normal"); //1


      lY += lYStep; 

      cChoiceAlgo = std::unique_ptr<MyChoiceButton>( new MyChoiceButton(lX, lY, lW, lH, "Algorithm", ChoiceCB, this ))  ;
      cChoiceAlgo->callback((Fl_Callback*)ChoiceCB, this );
      cChoiceAlgo->add( "Distance proportionnal"); //0
      cChoiceAlgo->add( "Square Distance proportionnal"); //1
      cChoiceAlgo->add( "Square Root Distance proportionnal"); //2
      cChoiceAlgo->add( "Distance independant"); //3
      //   cChoiceAlgo->value( (int)getMagnet().getAlgo());
      lY += lYStep; 

      
      cSliderSize = std::unique_ptr<MySlider>( new MySlider(lX, lY, lW, lH, "Size of Magnet", SliderCB, this, 0.001, 1000 ));
      cSliderSize->value( getMagnet().getSize() );
      lY += lYStep;
	 
   
       {
         Fl_Group* o = new Fl_Group(lX, lY, lW+20, lH*2, "Position:");
          o->box(FL_ENGRAVED_FRAME);
          o->align(Fl_Align(FL_ALIGN_TOP_LEFT));
          lY += lYStep/2;

            
          cCheckX = std::unique_ptr<MyCheckbutton>( new MyCheckbutton( lX+50, lY, 30,15, "X", CheckCB, this, 0 ));
          cCheckX->callback( (Fl_Callback*)CheckCB, this );
          //  cCheckX->value( getMagnet().isUsingX());
          
          cCheckY = std::unique_ptr<MyCheckbutton>(new MyCheckbutton( lX+100, lY, 30,15, "Y", CheckCB, this, 0 ));
          cCheckY->callback((Fl_Callback*)CheckCB, this );
          //  cCheckY->value(  getMagnet().isUsingY());
          
          cCheckZ = std::unique_ptr<MyCheckbutton>(new MyCheckbutton( lX+150, lY, 30,15, "Z", CheckCB, this, 0 ));
          cCheckZ->callback((Fl_Callback*)CheckCB, this );
          //  cCheckZ->value(  getMagnet().isUsingZ());
          
          lY += lYStep;
       }

      
		

      { Fl_Button* o = new Fl_Button(210, lY, 75, 25, "Close");
	o->callback((Fl_Callback*)CloseCB, this );
      } // Fl_Button* o

      majFromMagnet();

      cMyWindow->end();
		
      cMyWindow->show();
      //0, nullptr);
	
      std::cout << "*********************************** FIN DIALOGUE **************************" << std::endl;
      maj();
      
      //      while( Fl::wait() && MyDiag.cMyWindow != nullptr );

    }

    
    public:   

    //********************************************************************************
    // Calling when command's interface change
    void majFromMagnet()
    {
      cChoiceAction->value( (int)getMagnet().getAction());
      cChoiceDirection->value( (int)getMagnet().getDirection());
      cSliderSize->value(getMagnet().getSize());
      cChoiceAlgo->value( (int)getMagnet().getAlgo());
      cCheckX->value(  getMagnet().isUsingX() );
      cCheckY->value(  getMagnet().isUsingY() );
      cCheckZ->value(  getMagnet().isUsingZ() );
    }
    void show()
    {
      cMyWindow->show();
      //0, nullptr);
    }
    
  protected:   
    //----------------------------------------

    void maj()
    {
      std::cout << "Magnet::maj " << this << std::endl;

      getMagnet().setAction( (MagnetAction)cChoiceAction->value() );
      getMagnet().setDirection( (MagnetDirection)cChoiceDirection->value() );
      getMagnet().setAlgo( (MagnetAlgo)cChoiceAlgo->value() );
      getMagnet().setSize( cSliderSize->value() );
      getMagnet().useX( (cCheckX->value() ==1));
      getMagnet().useY( (cCheckY->value() ==1));
      getMagnet().useZ( (cCheckZ->value() ==1));

      TheCreat.redrawAll(PP3d::Compute::FacetAll);
    }
    //----------------------------------------

    static void CancelCB( Fl_Widget*, void* pUserData ) {
 
      DialogMagnet* lDialog = reinterpret_cast<DialogMagnet*>(pUserData);      

      TheCreat.redrawAllCanvas3d( PP3d::Compute::FacetAll );

      Fl::delete_widget( lDialog->cMyWindow );
      Diag.cMyWindow = nullptr;
    }
    //----------------------------------------
    static void SliderCB( Fl_Widget*, void*pUserData )
    {
      DialogMagnet* lDialog = reinterpret_cast<DialogMagnet*>(pUserData);
      lDialog->maj();
    }
    //----------------------------------------
    static  void SizeCB( Fl_Widget*, void*pUserData )
    {
      DialogMagnet* lDialog = reinterpret_cast<DialogMagnet*>(pUserData);
      lDialog->maj();
    }
    //----------------------------------------
    static void SizeSliderCB( Fl_Widget*, void*pUserData )
    {
      DialogMagnet* lDialog = reinterpret_cast<DialogMagnet*>(pUserData);
      lDialog->maj(); 
    }
    //----------------------------------------
    static void CheckCB( Fl_Widget*, void*pUserData )
    {		 
      DialogMagnet* lDialog = reinterpret_cast<DialogMagnet*>(pUserData);
      lDialog->maj(); 
    }
  //----------------------------------------
    static void ChoiceCB( Fl_Widget*, void*pUserData )
    {									 
      DialogMagnet* lDialog = reinterpret_cast<DialogMagnet*>(pUserData);
      lDialog->maj(); 
    }
    //----------------------------------------
    static void CloseCB( Fl_Widget*, void*pUserData )
    {
      DialogMagnet* lDialog = reinterpret_cast<DialogMagnet*>(pUserData);
      lDialog->maj();
      lDialog->close();
    }
    //************************
  };
} // namespace M3d

//************************
extern void CallDialogMagnet( M3d::Magnet & iMagnet )
{
  cout << "CallDialogMagnet " <<  Diag.isAlreadyRunning()  << endl;
  
  if( Diag.isAlreadyRunning() == false )
    {
      Diag.init( iMagnet  );     
      std::cout << "*************** After init **************" << std::endl;
    }
  else
    Diag.show();
}
//************************
extern void CloseDialogMagnet()
{
  cout << "CloseMagnet " <<  Diag.isAlreadyRunning()  << endl;
  
  if( Diag.isAlreadyRunning() )
    {
      Diag.close();
    }
}
//************************
extern void MajDialogMagnet()
{
  cout << "CloseMagnet " <<  Diag.isAlreadyRunning()  << endl;
  
  if( Diag.isAlreadyRunning() )
    {
      Diag.majFromMagnet();
    }
}
//************************

