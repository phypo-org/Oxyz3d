#ifndef  H__Win3D__H
#define  H__Win3D__H


#include "Gui/Win3dBase.h"

#include "Canvas3d.h"

#include "Shape/DataBase.h"


class Fl_Output;


namespace M3d {

  //****************************************
  class Win3d : public Win3dBase
  {
   PP3d::DataBase &       cDatabase;


  public:
    MyIntInput*             cCurrentUndo;
    MyIntInput*             cCurrentUndoMax;
    MyButton*               cButUndo;
    MyButton*               cButRedo;
    MyButton*               cButStore;
    
  public:
    std::unique_ptr<Canvas3d> cuCanvas3d;
    
    
    virtual Canvas3dBase & getCanvas() { return *(cuCanvas3d.get()); }

  public:
    Win3d( const char*pName, int pW, int pH, PP3d::DataBase & pDatabase);

    void setUndoRedoState();
    void recenterOnSelection(PP3d::Kamera & i);

    
    //   Canvas3dBase * createCanvas( int pX, int pY, int pW, int pH, const char* iName) override
    //  {
    //    return new Canvas3d( *this, pX, pY, pW, pH, iName);
    //  }
  
  };
  //****************************************

}

#endif
