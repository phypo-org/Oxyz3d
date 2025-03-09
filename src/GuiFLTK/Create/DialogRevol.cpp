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

#include "GuiFLTK/MySlider.h"

#include "Creation.h"

#include "Shape/Object.h"
#include "Shape/Maker.h"


using namespace PP3d;
//Tout mettre dans le Dialogue

using namespace std;

namespace M3d {


#define Diag M3d::DialogRevol::Instance()
 
  //************************
  class DialogRevol  : public virtual PPSingletonCrtp<DialogRevol>{

    TypeRevol cMyType;

    Fl_Double_Window* cMyWindow=nullptr;

    MySliderInt* cSliderPas= nullptr;
    MySlider* cSliderAngle= nullptr;
	
	
    MySlider* cSliderPosX= nullptr;
    MySlider* cSliderPosY= nullptr;
    MySlider* cSliderPosZ= nullptr;
	
    MyCheckbutton* cCheckTore= nullptr; 
    MyCheckbutton* cCheckSeal= nullptr;


    MyCheckbutton  * cCheckGear     = nullptr; 
    MySliderFloat  * cGearThickness = nullptr; 
    MySliderInt    * cGearFreq1 = nullptr; 
    MySliderInt    * cGearFreq2 = nullptr; 

    //	MyCheckbutton* cCheckCloseLow; 
    //	MyCheckbutton* cCheckCloseSeg;

	
  public:
    DialogRevol() {;}
    bool isAlreadyRunning() { return Diag.cMyWindow != nullptr; }



    //----------------------------------------

