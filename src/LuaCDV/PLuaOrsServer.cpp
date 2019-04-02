#include "PLuaOrsServer.h"


#include "ORS_DataBase/MasterBaseProto.h"
#include "ORS_DataBase/OrsObjBase.h"
#include "OrsCommon/OrsCommon.h"


namespace PLua {

//------------------------------------------------
// Renvoit l'id de la base  a partir de l'intervalle des id permis

CLUA_OPEN_CODE(LUA_GetBaseFromObjectId, 1);
 
int lResult = -1;
if( lua_isnumber(pLua, 1) )
{
	OrsObjBaseId lId;
	
	lua_number2int( lId, lua_tonumber(pLua, 1) );
	
	OrsObjBase* lBase = MasterBaseProto::GetBaseSliceBaseIdVect( lId );
	if( lBase )
		lResult = (int)lBase->getBaseId();
}


lua_pushnumber( pLua, lResult );

CLUA_CLOSE_CODE(1);

//------------------------------------------------
// Renvoit le nom d une base a partir de son id

CLUA_OPEN_CODE(LUA_GetBaseName, 1);

const char *lResult = "";

if( lua_isnumber(pLua, 1) )
{ 
	OrsObjBaseId lId;
	
	lua_number2int( lId, lua_tonumber(pLua, 1) );
  OrsObjBase* lBase =  OrsObjBase::FindDataBase( lId) ;
	
	if( lBase ) 
		lResult =  lBase->getBaseName();
}
 
lua_pushstring( pLua, lResult );

CLUA_CLOSE_CODE(1);
 
//------------------------------------------------
// Renvoit la base a partir de son id

CLUA_OPEN_CODE(LUA_GetBaseDico, 1);

OrsObjBase* lBase = NULL;

if( lua_isnumber(pLua, 1) )
{ 
	OrsObjBaseId lId;
	
	lua_number2int( lId, lua_tonumber(pLua, 1) );
  lBase =  OrsObjBase::FindDataBase( (OrsObjId)lId) ;
	
	if( lBase ) 
		{
			lua_createtable ( pLua, 0, 6 );
			int lPosTable = lua_gettop( pLua ); 
			
			lua_pushstring( pLua, lBase->getBaseName() );
			lua_setfield( pLua,   lPosTable, "Name" );
			
			lua_pushinteger( pLua, lBase->getBaseId() ); 
			lua_setfield( pLua,   lPosTable, "BaseId" );
			
			lua_pushstring( pLua, lBase->getLocal()?(lBase->getBackup()?"Backup":"Local"):"Remote");
			lua_setfield( pLua,   lPosTable, "Mode" );

			lua_pushboolean( pLua, lBase->getLockedState() );
			lua_setfield( pLua,   lPosTable, "Locked" );

			lua_pushinteger( pLua, lBase->getMinId() );
			lua_setfield( pLua,   lPosTable, "MinId" );

			lua_pushinteger( pLua, lBase->getMaxId() );
			lua_setfield( pLua,   lPosTable, "MaxId" );
		}
}

if( lBase == NULL )
	lua_pushnil( pLua );
 

CLUA_CLOSE_CODE(1);
 
//------------------------------------------------
// Renvoit la table a partir de son id

CLUA_OPEN_CODE(LUA_GetTableDico, 2);

OrsObjTable* lTable = NULL;

if( lua_isnumber(pLua, 1) &&  lua_isnumber(pLua, 2 ))
{ 
	OrsObjBaseId lId;
	
	lua_number2int( lId, lua_tonumber( pLua, 1) );
  OrsObjBase* lBase =  OrsObjBase::FindDataBase( lId) ;
	
	if( lBase ) 
		{
			lua_number2int( lId, lua_tonumber( pLua, 2) );
			lTable = lBase->findTable( lId );

			if( lTable )
				{					
					lua_createtable ( pLua, 0, 4 );
					int lPosTable = lua_gettop( pLua ); 
					
					lua_pushstring( pLua, lTable->getTableName() );
					lua_setfield( pLua,   lPosTable, "Name" );
					
					lua_pushinteger( pLua, lTable->getTableId() ); 
					lua_setfield( pLua,   lPosTable, "TableId" );

					lua_pushinteger( pLua, lTable->getNumberOfAllObj());
					lua_setfield( pLua,   lPosTable, "Length" );
					
					lua_pushinteger( pLua, lTable->getCurrentId()); 
					lua_setfield( pLua,   lPosTable, "CurrentId" );					
				}
		}
}

if( lTable ==NULL )
	lua_pushnil( pLua );
 

CLUA_CLOSE_CODE(1);
 
//------------------------------------------------
// Renvoit l'id de la base a partir de son nom

CLUA_OPEN_CODE(LUA_GetBaseByName, 1) 

int lResult = -1;

if( lua_isstring(pLua, 1) )
{
  OrsObjBase* lBase = OrsObjBase::FindDataBaseByName( lua_tostring(pLua, 1)  );
	if( lBase )
		lResult = (int) lBase->getBaseId();
}
lua_pushnumber( pLua, lResult );

CLUA_CLOSE_CODE(1);

//------------------------------------------------
// Renvoit le temps UTC du serveur en seconde depuis 1970

CLUA_OPEN_CODE( LUA_GetServerTimeUTC70, 0 );

