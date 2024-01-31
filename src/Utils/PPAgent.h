#ifndef H__PPAgent__H
#define H__PPAgent__H



#include <thread>

#include "PPNonCopyable.h"
#include "PPErrLog.h"

namespace PPu {
  
  //************************************
  class PPAgent : public PPNonCopyable {

  protected:
    std::thread    cThread;
    std::string    cName;
    bool           cRunning   = false;
    bool           cSoftStop  = false;

  public:
    PPAgent( std::string & iName );
    PPAgent( const char  * iName );
    
    virtual ~PPAgent();

    std::string & getName() { return cName; }
    
    
    virtual bool run      ();
    virtual void workBase();
    virtual bool workBegin();
    virtual bool work     () = 0;
    virtual void workEnd  ( bool lFlagBegin, bool lFlagwork);

    bool isRunning()      { return cRunning; }
    
    void softStop()       { cSoftStop = true; }
    
  };
  //************************************
  
} // namespace


#endif
