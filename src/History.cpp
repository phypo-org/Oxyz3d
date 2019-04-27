#include "History.h"


#include "Shape/SavRead.h"
#include "Shape/DataBase.h"

#include <memory>

namespace M3d{

	//************************************

	bool History::save( PP3d::DataBase& iData, SaveMode iMode)
	{
		static size_t sId=1; 
		std::unique_ptr<HistoryLink> luLink = std::make_unique<HistoryLink>(sId++);

		luLink->cSaveMode = iMode;
	
		if( iMode == SaveMode::SaveFull )
			{
				PP3d::MySav lSav( luLink->cSavData );
				luLink->cSavId =  PP3d::DataBase::sUniqueId.getCurrentId();
				if( lSav.save( iData ) )
					{
						cLinks.push_back( luLink.release() );
						cCurrentLink = -1;
						return true;
					}
			}

		// sav Saisie
		// sav Selection
		// sav View
		// ...
	
		return false;
	}
	//--------------------------------------
	PP3d::DataBase* History::restore( int iId )
	{
	
		HistoryLink* lLink = nullptr;
		if( iId >= (int)cLinks.size() ||  (lLink = cLinks[iId ]) == nullptr)
			{
				return nullptr;
			}

		if( lLink->cuMyBase.get() != nullptr )
			{
				// the database has already by reloaded 
				return  lLink->cuMyBase.get();
			}
	
		if( lLink->cSaveMode == SaveMode::SaveFull )
			{
				std::unique_ptr<PP3d::DataBase> luBase =  std::make_unique< PP3d::DataBase>();
  
				PP3d::MyRead lRead( lLink->cSavData );
				if( lRead.read( *luBase, true ) == false )
					{
						return nullptr;
					}
				lLink->cuMyBase = luBase.release(); 
				return lLink->cuMyBase.get();    //ATTENTION dans Application c ausi un unique_ptr
			}

		// read Saisie
		// read selection
		// read View
	
		// FAIRE QQ CHOSE
	}
	//--------------------------------------
};
