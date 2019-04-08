#ifndef __PLUA__
#define __PLUA__


#include "lua/lua.h"
#include "lua/lua.hpp"

#include <string>
#include <unordered_map>
#include <map>
#include <iostream>
#include <sstream>
#include <mutex>

#include "PPExcept.h"


struct lua_State;


 
 
#define PCOUT    std::cout
#define OS_TRACE(A)  { std::cerr << A << std::endl; }
#define OS_ERR_MIDDLE(A)  { std::cerr << A << std::endl; }
#define THROW_OS_ERROR( A, B ) { std::cerr << A << " : " << B <<  std::endl; throw std::exception(); }  


namespace PLua {

 
  //*************************************************

  class PLuaSession { 

  public:
    typedef	std::unordered_map< std::string, PLuaSession*> ContainerSessionLua;



  protected:
    std::string          cNameSession;
    lua_State            *cLuaState;       // the lua 
    std::ostream         *cCurrentStream;

    // Only for prototype
    PLuaSession();
    PLuaSession( std::ostream* pStream = nullptr );
    PLuaSession( const std::string& pNameSession,
		 std::ostream* pStream =nullptr );

  public:
    virtual     ~PLuaSession();
    const char* getSessionName() { return cNameSession.c_str(); }


  public:
    typedef int (*CLibraryFonction)(lua_State*); 
    bool registerFunction( const std::string& pLibName, const std::string& pName, CLibraryFonction pFtn);

  protected:
    virtual PLuaSession* getNewPrototypeSession(   const std::string& iSessionName, std::ostream* iStream );
	
  public:
    const char* doCode( const char* pCode);  // run lua code
    const char* doFile( const char* pFile);  // run lua file code
    void  setCurrentStream( std::ostringstream* pStream ) { cCurrentStream = pStream; }

    virtual void  registerInternals();
    virtual void  registerExternals();

    bool loadCLibrary( const std::string& pCLibraryName );
	


    //	STATIC 	STATIC 	STATIC 	STATIC 	STATIC 

  private:

    static 	ContainerSessionLua    sContainerLuaSessions;       // all the open lua session
    static 	std::mutex             sContainerLuaSessionsMutex;

    static void AddSession( PLuaSession* pSession );
    static bool RemoveSession( PLuaSession* iSession );             // remove session by pointer
    static bool RemoveSession( std::string &iSessionName );         // remove session by name

  public:
    static PLuaSession* GetSession( const std::string& iSessionName, std::ostream* pStream=nullptr );
    static PLuaSession* GetOrCreateSession( const std::string& pSessionName,  std::ostream* pStream =nullptr );

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
	
    std::ostream&     out() throw( std::exception );
    std::ostream&     err() throw( std::exception );

	
    // Fonctions appelables dans du code lua (voir PApplication.cc pour leur nom lua)
    static int LUA_PrintParametersToStream(lua_State* pLua);
    static int LUA_PrintParametersToStreamLn(lua_State* pLua);
    static int LUA_LoadCLibrary(lua_State* pLua);
    static int LUA_ListLibraryFonction(lua_State* pLua);
    static int LUA_ListLibrary(lua_State* pLua);
    static int LUA_LoadFile(lua_State* pLua);

    //	static int LUA_GetServerTimeUTC70(lua_State* pLua);


    // For function registry 
  public:
    // definition du type des fonctions d'enregistrement
    typedef bool (*CLibraryRegisterFunction)(const char*, PLuaSession&); 

  protected:


    std::map<std::string, CLibraryFonction> cVectRegisterFtn; // tableau des fonctions deja enregistrer

    // la structure qui stocke les fonctions de registration
    struct CLibraryRegisterFunctionStruct{

      CLibraryRegisterFunction cFtnRegist;
 
      CLibraryRegisterFunctionStruct( CLibraryRegisterFunction pFtnRegist  )
	:cFtnRegist( pFtnRegist )
      {
      }
    };


  public:
    //   typedef	std::unordered_map< std::string , CLibraryRegisterFunctionStruct*> ContainerCLibraryRegister;
    using ContainerCLibraryRegister = std::unordered_map< std::string , CLibraryRegisterFunctionStruct*> ;

    static bool RegisterCLibrary( const std::string& pLibraryName, CLibraryRegisterFunction pFtn );
    static ContainerCLibraryRegister sContainerCLibraryRegister;
    /*
    static bool CmdLuaCLibraryList( ExtendIOStream& pOs, bool pLas,  CmdPList* pCmdList, int pSecLevelt );
    */
  };
  //*************************************************
	       
}; // fin namespace PLua


#define CLUA_OPEN_CODE( NAME, NBPARAM)					\
  int NAME( lua_State* pLua )						\
  {									\
    int _NbReturn = 0;							\
    try {								\
      PLuaSession& lLuaSession( PLuaSession::lua_GetSession( pLua ));	\
      std::ostream&  lOut( lLuaSession.PLuaSession::out());		\
      std::ostream&  lErr( lLuaSession.PLuaSession::err());		\
      int lNbParam = lua_gettop(pLua);					\
      if( (lNbParam-1) != NBPARAM )					\
	{								\
	  lErr << "Bad parameters number for "  << #NAME << ": " << (lNbParam-1) << " since " << NBPARAM << std::endl; \
	  if( std::uncaught_exception() == false )			\
	    THROW_STREAM(  (#NAME), "Bad parameters number");		\
	  return 0;							\
	}                                                               \
      if(false)lOut <<	"just for avoid unused variable warning"; 	\


#define CLUA_CLOSE_CODE(A)			\
  } catch(...) {}				\
    if( _NbReturn == 0 )			\
      _NbReturn = A;				\
    return _NbReturn;				\
    }						\

	


#endif
