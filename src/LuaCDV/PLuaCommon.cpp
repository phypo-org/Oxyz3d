#include "PLuaCommon.h"

#include "OrsCommon.h"


#include "ptools/PXmlDef.h"


using namespace OrsCommon;


namespace PLua {
	

	static unsigned long HashPtr( OrsObjCommon* A )
	{
		return (unsigned long )A;
	}
	static long CmpPtr( OrsObjCommon* A, OrsObjCommon* B, void* )
	{
		return (((long)A)-((long)B));
	}
	static unsigned long HashSearchPtr( void* pVal )
	{
		return (unsigned long )pVal;
	}
	
	static long SearchPtr( OrsObjCommon* A, void* pVal, void* )
	{
		return (((long)A)-((long)pVal));
	}
	
//*************************************************
	PLuaCommon::PLuaCommon( ) 
		: PLuaSession(),
			cStockCopyOrsObjCommon( HashPtr, CmpPtr, 1024, HashSearchPtr, SearchPtr  ),
			cStockPtrOrsObjCommon( HashPtr, CmpPtr, 1024, HashSearchPtr, SearchPtr )
	{
		OS_TRACE( "PLuaCommon::PLuaCommon 1 Prototype" );
	}
	//------------------------------------------------
	PLuaCommon::PLuaCommon( int pSecurityLevel, const char* pNameSession, std::ostrstream* pStream ) 
		: PLuaSession( pSecurityLevel, pNameSession, pStream),
			cStockCopyOrsObjCommon( HashPtr, CmpPtr, 1024, HashSearchPtr, SearchPtr  ),
			cStockPtrOrsObjCommon( HashPtr, CmpPtr, 1024, HashSearchPtr, SearchPtr  )
	{
		OS_TRACE( "PLuaCommon::PLuaCommon 2" );
	}
	//------------------------------------------------
	PLuaCommon::~PLuaCommon()
	{
		OS_TRACE( "PLuaCommon::~PLuaCommon" );
		cStockCopyOrsObjCommon.deleteAllObject(); 
		cStockPtrOrsObjCommon.clear(); 
	}
 	//------------------------------------------------
	//------------------------------------------------
	//------------------------------------------------
	OrsObjCommon *
	PLuaCommon::getStoredOrsObj( int pPosStack  )
	{
		if( lua_isnumber(cLuaState, pPosStack ) )
			{
				int lOrsObjComHdl = -1;
				lua_number2int( lOrsObjComHdl, lua_tonumber( cLuaState, pPosStack) );
				if( lOrsObjComHdl >= 0 ) 
					{ 
						OrsObjCommon* lRes = cStockCopyOrsObjCommon.get( (OrsObjCommon*)lOrsObjComHdl ); 
						if( lRes == NULL )
							lRes = cStockPtrOrsObjCommon.get( (OrsObjCommon*)lOrsObjComHdl ); 
						return lRes;
					}
			}
		
		return NULL;
	}
	//------------------------------------------------
	bool	
	PLuaCommon::clearStoredOrsObj( long pHdl  )
	{
		if( lua_isnumber(cLuaState, 1 ) )
			{
				int lOrsObjComHdl = -1;
				lua_number2int( lOrsObjComHdl, lua_tonumber( cLuaState, 1) );
				if( lOrsObjComHdl >= 0 )
					{
						return cStockCopyOrsObjCommon.removeSearch( (OrsObjCommon*)pHdl )
							|| cStockPtrOrsObjCommon.removeSearch( (OrsObjCommon*)pHdl );
					}
			}
		
		return false;
	}
	//------------------------------------------------
	OrsField*
	PLuaCommon::getField( OrsObjCommon & pObj, int pPosStack  )
	{
		if( lua_isnumber(cLuaState, pPosStack ) )
			{
				int lFieldPos = -1;
				lua_number2int( lFieldPos, lua_tonumber( cLuaState, pPosStack) );
				if( lFieldPos >=0 && lFieldPos < pObj.getFieldCount() )
					{
						OrsField& lField = pObj.getField(lFieldPos);			 				
						return &lField;
					}
			}
		return NULL;
	}
	//------------------------------------------------
	OrsField*
	PLuaCommon::getField( int pPosStack  )
	{
		OrsObjCommon * lOrsObjCom= getStoredOrsObj( pPosStack );
		if( lOrsObjCom )
			{
				return getField( *lOrsObjCom, pPosStack+1 );
			}
		return NULL;
	}
	
