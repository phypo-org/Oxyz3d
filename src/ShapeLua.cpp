#include "ShapeLua.h"
#include "Application.h"


using namespace PLua;
using namespace PP3d;

namespace M3d {
	
	//-----------------------------------------
	CLUA_OPEN_CODE( LUA_AddCurrentPointXYZ, 3);

	

 	PP3d::DataBase* lDatabase = ((M3d::ShapeLua&)lLuaSession).cDatabase;
		
	double lX = lua_tonumber( pLua, 1) ;
	double lY = lua_tonumber( pLua, 2) ;
	double lZ = lua_tonumber( pLua, 3) ;
	
	std::cout << "************ lX:" << lX <<  " lY:" << lY << " lZ:" << lZ << std::endl;


	lDatabase->addPointToCurrentLine( Point3d(lX,lY,lZ) ); 
	Application::Instance().redrawAllCanvas3d();
	
	CLUA_CLOSE_CODE(0);
	
	//****************************************
	ShapeLua::ShapeLua(   const std::string& pNameSession, std::ostream* pStream )
		:PLuaSession(  pNameSession, pStream )
	{	
    registerFunction( "Shape", "AddPoint", LUA_AddCurrentPointXYZ  );
	}	
	//****************************************

}// end namesapce
