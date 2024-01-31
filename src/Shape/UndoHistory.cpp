#include "UndoHistory.h"


#include <ostream>
#include <fstream>
#include <istream>
#include <sstream>


#include "SavRead.h"
#include "Selection.h"

#include "Utils/PPErrLog.h"

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
              VERBOSELN4( "UndoHistory::sav " <<  lOs.str() );
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

        { //==== LOCK  LOCK  LOCK  LOCK  LOCK  LOCK  LOCK  LOCK ====
          std::unique_lock<std::mutex> lock(cMyMutex);
          
          cHistoSav.push_back( lOs.str() );
          cCurrent = getLast();
	
          cLastTimeSav = PPu::PPDate::GetCurrentDateTimeFloat70();
        }//===== UNLOCK  UNLOCK  UNLOCK  UNLOCK  UNLOCK  UNLOCK ====
         
        
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
   { //===== LOCK  LOCK  LOCK  LOCK  LOCK  LOCK  LOCK  LOCK =====
      std::unique_lock<std::mutex> lock(cMyMutex);

      
      if( cCurrent <= 0 )
        {
          cCurrent = 0;
          return false;
        }

      cLastTimeSav = PPu::PPDate::GetCurrentDateTimeFloat70();
    
      cCurrent--;
  
    }//===== UNLOCK  UNLOCK  UNLOCK  UNLOCK  UNLOCK  UNLOCK =====


    

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
    { //==== LOCK  LOCK  LOCK  LOCK  LOCK  LOCK  LOCK  LOCK ====
      std::unique_lock<std::mutex> lock(cMyMutex);

      cCurrent++;
    if( cCurrent <= 0 )
      cCurrent = 1;
    
    if( cCurrent >=  (int)cHistoSav.size() )
      {
	cCurrent = getLast();
	return false;
      }
    
    cLastTimeSav = PPu::PPDate::GetCurrentDateTimeFloat70();
  
    }//==== UNLOCK  UNLOCK  UNLOCK  UNLOCK  UNLOCK  UNLOCK ====

   
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
    { //==== LOCK  LOCK  LOCK  LOCK  LOCK  LOCK  LOCK  LOCK ====
      std::unique_lock<std::mutex> lock(cMyMutex);

      cHistoSav.pop_back();
      cCurrent = cHistoSav.size();
      
    }//==== UNLOCK  UNLOCK  UNLOCK  UNLOCK  UNLOCK  UNLOCK ====
  }
  //------------------------------ 

  void UndoHistory::setMaxHisto( size_t iVal ) {
    if( iVal < 4 )
      iVal = 4;

    if( iVal >1024 )
      iVal = 1024;
    
    size_t lOldMaxHisto = cMaxHisto;
    
    cMaxHisto = iVal;

    if( cMaxHisto < lOldMaxHisto )
      { //==== LOCK  LOCK  LOCK  LOCK  LOCK  LOCK  LOCK  LOCK ====      
        std::unique_lock<std::mutex> lock(cMyMutex);


      cHistoSav.erase( cHistoSav.begin(), cHistoSav.begin()-(lOldMaxHisto-cMaxHisto));	      cCurrent -= lOldMaxHisto-cMaxHisto;
	if( cCurrent < 0 )
	  cCurrent = 0;
      } //==== UNLOCK  UNLOCK  UNLOCK  UNLOCK  UNLOCK  UNLOCK ====
  }
  //------------------------------ 
  bool UndoHistory::externalThread_getSav(  PPu::PPDateTimeFloat70 iLastTimeSav, std::string & oResult )
  {
    //==== LOCK  LOCK  LOCK  LOCK  LOCK  LOCK  LOCK  LOCK ====
    std::unique_lock<std::mutex> lock(cMyMutex);

    VERBOSELN3( "UndoHistory::externalThread_getSav last sav:" <<  iLastTimeSav
             << " cLastTimeSav:" << cLastTimeSav << " diff:" << cLastTimeSav-iLastTimeSav );
    if( cLastTimeSav <= iLastTimeSav ) return false;

    VERBOSELN3( "UndoHistory::externalThread_getSav2 getLast:" <<  getLast()  );
      
    if( getLast() <= 0 )
      return false;

    oResult =  cHistoSav[ getLast()];

    return true;
  }
  //*************************************
}
