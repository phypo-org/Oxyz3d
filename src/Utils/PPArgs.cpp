#include "PPArgs.h"
#include "PPErrLog.h"
#include "PPString.h"

#include "string.h"


namespace PPu {
  
  //****************************
  PPArgs::PPArgs( int iArgc, char * iArgv[], char iSplitEgal )
  {
    // copy parameters

    for (int i = 0; i < iArgc; i++) 
      {
	if( iSplitEgal != 0 )
	  {
	    // if prameter like var=val we split 
	    char* lPtr = ::strchr( iArgv[i], iSplitEgal );
	    if( lPtr != nullptr )
	      {
		*lPtr = '\0'; // !!!!
		cVectParam.push_back( iArgv[i] );
		cVectParam.push_back( lPtr+1 );
		continue;
	      }
	  }
	
	cVectParam.push_back( iArgv[i] );	  
      }  
  }
  //---------------------------------------
  int PPArgs::exist( const char* lVar)
  {
    // find  parameter
    for( size_t i = 0; i< cVectParam.size(); i++ )
      {
	if( cVectParam[i] == lVar )
	  {
	    //	  VERBOSELN( "existParam " << lVar << " " << i );
	    
	    return (int)i;
	}
      }
    //  VERBOSELN( "existParam " << lVar << " not found" );
    return -1;
  }
  //---------------------------------------
  std::string PPArgs::getValue( const char* lVar )
  {
    int i = exist( lVar );
    
    if( i == -1 || i >= (int)cVectParam.size()-1 )
      {
	return "";
      }
    return cVectParam[i+1];
  }
  //--------------------------------------
  bool PPArgs::get( const char * iVariable, std::string & oValue )
  {
    int i = exist( iVariable );
    
    if( i == -1 || i >= (int)cVectParam.size()-1 )
      {
	return false;
      }  

    oValue = cVectParam[i+1];
      
    return true;
  }
  //---------------------------------------
  bool PPArgs::get(  const char* iVariable, long & oValue )
  {
    std::string lValue;
    bool lRet = get( iVariable, lValue );
    //    std::cout << "get " << iVariable << "." << iVariable<< "=" << lValue << std::endl;
    if( lRet )
      {
	try{
	  oValue = std::stol(lValue); 
	} catch( std::exception & e )
	  {
	    ERR_LOG( "Parameter convert value to long failed for "  << iVariable << "." << iVariable << ":" << lValue );
	    return false;
	  }
      }
    return lRet;
  }    
  //--------------------------------------
  bool PPArgs::get(  const char* iVariable, int & oValue )
  {
    std::string lValue;
    bool lRet = get(  iVariable, lValue );
    //    std::cout << "get " << iVariable << "." << iVariable<< "=" << lValue << std::endl;
    if( lRet )
      {
	try{
	  oValue = std::stoi(lValue); 
	} catch( std::exception & e )
	  {
	    ERR_LOG( "Parameter convert value to int  failed for "  << iVariable << "." << iVariable << ":" << lValue );
	    return false;
	  }
      }
    return lRet;
  }    
  //--------------------------------------
  bool PPArgs::get(  const char* iVariable, float & oValue )
  {
    std::string lValue;
    bool lRet = get(  iVariable, lValue );
    //    std::cout << "get " << iVariable << "." << iVariable<< "=" << lValue << std::endl;
    if( lRet )
      {
	try{
	  oValue = std::stof(lValue); 
	} catch( std::exception & e )
	  {
	    ERR_LOG( "Parameter convert value to float  failed for "  << iVariable << "." << iVariable << ":" << lValue );
	    return false;
	  }
      }
    return lRet;
  }    
  //--------------------------------------
  bool PPArgs::get(  const char* iVariable, double & oValue )
  {
    std::string lValue;
    bool lRet = get(  iVariable, lValue );
    //    std::cout << "get " << iVariable << "." << iVariable<< "=" << lValue << std::endl;
    if( lRet )
      {
	try{
	  oValue = std::stod(lValue); 
	} catch( std::exception & e )
	  {
	    ERR_LOG( "Parameter convert value to double  failed for "  << iVariable << "." << iVariable << ":" << lValue );
	    return false;
	  }
      }
    return lRet;
  }    

  //--------------------------------------
  bool PPArgs::get(  const char* iVariable, long double & oValue )
  {
    std::string lValue;
    bool lRet = get(  iVariable, lValue );
    //    std::cout << "get " << iVariable << "." << iVariable<< "=" << lValue << std::endl;
    if( lRet )
      {
	try{
	  oValue = std::stold(lValue); 
	} catch( std::exception & e )
	  {
	    ERR_LOG( "Parameter convert value to long double  failed for "  << iVariable << "." << iVariable << ":" << lValue );
	    return false;
	  }
      }
    return lRet;
  }    

  //--------------------------------------
  bool PPArgs::get(  const char* iVariable, bool & oValue )
  {
    std::string lValue;
    bool lRet = get( iVariable, lValue );
    if( lRet )
      {
	lValue = PPString::ToLower(lValue);
	if( lValue == "true" || lValue == "1" )
	  oValue = true;
	else
	  if( lValue == "false" || lValue == "0" )
	    oValue = false;
	else
	  {
	    ERR_LOG( "Parameter convert value to bool failed : " << iVariable << "." << iVariable << ":" << lValue );
	    return false;
	  }
      }
    return lRet;
  }
  
//****************************
} // namespace
