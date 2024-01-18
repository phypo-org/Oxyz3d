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

#include <stdlib.h>

#include "Gui/MySlider.h"

#include "Creation.h"

#include "Shape/Object.h"

//Tout mettre dans le Dialogue

namespace M3d {

  //************************
  class DialogPerspectiv {

    Canvas3d     *cMyCanvas;
    PP3d::Kamera &cKamera;
    PP3d::Kamera cKameraSav;

    Fl_Double_Window* myWindow;

    std::unique_ptr<MySlider> cSliderFovY;	
    std::unique_ptr<MySlider> cSliderAspect;
    std::unique_ptr<MySlider> cSliderZNear;
    std::unique_ptr<MySlider> cSliderZFar;
		
    std::unique_ptr<MySlider> cSliderSize;	

    std::unique_ptr<MySlider> cSliderPosX;
    std::unique_ptr<MySlider> cSliderPosY;
    std::unique_ptr<MySlider> cSliderPosZ;
		
    std::unique_ptr<MySlider> cSliderRotX;
    std::unique_ptr<MySlider> cSliderRotY;
    std::unique_ptr<MySlider> cSliderRotZ;

  public:

    bool cContinue;

  protected:

    //************************
    void maj()
    {
      std::cout << " FovY:"   << cSliderFovY->value()
	//		<< " Aspect:" << cSliderAspect->value()
		<< " ZNear:"  << cSliderZNear->value()
		<< " ZNear:"  << cSliderZFar->value()
		<< std::endl;
			
      std::cout << " Size: " << cSliderSize->value() << std::endl;
		
      std::cout << " X:" << cSliderPosX->value()  << " Y:" << cSliderPosY->value() << " Z:" << cSliderPosZ->value() << std::endl;
      std::cout << " rX:" << cSliderRotX->value()  << " rY:" << cSliderRotY->value() << " rZ:" << cSliderRotZ->value() << std::endl;
 
      cKamera.setPerspective( cSliderFovY->value(), cKamera.cAspect, cSliderZNear->value(),  cSliderZFar->value() );
    
      PP3d::Double3 lPos( cSliderPosX->value() , 	cSliderPosY->value() ,	cSliderPosZ->value() );
      PP3d::Double3 lRot( cSliderRotX->value() , 	cSliderRotY->value() ,	cSliderRotZ->value() );

      cKamera.position() = lPos;
      cKamera.angle() = lRot;
			
      double  lSz = cSliderSize->value();
    
      if( lSz < 0.0000001 )
	lSz =  0.0000001;
			
      if( lSz > 1E6 )
	lSz= 1E6;
			
			
      cKamera.scaleTo(lSz);

      cMyCanvas->redraw();
    }

    //----------------------------------------
    static  void SliderCB( Fl_Widget*, void*pUserData )
    {

      DialogPerspectiv* lDialog = reinterpret_cast<DialogPerspectiv*>(pUserData);
      lDialog->maj();
    }
    //----------------------------------------
    static void SizeCB( Fl_Widget*, void*pUserData )
    {
      DialogPerspectiv* lDialog = reinterpret_cast<DialogPerspectiv*>(pUserData);
      // lDialog->cSizeSliderX->value(  atof(wSizeX->value()) );
      lDialog->maj();
    }
    //----------------------------------------
    static void SizeSliderCB( Fl_Widget*, void*pUserData )
    {
      DialogPerspectiv* lDialog = reinterpret_cast<DialogPerspectiv*>(pUserData);
      lDialog->maj(); 
    }
    //----------------------------------------
	
    static void ResetCB( Fl_Widget*, void* pUserData ) {
		
      DialogPerspectiv* lDialog = reinterpret_cast<DialogPerspectiv*>(pUserData);
		
      lDialog->cKamera = lDialog->cKameraSav;
      lDialog->resetValueDialog();
      lDialog->cMyCanvas->redraw();
    }
    //----------------------------------------
	
    static void CancelCB( Fl_Widget*, void* pUserData ) {
		
      DialogPerspectiv* lDialog = reinterpret_cast<DialogPerspectiv*>(pUserData);
		
      lDialog->cKamera = lDialog->cKameraSav;
      lDialog->cMyCanvas->redraw();
		
      Fl::delete_widget( lDialog->myWindow );
      lDialog->cContinue = false;
    }
    //----------------------------------------
    static void OkCB( Fl_Widget*, void*pUserData ) {	 
		
      DialogPerspectiv* lDialog = reinterpret_cast<DialogPerspectiv*>(pUserData);


      Fl::delete_widget(lDialog->myWindow);  // Normly if I am understand the documentation, it will destroy all the children
      lDialog->cContinue = false;
    }
	
    //----------------------------------------
    void resetValueDialog()
    {
      cSliderFovY->value( (float)cKamera.cFovY );
      //		cSliderAspect->value( cKamera.cAspect );
      cSliderZNear->value((float) cKamera.cZnear );
      cSliderZFar->value( (float)cKamera.cZfar );

      cSliderSize->value( (float) cKamera.scale().x()  );

      cSliderPosX->value( (float)cKamera.position().x() );
      cSliderPosY->value( (float)cKamera.position().y() );
      cSliderPosZ->value( (float)cKamera.position().z() );
			
      cSliderRotX->value( (float)cKamera.angle().x() );
      cSliderRotY->value( (float)cKamera.angle().y() );
      cSliderRotZ->value( (float)cKamera.angle().z() );

    }
    //----------------------------------------
  public:
	
