#ifndef H__PPDATE__H
#define H__PPDATE__H

#include <sstream>

#include <sys/time.h>


namespace PPu {

  typedef  time_t   PPDateTime70;
  typedef  double   PPDateTimeFloat70;


  
  //*****************************************
  enum PPDateFormat{
    SDateFormat_AAAAMMJJ
    ,SDateFormat_AAAAMMJJ_HHMMSS
    ,SDateFormat_AAAAMMJJ_HH_MM_SS
    ,SDateFormat_AAAA_MM_JJ_HH_MM_SS
  };
  //*****************************************
  enum class PPDateUTC{ UTC, Local };
  //*****************************************
  class PPDate {
    
  public:
    //------------------------------------
    static PPDateTime70 GetCurrentDateTime70()
    {		
      struct timeval tp;
      GetTimeVal( tp);
    
      return( tp.tv_sec);		
    }
    //------------------------------------
    static void GetTimeVal( struct timeval& ioTv)
    {		
      ::gettimeofday( &ioTv, NULL );    
    }
    //------------------------------------
    static PPDateTimeFloat70  GetCurrentDateTimeFloat70()
    {
      struct timeval tp;
      GetTimeVal( tp );

      PPDateTimeFloat70 lTime = (PPDateTimeFloat70)tp.tv_sec + ((PPDateTimeFloat70)tp.tv_usec)*1E-6;
 		
      return lTime;
    }
    //------------------------------------
    static std::string   ConvertTimeToStr( PPDateTime70 pTime,
					   PPDateUTC iUTC,
					   bool pHours=false,
					   bool pFixsx=true,
					   char pSepD='/',
					   char pSepH=':',
					   char pSepDH=' ');
    //------------------------------------ 
    static std::ostream & ConvertTimeToStr( std::ostream & oOs,
					    PPDateTime70 pTime,
					    PPDateUTC iUTC,
					    bool pHours=false,
					    bool pFixsx=true,
					    char pSepD='/',
					    char pSepH=':',
					    char pSepDH=' ');
    
    //------------------------------------    
    static std::string ConvertTimeToStr(    PPDateTimeFloat70 iDate,
					    PPDateUTC iUTC,
					    bool pHours=false,
					    bool pFixsx=true,
					    char pSepD='/',
					    char pSepH=':',
					    char pSepDH=' ' );
    //------------------------------------    
    static std::ostream & ConvertTimeToStr( std::ostream & oOs,
					    PPDateTimeFloat70 iDate,
					    PPDateUTC iUTC,
					    bool pHours=false,
					    bool pFixsx=true,
					    char pSepD='/',
					    char pSepH=':',
					    char pSepDH=' ' );
  };
  //*****************************************

}  //  namespace


#endif
