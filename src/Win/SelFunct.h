#ifndef H__SEL_FUNCT__H
#define H__SEL_FUNCT__H

#include "Shape/Selection.h"
#include "Shape/DataBase.h"

//**************************************************
class SelFunct{
public:
  static void SelectMore(  PP3d::Selection & iSel, PP3d::DataBase & iBase );
  static void SelectLess(  PP3d::Selection & iSel, PP3d::DataBase & iBase );  

  static void SelectEdgeLoop( PP3d::Selection & iSel, PP3d::DataBase & iBase, double iEpsilon );
};

//**************************************************
 

#endif
