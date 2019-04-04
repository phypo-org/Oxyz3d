#include "PLua.h"

#include <condition_variable>


namespace PLua{


  PLuaSession::ContainerSessionLua PLuaSession::sContainerLuaSessions ;//= NULL;
  std::mutex                       PLuaSession::sContainerLuaSessionsMutex;// = NULL;
	
  PLuaSession* PLuaSession::sPLuaSessionPrototype=NULL;

 
#define PCOUT std::cout
 
  //*************************************************
  // CLibraryRegistry
  /*
  PLuaSession::ContainerCLibraryRegister *PLuaSession::sContainerCLibraryRegister = NULL;
	
  unsigned long HashAssocCLibraryRegistry( PLuaSession::ContainerCLibraryRegister::PAssocAtom *A )
  {
    return PPTools::hash_string( A->cKey );
  }
  //------------------------------------------------
  long CmpAssocCLibraryRegistry( PLuaSession::ContainerCLibraryRegister::PAssocAtom *A,
				 PLuaSession::ContainerCLibraryRegister::PAssocAtom *B,
				 void*)
  {	
    return strcmp( A->cKey, B->cKey );
  }
  */
  //------------------------------------------------

  bool 
  PLuaSession::RegisterCLibrary( std::string& pLibraryName, CLibraryRegisterFunction pFtn )
  {
    Init();
    sContainerCLibraryRegister.insert( { pLibraryName,  new CLibraryRegisterFunctionStruct( pFtn)} );
    return true;
  }
/*
  //------------------------------------------------
  long 
  CmpStrRegisterFtn( const char* A, const char*B, void * )
  {
    return strcmp( A, B );
  }
*/	
  // CLibraryRegistry
  //*************************************************




  //*************************************************
  /*
  unsigned long HashAssoc( PLuaSession::ContainerSessionLua::PAssocAtom *A )
  {
    return PPTools::hash_string( A->cKey );
  }

  long CmpAssoc( PLuaSession::ContainerSessionLua::PAssocAtom *A,
		 PLuaSession::ContainerSessionLua::PAssocAtom *B, void* )
  {	
    return strcmp( A->cKey, B->cKey );
  }
  */
  //*************************************************
  bool
  PLuaSession::Init()
  {
    /*
    if( sContainerLuaSessionsMutex == NULL && sContainerLuaSessions == NULL )
      {
	sContainerLuaSessionsMutex = new std::mutex();
	std::unique_lock<std::mutex> lLock(*sContainerLuaSessionsMutex);
	PPTools::PSimpleLocker lLock( *sContainerLuaSessionsMutex );
	sContainerLuaSessions = new ContainerSessionLua( HashAssoc, CmpAssoc );


	sContainerCLibraryRegister = new ContainerCLibraryRegister( HashAssocCLibraryRegistry, CmpAssocCLibraryRegistry );
			
	return true;
      } 
    */
    return false; 
  } 
  //------------------------------------------------
  //------------------------------------------------
  //------------------------------------------------
  bool 
  PLuaSession::registerFunction( std::string& pLibName, std::string& pName, CLibraryFonction pFtn, int pSecurityLevel  )
  {
    PCOUT << "registerFunction " << pLibName << " "<<  pName;

	
    std::string pStr( pLibName );
    pStr += '_';
    pStr += pName;
	
    lua_register( cLuaState, pStr.get(),  pFtn );
    cVectRegisterFtn.add( pStr.give() );

    return true;
  }
  //------------------------------------------------
  void 
  PLuaSession::registerInternals()
  {	
    lua_pushlightuserdata( cLuaState, (void *)sContainerLuaSessions);  /* push class address */
    lua_pushlightuserdata( cLuaState, this );
    lua_settable( cLuaState , LUA_REGISTRYINDEX);

    std::cout <<">>>>>>>>>>>>>>>>>>>>> this: "<<  this << std::endl;

    lua_register( cLuaState, "Ors_Cprint",         LUA_PrintParametersToStream  );
    lua_register( cLuaState, "Ors_Cprintln",       LUA_PrintParametersToStreamLn  );
    lua_register( cLuaState, "Ors_CloadLib",       LUA_LoadCLibrary  );
    lua_register( cLuaState, "Ors_ClistLibFtn",    LUA_ListLibraryFonction );
    lua_register( cLuaState, "Ors_ClistLib",       LUA_ListLibrary );
    lua_register( cLuaState, "Ors_CLoadFile",      LUA_LoadFile );
  }
  //------------------------------------------------
  void 
  PLuaSession::registerExternals()
  {
	
  }
  //------------------------------------------------
  // Only for prototype

