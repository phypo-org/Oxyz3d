#include "Application.h"


#include "Win3d.h"
#include "WinObjTree.h"
#include "WinHisto.h"

#include <FL/Fl_PNG_Image.H>

#include "Shape/DataBase.h"

#include <sstream>
#include <ostream>

#include <getopt.h>



namespace M3d{

  Application* Application::sTheAppli = nullptr;

  //************************************
  Application::Application()	

    :cuDatabase( std::unique_ptr<PP3d::DataBase>( new PP3d::DataBase() ))
    , cuHistory( std::unique_ptr<History>( new History() ))
    , cCurrentTransform( Transform::Nothing)
  {
    cuHistory->save( *cuDatabase, History::SaveMode::Full );
		
    INFO(  "========= Application::Application" );


		
    M3d::ShapeLua::SetPrototype();
		
    cLua=  (M3d::ShapeLua*)M3d::ShapeLua::GetOrCreateSession("Lua", &std::cout );
    cLua->setDatabase( *cuDatabase.get() );
		
    /*
    cLua->doCode( "PPrintln(\"Hello it's C++\" )");
    cLua->doCode( "PListLib()");
    cLua->doCode( "PListLibFtn()" );
    cLua->doCode( "print(\"Hello it's Lua\")" );

    cLua->doCode("ShapeAddCurrentPoint(2,4,6)");
    cLua->doCode("ShapeAddCurrentPoint(4,5,7)");
    */		
  }
  //-----------------------------------
  //	TODO  MAKE Database AutoSave 
  //-----------------------------------
  Application::~Application()
  {
    delete cLua;
    
    cLua = nullptr;
  }
  //-----------------------------------
  //-----------------------------------
  //-----------------------------------
  int Application::init( int argc, char* argv[] )
  {
    char c;
    while ( (c = getopt(argc, argv, "hf:vV:gG:")) != -1 ) 	
      {
	switch ( c )
	  {
	  case 'v':          
	    PP3d::sVerbose++;
	    break;
	    
	  case 'V':	
	    PP3d::sVerbose = atoi(optarg);
	    break;
	    
	  case 'g':          
	    PP3d::sDebug++;
	    break;
	  
	  case 'G':	
	  PP3d::sDebug = atoi(optarg);
	  break;
	  
	}
       
      }
    fl_open_display();

    Fl_RGB_Image* lImg = new Fl_PNG_Image("Icons/Oxyz3d.png");
    Fl_Window::default_icon( lImg );
    
    return 0;
  }
  //-----------------------------------
  //-----------------------------------
  //-----------------------------------
  Win3d & Application::createNewWin3d( int pW, int pH )
  {
    static int lNumWin=1;

    std::stringstream lStr;

    lStr << "Oxyz3d " << lNumWin++;

		
    std::unique_ptr<Win3d> luWin3d( new Win3d(  lStr.str().c_str(), pW, pH, *(cuDatabase.get()) ));

    Win3d* lReturn =luWin3d.get();

    cAllWin3d.push_back(  std::unique_ptr<Win3d>(luWin3d.release()) );
		
    return *lReturn;
	
  }
  //-----------------------------------	
  void Application::redrawAllCanvas3d(  DeferRedraw iFlagDeverRedraw )
  { 
    if( iFlagDeverRedraw == DeferRedraw::DeferTrue )
      {
	cDeferFlagRedraw =  DeferRedraw::DeferTrue;
      }
    else
      {
	trueRedrawAllCanvas3d();
      }    
  }
  //-----------------------------------	
  void Application::trueRedrawAllCanvas3d()
  {
    for( std::unique_ptr<Win3d> &lWin : cAllWin3d )
      {
	lWin->canvasRedraw();
      }
  }
  //-----------------------------------	
  void Application::setCursorPosition( PP3d::Point3d& pPos)
  {
    for( std::unique_ptr<Win3d> &lWin : cAllWin3d )
      {
	lWin->setCursorPosition(pPos);
      }
  }
  //-----------------------------------	
  Win3d* Application::findCanvas3d( int iId )
  {
    for( std::unique_ptr<Win3d> &lWin : cAllWin3d )
      {
	if( lWin->getId() == iId )
	  {
	    return lWin.get();
	  }
      }
    return nullptr;
  }
  //----------------------------------------
  void Application::createObjectTree()
  {
    WinObjTree::Instance().show();
    redrawObjectTree();
  }
  //----------------------------------------
  void Application::redrawObjectTree()
  {
    WinObjTree::Instance().rebuild();		
  }
  //----------------------------------------
  void Application::createWinHisto()
  {
    WinHisto::Instance().show();
    redrawWinHisto();
  }
  //----------------------------------------
  void Application::redrawWinHisto()
  {
    WinHisto::Instance().rebuild();		
  }
  //----------------------------------------
  void Application::validate( History::SaveMode iMode, DeferRedraw iFlagDefer )
  {
    if( iMode ==  History::SaveMode::Reset )
      {				
	cuHistory = std::make_unique<History>();
	cuHistory->save( *cuDatabase, History::SaveMode::Full );		
      }
    else
      {
	cuHistory->save(  *cuDatabase, iMode );
      }

    if( iFlagDefer == DeferRedraw::DeferTrue )
      {
	cDeferFlagRedraw = DeferRedraw::DeferTrue;
      }
    else
      {
	cDeferFlagRedraw = DeferRedraw::DeferFalse;
	
	redrawAllCanvas3d();
	redrawObjectTree();
	redrawWinHisto();
      }
  }
  //----------------------------------------
  void Application::makeDefer( )
  {
    DBG1( "Application::makeDefer redraw " << (int) cDeferFlagRedraw );
    
    if( cDeferFlagRedraw == DeferRedraw::DeferTrue )
      {
	DBG1( "Application::makeDefer redraw ");
	
	trueRedrawAllCanvas3d();
	redrawObjectTree();
	redrawWinHisto();
	cDeferFlagRedraw = DeferRedraw::DeferFalse;
      }
  }
  //----------------------------------------
  PP3d::DataBase* Application::swapBase( PP3d::DataBase* ioBase )
  {
    if( ioBase == nullptr )
      return nullptr;

    PP3d::DataBase* lOldBase = cuDatabase.release();
    cuDatabase.reset(ioBase );
 
    
    redrawAllCanvas3d();
    redrawObjectTree();
    redrawWinHisto();

    return lOldBase;
  }
  //----------------------------------------
  void Application::FlCheckCallback( void* )
  {
    DBG( "************************ FlCheckCallback ************************" );
      
    Instance().makeDefer();
  }
  //************************************
};
