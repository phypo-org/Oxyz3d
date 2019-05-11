#include "Application.h"


#include "Win3d.h"
#include "WinObjTree.h"
#include "WinHisto.h"

#include "Shape/DataBase.h"

#include <sstream>
#include <ostream>



namespace M3d{

  Application* Application::sTheAppli = nullptr;

  //************************************
  Application::Application()	

    :cuDatabase( std::unique_ptr<PP3d::DataBase>( new PP3d::DataBase() ))
    , cuHistory( std::unique_ptr<History>( new History() ))
    , cCurrentTransform( Transform::Nothing)
  {
    cuHistory->save( *cuDatabase, History::SaveMode::Full );
		
    std::cout << "========= Application::Application" << std::endl;


#ifdef USE_LUA
    M3d::ShapeLua::SetPrototype();
		
    cLua=  (M3d::ShapeLua*)M3d::ShapeLua::GetOrCreateSession("Lua", &std::cout );
    cLua->setDatabase( *cuDatabase.get() );
		
	
    cLua->doCode( "PPrintln(\"Hello it's C++\" )");
    cLua->doCode( "PListLib()");
    cLua->doCode( "PListLibFtn()" );
    cLua->doCode( "print(\"Hello it's Lua\")" );

    cLua->doCode("ShapeAddCurrentPoint(2,4,6)");
    cLua->doCode("ShapeAddCurrentPoint(4,5,7)");		
#endif
  }
  //-----------------------------------
  //	TODO  MAKE Database AutoSave 
  //-----------------------------------
  Application::~Application()
  {
#ifdef USE_LUA
    delete cLua;
    cLua = nullptr;
#endif
  }
  //-----------------------------------
  //-----------------------------------
  //-----------------------------------
  int Application::init( int argc, char* argv[] )
  {
    return 0;
  }
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
  void Application::redrawAllCanvas3d()
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
  void Application::validate( History::SaveMode iMode )
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
		
    redrawAllCanvas3d();
    redrawObjectTree();
    redrawWinHisto();	 
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
  //************************************
};
