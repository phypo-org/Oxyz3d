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
#include <math.h>

#include "MySlider.h"

#include "Application.h"

#include "Shape/Object.h"
#include "Shape/Maker.h"

#include "Shape/SubDiv.h"

//Tout mettre dans le Dialogue

namespace M3d {

//************************
class DialogSubDiv {
	
 Canvas3d *cMyCanvas;
	PP3d::GeometryType      cGeoType;
	PP3d::SubNormalizeType  cNormType;

	Fl_Double_Window* myWindow;

	std::unique_ptr<MySlider> cSliderDepth;
	std::unique_ptr<MySlider> cSlize;
		
	std::unique_ptr<MyCheckbutton> cCheckCentralPoint; 
	

	std::unique_ptr<MySlider> cSliderPosX;
	std::unique_ptr<MySlider> cSliderPosY;
	std::unique_ptr<MySlider> cSliderPosZ;

	//	std::unique_ptr<MyCheckbutton> cCheckCloseLow; 
	//	std::unique_ptr<MyCheckbutton> cCheckCloseSeg;

	
public:
	DialogSubDiv( Canvas3d* pCanvas,  TypeRevol pTypeRevol );
	bool cContinue;

protected:
	static void CancelCB    ( Fl_Widget*, void* iUserData );
	static void OkCB        ( Fl_Widget*, void* iUserData);
	static void ResetCB     ( Fl_Widget*, void* iUserData);

	static void SizeSliderCB( Fl_Widget*, void* iUserData );
 	static void SizeCB      ( Fl_Widget*, void* iUserData );
 	static void SliderCB    ( Fl_Widget*, void* iUserData );
 	static void CheckCB     ( Fl_Widget*, void* iUserData );
	void maj();
	
};


//************************
  void DialogSubDiv::maj()
  {
    std::cout << ">>> X:" << cSliderPosX->value()  << " Y:" << cSliderPosY->value() << " Z:" << cSliderPosZ->value() << std::endl;
		std::cout << "DialogSubDiv::maj " << this << std::endl;
     
    PP3d::Point3d lCenter( cSliderPosX->value() , 	cSliderPosY->value() ,	cSliderPosZ->value() );

		//    std::cout << " Slider " << cSliderPas->value() << std::endl;
    
    int  lNbPas = cSliderPas->value();
		double  lAngleTotal = cSliderAngle->value();


			PP3d::PolyPtr lShape  = PP3d::Maker::CreatePoly4FromFacet( cMyCanvas->getDataBase().getCurrentLine(), lNbPas, lMatTran, lFlagClose,
																																 lFlagCloseSeg, lFlagCloseSegEnd, lFlagCloseHight, lFlagCloseLow );
			if( lShape != nullptr )
				{
					cMyCanvas->getDataBase().swapCurrentCreation( new PP3d::ObjectPoly( "Revol", lShape ) );  
				}
		}
	
		Application::Instance().redrawAllCanvas3d();
  }

   //----------------------------------------
 
	DialogSubDiv::DialogSubDiv( Canvas3d* pCanvas,  TypeRevol pType )
		:cMyCanvas( pCanvas )
	{
		cMyType = pType;

		int lX = 20;
		int lY= 30;
		int lW = 300;
		int lH = 20;
		int lYStep = 40;

		myWindow = new Fl_Double_Window(500, 511, "Division");
		myWindow->callback((Fl_Callback*)CancelCB, this);

   
		cSliderDepth =  std::unique_ptr<MySlider>(new MySlider(lX+5, lY, lW, lH, "Iterations", SliderCB, this, 0, 10 ));
		cSliderPas->value( 1 );
		lY += lYStep;
	 
 		cSliderSize =  std::unique_ptr<MySlider>(new MySlider(lX+5, lY, lW, lH, "Angle", SliderCB, this, 0.1, 10 ));
		cSliderAngle->value( 1 );
		lY += lYStep;
	 
		cCheckCentralPoint = std::unique_ptr<MyCheckbutton>( new MyCheckbutton( lX, lY, 30,15, "Central Point", CheckCB, this, 0 ));
		cCheckTore->callback((Fl_Callback*)CheckCB, this );
		cCheckTore->value( false );
		lY += lYStep;;


		

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



		lY += lYStep;
			
		{ Fl_Button* o = new Fl_Button(10, lY, 75, 25, "Reset");
			o->callback((Fl_Callback*)ResetCB, this );
		} // Fl_Button* o
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
	
	}