  PLuaSession::PLuaSession()
    :cNameSession( "Prototype" ),
     cLuaState( NULL),
     cCurrentStream(NULL),
     cSecurityLevel( 0 ),
     cVectRegisterFtn( NULL, NULL, 0 )
  {
    OS_TRACE( "PLuaSession::PLuaSession 1 Prototype "  );
  }
  //------------------------------------------------
  PLuaSession::PLuaSession( int pSecurityLevel, std::ostrstream* pStream )
    :cNameSession( "Temporary" ),
     cLuaState( NULL),
     cCurrentStream(pStream),
     cSecurityLevel( pSecurityLevel ),
     cVectRegisterFtn( CmpStrRegisterFtn, CmpStrRegisterFtn, 32 )
  {
    OS_TRACE( "PLuaSession::PLuaSession 2" );

    // on creer un contexte d'execution de Lua
    cLuaState = lua_open();

    // on charge les bibliotheques standards de Lua
    luaL_openlibs(cLuaState);
    registerInternals();
    registerExternals();
  }
  //------------------------------------------------
  PLuaSession::PLuaSession( int pSecurityLevel, const char* pNameSession,std::ostrstream* pStream  )
    :cNameSession( pNameSession ),
     cLuaState( NULL),
     cCurrentStream(pStream),
     cSecurityLevel( pSecurityLevel ),
     cVectRegisterFtn( CmpStrRegisterFtn, CmpStrRegisterFtn, 32 )
  {
    OS_TRACE( "PLuaSession::PLuaSession 3" );

    // on creer un contexte d'execution de Lua
    cLuaState = lua_open();

    // on charge les bibliotheques standards de Lua
    luaL_openlibs(cLuaState);
    registerInternals();
    registerExternals();



    { //  LOCK
      PPTools::PSimpleLocker lLock( *sContainerLuaSessionsMutex );
      sContainerLuaSessions->add( cNameSession.get(), this );
    } //  UNLOCK
  }
  //------------------------------------------------
  PLuaSession::~PLuaSession()
  {
    OS_TRACE( "PLuaSession::~PLuaSession" );

    if( RemoveSession( this ) == false )
      {
	OS_ERR_MIDDLE( "PLuaSession::~PLuaSession " << cNameSession << " not found" );
	//		FAIRE UNE RECHERCHE SEQUENTIELLE POUR DETRUIRE CETTE3 PUTAIN DE LIGNE !!!
      }

    if(cLuaState ) 
      lua_close( cLuaState );

    cLuaState = NULL;
  }
  //------------------------------------------------
  void 
  PLuaSession::AddSession( PLuaSession* pSession )
  {
    Init();

    PPTools::PSimpleLocker lLock( *sContainerLuaSessionsMutex );
    sContainerLuaSessions->add( pSession->cNameSession.get(), pSession );
  }
  //------------------------------------------------
  bool 
  PLuaSession::RemoveSession( PLuaSession* pSession )
  {
    if( sContainerLuaSessions )
      {
	PPTools::PSimpleLocker lLock( *sContainerLuaSessionsMutex );
	return sContainerLuaSessions->removeType( pSession->cNameSession.get() );
      }
    return false;
  }
  //------------------------------------------------
  bool 
  PLuaSession::RemoveSession( const char* pSessionName )
  {
    if( sContainerLuaSessions )
      {
	PPTools::PSimpleLocker lLock( *sContainerLuaSessionsMutex );
	return sContainerLuaSessions->removeType( pSessionName );
      }
    return false;
  }
  //------------------------------------------------
  PLuaSession* 
  PLuaSession::GetSession(  int pSecurityLevel, const char* pSessionName, std::ostrstream* pStream )
  {
    if( sContainerLuaSessions )
      {
	PPTools::PSimpleLocker lLock( *sContainerLuaSessionsMutex );

	PLuaSession* lSession = NULL;

	if( sContainerLuaSessions->get( pSessionName, lSession ) )
	  { 
	    if( pStream != lSession->cCurrentStream && pStream != NULL)
	      lSession->cCurrentStream = pStream;
	    return lSession ;			
	  }
      } 
    return NULL;
  }
  //------------------------------------------------
  PLuaSession* 
  PLuaSession::getNewPrototypeSession( int pSecurityLevel, const char* pNameSession, std::ostrstream* pStream )
  {
    return new PLuaSession( pSecurityLevel, pNameSession, pStream );
  }
  //------------------------------------------------

