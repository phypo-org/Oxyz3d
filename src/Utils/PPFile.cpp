#include "PPFile.h"


#include <sys/types.h>
#include <sys/stat.h>

#if defined(_WIN64 ) || defined(_WIN32)
#include <io.h>
#include <process.h>
#include <direct.h >
#include <sys/stat.h>
#include <sys/types.h>
#include <io.h>
#include <stdio.h>

#define umask  _umask

#include "WinLib/dirent.h"

#else
#include <unistd.h>
#endif
#include <limits.h>

#include <fcntl.h>
#include <fstream> 
#include <sstream> 
#include <iostream>

#include "PPErrLog.h"
#include "PPDbgUtils.h"



namespace PPu {

#if  defined(_WIN32) || defined(_WIN64)
  const char  PPFile::sSepDirChar='\\';
  const char* PPFile::sSepDirStr="\\";
#else  
  const char  PPFile::sSepDirChar='/';
  const char* PPFile::sSepDirStr="/";
#endif



  //************************************

  bool PPFile::GetCurrentDir( std::string & oDir)
  {
    char lBuffer[PATH_MAX];
    
   if (getcwd(lBuffer, sizeof(lBuffer)) != nullptr) {
     oDir = lBuffer;
     return true;
   }
   ERRNO_LOG(  "PPFile::GetCurrentDir failed - " );
   
   return false;
  }
  //----------------------------------------------------  
  bool PPFile::DoMkdir( const std::string & iPath, const mode_t iMode, const mode_t iUMask  )
  {
    DBG_FIL(  "DoMkdir " << iPath  );
    struct stat	lStat;
    bool lStatus = true;
    
    if ( ::stat( iPath.c_str(), &lStat) != 0)
      {
	// Directory does not exist. EEXIST for race condition 
	////	umask(007);
        mode_t lOldUMask = ::umask(iUMask);

	//DBG_FIL( "************************* umask old:" << lOldUMask );
		     
#if defined( _WIN32) || defined( _WIN64)
        if (_mkdir(iPath.c_str()) != 0 && errno != EEXIST)
#else
       if (::mkdir(iPath.c_str(), iMode) != 0 && errno != EEXIST)
#endif  
	  {
	    lStatus = false;	    
	    
	    ERRNO_LOG(  "DoMkdir Create <<<" << iPath << ">>> fail"  );
	  }
	::umask(  lOldUMask);
      }	  
    else
      if (!S_ISDIR(lStat.st_mode))
	{
	  // File exists but is not a directory 	    
	  lStatus = false;
	  errno = ENOTDIR;
	  ERRNO_LOG("DoMkdir Create <<<" << iPath << ">>> not a directory" );
	}
	    
    return lStatus;
  }
  //---------------------------------------------------------  
  bool PPFile::DoMkSubdir( const std::string & iRoot, const std::string &  iSub, const mode_t iMode, const mode_t iUMask ) 
  {
    if( iSub.length() == 0)
      return false;
    
    //   DBG_FIL( "+++++++++ DoMkSubdir " << oRoot << " " << oSub );
    std::string lRoot = iRoot;
    std::string lSub  = iSub;
  
    // si besoin on ajoute une separateur a Root
    if (*lRoot.rbegin() != sSepDirChar )
      lRoot  += sSepDirChar;

    
    while( lSub.length() )
      {
	std::string lNewDir;
	
	size_t lPos = lSub.find(sSepDirChar);
	if( lPos !=  std::string::npos )
	  {
	    lNewDir = lSub.substr( 0, lPos );
	    lSub = lSub.substr( lPos+1 );
	  }
	else
	  {
	    lNewDir = lSub;
	    lSub = "";
	  }	

	if( lNewDir.length() == 0 )
	  continue;

	lRoot += lNewDir;

	//	DBG_FIL( "DoMkdir " << lRoot << "   " << lNewDir );
	if( DoMkdir( lRoot,  iMode, iUMask ) == false )
	  return false;
	

	lRoot += sSepDirChar;   
	//	DBG_FIL( "  lRoot: " + lRoot + " Sub:" + lSub );
      }
    
    return true;
  }
  //---------------------------------------------------------	      
  bool PPFile::RemoveFile(const std::string &iFilename)
  {
    if( ::remove( iFilename.c_str()) == 0 )
      {
	DBG_FIL("File removed: " << iFilename);
	return true;
      }
    else
      {
	ERRNO_LOG( "Fail to remove file:" << iFilename);
	return false;
      }
  }

