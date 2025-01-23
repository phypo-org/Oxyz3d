
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
      
      if( cMyTypeInput == TypeOfInput::INPUT_ENTRY )
        {
          cCheckJoin= new MyCheckbutton( lX, lY, 60, 15, "Join facets", CheckCB, this, 0 );
          cCheckJoin->callback((Fl_Callback*)CheckCB, this );
          cCheckJoin->value( true );
          lX += lXStep/2;          
      
          cCheckClose= new MyCheckbutton( lX, lY, 60, 15, "Close extremities", CheckCB, this, 0 );
          cCheckClose->callback((Fl_Callback*)CheckCB, this );
          cCheckClose->value( true );
          lX += lXStep;
     
        }
          cCheckAlign= new MyCheckbutton( lX, lY, 200, 15, "Align facets normal to path", CheckCB, this, 0 );
          cCheckAlign->callback((Fl_Callback*)CheckCB, this );
          cCheckAlign->value( true );
          lY += lYStep;
          lY += lYStep;
          lX = lMemX;
          
      if( cMyTypeInput == TypeOfInput::INPUT_ENTRY )
        {
      
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
        }

      //-----------  Localisation  ------------
      
   
      
      cSliderInter =  new MySliderInt(lX+5, lY, lW, lH, "Interpolate", SliderCB, this, 0, 64 );
      cSliderInter->value( 0 );
      lY += lYStep;
    
 	 
      cSliderSpin =  new MySliderFloat(lX+5, lY, lW, lH, "Spin each step", SliderCB, this, -120, 120 );
      cSliderSpin->value( 0 );
      lY += lYStep;
      
      cSliderGrow =  new MySliderFloat(lX+5, lY, lW, lH, "Grow at each step", SliderCB, this, 0, 10 );
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
         
   
      if( cMyTypeInput == TypeOfInput::INPUT_ENTRY
          && (TheInput.getNbCurrentPoints() <  2 || TheInput.getCurrentLine() == nullptr
              || TheSelect.getSelectType() != PP3d::SelectType::Facet
              || TheSelect.getNbSelected() < 1
              || TheSelect.getFirst()->getType() != PP3d::ShapeType::Facet ))
        {          
          return;
        }
      
      PP3d::Modif::ParamLoftingFacPath lParam;

      if( cMyTypeInput == TypeOfInput::INPUT_ENTRY )
        {
          lParam.cFlagJoin   = (cCheckJoin->value() != 0 );
          lParam.cFlagClose  = (cCheckClose->value() != 0 );
          lParam.cLocalisation = cLocalisation;
        }
      
      lParam.cFlagAlign  = (cCheckAlign->value() != 0 );
          //     std::cout << "========== join:" << lParam.cFlagJoin << " " << cCheckJoin->value() << std::endl;
      
      lParam.cNbInterpol = cSliderInter->value();
      lParam.cSpin       = -M_PI*cSliderSpin->value()/180.0;
      lParam.cGrow       = cSliderGrow->value();

   
      
      PP3d::Point3d lCenter;

      
      PP3d::FacetPtrVect lNewFacets;

  
      PPAutoPtr<Facet> lPath  = TheInput.getCurrentLine()->duplicate();	 
      ObjBSpline lObjBSpline( "BSplineTmpLofting", lPath, false );

      // ObjectPolylines      * lPath = TheInput.getCurrentLine();
      //========= Interpolation par une BSpline  =========
      if( lParam.cNbInterpol > 0 && TheInput.getNbCurrentPoints() >= 2 )
        {
          lObjBSpline.makePtsFromPoles( lParam.cNbInterpol );
          lPath = lObjBSpline.getSplinePts();
        }
      //========= End Interpolation =========


      if( cMyTypeInput == TypeOfInput::INPUT_ENTRY )
        {  
          PP3d::Modif::LoftingFacFromPath( &TheBase, (PP3d::FacetPtr)TheSelect.getFirst(),
                                           lPath, lNewFacets, lParam );      
     
          if(  lNewFacets.size() > 0 )
            {
              PP3d::PolyPtr lShape = TheBase.getNewPoly();          
              lShape->addFacet(lNewFacets);
         
              PP3d::ObjectPoly* lObjPoly =  new PP3d::ObjectPoly(  "Lofting", lShape );
         
              std::cout << "====== swapCurrentCreation :" << lObjPoly << std::endl;
              TheInput.swapCurrentCreation( lObjPoly );  
            }   
        }
      else    
        if( cMyTypeInput == TypeOfInput::INPUT_OBJECT )
          {
            //======== INPUT_OBJECT =========       
            std::unique_ptr<PP3d::DataBase> luTmpBase( new PP3d::DataBase() );

            // On duplique la selection dans la base temporaire
            //       Utils::DuplicateObject( TheBase,  TheSelect.getSelectionVect(), *luTmpBase );
            std::stringstream  lDupStr0;
            Utils::SaveObjectsInStream( TheBase, false, TheSelect.getSelectionVect(), lDupStr0);



            PP3d::VisitorMinMax lVisitMinMax;
            TheSelect.execVisitorOnlyOnObjects ( lVisitMinMax );
            Point3d  lCenter  =  lVisitMinMax.getCenter();
            
            PP3d::Mat4 lMatRecenter;
            lMatRecenter.initMove( lCenter ); //on positionne en fonction du centre de rotation ;
            PP3d::Point3d lNCenter =  -lCenter;					
            PP3d::Mat4 lMatZero;
            lMatZero.initMove( lNCenter ); //on revient a la postion originale en zero;

			
            // Get all the point of the Path
            PP3d::SortEntityVisitorPoint lVisitPath;
            
            lPath->execVisitor( lVisitPath );         
            GLuint lNbPt =  (GLuint)lVisitPath.cVectPoints.size();
            
            PP3d::Point3d lNormToMove( 0,1,0 );

            
            PP3d::Mat4 lMatSpin;
            lMatSpin.identity();         

            
            std::cout << std::endl <<"Grow:" << lParam.cGrow <<  std::endl << std::endl;
            double lGrow = lParam.cGrow / lNbPt;
            
             std::cout << std::endl <<"Grow:" << lGrow <<  std::endl << std::endl;
              
            for( GLuint i = 0; i<lNbPt; i++)  // Begining at index 1
              {           
                std::stringstream lDupStr( lDupStr0.str() );
                std::vector<PP3d::EntityPtr> lNewObjs;
                Utils::WriteObjectFromStream( lDupStr, *luTmpBase, lNewObjs );                 
                PP3d::VisitorGetPoints<PP3d::PointPtrSet> lVisitPt;
                for( PP3d::EntityPtr lObjPtr : lNewObjs )
                  lObjPtr->execVisitor( lVisitPt );
                
                //====== Move All to the begining of path       
                Point3d lMove = lPath->getPoint(i)->get() - lCenter;

                
                PP3d::Mat4 lMatAlign;
                lMatAlign.identity();

                Point3d lDir;
                if( i == 0 )                  
                  {
                    lDir = lPath->getPoint(0)->get() - lPath->getPoint(1)->get();
                  }
                else
                  {                                                    
                    lDir = lPath->getPoint(i-1)->get() - lPath->getPoint(i)->get();
                  }
                lDir.normalize();
                lMatAlign.rotateAlign( lDir, lNormToMove  ); 
                 
                
                PP3d::Mat4 lMatGrow;                              
                lMatGrow.initScale( 1+(lGrow*i));

                
                PP3d::Mat4 lMatMov;
                lMatMov.initMove( lMove);

                PP3d::Mat4 lMatTran ;
                if( lParam.cFlagAlign )
                  {
                    lMatSpin.initRotAxis( lDir, lParam.cSpin * i );
                    ///                    PP3d::Mat4 lMatTran    = lMatMov * lMatGrow * lMatRot ;  // Oldest
                    lMatTran = lMatRecenter * lMatMov * lMatSpin * lMatAlign  * lMatGrow * lMatZero;
                  }
                else
                  {
                    lMatSpin.initRotAxis( lNormToMove, lParam.cSpin * i );
                    lMatTran = lMatRecenter * lMatMov *  lMatSpin  * lMatGrow * lMatZero;
                  }
                
            
                for(  const PP3d::PointPtr lPoint : lVisitPt.getPoints() )
                  {
                    /*
                      lPoint->get() -= lCenter;    // put at 0,0,0 
                      lPoint->get() *= lMatAlign;      // rotation for alignment
                        
                      lPoint->get() += lCenter;    // move to it's orignal position
                      lPoint->get() += lMove;
                    */
                    lPoint->get() *= lMatTran;                
                  }
              }
            TheCreat.setDatabaseTmp( luTmpBase ); 
          }
      
      //--------------------------------------------------
      
      TheCreat.redrawAllCanvas3d(PP3d::Compute::FacetAll);    
    }
    //----------------------------------------
    
    static void CancelCB( Fl_Widget*, void* pUserData )
    {      
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
