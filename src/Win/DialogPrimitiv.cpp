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

#include "MySlider.h"

#include "Application.h"

#include "Shape/Object.h"

//Tout mettre dans le Dialogue

namespace M3d {

  //************************
  class DialogPrimitiv {

    Canvas3d *cMyCanvas;
    PP3d::PrimitivFactory::Type cMyType;

    Fl_Double_Window* myWindow;

    std::unique_ptr<MySlider> cSliderSize;
	
    std::unique_ptr<MySlider> cSliderPosX;
    std::unique_ptr<MySlider> cSliderPosY;
    std::unique_ptr<MySlider> cSliderPosZ;

    std::unique_ptr<MySlider> cSliderRotX;
    std::unique_ptr<MySlider> cSliderRotY;
    std::unique_ptr<MySlider> cSliderRotZ;


  public:
    DialogPrimitiv( Canvas3d* pCanvas, PP3d::PrimitivFactory::Type );
    bool cContinue;

  protected:
    static void CancelCB( Fl_Widget*, void* lUserData );
    static void OkCB( Fl_Widget*, void* lUserData);
    static void SizeSliderCB( Fl_Widget*, void*pUserData );
    static void SizeCB( Fl_Widget*, void*pUserData );
    static void SliderCB( Fl_Widget*, void*pUserData );
    void maj();
	
  };


  //************************
  void DialogPrimitiv::maj()
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
    // (precision limité processeurs) or celle ci s'accumulerais !

    // Sinon on pourrait mettre un rotation avec open gl sur l'objet
    // et faire tourner l'objet 
    // avec, par contre a la fin il faudrait vraiment faire
    // la modification sur les points (on est dans un modeleur, pas un jeu !)

    PP3d::Poly* lShape = PP3d::PrimitivFactory::Create( cMyType, (float)lSz );
    cMyCanvas->getDataBase().swapCurrentCreation( new PP3d::ObjectPoly( "Primitiv", lShape ) );  

    lShape->move(lPos );
  

