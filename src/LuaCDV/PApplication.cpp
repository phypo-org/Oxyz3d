/************************************************************************
 * $Branch: SharedApplications$
 *
 * $Archive: SharedApplications/OrsLibsSrv/MTServer/src/PApplication.cc$
 *
 * $Date: 09/20/2012 06:13:18 PM$
 *
 * $Revision: 32$
 *
 * $Author: jpch$
 *
 * Description:
 ************************************************************************/
static const char* _PApplication_cc_id_ = "@(#)$Header: PApplication.cc: Revision: 32: Author: jpch: Date: 09/20/2012 06:13:18 PM$ (CDV)";

#include "PApplication.h"

#include "Orbsock/OrbSock.h"

#ifndef _AWLinux
#include <thread.h>
#endif


 
#ifdef NO_STD
#include <iostream.h>
#include <iomanip.h>
#include <fstream.h>
#else
#include <iostream>
#include <iomanip>
#endif

#include <unistd.h>
#include <math.h>

#include <sys/types.h>
#include <sys/resource.h>
#include <unistd.h>
#include <netdb.h>

#include <stdio.h>
#include <stdlib.h>

#include "Objs/Thread.h"
#include "Objs/Env.h"
#include "Objs/Trace.h"
#include "Objs/File.h"
#include "Objs/Trace.h"


#include "ptools/pstring.h"
#include "ptools/PXml.h"
#include "ptools/cmdlist.h"
#include "ptools/PAutoPtr.h"

#include "MTServer.h"

#ifdef USE_PLUA
#include "PLua.h"
#endif

#include "AgentManager.h"
#include "AgentTune.h"
#include "AgentTuneXml.h"
#include "AgentClientSA.h"
#include "AgentTrace.h"


#include "Orbsock/OrtMsg.h"


#include "PPTools/PParam.h"


#ifdef USE_OPENSSL
#include "Orbsock/OrbSockSSL.h"

PApplication::ModeConnectOpenSSL PApplication::sModeSSL=OPENSSL_NONE;

#endif

using namespace PPTools;

char const* PApplication::LOG_PATH = NULL ;
char const* PApplication::INI_PATH = NULL ;
char const* PApplication::INI_FILE = NULL ; 
	
int         PApplication::LOG_LINE_MAXSIZE = 10000;

Env        *PApplication::sMyEnv = NULL;
Env        *PApplication::sRegenerateEnv = NULL;
const char* PApplication::sAppliName="NoName";
int         PApplication::MODE_TEST = FALSE;

bool PApplication::sStandAlone = false;
bool PApplication::sIsStarting = true;

char const* PApplication::OPENSSL_CERTIFICATE_FILE=NULL; 
char const* PApplication::OPENSSL_KEY_FILE=NULL; 

PApplication::TypeExploit  PApplication::sTypeExploit =  PApplication::TYPE_EXPLOIT_UNKNOWN;

//------------------------------------------------

const char* 
PApplication::GetStrTypeExploit(PApplication::TypeExploit pTypeExploit)
{
  switch( pTypeExploit )
    {
    case TYPE_EXPLOIT_UNKNOWN:
      return "Unknown" ;
    case TYPE_EXPLOIT_DEV:
      return "Dev";
    case TYPE_EXPLOIT_HOMOLO:
      return "Homolo";
    case TYPE_EXPLOIT_PROD:
      return "Prod";
    }

  return  "Unknown" ;
}

//------------------------------------------------

PApplication::TypeExploit 
PApplication::GetTypeExploit( const char* pStr, PApplication::TypeExploit  pDefault  )
{
  if( pStr )
    {
      if( strncasecmp( pStr, "Dev", 3 ) == 0 )
        return PApplication::TYPE_EXPLOIT_DEV;
      else
        if( strncasecmp( pStr, "Prod", 4 ) == 0 )
          return PApplication::TYPE_EXPLOIT_PROD;
        else
          if( strncasecmp( pStr, "Homolo", 6 ) == 0 )
            return PApplication::TYPE_EXPLOIT_HOMOLO;
    }
  return pDefault;
}

//*************************************************
PApplication::PApplication()
#ifdef USE_OPENSSL
  :cAgentListenSSL(NULL)
#endif
{
#ifdef SERVER_TEST_CLIENT
  sTypeExploit =  TYPE_EXPLOIT_DEV;
#endif
}
//------------------------------------------------
bool
PApplication::initAndRunAll( const char* pType, const char* pVersion,
														 int argc,  char* argv[],
														 bool pDaemon,
														 int *pModeTest)
{
	try{
		for ( int i = 1 ; i < argc ; i++ ) 
			{
				char*  lStr = &argv[i][0] ;

				if( initParameters( lStr ) == true )
					{
						std::cout << "return true pour " << lStr << std::endl;
						continue;
					}
        //		std::cout << "return false" << std::endl;
				
				if( PApplication::Init1StdParameters( lStr ) == false )
					{
						std::cout << "Unknown parameter <<<" << lStr << ">>>" << std::endl;
						help();
						Help();
						exit(5);
					}
			}

    PXml::PXmlFluxWriter::Init(); //initialisation de libxml2 pour le multithreading // FAIRE un xmlCleanupParser  ?

		PApplication::Init2Log( pType  );
		
		if( initAfterLog() == false )
			return false;
		
		MTServer::SetEnv( PApplication::Init3Env());
		
		if( initEnv() == false )
			::exit( 4 );

		
		PApplication::MakeRegenerationIni();
		
		
		PApplication::Init4Daemon( pType , pVersion, pDaemon );
		
    if( initApplication()== false )
			::exit(1);



		MTServer* lServer( initServer() );
		if( lServer == NULL )
			::exit(3);


#ifdef USE_OPENSSL
    if( sModeSSL == OPENSSL_OPTIONAL )
      {
        OrbSockSSL::InitLibSSL();
        cAgentListenSSL = initOpenSSL();
        OS_TRACE( "listen agent in OPENSSL_OPTIONAL mode running ... ");
      }
    else
    if( sModeSSL == OPENSSL_ONLY )
          {
            OrbSockSSL::InitLibSSL();
            OS_TRACE( "listen agent in OPENSLL_ONLY mode ");
            lServer->setUseSSL();
          }
#endif


    // phipo 20081127
		// Lancement du thread serveur (ecoute du service et arret si le meme serveur tourne deja 
		if( pModeTest == NULL || *pModeTest == 0 )
			{
				lServer->run() ;
				std::cout << " *********** RUN ************ " << std::endl ;

        // On attende que le serveur est reussi son listen (sauf s'il y echoue)
        while( MTServer::IsDispo() == FALSE )
          {
            if( lServer->isStoping() )
              break;
            sleep(1);
          }

#ifdef USE_OPENSSL
        if( cAgentListenSSL != NULL )
          {
            OS_TRACE( "AgentListenSSL.run "  );
            cAgentListenSSL->run() ;
          }
#endif
			}





		if( lServer->isStoping() == false )
      {
        
        Init5CmdVect();
        if( initCmdVect()== false )
          ::exit(2);

       
        if( initAgents())
          {
            if( getStandAlone() == false )
              AgentTune::LaunchTune( );
            
            
            lServer->setDispo( TRUE ); 
            sIsStarting = false;

            // Appel d'exec de l'agentManager sans le lancer en temps que thread.Il s'execute sur le thread principal
            new AgentManager( "Manager", lServer ) ;
            AgentBase::SetKey( AgentBase::KeyThreadObj, NULL ) ;
            AgentManager::TheAgentManager->exec() ;		
          }
        else
          MTServer::TheServer->stopServer();
      }
  }
  catch( IM_Error& ime )
    {
      OS_ERROR( ERR_GRAV_FATAL, LOG_INTERNE, 0, 
                "server :" << PApplication::sAppliName << " Exception :" << ime  << " STOPPED " << std::endl );
      
			exit(2) ;
    }
	PApplication::ServerEnding();

	xmlCleanupParser();
	::exit(0) ;
}

//------------------------------------------------
void 
PApplication::reloadIni()
{
		MTServer::SetEnv( PApplication::Init3Env());		
    initEnv();      
}
//*************************************************
	
