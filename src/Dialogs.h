#ifndef H__Dialogs__H
#define H__Dialogs__H


#include "Shape/PrimitivFactory.h"
#include "Shape/Kamera.h"
#include "Canvas3d.h"

#include <FL/Fl_Double_Window.H>

#include <memory>
 

void CallDialogPerspectiv( M3d::Canvas3d* pCanvas, PP3d::Kamera &pKamera );
void CallDialogPrimitiv( bool& pFlagAlreadyExist, M3d::Canvas3d* pCanvas, PP3d::PrimitivFactory::Type );



enum class TypeRevol{ RevolX, RevolY, RevolZ };

void CallDialogRevol( bool &cFlagAlreadyExist, M3d::Canvas3d* pCanvas, TypeRevol pTypeRevol);




Fl_Double_Window* CallConsolePython();
Fl_Double_Window* CallConsoleSystem();

#endif

