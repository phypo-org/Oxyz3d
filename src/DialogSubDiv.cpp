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
    PP3d::SubDiv::GeometryType      cGeoType;
    PP3d::SubDiv::SubNormalizeType  cNormType;

    Fl_Double_Window* myWindow;

    std::unique_ptr<MyChoiceButton> cChoiceDepth;
    std::unique_ptr<MySlider>       cSliderSize;
		
    std::unique_ptr<MyCheckbutton> cCheckCentralPoint;

    std::unique_ptr<MyChoiceButton> cChoiceNormalize;
    std::unique_ptr<MyChoiceButton> cChoiceGeometry;

  

    std::unique_ptr<MySlider> cSliderDepthGrowFactor;
    std::unique_ptr<MySlider> cSliderInitGrowFactor;

    std::unique_ptr<MyChoiceButton> cChoiceHoleFacet;
    std::unique_ptr<MyChoiceButton> cChoiceHoleDepth;
    



    std::unique_ptr<MySlider> cSliderPosX;
    std::unique_ptr<MySlider> cSliderPosY;
    std::unique_ptr<MySlider> cSliderPosZ;

    //	std::unique_ptr<MyCheckbutton> cCheckCloseLow; 
    //	std::unique_ptr<MyCheckbutton> cCheckCloseSeg;

	
  public:
    DialogSubDiv( Canvas3d* iCanvas );
    bool cContinue;

  protected:
    static void CancelCB    ( Fl_Widget*, void* iUserData );
    static void OkCB        ( Fl_Widget*, void* iUserData);
    static void ResetCB     ( Fl_Widget*, void* iUserData);

    static void SizeSliderCB( Fl_Widget*, void* iUserData );
    static void SizeCB      ( Fl_Widget*, void* iUserData );
    static void SliderCB    ( Fl_Widget*, void* iUserData );
    static void CheckCB     ( Fl_Widget*, void* iUserData );
    static void ChoiceCB     ( Fl_Widget*, void* iUserData );
    void maj();
	
  };


  //************************
  void DialogSubDiv::maj()
  {
    std::cout << ">>> X:" << cSliderPosX->value()  << " Y:" << cSliderPosY->value() << " Z:" << cSliderPosZ->value() << std::endl;
    std::cout << "DialogSubDiv::maj " << this << std::endl;
     
    PP3d::Point3d lCenter( cSliderPosX->value() , 	cSliderPosY->value() ,	cSliderPosZ->value() );

		
    std::cout << " Template  :" << cChoiceGeometry->value() << std::endl;
    std::cout << " Normalize :" << cChoiceNormalize->value() << std::endl;

    std::cout << " Depth    :" << cSliderDepth->value() << std::endl;
    std::cout << " Size     :" << cSliderSize->value()   << std::endl;
    std::cout << " Central  :" << (int)(cCheckCentralPoint->value()) << std::endl;


    
    PP3d::SubDiv::GeometryType     lGeoType  = static_cast<PP3d::SubDiv::GeometryType>( cChoiceGeometry ->value() );
    PP3d::SubDiv::SubNormalizeType lNormType = static_cast<PP3d::SubDiv::SubNormalizeType>(  cChoiceNormalize->value() );		
    int   lDepth  =  cChoiceDepth->value();
    float lSize   =  cSliderSize->value() ;
    bool  lCentralPoint = cCheckCentralPoint->value();

    float lDepthGrowFactor = cSliderDepthGrowFactor->value();
    float lInitGrowFactor  = lInitGrowFactor->value();

    int   lHoleFacet  =  cChoiceHoleFacet->value() -1;
    int   lHoleDepth  =  cChoiceHoleDepth->value() -1;

			
    std::cout << "2 Template  :" << (int)(lGeoType) << std::endl;
    std::cout << "2 Normalize :" << (int)(lNormType) << std::endl;

    std::cout << "2 Depth    :"  << lDepth << std::endl;
    std::cout << "2 Size     :"  << lSize  << std::endl;
    
    std::cout << "2 Central  :"  << (int)(lCentralPoint) << std::endl;
    
    std::cout << "2 GrowFactor  :" <<  lDepthGrowFactor<< std::endl;
    std::cout << "2 InitGrowFactor  :" << lInitGrowFactor<< std::endl;

    std::cout << "2 lHoleFacet  :" << lHoleFacet<< std::endl;
    std::cout << "2 lHoleDepth  :" << lHoleDepth<< std::endl;


    PP3d::SubDiv::SubParam lParam( lDepth, lSize, lCentralPoint, lNormType);
    
    lParam.cDepthGrowFactor = lDepthGrowFactor;
    lParam.cInitGrowFactor = lInitGrowFactor;

    lParam.cHoleFacet = lHoleFacet;
    lParam.cHoleDepth = lHoleDepth;
    
    
    PP3d::SubDiv::Create( lGeoType, lParam );
		
    PP3d::Poly* lShape = lParam.finish();
		
    cMyCanvas->getDataBase().swapCurrentCreation( new PP3d::ObjectPoly( "Subdivide", lShape ) );  
				
    lShape->move(lCenter );
		

    Application::Instance().redrawAllCanvas3d();
		
  }
  //---------------------------------------- 
  DialogSubDiv::DialogSubDiv( Canvas3d* pCanvas )
    :cMyCanvas( pCanvas )
  {
    int lX = 20;
    int lY= 30;
    int lW = 300;
    int lH = 20;
    int lYStep = 40;

    myWindow = new Fl_Double_Window(500, 540, "Subdivide object generator");
    myWindow->callback((Fl_Callback*)CancelCB, this);

		

    {  Fl_Group* o = new Fl_Group(lX-5, lY, lW+110, lH*12, "Generator:");
      o->box(FL_ENGRAVED_FRAME);
      lY += lYStep; 

      o->align(Fl_Align(FL_ALIGN_TOP_LEFT));

      cChoiceGeometry =  std::unique_ptr<MyChoiceButton>( new MyChoiceButton(lX+100, lY, lW, lH, "Initial template", ChoiceCB, this ))  ;
      cChoiceGeometry->callback((Fl_Callback*)ChoiceCB, this );
      cChoiceGeometry->add("Parallelepiped");																					
      cChoiceGeometry->add("Cube");																					
      cChoiceGeometry->add("Pyramid4");																					
      cChoiceGeometry->add("Octodron");																					
      cChoiceGeometry->add("Dodecahedron");																					
      cChoiceGeometry->add("Icosahedron");																					
      cChoiceGeometry->add("Tetrahedron");																					
      cChoiceGeometry->add("Odron");
      cChoiceGeometry->value(1);
		
      cChoiceGeometry->tooltip("The original object to be subdivided");
	
      //	cChoiceGeometry->add("");
      lY += lYStep;

      cChoiceNormalize =  std::unique_ptr<MyChoiceButton>( new MyChoiceButton(lX+100, lY, lW, lH, "Normalize", ChoiceCB, this ))  ;
      cChoiceNormalize->callback((Fl_Callback*)ChoiceCB, this );
      cChoiceNormalize->add("None");
      cChoiceNormalize->add("Normalize");
      cChoiceNormalize->add("Only init");
      cChoiceNormalize->add("Inc init");		
      cChoiceNormalize->add("Dec init");
      cChoiceNormalize->add("Only sub");
      cChoiceNormalize->add("Mul sub");
      cChoiceNormalize->add("Dec sub **");
      cChoiceNormalize->add("Inc sub ***");
      cChoiceNormalize->add("Mul init (trou ou pic (GrowFactor)");
      cChoiceGeometry->tooltip("The normalize method use for resize the subdivision");
      cChoiceNormalize->value( 8 );
      //	cChoiceNormalize->add("");
																										
      lY += lYStep;

   
      cChoiceDepth =   std::unique_ptr<MyChoiceButton>( new MyChoiceButton(lX+100, lY, lW, lH, "Depth", ChoiceCB, this ))
	cChoiceDepth->add("0");
	cChoiceDepth->add("1");
	cChoiceDepth->add("2");
	cChoiceDepth->add("3");
	cChoiceDepth->add("4");
	cChoiceDepth->add("5");
	cChoiceDepth->add("6");
	cChoiceDepth->add("7");
	cSliderDepth->value( 1 );
	lY += lYStep;
	 
      cSliderSize =  std::unique_ptr<MySlider>(new MySlider(lX+5, lY, lW, lH, "Size", SliderCB, this, 0.1, 10 ));
      cSliderSize->value( 1 );
      lY += lYStep;
	 
      cCheckCentralPoint = std::unique_ptr<MyCheckbutton>( new MyCheckbutton( lX, lY, 30,15, "Central Point", CheckCB, this, 0 ));
      cCheckCentralPoint->callback((Fl_Callback*)CheckCB, this );
      cCheckCentralPoint->value( false );
      lY += lYStep;;



      cSliderDepthGrowFactor =  std::unique_ptr<MySlider>(new MySlider(lX+5, lY, lW, lH, "Depth grow factor", SliderCB, this, 0, 5 ));
      cSliderDepthGrowFactor->value( 1 );
      lY += lYStep;

      cSliderInitGrowFactor =  std::unique_ptr<MySlider>(new MySlider(lX+5, lY, lW, lH, "Init frow factor", SliderCB, this, 0, 5 ));
      cSliderDepth->value( 0.3 );
      lY += lYStep;

   

      cChoiceHoleFacet =   std::unique_ptr<MyChoiceButton>( new MyChoiceButton(lX+100, lY, lW, lH, "Hole facet", ChoiceCB, this ))
	cChoiceHoleFacet->add("no");
	cChoiceHoleFacet->add("0");
	cChoiceHoleFacet->add("1");
	cChoiceHoleFacet->add("2");
	cChoiceHoleFacet->add("3");
	cChoiceHoleFacet->add("4");
	cChoiceHoleFacet->add("5");
	cChoiceHoleFacet->value( 0 );
	lY += lYStep;
   

      cChoiceHoleDepth=   std::unique_ptr<MyChoiceButton>( new MyChoiceButton(lX+100, lY, lW, lH, "Hole facet", ChoiceCB, this ))
	cChoiceHoleFacet->add("no");
	cChoiceHoleFacet->add("0");
	cChoiceHoleFacet->add("1");
	cChoiceHoleFacet->add("2");
	cChoiceHoleFacet->add("3");
	cChoiceHoleFacet->add("4");
	cChoiceHoleFacet->add("5");
	cChoiceHoleDepth->value( 0 );
	lY += lYStep;

      
      o->end();
    } // Fl_Group* o

    lY += lYStep;


		

    /*
      {  Fl_Group* o = new Fl_Group(lX-5, lY, lW+25, lH*8, "Position:");
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

      */

    lY += lYStep;
			


    { Fl_Button* o = new Fl_Button(125, lY, 75, 25, "OK");
      o->callback((Fl_Callback*)OkCB, this );
    } // Fl_Button* o
    { Fl_Button* o = new Fl_Button(210, lY, 75, 25, "Cancel");
      o->callback((Fl_Callback*)CancelCB, this );
    } // Fl_Button* o
    { Fl_Button* o = new Fl_Button(400, lY, 75, 25, "Reset");
      o->callback((Fl_Callback*)ResetCB, this );
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
  void DialogSubDiv::SliderCB( Fl_Widget*, void*pUserData )
  {
    std::cout << "DialogSubDiv::SliderCB " << pUserData << std::endl;

    DialogSubDiv* lDialog = reinterpret_cast<DialogSubDiv*>(pUserData);
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
  void DialogSubDiv::ChoiceCB( Fl_Widget*, void*pUserData )
  {
    //	std::cout << "DialogSubDiv::CheckCB " << pUserData << std::endl;
    //  MyCheckbutton *lCheck= reinterpret_cast<MyCheckbutton*>(pUserData);
    //  (*lCheck->cCallback)(pWidget, lCheck->cUserData);  
																											 
    DialogSubDiv* lDialog = reinterpret_cast<DialogSubDiv*>(pUserData);
    lDialog->maj(); 
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
 
    //  DialogSubDiv* lDialog = reinterpret_cast<DialogSubDiv*>(pUserData);
    //  lDialog->cMyCanvas->getDataBase().cancelCurrentCreation();

    Application::Instance().redrawAllCanvas3d();
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
} // fin namespace


//************************
extern void CallDialogSubDiv( bool& pFlagAlreadyExist, M3d::Canvas3d* iCanvas)
{
  if( pFlagAlreadyExist == true )
    return ;
	
  pFlagAlreadyExist = true;

	
  M3d::DialogSubDiv Diag( iCanvas );
	
  pFlagAlreadyExist = false;
}
//************************

