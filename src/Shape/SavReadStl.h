#ifndef  H__SavReadStl__H
#define H__SavReadStl__H

#include <ostream>
#include <istream>
#include <fstream>
#include <set>

#include "Entity.h"

namespace PP3d {

  class DataBase;
  class Selection;
	
  //*************************************
  class MySavStl{

    std::ostream & cOut;

  public:
    //-----------------------------------------
    MySavStl( std::ostream& pOut )
      :cOut( pOut )
    {;}
    
    bool saveTxt(  const char* iName, std::vector<FacetPtr>  cVectFacets );
    
    static bool Save( const char* iFileIn, DataBase & iDatabase, bool iTxt = true  ){
      return MySavStl::Save(iFileIn, & iDatabase, nullptr, iTxt );
    }
    static bool Save( const char* iFileIn, Selection & iSelect, bool iTxt = true ){
      return MySavStl::Save(iFileIn, nullptr, & iSelect, iTxt );
    }
    
  private:
    static bool Save( const char* iFileIn,
		      DataBase *  iDatabase,
		      Selection * iSelect,
		      bool iTxt );
  };
    
  //*************************************
  class MyReadStl{

    std::istream& cIn;
    std::vector<EntityPtr> *cCreateResult;

  public:
    MyReadStl( std::istream& pIn, std::vector<EntityPtr> *ioCreateResult = nullptr )
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
	  PP3d::MyReadStl lRead( lFileIn);
	  return lRead.read(  oDatabase, iSel, iConserverOldId);	  
	}
      return false;
    }
  };
	
  //*************************************

	
}


#endif
