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


//Tout mettre dans le Dialogue

namespace M3d {

//************************
class DialogRevol {

  Canvas3d *cMyCanvas;
	TypeRevol cMyType;

	Fl_Double_Window* myWindow;

	std::unique_ptr<MySlider> cSliderPas;
	std::unique_ptr<MySlider> cSliderAngle;
	
	
	std::unique_ptr<MySlider> cSliderPosX;
	std::unique_ptr<MySlider> cSliderPosY;
	std::unique_ptr<MySlider> cSliderPosZ;
	
	std::unique_ptr<MyCheckbutton> cCheckTore; 
	std::unique_ptr<MyCheckbutton> cCheckSeal; 
	//	std::unique_ptr<MyCheckbutton> cCheckCloseLow; 
	//	std::unique_ptr<MyCheckbutton> cCheckCloseSeg;

	
public:
	DialogRevol( Canvas3d* pCanvas,  TypeRevol pTypeRevol );
	bool cContinue;

protected:
	static void CancelCB( Fl_Widget*, void* lUserData );
	static void OkCB( Fl_Widget*, void* lUserData);
	static void SizeSliderCB( Fl_Widget*, void*pUserData );
 	static void SizeCB( Fl_Widget*, void*pUserData );
 	static void SliderCB( Fl_Widget*, void*pUserData );
 	static void CheckCB( Fl_Widget*, void*pUserData );
	void maj();
	
};


//************************
  void DialogRevol::maj()
  {
    std::cout << ">>> X:" << cSliderPosX->value()  << " Y:" << cSliderPosY->value() << " Z:" << cSliderPosZ->value() << std::endl;
		std::cout << "DialogRevol::maj " << this << std::endl;
     
    PP3d::Point3d lCenter( cSliderPosX->value() , 	cSliderPosY->value() ,	cSliderPosZ->value() );

		//    std::cout << " Slider " << cSliderPas->value() << std::endl;
    
    int  lNbPas = (int)cSliderPas->value();
    double  lAngleTotal = cSliderAngle->value();
    
    
    
    bool lFlagTore           = (cCheckTore->value() != 0 );
    bool lFlagSeal           = (cCheckSeal->value() != 0 );
    
    
    bool lFlagClose        = false;
    bool lFlagCloseSeg     = false;        
    bool lFlagCloseLow     = false;
    bool lFlagCloseHight   = false;
    bool lFlagCloseSegEnd  = false;
    

		if(lFlagSeal )
			{
				lFlagCloseLow = true;
				lFlagCloseHight = true;
				lFlagClose = true;
			}
		
		if( lAngleTotal == 360 )
			{				
				lFlagClose = true;
			}
			
		if( lFlagTore )
			{
				lFlagCloseSeg = true;
				lFlagCloseLow = false;
				lFlagCloseHight = false;
				
			if( lAngleTotal != 360  && lFlagSeal)
					{
						lFlagCloseSegEnd = true;
						lFlagClose = false;
					}
			}
 
			

		std::cout << "lFlagCloseHight:" << lFlagCloseHight <<  std::endl;

		//	int lNbPas = 30 ; // pour les tests 
    
		

		double lAngle  = lAngleTotal / lNbPas;
		
    
		/*  if( lAngle > 360 )
      lAngle= 360;
		*/
		// Il est TRES IMPORTANT de recreer l'objet a chaque fois
		// car s'il y a des operations dessus (comme la rotation)
		// il y a à chaque fois forcement des erreurs de calculs 
		// (precision limité processeurs) or celle ci s'accumulerais !

		// Sinon on pourrait mettre un rotation avec open gl sur l'objet
		// et faire tourner l'objet 
		// avec, par contre a la fin il faudrait vraiment faire
		// la modification sur les points (on est dans un modeleur, pas un jeu !)

		{
			std::cout << "Center:" << lCenter ;
			PP3d::Mat4 lMatRecenter;
			lMatRecenter.initMove( lCenter ); //on revient au centre;
			
			
			PP3d::Point3d lNCenter =  -lCenter;					
			std::cout << " Neg:" << lNCenter << std::endl;
			PP3d::Mat4 lMatZero;
			lMatZero.initMove( lNCenter ); //on se positionne en zero;

						
			PP3d::Mat4 lMatRot;
			switch( cMyType )
				{
				case TypeRevol::RevolX :
					lMatRot.initRotX( -M_PI*lAngle/180 );
					break;
									
				case TypeRevol::RevolY :
					lMatRot.initRotY( -M_PI*lAngle/180 );
					break;
					
				case TypeRevol::RevolZ :
					lMatRot.initRotZ( -M_PI*lAngle/180 );
					break;
					
				default:;
				}					
			
			PP3d::Mat4 lMatTran = lMatRecenter * lMatRot *  lMatZero;					

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
 
	DialogRevol::DialogRevol( Canvas3d* pCanvas,  TypeRevol pType )
		:cMyCanvas( pCanvas )
	{
		cMyType = pType;

		int lX = 20;
		int lY= 30;
		int lW = 300;
		int lH = 20;
		int lYStep = 40;

		myWindow = new Fl_Double_Window(500, 511, "Revolution");
		myWindow->callback((Fl_Callback*)CancelCB, this);

   
		cSliderPas =  std::unique_ptr<MySlider>(new MySlider(lX+5, lY, lW, lH, "Iterations", SliderCB, this, 1, 360 ));
		cSliderPas->value( 4 );
		lY += lYStep;
	 
 		cSliderAngle =  std::unique_ptr<MySlider>(new MySlider(lX+5, lY, lW, lH, "Angle", SliderCB, this, 0.1, 360 ));
		cSliderAngle->value( 360 );
		lY += lYStep;
	 
		cCheckTore = std::unique_ptr<MyCheckbutton>( new MyCheckbutton( lX, lY, 30,15, "Tore", CheckCB, this, 0 ));
		cCheckTore->callback((Fl_Callback*)CheckCB, this );
		cCheckTore->value( false );
		lY += lYStep;;
		cCheckSeal = std::unique_ptr<MyCheckbutton>( new MyCheckbutton( lX, lY, 30,15, "Seal", CheckCB, this, 0 ));
		cCheckSeal->callback((Fl_Callback*)CheckCB, this );
		cCheckSeal->value( false );
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

void DialogRevol::CancelCB( Fl_Widget*, void* pUserData ) {
 
  DialogRevol* lDialog = reinterpret_cast<DialogRevol*>(pUserData);
  lDialog->cMyCanvas->getDataBase().cancelCurrentCreation();

	Application::Instance().redrawAllCanvas3d();

  Fl::delete_widget( lDialog->myWindow );
	lDialog->cContinue = false;
}
//----------------------------------------
 void DialogRevol::SliderCB( Fl_Widget*, void*pUserData )
 {
	std::cout << "DialogRevol::SliderCB " << pUserData << std::endl;

   DialogRevol* lDialog = reinterpret_cast<DialogRevol*>(pUserData);
   std::cout << "DialogRevol::SliderCB " << lDialog->cSliderPas->value() << std::endl;
   lDialog->maj();
 }
//----------------------------------------
void DialogRevol::SizeCB( Fl_Widget*, void*pUserData )
{
	std::cout << "DialogRevol::SizeCB " << pUserData << std::endl;
  DialogRevol* lDialog = reinterpret_cast<DialogRevol*>(pUserData);
	// lDialog->cSizeSliderX->value(  atof(wSizeX->value()) );
  lDialog->maj();
}
//----------------------------------------
void DialogRevol::SizeSliderCB( Fl_Widget*, void*pUserData )
{
  DialogRevol* lDialog = reinterpret_cast<DialogRevol*>(pUserData);
  lDialog->maj(); 
}
//----------------------------------------
void DialogRevol::CheckCB( Fl_Widget*, void*pUserData )
{
	//	std::cout << "DialogRevol::CheckCB " << pUserData << std::endl;
																											 //  MyCheckbutton *lCheck= reinterpret_cast<MyCheckbutton*>(pUserData);
																											 //  (*lCheck->cCallback)(pWidget, lCheck->cUserData);  
																											 
  DialogRevol* lDialog = reinterpret_cast<DialogRevol*>(pUserData);
  lDialog->maj(); 
}
//----------------------------------------
void DialogRevol::OkCB( Fl_Widget*, void*pUserData )
{
  DialogRevol* lDialog = reinterpret_cast<DialogRevol*>(pUserData);
  lDialog->maj();
	PP3d::Object* lObj = lDialog->cMyCanvas->getDataBase().validCurrentCreation();
	if( lObj != nullptr )
		{
			lObj->rename(  "Revol"  );
		}


	Application::Instance().redrawAllCanvas3d();
	Application::Instance().redrawObjectTree();

  Fl::delete_widget(lDialog->myWindow);  // Normly if I am understand the documentation, it will destroy all the children
	lDialog->cContinue = false;
}
//************************
}


//************************
extern void CallDialogRevol( bool& pFlagAlreadyExist, M3d::Canvas3d* pCanvas, TypeRevol pType )
{
	// Il faudrait envoyer un ptr sur une variable de type DialogPrimitv
	// Que l'on remplirais et qui servirais de flag, en meme temps
	// on pourrait faire un show dessus!
	
	if( pFlagAlreadyExist == true )
		return ;
	
	pFlagAlreadyExist = true;
	M3d::DialogRevol( pCanvas, pType );
	pFlagAlreadyExist = false;
}
//************************

