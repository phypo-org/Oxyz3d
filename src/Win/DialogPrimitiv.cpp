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

#include "Utils/PPSingletonCrtp.h"

#include "MySlider.h"

#include "Application.h"

#include "Shape/Object.h"


//Tout mettre dans le Dialogue

namespace M3d {

  #define Diag M3d::DialogPrimitiv::Instance()


  //************************
  class DialogPrimitiv : public virtual PPSingletonCrtp<DialogPrimitiv>{

    Fl_Double_Window* cMyWindow = nullptr;
    
    DialogPrimitiv() {;}
    
    friend class PPSingletonCrtp;
    
    PP3d::PrimitivFactory::Type cMyType;

    MySlider* cSliderSize;
	
    MySlider* cSliderPosX;
    MySlider* cSliderPosY;
    MySlider* cSliderPosZ;

    MySlider* cSliderRotX;
    MySlider* cSliderRotY;
    MySlider* cSliderRotZ;


  public:
    
    bool isAlreadyRunning() { return Diag.cMyWindow != nullptr; }

    //************************
    void maj()
    {
      std::cout << " X:" << cSliderPosX->value()  << " Y:" << cSliderPosY->value() << " Z:" << cSliderPosZ->value() << std::endl;
    
      PP3d::Point3d lPos( cSliderPosX->value() , 	cSliderPosY->value() ,	cSliderPosZ->value() );
      PP3d::Point3d lRot( cSliderRotX->value() , 	cSliderRotY->value() ,	cSliderRotZ->value() );

      std::cout << " Slider " << cSliderSize->value() << std::endl;
    
      double  lSz = cSliderSize->value();
    
      if( lSz < 0.0000001 )
	lSz =  0.0000001;
    
      if( lSz > 1E6 )
	lSz= 1E6;
    
      // Il est TRES IMPORTANT de recreer l'objet a chaque fois
      // car s'il y a des operations dessus (comme la rotation)
      // il y a à chaque fois forcement des erreurs de calculs 
      // (precision limité des processeurs) or celle ci s'accumulerais !

      // Sinon on pourrait mettre un rotation avec open gl sur l'objet
      // et faire tourner l'objet 
      // avec, par contre a la fin il faudrait vraiment faire
      // la modification sur les points (on est dans un modeleur, pas un jeu !)

      PP3d::Poly* lShape = PP3d::PrimitivFactory::Create( cMyType, (float)lSz );
      Application::Instance().getDatabase()->swapCurrentCreation( new PP3d::ObjectPoly( "Primitiv", lShape ) );  

      lShape->move(lPos );
  
      Application::Instance().redrawAllCanvas3d();
    }

    //----------------------------------------
 
    void init( PP3d::PrimitivFactory::Type pType )
      {
	cMyType = pType;

	int lX = 20;
	int lY= 30;
	int lW = 300;
	int lH = 20;
	int lYStep = 40;

	cMyWindow = new Fl_Double_Window(500, 511, "Primitive");
	cMyWindow->callback((Fl_Callback*)CancelCB, this);

   
	// cSliderSize =  M
	cSliderSize = new MySlider(lX+5, lY, lW, lH, "Size", SliderCB, this, 0.001, 100 );
	cSliderSize->value( 1 );
	lY += lYStep;
	 
   
	 

	{  Fl_Group* o = new Fl_Group(lX, lY, lW+20, lH*7, "Position:");
	  o->box(FL_ENGRAVED_FRAME);
	  lY += lYStep; 

	  o->align(Fl_Align(FL_ALIGN_TOP_LEFT));
      
	  cSliderPosX = new MySlider(lX+5, lY, lW, lH, "X", SliderCB, this, -100, 100 );
	  cSliderPosX->value( 0 );
	  lY += lYStep;
 
	  cSliderPosY =  new MySlider(lX+5, lY, lW, lH, "Y", SliderCB, this, -100, 100 );
	  cSliderPosY->value( 0 );
	  lY += lYStep;
 
	  cSliderPosZ =  new MySlider(lX+5, lY, lW, lH, "Z", SliderCB, this, -100, 100 );
	  cSliderPosZ->value( 0 );
	  lY += lYStep;

	  o->end();
	} // Fl_Group* o

	//		lY += lYStep;

	{  Fl_Group* o = new Fl_Group(lX, lY, lW+20, lH*7, "Rotation:");
	  o->box(FL_ENGRAVED_FRAME);
	  lY += lYStep; 

	  o->align(Fl_Align(FL_ALIGN_TOP_LEFT));
      
	  cSliderRotX = new MySlider(lX+5, lY, lW, lH, "X", SliderCB, this, 0, 360 );
	  cSliderPosX->value( 0 );
	  lY += lYStep;
 
	  cSliderRotY =  new MySlider(lX+5, lY, lW, lH, "Y", SliderCB, this, 0, 360 );
	  cSliderPosY->value( 0 );
	  lY += lYStep;
 
	  cSliderRotZ =  new MySlider(lX+5, lY, lW, lH, "Z", SliderCB, this, 0, 360 );
	  cSliderPosZ->value( 0 );
	  lY += lYStep;

	  o->end();
	} // Fl_Group* o


	lY += lYStep;
			

	{ Fl_Button* o = new Fl_Button(210, lY, 75, 25, "Cancel");
	  o->callback((Fl_Callback*)CancelCB, this );
	} // Fl_Button* o
	{ Fl_Button* o = new Fl_Button(125, lY, 75, 25, "OK");
	  o->callback((Fl_Callback*)OkCB, this );
	} // Fl_Button* o
	 

	cMyWindow->end();
		
	cMyWindow->show( 0, nullptr);
	maj();


	// BON il y un probleme avec la sortie, mauvais  utilisation (j'ai eu tord de suivre les exemples ... ), ça marche quand meme, mais il faut faire attention !

	//while (Fl::wait() && cContinue );
      }
    //----------------------------------------

    static void CancelCB( Fl_Widget*, void* pUserData ) {
    

      TheAppli.getDatabase()->cancelCurrentCreation();

      TheAppli.redrawAllCanvas3d();

      Fl::delete_widget( Diag.cMyWindow );
      Diag.cMyWindow = nullptr;
    }
    //----------------------------------------
    static void SliderCB( Fl_Widget*, void*pUserData )
    {
      std::cout << "DialogPrimitiv::SliderCB " << Diag.cSliderSize->value() << std::endl;
      Diag.maj();
    }
    //----------------------------------------
    static void SizeCB( Fl_Widget*, void*pUserData )
    {
      Diag.maj();
    }
    //----------------------------------------
    static void SizeSliderCB( Fl_Widget*, void*pUserData )
    {
      Diag.maj(); 
    }
    //----------------------------------------
    static void OkCB( Fl_Widget*, void*pUserData )
    {
      Diag.maj();
      
      PP3d::Object* lObj = TheAppli.getDatabase()->validCurrentCreation();
      if( lObj != nullptr )
	{
	  lObj->rename( PP3d::PrimitivFactory::GetTypeName( Diag.cMyType) );
	}
    
      TheAppli.redrawAll();
      PushHistory();
    
      Fl::delete_widget(Diag.cMyWindow); 
      Diag.cMyWindow = nullptr;

    }
  };  
  //************************
}


//************************
extern void CallDialogPrimitiv( PP3d::PrimitivFactory::Type pType  )
{  
  if( Diag.isAlreadyRunning() == false )
    {
      Diag.init(pType);
    }	
}
//************************