	//------------------------------------------------
	bool 
	PLuaCommon::pushFieldValue( OrsField* pField )
	{			
		switch( pField->getType() )
			{
			case FIELD_TYPE_BOOLEAN:  	
				lua_pushboolean( cLuaState, pField->getInt16(0));
				break;	 
			case FIELD_TYPE_CHAR:       
				lua_pushinteger( cLuaState, pField->getChar(0));
					break;
			case FIELD_TYPE_INT16: 
				lua_pushinteger( cLuaState, pField->getInt16(0));
				break;
			case FIELD_TYPE_INT32:
					lua_pushinteger( cLuaState, pField->getInt32(0));
					break;
					//			case FIELD_TYPE_INT64:  
					//				lua_pushinteger( cLuaState, pField->getInt64(0));
					//				break;
				
			case FIELD_TYPE_FLOAT:      
					lua_pushnumber( cLuaState, pField->getFloat(0));
					break;
			case FIELD_TYPE_DOUBLE:     
				lua_pushnumber( cLuaState, pField->getDouble(0));
				break;
			case FIELD_TYPE_LONG_DOUBLE:
				lua_pushnumber( cLuaState, pField->getLongDouble(0));
				break;
				
			case FIELD_TYPE_STRING_DYN_FIX:   
			case FIELD_TYPE_STRING_DYN:
				case FIELD_TYPE_STRING_FIX: 
					lua_pushstring(  cLuaState, pField->getString(0)); 
					break;
			default:				
				return false;
			}
		return true;
	}


	//------------------------------------------------
	//------------------------------------------------
	//------------------------------------------------
	//------------------------------------------------
	// Renvoit le nombre de champs d'un objet Ors
	
	CLUA_OPEN_CODE( LUA_GetOrsObjFieldCount, 1 );
	
	int lResult = -1;
	
	OrsObjCommon * lOrsObjCom= ((PLuaCommon*)&lLuaSession)->getStoredOrsObj( 1 );
	
	if( lOrsObjCom != NULL )
		lResult = lOrsObjCom->getFieldCount();
	
	lua_pushinteger( pLua, lResult );
	
	CLUA_CLOSE_CODE(1);

	
	//------------------------------------------------
	// Renvoit le type du champ n d'un objet Ors
 
	
	CLUA_OPEN_CODE(LUA_GetOrsObjFieldType, 2 );
	
	int lResult = -1;
	
	OrsField* lField = ((PLuaCommon*)&lLuaSession)->getField( 1  );
	if( lField != NULL )
	{
		lResult =  lField->getType();
	}
	
	lua_pushinteger( pLua, lResult );
	
	CLUA_CLOSE_CODE(1);

	//------------------------------------------------
	// Renvoit la valeur d'un champs d'un objet Ors

	CLUA_OPEN_CODE(LUA_GetOrsObjStringFieldValue, 2 );
	
	
	int lResult = -1;
	
	OrsField* lField = ((PLuaCommon*)&lLuaSession)->getField( 1  );
	if( lField == NULL ) 
		lua_pushnil(  pLua ); 
	else
	{	
		if( lField->isNull(0) )
			lua_pushstring( pLua, "" );
		else 
			{
				std::ostrstream lStrStream;
				lField->print( lStrStream, 0 );
				lStrStream << std::ends;
				
				char* lTmpStr = lStrStream.str();
				if( lTmpStr == NULL )
					lua_pushstring(  pLua, "" );
				else {
					lua_pushstring( pLua, lTmpStr );
					delete [] lTmpStr;
				}
			}
	}
	
	CLUA_CLOSE_CODE(1);

	//------------------------------------------------
	// Renvoit la valeur une proprietes

	CLUA_OPEN_CODE(LUA_GetOrsObjFieldName, 2 );
	
	const char *lResult = NULL;
	OrsObjCommon * lOrsObjCom= ((PLuaCommon*)&lLuaSession)->getStoredOrsObj( 1 );
	
	if( lOrsObjCom != NULL )
	{
		if( lua_isnumber( pLua, 2 ) )
			{
				int lFieldPos = -1;
				lua_number2int( lFieldPos, lua_tonumber( pLua, 2) );
				if( lFieldPos >=0 && lFieldPos < lOrsObjCom->getFieldCount() )
					{
						lResult = (const char*)lOrsObjCom->getFieldPropsByPos( lFieldPos,  PXml::XML_ORS_FIELD_NAME);
					} 
			}
	}

	if( lResult != NULL )
		lua_pushstring( pLua, lResult );
	else
	  lua_pushnil( pLua ); 
	

