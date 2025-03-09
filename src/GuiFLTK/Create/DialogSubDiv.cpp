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
#include <FL/Fl_Native_File_Chooser.H>


#include <sstream>
#include <stdlib.h>
#include <fstream>

#include <stdlib.h>
#include <math.h>

#include "GuiFLTK/MySlider.h"

#include "Creation.h"

#include "Shape/Object.h"
#include "Shape/Maker.h"

#include "Shape/SubDiv.h"
#include "Shape/ExportObj.h"


//Tout mettre dans le Dialogue

namespace M3d {
  using namespace PP3d;

  //************************
  class DialogSubDiv {
	
    Canvas3d *cMyCanvas;
    
    PP3d::SubDiv::GeometryType      cGeoType;
    PP3d::SubDiv::SubNormalizeType  cNormType;

    Fl_Double_Window * myWindow;

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

	
  public:
    DialogSubDiv( Canvas3d* iCanvas );
    bool cContinue;

  protected:
    static void CancelCB    ( Fl_Widget*, void* iUserData );
    static void OkCB        ( Fl_Widget*, void* iUserData);
    static void ResetCB     ( Fl_Widget*, void* iUserData);
    static void DirectExpCB ( Fl_Widget*, void* iUserData);

    static void SizeSliderCB( Fl_Widget*, void* iUserData );
    static void SizeCB      ( Fl_Widget*, void* iUserData );
    static void SliderCB    ( Fl_Widget*, void* iUserData );
    static void CheckCB     ( Fl_Widget*, void* iUserData );
    static void ChoiceCB    ( Fl_Widget*, void* iUserData );
    void maj();
  };


