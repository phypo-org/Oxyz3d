#include "UndoHistory.h"


#include <ostream>
#include <fstream>
#include <istream>
#include <sstream>


#include "SavRead.h"
#include "Selection.h"

namespace PP3d {


  //*************************************
  bool UndoHistory::sav(  DataBase & iDb, Selection *iSel,  const char * iSavName )
  {
    std::ostringstream lOs;
    MySav lSav( lOs );
    
    if( lSav.save( iDb, iSel ) == true )
      {

        if( iSavName != nullptr ){
      
          std::ofstream lOut;						
          lOut.open( iSavName );
          if( lOut.good() )
            {
              std::cout << "<<<" <<  lOs.str()  << ">>>" << std::endl;
              lOut << lOs.str() << std::flush ;
            }
        }
  	
	if( cCurrent >= 0 && cHistoSav.size() && cCurrent != getLast() )
	  {
	    //	    std::cout << "HISTO ERASE " << +cCurrent+1 << std::endl;
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

    //    std::cout << "###################################################" << std::endl;
    //      std::cout <<  "UndoHistory::readPrev "<< cCurrent
    //		<< "<<<<<"<< cHistoSav[cCurrent] << ">>>>>>>>>>>>"
    //		<< std::endl;
    //     std::cout << "###################################################" << std::endl;

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

  void UndoHistory::setMaxHisto( size_t iVal ) {
    if( iVal < 4 )
      iVal = 4;
    
    size_t lOldMaxHisto = cMaxHisto;
    
    cMaxHisto = iVal;

    if( cMaxHisto < lOldMaxHisto )
      {	
	cHistoSav.erase( cHistoSav.begin(), cHistoSav.begin()-(lOldMaxHisto-cMaxHisto));	      cCurrent -= lOldMaxHisto-cMaxHisto;
	if( cCurrent < 0 )
	  cCurrent = 0;
      }
  }

  //*************************************
}