bool 
PApplication::Init1StdParameters( const char* argv)
{
	std::cout << "Init1StdParameters : " << argv << std::endl;

#ifndef _AWLinux 
	long                procs           = sysconf(_SC_NPROCESSORS_ONLN) ;
	long                l_concur        = thr_getconcurrency() ;
	std::cout << "Processeurs = " << procs << " Concurrency = " << l_concur << std::endl ;
#endif

  //======================
	const char* lOrbSockTraceFile=NULL;
	if( PParam::GetParamString( "-OrbSockTraceMsg", argv, lOrbSockTraceFile, "-M" ) )
		{
      if( lOrbSockTraceFile == NULL || strlen( lOrbSockTraceFile ) == 0 )
        {
          OrbSock::SetStreamTrace( 0 ) ;
        }
      if( strcmp( lOrbSockTraceFile, "out" ) == 0 )
        {
          OrbSock::SetStreamTrace( 1 ) ;
          OS_TRACE( "OrbSock::SetStreamTrace to standart out" );
        }
      else {
        int lFile =-1;
        if( ( lFile = ::open( lOrbSockTraceFile, O_RDWR| O_CREAT, 0666 ) ) == -1 ) 
          {
            OS_TRACE( "Orbsock Trace failed for file " << lOrbSockTraceFile );
          }
        else
          {
            OrbSock::SetStreamTrace( lFile );
            OS_TRACE(  "OrbSock::SetStreamTrace to " << lOrbSockTraceFile  );
          }
      }
        
			OrbSock::SetTraceAll(TRUE);
			return true;
		}
	//======================


	if( PParam::GetParamBool( "-UseOrsMonitor", argv, AgentTune::FLAG_AGENT_TUNE , "-T" ) )
		{		
      		std::cout << ">>>>>>>>>>>>>>>>>>>>>>> UseOrsMonitor " << std::endl;
           OS_TRACE( "UseOrsMonitor" );
			return true;
		}
	
	if( PParam::GetParamString( "-IniPath", argv, INI_PATH, "-I" )) 
		return true;
	
	if( PParam::GetParamString( "-IniFile", argv, INI_FILE, "-i" )) 
		return true;
	
	if( PParam::GetParamString( "-LogPath", argv, LOG_PATH, "-L" )) 
				return true;

	if( PParam::GetParamBool( "-AsyncTrace", argv, AgentTrace::USE_ASYNC_TRACE, "-A" )) 
    return true;
	
	if( PParam::GetParamString( "-Name",    argv, sAppliName, "-S" )) 
		{
      std::cout << "Init1StdParameters Name : " <<  sAppliName<< std::endl;    
      return true;
		}

	
	bool lTmpBool;
	if( PParam::GetParamBool( "-RegenerateIni", argv, lTmpBool, NULL )) 
		{
			sRegenerateEnv =  new Env( sAppliName, "RegenerateEnv.ini", true );
			return true;
		}

	return false;
}
//------------------------------------------------
void 
PApplication::Help()
{
	std::cout << "-I/-IniPath=path du repertoire du .ini" << std::endl ;
	std::cout << "-i/-IniFile=nom du  .ini)              " << std::endl ;
	std::cout << "-L/-LogPath=path du repertoire du log" << std::endl ;
	std::cout << "-A/-AsyncTrace=ecriture asynchrone du log" << std::endl ;
	std::cout << "-S/-Name=nom de l'application"         << std::endl ;
	std::cout << "-M/-OrbSockTraceMsg  : force la trace debug d'Orbsock "        << std::endl ;
	std::cout << "-RegenerateIni       :  cree un fichier RegenerateIni.ini de reference" << std::endl ;
	std::cout << "-T/UseOrsMonitor       : envoit des information au systeme de sureillance" <<  std::endl ;
}
//------------------------------------------------

void
PApplication::Init2Log( const char* pAppliType )
{ 
	
	if ( sAppliName == NULL || *sAppliName == '\0' ) 
		{
			std::cerr << "Application name needed !" << std::endl ;
      //      help();
			Help() ;
			::exit(-1);
		}	

  

	// Mise en place du fichier de log
	PStringDyn          lSyserrName ;
	if ( LOG_PATH ) 
		{
      std::cout << "LOG_PATH:" << LOG_PATH << std::endl;
      
			File            logDir( LOG_PATH ) ;
			
			if ( ! logDir.isDirectory()  ) 
				{
					if ( logDir.mkDirs() ) 
						{
							std::cout << "Le repertoire " 
												<< LOG_PATH << " est cree" << std::endl ;    // Attention, le fichier de log n est pas encore cree
						} 
					else 
						{
							std::cerr << "La creation du repertoire " 
												<< LOG_PATH << " a echouee ; errno = " << errno << std::endl ;
							exit(1) ;
						}
				}

			File logFile( logDir, sAppliName ) ;
      std::cout << "file:" << logFile.pathName().c_str() << std::endl << std::flush ;

			lSyserrName.set( logFile.pathName().c_str() ) ;
      
		}
	else 
		{
			lSyserrName.set( sAppliName ) ;
		}
  

  /******
  // On creer un Env temporaire pour le constructeur de trace
  Env lTmpEnv (sAppliName  );
			
  // default settings
  lTmpEnv.PutBool( EnvSect, EnableVar,    BOOL_TRUE );
  lTmpEnv.PutBool( EnvSect, AppendVar,    BOOL_TRUE );
  lTmpEnv.PutInt(  EnvSect, EnvNumLogs,    32 );
  lTmpEnv.PutStr(  EnvSect, EnvWrapPeriod, "DAY" );
  lTmpEnv.PutBool( EnvSect, TimestampFileNameVar, BOOL_TRUE );
  lTmpEnv.PutInt(  EnvSect, EnvWrapSize, 1900000000);
  lTmpEnv.PutInt(  EnvSect, EnvWrapLines, 0);
  lTmpEnv.PutStr(  EnvSect, FileVar, lSyserrName );
			
  // On ouvre une premiere fois pour recuperer les eventuelle info sur la trace
  EnvSectionPtr lTraceSection=NULL;
  PAutoPtr<Env> lLocalEnv( OpenEnv());
  // on ecrase avec les valeurs de l'utilisateur 
  if( lLocalEnv.isValid() && ( lTraceSection = lLocalEnv->GetSection( EnvSect )))
    {
      for( EnvVariableIterator vi = lTraceSection->GetIterator(); vi; vi++ )
        {
          EnvVariable *variable = *vi;
          lTmpEnv.PutStr( EnvSect, variable->Name(), variable->GetStr() );
        }
    }  
  //	new SysErrStream( lSyserrName.get() ) ;
  new SysErrStream( new Trace( &lTmpEnv ) ) ;
  *****/
  //	SysErrStream::TheTrace->WrapPeriod( LOG_WRAP_PERIOD_DAY ) ;
  //	SysErrStream::TheTrace->WrapSize( 2000000000 ) ;
  //	SysErrStream::TheTrace->NumLogs(32);




  // On ouvre une premiere fois pour recuperer les eventuelle info sur la trace
  EnvSectionPtr lTraceSection=NULL;

  PAutoPtr<Env> laLocalEnv( OpenEnv());
  if( laLocalEnv.isValid() )
      lTraceSection = laLocalEnv->GetSection( EnvSect );

  if( AgentTrace::USE_ASYNC_TRACE )
    {      
      AgentTrace*  lAgentTrace= new AgentTrace(*( new AsyncSysErrStream( lSyserrName, lTraceSection) ) );
      lAgentTrace->run();

      OS_TRACE("ASYNC_TRACE" );

      PCOUT << "ASYNC_TRACE" << std::endl;
   }
  else
    {
      new SysErrStream( lSyserrName, lTraceSection ); // creer SysErrStream::TheTrace
    }
	
	OS_TRACE( "Creation du log:" << lSyserrName.get() );
	
	PStringDyn  tmpstr(  UsePStringFormat, "%s %s", pAppliType, sAppliName ) ;
	SysErrStream::TheTrace->SetApplName( tmpstr.get() ) ;		


  if( PApplication::MODE_TEST >= 3 )
    SysErrStream::TheTrace->WrapSize(  150000000 );
                                          
 
}