  PLuaSession*  
  PLuaSession::GetOrCreateSession( int pSecurityLevel, const char* pSessionName, std::ostrstream* pStream )
  {
    if( pSessionName == NULL )
      THROW_OS_ERROR( "PLuaSession::GetOrCreateSession", "SessionNamestream is null", ERR_GRAV_0, LOG_INTERNE, ERR_STREAM_NULL );

    PLuaSession* lSession = NULL;
    if( (lSession=GetSession( pSecurityLevel, pSessionName ) ) == NULL)
      {
	if( sPLuaSessionPrototype != NULL )
	  lSession = sPLuaSessionPrototype->getNewPrototypeSession( pSecurityLevel, pSessionName, pStream );
	else
	  lSession = new PLuaSession( pSecurityLevel, pSessionName, pStream );

	AddSession( lSession );
      } 
    return lSession;
  }
  //------------------------------------------------
  //------------------------------------------------
  //------------------------------------------------
  bool
  PLuaSession::loadCLibrary( const char* pCLibraryName )
  {
    CLibraryRegisterFunctionStruct* lRegistryFtn = NULL;
	
    if( sContainerCLibraryRegister->get( pCLibraryName, lRegistryFtn ) )
      {
	if(	(*lRegistryFtn->cFtnRegist)( pCLibraryName, *this ) )
	  return true;
      }
    return false;
  }
  //------------------------------------------------
  const char*
  PLuaSession::doCode( const char* pCode)
  {
    //	if (luaL_dofile( cLuaState, pCode)!=0)
    if (luaL_dostring( cLuaState, pCode)!=0)
      {
	// il y a eu une erreur dans le script
	return lua_tostring( cLuaState ,-1);
      }
    return "lua>Ok";
  }
  //------------------------------------------------
  const char*
  PLuaSession::doFile( const char* pFile)
  {
    if (luaL_dofile( cLuaState, pFile )!=0)
      {
	OS_TRACE( "PLuaSession::doFile : " << pFile );
	// il y a eu une erreur dans le script
	return lua_tostring( cLuaState ,-1);
      }
	
    return "lua>Ok";
  }
  //------------------------------------------------
  //------------------------------------------------
  //------------------------------------------------
  bool 
  PLuaSession::CmdLua( ExtendIOStream& pOs, bool pLast,  CmdPList* pCmdPList, int pSecLevel  )
  {
    static long lNbSession = 0;

    //	pOs <<"CmdLua"  << std::endl;
    OS_TRACE( "Cmd");
    PCOUT << "Cmd" << std::endl ;
	
    const char* lCode     = pOs.getInputBuffer();
    if( lCode == NULL || strlen( lCode )<=0)
      {
	pOs << "No lua code found !" << std::endl;
      }
    //	pOs <<"Code:"  << lCode << std::endl;
    OS_TRACE( "Code:"<< lCode);
    PCOUT << "Code:" << lCode << std::endl ;
	
	
    PLuaSession lLuaSession( pSecLevel, &pOs); // session temporaire a la duré de l'appel
 	
    pOs << lLuaSession.doCode( lCode ) << std::endl;
	
    return TRUE;	
  }
  //------------------------------------------------
  bool 
  PLuaSession::CmdLuaFile( ExtendIOStream& pOs, bool pLast,  CmdPList* pCmdPList, int pSecLevel  )
  {
    static long lNbSession = 0;


    //	pOs <<"CmdLua"  << std::endl;
    OS_TRACE( "Cmd");
    PCOUT << "Cmd" << std::endl ;
	
    const char* lFilename    = pOs.getInputBuffer();
    if( lFilename == NULL || strlen( lFilename )<=0)
      {
	pOs << "No lua filename found !" << std::endl;
      }
    pOs <<"File:"  << lFilename << std::endl;
    OS_TRACE( "File:"<< lFilename );
    PCOUT << "File:" << lFilename << std::endl ;
	
    PLuaSession lLuaSession( pSecLevel, &pOs); // session temporaire a la duré de l'appel
	
    pOs << lLuaSession.doFile( lFilename ) << std::endl;
	
    return TRUE;	
  }
  //------------------------------------------------
  bool 
  PLuaSession::CmdLuaSession( ExtendIOStream& pOs, bool pLast,  CmdPList* pCmdPList, int pSecLevel  )
  {
    //	pOs <<"CmdLuaSession"  << std::endl;
    OS_TRACE( "CmdLuaSession");
    PCOUT << "CmdLuaSession" << std::endl ;
	
    const char* lSession  = pOs.getString();
    if( lSession == NULL || strlen( lSession )<=0 )
      {
	pOs << "No lua session !" << std::endl;
	return TRUE;	
      }
    //	pOs <<"Session:"  << lSession << std::endl;
    OS_TRACE( "Session:"<< lSession);
    PCOUT << "Session:" << lSession << std::endl ;
	
    const char* lCode     = pOs.getInputBuffer();
    if( lCode == NULL || strlen( lCode )<=0)
      {
	pOs << "No lua code found !" << std::endl;
      }
    //	pOs <<"Code:"  << lCode << std::endl;
    OS_TRACE( "Code:"<< lCode);
    PCOUT << "Code:" << lCode << std::endl ;
	
	
    PLuaSession * lLuaSession = GetOrCreateSession( pSecLevel, lSession, &pOs  );

    pOs << lLuaSession->doCode( lCode ) << std::endl;
	
    return TRUE;	
  }
  //------------------------------------------------
  bool 
  PLuaSession::CmdLuaSessionFile( ExtendIOStream& pOs, bool pLast,  CmdPList* pCmdPList, int pSecLevel  )
  {
    //	pOs <<"CmdLuaSessionFile"  << std::endl;
    OS_TRACE( "CmdLuaSessionFile");
    PCOUT << "CmdLuaSessionFile" << std::endl ;
	
    const char* lSession  = pOs.getString();
    if( lSession == NULL || strlen( lSession )<=0 )
      {
	pOs << "No lua session !" << std::endl;
	return TRUE;	
      }
    //	pOs <<"Session:"  << lSession << std::endl;
    OS_TRACE( "Session:"<< lSession);
    PCOUT << "Session:" << lSession << std::endl ;
	
    const char* lCode     = pOs.getInputBuffer();
    if( lCode == NULL || strlen( lCode )<=0)
      {
	pOs << "No lua filename found !" << std::endl;
      }
    //	pOs <<"File:"  << lCode << std::endl;
    OS_TRACE( "File:"<< lCode);
    PCOUT << "File:" << lCode << std::endl ;
	
    PLuaSession * lLuaSession = GetOrCreateSession( pSecLevel, lSession, &pOs  );

    pOs << lLuaSession->doFile( lCode ) << std::endl;
	
    return TRUE;	
  }
  //------------------------------------------------
  //------------------------------------------------
  //------------------------------------------------
  bool 
  PLuaSession::CmdLuaSessionClose( ExtendIOStream& pOs, bool pLast,  CmdPList* pCmdPList, int pSecLevel )
  {
    //	pOs << "CmdLuaSessionClose"  << std::endl;
    OS_TRACE( "CmdLuaSessionClose");
    PCOUT << "CmdLuaSessionClose" << std::endl ;
	
    const char* lSession  = pOs.getString();
    if( lSession == NULL || strlen( lSession )<=0 )
      {
	pOs << "No lua session !" << std::endl;
	return TRUE;	
      }

    pOs <<"Session:"  << lSession << std::endl;
    OS_TRACE( "Session:"<< lSession);
    PCOUT << "Session:" << lSession << std::endl ;

    if( RemoveSession( lSession ) )
      pOs << lSession << " removed." << std::endl ;
    else
      pOs << lSession << " not found."  << std::endl;


    return TRUE;	
  }
  //------------------------------------------------
  bool 
  PLuaSession::CmdLuaSessionList( ExtendIOStream& pOs, bool pLast,  CmdPList* pCmdPList, int pSecLevel )
  {
    PPTools::PSimpleLocker lLock( *sContainerLuaSessionsMutex );

    PPTools::PAssocTypeIter<const char*, PLuaSession*> lIter( *sContainerLuaSessions );
    while( lIter.hasNext() )
      {
	PPTools::PAssocType<const char*, PLuaSession*>::PAssocAtom *A = lIter.next();
	pOs << A->cData->cNameSession << std::endl;
      }

    return TRUE;
  }
  //------------------------------------------------
  //------------------------------------------------
  //------------------------------------------------

