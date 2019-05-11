#ifndef  H__Win3D__H
#define  H__Win3D__H



#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Sys_Menu_Bar.H>
#include <FL/Fl_Menu_Button.H>


#include <string>
#include <memory>

#include "Canvas3d.h"
#include "MyFloatInput.h"

#include "Shape/DataBase.h"

class Fl_Output;


namespace M3d {

  //****************************************
  class Win3d : public Fl_Window 
  {
    int                    cWinId=0;
    PP3d::DataBase &       cDatabase;
    Fl_Menu_Bar            cMenubar;
		
    MyFloatInput*           cXinput;
    MyFloatInput*           cYinput;
    MyFloatInput*           cZinput;


    Fl_Output*              cInfoOutput;

  public:
    std::unique_ptr<Canvas3d> cuCanvas3d;
    

  public:
    Win3d( const char*pName, int pW, int pH, PP3d::DataBase & pDatabase);
    void canvasRedraw() { cuCanvas3d->redraw(); }
    void setCursorPosition( PP3d::Point3d& pPos);
    PP3d::Kamera& getKamera() { return cuCanvas3d->cKamera;}
    int getId()  { return cWinId; }
  private:
    static void MyMenuCallback(Fl_Widget* w, void* pUserData );
    static void QuitCallback(Fl_Widget*, void*);	 

    friend class Canvas3d;
  };
  //****************************************
};


#endif
