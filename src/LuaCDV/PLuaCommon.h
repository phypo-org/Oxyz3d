#ifndef H__PLuaCommon__H
#define H__PLuaCommon__H

#include "PPTools/PHashType.h"

#include "MTServer/PLua.h"

#include "OrsObjCommon.h"


using namespace OrsCommon;


namespace PLua {

	//*************************************************
	class PLuaCommon : public PLuaSession {
		
		
	protected:
		
		// Stockage des objets OrsObjCommon
		PPTools::PHashClassPtr<OrsObjCommon*, void*> cStockCopyOrsObjCommon;
		PPTools::PHashClassPtr<OrsObjCommon*, void*> cStockPtrOrsObjCommon;
		
		
		
		PLuaCommon(); // for prototype
		
		//	PLuaCommon( int pSecurityLevel, std::ostrstream* pStream = NULL );
		PLuaCommon( int pSecurityLevel, const char* pNameSession, std::ostrstream* pStream =NULL ); 
		virtual ~PLuaCommon(); // for prototype
		virtual PLuaSession* getNewPrototypeSession( int pSecurityLevel, const char* pNameSession, std::ostrstream* pStream )
		{
			return new PLuaCommon( pSecurityLevel, pNameSession, pStream );
		}
		//------------------------------------------------
		
	public:
		void addCopy( PAutoPtr<OrsObjCommon>& paObj ) { cStockCopyOrsObjCommon.add( paObj.give() ); }
		void add( OrsObjCommon* pObj) { cStockPtrOrsObjCommon.add( pObj ); }


		OrsObjCommon *getStoredOrsObj( int pPosStack  );
		bool clearStoredOrsObj( long pHdl  );

		OrsField* getField( OrsObjCommon & pObj, int pPosStack  );
		OrsField* getField( int pPosStack  );
		bool      pushFieldValue( OrsField* pField );

		
	public :
		inline static void SetPrototype() {
			PLuaSession::SetPrototype( new PLuaCommon() );
		}
		
	};
	//*************************************************
	
	extern bool LuaRegisterOrsCommonLib( const char* pData, PLua::PLuaSession & pLuaSession );


}; // fin namspace

#endif
