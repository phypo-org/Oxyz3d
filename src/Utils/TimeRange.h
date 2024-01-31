#ifndef H__PPTimeRange__H
#define H__PPTimeRange__H



#include "PPNonCopyable.h"
#include "PPDate.h"

#include <thread>
#include <chrono>

#include <iostream>

namespace PPu{

  //*****************************************
  class TimeRange : public PPNonCopyable {
  private:
    
    PPDateTimeFloat70 cStart   = 0;
    PPDateTimeFloat70 cLast = 0;    
    
  public:
    
    TimeRange() {    
      start();
    }

    void start() {
      cStart = cLast = PPDate::GetCurrentDateTimeFloat70();
      //   std::cout << "TimeRange start :" << cStart << std::endl;
    } 
    TimeRange& measure() {
      cLast =  PPDate::GetCurrentDateTimeFloat70();
      // std::cout << "TimeRange measure  start :" << cStart << " last:" << cLast <<  std::endl;
      return *this;
    }
 
    PPDateTimeFloat70  getStart() { return cStart; }
    PPDateTimeFloat70  getLast()  { return cLast;  }
    PPDateTimeFloat70  range()    { return cLast - cStart; }
   //-------------------------------------------

    TimeRange & sleepRemain( PPDateTimeFloat70 iSleep)
    {
    
      PPDateTimeFloat70 lRange =  measure().range();
      
      //   std::cout << "TimeRange sleepRemain  sleep :" << iSleep << " range:" << lRange << std::endl;
      
      if ( lRange < iSleep )
	{
          TimeRange::Sleep( iSleep  - lRange );
	}
      return *this;
    }
    
    static void Sleep( PPDateTimeFloat70 iSleep )
    {
      std::this_thread::sleep_for(std::chrono::microseconds( ((uint64_t)(iSleep*1E6)) ));
    }
  };

  //*****************************************

} // Namespace


#endif
