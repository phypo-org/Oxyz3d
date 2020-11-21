#ifndef H__Dialogs__H
#define H__Dialogs__H


#include "Shape/PrimitivFactory.h"
#include "Shape/Kamera.h"
#include "Canvas3d.h"

#include <FL/Fl_Double_Window.H>
 
#include <memory>
 

void CallDialogPerspectiv( M3d::Canvas3d* pCanvas, PP3d::Kamera &pKamera );
void CallDialogPrimitiv( PP3d::PrimitivFactory::Type );

void CallDialogKeepFloatInit( M3d::Canvas3d* pCanvas );
bool CallDialogKeepFloat( double & ioVal);
bool CallDialogInt( double & ioVal);



enum class TypeRevol{ RevolX, RevolY, RevolZ, RevolAxis };

void CallDialogRevol( TypeRevol pTypeRevol );

extern void CallDialogRevol( bool& pFlagAlreadyExist, M3d::Canvas3d* pCanvas, TypeRevol pType );
extern bool CallDialogInputInt( const char* iLabel, int & ioVal);
extern bool CallDialogInputDouble(  const char* iLabel, double & ioVal);

extern void CallDialogSubDiv( bool& pFlagAlreadyExist, M3d::Canvas3d* iCanvas);

extern void CallDialogPref();
extern void CallDialogFile( const std::string & iTitle);


Fl_Double_Window* CallConsolePython();
Fl_Double_Window* CallConsoleSystem();

#ifdef USING_LUA
Fl_Double_Window* CallConsoleLua();
void AppendConsoleLua( const char* iTxt );
#endif


#endif

