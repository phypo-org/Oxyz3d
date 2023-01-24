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

#include "MySlider.h"

#include "Application.h"

#include "Shape/Object.h"
#include "Shape/Maker.h"

#include "Utils.h"

//Tout mettre dans le Dialogue

using namespace std;

namespace M3d {


#define Diag M3d::DialogSpiral::Instance()
 
  //************************
  class DialogSpiral  : public virtual PPSingletonCrtp<DialogSpiral>{

    TypeRevol    cMyTypeRevol;
    TypeOfInput  cMyTypeInput;
    
    Fl_Double_Window* cMyWindow=nullptr;

    MySlider* cSliderPas= nullptr;
    MySlider* cSliderAngle= nullptr;

    MySlider* cSliderMoveX= nullptr;
    MySlider* cSliderMoveY= nullptr;
    MySlider* cSliderMoveZ= nullptr;
    
    MySlider* cSliderGrow= nullptr;
    MySlider* cSliderTurn= nullptr;
	
    MySlider* cSliderSpin= nullptr;


    
    MySlider* cSliderCenterPosX= nullptr;
    MySlider* cSliderCenterPosY= nullptr;
    MySlider* cSliderCenterPosZ= nullptr;


    

    
    MyCheckbutton* cCheckCloseSeg ; 
    MyCheckbutton* cCheckCloseSegEnd; 

    MyCheckbutton* cCheckRotLeft = nullptr;
    MyCheckbutton* cCheckDecrease = nullptr;
    
	
  public:
    DialogSpiral(  ) {;}
    bool isAlreadyRunning() { return Diag.cMyWindow != nullptr; }



    //----------------------------------------