//------------------------------------------------
Env* 
PApplication::OpenEnv()
{
	PStringDyn tmpstr;
	
	if ( INI_PATH != NULL )
		{
			tmpstr.set( "%s/%s.ini", INI_PATH, (INI_FILE==NULL?sAppliName: INI_FILE)) ;
		} 
	else 
		{
			tmpstr.set( "%s.ini", (INI_FILE==NULL?sAppliName: INI_FILE) ) ;
		}
	
	std::cout << "read init file " << tmpstr.get() << std::endl ;
	
	Env* lEnv = new Env( sAppliName, tmpstr.get(), true ) ;
	
	if ( !lEnv->InitOk() ) 
		{
			std::cerr << std::endl << "*** Error read init file:" << tmpstr.get() << std::endl << std::endl ;
			exit(1) ;
		} 
	else 
		{
			std::cout << "Read init file OK\n" << std::endl ;
		}

  return lEnv;
}
//------------------------------------------------
Env*
PApplication::Init3Env( )
{	
	if( (sMyEnv = OpenEnv()) == NULL )	
		{
			exit(1) ;
		} 
	
	
	PApplication::GetIniInt(    "PROTOCOL",    "DEBUG_LEVEL",             MTServer::DEBUG_LEVEL );
	//	COrbProtocol::SetDefaultTrace( SysErrStream::TheTrace, MTServer::DEBUG_LEVEL ) ;
	
		
	PApplication::GetIniInt( "APLICATION", "MAX_CONNECTION", MTServer::MAX_CONNECTION ) ;
	PApplication::GetIniInt( "APPLICATION", "BOUND_THREADS",  MTServer::BOUND_THREADS ) ;
	PApplication::GetIniInt( "APPLICATION", "CONCURRENCY",    MTServer::CONCURRENCY ) ;
	
	
	PApplication::GetIniInt(    "APPLICATION", "LOG_LINE_MAXSIZE",       PApplication::LOG_LINE_MAXSIZE );
	PApplication::GetIniInt(    "APPLICATION", "MODE_TEST",              PApplication::MODE_TEST );
	PApplication::GetIniBool(    "APPLICATION", "USE_TRACE",              ErrStream::USE_TRACE );
  //	PApplication::GetIniBool(    "APPLICATION", "USE_ASYNC_TRACE",        AgentTrace::USE_ASYNC_TRACE );

	int lMinTraceLevel;
  if (PApplication::GetIniInt( "APPLICATION", "MIN_TRACE_LEVEL", lMinTraceLevel ) == false)
    {
      lMinTraceLevel = 0;
    }
  SysErrStream::SetMinTraceLevel(lMinTraceLevel);

  PApplication::GetIniInt(    "APPLICATION", "MAX_OPEN_FILE",      MTServer::MAX_OPEN_FILE  );

	
		// deux version .ini TUNE/ORS_MONITOR
	PApplication::GetIniString( "TUNE", "STAT_MACHINE", AgentTune::STAT_MACHINE );
	PApplication::GetIniString( "TUNE", "STAT_ERVICE", AgentTune::STAT_SERVICE );
	PApplication::GetIniString( "TUNE", "STAT_MACHINE_BACKUP", AgentTune::STAT_MACHINE_BACKUP );
	PApplication::GetIniString( "TUNE", "STAT_SERVICE_BACKUP", AgentTune::STAT_SERVICE_BACKUP );
	PApplication::GetIniInt(  "TUNE", "PERIODE_STAT_CONNECT", AgentTune::PERIODE_STAT_CONNECT );
	PApplication::GetIniLong( "TUNE", "TIME_COLLECT", PStatCptCollect::TIME_COLLECT );
	PApplication::GetIniLong( "TUNE", "TIME_INFO_GEN", PStatCptCollect::TIME_INFO_GEN );
  PApplication::GetIniBool( "TUNE", "USE_ASYNC_WRITE",    AgentTune::USE_ASYNC_WRITE );
  PApplication::GetIniString( "TUNE", "TUNE_PROTOCOL_VERSION",   AgentTune::TUNE_PROTOCOL_VERSION  );

  PApplication::GetIniBool( "APPLICATION", "PASSWORD_NO_CONTROL",   MTServer::PASSWORD_NO_CONTROL   );

  const char* lStrTypeExploit = NULL;
  if( PApplication::GetIniString( "APPLICATION", "TYPE_EXPLOIT", lStrTypeExploit ) )
  {
    sTypeExploit = GetTypeExploit( lStrTypeExploit, sTypeExploit );
  }

#ifdef USE_OPENSSL
  PApplication::GetIniString( "APPLICATION", "OPENSSL_CERTIFICATE_FILE",  PApplication::OPENSSL_CERTIFICATE_FILE );
  PApplication::GetIniString( "APPLICATION", "OPENSSL_KEY_FILE",          PApplication::OPENSSL_KEY_FILE );

  bool lUseOpenSSL = false;
  bool lUseOnlySSL = false;
  PApplication::GetIniBool( "APPLICATION", "USE_LISTEN_OPENSSL", lUseOpenSSL  );


  OS_TRACE( "APPLICATION:USE_LISTEN_OPENSSL lUseOpenSSL:" << lUseOpenSSL );
  //  PApplication::GetIniBool( "APPLICATION", "USE_LISTEN_ONLYSSL", lUseOnlySSL  );

  //  PApplication::GetIniBool( "APPLICATION", "USE_CONNEX_OPENSSL", lUseConnexSSL  );

  if( lUseOpenSSL )
    {
      sModeSSL = OPENSSL_OPTIONAL;
      OS_TRACE( "sModeSSL = OPENSSL_OPTIONAL ");
      //      if( lUseOnlySSL )
      //       sModeSSL = OPENSSL_ONLY;
    }

#endif


  PApplication::GetIniString( "ADMINISTRATION", "LDAPServers",    MTServer::LDAPServers      ) ;
  PApplication::GetIniString( "ADMINISTRATION", "LDAPRootDN",     MTServer::LDAPRootDN     ) ;
  PApplication::GetIniString( "ADMINISTRATION", "LDAPLogin",      MTServer::LDAPLogin     ) ;
  PApplication::GetIniString( "ADMINISTRATION", "LDAPPass",       MTServer::LDAPPass    ) ;
  PApplication::GetIniString( "ADMINISTRATION", "LDAPFilter",     MTServer::LDAPFilter      ) ;

  PApplication::GetIniString( "ADMINISTRATION", "LDAP_NDS_Servers",    MTServer::LDAP_NDS_Servers      ) ;
  PApplication::GetIniString( "ADMINISTRATION", "LDAP_NDS_RootDN",     MTServer::LDAP_NDS_RootDN     ) ;
  PApplication::GetIniString( "ADMINISTRATION", "LDAP_NDS_Login",      MTServer::LDAP_NDS_Login     ) ;
  PApplication::GetIniString( "ADMINISTRATION", "LDAP_NDS_Pass",       MTServer::LDAP_NDS_Pass    ) ;
  PApplication::GetIniString( "ADMINISTRATION", "LDAP_NDS_Filter",     MTServer::LDAP_NDS_Filter      ) ;

  PApplication::GetIniInt(    "ADMINISTRATION", "LDAPVerbose",    MTServer::LDAPVerbose      ) ;
  PApplication::GetIniBool(   "ADMINISTRATION", "UseLoginCache",  MTServer::UseLoginCache     ) ;
  PApplication::GetIniInt(    "ADMINISTRATION", "TIMEOUT_WAITLOCK_PASSWORD_SECOND",  MTServer::TIMEOUT_WAITLOCK_PASSWORD_SECOND ) ;

	
	
	PApplication::GetIniString( "ORS_MONITOR", "STAT_MACHINE",        AgentTune::STAT_MACHINE);
	PApplication::GetIniString( "ORS_MONITOR", "STAT_SERVICE",        AgentTune::STAT_SERVICE);
	PApplication::GetIniString( "ORS_MONITOR", "STAT_MACHINE_BACKUP", AgentTune::STAT_MACHINE_BACKUP);
	PApplication::GetIniString( "ORS_MONITOR", "STAT_SERVICE_BACKUP", AgentTune::STAT_SERVICE_BACKUP);
	
	PApplication::GetIniInt(    "ORS_MONITOR", "PERIODE_STAT_CONNECT", AgentTune::PERIODE_STAT_CONNECT);
	PApplication::GetIniLong(   "ORS_MONITOR", "TIME_COLLECT",         PStatCptCollect::TIME_COLLECT);
	PApplication::GetIniLong(   "ORS_MONITOR", "TIME_INFO_GEN",        PStatCptCollect::TIME_INFO_GEN);
  PApplication::GetIniBool(   "ORS_MONITOR", "USE_ASYNC_WRITE",       AgentTune::USE_ASYNC_WRITE );
  PApplication::GetIniString( "ORS_MONITOR", "TUNE_PROTOCOL_VERSION", AgentTune::TUNE_PROTOCOL_VERSION  );
  PApplication::GetIniBool(   "ORS_MONITOR", "STAT_USE_ORTMSG",       AgentTune::STAT_USE_ORTMSG  );


	
	
	PApplication::GetIniString( "ADMINISTRATION", "USER_SA",         MTServer::USER_SA );
	PApplication::GetIniString( "ADMINISTRATION", "PASSWORD_SA",     MTServer::PASSWORD_SA );
	PApplication::GetIniInt(    "ADMINISTRATION", "STOP_GMT_HOUR",   AgentManager::STOP_GMT_HOUR );
	PApplication::GetIniInt(    "ADMINISTRATION", "STOP_GMT_MINUTE", AgentManager::STOP_GMT_MINUTE );

	
	PApplication::GetIniString( "PATH", "LOG_PATH", PApplication::LOG_PATH );
	
	
	PApplication::GetIniLong( "CLIENT", "TIME_OUT_CONNECTION",        MTServer::TIME_OUT_CONNECTION );
	PApplication::GetIniLong( "CLIENT", "ORBSOCK_WRITE_TIMEOUT",      OrbSock::ORBSOCK_WRITE_TIMEOUT );
	PApplication::GetIniLong( "CLIENT", "ORBSOCK_WRITE_TIMEOUT_KILL", OrbSock::ORBSOCK_WRITE_TIMEOUT_KILL );
	PApplication::GetIniLong( "CLIENT", "ORTMSG_MAXSIZE",             OrtMsg::ORTMSG_MAXSIZE );
	PApplication::GetIniInt(  "CLIENT", "ORTMSG_DEFAULT_COMPRESS_LEVEL",     OrtMsg::DEFAULT_COMPRESS_LEVEL );
	PApplication::GetIniInt(  "CLIENT", "ORTMSG_THRESHOLD_SIZE_COMPRESS",    OrtMsg::THRESHOLD_SIZE_COMPRESS );

  // phipo 20100514
	PApplication::GetIniLong( "CLIENT", "ORBSOCK_SIZE_ALARM_ASYNC", OrbSock::ORBSOCK_SIZE_ALARM_ASYNC );
	PApplication::GetIniLong( "CLIENT", "ORBSOCK_SIZE_MAX_ASYNC",  OrbSock::ORBSOCK_SIZE_MAX_ASYNC );

  PApplication::GetIniLong( "CLIENT", "MAX_FREE_MSG_STORE",         OrbSock::MAX_FREE_MSG_STORE );
	PApplication::GetIniInt( "CLIENT", "MAX_NBMSG_READ",             AgentConnex::MAX_NBMSG_READ );
	PApplication::GetIniInt( "CLIENT", "MAX_NBMSG_SEND",             AgentConnex::MAX_NBMSG_SEND );
	PApplication::GetIniInt( "CLIENT", "WAIT_MSG_POLL",              AgentConnex::WAIT_MSG_POLL );
	PApplication::GetIniLong( "CLIENT", "MAX_WAITMSG_KILL",           AgentConnex::MAX_WAITMSG_KILL );
	PApplication::GetIniInt( "CLIENT", "WAIT_RECONNECT_1",           AgentConnexActif::WAIT_RECONNECT_1 );
	PApplication::GetIniInt( "CLIENT", "WAIT_RECONNECT_2",           AgentConnexActif::WAIT_RECONNECT_2 );
	PApplication::GetIniInt( "CLIENT", "NB_RECONNECT_1",             AgentConnexActif::NB_RECONNECT_1 );
  PApplication::GetIniBool( "CLIENT", "USE_ASYNC_WRITE",           MTServer::USE_ASYNC_WRITE );




	return sMyEnv;
}
//------------------------------------------------	
void
PApplication::MakeRegenerationIni()
{
	if( sRegenerateEnv )
		{			
			EnvSectionIterator lSectionIter  =  sMyEnv->GetIterator();
			lSectionIter.begin();
			for(;lSectionIter;lSectionIter++)
				{
					EnvSection* lSecPtr = *lSectionIter;
					EnvVariableIterator lVarIter = lSecPtr->GetIterator();
					
					for(;lVarIter;lVarIter++)
						{
							EnvVariable* lVar = *lVarIter;
							if( sRegenerateEnv->GetSection( lSecPtr->Name() ) == NULL 
									|| sRegenerateEnv->GetSection(lSecPtr->Name())->GetVariable(lVar->Name()) == NULL)
								{
									PStringDyn lOldVar( UsePStringFormat, "??? old ???  %s", lVar->Name());
									sRegenerateEnv->PutStr( lSecPtr->Name(), lOldVar.get(), lVar->GetStr() );
								}
						}
				}
			
			sRegenerateEnv->WriteBack();
			::exit(0);
		}	
}

