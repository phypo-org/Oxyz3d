#include "PPConfig.h"


#define FILE_WRITE(A)

#define EXTRACT 


#include <sstream>
#include <fstream>

#include "PPString.h"
#include "PPDbgUtils.h"

namespace PPu {


  //***********************************************************
  void PPConfig::addVar( const std::string& iEntity,
			 const std::string& iVar,
			 const std::string& iVal)
  {
    std::string lKey = iEntity + kSep + iVar;
    cData[ lKey ] = iVal ;
  }
  //--------------------------------------
  bool PPConfig::readFile( const std::string & iFile )
  {
    std::ifstream lstream( iFile );
    if( ! lstream )
      {
	int oErrno;
	if( PPFile::FileExist( iFile, &oErrno) )
	  {
	    ERR_LOG( "PPConfig::readFile : open file failed : " << iFile );
	  }

	return false;
      }
 
    return readFile( lstream );
  }
  //--------------------------------------

  bool PPConfig::readFile( std::istream &pIn  )
  {
    char lLine[200000];
  
    std::string lCurrentSection = "None";
    
    //=============== read the stream ================
    while( pIn.getline( &lLine[0], sizeof(lLine)-1 ))
      {
	char* lCurrent = lLine;
	
	if( *lCurrent == '#')  // Comment ?
	  continue;
	
	for( int i = (int)::strlen(lCurrent) -1 ; i >= 0; i-- )
	  {
	    if ( lCurrent[i] == '\r' )
	      continue;

	    lCurrent[i+1] = '\0';
	    break;
	  }

	while( isspace( *lCurrent )  ) lCurrent++;
	if( *lCurrent == '\0' )
	  continue;

	// A section ?
	if ( *lCurrent == '[' )
	  {
	    char *lEnd = ::strchr( lCurrent, ']' );

	    if( lEnd != nullptr )
	      *lEnd = '\0';
	    lCurrentSection = ++lCurrent;
	    continue;
	  }

	char* lEgal = ::strchr(lCurrent, '=');
	if( lEgal == nullptr )
	  {
	    DBG_CONF(  "ReadIni error in section " << lCurrentSection << " >>>" );
	    continue;
	  }
	*lEgal  = '\0';

	std::string lVar( lCurrent );
	PPString::Trim( lVar );
	lEgal++;

	if( lVar.length() == 0  )
	  {
	    DBG_CONF( lCurrentSection << " >>>" << lLine );
	    continue;
	  }
	std::string lKey = lCurrentSection;
	lKey += kSep + lVar;

	
	if(  strlen( lEgal ) <= 0 )
	  {
	    cData[ lKey ] = "";  
	  }
	else
	  {
	    //	std::cout << "ReadIni add values "  << lKey << '=' << lEgal << std::endl;
	    cData[ lKey ] = lEgal;
	  }
      }
    //================== fin read ===========================
    return true;
  }

 // utilisé pour generer le fichier de config !
  //--------------------------------------
  //--------------------------------------
  bool PPConfig::writeToFile( const std::string& iFileName  )
  {
    try {
      // FAITE UN RENAME DE L'ANCIEN FICHIER
      std::ofstream lFileIni( iFileName,  std::ios::binary | std::ios::trunc );
      if( lFileIni )
	{	  
	  return writeToStream( lFileIni );
	}
      else
	{
	  ERRNO_LOG(  "PPConfig::writeToFile open file failed : <<<" << iFileName << ">>>"  );	 
  	}
    } catch( std::exception & iEx ) {
      
      ERRNO_LOG(  "PPConfig::writeToFile exception " <<  iEx.what() << " <<<" << iFileName << ">>>"  );
    }
    
    return false;
  }
  //--------------------------------------
  bool PPConfig::writeToStream( std::ostream & oOut ) const
  {
    std::string lMemoSection;
    
    for( auto lPair :  cData )
      {
	std::string lKey = lPair.first;	  
	std::string lVal = lPair.second;

	size_t lPos = lPair.first.find( kSep );
	std::string lSection = lPair.first.substr( 0, lPos );
	std::string lVar = lPair.first.substr( lPos + kSep.length() );

	
	if( lSection.length() )
	  {
	    if( lSection != lMemoSection )
	      {
		oOut << "[" << lSection << "]" << std::endl;
		lMemoSection = lSection;
	      }
	    if( lVar.length() )
	      {
		oOut << lVar << "=" <<  lVal << std::endl;
	      }
	  }
	else
	  {
	    ERR_LOG(  "PPConfig::writeToFile : found void section for key/val " << lKey << "/" << lVal );	 
	  }
      }
    return true;
  }

  //--------------------------------------
  bool PPConfig::readParam( int iArgc, char *iArgv[] )
  {   
    for( int i=1; i< iArgc-1; i++ )
      {	
	DBG_CONF( "PPConfig::readParam param : " << iArgv[i] );
	
	if( cFilePathParam == iArgv[i] )
	  {
	    cName = iArgv[i+1];	    
	  }
	else
	  {
	    
	  }
      }
    
    if( cName.length() == 0  )
      {	
	return false;
      }
    return true;
  }
  //--------------------------------------

