#include "Utils/TimeRange.h"
#include "Utils/PPFile.h"


#include "Shape/UndoHistory.h"


#include "Preference.h"
#include "AgentAutoSav.h"


#include <fstream>
#include <string>


using namespace std;

namespace M3d {

  //***************************
  bool AgentAutoSav::work     ()
   {

     VERBOSELN( "AgentAutoSav::work");
     
     int lCyclingFile=0;

     PPu::PPDateTimeFloat70 cLastSav = 0;
     
     while( cSoftStop == false )
       {
        lCyclingFile %= MyPref.cNbFileCycling;
         
         try {           
           PPu::TimeRange lRange;
           
           std::ostringstream  lOs;
           lOs << MyPref.cCurrentDir  << PPu::PPFile::sSepDirChar
               << MyPref.cFileDefaultDir << PPu::PPFile::sSepDirChar
               << "#autosave" << lCyclingFile << "#" ;

           

           if( MyPref.cFileAutoSave )
             {
              std::string lSavBuffer;
               if( PP3d::UndoHistory::Instance().externalThread_getSav( cLastSav, lSavBuffer ) )
                 {
                   VERBOSELN( "AgentAutoSav::work file " << lOs.str() << " " << lSavBuffer.size());
                   
                   PPu::PPFile::DoMkSubdir( MyPref.cCurrentDir, MyPref.cFileDefaultDir );
                   std::ofstream lOut;						
                   lOut.open( lOs.str() );
                   if( lOut.good() )
                     {
                     lOut << lSavBuffer;
                       lOut.close();
                       lCyclingFile++;
                       cLastSav = PPu::PPDate::GetCurrentDateTimeFloat70();
                     }
                   else
                     ERR_LOG("AgentAutoSav failed for :" <<  lOs.str() );
                 }
             }
           //      cout << "SleepRemainTime for " << MyPref.cFileAutoSaveFrequency << endl;
           lRange.sleepRemain( MyPref.cFileAutoSaveFrequency );
         }
         catch (...) {
           ERR_LOG( "AgentAutoSav::work : Unexpected exception" );
         }
       }
     return true;
   }

  //***************************
  
} // namespace
