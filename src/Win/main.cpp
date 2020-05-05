
#include <FL/Fl.H>

#include "Application.h"
#include "Shape/SavRead.h"
#include "Shape/UndoHistory.h"

#include "Dialogs.h"

using namespace M3d;

//---------------------------------------
int existParam( std::vector<std::string> & iParams, const char* lVar)
{
  for( size_t i = 0; i< iParams.size(); i++ )
    {
      if( iParams[i] == lVar )
	{
	  //	  VERBOSELN( "existParam " << lVar << " " << i );
	  
	  return (int)i;
	}
    }
  //  VERBOSELN( "existParam " << lVar << " not found" );
  return -1;
}
//---------------------------------------
std::string getParamValue( std::vector<std::string> & iParams, const char* lVar)
{
  int i = existParam( iParams, lVar );
  
  if( i == -1 || i >= (int)iParams.size()-1 )
    {
      return "";
    }
  //  VERBOSELN(" getParamValue " << iParams[i+1] );
  return iParams[i+1];    
}
//-------------------------------------------

int main(int argc, char **argv) 
{
  std::vector<std::string> lVectParam;    

  // on recopie les parametres
  for (int i = 0; i < argc; i++) 
    {
      lVectParam.push_back( argv[i]);
    }
  
  
  if( TheAppli.getConfig().readFile( argc, argv) == false )
    {
      WARN_LOG( "Config file not found : " <<  TheAppli.getConfig().getFilename() );
    }
  
  
  Fl::use_high_res_GL(1);

  M3d::Application::Instance().createNewWin3d( 1000, 800 );
  M3d::Application::Instance().redrawObjectTree();	

  std::string lVal;
  if( (lVal = getParamValue( lVectParam, "-load" )).size() != 0 )
    {
      if( PP3d::MyRead::Read(lVal.c_str(),
			     *Application::Instance().getDatabase()))
	{
	  Application::Instance().redrawAll();
	}
    }
  
  if( existParam( lVectParam, "-lua" ) != -1 )
    {
      CallConsoleLua();     
    }
  
  PP3d::UndoHistory::Instance().sav( *Application::Instance().getDatabase() );

 
  return Fl::run();
}



//
// End of "$Id: gl_overlay.cxx 10498 2014-12-20 07:19:23Z manolo $".
//
