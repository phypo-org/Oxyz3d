#include "ShapeLua.h"
#include "Create/Creation.h"
#include "Create/Win3d.h"
#include "Create/Dialogs.h"

#include "Shape/Selection.h"

#include "Shape/SavRead.h"
#include "Shape/ExportObj.h"

#include <fstream>

#ifdef USING_LUA    

using namespace PLua;
using namespace PP3d;

namespace M3d {
	
  //-----------------------------------------
  // creer une nouvelle fenetre 3d et renvoie son id
  CLUA_OPEN_CODE( LUA_NewCanvas3d, 2);
  
  double lW = lua_tonumber( pLua, 1) ;
  double lH = lua_tonumber( pLua, 2) ;
  
  Win3d& lWin = Creation::Instance().createNewWin3d( (int)lW, (int)lH );

  lua_pushinteger( pLua, lWin.getId() );
 
  CLUA_CLOSE_CODE(1)
  
  //===========================================
  CLUA_OPEN_CODE( LUA_AddCurrentPointXYZ, 3);
	
		
  double lX = lua_tonumber( pLua, 1) ;
  double lY = lua_tonumber( pLua, 2) ;
  double lZ = lua_tonumber( pLua, 3) ;
  
  //  std::cout << "************ lX:" << lX <<  " lY:" << lY << " lZ:" << lZ << std::endl;
  
  TheInput.addPointToCurrentLine( Point3d(lX,lY,lZ) ); 
  //lua_pushinteger( pLua, lWin.getId() );
	
  CLUA_CLOSE_CODE(0)
  //-----------------------------------------
  CLUA_OPEN_CODE( LUA_CurrentToFacet, 0);
  
  PP3d::ObjectPtr lObj = TheInput.convertCurrentLineToFacet(TheBase);
  lua_pushinteger( pLua, lObj->getId() );
  
  CLUA_CLOSE_CODE(0)
  //-----------------------------------------
  CLUA_OPEN_CODE( LUA_CurrentToPoly, 0);
  
  PP3d::ObjectPtr lObj= TheInput.convertCurrentLineToPolylines(TheBase);
  lua_pushinteger( pLua, lObj->getId() );

  CLUA_CLOSE_CODE(0)
  //===========================================

  CLUA_OPEN_CODE( LUA_RedrawAllCanvas3d, 0);
  
  Creation::Instance().redrawAllCanvas3d(PP3d::Compute::FacetAll);
  
  CLUA_CLOSE_CODE(0)
  //-----------------------------------------
  CLUA_OPEN_CODE( LUA_RedrawAll, 0);
  
  Creation::Instance().redrawAll(PP3d::Compute::FacetAll);
  
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
	  
	  lSav.save( *Creation::Instance().getDatabase());	  
	  lFilSav.close();
	  lOut << "OK"<< std::endl;
	  AppendConsoleLua( "Save ok");
	  //	  Creation::Instance().getLua().append("OK");
	}
      */
      if( PP3d::MySav::Save( lua_tostring(pLua, 1),  
			     *Creation::Instance().getDatabase(), true, &TheSelect  ))
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
    //      Creation::Instance().getLua().append("Error : get filename");
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
	  lRead.read( *Creation::Instance().getDatabase() );
	  lFileIn.close();
	  lOut << "OK"<< std::endl;
	  AppendConsoleLua( "read ok");
	  //	  Creation::Instance().getLua().append("OK");
	  */
      
      if( PP3d::MyRead::Read( lua_tostring(pLua, 1),
			      *Creation::Instance().getDatabase(), &TheSelect ))
	{
	  Creation::Instance().redrawAll(PP3d::Compute::FacetAll);
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
    //      Creation::Instance().getLua().append("Error : get filename");
      //renvoyer une erreur 
    }
   
  CLUA_CLOSE_CODE(0)
  //-----------------------------------------
  CLUA_OPEN_CODE( LUA_exportAll2Obj, 1);
  
  if( lua_isstring(pLua, 1) )
    {     
      std::ofstream lFilSav;						
      lFilSav.open( lua_tostring(pLua, 1));
      if( lFilSav.good() )
	{	  
	  PP3d::MyExportObj lExpObj( lFilSav );
	  
	  lExpObj.save( *Creation::Instance().getDatabase());	  
	  lFilSav.close();
	  lOut << "OK"<< std::endl;
	  AppendConsoleLua( "export ok");
	  //	  Creation::Instance().getLua().append("OK");
	}
      else
	{
	  lOut<<"Bad file" << std::endl;
	}
    }
  else
    {
      AppendConsoleLua( "export failed" );
    //      Creation::Instance().getLua().append("Error : get filename");
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
	  lRead.read( *Creation::Instance().getDatabase() );
	  lFileIn.close();
	  lOut << "OK"<< std::endl;
	  AppendConsoleLua( "import ok");
	  //	  Creation::Instance().getLua().append("OK");
	  Creation::Instance().redrawAll(PP3d::Compute::FacetAll);
	}
      else
	{
	  lOut<<"Bad file" << std::endl;
	}
    }
  else
    {
      AppendConsoleLua( "import failed" );
    //      Creation::Instance().getLua().append("Error : get filename");
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
    registerFunction( "", "help", "h",  "List the functions",  PLuaSession::LUA_ListLibraryFonction );

    //=====================
    registerFunction( "Win",   "NewCanvas3d", "",  "Open a new 3d windows",     LUA_NewCanvas3d );
    //=====================
    
    registerFunction( "Shape", "AddCurrentPoint", "p", "Adding a point to the current input (3 arguments)",    LUA_AddCurrentPointXYZ  );
    //    registerFunction( "", "p",  "Adding a point to the current input (3 arguments)",    LUA_AddCurrentPointXYZ  );
    
    registerFunction( "Shape", "CurrentToFacet", "f", "Transform current inpu to facet",     LUA_CurrentToFacet  );
    //    registerFunction( "",      "facet",  "Transform current inpu to facet",     LUA_CurrentToFacet  );
    
    registerFunction( "Shape", "CurrentToPolyline", "l",  "Transform current inpu to polyline",      LUA_CurrentToPoly );
    //     registerFunction( "",     "lines",   "Transform current inpu to polyline",     LUA_CurrentToPoly );
   //=====================
    registerFunction( "Win",  "RedrawCanvas", "", "Actualise the canvas",       LUA_RedrawAllCanvas3d   );
    registerFunction( "Win",   "RedrawAll",   "R",  "Actualise all windows",       LUA_RedrawAll   );
    //    registerFunction( "",      "redraw",    "Actualise all windows",       LUA_RedrawAll   );
    //=====================
    registerFunction( "",      "save",  "",  "save database. Need name",           LUA_saveAll  );
    registerFunction( "",      "load",  "",  "load a database. Need name",         LUA_load  );
    registerFunction( "",      "exportObj", "",  "export database to .obj. Need name",         LUA_exportAll2Obj  );
    //=====================
  }	
  //****************************************

}// end namesapce
#endif
