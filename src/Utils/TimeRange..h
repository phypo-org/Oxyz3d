#ifndef H__PPTimeRange__H
#define H__PPTimeRange__H



#include "PPNonCopyable.h"
#include "PPDate.h"


namespace PPu{

  //*****************************************
  class TimeRange : public SNonCopyable
  {
  private:
    
    PPDateTimeFloat70 cStart   = 0;
    PPDateTimeFloat70 cLast = 0;    
    
  public:
    
    TimeRange() {    
      start();
    }

    void start() {
      cStart = cLast = PPDate::GetCurrentDateTimeFloat70();
    }
    TimeRange& measure() {
      cLast =  PPDate::GetCurrentDateTimeFloat70();
      return *this;
    }
 
    PPDateTimeFloat70  getStart() { return cStart; }
    PPDateTimeFloat70  getLast()  { return cLast;  }
    PPDateTimeFloat70  range()    { return cLast - cStart; }
   //-------------------------------------------

    TimeRange & sleepRemain( PPTimeFloat70 iSleep)
    {
     
      PPTimeFloat70 lRange =  measure().range();
      
      if ( lRange < iSleep )
	{
          TimeRange::Sleep( iSleep  - lRange );
	}
      return *this;
    }
    
    static void Sleep( STimeFloat70 iSleep )
    {
      std::this_thread::sleep_for(std::chrono::microseconds( ((uint64_t)(iSleep*1E6)) ));
    }
  };

  //*****************************************

} // Namespace


#endif