	CLUA_CLOSE_CODE(1);

	//------------------------------------------------
	// Renvoit la valeur d'un champs d'un objet Ors

	CLUA_OPEN_CODE(LUA_GetOrsObjFieldValue, 2 );
	
	
	int lResult = -1;
	
	OrsField* lField = ((PLuaCommon&)lLuaSession).getField( 1  );
	if( lField == NULL ) 
		lua_pushnil( pLua ); 
	else
	{	
		if( ((PLuaCommon&)lLuaSession).pushFieldValue(lField ) == false )
			{
				OS_ERROR( ERR_GRAV_MIDDLE, LOG_SYBASE, 0, "PLuaCommon::LUA_GetOrsObjFieldValue unknown type  for " << lField );
				lua_pushnil( pLua  ); 
			}
	}
	
	CLUA_CLOSE_CODE(1);
	//------------------------------------------------
	// Renvoit la valeur d'un objet Ors sous forme de table lua

	CLUA_OPEN_CODE(LUA_GetOrsObjTable, 1 );
	//			OS_TRACE( "LUA_GetOrsObjTable" );

	OrsObjCommon * lOrsObjCom= ((PLuaCommon&)lLuaSession).getStoredOrsObj( 1 );
	if( lOrsObjCom == NULL )
	{
		OS_TRACE( "LUA_GetOrsObjTable NIL" );
		lua_pushnil( pLua ); 
	}
	else 
	{
		//	lua_createtable ( pLua, 0, lOrsObjCom->getFieldCount() );
		//		int lPosTable = lua_gettop( pLua ); 
		OS_TRACE( "LUA_GetOrsObjTable " << lOrsObjCom->getFieldCount() );

		lua_checkstack( pLua, lOrsObjCom->getFieldCount()+10 );

		for( int lFieldPos=0; lFieldPos < lOrsObjCom->getFieldCount(); lFieldPos++ )
			{
				OrsField& lField = lOrsObjCom->getField(lFieldPos);			 				
				//				const char* lFieldName = lOrsObjCom->getFieldPropsByPos( lFieldPos,  PXml::XML_ORS_FIELD_NAME);
				//		lua_pushstring( pLua, lFieldName );
				((PLuaCommon&)lLuaSession).pushFieldValue( &lField );
				//		lua_setfield( pLua, lPosTable, lFieldName );
				OS_TRACE( "LUA_GetOrsObjTable "<< lFieldPos << "=" << lField );
			}
		_NbReturn =lOrsObjCom->getFieldCount();
		//	lua_error( pLua );	
	}
	
	CLUA_CLOSE_CODE(0); // Utilise _NbReturn
	//------------------------------------------------
	// Renvoit la valeur d'un objet Ors sous forme de table lua

	CLUA_OPEN_CODE(LUA_GetOrsObjDico, 1 );

	//				OS_TRACE( "LUA_GetOrsObjDico" );

	OrsObjCommon * lOrsObjCom= ((PLuaCommon&)lLuaSession).getStoredOrsObj( 1 );
	if( lOrsObjCom == NULL )
	{
		OS_TRACE( "LUA_GetOrsObjDico NIL" );
		lua_pushnil( pLua ); 
	}
	else 
	{
		lua_createtable ( pLua, 0, lOrsObjCom->getFieldCount() );
		int lPosTable = lua_gettop( pLua ); 
				OS_TRACE( "LUA_GetOrsObjDico " << lOrsObjCom->getFieldCount() );

		for( int lFieldPos=0; lFieldPos < lOrsObjCom->getFieldCount(); lFieldPos++ )
			{
				OrsField& lField = lOrsObjCom->getField(lFieldPos);			 				
				const char* lFieldName = lOrsObjCom->getFieldPropsByPos( lFieldPos,  PXml::XML_ORS_FIELD_NAME);
				//			lua_pushstring( pLua, lFieldName );
				((PLuaCommon&)lLuaSession).pushFieldValue( &lField );
				lua_setfield( pLua, lPosTable, lFieldName );
				//			OS_TRACE( "LUA_GetOrsObjDico "<< lPosTable << " " <<  lFieldName << "=" << lField );
			}
		//	OS_TRACE( "LUA_GetOrsObjDico top:" << lua_gettop( pLua ) );
		//	lua_error( pLua );	
	}
	
	CLUA_CLOSE_CODE(1);
	//------------------------------------------------
	// Enleve l'objet Ors de la memoire de la session
	
