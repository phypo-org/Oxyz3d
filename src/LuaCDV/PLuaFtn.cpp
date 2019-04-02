#include "PLuaFtn.h"


#include "ORS_DataBase/MasterBaseProto.h"
#include "ORS_DataBase/OrsObjBase.h"


using namespace PLua;

//------------------------------------------------

CLUA_OPEN_CODE(LUA_GetBaseFromObjectId, 1);

int lResult = -1;
if( lua_isnumber(pLua, 1) )
{
	long lId;
	
	lua_number2int( lId, lua_tonumber(pLua, 1) );
	
	OrsObjBase* lBase = MasterBaseProto::GetBaseSliceBaseIdVect( lId );
	if( lBase )
		lResult = lBase->getBaseId();
}


lua_pushnumber( pLua, lResult );

CLUA_CLOSE_CODE(1);

//------------------------------------------------

CLUA_OPEN_CODE(LUA_GetBaseName, 1);

const char *lResult = "";

if( lua_isnumber(pLua, 1) )
{
	long lId;
	
	lua_number2int( lId, lua_tonumber(pLua, 1) );
  OrsObjBase* lBase =  OrsObjBase::FindDataBase( lId) ;
	
	if( lBase ) 
		lResult =  lBase->getBaseName();
}

lua_pushstring( pLua, lResult );

CLUA_CLOSE_CODE(1);
 
//------------------------------------------------

CLUA_OPEN_CODE(LUA_GetBaseByName, 1) 

int lResult = -1;

if( lua_isstring(pLua, 1) )
{
  OrsObjBase* lBase = OrsObjBase::FindDataBaseByName( lua_tostring(pLua, 1)  );
	if( lBase )
			lResult =  lBase->getBaseId();
}
lua_pushnumber( pLua, lResult );

CLUA_CLOSE_CODE(1);

//------------------------------------------------

CLUA_OPEN_CODE( LUA_GetServerTimeUTC70, 0 );

lua_pushnumber( pLua, GetGMTime() );  
	
CLUA_CLOSE_CODE(1);			

//------------------------------------------------
//------------------------------------------------
//------------------------------------------------
// Pour les focntions SECURITY_LEVEL_VIEW
bool 
LuaRegisterOrsServerLib( PLuaSession&  pLuaSession )
{
	pLuaSession.registerFunction( "CgetServerTimeUTC70",        LUA_GetServerTimeUTC70,     CmdVect::SECURITY_LEVEL_VIEW  );
	pLuaSession.registerFunction( "CgetBaseIdFromObjectId",     LUA_GetBaseFromObjectId,    CmdVect::SECURITY_LEVEL_VIEW );
	pLuaSession.registerFunction( "CgetBaseName",               LUA_GetBaseName,            CmdVect::SECURITY_LEVEL_VIEW );
	pLuaSession.registerFunction( "CgetBaseByName",             LUA_GetBaseByName,          CmdVect::SECURITY_LEVEL_VIEW );

  return true;
}
//------------------------------------------------
