#ifndef  H__SavRead__H
#define H__SavRead__H

#include <ostream>
#include <istream>
#include <fstream>
#include <set>

#include "Entity.h"

namespace PP3d {

  class DataBase;
  class Selection;
	
  //*************************************
  class MySav{

    std::ostream & cOut;

  public:
    MySav( std::ostream& pOut )
      :cOut( pOut )
    {;}

    bool save( DataBase& pData, bool iFlagSavGroup, Selection * iSel= nullptr, std::set<Entity*> * iFilter = nullptr );

    static bool Save( const char* iFileIn, PP3d::DataBase& iDatabase, bool iFlagSavGroup, Selection * iSel )
    {
      std::ofstream lFilSav;						
      lFilSav.open( iFileIn );
      if( lFilSav.good() )
	{	  
	  MySav lSav( lFilSav );	  	  
	  return lSav.save( iDatabase, iFlagSavGroup, iSel );	  
	}
      return false;
    }
  };
  //*************************************
  class MyRead{

    std::istream& cIn;
    std::vector<EntityPtr> *cCreateResult;

  public:
    MyRead( std::istream& pIn, std::vector<EntityPtr> *ioCreateResult = nullptr )
      :cIn( pIn )
      ,cCreateResult(ioCreateResult)
    {
    }	  
    bool read( DataBase& pData, Selection * iSel= nullptr, bool lConserverOldId = false );
    
    static bool Read( const char* iFileIn, DataBase& oDatabase, Selection * iSel, bool iConserverOldId = false  )
    {
      std::ifstream lFileIn;						
      lFileIn.open( iFileIn  );
      
      if( lFileIn.good() )
	{	  
	  PP3d::MyRead lRead( lFileIn);
	  return lRead.read(  oDatabase, iSel, iConserverOldId);	  
	}
      return false;
    }
  };
	
  //*************************************

	
}


#endif
