#ifndef H__MODIF__H
#define H__MODIF__H


#include "Shape/Entity.h"
#include "Shape/SortVisitor.h"
#include "Shape/DataBase.h"
#include "Shape/SubDiv.h"
#include "Shape/EntityVisitor.h"

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
 
    static int sDbgModif;
    
    
    static PointPtr FindPointByExactValues( const std::vector<PointPtr> & iPoints, const Point3d & iVal )
    {
      for( PointPtr lPt : iPoints ) {
	if( lPt->get() == iVal ) return lPt;
      }
      return nullptr; 
    }
    
    static int  GetFacetFirstLineIndex( FacetPtr iFacet, PointPtr lPt);
    static bool RemplaceLineIntoFacet( Facet & iFact, LinePtr lLine,
				       std::vector<LinePtr> lLines );

    static bool CutLines( std::vector<LinePtr>&  iVect, int iNbCut, DataBase * iBase );
    static bool CutLine(  LinePtr iLine, int iNbCut, DataBase * iBase,
			  LinePtr * ioReverseLine = nullptr );

    static bool Connect2Point( PointPtr A, PointPtr B, DataBase * iBase );
    static bool ConnectPoints( std::vector<PointPtr>&  iVect, DataBase * iBase, int iDepth=0 );


    static bool SubdivideFacet(  std::vector<FacetPtr>&  iVect, DataBase * iBase, SubDiv::SubParam * iSubDiv = nullptr );

    static void PrepareChangePointToNeighbourLineAverage(  std::vector<PointPtr> & iVect, std::vector<Point3d> & iVectNewPos,  bool iMeToo =false);
    static void PrepareChangePointToNeighbourFacetAverage(  std::vector<PointPtr> & iVect, std::vector<Point3d> & iVectNewPos,  bool iMeToo =false);
    static void FinalizeChangePointToNeighbourAverage(  std::vector<PointPtr> & iVect, std::vector<Point3d> & iVectNewPos );
    
    static bool SubCatmullClark(  std::set<FacetPtr>&  iOldFacets, DataBase * iBase );
  };
  //**************************************************

  
#define DBG_MODIF( A )    if( Modif::sDbgModif > 0 ) std::cout << "DbgModif0> " << A << std::endl;
#define DBG_MODIF1( A )   if( Modif::sDbgModif > 1 ) std::cout << "DbgModif1> " << A << std::endl;
#define DBG_MODIF2( A )   if( Modif::sDbgModif > 2 ) std::cout << "DbgModif0> " << A << std::endl;
#define DBG_MODIF_NL( A ) if( Modif::sDbgModif > 0 ) std::cout << "DbgModif> " << A ;

  
} // end namespace
#endif
