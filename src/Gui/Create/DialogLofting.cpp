
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

#include "Gui/MySlider.h"

#include "Creation.h"

#include "Shape/Object.h"
#include "Shape/Maker.h"

#include "Modif/Modif.h"

#include "Utils.h"


//Tout mettre dans le Dialogue

using namespace std;
using namespace PP3d;

namespace M3d {


#define Diag M3d::DialogLofting::Instance()
 
  //************************
  class DialogLofting  : public virtual PPSingletonCrtp<DialogLofting>{
    TypeOfInput  cMyTypeInput;
    
    Fl_Double_Window* cMyWindow=nullptr;

    MyCheckbutton* cCheckJoint;

       
    MySlider* cSliderCenterPosX= nullptr;
    MySlider* cSliderCenterPosY= nullptr;
    MySlider* cSliderCenterPosZ= nullptr;

    
    
    MySlider* cSliderGrow= nullptr;
    MyCheckbutton* cCheckGrowX;
    MyCheckbutton* cCheckGrowY;   
    MyCheckbutton* cCheckGrowZ;
    bool cFlagGrowX = true;
    bool cFlagGrowY = true;
    bool cFlagGrowZ = true;

    MyCheckbutton* cCheckCloseSeg ; 
    MyCheckbutton* cCheckCloseSegEnd;

      public:
    DialogLofting() {;}
    bool isAlreadyRunning() { return Diag.cMyWindow != nullptr; }

