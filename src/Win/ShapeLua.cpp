#include "ShapeLua.h"
#include "Application.h"
#include "Win3d.h"

#include "Shape/Selection.h"

#include "Shape/SavRead.h"
#include "Shape/ExportObj.h"

#include <fstream>

using namespace PLua;
using namespace PP3d;

namespace M3d {
	
  //-----------------------------------------
  // creer une nouvelle fenetre 3d et renvoie son id
  CLUA_OPEN_CODE( LUA_NewCanvas3d, 2);
  
  double lW = lua_tonumber( pLua, 1) ;
  double lH = lua_tonumber( pLua, 2) ;
  
  Win3d& lWin = Application::Instance().createNewWin3d( (int)lW, (int)lH );

  lua_pushinteger( pLua, lWin.getId() );
 
  CLUA_CLOSE_CODE(1)
  
  //===========================================
  CLUA_OPEN_CODE( LUA_AddCurrentPointXYZ, 3);
	
  PP3d::DataBase* lDatabase = ((M3d::ShapeLua&)lLuaSession).cDatabase;
		
  double lX = lua_tonumber( pLua, 1) ;
  double lY = lua_tonumber( pLua, 2) ;
  double lZ = lua_tonumber( pLua, 3) ;
	
  //  std::cout << "************ lX:" << lX <<  " lY:" << lY << " lZ:" << lZ << std::endl;
  
	 lDatabase->addPointToCurrentLine( Point3d(lX,lY,lZ) ); 
  //lua_pushinteger( pLua, lWin.getId() );
	
  CLUA_CLOSE_CODE(0)
  //-----------------------------------------
  CLUA_OPEN_CODE( LUA_CurrentToFacet, 0);
  PP3d::DataBase* lDatabase = ((M3d::ShapeLua&)lLuaSession).cDatabase;
  
  PP3d::ObjectFacet* lObj = lDatabase->convertCurrentLineToFacet();
  lua_pushinteger( pLua, lObj->getId() );
  
  CLUA_CLOSE_CODE(0)
  //-----------------------------------------
  CLUA_OPEN_CODE( LUA_CurrentToPoly, 0);
  PP3d::DataBase* lDatabase = ((M3d::ShapeLua&)lLuaSession).cDatabase;
  
  PP3d::ObjectPolylines* lObj= lDatabase->convertCurrentLineToPolylines();
  lua_pushinteger( pLua, lObj->getId() );

  CLUA_CLOSE_CODE(0)
  //===========================================

  CLUA_OPEN_CODE( LUA_RedrawAllCanvas3d, 0);
  
  Application::Instance().redrawAllCanvas3d();
  
  CLUA_CLOSE_CODE(0)
  //-----------------------------------------
  CLUA_OPEN_CODE( LUA_RedrawAll, 0);
  
  Application::Instance().redrawAll();
  
  CLUA_CLOSE_CODE(0)
  //-----------------------------------------
  CLUA_OPEN_CODE( LUA_saveAll, 1);
  
  if( lua_isstring(pLua, 1) )
    {     
      std::ofstream lFilSav;						
      lFilSav.open( lua_tostring(pLua, 1));
      if( lFilSav.good() )
	{	  
	  PP3d::MySav lSav( lFilSav );	  
	  
	  lSav.save( *Application::Instance().getDatabase());	  
	  lFilSav.close();
	  lOut << "OK"<< std::endl;
	  //	  Application::Instance().getLua().append("OK");
	}
      else
	{
	  lOut<<"Bad file" << std::endl;
	}
    }
  else
    {
  	  lOut << "KO"<< std::endl;
    //      Application::Instance().getLua().append("Error : get filename");
      //renvoyer une erreur 
    }
   
  CLUA_CLOSE_CODE(0)
	
 	
  //****************************************
  ShapeLua::ShapeLua(   const std::string& pNameSession, std::ostream* pStream )
    :PLuaSession(  pNameSession, pStream )
  {
    registerMyFunction();
  }
  //-------------------------------------
  void ShapeLua::registerMyFunction()
  {
    ShapeLua::sLibSep=0;
    
    //=====================
    registerFunction( "Win",   "NewCanvas3d",         LUA_NewCanvas3d );
    //=====================		      
    registerFunction( "Shape", "AddCurrentPoint",     LUA_AddCurrentPointXYZ  );
    registerFunction( "Shape", "CurrentToFacet",      LUA_CurrentToFacet  );
    registerFunction( "Shape", "CurrentToPoly",       LUA_CurrentToPoly );
    //=====================
    registerFunction( "Oxyz",  "RedrawCanvas",        LUA_RedrawAllCanvas3d   );
    registerFunction( "Oxyz",  "RedrawAll",           LUA_RedrawAll   );
    registerFunction( "",  "save",            LUA_saveAll  );
    //=====================
  }	
  //****************************************

}// end namesapce
