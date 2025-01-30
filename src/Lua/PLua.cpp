#include "PLua.h"

#include <condition_variable>


#define lua_open()  luaL_newstate()

// for multitreading
//  #define LOCK std::unique_lock<std::mutex> lLock( sContainerLuaSessionsMutex );
#define LOCK


namespace PLua{

  char PLuaSession::sLibSep='_'; //Separateur entre lib et function

  PLuaSession::ContainerSessionLua PLuaSession::sContainerLuaSessions ;//= nullptr;
  std::mutex                       PLuaSession::sContainerLuaSessionsMutex;// = nullptr;
	
  PLuaSession* PLuaSession::sPLuaSessionPrototype=nullptr;
  PLuaSession::ContainerCLibraryRegister PLuaSession::sContainerCLibraryRegister;
  
  //*************************************************
  // CLibraryRegistry
  /*
    PLuaSession::ContainerCLibraryRegister *PLuaSession::sContainerCLibraryRegister = nullptr;
	
    unsigned long HashAssocibraryRegistry( PLuaSession::ContainerCLibraryRegister::PAssocAtom *A )
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
  PLuaSession::RegisterCLibrary( const std::string& pLibraryName, CLibraryRegisterFunction pFtn )
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
      if( sContainerLuaSessionsMutex == nullptr && sContainerLuaSessions == nullptr )
      {
      sContainerLuaSessionsMutex = new std::mutex();
      LOCK;
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
  PLuaSession::registerFunction( const std::string& iLibName,  const std::string& iName,
                                 const std::string& iAbbrev, const std::string& iHelp,
                                 CLibraryFonction iFtn )
  {
    PCOUT << "registerFunction " << iLibName << " "<<  iName;

	
    std::string lStr( iLibName );
    if( sLibSep != 0 )
      {
	lStr += sLibSep;
      }
    lStr += iName;
		
    PCOUT << "registerFunction " << iLibName << " "<<  iName << "->" << lStr << std::endl;

	
    lua_register( cLuaState, lStr.c_str(),  iFtn );
    cVectRegisterFtn.insert( { lStr, { iName, lStr, iAbbrev, iHelp, iFtn }} );


    if( iAbbrev.size() > 0 )
      {
        lStr = "";
        if( sLibSep != 0 )
          {
            lStr = sLibSep;
          }
        lStr += iAbbrev;
        
        lua_register( cLuaState, lStr.c_str(),  iFtn );
      }

    return true;
  }
  //------------------------------------------------
  void 
  PLuaSession::registerInternals()
  {	
    lua_pushlightuserdata( cLuaState, (void *)&sContainerLuaSessions);  /* push class address */
    lua_pushlightuserdata( cLuaState, this );
    lua_settable( cLuaState , LUA_REGISTRYINDEX);

    std::cout <<">>>>>>>>>>>>>>>>>>>>> this: "<<  this << std::endl;