  //---------------------------------------------------------	      
  bool PPFile::RemoveDir(const std::string &iDirname)
  {
    if( ::rmdir( iDirname.c_str()) == 0 )
      {
	DBG_FIL("Dir removed: " << iDirname);
	return true;
      }
    else
      {
	ERRNO_LOG(  "Fail to remove file:" << iDirname);
	return false;
      }
  }
  //---------------------------------------------------------
  bool PPFile::RenameFile(const std::string &iOldFilename, const std::string &iNewFilename )
  {
    if( ::rename( iOldFilename.c_str(), iNewFilename.c_str() ) == 0 )
      {
	DBG_FIL("File rename : " << iOldFilename
		  << " -> " << iNewFilename);
	
	return true;
      }
    else
      {
	ERRNO_LOG(  "Fail to rename :" << iOldFilename
		     << " -> " << iNewFilename );
	
	return false;
      }
  } 
  //---------------------------------------------------------
  //  bool PPFile::FileExists(const char* iFile)
  //  {
  //    struct stat lBuf;
  //    return (stat(iFile, &lBuf) == 0);
  //  }
  bool PPFile::FileExist(const std::string& iFile, int* oErrno)
  {
    struct stat lBuf;
    
    int lRet =::stat(iFile.c_str(), &lBuf);
    if( lRet == 0 )
      {
	return true;
      }

    if( oErrno )
      {
	*oErrno = errno;
      }
    else
      {	
	if( errno == ENOENT )
	  {	
	    ERR_LOG( "PPFile::FileExist : File path no exist <" << iFile << '>' );
	  }
	else
	  {
	    ERRNO_LOG(   "PPFile::FileExist : stat error <" << iFile << '>' );
	  }
      }
    
    return false;
  }
  //---------------------------------------------------------
  // Renvoie vrai si la date a changé, faux sinon
  // si ioTime est a 0, on renvoit la date du fichier si disponible
  // S'il se passe moins d'une seconde entre deux modifs cela ne marche pas toujours !
  
  bool PPFile::LazyFileChange( const std::string& iFile, time_t & ioTime, int* oErrno)
  {
    if( iFile.length() == 0 )
      return false;
    
    struct stat lBuf;
    
    int lRet =::stat(iFile.c_str(), &lBuf);
    if( lRet == 0 )
      {
	if( ioTime == 0 )
	  {
	    ioTime = lBuf.st_mtime;
	    return false;
	  }
	
	if( lBuf.st_mtime == ioTime  )
	  {
	    return false;
	  }
	ioTime = lBuf.st_mtime;
	return true;
      }

    if( oErrno )
      {
	*oErrno = errno;
      }
    else
      {	
	if( errno == ENOENT )
	  {	
	    ERR_LOG( "PPFile:LazyFileChange : File path no exist <" << iFile << '>' );
	  }
	else
	  {
	    ERRNO_LOG(   "PPFile::LazyFileChange : stat error <" << iFile << '>' );
	  }
      }
    
    return false;
  }
  //---------------------------------------------------------
  //  bool PPFile::FileExists(const char* iFile)
  //  {
  //    struct stat lBuf;
  //    return (stat(iFile, &lBuf) == 0);
  //  }
  bool PPFile::IsDir(const std::string& iFile, int* oErrno)
  {
    struct stat lBuf;
    
    int lRet =::stat(iFile.c_str(), &lBuf);
    if( lRet == 0 )
      {
	return S_ISDIR( lBuf.st_mode );
      }

    if( oErrno )
      {
	*oErrno = errno;
      }
    else
      {	
	if( errno == ENOENT )
	  {	
	    ERR_LOG( "PPFile::IsDir : File path no exist <" << iFile << '>' );
	  }
	else
	  {
	    ERRNO_LOG(   "PPFile::IsDir : stat error <" << iFile << '>' );
	  }
      }
    
    return false;
  }
  
#ifndef _GNU_SOURCE
#define _GNU_SOURCE         /* See feature_test_macros(7) */
#endif
  
#include <fcntl.h>

#undef CopyFile
#define CopyFile CopyFile
  //---------------------------------------------------------
  bool PPFile::CopyFile( const std::string &iIn, const std::string &iOut, const mode_t iMode )
  {
    try{
      std::ifstream src( iIn, std::ios::binary);
      std::ofstream dest(iOut, std::ios::binary);
      dest << src.rdbuf();
    } catch( std::exception & except) // verifier si cela peut arriver
      {
	ERR_LOG(  "PPFile::CopyFile -  failed - Exception : " << except.what() );
	return false;
      }


    return true;
  }
  //---------------------------------------------------------
  bool PPFile::CreateVoidFile( const std::string &iIn, const mode_t iMode )
  {
    int lFd = ::creat( iIn.c_str(), iMode );
    if( lFd < 0 )
      {
	ERRNO_LOG(  "PPFile::creat - failed fo <" <<  iIn << '>' );
	return false;
      }
    ::close( lFd );
    return true;
  }
  //---------------------------------------------------------
  bool PPFile::ReadFile( const std::string &iFileName, std::string& ioStr )
  {    
    try{
      std::ifstream lSrc( iFileName, std::ios::binary);
      if( lSrc )
	{
	  std::ostringstream lOStr;
	  lOStr << lSrc.rdbuf();
	  ioStr = lOStr.str();
	 
	 return true;
	}       
    }catch( ... )
       {
       }
    return false;
  }

