#include "Application.h"


#include "Win3d.h"
#include "WinObjTree.h"

#include "Shape/DataBase.h"

#include <sstream>
#include <ostream>

#include "Preference.h"

namespace M3d{


  //************************************
  Application::Application()	

    :cuDatabase( std::unique_ptr<PP3d::DataBase>( new PP3d::DataBase() ))
    ,cuDatabaseTransform( std::unique_ptr<PP3d::DataBase>( new PP3d::DataBase() ))
    ,cCurrentTransform( Transform::Nothing)
  {
    std::cout << "========= Application::Application" << std::endl;
    
		
    cSelectTransform.changeSelectType( PP3d::SelectType::Null );

#ifdef USING_LUA
    M3d::ShapeLua::SetPrototype();
    cLua = (M3d::ShapeLua*)M3d::ShapeLua::GetOrCreateSession("Lua", &std::cout );
	
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
#ifdef USING_LUA
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
  void Application::redrawAllCanvas(   PP3d::Compute iCompute  )
  {
    //    std::cout <<  "Application::redrawAllCanvas" << std::endl;

    recomputeAll( iCompute );
    
    for( std::unique_ptr<Win3d> &lWin : cAllWin3d )
      {
	//	std::cout <<  "      Application::redrawAllCanvas canvas" << std::endl;
	lWin->canvasRedraw();  
	lWin->setUndoRedoState();
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
  const char * Application::autoSave()
  {
    static std::string cAutoSave="AutoSave.oxyz";
    static int cNbSav=1;

    if( Preference::Instance().cAutoSave )
      {
	if( cNbSav++ % Preference::Instance().cAutoSaveFrequency == 0 )
	  {
	    std::cout << "Application::autoSave return " << cAutoSave.c_str() << std::endl;
	    return cAutoSave.c_str();
	  }
      }
    std::cout << "Application::autoSave return NULL" << std::endl;

    return nullptr;
  }
	 

  
  //************************************
}