    Application::Instance().redrawAllCanvas3d();
  }

  //----------------------------------------
 
  DialogPrimitiv::DialogPrimitiv( Canvas3d* pCanvas,  PP3d::PrimitivFactory::Type pType )
    :cMyCanvas( pCanvas )
  {
    cMyType = pType;

    int lX = 20;
    int lY= 30;
    int lW = 300;
    int lH = 20;
    int lYStep = 40;

    myWindow = new Fl_Double_Window(500, 511, "Primitive");
    myWindow->callback((Fl_Callback*)CancelCB, this);

   
    cSliderSize =  std::unique_ptr<MySlider>(new MySlider(lX+5, lY, lW, lH, "Size", SliderCB, this, 0.001, 100 ));
    cSliderSize->value( 1 );
    lY += lYStep;
	 
   
	 

    {  Fl_Group* o = new Fl_Group(lX, lY, lW+20, lH*7, "Position:");
      o->box(FL_ENGRAVED_FRAME);
      lY += lYStep; 

      o->align(Fl_Align(FL_ALIGN_TOP_LEFT));
      
      cSliderPosX = std::unique_ptr<MySlider>(new MySlider(lX+5, lY, lW, lH, "X", SliderCB, this, -100, 100 ));
      cSliderPosX->value( 0 );
      lY += lYStep;
 
      cSliderPosY =  std::unique_ptr<MySlider>(new MySlider(lX+5, lY, lW, lH, "Y", SliderCB, this, -100, 100 ));
      cSliderPosY->value( 0 );
      lY += lYStep;
 
      cSliderPosZ =  std::unique_ptr<MySlider>(new MySlider(lX+5, lY, lW, lH, "Z", SliderCB, this, -100, 100 ));
      cSliderPosZ->value( 0 );
      lY += lYStep;

      o->end();
    } // Fl_Group* o

    //		lY += lYStep;

    {  Fl_Group* o = new Fl_Group(lX, lY, lW+20, lH*7, "Rotation:");
      o->box(FL_ENGRAVED_FRAME);
      lY += lYStep; 

      o->align(Fl_Align(FL_ALIGN_TOP_LEFT));
      
      cSliderRotX = std::unique_ptr<MySlider>(new MySlider(lX+5, lY, lW, lH, "X", SliderCB, this, 0, 360 ));
      cSliderPosX->value( 0 );
      lY += lYStep;
 
      cSliderRotY =  std::unique_ptr<MySlider>(new MySlider(lX+5, lY, lW, lH, "Y", SliderCB, this, 0, 360 ));
      cSliderPosY->value( 0 );
      lY += lYStep;
 
      cSliderRotZ =  std::unique_ptr<MySlider>(new MySlider(lX+5, lY, lW, lH, "Z", SliderCB, this, 0, 360 ));
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
	 

    myWindow->end();
		
    myWindow->show( 0, nullptr);
    maj();


    // BON il y un probleme avec la sortie, mauvais  utilisation (j'ai eu tord de suivre les exemples ... ), ça marche quand meme, mais il faut faire attention !
    cContinue = true;
    while (Fl::wait() && cContinue );

	
    std::cout << "*********************************** FIN DIALIOGUE **************************" << std::endl;
    std::cout << "*********************************** FIN DIALIOGUE **************************" << std::endl;
    std::cout << "*********************************** FIN DIALIOGUE **************************" << std::endl;
    std::cout << "*********************************** FIN DIALIOGUE **************************" << std::endl;
    std::cout << "*********************************** FIN DIALIOGUE **************************" << std::endl;
    std::cout << "*********************************** FIN DIALIOGUE **************************" << std::endl;
    std::cout << "*********************************** FIN DIALIOGUE **************************" << std::endl;
  }
  //----------------------------------------

  void DialogPrimitiv::CancelCB( Fl_Widget*, void* pUserData ) {
  
    DialogPrimitiv* lDialog = reinterpret_cast<DialogPrimitiv*>(pUserData);
    lDialog->cMyCanvas->getDataBase().cancelCurrentCreation();

    Application::Instance().redrawAllCanvas3d();

    Fl::delete_widget( lDialog->myWindow );
    lDialog->cContinue = false;
  }
  //----------------------------------------
  void DialogPrimitiv::SliderCB( Fl_Widget*, void*pUserData )
  {

    DialogPrimitiv* lDialog = reinterpret_cast<DialogPrimitiv*>(pUserData);
    std::cout << "DialogPrimitiv::SliderCB " << lDialog->cSliderSize->value() << std::endl;
    lDialog->maj();
  }
  //----------------------------------------
  void DialogPrimitiv::SizeCB( Fl_Widget*, void*pUserData )
  {
    DialogPrimitiv* lDialog = reinterpret_cast<DialogPrimitiv*>(pUserData);
    // lDialog->cSizeSliderX->value(  atof(wSizeX->value()) );
    lDialog->maj();
  }
  //----------------------------------------
  void DialogPrimitiv::SizeSliderCB( Fl_Widget*, void*pUserData )
  {
    DialogPrimitiv* lDialog = reinterpret_cast<DialogPrimitiv*>(pUserData);
    lDialog->maj(); 
  }
  //----------------------------------------
  void DialogPrimitiv::OkCB( Fl_Widget*, void*pUserData )
  {
    DialogPrimitiv* lDialog = reinterpret_cast<DialogPrimitiv*>(pUserData);
    lDialog->maj();
    PP3d::Object* lObj = lDialog->cMyCanvas->getDataBase().validCurrentCreation();
    if( lObj != nullptr )
      {
	lObj->rename( PP3d::PrimitivFactory::GetTypeName( lDialog->cMyType) );
      }


    Application::Instance().redrawAllCanvas3d();
    Application::Instance().redrawObjectTree();

    Fl::delete_widget(lDialog->myWindow);  // Normly if I am understand the documentation, it will destroy all the children
    lDialog->cContinue = false;
  }
  //************************
}


//************************
extern void CallDialogPrimitiv( bool& pFlagAlreadyExist, M3d::Canvas3d* pCanvas, PP3d::PrimitivFactory::Type pType  )
{
  // Il faudrait envoyer un ptr sur une variable de type DialogPrimitv
  // Que l'on remplirais et qui servirais de flag, en meme temps
  // on pourrait faire un show dessus!
	
  if( pFlagAlreadyExist == true )
    return ;
	
  pFlagAlreadyExist = true;
  M3d::DialogPrimitiv( pCanvas, pType );
  pFlagAlreadyExist = false;
}
//************************

