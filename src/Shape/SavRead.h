#ifndef  H__SavRead__H
#define H__SavRead__H

#include <ostream>
#include <istream>

namespace PP3d {

	class DataBase;
	
	//*************************************
	class MySav{

		std::ostream & cOut;

	public:
		MySav( std::ostream& pOut )
			:cOut( pOut )
		{;}

		bool save( DataBase& pData );		
	};
	//*************************************
	class MyRead{

		std::istream& cIn;

	public:
		MyRead( std::istream& pIn )
			:cIn( pIn )
		{
		}
		bool read( DataBase& pData, bool lConserverOldId = false );
	};
	
	//*************************************

	
}


#endif