   //----------------------------------------
    void init( TypeOfInput iTypeInput )
    {
             PushHistory();

      std::cout << "*********************************** init  **************************" << std::endl;
      cMyTypeInput = iTypeInput;

      int lX = 20;
      int lY= 30;
      int lW = 300;
      int lH = 20;
      int lYStep = 40;
      int lXStep = 200;
      int lMemX=0;

      cMyWindow = new Fl_Double_Window(500, 800, "Lofting");
      cMyWindow->callback((Fl_Callback*)CancelCB, this);

      cCheckJoint= new MyCheckbutton( lX+50, lY, 60, 15, "Joint", CheckCB, this, 0 );
      cCheckJoint->callback((Fl_Callback*)CheckCB, this );
      cCheckJoint->value( true );
      lY += lYStep; 
 	 
      cSliderGrow =  new MySlider(lX+5, lY, lW, lH, "Grow each turn", SliderCB, this, 0, 5 );
      cSliderGrow->value( 0 );

     lY += lYStep; 
      cCheckGrowX= new MyCheckbutton( lX+50, lY, 30,15, "X", CheckCB, this, 0 );
      cCheckGrowX->callback((Fl_Callback*)CheckCB, this );
      cCheckGrowX->value( cFlagGrowX );
      
      cCheckGrowY= new MyCheckbutton( lX+100, lY, 30,15, "Y", CheckCB, this, 0 );
      cCheckGrowY->callback((Fl_Callback*)CheckCB, this );
      cCheckGrowY->value( cFlagGrowY );
      
      cCheckGrowZ= new MyCheckbutton( lX+150, lY, 30,15, "Z", CheckCB, this, 0 );
      cCheckGrowZ->callback((Fl_Callback*)CheckCB, this );
      cCheckGrowZ->value( cFlagGrowZ );
       

      {
        Fl_Group* o = new Fl_Group(lX, lY, lW+20, lH*7, "Position:");
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
    }
    
  protected:   

    
    //********************************************************************************
    // Calling when command's interface change
    
    void maj()
    {     
      std::cout << "DialogLofting::maj0 " <<std::endl;
         
   
      if( TheInput.getNbCurrentPoints() <  2 || TheInput.getCurrentLine() == nullptr
          || TheSelect.getSelectType() != PP3d::SelectType::Facet
          || TheSelect.getNbSelected() < 1
          || TheSelect.getFirst()->getType() != PP3d::ShapeType::Facet )          
        {          
          return;
        }
      


      bool lFlagJoint = (cCheckJoint->value() != 0 );
      
      PP3d::Point3d lCenter;

      
      PP3d::FacetPtrVect lNewFacets;
      
      if( PP3d::Modif::LoftingFacFromPath( &TheBase, (PP3d::FacetPtr)TheSelect.getFirst(), TheInput.getCurrentLine(), lNewFacets, lFlagJoint ) )
        {                 
          PP3d::PolyPtr lShape = TheBase.getNewPoly();          
          lShape->addFacet(lNewFacets);
          
          PP3d::ObjectPoly* lObjPoly =  new PP3d::ObjectPoly(  "Lofting", lShape );
          
       PushHistory();
         
          std::cout << "====== swapCurrentCreation :" << lObjPoly << std::endl;
          TheInput.swapCurrentCreation( lObjPoly );  
       }   
      
      TheCreat.redrawAllCanvas3d(PP3d::Compute::FacetAll);
      
    }
    //----------------------------------------

    static void CancelCB( Fl_Widget*, void* pUserData ) {
 
      DialogLofting* lDialog = reinterpret_cast<DialogLofting*>(pUserData);

      
      //=============================
      if( lDialog->cMyTypeInput == TypeOfInput::INPUT_ENTRY )
        {        
          TheInput.cancelCurrentCreation();
        }
      else if( lDialog->cMyTypeInput == TypeOfInput::INPUT_OBJECT )
        {
          TheCreat.clearDatabaseTmp();
        }
      
      //=============================
   

      TheCreat.redrawAllCanvas3d( PP3d::Compute::FacetAll );

      Fl::delete_widget( lDialog->cMyWindow );
      Diag.cMyWindow = nullptr;
    }
    //----------------------------------------
    static void SliderCB( Fl_Widget*, void*pUserData )
    {
      std::cout << "DialogLofting::SliderCB " << pUserData << std::endl;

      DialogLofting* lDialog = reinterpret_cast<DialogLofting*>(pUserData);
      lDialog->maj();
    }
    //----------------------------------------
    static  void SizeCB( Fl_Widget*, void*pUserData )
    {
      std::cout << "DialogLofting::SizeCB " << pUserData << std::endl;
      DialogLofting* lDialog = reinterpret_cast<DialogLofting*>(pUserData);
      lDialog->maj();
    }
    //----------------------------------------
    static void SizeSliderCB( Fl_Widget*, void*pUserData )
    {
      DialogLofting* lDialog = reinterpret_cast<DialogLofting*>(pUserData);
      lDialog->maj(); 
    }
    //----------------------------------------
    static void CheckCB( Fl_Widget*, void*pUserData )
    {
      DialogLofting* lDialog = reinterpret_cast<DialogLofting*>(pUserData);
      lDialog->maj(); 
    }
    //----------------------------------------
    static void OkCB( Fl_Widget*, void*pUserData )
    {
      DialogLofting* lDialog = reinterpret_cast<DialogLofting*>(pUserData);
      lDialog->maj();

      //=============================
      if(  lDialog->cMyTypeInput == TypeOfInput::INPUT_ENTRY )
        {
          PP3d::Object* lObj = TheInput.validCurrentCreation(TheBase);
   
          if( lObj != nullptr )
            {
              lObj->rename(  "Lofting"  );
            }
        }
      else
        if(  lDialog->cMyTypeInput == TypeOfInput::INPUT_OBJECT )
          {
            TheCreat.validateDatabaseTmp();
          }
      //=============================

          
      PushHistory();
      

      TheCreat.redrawAll(PP3d::Compute::FacetAll);

          
      Fl::delete_widget(lDialog->cMyWindow);  // Normly if I am understand the documentation, it will destroy all the children
      Diag.cMyWindow = nullptr;
    }
    //************************


    
  };
} // namespace M3d

//************************
extern void CallDialogLofting( TypeOfInput iTypeInput )
{
  cout << "CallDialogLofting " <<  endl;
  
  if( Diag.isAlreadyRunning() == false )
    {
      Diag.init( iTypeInput  );     
    }
}
//************************