  std::ostrstream&  
  PLuaSession::out() throw( IM_Error )
  {
    if( cCurrentStream == NULL )
      THROW_OS_ERROR( "PLuaSession::out", "stream is null", ERR_GRAV_0, LOG_INTERNE, ERR_STREAM_NULL );

    return *cCurrentStream;
  }
  //------------------------------------------------

  std::ostrstream&  
  PLuaSession::err() throw( IM_Error )
  {
    if( cCurrentStream == NULL )
      THROW_OS_ERROR( "PLuaSession::err", "stream is null", ERR_GRAV_0, LOG_INTERNE, ERR_STREAM_NULL );

    return *cCurrentStream;
  }

  //------------------------------------------------
  PLuaSession& 
  PLuaSession::lua_GetSession(lua_State* pLua)
  {
    lua_pushlightuserdata( pLua, (void *)sContainerLuaSessions);  /* push class address */
    lua_gettable( pLua, LUA_REGISTRYINDEX);  /* retrieve value */
    void* lPtr  = lua_touserdata(pLua, -1);  /* convert to number */

    std::cout <<">>>>>>>>>>>>> LUA_GetSession: "<<  lPtr<< std::endl;

    if( lPtr == NULL )
      THROW_OS_ERROR( "PLuaSession::lua_GetSession", "lua session is null", ERR_GRAV_0, LOG_INTERNE, ERR_LUA_SESSION_NULL );

	
    return *((PLuaSession*)lPtr);
  }
  //------------------------------------------------
  //------------------------------------------------
  //------------------------------------------------
  int 
  PLuaSession::PrintParametersToStream(lua_State* pLua, bool pLn)
  {
    try { 
      PLuaSession& lLuaSession( PLuaSession::lua_GetSession( pLua )); 
      std::ostrstream&  lOut( lLuaSession.PLuaSession::out()); 
      std::ostrstream&  lErr( lLuaSession.PLuaSession::err()); 
      int lNbParam = lua_gettop(pLua); \
	
	
      for (int i = 1; i < lNbParam; i++)
	{				
	  if (lua_isnumber(pLua,i))
	    {
	      lOut << lua_tonumber(pLua,i);
	    }
	  else if (lua_isboolean(pLua,i))
	    {
	      if (lua_toboolean(pLua,i) )
		lOut << "true";
	      else
		lOut << "false";
	    }
	  else if (lua_isstring(pLua,i))
	    {
	      lOut << lua_tostring(pLua,i);
	    }

	  lOut << ' ';
	}
	
      if( pLn )
	lOut << std::endl;

    } catch(...){
    }		
    return 0;
  } 
  //------------------------------------------------
  int 
  PLuaSession::LUA_PrintParametersToStream(lua_State* pLua)
  {
    PrintParametersToStream( pLua, false );
    return 0;		
  }
  //------------------------------------------------
  int 
  PLuaSession::LUA_PrintParametersToStreamLn(lua_State* pLua)
  {
    PrintParametersToStream( pLua, true );
    return 0;
  }
  //------------------------------------------------
  CLUA_OPEN_CODE( (PLuaSession::LUA_LoadCLibrary), 1)

