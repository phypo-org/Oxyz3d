#include "PPDate.h"

#include <string>
#include <iomanip>
#include <sstream>
#include <iostream>

#include <sys/time.h>

namespace PPu {
  
  //------------------------------------
inline  void SplitTime( PPDateTime70 pTime, PPDateUTC iUTC, int32_t* aaaa, int32_t* mm, int32_t *jj, int32_t *hh, int32_t* minu, int32_t* s )
  {
    struct tm * TM2;
    
    struct tm TM;
    TM2 = & TM;

    if( iUTC == PPDateUTC::UTC )
      ::gmtime_r    ((time_t*) &pTime, &TM);
    else
      ::localtime_r ((time_t*)&pTime, &TM );

    
    if( aaaa )
      *aaaa = TM2->tm_year+1900;
    if( mm )
      *mm   = TM2->tm_mon+1;
    if( jj )
      *jj   = TM2->tm_mday;
    if( hh)
      *hh   = TM2->tm_hour;
    if( minu )
      *minu = TM2->tm_min;
    if( s )
      *s    = TM2->tm_sec;
  }  
  //------------------------------------
  std::string PPDate::ConvertTimeToStr( PPDateTime70 pTime, PPDateUTC iUTC,  bool pHours, bool pFixsx, char pSepD, char pSepH, char pSepDH )
  {
    std::ostringstream  osstr;
    
    ConvertTimeToStr( osstr, pTime, iUTC, pHours, pFixsx, pSepD, pSepH, pSepDH );
    		


    return osstr.str();
  }
  //------------------------------------
  std::ostream & PPDate::ConvertTimeToStr( std::ostream & oOs, PPDateTime70 pTime, PPDateUTC iUTC, bool pHours, bool pFixsx, char pSepD, char pSepH, char pSepDH )
  {
    int32_t aaaa, mm, jj, hh, mn, sec;
    
    SplitTime( pTime, iUTC, &aaaa, &mm, &jj, &hh, &mn, &sec );
    
    if( pSepD != '\0' )
      oOs << std::setfill('0') << std::setw(4) << aaaa << pSepD << std::setw(2) << mm << pSepD << std::setw(2) << jj;
    else
      oOs << std::setfill('0') << std::setw(4) << aaaa << std::setw(2) << mm  << std::setw(2) << jj;
      
		
    if( pHours )
      {
	if( pSepDH != '\0' )
	  oOs 	 << pSepDH;

	if( pSepH != '\0' )	 
	  oOs 	  << std::setw(2) << hh << pSepH << std::setw(2) << mn << pSepH << std::setw(2) << sec;
	else
	  oOs 	  << std::setw(2) << hh << std::setw(2) << mn << std::setw(2) << sec;
      }
    
    return oOs;
  }
  //------------------------------------
  std::ostream & PPDate::ConvertTimeToStr( std::ostream & oOs, PPDateTimeFloat70  pTime, PPDateUTC iUTC, bool pHours, bool pFixsx, char pSepD, char pSepH, char pSepDH )
  {
    PPDateTime70 lSec  =  (PPDateTime70) pTime;
    
    ConvertTimeToStr( oOs, lSec, iUTC, pHours, pFixsx, pSepD, pSepH, pSepDH );

    double t1 = pTime - (double)lSec;

    if( t1 != 0 )
      {
	oOs << std::to_string( t1 ).substr( 1 );
      }
    else
      {
	oOs << ".000000";
      }
         
    return oOs;
  }
  //------------------------------------
  std::string PPDate::ConvertTimeToStr( PPDateTimeFloat70  pTime, PPDateUTC iUTC,  bool pHours, bool pFixsx, char pSepD, char pSepH, char pSepDH )
  {
    std::ostringstream  osstr;
    
    ConvertTimeToStr( osstr, pTime, iUTC, pHours, pFixsx, pSepD, pSepH, pSepDH );
    		
    return osstr.str();
  }

  //*****************************************
  
} // namespace

 
 
