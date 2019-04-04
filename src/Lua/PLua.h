#ifndef __PLUA__
#define __PLUA__


#include "lua/lua.hpp"

#include <string>
#include <unordered_map>
#include <map>
#include <iostream>
#include <sstream>
#include <mutex>




struct lua_State;


 
namespace PLua {


  //*************************************************

  class PLuaSession { 

  public:
    typedef	std::unordered_map< std::string, PLuaSession*> ContainerSessionLua;



  protected:
    std::string          cNameSession;
    lua_State            *cLuaState;

    std::ostringstream      *cCurrentStream;

    // Only for prototype
    PLuaSession();

    PLuaSession( std::ostringstream* pStream = NULL );
    PLuaSession( const char* pNameSession, std::ostringstream* pStream =NULL );

  public:
    virtual     ~PLuaSession();
    const char* getSessionName() { return cNameSession.c_str(); }


  public:
    typedef int (*CLibraryFonction)(lua_State*); 
    bool registerFunction( std::string& pLibName, std::string& pName, CLibraryFonction pFtn, int pSecurityLevel);

  protected:
    virtual PLuaSession* getNewPrototypeSession(  const char* pNameSession, std::ostringstream* pStream );
	
  public:
    const char* doCode( const char* pCode);
    const char* doFile( const char* pFile);
    void  setCurrentStream( std::ostringstream* pStream ) { cCurrentStream = pStream; }

    virtual void  registerInternals();
    virtual void  registerExternals();

    bool loadCLibrary( const char* pCLibraryName );
	


    //	STATIC 	STATIC 	STATIC 	STATIC 	STATIC 

  private:

    static 	ContainerSessionLua    sContainerLuaSessions;
    static 	std::mutex             sContainerLuaSessionsMutex;

    static void AddSession( PLuaSession* pSession );
    static bool RemoveSession( PLuaSession* pSession );
    static bool RemoveSession( const char* pSessionName );

  public:
    static PLuaSession* GetSession( const char* pSessionName, std::ostringstream* pStream =NULL );
    static PLuaSession* GetOrCreateSession( int pSecurityLevel, const char* pSessionName, std::ostringstream* pStream =NULL );

    // use for derivated class

  public:
    static PLuaSession* sPLuaSessionPrototype;
  protected:
    static void SetPrototype( PLuaSession* pSession ) { sPLuaSessionPrototype = pSession; }



  public:
    static bool Init();	


    /*
      static bool CmdLua( ExtendIOStream& os, bool pLast,  CmdPList* pCmdPList, int pSecLevel  );
      static bool CmdLuaFile( ExtendIOStream& os, bool pLast,  CmdPList* pCmdPList, int pSecLevel  );
      static bool CmdLuaSession( ExtendIOStream& os, bool pLast,  CmdPList* pCmdPList, int pSecLevel  );
      static bool CmdLuaSessionFile( ExtendIOStream& os, bool pLast,  CmdPList* pCmdPList, int pSecLevel  );
      static bool CmdLuaSessionClose( ExtendIOStream& os, bool pLast,  CmdPList* pCmdPList, int pSecLevel );
      static bool CmdLuaSessionList( ExtendIOStream& os, bool pLas,  CmdPList* pCmdList, int pSecLevelt );
    */


    static int PrintParametersToStream(lua_State* pLua, bool pLn);

    // Fonction a utilser dans le code C++ appelé par lua 
    static PLuaSession& lua_GetSession(lua_State* pLua);
	
    std::ostringstream&     out() throw( std::exception );
    std::ostringstream&     err() throw(  std::exception );

	
    // Fonctions appelables dans du code lua (voir PApplication.cc pour leur nom lua)
    static int LUA_PrintParametersToStream(lua_State* pLua);
    static int LUA_PrintParametersToStreamLn(lua_State* pLua);
    static int LUA_LoadCLibrary(lua_State* pLua);
    static int LUA_ListLibraryFonction(lua_State* pLua);
    static int LUA_ListLibrary(lua_State* pLua);
    static int LUA_LoadFile(lua_State* pLua);

    //	static int LUA_GetServerTimeUTC70(lua_State* pLua);




    // CLibraryRegistry


  public:
    // definition du type des fonctions d'enregistrement
    typedef bool (*CLibraryRegisterFunction)(const char*, PLuaSession&); 

  protected:


    std::map<const char*, const char*> cVectRegisterFtn; // tableau des fonctions deja enregistrer

    // la structure qui stocke les fonctions de registration
    struct CLibraryRegisterFunctionStruct{

      CLibraryRegisterFunction cFtnRegist;
      //		int cSecurityLevel;

      CLibraryRegisterFunctionStruct( CLibraryRegisterFunction pFtnRegist  )
	:cFtnRegist( pFtnRegist )
      {
      }
    };


  public:
    typedef	std::unordered_map< std::string , CLibraryRegisterFunctionStruct*> ContainerCLibraryRegister;

    static bool RegisterCLibrary( std::string& pLibraryName, CLibraryRegisterFunction pFtn );
    static ContainerCLibraryRegister sContainerCLibraryRegister;
    /*
    static bool CmdLuaCLibraryList( ExtendIOStream& pOs, bool pLas,  CmdPList* pCmdList, int pSecLevelt );
    */


    // CLibraryRegistry


  };
  //*************************************************
	
	

}; // fin namespace PLua


#define CLUA_OPEN_CODE( NAME, NBPARAM)					\
  int NAME( lua_State* pLua )						\
  {									\
    int _NbReturn = 0;							\
    try {								\
      PLuaSession& lLuaSession( PLuaSession::lua_GetSession( pLua ));	\
      std::ostrstream&  lOut( lLuaSession.PLuaSession::out());		\
      std::ostrstream&  lErr( lLuaSession.PLuaSession::err());		\
      int lNbParam = lua_gettop(pLua);					\
      if( (lNbParam-1) != NBPARAM )					\
	{								\
	  lErr << "Bad parameters number for "  << #NAME << ": " << (lNbParam-1) << " since " << NBPARAM << std::endl; \
	  if( std::uncaught_exception() == false )			\
	    throw IM_Error( (#NAME), std::string("Bad parameters number"), ERR_GRAV_0, LOG_INTERNE, ERR_LUA_PARAMETERS, __FILE__, __LINE__ ); \
	  return 0;							\
	}								\


#define CLUA_CLOSE_CODE(A)			\
  } catch(...) {}				\
    if( _NbReturn == 0 )			\
      _NbReturn = A;				\
    return _NbReturn;				\
    }						\

	


#endif
