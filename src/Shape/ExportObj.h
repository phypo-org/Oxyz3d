#ifndef  H__ExportObj__H
#define  H__ExportOb__H

#include <ostream>
#include <istream>
#include <unordered_map>


#include "Entity.h"

// Wavefront 3D Object File

namespace PP3d {

	class DataBase;
	
	//*************************************
	class MyExportObj{


	public:
		std::ostream & cOut;

		std::unordered_map<EntityId,EntityId>  cHashId;
		EntityId                               cCurrentPointId=1;
											 

		MyExportObj( std::ostream& pOut )
			:cOut( pOut )
		{;}

		bool save( DataBase& pData );		
	};
	//*************************************
	class MyImportObj{

		std::istream& cIn;

	public:
		MyImportObj( std::istream& pIn )
			:cIn( pIn )
		{
		}
		bool read( DataBase& pData, bool lConserverOldId = false );
	};
	
	//*************************************

	
};


#endif
