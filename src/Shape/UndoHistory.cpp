#include "UndoHistory.h"


#include <ostream>
#include <istream>
#include <sstream>


#include "SavRead.h"
#include "Selection.h"

namespace PP3d {


  //*************************************
  bool UndoHistory::sav(  DataBase & iDb, Selection *iSel )
  {
    std::ostringstream lOs;
    MySav lSav( lOs );
    if( lSav.save( iDb, iSel ) == true )
      {	  
	if( cCurrent > 0 && cCurrent != getLast() )
	  {
	    cHistoSav.erase( cHistoSav.begin()+cCurrent+1, cHistoSav.end());
	  }
	  
	if( cHistoSav.size() >= cMaxHisto )
	  {
	    cHistoSav.erase(cHistoSav.begin());
	  }

	cHistoSav.push_back( lOs.str() );
	cCurrent = getLast();
	
	//   std::cout << "###################################################" << std::endl;
	//std::cout <<  "UndoHistory::sav "<< cCurrent << "<<<<<"<< lOs.str() << ">>>>>>>>>>>>" << std::endl;
	// std::cout << "###################################################" << std::endl;

	//	std::cout <<  "UndoHistory::sav <<<<<"<< lOs.str() << ">>>>>>>>>>>>" << std::endl;
	 
	return true;
      }
    return false;
  }
  //------------------------------ 
  bool UndoHistory::readPrev( DataBase & oDb, Selection *ioSel )
  {

    if( cCurrent <= 0 )
      {
	cCurrent = 0;
	return false;
      }
    
    cCurrent--;

    std::cout << "###################################################" << std::endl;
      std::cout <<  "UndoHistory::readPrev "<< cCurrent
		<< "<<<<<"<< cHistoSav[cCurrent] << ">>>>>>>>>>>>"
		<< std::endl;
      std::cout << "###################################################" << std::endl;

    std::istringstream lIs( cHistoSav[cCurrent] );
      
         
    MyRead lRead( lIs );
    return  lRead.read( oDb, ioSel, true );
  }   
  //------------------------------ 
  bool  UndoHistory::readNext( DataBase & oDb, Selection *ioSel )
  {    
    cCurrent++;
    if( cCurrent <= 0 )
      cCurrent = 1;
    
    if( cCurrent >=  (int)cHistoSav.size() )
      {
	cCurrent = getLast();
	return false;
      }
   
    // std::cout << "###################################################" << std::endl;
    // std::cout <<  "UndoHistory:readNext "<< cCurrent << " <<<<<"<< cHistoSav[cCurrent] << ">>>>>>>>>>>>" << std::endl;
    //  std::cout << "###################################################" << std::endl;

    std::istringstream lIs( cHistoSav[cCurrent] );
      
    MyRead lRead( lIs );
    return  lRead.read( oDb, ioSel, true ); 
  }   
  //------------------------------ 
  void UndoHistory::cancelLastSav()
  {
    cHistoSav.pop_back();
    cCurrent = cHistoSav.size();
  }
  //*************************************
}
