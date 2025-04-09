#include "Dialogs.h"

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Float_Input.H>
#include <FL/Fl_Hor_Value_Slider.H>
#include <FL/Fl_Hor_Nice_Slider.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Light_Button.H>

#include "Utils/PPSingletonCrtp.h"


#define DiagFile M3d::DialogFile::Instance()

namespace M3d {

  //************************
  class DialogFile  : public virtual PPSingletonCrtp<DialogFile>{

    Fl_Double_Window* cMyWindow = nullptr;
    DialogFile() {;}
    
    friend class PPSingletonCrtp;
  public :
    
    bool isAlreadyRunning() { return DiagFile.cMyWindow != nullptr; }

    //----------------------------------------
    bool init( const std::string iTitle )
    {  
      cMyWindow = new Fl_Double_Window( 500 , 400, iTitle.c_str());
      cMyWindow->callback((Fl_Callback*)CancelCB, nullptr);
      
      Fl_File_Chooser* lFc = new Fl_File_Chooser(".", "*", Fl_File_Chooser::SINGLE, iTitle.c_str());
           
      cMyWindow->end();		
      cMyWindow->show();
      //0, nullptr);
      lFc->show();

      while (lFc->visible()) {
	Fl::wait();
      }
      return true;
    }        
    //----------------------------------------
    static void CancelCB( Fl_Widget*, void* pUserData )
    {
      Fl::delete_widget( DiagFile.cMyWindow);
      DiagFile.cMyWindow = nullptr;
    }
    //----------------------------------------
  };
  //************************
} // namespace
//---------------------------------
    extern void CallDialogFile( const std::string & iTitle)
{
  std::cout << "CallDialogFile" <<std::endl;
  if( DiagFile.isAlreadyRunning() == false )
    {
      DiagFile.init( iTitle );
    }
}