    void init(  TypeRevol iType, TypeOfInput iTypeInput )
    {
      std::cout << "*********************************** init  **************************" << std::endl;
      cMyTypeRevol      = iType;
      cMyTypeInput = iTypeInput;

      int lX = 20;
      int lY= 30;
      int lW = 300;
      int lH = 20;
      int lYStep = 40;

      cMyWindow = new Fl_Double_Window(500, 800, "Spiral");
      cMyWindow->callback((Fl_Callback*)CancelCB, this);

   
      cSliderPas =  new MySlider(lX+5, lY, lW, lH, "Iterations", SliderCB, this, 1, 360 );
      cSliderPas->value( 4 );
      lY += lYStep;
	 
      cSliderAngle =  new MySlider(lX+5, lY, lW, lH, "Angle", SliderCB, this, 0.1, 360 );
      cSliderAngle->value( 360 );
      lY += lYStep;

      
      cSliderMoveX =  new MySlider(lX+5, lY, lW, lH, "Move each turn X", SliderCB, this, -20, 20 );
      cSliderMoveX->value( 0 );
      lY += lYStep;
      cSliderMoveY =  new MySlider(lX+5, lY, lW, lH, "Move each turn Y", SliderCB, this, -20, 20 );
      cSliderMoveY->value( 0 );
      lY += lYStep;
      cSliderMoveZ =  new MySlider(lX+5, lY, lW, lH, "Move each turn Z", SliderCB, this, -20, 20 );
      cSliderMoveZ->value( 0 );
      lY += lYStep;
	 
      cSliderGrow =  new MySlider(lX+5, lY, lW, lH, "Grow each turn", SliderCB, this, 0, 5 );
      cSliderGrow->value( 0 );
      lY += lYStep;

      cSliderTurn =  new MySlider(lX+5, lY, lW, lH, "Number of turn", SliderCB, this, 0, 20 );
      cSliderTurn->value( 0 );
      lY += lYStep;
      
      if( cMyTypeInput == TypeOfInput::INPUT_ENTRY )
        {
          cCheckCloseSeg= new MyCheckbutton( lX, lY, 30,15, "Close U", CheckCB, this, 0 );
          cCheckCloseSeg->callback((Fl_Callback*)CheckCB, this );
          cCheckCloseSeg->value( false );
          lY += lYStep;          
          
          cCheckCloseSegEnd= new MyCheckbutton( lX, lY, 30,15, "Close V", CheckCB, this, 0 );
          cCheckCloseSegEnd->callback((Fl_Callback*)CheckCB, this );
          cCheckCloseSegEnd->value( false );
          lY += lYStep;;
        }
      
      cCheckRotLeft= new MyCheckbutton( lX, lY, 30,15, "Clockwise", CheckCB, this, 0 );
      cCheckRotLeft->callback((Fl_Callback*)CheckCB, this );
      cCheckRotLeft->value( false );
      lY += lYStep;
      
      cCheckDecrease= new MyCheckbutton( lX, lY, 30,15, "Decrease", CheckCB, this, 0 );
      cCheckDecrease->callback((Fl_Callback*)CheckCB, this );
      cCheckDecrease->value( false );
      lY += lYStep;          


      
      cSliderSpin =  new MySlider(lX+5, lY, lW, lH, "Spin", SliderCB, this, 0, 360*10 );
      cSliderSpin->value( 0 );
      lY += lYStep;
    
   
      
      /*
        cCheckTore = new MyCheckbutton( lX, lY, 30,15, "Tore", CheckCB, this, 0 );
        cCheckTore->callback((Fl_Callback*)CheckCB, this );
        cCheckTore->value( false );
        lY += lYStep;;
        cCheckSeal = new MyCheckbutton( lX, lY, 30,15, "Seal", CheckCB, this, 0 );
        cCheckSeal->callback((Fl_Callback*)CheckCB, this );

        cCheckSeal->value( false );
        lY += lYStep;;
      */

      
      if( cMyTypeRevol != TypeRevol::RevolAxis )	
        {  Fl_Group* o = new Fl_Group(lX, lY, lW+20, lH*7, "Position:");
          o->box(FL_ENGRAVED_FRAME);
          lY += lYStep;

          o->align(Fl_Align(FL_ALIGN_TOP_LEFT));
      
          cSliderCenterPosX = new MySlider(lX+5, lY, lW, lH, "Center X", SliderCB, this, -100, 100 );
          cSliderCenterPosX->value( 0 );
          lY += lYStep;
 
          cSliderCenterPosY = new MySlider(lX+5, lY, lW, lH, "Center Y", SliderCB, this, -100, 100 );
          cSliderCenterPosY->value( 0 );
          lY += lYStep;
 
          cSliderCenterPosZ = new MySlider(lX+5, lY, lW, lH, "Center Z", SliderCB, this, -100, 100 );
          cSliderCenterPosZ->value( 0 );
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
	
      std::cout << "*********************************** FIN DIALOGUE **************************" << std::endl;
      maj();
      
      //      while( Fl::wait() && MyDiag.cMyWindow != nullptr );

    }

    
  protected:   

    //********************************************************************************
    // Calling when command's interface change
    
    void maj()
    {
      std::cout << "DialogSpiral::maj " << this << std::endl;
     
      PP3d::Point3d lCenter;

      
      if( cMyTypeRevol == TypeRevol::RevolAxis )
	{
	  PP3d::Point3d lPtZero;
	  PP3d::Point3d lAxis;
	  if( TheAppli.getAxis( lPtZero, lAxis ))
	    {
	      lCenter = lPtZero;	      
	    }
	}
      else
	{
	  std::cout << ">>> X:" << cSliderCenterPosX->value()  << " Y:" << cSliderCenterPosY->value() << " Z:" << cSliderCenterPosZ->value() << std::endl;
	  lCenter.set( cSliderCenterPosX->value() , 	cSliderCenterPosY->value() ,	cSliderCenterPosZ->value() );
	}

      //    std::cout << " Slider " << cSliderPas->value() << std::endl;
    
      int     lNbPas      = (int)cSliderPas->value();
      double  lAngleTotal = cSliderAngle->value();
      
      double  lNbTurn     = cSliderTurn->value();
      double  lSpin       = cSliderSpin->value()/360.0; // for a rotation of one degre;
      double  lGrow       = cSliderGrow->value()/360.0; // for a rotation of one degre
      double  lMoveX      = cSliderMoveX->value()/360.0;
      double  lMoveY      = cSliderMoveY->value()/360.0;
      double  lMoveZ      = cSliderMoveZ->value()/360.0;

    
    
      bool lFlagCloseSeg     = false;        
      bool lFlagCloseSegEnd  = false;
   	
      bool lFlagRotLeft     = false;        
      bool lFlagDecrease    = false;
      if( cMyTypeInput == TypeOfInput::INPUT_ENTRY )
        {
          lFlagCloseSeg = (cCheckCloseSeg->value() != 0 );
          lFlagCloseSegEnd = (cCheckCloseSegEnd->value() != 0 );
        }

      
      lFlagRotLeft  = (cCheckRotLeft->value() != 0 );
      lFlagDecrease = (cCheckDecrease->value() != 0 );

      
      

      
      //	int lNbPas = 30 ; // pour les tests

      if(lNbTurn> 0)
        {
          lNbPas     += lNbPas * lNbTurn;
        }
      lAngleTotal    += lNbTurn * 360;           // adding 360 for each turn
 
      
      double lAngle  = lAngleTotal / lNbPas;
      if( lFlagRotLeft )
        {
          lAngle = -lAngle;
        }

      double lSpinAngle = (lSpin*360*lNbTurn)/ lNbPas;

      lMoveX *= lAngle;  // Move for each rotation
      lMoveY *= lAngle;  // Move for each rotation
      lMoveZ *= lAngle;  // Move for each rotation
      lGrow  *= lAngle;  // Grow for each rotation

      if( lFlagDecrease == false )
        { 
          lGrow  = 1+std::abs(lGrow);
        }
      else
        { 
          lGrow  = 1-std::abs(lGrow);
        }

		
    
      /*  if( lAngle > 360 )
	  lAngle= 360;
      */
  
      {
	std::cout << "Center:" << lCenter ;
	PP3d::Mat4 lMatRecenter;
	lMatRecenter.initMove( lCenter ); //on positionne en fonction du centre de rotation ;
			
			
	PP3d::Point3d lNCenter =  -lCenter;					
	std::cout << " Neg:" << lNCenter << std::endl;
	PP3d::Mat4 lMatZero;
	lMatZero.initMove( lNCenter ); //on revient a la postion originale en zero;

						
	PP3d::Mat4 lMatRot;
        lMatRot.identity();

	PP3d::Mat4 lMatMov;
        lMatMov.identity();
        
	PP3d::Mat4 lMatGrow;
        lMatGrow.identity();

	PP3d::Mat4 lMatSpin;
        lMatSpin.identity();
        
	switch( cMyTypeRevol )
	  {
	  case TypeRevol::RevolX :
            lMatMov.initMove( lMoveX, lMoveY, lMoveZ );
	    lMatRot.initRotX( -M_PI*lAngle/180 );

            lMatSpin.initRotY( -M_PI*lSpinAngle/180 );
  	    break;
		
	  case TypeRevol::RevolY :
            lMatMov.initMove( lMoveX, lMoveY, lMoveZ );
	    lMatRot.initRotY( -M_PI*lAngle/180 );
            
            lMatSpin.initRotX( -M_PI*lSpinAngle/180 );
 	    break;
					
	  case TypeRevol::RevolZ :
            lMatMov.initMove( lMoveX, lMoveY, lMoveZ );
	    lMatRot.initRotZ( -M_PI*lAngle/180 );
            
            lMatSpin.initRotX( -M_PI*lSpinAngle/180 );
  	    break;

	  case TypeRevol::RevolAxis :
	    {
	      PP3d::Point3d lPtZero;
	      PP3d::Point3d lAxis;
	      if( TheAppli.getAxis( lPtZero, lAxis ))
		{
                  lMatMov.initMove( lMoveX, lMoveY, lMoveZ );  // A FAIRE lMove/Norme ???
                  
		  lAxis -= lPtZero;
		  lAxis.normalize();
		  cout << "Angle:" << -M_PI*lAngle/180.0 << endl;
		  lMatRot.initRotAxis( lAxis,  -M_PI*lAngle/180.0 );
		}
	    }
	    break;
	  
					
	  default:;
	  }
        
        if( lGrow != 0 )
          {
            lMatGrow.initScale( lGrow, lGrow, lGrow );
          }


        // A FAIRE sauvegarder les parametere au format json avec un nom !
        // et pouvoir les recuperer dans le dialogue


        // Pour le spin il faut trouver le centre de l'objet (ou des objets)
        // CenterOfObj =
        // decaler vers ce centre  appliqué le spin courant (celon le pas) et remettre les objets en place , il faut calculer la matrice pour cela et faire le calcul avant les autres

        // A FAIRE ...
        

        //=============================
        if( cMyTypeInput == TypeOfInput::INPUT_ENTRY )
          {                    
            PP3d::Mat4 lMatTran = lMatRecenter * lMatMov * lMatGrow * lMatRot *  lMatZero;
            PP3d::PolyPtr lShape  = PP3d::Maker::CreatePoly4FromFacet( TheInput.getCurrentLine(), lNbPas, lMatTran, false,
                                                                       lFlagCloseSeg, lFlagCloseSegEnd, false, false );
            if( lShape != nullptr )
              {
                TheInput.swapCurrentCreation( new PP3d::ObjectPoly( "Spiral", lShape ) );  
              }
          }
        else
          if( cMyTypeInput == TypeOfInput::INPUT_OBJECT )            
            {
              std::cout << ">>>>>>>>SPIRAL OBJECT Select " << TheSelect.getNbSelected() <<":" <<  TheSelect.getSelectionVect().size() << std::endl;
              std::unique_ptr<PP3d::DataBase> luTmpBase( new PP3d::DataBase() );

              // On duplique la selection dans la base temporaire
              //       Utils::DuplicateObject( TheBase,  TheSelect.getSelectionVect(), *luTmpBase );

              
              std::stringstream  lDupStr0;
              Utils::SaveObjectsInStream( TheBase, TheSelect.getSelectionVect(), lDupStr0);

              // On duplique lNbPas les objets en les multipliant par la matrice a chaque fois.
              // Les premiers dupliqué sont multiplié plusieurs fois.
              
              PP3d::Mat4 lMatCurrent = lMatRecenter ;
              PP3d::Mat4 lMatTran    = lMatMov * lMatGrow * lMatRot ;  // Oldest
              // PP3d::Mat4 lMatTran    = lMatRot * lMatGrow * lMatMov  ;
              PP3d::Mat4 lMatTran0;
              
              for( int i=1; i< lNbPas; i++ )
                {
                  std::stringstream lDupStr( lDupStr0.str() );
                  
                  lMatCurrent = lMatCurrent * lMatTran;
                  lMatTran0   = lMatCurrent * lMatZero;

                  //      lDupStr.seekg (0, lDupStr.beg);
                  std::vector<PP3d::EntityPtr> lNewObjs;
                  Utils::WriteObjectFromStream( lDupStr, *luTmpBase, lNewObjs );                 

                  PP3d::VisitorGetPoints<PP3d::PointPtrSet> lVisit;
                  for( PP3d::EntityPtr lObjPtr : lNewObjs )
                       lObjPtr->execVisitor( lVisit );

                  for( PP3d::PointPtr lPoint : lVisit.getPoints() )
                    {
                      lPoint->get() *= lMatTran0;
                    }
               }
              TheAppli.setDatabaseTmp( luTmpBase ); 
              std::cout << "<<<<<<<<<<<<< SPIRAL OBJECT END" << std::endl;
          }
        //=============================
 	
        TheAppli.redrawAllCanvas(PP3d::Compute::FacetAll);
      }
    }

    //----------------------------------------

    static void CancelCB( Fl_Widget*, void* pUserData ) {
 
      DialogSpiral* lDialog = reinterpret_cast<DialogSpiral*>(pUserData);

      
      //=============================
      if( lDialog->cMyTypeInput == TypeOfInput::INPUT_ENTRY )
        {        
          TheInput.cancelCurrentCreation();
        }
      else if( lDialog->cMyTypeInput == TypeOfInput::INPUT_OBJECT )
        {
          TheAppli.clearDatabaseTmp();
        }
      
      //=============================
   

      TheAppli.redrawAllCanvas( PP3d::Compute::FacetAll );

      Fl::delete_widget( lDialog->cMyWindow );
      Diag.cMyWindow = nullptr;
    }
    //----------------------------------------
    static void SliderCB( Fl_Widget*, void*pUserData )
    {
      std::cout << "DialogSpiral::SliderCB " << pUserData << std::endl;

      DialogSpiral* lDialog = reinterpret_cast<DialogSpiral*>(pUserData);
      std::cout << "DialogSpiral::SliderCB " << lDialog->cSliderPas->value() << std::endl;
      lDialog->maj();
    }
    //----------------------------------------
    static  void SizeCB( Fl_Widget*, void*pUserData )
    {
      std::cout << "DialogSpiral::SizeCB " << pUserData << std::endl;
      DialogSpiral* lDialog = reinterpret_cast<DialogSpiral*>(pUserData);
      // lDialog->cSizeSliderX->value(  atof(wSizeX->value()) );
      lDialog->maj();
    }
    //----------------------------------------
    static void SizeSliderCB( Fl_Widget*, void*pUserData )
    {
      DialogSpiral* lDialog = reinterpret_cast<DialogSpiral*>(pUserData);
      lDialog->maj(); 
    }
    //----------------------------------------
    static void CheckCB( Fl_Widget*, void*pUserData )
    {
      //	std::cout << "DialogSpiral::CheckCB " << pUserData << std::endl;
      //  MyCheckbutton *lCheck= reinterpret_cast<MyCheckbutton*>(pUserData);
      //  (*lCheck->cCallback)(pWidget, lCheck->cUserData);  																											 
      DialogSpiral* lDialog = reinterpret_cast<DialogSpiral*>(pUserData);
      lDialog->maj(); 
    }
    //----------------------------------------
    static void OkCB( Fl_Widget*, void*pUserData )
    {
      DialogSpiral* lDialog = reinterpret_cast<DialogSpiral*>(pUserData);
      lDialog->maj();

      //=============================
      if(  lDialog->cMyTypeInput == TypeOfInput::INPUT_ENTRY )
        {
          PP3d::Object* lObj = TheInput.validCurrentCreation(TheBase);
   
          if( lObj != nullptr )
            {
              lObj->rename(  "Revol"  );
            }

        }
      else
        if(  lDialog->cMyTypeInput == TypeOfInput::INPUT_OBJECT )
          {
            TheAppli.validateDatabaseTmp();
          }
      //=============================

          
      PushHistory();
      

      TheAppli.redrawAll(PP3d::Compute::FacetAll);

          
      Fl::delete_widget(lDialog->cMyWindow);  // Normly if I am understand the documentation, it will destroy all the children
      Diag.cMyWindow = nullptr;
    }
    //************************
  };
} // namespace M3d

//************************
extern void CallDialogSpiral( TypeRevol iTypeRevol, TypeOfInput iTypeInput )
{
  cout << "CallDialogSpiral " <<  endl;
  
  if( Diag.isAlreadyRunning() == false )
    {
      Diag.init( iTypeRevol, iTypeInput  );     
      std::cout << "*************** After init **************" << std::endl;
    }
}
//************************

