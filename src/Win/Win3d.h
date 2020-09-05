#ifndef  H__Win3D__H
#define  H__Win3D__H



#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Sys_Menu_Bar.H>
#include <FL/Fl_Menu_Button.H>


#include <FL/Fl_Native_File_Chooser.H>


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

    MyFloatInput*           cCurrentInput1;
    
  public:
    MyIntInput*             cCurrentUndo;
    MyIntInput*             cCurrentUndoMax;
    MyButton*               cButUndo;
    MyButton*               cButRedo;
    MyButton*               cButStore;
  private:
    Fl_Output*              cInfoOutput;

    Fl_Native_File_Chooser  *cFileChooser = nullptr;
    
  public:
    std::unique_ptr<Canvas3d> cuCanvas3d;
    

  public:
    Win3d( const char*pName, int pW, int pH, PP3d::DataBase & pDatabase);
    void canvasRedraw() { cuCanvas3d->redraw(); }
    void setCursorPosition( PP3d::Point3d& pPos);
    void setCurrentVal( const char* iLabel, double iVal);
    void setUndoRedoState();
    double getCurrentVal();
    PP3d::Kamera& getKamera() { return cuCanvas3d->cKamera;}
    int getId()  { return cWinId; }
  private:
    static void MyMenuCallback(Fl_Widget* w, void* pUserData );
    static void QuitCallback(Fl_Widget*, void*);	 

    friend class Canvas3d;
  };
  //****************************************
}


#define INFO( A )	{ ostringstream lInfoStream ; lInfoStream << A; cInfoOutput->value(lInfoStream.str().c_str() );}
#define SINFO( WIN3D, A )	{ ostringstream lInfoStream ; lInfoStream << A; WIN3D->cInfoOutput->value(lInfoStream.str().c_str() );}

#define CINFO( A )	{ ostringstream lInfoStream ; lInfoStream << A; cMyWin3d.cInfoOutput->value(lInfoStream.str().c_str() );}


#endif
