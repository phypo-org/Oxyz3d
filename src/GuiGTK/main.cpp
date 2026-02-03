#include <gtkmm/application.h> // GTK



#include "Creation.h"

#include "Shape/SavRead.h"
#include "Shape/UndoHistory.h"

#include "GuiFLTK/Preference.h"

#include "Dialogs.h"
#include "GuiFLTK/AgentAutoSav.h"


#include <iostream> 

using namespace M3d;
using namespace std;



//-------------------------------------------

int main(int argc, char **argv) 
{
  auto lAppli = Gtk::Application::create(argc, argv, "org.phypo.Oxyz3d");

 

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
	  Creation::Instance().redrawAll(PP3d::Compute::FacetAll );
          Creation::Instance().changeCurrentPath(lVal);
	}
    }
  
  
#ifdef USING_LUA
  if( lArgs.exist( "-lua" ) != -1 )
    {
      CallConsoleLua();     
    }
#endif
  
#ifdef USING_PYTHON
  if( lArgs.exist( "-python" ) != -1 )
    {
      CallConsolePython();     
    }
#endif  
  PP3d::UndoHistory::Instance().sav( *Creation::Instance().getDatabase(), &TheSelect );


  
  AgentAutoSav lAutoSav; 
  lAutoSav.run();


 
  
  
  if( lArgs.get(  "-img", lVal ) )
    {
      std::cout << "CALL TO : Creation::Instance().loadModelImage" <<std::endl;
      Creation::Instance().loadModelImage( lVal );	
    }

  
  return lAppli->make_window_and_run<Win3dBase>(argc, argv);
}
