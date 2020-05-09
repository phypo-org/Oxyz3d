#ifndef H__MODIF__H
#define H__MODIF__H


#include "Shape/Entity.h"
#include "Shape/SortVisitor.h"
#include "Shape/DataBase.h"


#include <map>

namespace PP3d{
  //**************************************************
  
  struct UniquePoints{
    
    std::vector<PointPtr> cVectPt; // les points dans le bon ordre
    std::set<PointPtr>    cUnikPt; // pour eviter les doublons
    
    //------------------------
    void add( PointPtr iPt )
    {
      if( cUnikPt.find( iPt ) == cUnikPt.end() )
	{
	  cUnikPt.emplace( iPt );
	  cVectPt.push_back( iPt );
	}
    }
  };
  //----------------------------
  // to store points sorted by facet
  typedef std::map< FacetPtr, UniquePoints> FacetsPoints;
  
   //**************************************************

  struct Modif{
    static int  GetFacetFirstLineIndex( FacetPtr iFacet, PointPtr lPt);
    static bool RemplaceLineIntoFacet( Facet & iFact, LinePtr lLine,
				       std::vector<LinePtr> lLines );

    static bool CutLines( std::vector<LinePtr>&  iVect, int iNbCut, DataBase * iBase );
    static bool CutLine(  LinePtr iLine, int iNbCut, DataBase * iBase,
			  LinePtr * ioReverseLine = nullptr );

    static bool ConnectPoints( std::vector<PointPtr>&  iVect, DataBase * iBase );
    static bool Connect2Point( PointPtr A, PointPtr B, DataBase * iBase );

  };
  //**************************************************
  
} // end namespace
#endif
