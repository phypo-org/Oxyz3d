#include "PPAgent.h"
#include "PPExcept.h"

using namespace std;

namespace PPu {

  //**********************************
  PPAgent::PPAgent( std::string & iName )
    :cName( iName )
  {
  }
  
  //---------------------------
  PPAgent::PPAgent( const char  * iName )
    :cName( iName )
  {
  }
  //---------------------------
  PPAgent::~PPAgent()
  {
  }
  //---------------------------
  bool PPAgent::run()
  {
    VERBOSELN( "PPAgent " << cName << " run " );
    
    if( cRunning )
      {
        ERR_LOG( "Thread " << cName << " already running" );
        return false;
      }
 
  
    cThread  = std::thread( &PPAgent::workBase, this );
 
    return true;
  }
  //---------------------------
  void PPAgent::workBase()
  {
    VERBOSELN4( "PPAgent::workBase" );
    cRunning = true;
    
    VERBOSELN( "PPAgent " << cName << " workBase " );
    bool lBegin = false;
    bool lWork  = false;

    try {
      lBegin = workBegin();
    }
    catch(PPExcept & iEx )
      {
        ERR_LOG( "Catch exception : " << iEx );
      }
    catch(std::exception & iEx)
      {
        ERR_LOG( "Catch C++  exception : " << iEx.what() );
      } 
    catch(...)
      {
        ERR_LOG( "Catch unknow exception");
      }
    
    VERBOSELN4( "PPAgent::workBase begin:" << lBegin );

    if( lBegin )
      {
        try{
          lWork = work();
        }
        catch(PPExcept & iEx )
          {
            ERR_LOG( "Catch exception : " << iEx );
          }
        catch(std::exception & iEx)
          {
            ERR_LOG( "Catch C++  exception : " << iEx.what() );
          } 
        catch(...)
          {
            ERR_LOG( "Catch unknow exception");
          }  
        
      }
    try{
      workEnd( lBegin, lWork );
    }
    catch(PPExcept & iEx )
      {
        ERR_LOG( "Catch exception : " << iEx );
      }
    catch(std::exception & iEx)
      {
        ERR_LOG( "Catch C++  exception : " << iEx.what() );
      } 
    catch(...)
      {
        ERR_LOG( "Catch unknow exception" );
      }
    
   cRunning = false;    
  }
  //---------------------------
  bool PPAgent::workBegin()
  {
    
    return true;    
  }  
  //---------------------------
  void PPAgent::workEnd( bool lFlagBegin, bool lFlagwork )
  {
  }
  //************************************


} // namespace