//------------------------------------------------	
void 
PApplication::Init4Daemon( const char* pAppliType, const char* pVersion, bool pDaemon )
{
	
	//------------- DEMONIFICATION -------------
	if ( MODE_TEST < 2 && pDaemon && sStandAlone == false )
		{
		/*
		** `fork()' so the parent can exit, this returns control to the command
		** line or shell invoking your program.  This step is required so that
		** the new process is guaranteed not to be a process group leader. The
		** next step, `setsid()', fails if you're a process group leader.
		*/
			
			int ret = fork() ;
			switch ( ret ) {
			case 0:
				break ;
				
			case -1:
				std::cerr << "fork returns -1" << std::endl ;
				break ;
				
			default:
				std::cerr << "first fork " << sAppliName << " PID = " << ret << std::endl ;
				::exit (0) ;
			}
			/*
			** `setsid()' to become a process group and session group leader. Since a
			** controlling terminal is associated with a session, and this new
			** session has not yet acquired a controlling terminal our process now
			** has no controlling terminal, which is a Good Thing for daemons.
			*/
			
			if ( setsid() < 0 ) {
				std::cerr << "setsid() failed --> exit" << std::endl ;
			}
			
		/*
		**  `fork()' again so the parent, (the session group leader), can exit.
		** This means that we, as a non-session group leader, can never regain a
		** controlling terminal.
		*/
			
		// PHI 99/05/06   nouveautes a tester en mode daemon !
			ret = fork() ;    // conseil UNIX
			switch( ret ) 
				{
				case 0:
					break ;
					
				case -1:
					std::cerr << "second's fork returns -1" << std::endl ;
					break ;
					
				default:
					std::cerr << "Server " << sAppliName << " PID = " << ret << std::endl ;
					exit(0) ;
				}
			
			/*
			** `umask(0)' so that we have complete control over the permissions of
			** anything we write. We don't know what umask we may have inherited.
			*/
		}
	
	
	//-------------- Agent de recuperation des signaux ------------
	AgentSignalHandler*  agent_signal_hdl = new AgentSignalHandler;
	agent_signal_hdl->run() ;
	
	{
		PStringDyn          tmpstr( pAppliType ) ;
		tmpstr += PApplication::sAppliName ;
		tmpstr += " " ; 
		tmpstr +=  pVersion;
		
		SysErrStream::TheTrace->SetApplName( (char*) tmpstr ) ;			
	}
	
	// Permet au process d'emettre ces signaux jusqu'a ce que tous les threads l'aient intercepte
	struct sigaction vec ;
	sigemptyset(&vec.sa_mask) ;
	sigaddset( &vec.sa_mask, SIGIO ) ;
	sigaddset( &vec.sa_mask, SIGURG ) ;
	sigaddset( &vec.sa_mask, SIGPIPE ) ;
	sigaddset( &vec.sa_mask, SIGALRM ) ;
	sigaddset( &vec.sa_mask, SIGINT ) ;
	pthread_sigmask( SIG_BLOCK, &vec.sa_mask, NULL ) ;
	
}




#ifdef PTEST_USE_PLUA

int LUA_Coucou1(lua_State* pLua)
{
  try {
		PLua::PLuaSession& lLuaSession( PLua::PLuaSession::lua_GetSession( pLua ));
		std::ostrstream&  lOut( lLuaSession.PLuaSession::out());
		std::ostrstream&  lErr( lLuaSession.PLuaSession::err());
    
		lOut << "Coucou 1" << std::endl;
		return 0;
	}
	catch( ... ) {		
	}
	return 1;
	
}

int LUA_Coucou2(lua_State* pLua)
{
  try {
		PLua::PLuaSession& lLuaSession( PLua::PLuaSession::lua_GetSession( pLua ));
		std::ostrstream&  lOut( lLuaSession.PLuaSession::out());
		std::ostrstream&  lErr( lLuaSession.PLuaSession::err());
    
		lOut << "Coucou 2" << std::endl;
		return 0;
	}
	catch( ... ) {		
	}
	return 1;
}

