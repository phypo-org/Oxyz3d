#ifndef  H__SavRead__H
#define H__SavRead__H

#include <ostream>
#include <istream>
#include <fstream>

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

    static bool Save( const char* iFileIn, PP3d::DataBase& iDatabase )
    {
      std::ofstream lFilSav;						
      lFilSav.open( iFileIn );
      if( lFilSav.good() )
	{	  
	  MySav lSav( lFilSav );	  	  
	  return lSav.save( iDatabase );	  
	}
      return false;
    }
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
    
    static bool Read( const char* iFileIn, DataBase& oDatabase, bool iConserverOldId = false  )
    {
      std::ifstream lFileIn;						
      lFileIn.open( iFileIn  );
      
      if( lFileIn.good() )
	{	  
	  PP3d::MyRead lRead( lFileIn);
	  return lRead.read(  oDatabase, iConserverOldId);	  
	}
      return false;
    }
  };
	
  //*************************************

	
}


#endif
