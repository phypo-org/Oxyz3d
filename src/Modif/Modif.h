#ifndef H__MODIF__H
#define H__MODIF__H


#include "Shape/Entity.h"
#include "Shape/SortVisitor.h"
#include "Shape/DataBase.h"



namespace PP3d{

  struct Modif{
    static bool CutLines( std::vector<LinePtr>&  iVect, int iNbCut, DataBase * iBase );
    static bool CutLine(  LinePtr iLine, int iNbCut, DataBase * iBase );
    static bool RemplaceLineIntoFacet( Facet & iFact, LinePtr lLine,  std::vector<LinePtr> lLines );
  };
} // end namespace
#endif