  //lOut << "Params:" << lNbParams << std::endl;

  for (int i = 1; i < lNbParam; i++)
    {
      if (lua_isstring(pLua,i))
	{
	  const char* lLibrary = lua_tostring(pLua,i);
	  if( lLuaSession.loadCLibrary( lLibrary ) == false )
	    {
	      lErr << "Clibrary loading failed : " << i <<  " : " << lLibrary << std::endl ;
	      lua_pushboolean( pLua, 0 );  
	    }
	  else
	    lua_pushboolean( pLua, 1 );  
	}
      else
	{
	  lErr << "Bad type for Clibrary parameter : " << i << "  string is requeired"<< std::endl;
	  lua_pushboolean( pLua, 0 );  
	  //	renvoyer un code d'erreur pour lua
	}
    }
  CLUA_CLOSE_CODE(1);	
  //------------------------------------------------
  CLUA_OPEN_CODE( (PLuaSession::LUA_LoadFile), 1)

  //lOut << "Params:" << lNbParams << std::endl;

  if (lua_isstring(pLua,1))
    {
      const char* lFile = lua_tostring(pLua,1);
      if (luaL_dofile( pLua, lFile )!=0)
	{
	  lua_pushboolean( pLua, 0 );			 
	}
      else
	lua_pushboolean( pLua, 1 );
 
    }
  else
    lua_pushboolean( pLua, 0 );


