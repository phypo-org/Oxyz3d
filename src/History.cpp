#include "History.h"


#include "Shape/SavRead.h"
#include "Shape/DataBase.h"

#include "Application.h"

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
	    //    std::cout << " History::internalSave <<<<<<"
	    //	      <<  luLink->cSavData.str()
	    //	      << ">>>>>>>>>>" << std::endl;
	    
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

    std::cout << " History::internalRestore : " << iId  << std::endl;
    /*    if( lLink->cuMyBase.get() != nullptr )
      {
	// the database has already by reloaded 
	return  lLink->cuMyBase.get();
      }
    */

    //=================
		
    //   if( lLink->cSaveMode == SaveMode::Full )
    //      {
    std::unique_ptr<PP3d::DataBase> luBase( new PP3d::DataBase() );

    // std::cout << " History::internalRestore <<<<<<"
    //	    <<  lLink->cSavData.str()
    //	    << ">>>>>>>>>>" << std::endl;
  
    PP3d::DataBase::sUniqueId.setCurrentId( lLink->cSavId );

	PP3d::MyRead lRead( lLink->cSavData );
	if( lRead.read( *luBase, true ) == false )
	  {
	    return nullptr;
	  }
	//				PP3d::DataBase * lBase =  luBase.release(); 
	
	// read Saisie
	// read selection
	// read View
	
	// FAIRE QQ CHOSE    	lLink->cuMyBase = std::move( luBase );

	//      }	
    //   return lLink->cuMyBase.get();    //ATTENTION dans Application c ausi un unique_ptr
    return luBase.release();
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
  bool History::restore( int lOldPos, int iPos)
  {
    std::cout << "History::restore " << iPos << std::endl;
    // LOCK MUTEX

    
    PP3d::DataBase* lBase = internalRestore( iPos );
    if( lBase == nullptr )
      {
	cCurrentLink = lOldPos;
	std::cout << "History::internalRestore return null" << std::endl;
	return false;
      }

    PP3d::DataBase* lOldBase = Application::Instance().swapBase( lBase );

    if(lOldBase != nullptr )
      {
	delete lOldBase; // pour le moment !
      }


    return true;
  }
  //--------------------------------------
  bool History::reset(  PP3d::DataBase& iBase )
  {
    std::cout << "History::reset" << std::endl;
    // LOCK MUTEX
			
    return true;
  }
  //--------------------------------------
  bool History::undo()
  {
    int lMemoCurrentLink = cCurrentLink;
    
    if( cCurrentLink == -1 )
      {
	cCurrentLink = cLinks.size()-1;
      }
    else
      {
	cCurrentLink--;
      }
    if( cCurrentLink < 0 )
      {
	cCurrentLink = 0;
 	std::cout << "already first SAV" << std::endl;
     }

    if( lMemoCurrentLink != cCurrentLink )      
      return restore( lMemoCurrentLink, cCurrentLink );

    return true;
  }
  //--------------------------------------
  bool History::redo()
  {
     int lMemoCurrentLink = cCurrentLink;
    
    if( cCurrentLink == -1 )
      {
	std::cout << "already Last SAV" << std::endl;
	return true;
      }
    else
      {
	cCurrentLink++;
      }
    if( cCurrentLink >= (int)cLinks.size() )
      cCurrentLink = -1;

    if( lMemoCurrentLink != cCurrentLink )      
      return restore( lMemoCurrentLink, cCurrentLink );
   
    return true;
  }
  //************************************
}
