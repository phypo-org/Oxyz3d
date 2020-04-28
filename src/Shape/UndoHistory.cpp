#include "UndoHistory.h"


#include <ostream>
#include <istream>
#include <sstream>


#include "SavRead.h"

namespace PP3d {


  //*************************************
  bool UndoHistory::sav(  DataBase & iDb )
  {
    std::ostringstream lOs;
    MySav lSav( lOs );
    if( lSav.save( iDb ) == true )
      {
	if( cHistoSav.size() > cMaxHisto )
	  {
	    cHistoSav.erase(cHistoSav.begin());
	  }
	  
	if( cCurrent != getLast() )
	  {
	    cHistoSav.erase( cHistoSav.begin()+cCurrent, cHistoSav.end() );
	  }
	  
	cHistoSav.push_back( lOs.str() );
	cCurrent = cHistoSav.size()-1;

	//	std::cout <<  "UndoHistory::sav <<<<<"<< lOs.str() << ">>>>>>>>>>>>" << std::endl;
	 
	return true;
      }
    return false;
  }
  //------------------------------
  bool UndoHistory::read( DataBase & oDb, int iPos ) //-1 the last !
  {
    if( iPos == -1 || iPos >= (int)cHistoSav.size() )
      {
	iPos = getLast();
      }
      
    if( iPos == -1 )
      {
	return false;
      }

    std::istringstream lIs( cHistoSav[iPos] );
      
    cCurrent = iPos;
      
    MyRead lRead( lIs );
    return  lRead.read( oDb, false ); 
  }
  //------------------------------ 
  bool  UndoHistory::readCurrent( DataBase & oDb )
  {      
    if( cCurrent <= -1  )
      {
	cCurrent = 0;
 	return false;
     }
 
    
    //   std::cout <<  "UndoHistory::readCurrent "<< cCurrent << "<<<<<"<< cHistoSav[cCurrent] << ">>>>>>>>>>>>" << std::endl;

    std::istringstream lIs( cHistoSav[cCurrent] );
      
    cCurrent--;
      
    MyRead lRead( lIs );
    return  lRead.read( oDb, false ); 
  }   
  //------------------------------ 
  bool  UndoHistory::readNext( DataBase & oDb )
  {    
    cCurrent++;
    if( cCurrent == 0 )
      cCurrent = 1;
    
    if( cCurrent >=  (int)cHistoSav.size() )
      cCurrent = getLast();
   
    if( cCurrent <= -1  )
      {
	cCurrent = getLast();
 	return false;
     }
 
    //    std::cout <<  "UndoHistory:readNext "<< cCurrent << " <<<<<"<< cHistoSav[cCurrent] << ">>>>>>>>>>>>" << std::endl;

    std::istringstream lIs( cHistoSav[cCurrent] );
      
    MyRead lRead( lIs );
    return  lRead.read( oDb, false ); 
  }   
  //------------------------------ 
  void UndoHistory::cancelLastSav()
  {
    cHistoSav.pop_back();
    cCurrent = cHistoSav.size();
  }
  //*************************************
}