//----------------------------------------

void DialogSubDiv::CancelCB( Fl_Widget*, void* pUserData ) {
 
  DialogSubDiv* lDialog = reinterpret_cast<DialogSubDiv*>(pUserData);
  lDialog->cMyCanvas->getDataBase().cancelCurrentCreation();

	Application::Instance().redrawAllCanvas3d();

  Fl::delete_widget( lDialog->myWindow );
	lDialog->cContinue = false;
}
//----------------------------------------
void DialogSubDiv::ResetCB( Fl_Widget*, void* pUserData ) {
 
  DialogSubDiv* lDialog = reinterpret_cast<DialogSubDiv*>(pUserData);
	//  lDialog->cMyCanvas->getDataBase().cancelCurrentCreation();

	Application::Instance().redrawAllCanvas3d();

  Fl::delete_widget( lDialog->myWindow );
	lDialog->cContinue = false;
}
//----------------------------------------
 void DialogSubDiv::SliderCB( Fl_Widget*, void*pUserData )
 {
	std::cout << "DialogSubDiv::SliderCB " << pUserData << std::endl;

   DialogSubDiv* lDialog = reinterpret_cast<DialogSubDiv*>(pUserData);
   std::cout << "DialogSubDiv::SliderCB " << lDialog->cSliderPas->value() << std::endl;
   lDialog->maj();
 }
//----------------------------------------
void DialogSubDiv::SizeCB( Fl_Widget*, void*pUserData )
{
	std::cout << "DialogSubDiv::SizeCB " << pUserData << std::endl;
  DialogSubDiv* lDialog = reinterpret_cast<DialogSubDiv*>(pUserData);
	// lDialog->cSizeSliderX->value(  atof(wSizeX->value()) );
  lDialog->maj();
}
//----------------------------------------
void DialogSubDiv::SizeSliderCB( Fl_Widget*, void*pUserData )
{
  DialogSubDiv* lDialog = reinterpret_cast<DialogSubDiv*>(pUserData);
  lDialog->maj(); 
}
//----------------------------------------
void DialogSubDiv::CheckCB( Fl_Widget*, void*pUserData )
{
	//	std::cout << "DialogSubDiv::CheckCB " << pUserData << std::endl;
																											 //  MyCheckbutton *lCheck= reinterpret_cast<MyCheckbutton*>(pUserData);
																											 //  (*lCheck->cCallback)(pWidget, lCheck->cUserData);  
																											 
  DialogSubDiv* lDialog = reinterpret_cast<DialogSubDiv*>(pUserData);
  lDialog->maj(); 
}
//----------------------------------------
void DialogSubDiv::OkCB( Fl_Widget*, void*pUserData )
{
  DialogSubDiv* lDialog = reinterpret_cast<DialogSubDiv*>(pUserData);
  lDialog->maj();
	PP3d::Object* lObj = lDialog->cMyCanvas->getDataBase().validCurrentCreation();
	if( lObj != nullptr )
		{
			lObj->rename(  "Polyedre : Division"  );
		}


	Application::Instance().redrawAllCanvas3d();
	Application::Instance().redrawObjectTree();

  Fl::delete_widget(lDialog->myWindow);  // Normly if I am understand the documentation, it will destroy all the children
	lDialog->cContinue = false;
}
//************************
};


//************************
extern void CallDialogSubDiv( bool& pFlagAlreadyExist, M3d::Canvas3d* pCanvas, TypeRevol pType )
{
	// Il faudrait envoyer un ptr sur une variable de type DialogPrimitv
	// Que l'on remplirais et qui servirais de flag, en meme temps
	// on pourrait faire un show dessus!
	
	if( pFlagAlreadyExist == true )
		return ;
	
	pFlagAlreadyExist = true;
	M3d::DialogSubDiv( pCanvas, pType );
	pFlagAlreadyExist = false;
}
//************************

