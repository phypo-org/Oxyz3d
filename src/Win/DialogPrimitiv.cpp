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
#include <FL/Fl_Spinner.H>
#include <FL/Fl_Counter.H>
#include <FL/Fl_Roller.H>
#include <FL/Fl_Adjuster.H>


#include <sstream>
#include <stdlib.h>

#include <stdlib.h>
#include <cmath>

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
    std::string  cName;

    Fl_Group *      cGroupExt         = nullptr;
    MySlider *      cSliderU          = nullptr;
    MySlider *      cSliderV          = nullptr;
    MySlider *      cSliderW          = nullptr;
    MySlider *      cSliderWidth      = nullptr;
    MySlider *      cSliderHeight     = nullptr;
    MySlider *      cSliderTop        = nullptr;
    MySlider *      cSliderBottom     = nullptr;
    MySlider *      cSliderThickness  = nullptr;
    MyCheckbutton * cCheckInvRotation = nullptr; 
    MyCheckbutton * cCheckDouble      = nullptr; 
    MyCheckbutton * cCheckHole        = nullptr; 

    
    
    MySlider * cSliderSize;
	
    MySlider * cSliderPosX;
    MySlider * cSliderPosY;
    MySlider * cSliderPosZ;

    MySlider * cSliderRotX;
    MySlider * cSliderRotY;
    MySlider * cSliderRotZ;

    Transform cCurrentTransform;
    

  public:
    
    bool isAlreadyRunning() { return Diag.cMyWindow != nullptr; }

    //************************
    void maj()
    {
      PP3d::PrimitivParam lParam;

      ///// MARCHE MAL - pas prevu dans FLTK ?      if( cGroupExt )        cGroupExt->label( cName.c_str() );

      if( cSliderU )         lParam.cNbU       = cSliderU->value();
      if( cSliderV )         lParam.cNbV       = cSliderV->value();
      if( cSliderW )         lParam.cNbW       = cSliderW->value();
      if( cSliderHeight )    lParam.cHeight    = cSliderHeight->value();
      if( cSliderWidth )     lParam.cWidth     = cSliderWidth->value();
      if( cSliderTop )       lParam.cTop       = cSliderTop->value();
      if( cSliderBottom )    lParam.cBottom    = cSliderBottom->value();
      if( cSliderThickness ) lParam.cThickness = cSliderThickness->value();


      if( cCheckInvRotation) lParam.cCheckInvRot = (cCheckInvRotation->value() != 0 );;
      if( cCheckDouble)      lParam.cCheckDouble = (cCheckDouble->value() != 0 );
      if( cCheckHole)        lParam.cCheckHole   = (cCheckHole->value() != 0 );

      
      
      //======================
      
      std::cout << " X:" << cSliderPosX->value()  << " Y:" << cSliderPosY->value() << " Z:" << cSliderPosZ->value() << std::endl;
      std::cout << " RX:" << cSliderRotX->value()  << " RY:" << cSliderRotY->value() << " RZ:" << cSliderRotZ->value() << std::endl;
    
      PP3d::Point3d lPos( cSliderPosX->value() , 	cSliderPosY->value() ,	cSliderPosZ->value() );
      PP3d::Point3d lRot( cSliderRotX->value() , 	cSliderRotY->value() ,	cSliderRotZ->value() );

      std::cout << " Slider " << cSliderSize->value() << std::endl;
    
      double  lSz =  cSliderSize->value();
      if( lSz < 1 && lSz > -1  )
	lSz = 1;
      
      if( lSz < -1 )
	lSz = 1.0/-lSz;
    
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

      PP3d::Poly* lShape = PP3d::PrimitivFactory::Create( cMyType, cName, & lParam);

      if( lShape == nullptr )
	{
	  std::cout << "Shape is null" << std::endl;
	  return;
	}
      TheInput.swapCurrentCreation( new PP3d::ObjectPoly( "Primitiv", lShape ) );  

      //  lShape->move(lPos );
      		
      PP3d::Point3d lScale( lSz, lSz, lSz );
      lShape->scale( lScale );

     
      PP3d::Mat4 lMatRot;
      lMatRot.identity();
 
      lMatRot.initRotX( M_PI*cSliderRotX->value()/180.0 );
      lShape->modify( lMatRot );

      lMatRot.identity();
      lMatRot.initRotY( M_PI*cSliderRotY->value()/180.0 );
      lShape->modify( lMatRot );
      
      lMatRot.identity();
      lMatRot.initRotZ( M_PI*cSliderRotZ->value()/180.0 );
      lShape->modify( lMatRot );
		  
     
      PP3d::Mat4 lMatTran;
      lMatTran.identity();
 
      lMatTran.initMove( cSliderPosX->value(), cSliderPosY->value(), cSliderPosZ->value() );
      lShape->modify( lMatTran );

      Application::Instance().redrawAllCanvas(PP3d::Compute::FacetAll);
    }

    //----------------------------------------
 
    void init( PP3d::PrimitivFactory::Type pType )
    {
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

      int lY = 20;

      int lMul = 14;
      
      if( pType == PP3d::PrimitivFactory::Type::FACET_N )
	lMul += 4;
      else if(  pType == PP3d::PrimitivFactory::Type::CYLINDER )
	lMul += 6;
      else if(  pType == PP3d::PrimitivFactory::Type::SPHERE )
	lMul += 6;


      cMyWindow = new Fl_Double_Window( lW_G0+lX*2, lYStep*lMul, "Primitive");
      cMyWindow->callback((Fl_Callback*)CancelCB, this);

      
      // Les reglages de la primitives

	  lY += lYStep;	  
	  //============================================
	  if( pType == PP3d::PrimitivFactory::Type::FACET_N )
	    {
	      cGroupExt = new Fl_Group(lX_G0, lY, lW_G0, lYStep*4,
					 PP3d::PrimitivFactory::GetTypeName(cMyType));
	      cGroupExt->box(FL_ENGRAVED_FRAME);
	      lY += lYStep;
	      
	      cSliderU = new MySlider( lX_SL, lY, lW, lH, "Number of point", MajCB, this, 3, 64 );
	      cSliderU->value( 4 );
	      lY += lYStep;
	      /*
		cSliderV = = new MySlider( lX_SL, lY, lW, lH, "V", MajCB, this, -50, 50 );
		  cSliderV->value( 1 );
		  lY += lYStep;
	      */
	      
	      cCheckInvRotation = new MyCheckbutton( lX, lY, 30,15, "Clockwise", MajCB, this, 0 ); 
	      lY += lYStep;
	      
	      cCheckDouble = new MyCheckbutton( lX, lY, 30, 15, "Two Faces", MajCB, this, 0 ); 
	      lY += lYStep;
	      cGroupExt->end();	      
	    }
	  //============================================
	  else if(  pType == PP3d::PrimitivFactory::Type::CYLINDER )
	    {
	      cGroupExt = new Fl_Group(lX_G0, lY, lW_G0, lYStep*6,
					 PP3d::PrimitivFactory::GetTypeName(cMyType));
	      cGroupExt->box(FL_ENGRAVED_FRAME);
	      lY += lYStep;
	      
	      cSliderU = new MySlider( lX_SL, lY, lW, lH, "Sections", MajCB, this, 3, 128 );
	      cSliderU->value( 16 );
	      lY += lYStep;

	      cSliderHeight = new MySlider( lX_SL, lY, lW, lH, "Height", MajCB, this, 0.00001, 100 );
	      cSliderHeight->value( 1 );
	      lY += lYStep;

	      cSliderTop = new MySlider( lX_SL, lY, lW, lH, "Top radius", MajCB, this, 0, 100 );
	      cSliderTop->value( 1 );
	  
	      cSliderBottom = new MySlider( lX2, lY, lW, lH, "Bottom radius", MajCB, this, 0, 100 );
	      cSliderBottom->value( 1 );
	      lY += lYStep;
	      lY += lYStep;

	      /*
		cSliderV = = new MySlider( lX_SL, lY, lW, lH, "V", MajCB, this, -50, 50 );
		  cSliderV->value( 1 );
		  lY += lYStep;
	      */
	      
	      cCheckHole = new MyCheckbutton( lX, lY, 30,15, "Tube", MajCB, this, 0 ); 
	   
	      cSliderThickness = new MySlider( lX2, lY, lW, lH, "Thickness", MajCB, this, 0.0001, 100 );
	      cSliderThickness->value( 0.1 );
	      lY += lYStep;

	      cGroupExt->end();	      	      
	    }
	  //============================================
	  if( pType == PP3d::PrimitivFactory::Type::SPHERE )
	    {
	      cGroupExt = new Fl_Group(lX_G0, lY, lW_G0, lYStep*6,
					 PP3d::PrimitivFactory::GetTypeName(cMyType));
	      cGroupExt->box(FL_ENGRAVED_FRAME);
	      lY += lYStep;
	      
	      cSliderU = new MySlider( lX_SL, lY, lW, lH, "Sections", MajCB, this, 3, 128 );
	      cSliderU->value( 16 );
	      lY += lYStep;
	      
	      cSliderV = new MySlider( lX_SL, lY, lW, lH, "Slices", MajCB, this, 3, 128 );
	      cSliderV->value( 8 );
	      lY += lYStep;

	      
	      cSliderWidth = new MySlider( lX_SL, lY, lW, lH, "Width", MajCB, this, 0.00001, 50 );
	      cSliderWidth->value( 1 );
	      lY += lYStep;
	      

	      cSliderHeight = new MySlider( lX_SL, lY, lW, lH, "Height", MajCB, this, 0.00001, 50 );
	      cSliderHeight->value( 1 );
	      lY += lYStep;
	 	      
	      cCheckHole = new MyCheckbutton( lX, lY, 30,15, "Tore", MajCB, this, 0 ); 
	   
	      cSliderThickness = new MySlider( lX2, lY, lW, lH, "Size Hole", MajCB, this, 0, 50 );
	      cSliderThickness->value( 1 );
	      lY += lYStep;
	      
	      cGroupExt->end();	      
	    }
	  //============================================
      lY += lYStep;	
   
      // Reglages generaux, position , rotations ... 
      {  Fl_Group* o = new Fl_Group(lX_G0, lY, lW_G0, lYStep*8, "Transformation");
	  o->box(FL_ENGRAVED_FRAME);
	
	lY += lYStep;
	 
      // cSliderSize =  M
      cSliderSize = new MySlider(lX_SL, lY, lW, lH, "Size", MajCB, this, -50, 50 );
      cSliderSize->value( 1 );
      lY += lYStep;
      lY += lYStep;

     	int lMemY = lY;
	{  Fl_Group* o = new Fl_Group(lX +lX_G1, lY, lW+lX, lH*8, "Position:");
	  o->box(FL_ENGRAVED_FRAME);
	  lY += lYStep; 

	  o->align(Fl_Align(FL_ALIGN_TOP_LEFT));
      
	  cSliderPosX = new MySlider(lX + lX_SL, lY, lW, lH, "X", MajCB, this, -100, 100 );
	  cSliderPosX->value( 0 );
	  lY += lYStep;
 
	  cSliderPosY =  new MySlider(lX + lX_SL, lY, lW, lH, "Y", MajCB, this, -100, 100 );
	  cSliderPosY->value( 0 );
	  lY += lYStep;
 
	  cSliderPosZ =  new MySlider(lX + lX_SL, lY, lW, lH, "Z", MajCB, this, -100, 100 );
	  cSliderPosZ->value( 0 );
	  lY += lYStep;

	  o->end();
	} // Fl_Group* o

	lY = lMemY;
	int lMemX = lX;

	//		lY += lYStep;

	{  Fl_Group* o = new Fl_Group(lX2+lX_G1, lY, lW+lX, lH*8, "Rotation:");
	  o->box(FL_ENGRAVED_FRAME);
	  lY += lYStep; 

	  o->align(Fl_Align(FL_ALIGN_TOP_LEFT));
      
	  cSliderRotX = new MySlider(lX2+lX_SL, lY, lW, lH, "X", MajCB, this, -360, 360 );
	  cSliderPosX->value( 0 );
	  lY += lYStep;
 
	  cSliderRotY =  new MySlider(lX2+lX_SL, lY, lW, lH, "Y", MajCB, this, -360, 360 );
	  cSliderPosY->value( 0 );
	  lY += lYStep;
 
	  cSliderRotZ =  new MySlider(lX2+lX_SL, lY, lW, lH, "Z", MajCB, this, -360, 360 );
	  cSliderPosZ->value( 0 );
	  lY += lYStep;

	  /*
	    new Fl_Spinner( LX_SL, lY, lW, lH, "X" );
	    lY += lYStep;
	    new Fl_Counter( LX_SL, lY, lW, lH, "Y" );
	    lY += lYStep;
	    {
	    Fl_Roller * o = new Fl_Roller( LX_SL, lY, lW, lH*4, "Z" );
	    o->type(1);
	    }
	      
	    lY += lYStep*4;
	    new Fl_Adjuster ( LX_SL, lY, lW, lH*4, "RZ" );
	    lY += lYStep*4;
	  */

	
	  o->end();
	} // Fl_Group* o
	
 	  o->end();
     }

      lY += lYStep;
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
    static void MajCB( Fl_Widget*, void*pUserData )
    {
      Diag.maj();
    }
    //----------------------------------------

    static void CancelCB( Fl_Widget*, void* pUserData ) {

      std::cout << "CANCEL" << std::endl;

      TheInput.cancelCurrentCreation();

      TheAppli.redrawAllCanvas(PP3d::Compute::FacetAll);

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
	  lObj->rename( Diag.cName.c_str() );
	}
    
      TheAppli.redrawAll(PP3d::Compute::FacetAll);
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

