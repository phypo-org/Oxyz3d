#ifndef H__PApplication__H
#define H__PApplication__H


#include "PPTools/PDefinition.h"
#include "MTServer.h"

struct lua_State;

class Env;
	
//*************************************************

class PApplication{


public:

	enum TypeExploit {
		TYPE_EXPLOIT_UNKNOWN,
		TYPE_EXPLOIT_DEV,
		TYPE_EXPLOIT_HOMOLO,
		TYPE_EXPLOIT_PROD
	};


// supprime par JPC le 7/3/2012 pour limiter l'usage de -DUSE_OPENSSL #ifdef USE_OPENSSL
	enum ModeConnectOpenSSL {
		OPENSSL_NONE,
		OPENSSL_ONLY,
		OPENSSL_OPTIONAL
	};

	static ModeConnectOpenSSL sModeSSL;
	AgentListen *cAgentListenSSL;

	virtual AgentListen* initOpenSSL(){ return NULL;}

// #endif
		
	static TypeExploit GetTypeExploit() {return sTypeExploit; }
	static TypeExploit GetTypeExploit(const char*pStr,TypeExploit pDefault );
	static const char* GetStrTypeExploit( TypeExploit  );
	static const char* GetStrTypeExploit( ) { return GetStrTypeExploit(sTypeExploit);} 


	PApplication();

	void setStandAlone() { sStandAlone = true; }
	bool getStandAlone() { return sStandAlone; }

	bool initAndRunAll( const char* pType, const char* pVersion, int argc, char* argv[], bool pDaemon, int *pModeTest = NULL );
	void reloadIni();

protected:
	virtual void help(){;}
	virtual bool initParameters( const char* pStr ){return false;}
	virtual bool initAfterLog(){return true;}
	virtual	bool initEnv(){return true;}
	virtual bool initApplication(){return true;}
	virtual MTServer* initServer(){ return NULL;}
	virtual bool initAgents(){return true;}
	virtual bool initCmdVect(){return true;}


private:
	static Env* OpenEnv();

public:
	static int LUA_GetCmdPListVar( lua_State* );
	static int LUA_SetCmdPListVar( lua_State* );

	static bool CmdSetMinTraceLevel( ExtendIOStream& os, bool last, CmdPList* pCmdPList, int pSecLevel);
public:

	static bool sStandAlone;
	static bool sIsStarting;
	static char const* LOG_PATH;		
	static char const* INI_PATH;
	static char const* INI_FILE; 
	
	static char const* OPENSSL_CERTIFICATE_FILE; 
	static char const* OPENSSL_KEY_FILE; 

	
	static int  LOG_LINE_MAXSIZE;
	static int  MODE_TEST;
  static TypeExploit sTypeExploit;
	
	static const char* sAppliName;
	static Env        *sRegenerateEnv;
	static Env        *sMyEnv;

	
	static bool Init1StdParameters( const char* argv );		
	static void Init2Log( const char* pAppliType );
	static Env* Init3Env(  );		
	static void Init4Daemon(  const char* pAppliType, const char* pVersion, bool pDaemon = false );
	static void Init5CmdVect();
	
	static void MakeRegenerationIni();
	static void Help();
	static void ServerEnding();
	
	static bool IsEnvVar( const char* pSection, const char* pName );
	
	static bool GetIniBool(   const char* pSection, const char* pName, bool    & pVal );
	static bool GetIniInt(    const char* pSection, const char* pName, int    & pVal );
	static bool GetIniLong(   const char* pSection, const char* pName, long    & pVal );
	static bool GetIniDouble( const char* pSection, const char* pName, double & pVal );
	static bool GetIniString( const char* pSection, const char* pName, const char*& pVal );
	static bool GetIniString( const char* pSection, const char* pName, char*& pVal );
	static EnvSectionPtr GetSection( const char* pSection);
};
	
//*************************************************


#endif