  //************************
  void DialogSubDiv::maj()
  {
    /*
      std::cout << ">>> X:" << cSliderPosX->value()  << " Y:" << cSliderPosY->value() << " Z:" << cSliderPosZ->value() << std::endl;
      std::cout << "DialogSubDiv::maj " << this << std::endl;
    */
    
    PP3d::Point3d lCenter( cSliderPosX->value() , 	cSliderPosY->value() ,	cSliderPosZ->value() );
		
    std::cout << " Template  :" << SubDiv::GeometryType2Str((SubDiv::GeometryType)cChoiceGeometry->value()) << std::endl;
    
    
    std::cout << " Normalize :" << SubDiv::SubNormalizeType2Str((SubDiv::SubNormalizeType)cChoiceNormalize->value()) << std::endl;

    std::cout << " Depth    :" << cChoiceDepth->value() << std::endl;
    std::cout << " Size     :" << cSliderSize->value()   << std::endl;
    std::cout << " Central  :" << (int)(cCheckCentralPoint->value()) << std::endl;
    

    
    PP3d::SubDiv::GeometryType     lGeoType  = static_cast<PP3d::SubDiv::GeometryType>( cChoiceGeometry ->value() );
    PP3d::SubDiv::SubNormalizeType lNormType = static_cast<PP3d::SubDiv::SubNormalizeType>(  cChoiceNormalize->value() );
    
    int   lDepth  =  cChoiceDepth->value();
    float lSize   =  cSliderSize->value() ;
    bool  lCentralPoint = cCheckCentralPoint->value();

    float lDepthGrowFactor = cSliderDepthGrowFactor->value();
    float lInitGrowFactor  = cSliderInitGrowFactor->value();

    int   lHoleFacet  =  cChoiceHoleFacet->value() -1;
    int   lHoleDepth  =  cChoiceHoleDepth->value() -1;

    	
    std::cout << "2 Template  :" << SubDiv::GeometryType2Str    (lGeoType) << std::endl;
    std::cout << "2 Normalize :" << SubDiv::SubNormalizeType2Str(lNormType) << std::endl;

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
		
    TheInput.swapCurrentCreation( new PP3d::Object( "Subdivide", ObjectType::ObjPoly, lShape ) );  
				
    lShape->move(lCenter );
		

    Creation::Instance().redrawAllCanvas3d(PP3d::Compute::Nothing);
		
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

    
    myWindow = new Fl_Double_Window(500, 800, "Subdivide object generator");
    myWindow->callback((Fl_Callback*)CancelCB, this);

		

    { Fl_Group* o = new Fl_Group(lX-5, lY, lW+110, lH*30, "Generator:");
      o->box(FL_ENGRAVED_FRAME);
      lY += lYStep; 

      o->align(Fl_Align(FL_ALIGN_TOP_LEFT));

      cChoiceGeometry =  std::unique_ptr<MyChoiceButton>( new MyChoiceButton(lX+100, lY, lW, lH, "Initial template", ChoiceCB, this ))  ;
      cChoiceGeometry->callback((Fl_Callback*)ChoiceCB, this );
      cChoiceGeometry->add("Tetrahedron"); // 0
      cChoiceGeometry->add("Pyramid");    // 1
      cChoiceGeometry->add("Cube");        // 2
      cChoiceGeometry->add("Octahedron");  // 3
      cChoiceGeometry->add("Dodecahedron"); // 4      
      cChoiceGeometry->add("Icosahedron"); //5
      //      cChoiceGeometry->add("Octahedron2"); //6
      //      cChoiceGeometry->add("Odron"); //7

      cChoiceGeometry->value(2);
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
      cChoiceNormalize->add("Half init");
      cChoiceNormalize->add("Only sub");
      cChoiceNormalize->add("Mul sub");
      cChoiceNormalize->add("Dec sub **");
      cChoiceNormalize->add("Inc sub ***");
      cChoiceNormalize->add("Mul init (trou ou pic (GrowFactor)");
      cChoiceNormalize->tooltip("The normalize method use for resize the subdivision");
      cChoiceNormalize->value( 7 );
    					
      lY += lYStep;

   
      cChoiceDepth =   std::unique_ptr<MyChoiceButton>( new MyChoiceButton(lX+100, lY, lW, lH, "Depth", ChoiceCB, this )) ;
      cChoiceDepth->callback((Fl_Callback*)ChoiceCB, this );
      cChoiceDepth->add("0");
      cChoiceDepth->add("1");
      cChoiceDepth->add("2");
      cChoiceDepth->add("3");
      cChoiceDepth->add("4");
      cChoiceDepth->add("5");
      cChoiceDepth->add("6");
      cChoiceDepth->add("7");
      cChoiceDepth->value( 1 );
      lY += lYStep;
	 
      cSliderSize =  std::unique_ptr<MySlider>(new MySlider(lX+5, lY, lW, lH, "Size", SliderCB, this, 0.1, 10 ));
      // cSliderSize->callback((Fl_Callback*)SliderCB, this );
      cSliderSize->value( 1 );
      lY += lYStep;
	 
      cCheckCentralPoint = std::unique_ptr<MyCheckbutton>( new MyCheckbutton( lX, lY, 30,15, "Central Point", CheckCB, this, 0 ));
      cCheckCentralPoint->callback((Fl_Callback*)CheckCB, this );
      cCheckCentralPoint->value( false );
      lY += lYStep;;



      cSliderDepthGrowFactor =  std::unique_ptr<MySlider>(new MySlider(lX+5, lY, lW, lH, "Depth grow factor", SliderCB, this, 0, 5 ));
      //      cSliderDepthGrowFactor->callback((Fl_Callback*)SliderCB, this );
      cSliderDepthGrowFactor->value( 1 );
      lY += lYStep;

      cSliderInitGrowFactor =  std::unique_ptr<MySlider>(new MySlider(lX+5, lY, lW, lH, "Init frow factor", SliderCB, this, 0, 5 ));
      //      cSliderInitGrowFactor->callback((Fl_Callback*)SliderCB, this );
      cSliderInitGrowFactor->value( 0.3f );
      lY += lYStep;
   

      cChoiceHoleFacet = std::unique_ptr<MyChoiceButton>( new MyChoiceButton(lX+100, lY, lW, lH, "Hole facet", ChoiceCB, this ));
      cChoiceHoleFacet->callback((Fl_Callback*)ChoiceCB, this );
      cChoiceHoleFacet->add("no");
      cChoiceHoleFacet->add("0");
      cChoiceHoleFacet->add("1");
      cChoiceHoleFacet->add("2");
      cChoiceHoleFacet->add("3");
      cChoiceHoleFacet->add("4");
      cChoiceHoleFacet->add("5");
      cChoiceHoleFacet->value( 0 );
      lY += lYStep;
   

      cChoiceHoleDepth = std::unique_ptr<MyChoiceButton>( new MyChoiceButton(lX+100, lY, lW, lH, "Hole depth", ChoiceCB, this ));
      cChoiceHoleDepth->callback((Fl_Callback*)ChoiceCB, this );
      cChoiceHoleDepth->add("no");
      cChoiceHoleDepth->add("0");
      cChoiceHoleDepth->add("1");
      cChoiceHoleDepth->add("2");
      cChoiceHoleDepth->add("3");
      cChoiceHoleDepth->add("4");
      cChoiceHoleDepth->add("5");
      cChoiceHoleDepth->value( 0 );
      lY += lYStep;

      
      o->end();
    } // Fl_Group* o

    lY += lYStep;


		

    
    { Fl_Group* o2 = new Fl_Group(lX-5, lY, lW+25, lH*8, "Position:");
      o2->box(FL_ENGRAVED_FRAME);
      lY += lYStep; 

      o2->align(Fl_Align(FL_ALIGN_TOP_LEFT));
      
      cSliderPosX = std::unique_ptr<MySlider>(new MySlider(lX+5, lY, lW, lH, "X", SliderCB, this, -100, 100 ));
      cSliderPosX->value( 0 );
      lY += lYStep;
 
      cSliderPosY =  std::unique_ptr<MySlider>(new MySlider(lX+5, lY, lW, lH, "Y", SliderCB, this, -100, 100 ));
      cSliderPosY->value( 0 );
      lY += lYStep;
 
      cSliderPosZ =  std::unique_ptr<MySlider>(new MySlider(lX+5, lY, lW, lH, "Z", SliderCB, this, -100, 100 ));
      cSliderPosZ->value( 0 );
      lY += lYStep;

      o2->end();
    }// Fl_Group* o

    //		lY += lYStep;

      

    lY += lYStep;
			

    { Fl_Group* o3 = new Fl_Group(lX-5, lY, lW+130, lH*6, "");
      o3->box(FL_ENGRAVED_FRAME);
      lY += lYStep/3;

      { Fl_Button* o = new Fl_Button(lX, lY, 75, 25, "OK");
	o->callback((Fl_Callback*)OkCB, this );
      } // Fl_Button* o
			
      { Fl_Button* o = new Fl_Button(lX+100, lY, 75, 25, "Cancel");
	o->callback((Fl_Callback*)CancelCB, this );
      } // Fl_Button* o
			
      { Fl_Button* o = new Fl_Button(lX+250, lY, 75, 25, "Reset");
	o->callback((Fl_Callback*)ResetCB, this );
      } // Fl_Button* o
			
      { Fl_Button* o = new Fl_Button(lX+350, lY, 100, 25, "Direct export");
	o->callback((Fl_Callback*)DirectExpCB, this );
      } // Fl_Button* o
      o3->end();
    }
    

    myWindow->end();
		
    myWindow->show( 0, nullptr);
    //  maj();


    // BON il y un probleme avec la sortie, mauvais  utilisation (j'ai eu tord de suivre les exemples ... ), ça marche quand meme, mais il faut faire attention !
    std::cout << "***********************************  DIALOGUE **************************" << std::endl;
    std::cout << "***********************************  DIALOGUE **************************" << std::endl;
    std::cout << "***********************************  DIALOGUE **************************" << std::endl;
	
    maj();
  
    cContinue = true;
    while (Fl::wait() && cContinue );

	
    std::cout << "*********************************** FIN DIALOGUE **************************" << std::endl;
    std::cout << "*********************************** FIN DIALOGUE **************************" << std::endl;
    std::cout << "*********************************** FIN DIALOGUE **************************" << std::endl;

  }