	CLUA_OPEN_CODE( LUA_ClearStoredOrsObj, 1 );
	
	bool lResult = false;
	
	OrsObjCommon * lOrsObjCom= ((PLuaCommon*)&lLuaSession)->getStoredOrsObj( 1 );
	
	if( lOrsObjCom != NULL )
		lResult = ((PLuaCommon*)&lLuaSession)->clearStoredOrsObj((long)lOrsObjCom);
		
	lua_pushboolean( pLua, lResult );
	
	CLUA_CLOSE_CODE(1); 

	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	// Implementation des Fonctions disponibles dans OrsCommon 
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	// Renvoie le nombre de type d'objets OrsObj total

	CLUA_OPEN_CODE( LUA_GetNbTypeOrsObj, 0 );
	
	if( OrsCommon::OrsCommonBase::sTheOrsCommon )
		lua_pushinteger( pLua, OrsCommon::OrsCommonBase::sTheOrsCommon->getNbObject() );
	else
  	lua_pushinteger( pLua, 0); 
	
	
	CLUA_CLOSE_CODE(1); 

	//------------------------------------------------
	// Renvoie le type d'objet pour le n iéme objets de la liste

	CLUA_OPEN_CODE( LUA_GetTypeOrsObj, 1 );

	OrsObjId  lResult = -1;
	
	if(  lua_isnumber( pLua, 1 ) )
	{
		int lPos = -1;
		lua_number2int( lPos, lua_tonumber( pLua, 1) );		
		if( OrsCommon::OrsCommonBase::sTheOrsCommon )
			lResult =  OrsCommon::OrsCommonBase::sTheOrsCommon->getObjectType( lPos );
	}
	lua_pushinteger( pLua, (long)lResult );
		
	CLUA_CLOSE_CODE(1); 
	//------------------------------------------------
	CLUA_OPEN_CODE( LUA_GetNameOrsObj, 1 );

	const char* lStr = NULL;
	
	if(  lua_isnumber( pLua, 1 ) )
	{	
	int lType = -1;
		lua_number2int( lType, lua_tonumber( pLua, 1) );
		
		if( OrsCommon::OrsCommonBase::sTheOrsCommon )
			lStr = OrsCommon::OrsCommonBase::sTheOrsCommon->getObjectName( lType );
	}
	if( lStr )
		lua_pushstring( pLua, lStr );
	else
	  lua_pushnil( pLua );
	
	CLUA_CLOSE_CODE(1); 
	
	//------------------------------------------------
	CLUA_OPEN_CODE( LUA_GetSqlTableNameOrsObj, 1 );
	
	const char* lStr = NULL;
	
	if(  lua_isnumber( pLua, 1 ) )
	{
		int lType = -1;
		lua_number2int( lType, lua_tonumber( pLua , 1) );

		if( OrsCommon::OrsCommonBase::sTheOrsCommon )
			lStr = OrsCommon::OrsCommonBase::sTheOrsCommon->getObjectSqlTableName( lType );
	}
	if( lStr )
		lua_pushstring( pLua, lStr );
	else
	  lua_pushnil( pLua );

	
	CLUA_CLOSE_CODE(1); 
	//------------------------------------------------
	CLUA_OPEN_CODE( LUA_GetSqlBaseNameOrsObj, 1 );
	
	const char* lStr = NULL;
	
	if(  lua_isnumber( pLua, 1 ) )
	{
		int lType = -1;
		lua_number2int( lType, lua_tonumber( pLua, 1) );
		
		if( OrsCommon::OrsCommonBase::sTheOrsCommon )
			lStr = OrsCommon::OrsCommonBase::sTheOrsCommon->getObjectSqlBaseName( lType );
	}
	if( lStr )
		lua_pushstring( pLua, lStr );
	else
	lua_pushnil( pLua );
	
	 CLUA_CLOSE_CODE(1); 
	//------------------------------------------------
	// Renvoit un objet Ors sous forme de table lua

	CLUA_OPEN_CODE(LUA_GetInfoOrsObjDico, 1 );

