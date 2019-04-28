#include "History.h"


#include "Shape/SavRead.h"
#include "Shape/DataBase.h"

#include <memory>

namespace M3d{

	//************************************

	bool History::internalSave( PP3d::DataBase& iData, SaveMode iMode)
	{
		static size_t sId=1; 
		std::unique_ptr<HistoryLink> luLink = std::make_unique<HistoryLink>(sId++);

		luLink->cSaveMode = iMode;
	
		if( iMode == SaveMode::Full
				|| iMode == SaveMode::Diff ) // a virer plus tard 
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
	PP3d::DataBase* History::internalRestore( int iId )
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

		//=================
		
		if( lLink->cSaveMode == SaveMode::Full )
			{
				std::unique_ptr<PP3d::DataBase> luBase =  std::make_unique< PP3d::DataBase>();
  
				PP3d::MyRead lRead( lLink->cSavData );
				if( lRead.read( *luBase, true ) == false )
					{
						return nullptr;
					}
				//				PP3d::DataBase * lBase =  luBase.release(); 
	
				// read Saisie
				// read selection
				// read View
	
				// FAIRE QQ CHOSE			lLink->cuMyBase = std::move( luBase );

			}	
		return lLink->cuMyBase.get();    //ATTENTION dans Application c ausi un unique_ptr
	}
 
	//--------------------------------------
	bool History::save(  PP3d::DataBase& iBase, SaveMode iSaveMode)
	{
			
		// AFAIRE
		// LOCK MUTEX
		// LANCER DANS UN THREAD INDEPENDANT
		// FAIRE UNE PURGE EVENTUELLE SI TROP RESSOURCE UTILISE
		// OU DEPASSEMENT DE cMaxDepthness
			
		if( iSaveMode == SaveMode::NoSav )
			return true;
			
		return internalSave( iBase, iSaveMode );
	}
	//--------------------------------------
	bool History::restore( int iPos)
	{
		// LOCK MUTEX
		return internalRestore( iPos );
	}
	//--------------------------------------
	bool History::reset(  PP3d::DataBase& iBase )
	{
		// LOCK MUTEX
			
		return true;
	}
	//************************************
}