int LUA_Coucou3(lua_State* pLua)
{
  try {
		PLua::PLuaSession& lLuaSession( PLua::PLuaSession::lua_GetSession( pLua ));
		std::ostrstream&  lOut( lLuaSession.PLuaSession::out());
		std::ostrstream&  lErr( lLuaSession.PLuaSession::err());
    
		lOut << "Coucou 3" << std::endl;
		return 0;
	}
	catch( ... ) {		
	}
	return 1;
}

// Pour tester
bool 
TestRegisterCLibrary1( PLua::PLuaSession & pLuaSession )
{
   pLuaSession.registerFunction( "Test.Coucou1",       LUA_Coucou1, CmdVect::SECURITY_LEVEL_ZERO );
   pLuaSession.registerFunction( "Test.Coucou2",       LUA_Coucou2, CmdVect::SECURITY_LEVEL_VIEW );
  return true;
}
 
bool 
TestRegisterCLibrary2( PLua::PLuaSession & pLuaSession  )
{
  pLuaSession.registerFunction( "Test.Coucou3",       LUA_Coucou3, CmdVect::SECURITY_LEVEL_EXPLOIT  );
  return true;
}

bool 
TestRegisterCLibrary3( PLua::PLuaSession & pLuaSession  )
{
  return false;
}
#endif

#ifdef USE_PLUA

//------------------------------------------------

using namespace PLua;

CLUA_OPEN_CODE( PApplication::LUA_GetCmdPListVar, 1 )
 
if( lua_isstring(pLua, 1) )
{
  const char* lName = lua_tostring(pLua, 1);

   CmdPList::SrvVarStruct* var = CmdPList::TheCmdPList->listvar.find( const_cast<char*>(lName) );

  if( var )
    {
      if( var->cLevelSecurity > lLuaSession.getSecurityLevel() )
        {
          lErr << "GetCmdPListVar variable access deny for : " 
               << lName 
               << " Verify your security level." << std::endl;
           return 0;
        }           
		switch( var->typ )
      {
      case  CmdPList::SRV_VAL_BOOL :
        if( (*var->valbool) )
          lua_pushboolean( pLua, 1 );  
        else 
          lua_pushboolean( pLua, 0 );  
        break;
      case  CmdPList::SRV_VAL_STR:
        lua_pushstring( pLua,  (*var->valstr) );
        break;
      case  CmdPList::SRV_VAL_LONG:
        lua_pushnumber( pLua, (*var->vallong) );
        break;
      case  CmdPList::SRV_VAL_INT:
        lua_pushnumber( pLua, (*var->valint));
        break;
      case  CmdPList::SRV_VAL_DOUBLE:
        lua_pushnumber( pLua, (*var->valdouble));
			break;
      }     
    return 1;
    }
  else
    {
      lErr  << "GetCmdPListVar unknown variable : " << lName << std::endl;
    }
 }

CLUA_CLOSE_CODE(0); // return 0 parametre en erreur

//------------------------------------------------

CLUA_OPEN_CODE( PApplication::LUA_SetCmdPListVar, 2 )
  
if( lua_isstring(pLua, 1) == false )  
{
  lErr  << "SetCmdPListVar bad parameter name"<< std::endl;
  lua_pushboolean( pLua, 0 );  
  return 1;
}

const char* lName = lua_tostring(pLua, 1);

 CmdPList::SrvVarStruct* lVar = CmdPList::TheCmdPList->listvar.find( const_cast<char*>(lName) );

if( lVar == NULL )
{     
  lErr  << "GetCmdPListVar unknown variable : " << lName << std::endl;
  lua_pushboolean( pLua, 0 );  
  return 1;
}

if( lVar->cLevelSecurity > lLuaSession.getSecurityLevel() )
{
  lErr << "SetCmdPListVar variable access deny for : " 
       << lName 
       << " Verify your security level." << std::endl;
  lua_pushboolean( pLua, 0 );  
  return 1;
}           
if( lVar->modif == FALSE )
{
  lErr << "SetCmdPListVar read only variable  : " 
       << lName  << std::endl;
  lua_pushboolean( pLua, 0 );  
  return 1;
 
}

switch( lVar->typ )
{
   //-------------------
 case  CmdPList::SRV_VAL_BOOL :
   if (lua_isboolean(pLua,2) == false )
     {
       lErr  << "GetCmdPListVar unknown variable : " << lName << std::endl;
       lua_pushboolean( pLua, 0 );  
       return 1;            
     } 
   *lVar->valbool = lua_toboolean(pLua,2);  
   break;
   
   //-------------------
 case  CmdPList::SRV_VAL_STR:
   {
     if (lua_isstring(pLua,2) == false )
       {
         lErr  << "GetCmdPListVar bad parameter type for : " << lName << std::endl;
         lua_pushboolean( pLua, 0 );  
         return 1;            
       }
     
     const char* lVal = lua_tostring(pLua,2);
     
     int l = (int)::strlen( lVal );
     char* tmp = new char[ l + 1 ] ;
     strcpy( tmp, lVal) ;
     *lVar->valstr = tmp ;     // on va eventuellement perdre de la memoire !     
   }
   break;

   //-------------------   
 case  CmdPList::SRV_VAL_LONG:
   {
     if( lua_isnumber(pLua,2) == false )
       {
         lErr  << "GetCmdPListVar bad parameter type for : " << lName << std::endl;
         lua_pushboolean( pLua, 0 );  
         return 1;            
       }
     

     long lLong;
     lua_number2int( lLong, lua_tonumber(pLua,2));
     
     *(lVar->vallong) = lLong;
   }
   break;
   //-------------------
   
 case  CmdPList::SRV_VAL_INT:
   {
     if( lua_isnumber(pLua,2) == false )
       {
         lErr  << "GetCmdPListVar bad parameter type for : " << lName << std::endl;
         lua_pushboolean( pLua, 0 );  
         return 1;            
       }
     
     int lInt;
     lua_number2int( lInt, lua_tonumber(pLua,2));
     
     *(lVar->valint) = lInt;
   }
   break;
   //-------------------

 case  CmdPList::SRV_VAL_DOUBLE:
   if( lua_isnumber(pLua,2) == false )
     {
       lErr  << "GetCmdPListVar bad parameter type for : " << lName << std::endl;
       lua_pushboolean( pLua, 0 );  
       return 1;            
     }
   
   *(lVar->valdouble) = lua_tonumber(pLua,2);
   break;
}     
lua_pushboolean( pLua, 1 );  

CLUA_CLOSE_CODE(1);
//------------------------------------------------

bool 
LuaRegisterCmdPList( const char* pData, PLua::PLuaSession & pLuaSession  )
{
  pLuaSession.registerFunction( pData, "CgetVar",   PApplication::LUA_GetCmdPListVar, CmdPList::SECURITY_LEVEL_ZERO  );
  pLuaSession.registerFunction( pData, "CsetVar",   PApplication::LUA_SetCmdPListVar, CmdPList::SECURITY_LEVEL_ZERO  );

  return true;
}

