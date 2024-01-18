#include "Creation.h"


#include "Win3d.h"
#include "WinObjTree.h"

#include "Shape/DataBase.h"

#include <sstream>
#include <ostream>

#include "Gui/Preference.h"
#include "Utils.h"

namespace M3d{


  //************************************
  Creation::Creation()	

    :cuDatabase( std::unique_ptr<PP3d::DataBase>( new PP3d::DataBase() ))
    ,cCurrentTransform( Transform::Nothing)
  {
    std::cout << "========= Creation::Creation" << std::endl;
    
		

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
  Creation::~Creation()
  {
#ifdef USING_LUA
    delete cLua;
    cLua = nullptr;
#endif    
  }
  //-----------------------------------
  //-----------------------------------
  //-----------------------------------
  int Creation::init( int argc, char* argv[] )
  {
    return 0;
  }
  //-----------------------------------
  void Creation::validateDatabaseTmp()
  {
    Utils::DuplicateObjects( *getDatabaseTmp(), getDatabaseTmp()->getAllObject(), TheBase );
    clearDatabaseTmp();
  }
  //-----------------------------------
  Win3d & Creation::createNewWin3d( int pW, int pH )
  {
    static int lNumWin=1;

    std::stringstream lStr;

    lStr << "Oxyz3d " << lNumWin++;

		
    std::unique_ptr<Win3d> luWin3d( new Win3d(  lStr.str().c_str(), pW, pH, *(cuDatabase.get()) ));
    
    /////////////////////////    luWin3d->initCanvas(  pW, pH );
                  
    Win3d* lReturn =luWin3d.get();

    cAllWin3d.push_back(  std::unique_ptr<Win3d>(luWin3d.release()) );
		
    return *lReturn;
	
  }
  //-----------------------------------	
  void Creation::redrawAllCanvas3d(   PP3d::Compute iCompute  )
  {
    //    std::cout <<  "Creation::redrawAllCanvas" << std::endl;    
    for( std::unique_ptr<Win3d> &lWin : cAllWin3d )
      {
	//	std::cout <<  "      Creation::redrawAllCanvas canvas" << std::endl;
	lWin->canvasRedraw();  
      }
  }
  //-----------------------------------	
  void Creation::redrawAllWin3d(   PP3d::Compute iCompute  )
  {    
    recomputeAll( iCompute );
    
    for( std::unique_ptr<Win3d> &lWin : cAllWin3d )
      {
	//	std::cout <<  "      Creation::redrawAllCanvas canvas" << std::endl;
	lWin->canvasRedraw();  
	lWin->setUndoRedoState();
        lWin->redraw();
      }
  }
  //-----------------------------------	 
  void Creation::setCursorPosition( PP3d::Point3d& pPos)
  {
    for( std::unique_ptr<Win3d> &lWin : cAllWin3d )
      {
	lWin->setCursorPosition(pPos);
      }
  }
  //-----------------------------------	
  Win3d* Creation::findCanvas3d( int iId )
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
  void Creation::createObjectTree()
  {
    WinObjTree::Instance().show();
    redrawObjectTree();
  }
  //----------------------------------------
  void Creation::redrawObjectTree()
  {
    WinObjTree::Instance().rebuild();		
  }
  //----------------------------------------
  void Creation::info( const std::string & iStr )      
  {
    for( std::unique_ptr<Win3d> &lWin :cAllWin3d )
      {
	lWin->setInfo( iStr);
      }
  } 
  //----------------------------------------
  const char * Creation::autoSave()
  {
    static std::string cAutoSave="AutoSave.oxyz";
    static int cNbSav=1;

    if( Preference::Instance().cFileAutoSave )
      {
	if( cNbSav++ % Preference::Instance().cFileAutoSaveFrequency == 0 )
	  {
	    std::cout << "Creation::autoSave return " << cAutoSave.c_str() << std::endl;
	    return cAutoSave.c_str();
	  }
      }
    std::cout << "Creation::autoSave return NULL" << std::endl;

    return nullptr;
  }
  //----------------------------------------
    bool Creation::setCurrentAxis(PP3d::EntityPtr iObj)
    {
      switch( iObj->getType() )
        {
        case PP3d::ShapeType::Line :
          return setCurrentAxis( ((PP3d::LinePtr)iObj));

          // METTRE Dyn_cast !!!
          
        case PP3d::ShapeType::Object :
          if( ((PP3d::ObjectPtr)iObj)->getObjType() ==  PP3d::ObjectType::ObjLine )
            {
              return setCurrentAxis( ((PP3d::ObjectLinePtr)iObj)->getLine());
            }
        default: ;
        }
      return false;
    }

  
 
  //************************************
}