    void init(  TypeRevol pType )
    {
       std::cout << "*********************************** DialogRevol::init  **************************" << std::endl;
     cMyType = pType;
 
 

      int lW = 300;
      int lH = 20;
      
      int lYStep = lH*2;
      
      int lX = 20; 
      
      int lX_G0 = lX/2;
      int lW_G0 = lW*2.5 ; // +lX*5;
      int lX_G1 = lX;
      int lX_SL = lX+lX/4;
      int lX2   = lX + lW+ lX*3;
      int lX1   = lX + lW;
      int lX3   = lX2 + lW+ lX*3;

      int lY = 20;

      int lMul = 14;
      
      cMyWindow = new Fl_Double_Window( lW_G0+lX*2,  lYStep*lMul, "Revolution");
      cMyWindow->callback((Fl_Callback*)CancelCB, this);

   
      cSliderPas =  new MySliderInt(lX+5, lY, lW, lH, "Iterations", MajCB, this, 1, 360 );
      cSliderPas->value( 4 );
      lY += lYStep;
	 
      cSliderAngle =  new MySliderFloat(lX+5, lY, lW, lH, "Angle", MajCB, this, 0.1, 360 );
      cSliderAngle->value( 360 );
      lY += lYStep;
	 
      cCheckTore = new MyCheckbutton( lX, lY, 30,15, "Tore", MajCB, this, 0 );
      cCheckTore->callback((Fl_Callback*)MajCB, this );
      cCheckTore->value( false );
      lY += lYStep;;
      cCheckSeal = new MyCheckbutton( lX, lY, 30,15, "Seal", MajCB, this, 0 );
      cCheckSeal->callback((Fl_Callback*)MajCB, this );

      cCheckSeal->value( false );
      lY += lYStep;;

      
      cCheckGear    = new MyCheckbutton( lX, lY, 30,15, "Gear", MajCB, this, 0 ); 

      cGearThickness = new MySliderFloat( lX2, lY, lW, lH, "Gear Thickness", MajCB, this, 0.0001, 100 );
      cGearThickness->value( 0.5 );
      lY += lYStep;
	   	              
      cGearFreq1 = new MySliderInt( lX, lY, 200, lH, "Freq1", MajCB, this, 1, 100 );
      cGearFreq1->value( 1 );
      cGearFreq2 = new MySliderInt( lX2, lY, 200, lH, "Freq2", MajCB, this, 1, 100 );
      cGearFreq2->value( 1 );
      lY += lYStep;
      lY += lYStep;
 
      
      if( cMyType != TypeRevol::RevolAxis )	
      {  Fl_Group* o = new Fl_Group(lX, lY, lW+20, lH*7, "Position:");
	o->box(FL_ENGRAVED_FRAME);
	lY += lYStep; 

	o->align(Fl_Align(FL_ALIGN_TOP_LEFT));
      
	cSliderPosX = new MySliderFloat(lX+5, lY, lW, lH, "X", MajCB, this, -100, 100 );
	cSliderPosX->value( 0 );
	lY += lYStep;
 
	cSliderPosY = new MySliderFloat(lX+5, lY, lW, lH, "Y", MajCB, this, -100, 100 );
	cSliderPosY->value( 0 );
	lY += lYStep;
 
	cSliderPosZ = new MySliderFloat(lX+5, lY, lW, lH, "Z", MajCB, this, -100, 100 );
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
	 

      cMyWindow->end();
		
      cMyWindow->show( 0, nullptr);
	
      std::cout << "*********************************** fin init **************************" << std::endl;
    
     maj();
      
      //      while( Fl::wait() && MyDiag.cMyWindow != nullptr );

    }

    
  protected:   

    //************************
    void maj()   {
      std::cout << "DialogRevol::maj " << this << " G:" << cCheckGear->cUserData1 << std::endl;
      
     
      PP3d::Point3d lCenter;

      
      if( cMyType == TypeRevol::RevolAxis )
	{
	  PP3d::Point3d lPtZero;
	  PP3d::Point3d lAxis;
	  if( TheCreat.getAxis( lPtZero, lAxis ))
	    {
	      lCenter = lPtZero;	      
	    }
	}
      else
	{
	  std::cout << ">>> X:" << cSliderPosX->value()  << " Y:" << cSliderPosY->value() << " Z:" << cSliderPosZ->value() << std::endl;
	  lCenter.set( cSliderPosX->value() , 	cSliderPosY->value() ,	cSliderPosZ->value() );
	}

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
    
      
      /*      if( cCheckGear && cCheckGear->value() )
        {
          // std::cout << "maj::Gear !" << std::endl;
            {
              lParam.cTop        -= lParam.cParam2;
              lParam.cBottom     -= lParam.cParam2;
            }
        }
      */
 		

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

        
        Point3d lTmpPt;					
	PP3d::Mat4 lMatRot;
	switch( cMyType )
	  {
	  case TypeRevol::RevolX :
	    lMatRot.initRotX( -M_PI*lAngle/180 );
            lTmpPt.z() =cGearThickness->value();
	    break;
									
	  case TypeRevol::RevolY :
	    lMatRot.initRotY( -M_PI*lAngle/180 );
            lTmpPt.z() =cGearThickness->value();
	    break;
					
	  case TypeRevol::RevolZ :
	    lMatRot.initRotZ( -M_PI*lAngle/180 );
            lTmpPt.x() =cGearThickness->value();
	    break;

	  case TypeRevol::RevolAxis :
	    {
	      PP3d::Point3d lPtZero;
	      PP3d::Point3d lAxis;
	      if( TheCreat.getAxis( lPtZero, lAxis ))
		{
		  lAxis -= lPtZero;
		  lAxis.normalize();
		  cout << "Angle:" << -M_PI*lAngle/180.0 << endl;
		  lMatRot.initRotAxis( lAxis,  -M_PI*lAngle/180.0 );
		}
              
              lTmpPt = lAxis * cGearThickness->value();
	    }
	    break;
	  
					
	  default:;
	  }					
			
	PP3d::Mat4 lMatTran = lMatRecenter * lMatRot *  lMatZero;					
      

	PP3d::PolyPtr lShape  = PP3d::Maker::CreatePoly4FromFacet( TheInput.getCurrentLine(), lNbPas, lMatTran,
                                                                   lFlagClose       ? CloseRevol::Yes  : CloseRevol::No,
								   lFlagCloseSeg    ? CloseSeg::Yes    : CloseSeg::No,
                                                                   lFlagCloseSegEnd ? CloseSegEnd::Yes : CloseSegEnd::No,
                                                                   lFlagCloseHight  ? CloseHight::Yes  : CloseHight::No,
                                                                   lFlagCloseLow    ? CloseLow::Yes    : CloseLow::No,
                                                                   WithGrid::No,
                                                                   (cCheckGear->value() ?  &lTmpPt : nullptr),
                                                                    cGearFreq1->value(), cGearFreq2->value() );
        
	if( lShape != nullptr )
	  {
	    TheInput.swapCurrentCreation( new PP3d::Object( "Revol", PP3d::ObjectType::ObjPolyline, lShape ) );  
	  }
      }
	
      TheCreat.redrawAllCanvas3d(PP3d::Compute::FacetAll);
    }

    //----------------------------------------
    static void MajCB( Fl_Widget*, void*pUserData )
    {
      Diag.maj();
    }
   //----------------------------------------
    static void CancelCB( Fl_Widget*, void* pUserData ) {
      
       std::cout << "CANCEL" << std::endl;

      TheInput.cancelCurrentCreation();

      TheCreat.redrawAllCanvas3d( PP3d::Compute::FacetAll );

      Fl::delete_widget( Diag.cMyWindow );
      Diag.cMyWindow = nullptr;
    }
    //----------------------------------------
    static void OkCB( Fl_Widget*, void*pUserData )
    {
      Diag.maj();
      PP3d::Object* lObj = TheInput.validCurrentCreation(TheBase);
      if( lObj != nullptr )
	{
	  lObj->rename(  "Revol"  );
	}
    
      PushHistory();


      TheCreat.redrawAll(PP3d::Compute::FacetAll);


      Fl::delete_widget(Diag.cMyWindow);  // Normly if I am understand the documentation, it will destroy all the children
      Diag.cMyWindow = nullptr;
    }
    //************************
  };
} // namespace M3d

//************************
extern void CallDialogRevol( TypeRevol pType )
{
  cout << "CallDialogRevol" << endl;
  
  if( Diag.isAlreadyRunning() == false )
    {
      Diag.init(pType);
      std::cout << "*************** After init **************" << std::endl;
    }
}
//************************