/*
bool 
LuaRegisterCmdPList( PLua::PLuaSession & pLuaSession  )
{
  SrvVarStruct* var=NULL;
	int i;

  for( i=0; i< TheCmdPList->listvar.getLen(); i++ )
    {
      var=TheCmdPList->listvar.get(i);
      if( var->cLevelSecurity > pLuaSession.getSecurityLevel() )
        continue;    
      
    }
	}

}
*/
#endif
//------------------------------------------------	
void 
PApplication::Init5CmdVect()  
{ 

#ifdef USE_PLUA 

PLua::PLuaSession::Init();
PLua::PLuaSession::RegisterCLibrary( "MTServer", LuaRegisterCmdPList );

//  PLua::PLuaSession::RegisterCLibrary( "TestCLib1", TestRegisterCLibrary1 );
//  PLua::PLuaSession::RegisterCLibrary( "TestCLib2", TestRegisterCLibrary2 );
//  PLua::PLuaSession::RegisterCLibrary( "TestCLib3", TestRegisterCLibrary3 );

 CmdPList::TheCmdPList->registerFunction(  "lua_lib_list 0",
                                          PLua::PLuaSession::CmdLuaCLibraryList, CmdPList::SECURITY_LEVEL_VIEW, CmdPList::NO_CONFIRMATION,
																				 "Liste les libraries de fonction Ors enregistrées pour lua" ) ;

	CmdPList::TheCmdPList->registerFunction( "lua 0",
    PLua::PLuaSession::CmdLua, CmdPList::SECURITY_LEVEL_EXPLOIT, CmdPList::CONFIRM_LEVEL_PASS,
																				 "Execute un code lua. Usage : lua ..." ) ;

	CmdPList::TheCmdPList->registerFunction( "lua_file 0",
    PLua::PLuaSession::CmdLuaFile, CmdPList::SECURITY_LEVEL_EXPLOIT, CmdPList::CONFIRM_LEVEL_PASS,
																				 "Execute un  fichier de code lua. Usage : lua_file filename" ) ;

	CmdPList::TheCmdPList->registerFunction( "lua_session 0",
    PLua::PLuaSession::CmdLuaSession, CmdPList::SECURITY_LEVEL_EXPLOIT, CmdPList::CONFIRM_LEVEL_PASS,
																				 "Execute un code lua. Usage : lua_session session_name ..." ) ;

	CmdPList::TheCmdPList->registerFunction( "lua_session_file 0",
    PLua::PLuaSession::CmdLuaSessionFile, CmdPList::SECURITY_LEVEL_EXPLOIT, CmdPList::CONFIRM_LEVEL_PASS,
																				 "Execute un fichier de code lua. Usage : lua_session_file session_name filename" ) ;

	CmdPList::TheCmdPList->registerFunction( "lua_session_close 0",
    PLua::PLuaSession::CmdLuaSessionClose, CmdPList::SECURITY_LEVEL_EXPLOIT, CmdPList::CONFIRM_LEVEL_PASS,
																				 "Ferme une session lua : Usage : lua_session_close session_name" ) ;

CmdPList::TheCmdPList->registerFunction( "lua_session_list 0",
  PLua::PLuaSession::CmdLuaSessionList, CmdPList::SECURITY_LEVEL_VIEW, CmdPList::NO_CONFIRMATION,
																				 "Liste les sessions lua ouvertes: Usage : lua_session_list" ) ;



  // faire une fonction lua_close 
  // faire une fonction lua_list
  // faire une fonction lua_exec_file

#endif
	CmdPList::TheCmdPList->registerFunction( "objets 0",
																				 PObjBase::CmdObjList, CmdPList::SECURITY_LEVEL_VIEW, CmdPList::NO_CONFIRMATION,
																				 "Liste des objets geres par le serveur." ) ;
	
	CmdPList::TheCmdPList->registerFunction( "GetGmtTimeServer",
																				 MTServer::CmdGetGmtTimeSrv, CmdPList::SECURITY_LEVEL_VIEW, CmdPList::NO_CONFIRMATION,
																				 "Donne le temps gmt du serveur." ) ;


#ifdef SERVER_TEST_CLIENT
	CmdPList::TheCmdPList->registerFunction( "SetTimeServer",
    MTServer::CmdSetTime, CmdPList::SECURITY_LEVEL_SUPER, CmdPList::CONFIRM_LEVEL_TWO_PASS,
																				 "Regle le serveur a un temps donné !!! (pas la machine !)" ) ;
#endif


   CmdPList::TheCmdPList->registerFunction( "TestPassword",
      MTServer::CmdTestPassword, CmdPList::SECURITY_LEVEL_EXPLOIT, CmdPList::NO_CONFIRMATION,
																					 "Teste la validite d'un password pour un utilisateur. Usage: TestPassword user pass" );

   CmdPList::TheCmdPList->registerFunction( "LoginCache",
      MTServer::CmdLoginCache, CmdPList::SECURITY_LEVEL_EXPLOIT, CmdPList::CONFIRM_LEVEL_ONE,
																					 "Gere le cache des login/password. Usage: LoginCache [empty]" );

   CmdPList::TheCmdPList->registerFunction( "SetMinTraceLevel",
       PApplication::CmdSetMinTraceLevel, CmdPList::SECURITY_LEVEL_EXPLOIT, CmdPList::NO_CONFIRMATION,
                                           "Modifie le niveau de trace. Usage: SetMinTraceLevel level [0-5]" );

   CmdPList::TheCmdPList->registerVar( "UseLoginCache", 
      &MTServer::UseLoginCache, CmdPList::SECURITY_LEVEL_EXPLOIT, CmdPList::CONFIRM_LEVEL_ONE,
      "Arme/desarme l'utilisation du cache des login/password",
     TRUE ) ;

		CmdPList::TheCmdPList->registerVar( "TIMEOUT_WAITLOCK_PASSWORD_SECOND",
      &MTServer::TIMEOUT_WAITLOCK_PASSWORD_SECOND, CmdPList::SECURITY_LEVEL_EXPLOIT, CmdPList::CONFIRM_LEVEL_ONE,
      "Timeout pour l'attente du lock pour le test des login/password",
      TRUE ) ;
    
    CmdPList::TheCmdPList->registerVar( "PASSWORD_NO_CONTROL",
      &MTServer::PASSWORD_NO_CONTROL, CmdPList::SECURITY_LEVEL_EXPLOIT, CmdPList::CONFIRM_LEVEL_PASS,
      "Inhibe le controle des password en conjonction avec MODE_TEST" ,
      TRUE ) ;

	
    CmdPList::TheCmdPList->registerVar( "LOG_LINE_MAXSIZE",
      &LOG_LINE_MAXSIZE, CmdPList::SECURITY_LEVEL_EXPLOIT, CmdPList::CONFIRM_LEVEL_ONE,
      "Taille maximum des lignes de trace" ,
      TRUE) ;
	
    CmdPList::TheCmdPList->registerVar( "USE_TRACE",
      &ErrStream::USE_TRACE, CmdPList::SECURITY_LEVEL_EXPLOIT, CmdPList::CONFIRM_LEVEL_ONE,
                                        "Permet/interdit les traces (pour les tests)" ,
                                        TRUE) ;

    CmdPList::TheCmdPList->registerVar( "USE_ASYNC_TRACE",
      &AgentTrace::USE_ASYNC_TRACE, CmdPList::SECURITY_LEVEL_EXPLOIT, CmdPList::CONFIRM_LEVEL_ONE,
                                        "Permet les trac en asynchrone" ,
                                        TRUE) ;
	
	CmdPList::TheCmdPList->registerVar( "MAX_CONNECTION",
    &MTServer::MAX_CONNECTION , CmdPList::SECURITY_LEVEL_EXPLOIT, CmdPList::CONFIRM_LEVEL_ONE,
    "Nombre maximum de connexions" ,
    FALSE ) ;
	
	CmdPList::TheCmdPList->registerVar( "MAX_NBMSG_READ",
    &AgentConnex::MAX_NBMSG_READ , CmdPList::SECURITY_LEVEL_EXPLOIT, CmdPList::CONFIRM_LEVEL_ONE,
    "nombre maximum de messages lu a la fois par un client" ,
    TRUE ) ;
	
	CmdPList::TheCmdPList->registerVar( "MAX_NBMSG_SEND",
    &AgentConnex::MAX_NBMSG_SEND ,CmdPList::SECURITY_LEVEL_EXPLOIT, CmdPList::CONFIRM_LEVEL_ONE,
    "nombre maximum de messages ecrit a la fois par un client" ,
    TRUE ) ;
	
	CmdPList::TheCmdPList->registerVar( "WAIT_MSG_POLL",
    &AgentConnex::WAIT_MSG_POLL ,CmdPList::SECURITY_LEVEL_EXPLOIT, CmdPList::CONFIRM_LEVEL_ONE,
    "temps d attente sur le poll pour un client" ,
    TRUE ) ;
	
	CmdPList::TheCmdPList->registerVar( "MAX_WAITMSG_KILL",
    &AgentConnex::MAX_WAITMSG_KILL ,CmdPList::SECURITY_LEVEL_EXPLOIT, CmdPList::CONFIRM_LEVEL_ONE,
    "nombre maximum de message dans la queue d'ecriture d'un client" ,
    TRUE ) ;

	CmdPList::TheCmdPList->registerVar( "ORTMSG_MAXSIZE", 
    &OrtMsg::ORTMSG_MAXSIZE,CmdPList::SECURITY_LEVEL_EXPLOIT, CmdPList::CONFIRM_LEVEL_ONE,
    "nombre maximum de message dans la queue d'ecriture d'un client" ,
    TRUE ) ;

  CmdPList::TheCmdPList->registerVar( "ORTMSG_DEFAULT_COMPRESS_LEVEL",
    &OrtMsg::DEFAULT_COMPRESS_LEVEL,CmdPList::SECURITY_LEVEL_EXPLOIT, CmdPList::CONFIRM_LEVEL_ONE,
    "Niveau de compression par defaut pour les OrtMsg" ,
    TRUE ) ;

	CmdPList::TheCmdPList->registerVar( "ORTMSG_THRESHOLD_SIZE_COMPRESS",
    &OrtMsg::THRESHOLD_SIZE_COMPRESS,CmdPList::SECURITY_LEVEL_EXPLOIT, CmdPList::CONFIRM_LEVEL_ONE,
    "Taille declenchant la compression aux données des OrtMsg" ,
    TRUE ) ;

	CmdPList::TheCmdPList->registerVar( "WAIT_RECONNECT_1",
    &AgentConnexActif::WAIT_RECONNECT_1 ,CmdPList::SECURITY_LEVEL_EXPLOIT, CmdPList::CONFIRM_LEVEL_ONE,
    "temps de reconnection rapide apres deconnexion pour un agent actif" ,
    TRUE ) ;

	CmdPList::TheCmdPList->registerVar( "WAIT_RECONNECT_2",
    &AgentConnexActif::WAIT_RECONNECT_2 ,CmdPList::SECURITY_LEVEL_EXPLOIT, CmdPList::CONFIRM_LEVEL_ONE,
    "temps de reconnection lent apres deconnexion pour un agent actif" ,
    TRUE ) ;
	
	CmdPList::TheCmdPList->registerVar( "NB_RECONNECT_1",
    &AgentConnexActif::NB_RECONNECT_1 ,CmdPList::SECURITY_LEVEL_EXPLOIT, CmdPList::CONFIRM_LEVEL_ONE,
    "nombre d'essai de reconnexion rapide avent de passer en reconnexion lente" ,
    TRUE ) ;

	CmdPList::TheCmdPList->registerVar( "CLIENT_USE_ASYNC_WRITE",
    &MTServer::USE_ASYNC_WRITE ,CmdPList::SECURITY_LEVEL_EXPLOIT,  CmdPList::CONFIRM_LEVEL_ONE,
    "Use async write for client/server communication" ,
    TRUE ) ;

	CmdPList::TheCmdPList->registerVar( "MONITOR_USE_ASYNC_WRITE",
    &AgentTune::USE_ASYNC_WRITE ,CmdPList::SECURITY_LEVEL_EXPLOIT,  CmdPList::CONFIRM_LEVEL_ONE,
    "Use async write for monitor communication" ,
    TRUE ) ;

	CmdPList::TheCmdPList->registerVar( "STAT_USE_ORTMSG",
    &AgentTune::STAT_USE_ORTMSG, CmdPList::SECURITY_LEVEL_EXPLOIT,  CmdPList::CONFIRM_LEVEL_ONE,
    "Use OrtMsg for  for monitor communication" ,
    TRUE ) ;


	CmdPList::TheCmdPList->registerVar( "STAT_MACHINE",
    &AgentTune::STAT_MACHINE  ,CmdPList::SECURITY_LEVEL_EXPLOIT,  CmdPList::CONFIRM_LEVEL_ONE,
    "Stat machine" ,
    TRUE ) ;
	CmdPList::TheCmdPList->registerVar( "STAT_MACHINE_BACKUP",
    &AgentTune::STAT_MACHINE_BACKUP  ,CmdPList::SECURITY_LEVEL_EXPLOIT,  CmdPList::CONFIRM_LEVEL_ONE,
    "Stat machine backup" ,
    TRUE ) ;

	CmdPList::TheCmdPList->registerVar( "STAT_SERVICE",
    &AgentTune::STAT_SERVICE  ,CmdPList::SECURITY_LEVEL_EXPLOIT,  CmdPList::CONFIRM_LEVEL_ONE,
    "Stat service" ,
    TRUE ) ;
	CmdPList::TheCmdPList->registerVar( "STAT_SERVICE_BACKUP",
    &AgentTune::STAT_SERVICE_BACKUP  ,CmdPList::SECURITY_LEVEL_EXPLOIT,  CmdPList::CONFIRM_LEVEL_ONE,
    "Stat service backup" ,
    TRUE ) ;

	CmdPList::TheCmdPList->registerVar( "PERIODE_STAT_CONNECT",
    &AgentTune::PERIODE_STAT_CONNECT  ,CmdPList::SECURITY_LEVEL_EXPLOIT,  CmdPList::CONFIRM_LEVEL_ONE,
    "Stat periode connect" ,
    TRUE ) ;
	CmdPList::TheCmdPList->registerVar( "TIME_COLLECT",
    &PStatCptCollect::TIME_COLLECT   ,CmdPList::SECURITY_LEVEL_EXPLOIT,  CmdPList::CONFIRM_LEVEL_ONE,
    "Periode de collecte des information pour OrsMonitor" ,
    TRUE ) ;
	CmdPList::TheCmdPList->registerVar( "TIME_INFO_GEN",
    &PStatCptCollect::TIME_INFO_GEN  ,CmdPList::SECURITY_LEVEL_EXPLOIT,  CmdPList::CONFIRM_LEVEL_ONE,
    "Periode de collecte des information generales pour OrsMonitor" ,
    TRUE ) ;




	CmdPList::TheCmdPList->registerVar( "TUNE_PROTOCOL_VERSION", 
    &AgentTune::TUNE_PROTOCOL_VERSION ,CmdPList::SECURITY_LEVEL_EXPLOIT, CmdPList::CONFIRM_LEVEL_ONE,
    "Protocol version use to send tune data" ,
    TRUE ) ;
	
	CmdPList::TheCmdPList->registerVar( "STOP_GMT_HOUR", 
    &AgentManager::STOP_GMT_HOUR ,CmdPList::SECURITY_LEVEL_EXPLOIT, CmdPList::CONFIRM_LEVEL_PASS,
    "Heure d'arret exprimee en GMT" ,
    TRUE ) ;
	
	CmdPList::TheCmdPList->registerVar( "STOP_GMT_MINUTE",
    &AgentManager::STOP_GMT_MINUTE ,CmdPList::SECURITY_LEVEL_EXPLOIT, CmdPList::CONFIRM_LEVEL_PASS,
    "Minute d'arret exprimee en GMT" ,
    TRUE ) ;


	
	CmdPList::TheCmdPList->registerVar( "MODE_TEST",
    &MODE_TEST,CmdPList::SECURITY_LEVEL_SUPER, CmdPList::CONFIRM_LEVEL_PASS,
    "Place le serveur en mode test (certaine securite en moins, champs debug supplementaire en xml ...)" ,
    TRUE ) ;
	
	
	// section PROTOCOL
	// ----------------
	CmdPList::TheCmdPList->registerVar( "DEBUG_LEVEL",
    &MTServer::DEBUG_LEVEL , CmdPList::SECURITY_LEVEL_EXPLOIT, CmdPList::CONFIRM_LEVEL_PASS,
    "Niveau de debug du protocole" ,
    TRUE ) ;
	

	CmdPList::TheCmdPList->registerVar( "TIME_OUT_CONNECTION", 
    &MTServer::TIME_OUT_CONNECTION, CmdPList::SECURITY_LEVEL_EXPLOIT, CmdPList::CONFIRM_LEVEL_ONE,
    "Periode de timeout sur le poll de la socket",
    TRUE ) ;
	
	CmdPList::TheCmdPList->registerVar( "ORBSOCK_WRITE_TIMEOUT",
    &OrbSock::ORBSOCK_WRITE_TIMEOUT, CmdPList::SECURITY_LEVEL_EXPLOIT, CmdPList::CONFIRM_LEVEL_ONE,
    "Periode de timeout en ecriture pour les sockets - Message a l'exploitation",
    TRUE ) ;

  CmdPList::TheCmdPList->registerVar( "ORBSOCK_MAX_FREE_MSG_STORE", 
    &OrbSock::MAX_FREE_MSG_STORE, CmdPList::SECURITY_LEVEL_EXPLOIT, CmdPList::CONFIRM_LEVEL_ONE,
    "nombre maximum de message libre garder pour reutilisation",
    TRUE ) ;
  
	CmdPList::TheCmdPList->registerVar( "ORBSOCK_WRITE_TIMEOUT_KILL",
    &OrbSock::ORBSOCK_WRITE_TIMEOUT_KILL, CmdPList::SECURITY_LEVEL_EXPLOIT, CmdPList::CONFIRM_LEVEL_ONE, 
    "Periode de timeout en ecriture pour les sockets - Arret du thread",
    TRUE ) ;

  CmdPList::TheCmdPList->registerVar( "ORBSOCK_SIZE_ALARM_ASYNC",
  &OrbSock::ORBSOCK_SIZE_ALARM_ASYNC, CmdPList::SECURITY_LEVEL_EXPLOIT, CmdPList::CONFIRM_LEVEL_ONE,
  "Taille données stockées dans le buffer provocant une alarme ",
  TRUE ) ;

  CmdPList::TheCmdPList->registerVar( "ORBSOCK_SIZE_MAX_ASYNC",
  &OrbSock::ORBSOCK_SIZE_MAX_ASYNC, CmdPList::SECURITY_LEVEL_EXPLOIT, CmdPList::CONFIRM_LEVEL_ONE,
  "Taille données stockées dans le buffer provocant une alarme ",
  TRUE ) ;


  // section ADMINISTRATION
  // ----------------------
  CmdPList::TheCmdPList->registerVar( "LDAPVerbose",
    &MTServer::LDAPVerbose , CmdPList::SECURITY_LEVEL_ZERO, CmdPList::NO_CONFIRMATION,
    "Niveau de la trace LDAP et SSL" ,
    TRUE ) ;

}
//------------------------------------------------	

