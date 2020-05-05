#ifndef H__PPSTRING__H
#define H__PPSTRING__H

#include <algorithm>
#include <sstream>
#include <vector>
#include <ctype.h>

#include "string.h"

namespace PPu {
  //******************************
  class PPString{
  public:

    static constexpr const char* sToTrim = " \t\n\r\f\v";
    //---------------------------------------------------- 
    // trim from end of string (right)
    static std::string& RightTrim(std::string& s, const char* t = sToTrim)
    {
      s.erase(s.find_last_not_of(t) + 1);
      return s;
    }
    //----------------------------------------------------
    // trim from beginning ostd::vectorf string (left)
    static std::string& LeftTrim(std::string& s, const char* t = sToTrim)
    {
      s.erase(0, s.find_first_not_of(t));
      return s;
    }

    //----------------------------------------------------
    // trim from both ends of string (left & right)
    static std::string& Trim(std::string& s, const char* t = sToTrim)
    {
      return LeftTrim(RightTrim(s, t), t);
    }
    //----------------------------------------------------
    static std::string& ToUpper( std::string& data )
    {
      std::for_each( data.begin(), data.end(), [](  char &c)
		     { c = static_cast<char>(std::toupper(static_cast<unsigned char>(c))); });
      return data;
    }
    //----------------------------------------------------
    static std::string ToUpper( const std::string& iData )
    {
      std::string lTmp( iData );
      std::for_each( lTmp.begin(), lTmp.end(), [](  char &c)
		     { c = static_cast<char>(std::toupper(static_cast<unsigned char>(c))); });
      return lTmp;
    }
    //----------------------------------------------------
    static std::string& ToLower( std::string& data )
    {      
      std::for_each(data.begin(), data.end(), [](char &c)
		    { c = static_cast<char>(std::tolower(static_cast<unsigned char>(c))); });
      return data;
    }
    //----------------------------------------------------
    static std::string ToLower( const std::string& iData )
    {
      std::string lTmp( iData );
      std::for_each(lTmp.begin(), lTmp.end(), [](char &c)
		    { c = static_cast<char>(std::tolower(static_cast<unsigned char>(c))); });
      return lTmp;
    }
    //----------------------------------------------------
    static char* ToUpper( char* s)
    {
      char *s0 = s;
      while( *s ) { *s = static_cast<char>(std::toupper(*s)); s++;};
      return s0;
    }
    //----------------------------------------------------
    static char* ToLower( char* s)
    {      
      char *s0 = s;
      while( *s ) { *s = static_cast<char>(std::tolower(*s)); s++;};
      return s0;
    }          
    //----------------------------------------------------
    static std::string DoubleChar( std::string& data, char pTarget )
    {
 
      size_t lSz = data.size();
      
      std::string lDest ;
      
      for( size_t i=0; i< lSz; i++ )
	{
	  char c = data[i];
	  lDest += c;
	  if( c == pTarget )
	    lDest += c;
	}
      return lDest;
    }
    //----------------------------------------------------    
    static std::string DoubleChar( const std::string& data, const char* pTarget )
    {
      size_t lSz = data.size();
      
      std::string lDest ;
      
      for( size_t i=0; i< lSz; i++ )
	{
	  char c = data[i];
	  lDest += c;
	  
	  for( const char* lTmp= pTarget; *lTmp != '\0'; lTmp++)
	    {
	      if( *lTmp == c )
		{
		  lDest += c;
		  break;
		}	      
	    }	    
	}
      return lDest;
    }
    //----------------------------------------------------    
    static std::string DoubleChar( const char* data, const char* pTarget )
    {
      size_t lSz = ::strlen( data );
      
      std::string lDest ;
      
      for( size_t i=0; i< lSz; i++ )
	{
	  char c = data[i]; // mettre iterateur
	  lDest += c;
	  
	  for( const char* lTmp= pTarget; *lTmp != '\0'; lTmp++)
	    {
	      if( *lTmp == c )
		{
		  lDest += c;
		  break;
		}	      
	    }	    
	}
      return lDest;
    }
    //----------------------------------------------------
    static void ToLowerString(std::string& io_Protocol)
    {
      std::transform(io_Protocol.begin(), io_Protocol.end(), io_Protocol.begin(), ::tolower);
    }
    //----------------------------------------------------
    static bool IsFullNumber(const std::string& iStrToCheck)
    {
      bool lRet = false;
    
      if (!iStrToCheck.empty())
	{
	  size_t lSize = iStrToCheck.size();
	  size_t i = 0;
      
	  while ((i < lSize) &&
		 ((' ' == iStrToCheck[i]) ||
		  (isdigit(iStrToCheck[i]))))
	    {
	      i++;
	    }
	  if (i == lSize) {
	    lRet = true;
	  }
	}
      return lRet;
    }

