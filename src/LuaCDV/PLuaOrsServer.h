#ifndef H__PLuaFtn__H
#define H__PLuaFtn__H

//#include "MTServer/PLua.h"
#include "OrsCommon/PLuaCommon.h"


namespace PLua {
	extern bool LuaRegisterOrsServerLib( const char * pData, PLua::PLuaSession & pLuaSession );
	
	class PLuaOrsServer : public PLuaCommon {
		
		PLuaOrsServer(){
			OS_TRACE( "PLuaOrsServer::PLuaOrsServer 1 Prototype" );

	};
		
		PLuaOrsServer( int pSecurityLevel, const char* pNameSession, std::ostrstream* pStream =NULL )
			:PLuaCommon(  pSecurityLevel,  pNameSession, pStream  )
		{
			OS_TRACE( "PLuaOrsServer::PLuaOrsServer 3" );
		}
		//------------------------------------------------
		PLuaSession* getNewPrototypeSession( int pSecurityLevel, const char* pNameSession, std::ostrstream* pStream )
		{
			return new PLuaOrsServer( pSecurityLevel, pNameSession, pStream );
		}

		
	public :
		inline static void SetPrototype() {
			OS_TRACE( "SetPrototype" );
			PLuaSession::SetPrototype(	new PLuaOrsServer() );
		}
	};
};

#endif