	lua_pushnumber( pLua, (lua_Number)MTServer::GetServerTimeUTC() );  
	
CLUA_CLOSE_CODE(1);			
//------------------------------------------------
// Memorise un copie d'un objet Ors dans la sessiOn courante
// pour un usage ulterieur 
// renvoit un identifiant sur la copie, Ou 0 En cas d'erreur

CLUA_OPEN_CODE(LUA_GetCopyOrsObject, 3);

long lResult = 0;

if( lua_isnumber(pLua, 1) 
		&& lua_isnumber(pLua, 2) 
		&& lua_isnumber(pLua, 3 ))
{
	OrsObjBaseId lBaseId;
	
	lua_number2int( lBaseId, lua_tonumber(pLua, 1) );
  OrsObjBase* lBase = OrsObjBase::FindDataBase( lBaseId) ;
	
	if( lBase != NULL ) 
		{
			OrsTypeObject lTypeId;			
			lua_number2int( lTypeId, lua_tonumber(pLua, 2) );
			
			OrsObjId lObjId;
			lua_number2int( lObjId, lua_tonumber(pLua, 3) );
			
	
			try {
				PAutoPtr<OrsObjCommon> laObj( lBase->getCopyOrThrow( lTypeId, lObjId, 1000  ) );
				if( laObj.isValid() )
					{
						long lTmpId = (long) laObj.getPtr(); 
						((PLuaCommon*)&lLuaSession)->addCopy( laObj ); 
						lResult = lTmpId;
					}
				else
					{
						OS_TRACE( "LUA_GetCopyOrsObject object not found for base:" << lBaseId << " type:" << lTypeId  << " id:" << lObjId  );
					}
			} catch(...) {				
				OS_TRACE( "LUA_GetCopyOrsObject Lock failed for base:" << lBaseId << " type:" << lTypeId  << " id:" << lObjId  );
			}

															
		}
	else
		OS_TRACE( "LUA_GetCopyOrsObject Base not found for " << lBaseId );
		
}
lua_pushnumber( pLua, (lua_Number)lResult );

CLUA_CLOSE_CODE(1);

//------------------------------------------------
// Renvoit la liste de toutes les bases gérées par 

CLUA_OPEN_CODE(LUA_GetBases, 0);


if( OrsCommonBase::sTheOrsCommon )
{
	PPTools::PVectorTypeIter< OrsObjBase* >		lIterBase( OrsObjBase::sOrsObjBaseVect ) ;
	
	while( lIterBase.hasNext() )
		{
			OrsObjBase*  lBase = lIterBase.next() ;
				
			if( lBase )
				{
					lua_checkstack( pLua, _NbReturn+10 );
					lua_pushnumber( pLua, (lua_Number)lBase->getBaseId() );
					_NbReturn++;
				}
		}
}
CLUA_CLOSE_CODE(0); // Utilise _NbReturn

//------------------------------------------------
// Renvoit la liste de toutes les tables d'une base

CLUA_OPEN_CODE(LUA_GetTables, 1);


if( OrsCommonBase::sTheOrsCommon )
{
	OrsObjBaseId lBaseId;
	
	lua_number2int( lBaseId, lua_tonumber(pLua, 1) );
  OrsObjBase* lBase = OrsObjBase::FindDataBase( lBaseId) ;
	
	if( lBase != NULL ) 
		{
			for ( int j = 0 ; j < ORS_OBJ_NBR_MAX ; ++ j ) 
				{	  
					OrsObjTable* lTable = lBase->findTable( j ) ;
					if ( lTable != NULL ) 
						{
							lua_checkstack( pLua, _NbReturn+10 );
							lua_pushnumber( pLua, (lua_Number)lTable->getTableId() );
							_NbReturn++;
						}
				}

		}
}
CLUA_CLOSE_CODE(0); // Utilise _NbReturn

//------------------------------------------------
CLUA_OPEN_CODE(LUA_GetTableContents, 2);

OrsObjTable* lTable = NULL;

if( lua_isnumber(pLua, 1) &&  lua_isnumber(pLua, 2 ))
{ 
	OrsObjBaseId lBaseId;
	
	lua_number2int( lBaseId, lua_tonumber( pLua, 1) );
  OrsObjBase* lBase =  OrsObjBase::FindDataBase( lBaseId) ;
	
	if( lBase ) 
		{
			OrsObjTableId lTableId;
			lua_number2int( lTableId, lua_tonumber( pLua, 2) );
			lTable = lBase->findTable(lTableId );

			if( lTable )
				{					
					PPTools::PVectorType<OrsObjId> lVectId;

					lTable->getAllId( lVectId );

					lua_createtable ( pLua, (int)lVectId.size(), 0 );
					int lPosTable = lua_gettop( pLua ); 

					int i=1;
					PPTools::PVectorTypeIter<OrsObjId> lIterId( lVectId );
					while( lIterId.hasNext() )
						{
							lua_pushinteger( pLua, i++ );
							lua_pushinteger( pLua, lIterId.next()  );
							lua_settable( pLua,   lPosTable  );	
						}							
				}
		}
}

if( lTable ==NULL )
	lua_pushnil( pLua );
 

CLUA_CLOSE_CODE(1);

//------------------------------------------------
// FAIRE AVEC UN LOCK pour modification
// Faire une boucle sur une table !!!!  evec int + next 

//------------------------------------------------
//------------------------------------------------
//------------------------------------------------
// Pour les focntions SECURITY_LEVEL_VIEW
bool 
LuaRegisterOrsServerLib( const char* pData, PLua::PLuaSession&  pLuaSession )
{
	pLuaSession.registerFunction( pData, "CgetServerTimeUTC70",        LUA_GetServerTimeUTC70,     CmdPList::SECURITY_LEVEL_VIEW  );
	pLuaSession.registerFunction( pData, "CgetBaseIdFromObjectId",     LUA_GetBaseFromObjectId,    CmdPList::SECURITY_LEVEL_VIEW );
	pLuaSession.registerFunction( pData, "CgetBaseName",               LUA_GetBaseName,            CmdPList::SECURITY_LEVEL_VIEW );
	pLuaSession.registerFunction( pData, "CgetBaseByName",             LUA_GetBaseByName,          CmdPList::SECURITY_LEVEL_VIEW );
	
	// renvoit les informations sur une base sous forme de Dictionnaire lua
	pLuaSession.registerFunction( pData, "CgetBaseDico",               LUA_GetBaseDico,            CmdPList::SECURITY_LEVEL_VIEW );

	// renvoit les informations sur une table sous forme de Dictionnaire lua
	pLuaSession.registerFunction( pData, "CgetTableDico",              LUA_GetTableDico,           CmdPList::SECURITY_LEVEL_VIEW );

	pLuaSession.registerFunction( pData, "CgetCopyOrsObject",          LUA_GetCopyOrsObject,       CmdPList::SECURITY_LEVEL_VIEW );

	// Recupere la liste des bases sous forme d'id dans la pile
	pLuaSession.registerFunction( pData, "CgetBases",                  LUA_GetBases,               CmdPList::SECURITY_LEVEL_VIEW );

	//Recupere la liste des tables dans une base sous forme d'id dans la pile
	pLuaSession.registerFunction( pData, "CgetTables",                 LUA_GetTables,              CmdPList::SECURITY_LEVEL_VIEW );

	//Recupere la liste des id des OrsObj dans une base/table sous forme de table
 	pLuaSession.registerFunction( pData, "CgetTableContents",          LUA_GetTableContents,       CmdPList::SECURITY_LEVEL_VIEW );


  return true;
}
//------------------------------------------------
}; // fin namespace