void 
PApplication::ServerEnding()
{
	if( MTServer::TheServer )
    {
      OS_TRACE( "ServerEnding Threads Count:" << MTServer::TheServer->getAgentCount());
      std::cout << "Threads Count:" << MTServer::TheServer->getAgentCount() << std::endl ;
      int attente = 0 ;
      while( MTServer::TheServer != NULL 
             && MTServer::TheServer->getAgentCount() > 2 && attente++ < 5 )
        {
          sleep(1) ;
          OS_TRACE( "ServerEnding Threads Count:" << MTServer::TheServer->getAgentCount()  );  
          std::cout << "Threads Count:" << MTServer::TheServer->getAgentCount() << std::endl ;
        }				
			std::cout << "Threads Count:" << MTServer::TheServer->getAgentCount() << std::endl ;
      OS_TRACE( "ServerEnding Ends Threads Count:" << MTServer::TheServer->getAgentCount()  );  
    }
  else
    {
      OS_TRACE( "ServerEnding"  );  
    }
}
//------------------------------------------------
//------------------------------------------------
//------------------------------------------------
//------------------------------------------------
bool 
PApplication::IsEnvVar( const char* pSection, const char* pName )
{
	if( GetSection(pSection) == NULL || GetSection(pSection)->GetVariable(pName) == NULL )
    {
      OS_INFO( ERR_GRAV_0, LOG_INTERNE, 0, "PApplication::GetIniBool " << pSection << "." << pName  <<
               " No define: use default"  );  
      return false;
    }

  return true;
}
//------------------------------------------------
bool 
PApplication::GetIniBool(   const char* pSection, const char* pName, bool    & pVal )
{
	if( IsEnvVar( pSection, pName ) == false ) return false;

	if( sMyEnv->GetBool( pSection, pName ) )
    {
      pVal = true;
    }
	else
		pVal = false;
	
	OS_INFO( ERR_GRAV_0, LOG_INTERNE, 0, "PApplication::GetIniBool " << pSection << "." << pName  << "\t=" << pVal );  
	if( sRegenerateEnv )
		sRegenerateEnv->PutBool(  pSection , pName,  pVal); 
	
	
	return true;
}
	//------------------------------------------------