    //---------------------------------------------------------
    // Check if 'fname' ends with 'end'
    //--------------------------------------------------------- 
    static bool FinishBy(  const std::string &iName, const std::string &iEnd)
    {
      if (iName.length() >= iEnd.length() &&
	  iName.compare( iName.length() - iEnd.length(), iEnd.length(), iEnd) == 0) {
	return true;
      }
      return false;
    }
    //--------------------------------------------------------- 
    static std::string ReplaceAllString( std::string iSubject, const std::string& iSearch,
					 const std::string& iReplace)
    {
      size_t lPos = 0;
      while( (lPos = iSubject.find( iSearch, lPos)) != std::string::npos)
	{
	  iSubject.replace( lPos, iSearch.length(), iReplace);
	  lPos += iReplace.length();
	}
      return iSubject;
    }
    //--------------------------------------------------------- 
    static std::string ReplaceFirstString( std::string iSubject, const std::string& iSearch,
					   const std::string& iReplace)
    {
      size_t lPos = 0;
      if( (lPos = iSubject.find( iSearch)) != std::string::npos) 
	{
	  iSubject.replace( lPos, iSearch.length(), iReplace);
	}
      return iSubject;
    }
    //---------------------------------------------------------
    // 
    static void ChangeFirstString( std::string & ioSubject,
				   const std::string& iSearch,
				   const std::string& iReplace)
    {
      size_t lPos = 0;
      if( (lPos = ioSubject.find( iSearch)) != std::string::npos) 
	{
	  ioSubject.replace( lPos, iSearch.length(), iReplace);
	}
    }
    //--------------------------------------------------------- 
    // string form vector strings, returns the result */
    static std::string ArrayToString( std::vector<std::string> iArray )
    {
      std::string lResult;

      for( std::string & lTmp : iArray )
	lResult += lTmp;
      
      return lResult;
    }
  
    //--------------------------------------------------------- 
    static bool IsGoodForPathname( const std::string & iStr, const char* iPermit=nullptr)
    {      
      return IsGoodForPathname( iStr.c_str(), iStr.size(), iPermit );
    }
    //--------------------------------------------------------- 
    static bool IsGoodForPathname( const char* iStr, size_t iMaxSize, const char* iPermit=nullptr)
    {
      for( size_t i=0; i< iMaxSize; i++ )
	{
	  char lCar = iStr[i];
	  if( lCar == '\0')
	    break;
	      
	  if( (::isalnum(lCar) || isspace(lCar)
	       || lCar =='.' || lCar == '/' ||  lCar == '~' ||  lCar == '_' )
	      || ( iPermit != nullptr && ::strchr( iPermit, lCar ) != nullptr ))
	    {
	      continue;		    
	    }
	  return false;
	}
      return true;
    }
    //--------------------------------------------------------- 
    static bool IsDigit( const std::string & iStr )
    {
      return std::all_of(iStr.begin(), iStr.end(), ::isdigit); 
    }
    //--------------------------------------------------------- 
    static bool IsHexa( const std::string & iStr )
    {
      return std::all_of(iStr.begin(), iStr.end(), ::isxdigit); 
    }
    //--------------------------------------------------------- 
    static size_t ReadLinesToVect( const char* iSrc, std::vector< std::string > & iVectDest )
    {
      if( iSrc == nullptr )   return 0;
      
      const char*lBegin=iSrc;
      
      while( true )
	{
	  if( *iSrc == '\n' || *iSrc == '\0' )
	    {
	      if( iSrc-lBegin > 0 )
		iVectDest.push_back( std::string( lBegin, iSrc-lBegin ));
	      
	      if( *iSrc == '\0' )
		break;
	      
	      iSrc++;
	      lBegin = iSrc;
	    }
	  else
	    {
	      iSrc++;
	    }
	}
      return iVectDest.size();
    }

  };

  //******************************

} //namespace

#endif
