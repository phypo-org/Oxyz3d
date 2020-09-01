#ifndef H__MODIF__H
#define H__MODIF__H


#include "Shape/Entity.h"
#include "Shape/SortVisitor.h"
#include "Shape/DataBase.h"
#include "Shape/SubDiv.h"
#include "Shape/EntityVisitor.h"
#include "Shape/Maker.h"

#include <map>

namespace PP3d{

  static double sGoldNumber = 1.61803398875;
  static double sInvGoldNumber = 1.0/1.61803398875;

  enum class SubDivFacetType{ ANGLE_SIMPLE, ANGLE_FACET_MARGE, CENTRAL_POINT, CENTRAL_FACET, CENTRAL_FACET_MARGE };

  enum class SubDivSelectType{ SELECT_NONE=0, SELECT_MARGE=1, SELECT_CENTRAL=2, SELECT_ALL=3, };

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
    static PointPtr FindPointByEpsilon( const std::vector<PointPtr> & iPoints, const Point3d & iVal, long double iEpsilon = PP3d::Maker::kEpsilon )
    {
      for( PointPtr lPt : iPoints ) {
	if( lPt->get().sameEpsi( iEpsilon, iVal) )
	  return lPt;
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
    
    static bool SubCatmullClark( DataBase * iBase, std::set<FacetPtr>&  iFacets, std::set<PointPtr> &  iOldPoint, std::vector<EntityPtr> & oNewFacet, bool iChgOldPt=true );

    static bool SubDivMiddle( DataBase * iBase,
			      std::set<FacetPtr>&  iFacets,
			      std::set<PointPtr> &  iOldPoint,
			      std::vector<EntityPtr> & oNewFacet,
			      SubDivFacetType iSubDivType,
			     SubDivSelectType iSubDivSelect = SubDivSelectType::SELECT_ALL,
			     double iMargeFactor=sInvGoldNumber );
    static bool SubDivAngle( DataBase * iBase,
			     std::set<FacetPtr>&  iFacets,
			     std::set<PointPtr> &  iOldPoint,
			     std::vector<EntityPtr> & oNewFacet,
			     SubDivFacetType iSubDivType,
			     SubDivSelectType iSubDivSelect = SubDivSelectType::SELECT_ALL,
			     double iMargeFactor=sInvGoldNumber );

    static bool DeletePoints( DataBase * iBase, std::vector<PointPtr>& iPoints, bool iCreateFacet);
    static bool DeletePoint( DataBase * iBase, PointPtr iPoint, bool iCreateFacet );
    static bool DeletePointFromFacet( DataBase * iBase, PointPtr iPoint , FacetPtr ioFacet, std::vector<PointPtr> &oPoints  );  };
  //**************************************************
  // Calcule les coordonnees du milieu d"une facette
  class VisitorComputeMiddleFacet : public EntityVisitor	{
    
    Point3d  cMiddle;
    size_t   cNbPt=0;
    
  public:  
    void execPoint( Point* pPoint ) override
    {
      cMiddle += pPoint->get();
      cNbPt++;
    }
    Point3d getMiddle()
    {
      return cMiddle / cNbPt;
    }
  };
  //**************************************************
  // Retrouve le point par sa position, ou en cree un nouveau
  
  inline PointPtr getPoint( DataBase * iBase, std::vector<PointPtr> &ioPts,  Point3d & iPt )
  {
    PointPtr lSearchPt = Modif::FindPointByEpsilon( ioPts, iPt );
    
    if( lSearchPt == nullptr )
      {
	lSearchPt = iBase->getNewPoint(iPt);
	ioPts.push_back( lSearchPt );        // pour un recherche ulterieure
      }
    return lSearchPt;
  }

  
#define DBG_MODIF( A )    if( Modif::sDbgModif > 0 ) std::cout << "DbgModif0> " << A << std::endl;
#define DBG_MODIF1( A )   if( Modif::sDbgModif > 1 ) std::cout << "DbgModif1> " << A << std::endl;
#define DBG_MODIF2( A )   if( Modif::sDbgModif > 2 ) std::cout << "DbgModif0> " << A << std::endl;
#define DBG_MODIF_NL( A ) if( Modif::sDbgModif > 0 ) std::cout << "DbgModif> " << A ;

  
} // end namespace
#endif