bool 
PApplication::GetIniInt( const char* pSection, const char* pName, int & pVal )
{
	if( IsEnvVar( pSection, pName ) == false ) return false;

	
	int lTmp = sMyEnv->GetInt( pSection, pName );
	if( lTmp != _LONG_MAX_  )
		pVal = lTmp;
	
	OS_TRACE( "PApplication::GetIniInt " << pSection << "." << pName  << "\t=" << pVal );  
	if( sRegenerateEnv )
		{
			if( lTmp == _LONG_MAX_) sRegenerateEnv->PutInt(  pSection , 
																											 (char*)PStringDyn( UsePStringFormat, "# default %s", pName),  pVal); 
			else sRegenerateEnv->PutInt(  pSection , pName,  pVal); 
		} 
	
	return true;
}
//------------------------------------------------
bool 
PApplication::GetIniLong( const char* pSection, const char* pName, long & pVal )
{
	if( IsEnvVar( pSection, pName ) == false ) return false;

  double lTmp = (double)pVal;
  bool lFlag =  PApplication::GetIniDouble( pSection, pName, lTmp );

  if( lFlag )
    pVal = (long)lTmp;

	return lFlag;
}
//------------------------------------------------
bool 
PApplication::GetIniDouble( const char* pSection, const char* pName, double & pVal )
{
	if( IsEnvVar( pSection, pName ) == false ) return false;

	
	double lTmp = sMyEnv->GetDouble( pSection, pName );
	if( isnan( (float)lTmp ) == 0 )
		{
      pVal =  lTmp; 
      OS_TRACE( "PApplication::GetIniDouble " << pSection << "." << pName  << "\t=" << pVal );  
      if( sRegenerateEnv )    sRegenerateEnv->PutDouble(  pSection , pName,  pVal ); 
		} 
	else 
		{ 
			OS_TRACE( "PApplication::GetIniDouble " <<  pSection << "." << pName  << " is NaN : use default"); 
			if( sRegenerateEnv )    sRegenerateEnv->PutDouble(  pSection, 
																													(char*)PStringDyn( UsePStringFormat, "# default %s", pName),  pVal ); 
		} 		
	return true;
}
//------------------------------------------------
bool 
PApplication::GetIniString( const char* pSection, const char* pName, const char* & pVal )
{
	if( IsEnvVar( pSection, pName ) == false ) return false;

	
	char const* lTmp = sMyEnv->GetStr( pSection, pName ) ;   
	if ( lTmp != NULL ) 
		{                              
			pVal = strdup( lTmp ) ;                           
			OS_INFO( ERR_GRAV_0, LOG_INTERNE, 0,  "PApplication::GetIniString " << pSection << "." << pName  << "\t=" << pVal ); 
			if( sRegenerateEnv )    sRegenerateEnv->PutStr( pSection , pName,  pVal ); 
		}  
	else 
		{ 
			OS_INFO( ERR_GRAV_0, LOG_INTERNE, 0, "PApplication::GetIniString " <<  pSection << "." << pName 
								 << "\t=" << " use default : " << pVal ); 	//------------------------------------------------
			
			
			if( sRegenerateEnv )    sRegenerateEnv->PutStr(  pSection, 
																											 (char*)PStringDyn( UsePStringFormat, "# default %s", pName), (pVal==NULL?"NULL":pVal)); 
		} 
	return true;
}
//------------------------------------------------
bool 
PApplication::GetIniString( const char* pSection, const char* pName, char* & pVal )
{
	return GetIniString( pSection, pName, const_cast<const char* &>(pVal)
 );
}
///------------------------------------------------
EnvSectionPtr
PApplication::GetSection( const char* pSection)
{
	if( sMyEnv == NULL )
		{
			OS_ERROR( ERR_GRAV_0, LOG_INTERNE, 0, "PApplication::GeSection Env is NULL for " << pSection );  
			return NULL;
		}
	
  return sMyEnv->GetSection( pSection );
}
//------------------------------------------------
bool
PApplication::CmdSetMinTraceLevel( ExtendIOStream& os, bool last, CmdPList* pCmdPList, int pSecLevel)
{
  OS_TRACE( "PApplication::CmdSetMinTraceLevel" );

  long int lMinTraceLevel;
  if (os.getLong(lMinTraceLevel) == NULL)
    {
      os << "bad parameter count" << std::endl ;
      os.setError( -1);
      os.endResult() ;
      return true;
    }

  if (SysErrStream::GetTextTraceLevel(lMinTraceLevel) == NULL)
    {
      os << lMinTraceLevel << " : bad trace level" << std::endl ;
      os << "trace level must be from 0 to 5 included" << std::endl ;
      os << "current trace level is : " << SysErrStream::GetTextTraceLevel(ErrStream::MIN_TRACE_LEVEL) << std::endl ;
      os.setError( -1);
      os.endResult() ;
      return true;
    }

  os << "previous trace level is : " << SysErrStream::GetTextTraceLevel(ErrStream::MIN_TRACE_LEVEL) << std::endl ;
  SysErrStream::SetMinTraceLevel(lMinTraceLevel);
  os << "current trace level is : " << SysErrStream::GetTextTraceLevel(ErrStream::MIN_TRACE_LEVEL) << std::endl ;

  os.endResult() ;
  return true;
}
//*************************************************

//  Local Variables:                    --- for emacs
//  mode: c++                           --- for emacs
//  indent-tabs-mode: nil               --- for emacs
//  tab-width: 2                        --- for emacs
//  End:                                --- for emacs
//  vim: set ts=2 sw=4 filetype=cpp et: --- for vim