  //----------------------------------------
  void DialogSubDiv::SliderCB( Fl_Widget*, void*pUserData )
  {
    DialogSubDiv* lDialog = reinterpret_cast<DialogSubDiv*>(pUserData);
    lDialog->maj();
  }
  //----------------------------------------
  void DialogSubDiv::SizeCB( Fl_Widget*, void*pUserData )
  {
    DialogSubDiv* lDialog = reinterpret_cast<DialogSubDiv*>(pUserData);
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
    DialogSubDiv* lDialog = reinterpret_cast<DialogSubDiv*>(pUserData);
    lDialog->maj(); 
  }
  //----------------------------------------
  void DialogSubDiv::ChoiceCB( Fl_Widget*, void*pUserData )
  {									 
    DialogSubDiv* lDialog = reinterpret_cast<DialogSubDiv*>(pUserData);
    lDialog->maj(); 
  }
  //----------------------------------------
  void DialogSubDiv::CancelCB( Fl_Widget*, void* pUserData ) {
 
    DialogSubDiv* lDialog = reinterpret_cast<DialogSubDiv*>(pUserData);
    TheInput.cancelCurrentCreation();

    Creation::Instance().redrawAllCanvas3d(PP3d::Compute::Nothing);

    Fl::delete_widget( lDialog->myWindow );
    lDialog->cContinue = false;
  }
  //----------------------------------------
  void DialogSubDiv::DirectExpCB( Fl_Widget*, void* pUserData )
  {
    DialogSubDiv* lDialog = reinterpret_cast<DialogSubDiv*>(pUserData);

    Fl_Native_File_Chooser fnfc;
    fnfc.title("Pick a file for export");
    fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
    fnfc.filter("3D obj\t*.obj\n"
		"3D obj Files\t*.{obj}");
    fnfc.directory(".");           // default directory to use
    // Show native chooser
    switch ( fnfc.show() )
      {
      case -1: printf("ERROR: %s\n", fnfc.errmsg());    break;  // ERROR
      case  1: printf("CANCEL\n");                      break;  // CANCEL
      default:
	{
	  std::cout << "PICKED: " << fnfc.filename() << std::endl;
	  PP3d::Object* lObject =  TheInput.getCurrentCreation();
	  if( lObject != nullptr )
	    {
	      std::ofstream lOut;						
	      lOut.open( fnfc.filename());
	      if( lOut.good() )
		{
									
		  PP3d::MyExportObj lExpObj( lOut );
		  {
		    lExpObj.save(  lObject );
		  }
									
		  lOut.close();
		}
	    }
	}
	break;						
      }				 
  }
  //----------------------------------------
  void DialogSubDiv::ResetCB( Fl_Widget*, void* pUserData )
  {
 
    //    DialogSubDiv* lDialog = reinterpret_cast<DialogSubDiv*>(pUserData);
    //  lDialog->cMyCanvas->getDataBase().cancelCurrentCreation();

    Creation::Instance().redrawAllCanvas3d(PP3d::Compute::Nothing);
  }
  //----------------------------------------
  void DialogSubDiv::OkCB( Fl_Widget*, void*pUserData )
  {
    DialogSubDiv* lDialog = reinterpret_cast<DialogSubDiv*>(pUserData);
    lDialog->maj();
    PP3d::Object* lObj = TheInput.validCurrentCreation(TheBase);
    if( lObj != nullptr )
      {
	lObj->rename(  "Polyedre : Division"  );
      }
    
    PushHistory();


    Creation::Instance().redrawAllCanvas3d(PP3d::Compute::FacetAll);
    Creation::Instance().redrawObjectTree();

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