    DialogPerspectiv( Canvas3d* pCanvas,  PP3d::Kamera &pKamera)
      :cMyCanvas( pCanvas )
      ,cKamera(pKamera)
      ,cKameraSav( pKamera )
    {
      int lX = 20;
      int lY= 30;
      int lW = 400;
      int lH = 20;
      int lYStep = 40;


      myWindow = new Fl_Double_Window(lW+lX*3, 100, "View settings");
      myWindow->callback((Fl_Callback*)CancelCB, this);
      myWindow->position( 100, 100) ;

      {  Fl_Group* o = new Fl_Group(lX, lY, lW+lX*2, lYStep*4, " Perspective:");
	o->box(FL_ENGRAVED_FRAME);
	lY += lYStep;

			
	cSliderFovY =  std::unique_ptr<MySlider>(new MySlider(lX+5, lY, lW, lH, "FovY", SliderCB, this, 0, 1000 ));
		 
	lY += lYStep;
	 
      
	//			cSliderAspect =  std::unique_ptr<MySlider>(new MySlider(lX+5, lY, lW, lH, "Aspect", SliderCB, this, -100, 100 ));
	//		lY += lYStep;

		
	cSliderZNear =  std::unique_ptr<MySlider>(new MySlider(lX+5, lY, lW, lH, "Z near", SliderCB, this, 0.1, 100 ));
	lY += lYStep;

	cSliderZFar=  std::unique_ptr<MySlider>(new MySlider(lX+5, lY, lW, lH, "Z far", SliderCB, this, 0.1, 1000 ));
	lY += lYStep;
			
	o->end();
      } // Fl_Group* o

      lY += lYStep;


      cSliderSize =  std::unique_ptr<MySlider>(new MySlider(lX+5, lY, lW, lH, "Size", SliderCB, this, 0.0000001, 100 ));
      cSliderSize->value(  (float)cKamera.scale().x()  );
      lY += lYStep;
      lY += lYStep;


      {  Fl_Group* o = new Fl_Group(lX, lY, lW+lX*2, lYStep*4, "Position:");
	o->box(FL_ENGRAVED_FRAME);
	lY += lYStep; 

	o->align(Fl_Align(FL_ALIGN_TOP_LEFT));
      
	cSliderPosX = std::unique_ptr<MySlider>(new MySlider(lX+5, lY, lW, lH, "X", SliderCB, this, -100, 100 ));
	lY += lYStep;
 
	cSliderPosY =  std::unique_ptr<MySlider>(new MySlider(lX+5, lY, lW, lH, "Y", SliderCB, this, -100, 100 ));
	lY += lYStep;
 
	cSliderPosZ =  std::unique_ptr<MySlider>(new MySlider(lX+5, lY, lW, lH, "Z", SliderCB, this, -100, 100 ));
	lY += lYStep;

	o->end();
      } // Fl_Group* o
      lY += lYStep;



      {  Fl_Group* o = new Fl_Group(lX, lY, lW+lX*2, lYStep*4, "Rotation:");
	o->box(FL_ENGRAVED_FRAME);
	lY += lYStep; 

	o->align(Fl_Align(FL_ALIGN_TOP_LEFT));
      
	cSliderRotX = std::unique_ptr<MySlider>(new MySlider(lX+5, lY, lW, lH, "X", SliderCB, this, 0, 360 ));
	lY += lYStep;
 
	cSliderRotY =  std::unique_ptr<MySlider>(new MySlider(lX+5, lY, lW, lH, "Y", SliderCB, this, 0, 360 ));
	lY += lYStep;
 
	cSliderRotZ =  std::unique_ptr<MySlider>(new MySlider(lX+5, lY, lW, lH, "Z", SliderCB, this, 0, 360 ));
	lY += lYStep;

	o->end();
      } // Fl_Group* o

		
      resetValueDialog();


		
		
      lY += lYStep;
			

      { Fl_Button* o = new Fl_Button( 10, lY, 75, 25, "Reset");
	o->callback((Fl_Callback*)ResetCB, this );
      } // Fl_Button* o
      { Fl_Button* o = new Fl_Button((int)(((float)lW)*0.5f), lY, 75, 25, "Cancel");
	o->callback((Fl_Callback*)CancelCB, this );
      } // Fl_Button* o
      { Fl_Button* o = new Fl_Button((int)(((float)lW)*0.75f), lY, 75, 25, "OK");
	o->callback((Fl_Callback*)OkCB, this );
      } // Fl_Button* o
			
      lY += lYStep;
	
      myWindow->size(  lW+lX*3, lY );
      myWindow->end();
		
      myWindow->show( 0, nullptr);
      maj();


      // BON il y un probleme avec la sortie, mauvais  utilisation (j'ai eu tord de suivre les exemples ... ), ça marche quand meme, mais il faut faire attention !
      cContinue = true;
      while (Fl::wait() && cContinue );

	
      std::cout << "*********************************** FIN DIALIOGUE **************************" << std::endl;

    }
    //************************
  };

}


//************************
extern void CallDialogPerspectiv( M3d::Canvas3d* pCanvas, PP3d::Kamera& pKamera  )
{
  M3d::DialogPerspectiv( pCanvas, pKamera );
}
//************************