  //---------------------------------------------------------
  size_t PPFile::ReadFileLines( const std::string &iFileName, std::vector<std::string>& ioLines, size_t iNbLine  )
  {    
    try{
      std::ifstream lSrc( iFileName );
     
      std::string lLine;
      while(  ioLines.size() < iNbLine
	      && lSrc
	      && std::getline( lSrc, lLine)  )
	{
	  ioLines.push_back( lLine );
	}      	  
      
    }catch(  std::exception & iEx )
       {
	 ERRNO_LOG(  "ReadFileLines exception " <<  iEx.what() << " <<<" << iFileName << ">>>"  );
	 return 0;
       }
    return ((int32_t)ioLines.size()); 
  }
  //---------------------------------------------------------
  bool PPFile::WriteFile( const std::string &iFileName, const char* oStr, size_t iSz )
  {
    try{
      std::ofstream lOsFile( iFileName, std::ios::binary | std::ios::trunc );
      
      if( lOsFile  ) 
      {
	lOsFile.write( oStr, iSz );
	DBG_FIL("Writing data into "<< iFileName << " Sz:" << iSz);	
	return true;
      } else
	{
	  ERRNO_LOG(  "WriteFile failed for  <<<" << iFileName << ">>>"  );
	  return false;
	} 
      lOsFile.close();
      
    }catch( std::exception & iEx )
       {
	 ERRNO_LOG(  "WriteFile exception " <<  iEx.what() << " <<<" << iFileName << ">>>"  );
       }
    return false;
  }    
  //---------------------------------------------------------
  bool PPFile::WriteFile( const std::string &iFileName, const std::string& ioStr )
  {
    return WriteFile( iFileName, ioStr.c_str(), ioStr.length() );    
  }    
  //---------------------------------------------------------
  bool PPFile::AddFile( const std::string &iFileName, const std::string& ioStr )
  {
    try{
      std::ofstream lOsFile( iFileName, std::ios::binary |  std::ios::app  );
      
      if( lOsFile  ) 
      {	
	lOsFile << ioStr;	
	return true;
      } else
	{
	  ERRNO_LOG(  "AddFile failed for  <<<" << iFileName << ">>>"  );
	  return false;
	} 
      lOsFile.close();
      
    }catch( std::exception & iEx )
       {
	 ERRNO_LOG(  "AddFile exception " <<  iEx.what() << " <<<" << iFileName << ">>>"  );
       }
    return false;
  }    
 //************************************
} // end namespace