	if(  lua_isnumber( pLua, 1 ) )
	{
		int lType = -1;
		lua_number2int( lType, lua_tonumber( pLua, 1) );
		if( OrsCommon::OrsCommonBase::sTheOrsCommon && lType >= 0 )
			{
				const char*   lStr;
				OrsTypeObject lInt; 

				lStr  = OrsCommon::OrsCommonBase::sTheOrsCommon->getObjectName( lType );	

				if( lStr != NULL )
					{
						lua_createtable ( pLua, 0, 5 );
						int lPosTable = lua_gettop( pLua ); 
						
						lua_pushstring( pLua, lStr);
						lua_setfield( pLua, lPosTable, "Name" );
						
						lInt = OrsCommon::OrsCommonBase::sTheOrsCommon->getObjectType( lType );
						lua_pushinteger( pLua, lInt );
						lua_setfield( pLua, lPosTable, "Type" );
						
						lStr  = OrsCommon::OrsCommonBase::sTheOrsCommon->getObjectSqlBaseName( lType );
						if( lStr )
							{
								lua_pushstring( pLua, lStr);
								lua_setfield( pLua, lPosTable, "SqlBaseName" );
							}
						
						lStr  = OrsCommon::OrsCommonBase::sTheOrsCommon->getObjectSqlTableName( lType );
						if( lStr )
							{
								lua_pushstring( pLua, lStr);
								lua_setfield( pLua, lPosTable, "SqlName" );
							}
						
						OrsCommon::OrsObjCommon*	lOrsObj = OrsCommon::OrsCommonBase::sTheOrsCommon->getObject( lType );
						if( lOrsObj )
							{
								lInt = lOrsObj->getFieldCount();
								lua_pushinteger( pLua, lInt );
								lua_setfield( pLua, lPosTable, "FieldCount" );
							}				
					}
				else
					lua_pushnil( pLua );				
			}
		else
				lua_pushnil( pLua );
	}
	else
	lua_pushnil( pLua );
	
	CLUA_CLOSE_CODE(1);

	//------------------------------------------------
	//------------------------------------------------
	//------------------------------------------------
	bool 
	LuaRegisterOrsCommonLib( const char* pData,  PLuaSession&  pLuaSession )
	{
		pLuaSession.registerFunction( pData, "CgetOrsObjFieldCount",     LUA_GetOrsObjFieldCount,      CmdPList::SECURITY_LEVEL_VIEW );
		pLuaSession.registerFunction( pData, "CgetOrsObjFieldType",      LUA_GetOrsObjFieldType,       CmdPList::SECURITY_LEVEL_VIEW );
		pLuaSession.registerFunction( pData, "CgetOrsObjFieldValue",     LUA_GetOrsObjFieldValue,      CmdPList::SECURITY_LEVEL_VIEW );
		pLuaSession.registerFunction( pData, "CgetOrsObjFieldName",      LUA_GetOrsObjFieldName,       CmdPList::SECURITY_LEVEL_VIEW );
		pLuaSession.registerFunction( pData, "CgetOrsObjStringFieldValue", LUA_GetOrsObjStringFieldValue, CmdPList::SECURITY_LEVEL_VIEW );


		pLuaSession.registerFunction( pData, "CgetOrsObjTable",  LUA_GetOrsObjTable, CmdPList::SECURITY_LEVEL_VIEW );
		pLuaSession.registerFunction( pData, "CgetOrsObjDico",   LUA_GetOrsObjDico, CmdPList::SECURITY_LEVEL_VIEW );

		pLuaSession.registerFunction( pData, "CclearOrsObj",             LUA_ClearStoredOrsObj,        CmdPList::SECURITY_LEVEL_VIEW );
		
		pLuaSession.registerFunction( pData, "CgetGlobalOrsObjCount",       LUA_GetNbTypeOrsObj,          CmdPList::SECURITY_LEVEL_VIEW );
		pLuaSession.registerFunction( pData, "CgetGlobalOrsObjType",        LUA_GetTypeOrsObj,            CmdPList::SECURITY_LEVEL_VIEW );
		pLuaSession.registerFunction( pData, "CgetGlobalOrsObjName",        LUA_GetNameOrsObj,            CmdPList::SECURITY_LEVEL_VIEW );
		pLuaSession.registerFunction( pData, "CgetGlobalOrsObjSqlTablename",   LUA_GetSqlTableNameOrsObj,    CmdPList::SECURITY_LEVEL_VIEW );
		pLuaSession.registerFunction( pData, "CgetGlobalOrsObjSqlBasename",    LUA_GetSqlBaseNameOrsObj,     CmdPList::SECURITY_LEVEL_VIEW );

		pLuaSession.registerFunction( pData, "CgetGlobalOrsObj",            LUA_GetInfoOrsObjDico,     CmdPList::SECURITY_LEVEL_VIEW );

		return true;
	}		
};  // fin nanmespace

//*************************************************
