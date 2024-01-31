#ifndef __SFILE_SESAM__
#define __SFILE_SESAM__

#include <string>
#include <vector>

#include <sys/stat.h>
#include "PPNonCopyable.h"


#if  defined(_WIN32) || defined(_WIN64)
#define mode_t int 
#endif

namespace PPu {

#undef CopyFile


  class SDynBuffer;
  
  //*****************************************
  class PPFile: public PPNonCopyable{
    
  public :
    
    static const char  sSepDirChar;
    static const char* sSepDirStr;



    static const mode_t DefaultDirMode = 0770;
    static const mode_t DefaultDirMask = 0007;


    static constexpr const char* FILE_ENDING_TMP	= "tmp";
    static constexpr const char* FILE_ENDING_JSON	= "json";
    static constexpr const char* FILE_ENDING_ALERT      = "alrt";
    static constexpr const char* FILE_ENDING_META       = "meta";

    static constexpr const char* FILE_ENDING_DOT_TMP	= ".tmp";
    static constexpr const char* FILE_ENDING_DOT_JSON	= ".json";
    static constexpr const char* FILE_ENDING_DOT_ALERT  = ".alrt";
    static constexpr const char* FILE_ENDING_DOT_META   = ".meta";


  public:
    //----------------------------------------------------  
    static inline std::string ParentPath(const std::string& iDirname )
    {
      size_t lLength =  iDirname.length();
      
      size_t i = iDirname.rfind( sSepDirChar, lLength );
      if (i != std::string::npos)
	{
	  return( iDirname.substr(0, lLength - (lLength - i) ));
	}
      
      return("");
    }
    //----------------------------------------------------  
    static inline std::string LastPath( const std::string& iDirname )
    {
      size_t lLength =  iDirname.length();
      
      size_t i = iDirname.rfind( sSepDirChar, lLength);
      if (i != std::string::npos)
	{
	  return(iDirname.substr(i+1, lLength - i));
	}

      return(iDirname);
    }
    //----------------------------------------------------  
    static inline std::string Extension( const std::string& iFilePath )
    {
      size_t lLength =  iFilePath.length();
      
      size_t i = iFilePath.rfind( '.', lLength);
      if (i != std::string::npos)
	{
	  return(iFilePath.substr(i+1, lLength - i));
	}
      
      return "";
    }
    //----------------------------------------------------  
    static inline std::string WithoutExtension( const std::string& iFilePath )
    {
      size_t lLength =  iFilePath.length();
      
      size_t i = iFilePath.rfind( '.', lLength);
      if (i != std::string::npos)
	{
	  return(iFilePath.substr(0, i ));
	}
      
      return iFilePath;
    }
     //----------------------------------------------------  
    static inline std::string FullExtension( const std::string& iFilePath )
    {           
      std::string lFileName = LastPath( iFilePath );
      
      size_t i = lFileName.find( '.' );
				 
      if (i != std::string::npos)
	{
	  return lFileName.substr(i+1);
	}
      
      return "";
    }
    //----------------------------------------------------  
    static inline std::string WithoutFullExtension( const std::string& iFilePath )
    {
      size_t lLength =  iFilePath.length();
      
      size_t i = iFilePath.find( '.' );
      if (i != std::string::npos)
	{
	  return(iFilePath.substr(0, i ));
	}
      
      return iFilePath;
    }

   //----------------------------------------------------  
   static inline std::string JoinPathNames(const std::string& iDirname, const std::string& iName)
    {
      std::string	 lFullname; 
      
      // TODO : faire une version Windows      
      // sous unix s'il y a plusieurs /  d'affilé comme // ou ///// cela fait /
      // donc je ne traite pas le cas
      
      if (*iDirname.rbegin() == sSepDirChar || *iName.begin() == sSepDirChar )
	lFullname = iDirname + iName;
      else
	lFullname = iDirname + sSepDirStr + iName;
      return  lFullname;
    }
   //----------------------------------------------------  
    static bool GetCurrentDir( std::string & oDir);   
    static bool DoMkdir( const std::string &, const mode_t iMode=DefaultDirMode, const mode_t iUMask = DefaultDirMask  );    
    static bool DoMkSubdir( const std::string & iRoot, const std::string & iSub, const mode_t iMode=DefaultDirMode, const mode_t iUMask=DefaultDirMask );
    static bool RemoveFile(const std::string &iFilename);
    static bool RemoveDir(const std::string &iDirname);
    static bool RenameFile(const std::string &iOldFilename, const std::string &iNewFilename );
    static bool FileExist(const std::string & iFile, int* oErrno=nullptr);
    static bool LazyFileChange( const std::string& iFile, time_t & ioTime, int* oErrno=nullptr);
    static bool IsDir(const std::string & iFile, int* oErrno=nullptr);
    static bool CopyFile( const std::string &iIn, const std::string &iOut, const mode_t iMode=DefaultDirMode );
    static bool CreateVoidFile( const std::string &iIn, const mode_t iMode=DefaultDirMode );
    
    static bool ReadFile( const std::string &iFileName, std::string& ioStr );
    static size_t ReadFileLines( const std::string &iFileName, std::vector<std::string>& ioLines, size_t iNbLine =0xFFFFFFFF );
    static bool WriteFile( const std::string &iFileName, const char* oStr, size_t iSz );
    static bool WriteFile( const std::string &iFileName, const std::string& ioStr );
    static bool AddFile( const std::string &iFileName, const std::string& ioStr );


    static std::string sDirHTTP;
    static std::string sDirDNSA;
    static std::string sDirDNSQ;
    static std::string sDirSMTP;
    static std::string sDirIP ;
    static std::string sDirX509;
    static std::string sDirOther;

    // mdel test pour voir si cela fonctionne avec le DPI
    static std::string sDirFile;
    static std::string sDirCerts;

    
    static std::string sDirScanYara;

  };

  //*****************************************


} //end namespace

#endif
