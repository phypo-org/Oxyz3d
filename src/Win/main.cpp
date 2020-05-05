
#include <FL/Fl.H>

#include "Application.h"

#include "Shape/SavRead.h"
#include "Shape/UndoHistory.h"

#include "Win/Preference.h"

#include "Dialogs.h"

using namespace M3d;


//-------------------------------------------

int main(int argc, char **argv) 
{
  std::vector<std::string> lVectParam;    

 
  if( TheAppli.getConfig().readFile( argc, argv) == false )
    {
      WARN_LOG( "Config file not found : " <<  TheAppli.getConfig().getFilename() );
    }
  
  MyPref.initFromIni( TheAppli.getConfig() );
  
  
  PPu::PPArgs lArgs( argc, argv );
  MyPref.initFromArg( lArgs );
  
  Fl::use_high_res_GL(1);
 
 
  M3d::Application::Instance().createNewWin3d( 1000, 800 );
  M3d::Application::Instance().redrawObjectTree();	

  std::string lVal;
  if( (lVal = lArgs.getValue(  "-load" )).size() != 0 )
    {
      if( PP3d::MyRead::Read(lVal.c_str(),
			     *Application::Instance().getDatabase()))
	{
	  Application::Instance().redrawAll();
	}
    }
  
  if( lArgs.exist( "-lua" ) != -1 )
    {
      CallConsoleLua();     
    }
  
  PP3d::UndoHistory::Instance().sav( *Application::Instance().getDatabase() );

 
  return Fl::run();
}



//
// End of "$Id: gl_overlay.cxx 10498 2014-12-20 07:19:23Z manolo $".
//