    lua_register( cLuaState, "PPrint",         LUA_PrintParametersToStream  );
    lua_register( cLuaState, "PPrintln",       LUA_PrintParametersToStreamLn  );
    lua_register( cLuaState, "PLoadLib",       LUA_LoadCLibrary  );
    lua_register( cLuaState, "PListLibFtn",    LUA_ListLibraryFonction );
    lua_register( cLuaState, "PListLib",       LUA_ListLibrary );
    lua_register( cLuaState, "PLoadFile",      LUA_LoadFile );
  }
  //------------------------------------------------
  void 
  PLuaSession::registerExternals()
  {
	
  }
  //------------------------------------------------
  // Only for prototype

  PLuaSession::PLuaSession()
    :cNameSession( "Prototype")
    ,cLuaState( nullptr)
    ,cCurrentStream(nullptr)
  {
    //// OS_TRACE( "PLuaSession::PLuaSession 1 Prototype " );
  }

  //------------------------------------------------
  PLuaSession::PLuaSession( std::ostream* pStream )
    :cNameSession( "Temporary" )
    ,cLuaState( nullptr)
    ,cCurrentStream(pStream)
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
  PLuaSession::PLuaSession( const std::string& pNameSession,
			    std::ostream* pStream  )
    :cNameSession( pNameSession )
    ,cLuaState( nullptr)
    ,cCurrentStream(pStream)
  {
    OS_TRACE( "PLuaSession::PLuaSession 3" );

    // on creer un contexte d'execution de Lua
    cLuaState = lua_open();

    // on charge les bibliotheques standards de Lua
    luaL_openlibs(cLuaState);
    registerInternals();
    registerExternals();



    { //  LOCK
      LOCK;
      std::unique_lock<std::mutex> lLock( sContainerLuaSessionsMutex );
      sContainerLuaSessions.insert( {cNameSession, this} );
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

    cLuaState = nullptr;
  }
  //------------------------------------------------
  void 
  PLuaSession::AddSession( PLuaSession* pSession )
  {
    Init();

    LOCK;
    std::unique_lock<std::mutex> lLock( sContainerLuaSessionsMutex );
    sContainerLuaSessions.insert( { pSession->cNameSession, pSession} );
  }
  //------------------------------------------------
  bool 
  PLuaSession::RemoveSession( PLuaSession* iSession )
  {
    {
      LOCK;
      return sContainerLuaSessions.erase( iSession->cNameSession) !=0;
    }
    return false;
  }
  //------------------------------------------------
  bool 
  PLuaSession::RemoveSession( std::string& iSessionName )
  {
    {
      LOCK;
      return sContainerLuaSessions.erase( iSessionName  );
    }
    return false;
  }
  //------------------------------------------------
  PLuaSession* 
  PLuaSession::GetSession( const std::string& iSessionName, std::ostream* iStream )
  {    
    PLuaSession* lSession = nullptr;
    
    try {
      LOCK;
      
      lSession = sContainerLuaSessions.at( iSessionName );
      if( iStream != lSession->cCurrentStream && iStream != nullptr)
	lSession->cCurrentStream = iStream;	  
      return lSession;
    }catch( ... )
      { ;}	    	  
    
    return nullptr;
  }
  //------------------------------------------------
  PLuaSession* 
  PLuaSession::getNewPrototypeSession( const std::string& iSessionName, std::ostream* iStream )
  {
    return new PLuaSession( iSessionName, iStream );
  }
  //------------------------------------------------
  
  PLuaSession*  
  PLuaSession::GetOrCreateSession( const std::string& iSessionName, std::ostream* iStream )
  {
    PLuaSession* lSession = nullptr;
    
    if( (lSession=GetSession(iSessionName, iStream ) ) == nullptr)
      {
	if( sPLuaSessionPrototype != nullptr )
	  lSession = sPLuaSessionPrototype->getNewPrototypeSession(iSessionName, iStream );
	else
	  lSession = new PLuaSession(  iSessionName, iStream );

	AddSession( lSession );
      } 
    return lSession;
  }
  //------------------------------------------------
  //------------------------------------------------
  //------------------------------------------------
  bool
  PLuaSession::loadCLibrary( const std::string & pCLibraryName )
  {
    CLibraryRegisterFunctionStruct* lRegistryFtn = nullptr;

    try{
      lRegistryFtn = sContainerCLibraryRegister.at( pCLibraryName );
      
      if( (*lRegistryFtn->cFtnRegist)( pCLibraryName.c_str(), *this ) )
	return true;
      
    } catch(...){;}
  
    return false;
  }
  //------------------------------------------------
  const char*
  PLuaSession::doCode( const char* pCode, std::ostream* iStream )
  {
    std::ostream* lSavStream=nullptr ;
    if( iStream != nullptr )
      {
	lSavStream = setCurrentStream( iStream );
      }
    
    std::cout <<"DBG>docode <" << pCode << ">" << std::endl;
    
    //	if (luaL_dofile( cLuaState, pCode)!=0)
    int lRet =luaL_dostring( cLuaState, pCode);
    if (lRet!=0)
      {	
	if( cCurrentStream )
	  {
	    *cCurrentStream << "ERROR Lua::doCode - luaL_dostring "
			    << lua_tostring( cLuaState ,-1) ;//<< std::endl;
	  }
	else {
	  std::cerr << "ERROR Lua::doCode - luaL_dostring "
		    << lua_tostring( cLuaState ,-1) << std::endl;
	}
	
	// il y a eu une erreur dans le script
	return lua_tostring( cLuaState ,-1);
      }
    
    if( lSavStream != nullptr )
      {
	setCurrentStream( lSavStream  );
      }
		
    return nullptr;
  }
  //------------------------------------------------
  const char*
  PLuaSession::doFile( const char* pFile, std::ostream* iStream)
  {
    std::ostream* lSavStream=nullptr	;
    if( iStream != nullptr )
      {
	lSavStream = setCurrentStream( iStream );
      }

    int lRet =  luaL_dofile( cLuaState, pFile );
		
    if( iStream != nullptr )
      {
	setCurrentStream( lSavStream  );
      }
		
    if( lRet != 0 )
      {
	// il y a eu une erreur dans le script
	OS_TRACE( "PLuaSession::doFile : " << pFile );
				
	return lua_tostring( cLuaState ,-1);
      }

    return nullptr;
  }
  //------------------------------------------------
  //------------------------------------------------
  //------------------------------------------------

  std::ostream&  
  PLuaSession::out()
  {
    if( cCurrentStream == nullptr )
      THROW_OS_ERROR( "PLuaSession::out", "stream is null" );

    return *cCurrentStream;
  }
  //------------------------------------------------

  std::ostream&  
  PLuaSession::err()
  {
    if( cCurrentStream == nullptr )
      THROW_OS_ERROR( "PLuaSession::err", "stream is null" );

    return *cCurrentStream;
  }

  //------------------------------------------------
  PLuaSession& 
  PLuaSession::lua_GetSession(lua_State* pLua)
  {
    lua_pushlightuserdata( pLua, (void *)&sContainerLuaSessions);  /* push class address */
    lua_gettable( pLua, LUA_REGISTRYINDEX);  /* retrieve value */
    void* lPtr  = lua_touserdata(pLua, -1);  /* convert to number */

    std::cout <<">>>>>>>>>>>>> LUA_GetSession: "<<  lPtr<< std::endl;

    if( lPtr == nullptr )
      THROW_OS_ERROR( "PLuaSession::lua_GetSession", "lua session is null" );

	
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
      std::ostream&  lOut( lLuaSession.PLuaSession::out() ); 
      //  std::ostream&  lErr( lLuaSession.PLuaSession::err() );
      
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
  CLUA_CLOSE_CODE(1)
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
 
 
  CLUA_CLOSE_CODE(1)
  //------------------------------------------------
  CLUA_OPEN_CODE( (PLuaSession::LUA_ListLibraryFonction), 0)

  //lOut << "Params:" << lNbParams << std::endl;
  for( auto lPair: lLuaSession.cVectRegisterFtn )
    {
      lOut << lPair.first << "\t: " << lPair.second.cHelp;
        if( lPair.second.cAbbrev.size()> 0 )
          lOut << "\t -> " <<  lPair.second.cAbbrev;
      lOut << std::endl;
      PCOUT << lPair.first << std::endl;
      lua_pushstring( pLua, lPair.first.c_str() );
    }
  PCOUT << lLuaSession.cVectRegisterFtn.size() << std::endl;
  return (int)lLuaSession.cVectRegisterFtn.size();

  CLUA_CLOSE_CODE( 0 )

  //------------------------------------------------
  CLUA_OPEN_CODE( (PLuaSession::LUA_ListLibrary), 0)

  //lOut << "Params:" << lNbParams << std::endl;
	

  // A TERME RENVOYER LES VALEURS DANS  UN TABLEAU LUA
  for( auto lPair: sContainerCLibraryRegister )
    {
      lOut << lPair.first << std::endl;
      PCOUT << lPair.first<< std::endl;
      lua_pushstring( pLua, lPair.first.c_str() );
    }
  PCOUT << lLuaSession.cVectRegisterFtn.size() << std::endl;
  return (int)lLuaSession.cVectRegisterFtn.size();

  CLUA_CLOSE_CODE( 0 )

  
  //*************************************************

} // fin namespace PLua
 
