
#include <FL/Fl.H>

#include "Application.h"

//-------------------------------------------

int main(int argc, char *argv[] ) 
{
  Fl::use_high_res_GL(1);

  M3d::Application::Instance().init( argc, argv );
  M3d::Application::Instance().createNewWin3d( 1000, 800 );
  M3d::Application::Instance().redrawObjectTree();	
  M3d::Application::Instance().redrawWinHisto();
  Fl::add_check( M3d::Application::FlCheckCallback );

  
  return Fl::run();
}



//
// End of "$Id: gl_overlay.cxx 10498 2014-12-20 07:19:23Z manolo $".
//
