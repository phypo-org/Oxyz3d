
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
#include "Utils/PPAutoPtr.h"


//Tout mettre dans le Dialogue

using namespace std;
using namespace PP3d;
using namespace PPu;

namespace M3d {


#define Diag M3d::DialogLofting::Instance()
 
  //************************
  class DialogLofting  : public virtual PPSingletonCrtp<DialogLofting>{
    TypeOfInput  cMyTypeInput;
    
    Fl_Double_Window * cMyWindow  = nullptr;

    MyCheckbutton *  cCheckJoin   = nullptr;
    MyCheckbutton *  cCheckClose  = nullptr;
    MyCheckbutton *  cCheckAlign  = nullptr;
    

    Fl_Choice     *  cLocalChoice = nullptr;
  
    MySliderInt   * cSliderInter   = nullptr;
    MySlider      * cSliderSpin   = nullptr;
    MySlider      * cSliderGrow   = nullptr;
    Modif::LoftingFacPathLocalisation  cLocalisation=Modif::LoftingFacPathLocalisation::BeginOfPath;

      
    MySlider      *  cSliderCenterPosX   = nullptr;
    MySlider      *  cSliderCenterPosY   = nullptr;
    MySlider      *  cSliderCenterPosZ   = nullptr;

  
    MySlider * cSliderPosX  = nullptr;
    MySlider * cSliderPosY  = nullptr;
    MySlider * cSliderPosZ  = nullptr;

    

  public:
    DialogLofting() {;}
    bool isAlreadyRunning() { return Diag.cMyWindow != nullptr; }

    //----------------------------------------
    void init( TypeOfInput iTypeInput )
    {
      std::cout << "*********************************** DialogLofting Init  **************************" << std::endl;
      
      cMyTypeInput = iTypeInput;

      int lX = 20;
      int lY = 30;
      int lW = 300;
      int lH = 20;
      int lYStep = lH*2;
      int lXStep = 200;
      int lMemX=lX;

      cMyWindow = new Fl_Double_Window( 700, 600, "Lofting");
      cMyWindow->callback((Fl_Callback*)CancelCB, this);
      

      cCheckJoin= new MyCheckbutton( lX, lY, 60, 15, "Join facets", CheckCB, this, 0 );
      cCheckJoin->callback((Fl_Callback*)CheckCB, this );
      cCheckJoin->value( true );
      lX += lXStep/2;
      
      cCheckClose= new MyCheckbutton( lX, lY, 60, 15, "Close extremities", CheckCB, this, 0 );
      cCheckClose->callback((Fl_Callback*)CheckCB, this );
      cCheckClose->value( true );
      lX += lXStep;

      cCheckAlign= new MyCheckbutton( lX, lY, 200, 15, "Align facets normal to path", CheckCB, this, 0 );
      cCheckAlign->callback((Fl_Callback*)CheckCB, this );
      cCheckAlign->value( true );
      lY += lYStep;
      lY += lYStep;
      lX = lMemX;

   
      
      //-----------  Localisation of begin of path  ------------
      cLocalChoice =  new Fl_Choice( lX+200, lY, 150, 25, "Local of generation" );
      cLocalChoice->add( "Begining of path" );
      cLocalChoice->add( "Selected facet(change orientation of path to facet normal)" );
      cLocalChoice->add( "Defined position" ); 
      cLocalChoice->value(0);
            
      cLocalChoice->callback((Fl_Callback *) [](Fl_Widget *w, void *pUserData)
                             {
                               DialogLofting  *lDiagLoft = (DialogLofting*)pUserData;
                               
                               std::string lName = lDiagLoft->cLocalChoice->text();
                               lDiagLoft->cLocalisation = PP3d::Modif::GetLoftingFacPathLocalisation(lName );                               
                             }, this);      
      lY += lYStep;
      lY += lYStep;
      //-----------  Localisation  ------------
      
   
      
  cSliderInter =  new MySliderInt(lX+5, lY, lW, lH, "Interpolate", SliderCB, this, 0, 64 );
  cSliderInter->value( 0 );
  lY += lYStep;
    
 	 
  cSliderSpin =  new MySliderFloat(lX+5, lY, lW, lH, "Spin each step", SliderCB, this, -120, 120 );
  cSliderSpin->value( 0 );
  lY += lYStep;
      
  cSliderGrow =  new MySliderFloat(lX+5, lY, lW, lH, "Grow at each step", SliderCB, this, -5, 5 );
  cSliderGrow->value( 0 );
  lY += lYStep;


      
  lY += lYStep;
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
      
  PP3d::Modif::ParamLoftingFacPath lParam;

  lParam.cFlagJoin   = (cCheckJoin->value() != 0 );
  lParam.cFlagClose  = (cCheckClose->value() != 0 );
  lParam.cFlagAlign  = (cCheckAlign->value() != 0 );

  lParam.cNbInterpol = cSliderInter->value();
  lParam.cSpin        = cSliderSpin->value();
  lParam.cGrow        = cSliderGrow->value();

  lParam.cLocalisation = cLocalisation;

  std::cout << "========== join:" << lParam.cFlagJoin << " " << cCheckJoin->value() << std::endl;
      
  PP3d::Point3d lCenter;

      
  PP3d::FacetPtrVect lNewFacets;

  
  //========= Interpolation par une BSpline  =========
  if( lParam.cNbInterpol > 0 && TheInput.getNbCurrentPoints() >= 2 )
    {
      PPAutoPtr<Facet> lFac      = TheInput.getCurrentLine()->duplicate();	
 
      ObjBSpline lObjBSpline( "BSplineTmpLofting", lFac, false );
      lObjBSpline.makePtsFromPoles( lParam.cNbInterpol );
      
      PP3d::Modif::LoftingFacFromPath( &TheBase, (PP3d::FacetPtr)TheSelect.getFirst(), lObjBSpline.getSplinePts(), lNewFacets, lParam );    }
  //========= End Interpolation =========
  else
    {
      PP3d::Modif::LoftingFacFromPath( &TheBase, (PP3d::FacetPtr)TheSelect.getFirst(), TheInput.getCurrentLine(), lNewFacets, lParam );
    }
    
  if( lNewFacets.size() > 0 )
    {
      PP3d::PolyPtr lShape = TheBase.getNewPoly();          
      lShape->addFacet(lNewFacets);
      
      PP3d::ObjectPoly* lObjPoly =  new PP3d::ObjectPoly(  "Lofting", lShape );
      
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


