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

#include "MySlider.h"

#include "Application.h"

#include "Shape/Object.h"
#include "Shape/Maker.h"


//Tout mettre dans le Dialogue

namespace M3d {

  //************************
  class DialogInputInt {

    Canvas3d *cMyCanvas;
    Fl_Double_Window* myWindow;


    std::unique_ptr<MySlider> cSliderPas;
    int   &    cVal;
    bool  &    cRetVal;
	
  public:
    bool cContinue;


    //----------------------------------------
 
    DialogInputInt( Canvas3d* pCanvas, const char* iLabel, int & ioVal, bool & ioRetVal )
      :cMyCanvas( pCanvas )
      ,cVal( ioVal )
      ,cRetVal( ioRetVal )
    {   
      int lX = 20;
      int lY= 30;
      int lW = 400;
      int lH = 20;
      int lYStep = 40;

      myWindow = new Fl_Double_Window(lW+lX*3, 100, iLabel);
      myWindow->callback((Fl_Callback*)CancelCB, this);
      myWindow->position( 100, 100) ;
   
      cSliderPas =  std::unique_ptr<MySlider>(new MySlider(lX+5, lY, lW, lH, "value", SliderCB, this, 2, 100 ));
      cSliderPas->value( cVal );
    
      lY += lYStep;
      lY += lYStep;
	 

      { Fl_Button* o = new Fl_Button(10, lY, 75, 25, "Cancel");
	o->callback((Fl_Callback*)CancelCB, this );
      } // Fl_Button* o
      { Fl_Button* o = new Fl_Button(125, lY, 75, 25, "OK");
	o->callback((Fl_Callback*)OkCB, this );
      } // Fl_Button* o
	 
      lY += lYStep;

      myWindow->size(  lW+lX*3, lY );
      myWindow->end();
		
      myWindow->show( 0, nullptr);

      
      cContinue = true;
      while (Fl::wait() && cContinue );


      std::cout << "*********************************** FIN DIALOGUE **************************" << std::endl;
    }
    //----------------------------------------
    static  void SliderCB( Fl_Widget*, void*pUserData )
    {
      std::cout << "DialogInputInt::SliderCB " <<   std::endl;
      std::cout << "DialogInputInt::SliderCB " <<   std::endl;
      std::cout << "DialogInputInt::SliderCB " <<   std::endl;
      std::cout << "DialogInputInt::SliderCB " <<   std::endl;
      std::cout << "DialogInputInt::SliderCB " << pUserData << std::endl;

      DialogInputInt* lDialog = reinterpret_cast<DialogInputInt*>(pUserData);
      //      std::cout << "DialogInputInt::SliderCB " << lDialog->cSliderPas->value() << std::endl;
    }
    //----------------------------------------
    static  void CancelCB( Fl_Widget*, void* pUserData )
    {
      DialogInputInt* lDialog = reinterpret_cast<DialogInputInt*>(pUserData);

      Fl::delete_widget( lDialog->myWindow );
      lDialog->cContinue = false;
      lDialog->cRetVal = false;
    }
    //----------------------------------------
    static  void OkCB( Fl_Widget*, void*pUserData )
    {
      DialogInputInt* lDialog = reinterpret_cast<DialogInputInt*>(pUserData); 
      
      Fl::delete_widget(lDialog->myWindow);  // Normly if I am understand the documentation, it will destroy all the children
      
      lDialog->cVal = (int)lDialog->cSliderPas->value();
      lDialog->cRetVal = true ;
      lDialog->cContinue = false;
            
    }
    //************************
 
  };
}

  //************************
extern bool CallDialogInputInt( bool& pFlagAlreadyExist, M3d::Canvas3d* pCanvas,  const char* iLabel, int & ioVal)
  {
    // Il faudrait envoyer un ptr sur une variable de type DialogPrimitv
    // Que l'on remplirais et qui servirais de flag, en meme temps
    // on pourrait faire un show dessus!
	
    if( pFlagAlreadyExist == true )
      return false;

    bool lRetVal = false;
    pFlagAlreadyExist = true;
    
    M3d::DialogInputInt( pCanvas, iLabel, ioVal, lRetVal );
    
    pFlagAlreadyExist = false;


    std::cout << "Result:" << ioVal << std::endl;
    std::cout << "Result:" << ioVal << std::endl;
    std::cout << "Result:" << ioVal << std::endl;
    std::cout << "Result:" << ioVal << std::endl;
    std::cout << "Result:" << ioVal << std::endl;
    std::cout << "Result:" << ioVal << std::endl;
    std::cout << "Result:" << ioVal << std::endl;
    std::cout << "Result:" << ioVal << std::endl;
   return lRetVal;
  }
  //************************



