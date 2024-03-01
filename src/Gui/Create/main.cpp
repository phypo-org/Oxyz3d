
#include <FL/Fl.H>

#include "Creation.h"

#include "Shape/SavRead.h"
#include "Shape/UndoHistory.h"

#include "Gui/Preference.h"

#include "Dialogs.h"
#include "Gui/AgentAutoSav.h"


#include <iostream>

using namespace M3d;
using namespace std;


//-------------------------------------------

int main(int argc, char **argv) 
{
  MyPref.initDefaultPaths();
  
  PPu::PPErrLog::Instance().init( & std::cout, & std::cerr, nullptr );
  
  PPu::PPArgs lArgs( argc, argv );
  std::string lVal;
 
  if( lArgs.get(  "-HomePath", lVal ) )
    {      
      MyPref.initDefaultPaths( lVal );
      cout << "HomePath:" << lVal << endl;
    }
  
  
  //---------------- Preference ------------------
  std::string lName("Oxyz3d.ini"); 
  TheCreat.getConfig().setFilename( PPu::PPFile::JoinPathNames( MyPref.cConfigPath, lName));

  std::cout << "%%%%% PREFRENCE:" << PPu::PPFile::JoinPathNames( MyPref.cConfigPath, lName) << std::endl;
  
  if( TheCreat.getConfig().readFile() == false )
    {
      WARN_LOG( "Config file not found : " <<  TheCreat.getConfig().getFilename() );  
    }
  else
    INFO_LOG( "Read config file : " <<  TheCreat.getConfig().getFilename() );  

  std::cout << "Read config file : " <<  TheCreat.getConfig().getFilename()  << std::endl;
  
  MyPref.initFromIni( TheCreat.getConfig() );  
  MyPref.initFromArg( lArgs );
  
  //---------------- Preference ------------------
  MyPref.initFromArgs(lArgs );

  

  Fl::use_high_res_GL(1);
 
 
  M3d::Creation::Instance().createNewWin3d( 1000, 800 );
  //  M3d::Creation::Instance().redrawObjectTree();	


  if( lArgs.get(  "-load", lVal ) )
    {
      if( PP3d::MyRead::Read(lVal.c_str(),
			     *Creation::Instance().getDatabase(), &TheSelect ))
	{
	  Creation::Instance().redrawAll(PP3d::Compute::FacetAll
					    );
	}
    }
  
#ifdef USING_LUA
  if( lArgs.exist( "-lua" ) != -1 )
    {
      CallConsoleLua();     
    }
#endif  
  PP3d::UndoHistory::Instance().sav( *Creation::Instance().getDatabase(), &TheSelect );


  
  AgentAutoSav lAutoSav; 
  lAutoSav.run();
  
  return Fl::run();
}



//
// End of "$Id: gl_overlay.cxx 10498 2014-12-20 07:19:23Z manolo $".
//
