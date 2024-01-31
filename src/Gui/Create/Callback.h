#ifndef H__CALLBACK__H
#define H__CALLBACK__H


#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Hor_Slider.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/math.h>

#include <FL/Fl_Sys_Menu_Bar.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/Fl_Choice.H>

#include <FL/Fl_File_Icon.H>
#include <FL/Fl_Button.H>

#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/fl_ask.H>


#include "Shape/DataBase.h"
#include "Shape/ObjectFacet.h"
#include "Shape/ObjectLine.h"
#include "Shape/UndoHistory.h"
#include "Shape/SortVisitor.h"
#include "Shape/Selection.h"
#include "Shape/SavRead.h"
#include "Shape/SavReadStl.h"
#include "Shape/ExportObj.h"

#include "Utils/PPFile.h"
#include "Shape/PP3dUtils.h"

#include "Win3d.h"
#include "Canvas3d.h"
#include "Dialogs.h"

#include "Creation.h"

#include "Gui/MyFlWidget.h"
#include "Gui/MyImage.h"
#include "Gui/Preference.h"



#include <memory>

#include <fstream>
#include <stdlib.h>

namespace M3d {

extern bool SaveBase( PP3d::DataBase * ioDatabase, const std::string & iName, void* iUseSelect );
extern bool OpenBase( PP3d::DataBase * ioDatabase,const std::string & iName, bool iKeepId );
extern bool ExportD3dObj( PP3d::DataBase * ioDatabase, const std::string & iName, void* iUseSelect );
extern bool ImportD3dObj( PP3d::DataBase * ioDatabase, const std::string & iName );
extern void SaveBaseCB( Fl_File_Chooser *cFc, void            *cData );
extern void ExportD3dObjCB( Fl_File_Chooser *cFc, void            *cData);
extern void ImportD3dObjCB( Fl_File_Chooser *cFc, void            *cData);
extern void UndoCB(Fl_Widget*w, void*pData);
extern void RedoCB(Fl_Widget*w, void*pData);
extern void PushHistoryCB(Fl_Widget*w, void*pData);
extern void CB_ViewTransfrom(Fl_Widget*w, void*pData);
extern void ExportStlCB( Fl_File_Chooser * cFc, void * iUseSelect);
extern void BasculeSelModeGeoCB(Fl_Widget*w, void*pData);
extern void BasculeSelModeCB(Fl_Widget*w, void*pData);
extern void BasculeViewModeCB(Fl_Widget*w, void*pData);
extern void BasculeBoolCB(Fl_Widget*w, void*pData);
extern void BasculeGridCB(Fl_Widget*w, void*pData);
extern void BasculePerspective(Fl_Widget*w, void*pData);
}

#endif