  CLUA_CLOSE_CODE(1);	
  //------------------------------------------------
  CLUA_OPEN_CODE( (PLuaSession::LUA_ListLibraryFonction), 0)

  //lOut << "Params:" << lNbParams << std::endl;
	


  // A TERME RENVOYER LES VALEURS DANS  UN TABLEAU LUA

  PPTools::PVectorTypeIter<const char*> lIter( lLuaSession.cVectRegisterFtn );
  while( lIter.hasNext() )
    {
      const char* lStr = lIter.next();
      lOut << lStr << std::endl;
      PCOUT << lStr << std::endl;
      lua_pushstring( pLua, lStr );
    }
  PCOUT << lLuaSession.cVectRegisterFtn.size() << std::endl;
  return (int)lLuaSession.cVectRegisterFtn.size();

  CLUA_CLOSE_CODE( 0 );	

  //------------------------------------------------
  CLUA_OPEN_CODE( (PLuaSession::LUA_ListLibrary), 0)

  //lOut << "Params:" << lNbParams << std::endl;
	


  // A TERME RENVOYER LES VALEURS DANS  UN TABLEAU LUA

  PPTools::PAssocTypeIter<const char*, CLibraryRegisterFunctionStruct*> lIter( *sContainerCLibraryRegister );
  while( lIter.hasNext() )
    {
      ContainerCLibraryRegister::PAssocAtom *lAtom = lIter.next();

      lOut << lAtom->cKey << std::endl;
      PCOUT << lAtom->cKey << std::endl;
      lua_pushstring( pLua, lAtom->cKey );
    }
  PCOUT << lLuaSession.cVectRegisterFtn.size() << std::endl;
  return (int)lLuaSession.cVectRegisterFtn.size();

  CLUA_CLOSE_CODE( 0 );	

  //*************************************************






}; // fin namespace PLua
 
