#include "ShapeLua.h"
#include "Application.h"
#include "Win3d.h"
#include "Dialogs.h"

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
	
		
  double lX = lua_tonumber( pLua, 1) ;
  double lY = lua_tonumber( pLua, 2) ;
  double lZ = lua_tonumber( pLua, 3) ;
	
  //  std::cout << "************ lX:" << lX <<  " lY:" << lY << " lZ:" << lZ << std::endl;
  
  Application::Instance().getDatabase()->addPointToCurrentLine( Point3d(lX,lY,lZ) ); 
  //lua_pushinteger( pLua, lWin.getId() );
	
  CLUA_CLOSE_CODE(0)
  //-----------------------------------------
  CLUA_OPEN_CODE( LUA_CurrentToFacet, 0);
  
  PP3d::ObjectFacet* lObj = Application::Instance().getDatabase()->convertCurrentLineToFacet();
  lua_pushinteger( pLua, lObj->getId() );
  
  CLUA_CLOSE_CODE(0)
  //-----------------------------------------
  CLUA_OPEN_CODE( LUA_CurrentToPoly, 0);
  
  PP3d::ObjectPolylines* lObj= Application::Instance().getDatabase()->convertCurrentLineToPolylines();
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
      /*
      std::ofstream lFilSav;						
      lFilSav.open( lua_tostring(pLua, 1));
      if( lFilSav.good() )
	{	  
	  PP3d::MySav lSav( lFilSav );	  
	  
	  lSav.save( *Application::Instance().getDatabase());	  
	  lFilSav.close();
	  lOut << "OK"<< std::endl;
	  AppendConsoleLua( "Save ok");
	  //	  Application::Instance().getLua().append("OK");
	}
      */
      if( PP3d::MySav::Save( lua_tostring(pLua, 1),
			     *Application::Instance().getDatabase(), &TheSelect  ))
	{
	  //	  lOut << "OK"<< std::endl;
	  AppendConsoleLua( "Save ok");	  
	}
      else
	{
	  //	  lOut<<"Bad file" << std::endl;
	  AppendConsoleLua( "save failed");	  
	}
    }
  else
    {
      AppendConsoleLua( "Save failed" );
    //      Application::Instance().getLua().append("Error : get filename");
      //renvoyer une erreur 
    }
   
  CLUA_CLOSE_CODE(0)
  //-----------------------------------------
  CLUA_OPEN_CODE( LUA_load, 1);
  
  if( lua_isstring(pLua, 1) )
    {
      /*
	std::ifstream lFileIn;						
      lFileIn.open(  lua_tostring(pLua, 1));
      
      if( lFileIn.good() )
	{	  
	  PP3d::MyRead lRead( lFileIn );
	  lRead.read( *Application::Instance().getDatabase() );
	  lFileIn.close();
	  lOut << "OK"<< std::endl;
	  AppendConsoleLua( "read ok");
	  //	  Application::Instance().getLua().append("OK");
	  */
      
      if( PP3d::MyRead::Read( lua_tostring(pLua, 1),
			      *Application::Instance().getDatabase(), &TheSelect ))
	{
	  Application::Instance().redrawAll();
	  lOut << "OK"<< std::endl;
	  AppendConsoleLua( "read ok");
	}
      else
	{
	  lOut<<"Bad file" << std::endl;
	}
    }
  else
    {
      AppendConsoleLua( "read failed" );
    //      Application::Instance().getLua().append("Error : get filename");
      //renvoyer une erreur 
    }
   
  CLUA_CLOSE_CODE(0)
  //-----------------------------------------
  CLUA_OPEN_CODE( LUA_exportAll, 1);
  
  if( lua_isstring(pLua, 1) )
    {     
      std::ofstream lFilSav;						
      lFilSav.open( lua_tostring(pLua, 1));
      if( lFilSav.good() )
	{	  
	  PP3d::MyExportObj lExpObj( lFilSav );
	  
	  lExpObj.save( *Application::Instance().getDatabase());	  
	  lFilSav.close();
	  lOut << "OK"<< std::endl;
	  AppendConsoleLua( "export ok");
	  //	  Application::Instance().getLua().append("OK");
	}
      else
	{
	  lOut<<"Bad file" << std::endl;
	}
    }
  else
    {
      AppendConsoleLua( "export failed" );
    //      Application::Instance().getLua().append("Error : get filename");
      //renvoyer une erreur 
    }
   
  CLUA_CLOSE_CODE(0)
  //-----------------------------------------
  CLUA_OPEN_CODE( LUA_import, 1);
  
  if( lua_isstring(pLua, 1) )
    {     
      std::ifstream lFileIn;						
      lFileIn.open(  lua_tostring(pLua, 1));
      
      if( lFileIn.good() )
	{	  
	  PP3d::MyImportObj lRead( lFileIn );
	  lRead.read( *Application::Instance().getDatabase() );
	  lFileIn.close();
	  lOut << "OK"<< std::endl;
	  AppendConsoleLua( "import ok");
	  //	  Application::Instance().getLua().append("OK");
	  Application::Instance().redrawAll();
	}
      else
	{
	  lOut<<"Bad file" << std::endl;
	}
    }
  else
    {
      AppendConsoleLua( "import failed" );
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
    registerFunction( "", "help",     PLuaSession::LUA_ListLibraryFonction );

    //=====================
    registerFunction( "Win",   "NewCanvas3d",         LUA_NewCanvas3d );
    //=====================		      
    registerFunction( "Shape", "AddCurrentPoint",     LUA_AddCurrentPointXYZ  );
    registerFunction( "", "p",     LUA_AddCurrentPointXYZ  );
    
    registerFunction( "Shape", "CurrentToFacet",      LUA_CurrentToFacet  );
    registerFunction( "", "facet",      LUA_CurrentToFacet  );
    
    registerFunction( "Shape", "CurrentToPolyline",       LUA_CurrentToPoly );
     registerFunction( "", "lines",       LUA_CurrentToPoly );
   //=====================
    registerFunction( "Win",  "RedrawCanvas",        LUA_RedrawAllCanvas3d   );
    registerFunction( "Win",  "RedrawAll",           LUA_RedrawAll   );
    registerFunction( "",  "redraw",           LUA_RedrawAll   );
    //=====================
    registerFunction( "",      "save",                LUA_saveAll  );
    registerFunction( "",      "load",                LUA_load  );
    registerFunction( "",      "export",              LUA_exportAll  );
    //=====================
  }	
  //****************************************

}// end namesapce