  bool PPConfig::readFile( int iArgc, char *iArgv[] )
  {
    readParam( iArgc, iArgv );
         	
    return readFile(cName);
  }
//--------------------------------------
  bool PPConfig::internalReadString( const char * iSection, const char * iVariable, std::string & ioValue )
  {    
    std::string lStrFind( iSection + kSep+ iVariable );
     
    auto lIterator = cData.find(  lStrFind );
    if( lIterator  != cData.end() )
      {
	ioValue   = lIterator->second;
	DBG_CONF( "iniReadString [" << iSection << "]" << iVariable << "=" << ioValue );

	return true;
      }

    
    return false;
  }
  //--------------------------------------
  bool PPConfig::get( const char * iSection, const char * iVariable, std::string & oValue )
  {
    std::string lTmpVal;
    bool lRet = internalReadString( iSection, iVariable, lTmpVal);
    if( lRet )
      {
	oValue = lTmpVal ;
      }
      
    return lRet;
  }
  //--------------------------------------
  bool PPConfig::get( const char* iSection, const char* iVariable, long long & oValue )
  {
    std::string lValue;
    bool lRet = get( iSection, iVariable, lValue );
    //    std::cout << "get " << iSection << "." << iVariable<< "=" << lValue << std::endl;
    if( lRet )
      {
	try{
	  oValue = std::stoll(lValue); 
	} catch( std::exception & e )
	  {
	    ERR_LOG( "Config convert value to long long failed : "  << iSection << "." << iVariable << ":" << lValue );
	    return false;
	  }
      }
    return lRet;
  }    
  //--------------------------------------
  bool PPConfig::get( const char* iSection, const char* iVariable, long & oValue )
  {
    std::string lValue;
    bool lRet = get( iSection, iVariable, lValue );
    //    std::cout << "get " << iSection << "." << iVariable<< "=" << lValue << std::endl;
    if( lRet )
      {
	try{
	  oValue = std::stol(lValue); 
	} catch( std::exception & e )
	  {
	    ERR_LOG( "Config convert value to long failed for "  << iSection << "." << iVariable << ":" << lValue );
	    return false;
	  }
      }
    return lRet;
  }    
  //--------------------------------------
  bool PPConfig::get( const char* iSection, const char* iVariable, int & oValue )
  {
    std::string lValue;
    bool lRet = get( iSection, iVariable, lValue );
    //    std::cout << "get " << iSection << "." << iVariable<< "=" << lValue << std::endl;
    if( lRet )
      {
	try{
	  oValue = std::stoi(lValue); 
	} catch( std::exception & e )
	  {
	    ERR_LOG( "Config convert value to int  failed for "  << iSection << "." << iVariable << ":" << lValue );
	    return false;
	  }
      }
    return lRet;
  }    
  //--------------------------------------
  bool PPConfig::get( const char* iSection, const char* iVariable, float & oValue )
  {
    std::string lValue;
    bool lRet = get( iSection, iVariable, lValue );
    //    std::cout << "get " << iSection << "." << iVariable<< "=" << lValue << std::endl;
    if( lRet )
      {
	try{
	  oValue = std::stof(lValue); 
	} catch( std::exception & e )
	  {
	    ERR_LOG( "Config convert value to float  failed for "  << iSection << "." << iVariable << ":" << lValue );
	    return false;
	  }
      }
    return lRet;
  }    
  //--------------------------------------
  bool PPConfig::get( const char* iSection, const char* iVariable, double & oValue )
  {
    std::string lValue;
    bool lRet = get( iSection, iVariable, lValue );
    //    std::cout << "get " << iSection << "." << iVariable<< "=" << lValue << std::endl;
    if( lRet )
      {
	try{
	  oValue = std::stod(lValue); 
	} catch( std::exception & e )
	  {
	    ERR_LOG( "Config convert value to double  failed for "  << iSection << "." << iVariable << ":" << lValue );
	    return false;
	  }
      }
    return lRet;
  }    

  //--------------------------------------
  bool PPConfig::get( const char* iSection, const char* iVariable, long double & oValue )
  {
    std::string lValue;
    bool lRet = get( iSection, iVariable, lValue );
    //    std::cout << "get " << iSection << "." << iVariable<< "=" << lValue << std::endl;
    if( lRet )
      {
	try{
	  oValue = std::stold(lValue); 
	} catch( std::exception & e )
	  {
	    ERR_LOG( "Config convert value to long double  failed for "  << iSection << "." << iVariable << ":" << lValue );
	    return false;
	  }
      }
    return lRet;
  }    

  //--------------------------------------
  bool PPConfig::get( const char* iSection, const char* iVariable, bool & oValue )
  {
    std::string lValue;
    bool lRet = get( iSection, iVariable, lValue );
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
	    ERR_LOG( "Config convert value to bool failed : " << iSection << "." << iVariable << ":" << lValue );
	    return false;
	  }
      }
    return lRet;
  }
 
  //--------------------------------------
  //--------------------------------------
  //--------------------------------------
  bool PPConfig::set ( const char* iSection, const char* iVariable, const std::string & iValue )
  {
    std::string lKey = iSection;
    lKey += kSep +iVariable;
    cData[ lKey ] = iValue;
    
    return true; 
  }
  //--------------------------------------
  bool PPConfig::set ( const char* iSection, const char* iVariable, const char* iValue )
  {
    std::string lKey = iSection;
    lKey += kSep +iVariable;
    cData[ lKey ] = iValue;
    
    return true; 
  }

  //--------------------------------------
  //--------------------------------------
  //--------------------------------------
  void PPConfig::print( std::ostream& iOs ) const
  {
    writeToStream( iOs );
  }

  //**************************************  
} //end namespace


